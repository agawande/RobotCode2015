#include "Motor_Control.h"

// Pin Definitions
const int IN1 = 0;
const int IN2 = 1;
const int IN3 = 2;
const int IN4 = 3;
const int ENA = 4;
const int ENB = 5;

int lSpeed = 0;
int rSpeed = 0;

// Setting up the MotorControl object with the proper pins.
Motor rightMotor(ENA,IN1,IN2);
Motor leftMotor(ENB,IN3,IN4);

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
  
  // Making sure the speeds set are not out of range for PWM.
  if (lSpeed > 255) {
    lSpeed = 255;
  }
  
  if (lSpeed < -255) {
    lSpeed = -255;
  }
  
  if (rSpeed > 255) {
    rSpeed = 255;
  }
  
  if (rSpeed < -255) {
    rSpeed = -255;
  }
    
  // Begin Parsing
  if (stringComplete) {
    Serial.println(inputString); 
    
    if(inputString == "f")
    {
      while (lSpeed < 255 || rSpeed < 255) {
        // Check to make sure the speeds are balanced.
        if (lSpeed != rSpeed) {
          // If not, balance them by accelerating the slower one.
          if (lSpeed < rSpeed) {
            lSpeed++;
          } else {
            rSpeed++;
          }
          rightMotor.signedDrive(rSpeed);
          leftMotor.signedDrive(lSpeed);
        } else {
          // Otherwise, bring them both up to full speed slowly.
          lSpeed++;
          rSpeed++;
          rightMotor.signedDrive(rSpeed);
          leftMotor.signedDrive(lSpeed);
        }
      } 
    }
    
    else if(inputString == "s")
    {
      // Reverse values get increased to 0, forward values get decreased to 0.
      if (lSpeed < 0) {
        int lTick = 1;
      } else {
        int lTick = -1;
      }
      
      if (rSpeed < 0) {
        int rTick = 1;
      } else {
        int rTick = -1;
      }
      
      while (lSpeed != 0 && rSpeed != 0) {
        // If their absolute speeds are not balanced, balance them first.
        if (abs(lSpeed) != abs(rSpeed)) {
          if (abs(lSpeed) > abs(rSpeed)) {
            lSpeed += lTick;
          } else {
            rSpeed += rTick;
          }
          rightMotor.signedDrive(rSpeed);
          leftMotor.signedDrive(lSpeed);
        // Then bring them both to 0 slowly.
        } else {
          lSpeed += lTick;
          rSpeed += rTick;
          rightMotor.signedDrive(rSpeed);
          leftMotor.signedDrive(lSpeed);
        }
      }   
    }
    
    else if(inputString == "b")
    {
      while (lSpeed > -255 || rSpeed > -255) {
        // Check to make sure the speeds are balanced.
        if (lSpeed != rSpeed) {
          // If not, balance them by accelerating the slower one.
          if (lSpeed > rSpeed) {
            lSpeed--;
          } else {
            rSpeed--;
          }
          rightMotor.signedDrive(rSpeed);
          leftMotor.signedDrive(lSpeed);
        } else {
          // Otherwise, bring them both up to full speed slowly.
          lSpeed--;
          rSpeed--;
          rightMotor.signedDrive(rSpeed);
          leftMotor.signedDrive(lSpeed);
        }
      } 
    }
    
    else if(inputString == "h")
    {
      leftMotor.halt();
      rightMotor.halt();
    } 
    
    else if(inputString == "r")
    {
      // To turn right in place, the robot turns the left motor backwards and the right motor forward.
      // This is not done gently.
      
      lSpeed = -255;
      rSpeed = 255;
      rightMotor.signedDrive(rSpeed);
      leftMotor.signedDrive(lSpeed);
      
    }
    
    else if(inputString == "l")
    {
      // To turn left in place, the robot turns the left motor forward and the right motor backwards.
      // This is not done gently.
      
      lSpeed = 255;
      rSpeed = -255;
      rightMotor.signedDrive(rSpeed);
      leftMotor.signedDrive(lSpeed);
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
