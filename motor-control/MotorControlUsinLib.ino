#include "MotorControl.h"

const int ENA = 10;    // Define the pin used on the Arduino
const int ENB = 9;
const int IN1 = 2;    // Define the pin used on the Arduino
const int IN2 = 3;    // Define the pin used on the Arduino
const int IN3 = 4;
const int IN4 = 5;

MotorControl robotMotor(10,9,2,3,4,5);

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void setup()
{
 Serial.begin(9600);     // Set serial comm speed
 Serial.println("Motor Test");     // Print a label for data to be displayed in serial terminal
 inputString.reserve(200);
}

void loop()
{
  if (stringComplete) {
    Serial.println(inputString); 
    if(inputString == "f")
    {
      robotMotor.accelerateForward();
      robotMotor.driveForward();
    }
    else if(inputString == "s")
    {      
      robotMotor.slowStop();
    }
    else if(inputString == "b")
    {
      robotMotor.accelerateBack();
      robotMotor.driveBack();
    }
    else if(inputString == "h")
    {
      robotMotor.halt();
    }    
    else if(inputString == "r")
    {
      robotMotor.driveRight();
    }
    else if(inputString == "l")
    {
      robotMotor.driveLeft();
    }
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}
///////////////////////////////////////////////////////////////////////////

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it
    // f = accelerate and forward, b = accelerate and backward, s = decelarate to stop, h=halt
    if (inChar == 'f' || inChar == 'b' || inChar == 's' || inChar == 'h' || inChar == 'l' || inChar =='r') {
      stringComplete = true;
    } 
  }
}
