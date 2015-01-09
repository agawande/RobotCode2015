#include "APhotoResist.h"

// Pin Definitions
const int pin = 1;  // Analog Pin 1.

// Setting up the MotorControl object with the proper pins.
AphotoResist go_sensor(pin);

void setup()
{
 Serial.begin(9600);
 Serial.println("Go Sensor Test");     // Ready statement.
}

void loop()
{
  // TODO: Fetch value from photoresistor and spit it out to serial.
 ;
}
