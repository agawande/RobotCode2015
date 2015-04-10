#include "APhotoResist.h"

// Pin Definitions
const int pin = 0;  // Analog Pin 0.

// Setting up the MotorControl object with the proper pins.
APhotoResist go_sensor(pin);

void setup()
{
 Serial.begin(9600);
 Serial.println("Go Sensor Test");     // Ready statement.
 while(go_sensor.fetchValue()>700){;}
 //while(go_sensor.fetchValue()<200){}
}

void loop()
{
  // TODO: Fetch value from photoresistor and spit it out to serial.
  // start wheel
  Serial.println("move!!!");
}
