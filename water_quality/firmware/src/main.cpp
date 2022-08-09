#include <Arduino.h>
#include <Ezo_i2c.h>                        //include the EZO I2C library from https://github.com/Atlas-Scientific/Ezo_I2c_lib
#include <Wire.h>                           // enable I2C.

#define ADDRESS_RTD 102

Ezo_board do_b = Ezo_board(97, "do");         
Ezo_board orp_b = Ezo_board(98, "orp");         
Ezo_board ph_b = Ezo_board(99, "ph");         
Ezo_board ec_b = Ezo_board(100, "ec");         
Ezo_board rtd_b = Ezo_board(ADDRESS_RTD, "temp");     

unsigned long next_ezo_time = 0;            // holds the time when the next EZO reading is due
boolean request_pending = false;            // wether we've requested a reading from the EZO devices

const unsigned int BLINK_FREQUENCY = 250;   // the frequency of the led blinking, in milliseconds
unsigned long next_blink_time;              // holds the next time the led should change state
boolean led_state = LOW;                    // keeps track of the current led state

#define DEFAULT_TEMP 15.0                   // more suitable temp in my Pond compared to EZO default
float lastTemp = DEFAULT_TEMP;

void receive_reading(Ezo_board &Sensor);


void setup() {
  pinMode(13, OUTPUT);                      // set the led output pin
  Serial.begin(115200);                     // Set the hardware serial port.
  Wire.begin();                             // enable I2C port.

  next_ezo_time = millis() + 1000;
}


// blinks a led on pin 13. this function returns immediately, if it is not yet time to blink
void blink_led() {
  if (millis() >= next_blink_time) {              // is it time for the blink already?
    led_state = !led_state;                       // toggle led state on/off
    digitalWrite(13, led_state);                  // write the led state to the led pin
    next_blink_time = millis() + BLINK_FREQUENCY; // calculate the next time a blink is due
  }
}

// take sensor readings every second. this function returns immediately, if it is not time to interact with the EZO devices.
void read_ezo() {

  if (request_pending) {                    // is a request pending?
    if (millis() >= next_ezo_time) {        // is it time for the reading to be taken?
      receive_reading(do_b);                  // get the reading from the PH circuit
      receive_reading(ph_b);                  // get the reading from the PH circuit
      receive_reading(orp_b);                  // get the reading from the PH circuit
      receive_reading(ec_b);                  // get the reading from the PH circuit
      receive_reading(rtd_b);                 // get the reading from the RTD circuit
      request_pending = false;
      next_ezo_time = millis() + 3000;
    }
  } else {                                  // it's time to request a new reading

    do_b.send_read_with_temp_comp(lastTemp);  // During startup this might be default 
    ph_b.send_read_with_temp_comp(lastTemp);
    ec_b.send_read_with_temp_comp(lastTemp);
    
    orp_b.send_read_cmd();
    rtd_b.send_read_cmd();
    request_pending = true;
  }
}

// update the display
void update_display() {

  // add your display code here

}

// upload data to cloud
void upload_cloud() {

  // add your cloud upload code here
}

// function to decode the reading after the read command was issued
void receive_reading(Ezo_board &Sensor) {
  Serial.print("{");
  Serial.print(Sensor.get_name()); Serial.print(",");  
  Sensor.receive_read_cmd();                            // get the response data

  switch (Sensor.get_error()) {                         // switch case based on what the response code is.
    case Ezo_board::SUCCESS:
      if(Sensor.get_address() == ADDRESS_RTD) {
        lastTemp = Sensor.get_last_received_reading();
      } 

      Serial.print(Sensor.get_last_received_reading()); //the command was successful, print the reading
      break;

    case Ezo_board::FAIL:
      Serial.print("failed");                          //means the command has failed.
      break;

    case Ezo_board::NOT_READY:
      Serial.print("pending");                         //the command has not yet been finished calculating.
      break;

    case Ezo_board::NO_DATA:
      Serial.print("no_data");                         //the sensor has no data to send.
      break;

    default:
      Serial.print("other_problem");
      break;
  }
  Serial.println("}");

}

void loop() {
  // non of these functions block or delay the execution
  read_ezo();
  blink_led();
  update_display();
  upload_cloud();
}
