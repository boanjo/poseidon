#include <Arduino.h>
#include <stdlib.h>
#include <ctype.h>

#define LEVEL_SENSOR_KUS   0
#define LEVEL_SENSOR_ETAPE 1

long levelSensorType = LEVEL_SENSOR_KUS;
#define WATER_LEVEL_PIN A0

// For a KUS reed swith type of sensor it's optimal to find a wanted level between two reistances (the sensor has a granularity
// of 20-22mm). If you select it carfully you can still accomplish a threshold that is within +-5mm or so. In the example below
// i have a 300mm KUS sensor which then has 13 steps(0-192 fixed resistances. so the value i want to stay at is just when it
// toggles to the second highest level 91.5% and the 3:d highest level is at 86.5%. hence i have selected min and max level as
// two values inbetween. The reed switched has a built in hysteresis. So when a reed switch gets activated it will continue to
// hold slightly longer until the next switch gets activated.
// Operations will then be: we fill until the 91.5% is reached and then stop. When the level is slowwly decreasing (remove water
// or waporize) iit will continue to hold the level at 91.5% for another 5-10mm and then toggle to 86.5%. We will then be below the
// min and once again we will fill up again. I.e. we can achieve 5-10 mm operations accuracy even if it is only 20-22mm for the
// actual sensor.
long waterLevelLow = 88;
long waterLevelHigh = 89;

#define FLOW_SENSOR_PIN 2
#define SPRINKLER_1_CONTROL_PIN 6
#define WATER_CONTROL_PIN 5

#define STATE_WATER_OFF 0
#define STATE_WATER_ON  1

#define NO_OF_LEVEL_AVERAGE_ITEMS 8

#define MODE_WATER_MANUAL 0
#define MODE_WATER_AUTOMATIC 1


float levelArray[NO_OF_LEVEL_AVERAGE_ITEMS];
long levelAvgCnt = 0;
byte waterMode = MODE_WATER_AUTOMATIC;
byte waterState = STATE_WATER_OFF;
byte sprinkler1State = STATE_WATER_OFF;
unsigned long previousMillis = 0;
unsigned long interval = 3000;
String inputString = ""; // a string to hold incoming data

volatile uint32_t flowPulses = 0;
volatile uint8_t lastFlowPinState = LOW;
long seqNo = 0;
// Interrupt is called once a millisecond, looks for any pulses from the sensor!
SIGNAL(TIMER0_COMPA_vect)
{

  uint8_t x = digitalRead(FLOW_SENSOR_PIN);

  if ((x == HIGH) && (lastFlowPinState == LOW))
  {
    //low to high transition!
    flowPulses++;
  }
  lastFlowPinState = x;
}

void useInterrupt(boolean v)
{
  if (v)
  {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
  }
  else
  {
    // do not call the interrupt function COMPA anymore
    TIMSK0 &= ~_BV(OCIE0A);
  }
}

void setup(void)
{
  Serial.begin(115200);
  digitalWrite(WATER_CONTROL_PIN, LOW);
  pinMode(WATER_CONTROL_PIN, OUTPUT);

  digitalWrite(SPRINKLER_1_CONTROL_PIN, LOW);
  pinMode(SPRINKLER_1_CONTROL_PIN, OUTPUT);

  pinMode(WATER_LEVEL_PIN, INPUT);

  pinMode(FLOW_SENSOR_PIN, INPUT);
  digitalWrite(FLOW_SENSOR_PIN, HIGH);
  lastFlowPinState = digitalRead(FLOW_SENSOR_PIN);

  useInterrupt(true);

  Serial.println("Starting!");
}

int getSerialValue(String str)
{
  String subStr = str.substring(str.indexOf(",") + 1, str.length());
  return subStr.toInt();
}

void printState(bool state) {
  if (state)
    Serial.print("on");
  else
    Serial.print("off");
}

