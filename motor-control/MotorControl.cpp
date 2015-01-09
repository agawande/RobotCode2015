/*
MotorControl.cpp Library for controlling a two wheel robot (i.e. control two motors)
Created by Ashlesh, Dec 19, 2014
*/

#include "Arduino.h"
#include "MotorControl.h"


MotorControl::MotorControl(int ENA, int ENB, int IN1, int IN2, int IN3, int IN4)
{
	 //wheel 1
	 pinMode(ENA, OUTPUT);
	 pinMode(IN1, OUTPUT);
	 pinMode(IN2, OUTPUT);
	 
	 //wheel 2
	 pinMode(ENB, OUTPUT);
	 pinMode(IN3, OUTPUT);
	 pinMode(IN4, OUTPUT);
	 
	_ENA = ENA;
	_ENB = ENB;
	_IN1 = IN1;
	_IN2 = IN2;
	_IN3 = IN3;
	_IN4 = IN4;
}

void MotorControl::accelerateForward()
{
  // This function slowly brings the robot to full speed to avoid jerky, potentially dangerous movement.
  // ENA and ENB are the enable pins for each motor. By using analog write on these pins, we can pulse the motors with a square wave
  // This lets us adjust the motor speed by altering the square wave (PWM).
  
  // TODO: Alter this code to accept an argument for final speed. Code should select proper step and delay for whatever speed is given.
  // TODO: this code should probably also be general purpose for fading between any two speeds. Can we get current speed? If not, it should be a member variable for this object.
  
  // Setting the pins as H-L drives the motor forward. This is done for both motors.
  digitalWrite(_IN1, HIGH);
  digitalWrite(_IN2, LOW);
  
  digitalWrite(_IN3, HIGH);
  digitalWrite(_IN4, LOW);

  // Accelleration loop.
  int i = 0;
  while(i < 255)
  {
    i += 5;
    analogWrite(_ENA, i);     // Send current motor speed setting to motor
    analogWrite(_ENB, i);
    delay(30);                // Wait 30 ms between steps to make the accelleration gentler.
  }
}

void MotorControl::driveForward()
{
  // This function drives the motors continuously forward at whatever speed has been given. Again, H-L sets motors forward.
  // TODO: Accept speed input here, too.
  
  digitalWrite(_IN1, HIGH);
  digitalWrite(_IN2, LOW);
  analogWrite(_ENA, 255);
  
  digitalWrite(_IN3, HIGH);
  digitalWrite(_IN4, LOW);
  analogWrite(_ENB, 255);
} 

void MotorControl::accelerateBack()
{
  // Same as accellerateForward, but with motors reversed. L-H sets motors backwards.
 
  digitalWrite(_IN1, LOW);
  digitalWrite(_IN2, HIGH);
  digitalWrite(_IN3, LOW);
  digitalWrite(_IN4, HIGH);

  // Accelleration loop.
  int i = 0;
  while(i < 255)
  {
    i+=5;
    analogWrite(_ENA, i);      // Send current motor speed setting to motor
    analogWrite(_ENB, i);
    delay(30);                 // Wait 30 ms between steps to make the accelleration gentler.
  }
}

void MotorControl::driveBack()
{
  // This function drives the motors continuously backwards at whatever speed has been given. L-H sets motors driving backwards.
  // TODO: Accept speed input here, too.
  
  digitalWrite(_IN1, LOW);     // Sets motor to run forward
  digitalWrite(_IN2, HIGH);     // Sets motor to run forward
  analogWrite(_ENA, 255);      // Turns the motor on and sets speed to 255   
  
  digitalWrite(_IN3, LOW);     // Sets motor to run forward
  digitalWrite(_IN4, HIGH);     // Sets motor to run forward
  analogWrite(_ENB, 255);      // Turns the motor on and sets speed to 255
  
}

void MotorControl::slowStop()
{
  // The reverse of either accellerate function. This brings the motor speeds from 255 down to 0 gently.
  // TODO right now, if slowStop is called while the robot is running at some other speed, it will jerk to full power before gently stopping.
    // Again, knowing our current motor speeds will be useful.
    
  // Only run if either or both motors are on.  
  if(analogRead(_ENA) != 0 || analogRead(_ENB) != 0 )
  {
    
    // Speed's goin' down, down, down.
    for(int i=255; i>=0; i-=5) 
    {
      // Push the new speed to the motors, and delay for gentleness.
      analogWrite(_ENA, i);
      analogWrite(_ENB, i);
      delay(30);
    }
  }
}

void MotorControl::halt()
{
  // A hard/emergency stop. No gentleness here. Immediately disable both motors.
  
  analogWrite(_ENA, LOW);
  analogWrite(_ENB, LOW);
}

void MotorControl::turnRight()
{
  // turnRight turns the robot on a hard right. This is done by driving the left motor forward and the right motor backward.
  // TODO: either accept speed input or use current speed.
  // TODO: get PID control working so this command always turns the bot exactly 90 degrees.
  
  // Left wheel forward (H-L)
  digitalWrite(_IN1, HIGH);
  digitalWrite(_IN2, LOW);
  analogWrite(_ENA, 255);
 
  // Right wheel backward (L-H)
  digitalWrite(_IN3, LOW);
  digitalWrite(_IN4, HIGH);
  analogWrite(_ENB, 255);
}

void MotorControl::turnLeft()
{
  // turnLeft turns the robot on a hard left. This is done by driving the left motor backward and the right motor forward.
  // TODO: either accept speed input or use current speed.
  // TODO: get PID control working so this command always turns the bot exactly 90 degrees.
  
  // Left wheel backward (L-H)
  digitalWrite(_IN1, LOW);
  digitalWrite(_IN2, HIGH);
  analogWrite(_ENA, 255);
 
  // Right wheel forward (H-L)
  digitalWrite(_IN3, HIGH);
  digitalWrite(_IN4, LOW);
  analogWrite(_ENB, 255);
}

