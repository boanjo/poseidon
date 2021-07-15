#include <Arduino.h>
#include <stdlib.h>
#include <ctype.h>

#define SERIES_RESISTOR 560

#define WATER_LEVEL_PIN A0
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
long waterLevelLow = 160;
long waterLevelHigh = 165;

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
  byte present = 0;
  byte type_s;
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

    // Calculate the Water level in pond
    waterLevel = analogRead(WATER_LEVEL_PIN);

    // convert the value to resistance
    waterLevel = (1023 / waterLevel) - 1;
    waterLevel = SERIES_RESISTOR / waterLevel;
    waterLevel = 1483 - waterLevel;
    waterLevel = waterLevel / 6;
    waterLevel += 50;

    // TODO Fixme magic number
    waterLevel += 30;

    levelArray[levelAvgCnt % NO_OF_LEVEL_AVERAGE_ITEMS] = waterLevel;
    levelAvgCnt++;

    if (levelAvgCnt >= NO_OF_LEVEL_AVERAGE_ITEMS)
    {
      for (int i = 0; i < NO_OF_LEVEL_AVERAGE_ITEMS; i++)
      {
        waterLevelAvg += levelArray[i];
      }

      waterLevelAvg = waterLevelAvg / NO_OF_LEVEL_AVERAGE_ITEMS;

      // The etape is only supposed to read values starting from 1 inch (i.e. 25.4mm) so how
      // can we get lower values? And beyond 8*25.4 should not be possible either
      // Lets turn off water and auto just to not overfill. Something is wrong....
      if ((waterLevelAvg < 15) || (waterLevelAvg > 200))
      {
        waterMode = MODE_WATER_MANUAL;

        //waterLevelAvg = 0;
        
        digitalWrite(WATER_CONTROL_PIN, LOW);
        waterState = STATE_WATER_OFF;
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