void loop(void)
{
  const byte strLen = 8;
  char litersStr[strLen];
  char levelStr[strLen];
  float waterLevel;
  float waterLevelAvg = 0;
  char inChar;
  boolean ack_requested = false;
  
  // USB HW serial
  if (Serial.available() > 0)
  {
    inChar = Serial.read();

    if (inChar == '{')
    {
      inputString = "";
    }
    else if (inChar == '}')
    {
      if (inputString.length() > 0)
      {

        if (inputString.indexOf("water_level_low") != -1)
        {
          waterLevelLow = getSerialValue(inputString);
        }
        else if (inputString.indexOf("water_level_high") != -1)
        {
          waterLevelHigh = getSerialValue(inputString);
        }
        else if (inputString.indexOf("ack_request") != -1)
        {
          seqNo = getSerialValue(inputString);
	  ack_requested = true;
        }
        else if (inputString.indexOf("water") != -1)
        {
	  String subCmd = inputString.substring(inputString.indexOf(",")+1, inputString.length());
          if (subCmd.indexOf("off") != -1)
          {
            digitalWrite(WATER_CONTROL_PIN, LOW);
            waterState = STATE_WATER_OFF;
          }
          else if (subCmd.indexOf("on") != -1)
          {
            digitalWrite(WATER_CONTROL_PIN, HIGH);
            waterState = STATE_WATER_ON;
          }
        }
        else if (inputString.indexOf("auto_level") != -1)
        {
	  String subCmd = inputString.substring(inputString.indexOf(",")+1, inputString.length());

	  if(subCmd.indexOf("off") != -1)
          {
            waterMode = MODE_WATER_MANUAL;
          }
          else if (subCmd.indexOf("on") != -1)
          {
            waterMode = MODE_WATER_AUTOMATIC;
          }
        }
        else if (inputString.indexOf("sprinkler1") != -1)
        {
	  String subCmd = inputString.substring(inputString.indexOf(",")+1, inputString.length());
	  
	  if(subCmd.indexOf("off") != -1)
          {
            digitalWrite(SPRINKLER_1_CONTROL_PIN, LOW);
            sprinkler1State = STATE_WATER_OFF;
          }
	  else if(subCmd.indexOf("on") != -1)
          {
            digitalWrite(SPRINKLER_1_CONTROL_PIN, HIGH);
            sprinkler1State = STATE_WATER_ON;
          }
        }

        inputString = "";
      }
    }
    else
    {
      // add it to the inputString:
      inputString += inChar;
    }
  }

  unsigned long currentMillis = millis();

  // Handle wrap around
  if (currentMillis < previousMillis)
  {
    previousMillis = currentMillis;
  }
  if (((currentMillis - previousMillis) > interval) || ack_requested)
  {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // Calculate amount of added water in liters
    float liters = flowPulses;
    liters /= 8.1;
    liters -= 6;
    liters /= 60.0;

    if (liters < 0)
      liters = 0;
    dtostrf(liters, 7, 1, litersStr);

    // Read the KUS or etape voltage divider
    int analogLevelValue = analogRead(WATER_LEVEL_PIN);

    if(levelSensorType == LEVEL_SENSOR_KUS) {
      // KUS European level switch goes between 0-192 (or actually 0-180 according to datasheet) ohm (192 100% full)
      // Voltage divider tells us R2/(R1+R2) where R2 is the KUS variable resistance
      // and R1 is the fixed 100ohm resistance
      // So the max analog read would be 192/(192+100)*1023=673
      // So to get the percentage fill level 0-100% we can divide the analog value
      // with 6.73
      
      waterLevel = analogLevelValue / 6.73;
    } else {

      // milonetech etape (use RSense i.e. middle two pins as R2 and outer two pins (RRef) as R1
      // For an empty level (i.e. no water contact on Rsense) the two resistances should be roughly the same
      // 2.2k/(2.2k+2.2k)*1023=512 
      waterLevel = 100.0 - (analogLevelValue / 5.12);

      // As an example a completly submerged etape will have a resistance of 400ohm on RSense. This means that
      // the fill level will never reach 100% (analogLevelValue will show ~93 i.e. waterLevel = 82% (100-(93/5.12)
      // Select the level min max (waterLevelLow, waterLevelHigh) thresholds accordingly
      
      if(waterLevel < 0.0) {
	waterLevel = 0.0;
      }
    }
    
    levelArray[levelAvgCnt % NO_OF_LEVEL_AVERAGE_ITEMS] = waterLevel;
    levelAvgCnt++;

    if (levelAvgCnt >= NO_OF_LEVEL_AVERAGE_ITEMS)
    {
      for (int i = 0; i < NO_OF_LEVEL_AVERAGE_ITEMS; i++)
      {
        waterLevelAvg += levelArray[i];
      }

      waterLevelAvg = waterLevelAvg / NO_OF_LEVEL_AVERAGE_ITEMS;

      // Something is fishy, better turn off the auto mode too
      if (waterLevelAvg > 95)
      {
	if(waterMode == MODE_WATER_AUTOMATIC)
	{
	  waterMode = MODE_WATER_MANUAL;

	  digitalWrite(WATER_CONTROL_PIN, LOW);
	  waterState = STATE_WATER_OFF;
	}
      }

      if (waterMode == MODE_WATER_AUTOMATIC)
      {
        if ((waterLevelAvg < waterLevelLow) && (waterState == STATE_WATER_OFF))
        {
          digitalWrite(WATER_CONTROL_PIN, HIGH);
          waterState = STATE_WATER_ON;
        }
        else if ((waterLevelAvg > waterLevelHigh) && (waterState == STATE_WATER_ON))
        {
          digitalWrite(WATER_CONTROL_PIN, LOW);
          waterState = STATE_WATER_OFF;
        }
      }
    }

    dtostrf(waterLevelAvg, 5, 1, levelStr);

   
    //######### SERIAL SEND ################

    Serial.print("{analog_level_value,");
    Serial.print(analogLevelValue);
    Serial.println("}");

    Serial.print("{flow,");
    for(int i = 0; i < strLen; i++) {
      if(litersStr[i] == '\0')
	break;
      if(!isspace(litersStr[i])) {
	Serial.print(litersStr[i]);
      }
    }      
    Serial.println("}");
    Serial.print("{level,");
    for(int i = 0; i < strLen; i++) {
      if(levelStr[i] == '\0')
	break;
      if(!isspace(levelStr[i])) {
	Serial.print(levelStr[i]);
      }
    }      
    Serial.println("}");

    Serial.print("{auto_level,");
    printState(waterMode == MODE_WATER_AUTOMATIC);
    Serial.println("}");

    Serial.print("{water,");
    printState(waterState == STATE_WATER_ON);
    Serial.println("}");

    Serial.print("{sprinkler1,");
    printState(sprinkler1State == STATE_WATER_ON);
    Serial.println("}");

    if(ack_requested) {
      Serial.print("{ack,");
      Serial.print(seqNo);
      Serial.println("}");
    }
  }
}
