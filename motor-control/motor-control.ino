#include "MotorControl.h"

// Pin Definitions
const int ENA = 10;
const int ENB = 9;
const int IN1 = 2;
const int IN2 = 3;
const int IN3 = 4;
const int IN4 = 5;

// Setting up the MotorControl object with the proper pins.
MotorControl robotMotor(ENA,ENB,IN1,IN2,IN3,IN4);

// String stuff for serial communications. This will eventually be phased out.
// TODO: we don't really need a string using up 200 bytes of memory. If we're only using one character, we can conver this to a char to save resources.
// TODO: determine a standard data type for commands maybe? The uCs will have to talk to eachother over I2C anyway, so it might be good to start in on this now.
String inputString = "";         // Container for incoming data.
boolean stringComplete = false;

void setup()
{
 Serial.begin(9600);
 Serial.println("Motor Test");     // Ready statement.
 inputString.reserve(200);
}

void loop()
{
  // Parsing input.
  // Commands:
    // f = Accelerate, then drive forward.
    // b = Accelerate, then drive backward. 
    // s = slow stop
    // h = halt/e-stop
    // r = turn right
    // l = turn left
    
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
      robotMotor.turnRight();
    }
    else if(inputString == "l")
    {
      robotMotor.turnLeft();
    }
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();     // get the new byte
    inputString += inChar;                 // add it to the inputString
    
    // if the incoming character is a newline, set a flag so the main loop can do something about it
    // f = accelerate and forward, b = accelerate and backward, s = decelarate to stop, h=halt
    if (inChar == 'f' || inChar == 'b' || inChar == 's' || inChar == 'h' || inChar == 'l' || inChar =='r') {
      stringComplete = true;
    }  
  }
}
