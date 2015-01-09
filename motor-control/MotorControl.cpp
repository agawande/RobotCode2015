/*
MotorControl.cpp Library for controlling a two wheel robot (i.e. control two motors)
Created by Ashlesh, Dec 19, 2014
*/

#include "Arduino.h"
#include "MotorControl.h"


MotorControl::MotorControl(int ENA, int ENB, int IN1, int IN2, int IN3, int IN4)
{
	 //wheel 1
	 pinMode(ENA, OUTPUT);     // Define as an output
	 pinMode(IN1, OUTPUT);     // Define as an output
	 pinMode(IN2, OUTPUT);     // Define as an output
	 
	 //wheel 2
	 pinMode(ENB, OUTPUT);     // Define as an output
	 pinMode(IN3, OUTPUT);     // Define as an output
	 pinMode(IN4, OUTPUT);     // Define as an output
	 
	_ENA = ENA;
	_ENB = ENB;
	_IN1 = IN1;
	_IN2 = IN2;
	_IN3 = IN3;
	_IN4 = IN4;
}

void MotorControl::accelerateForward()
{
 digitalWrite(_IN1, HIGH);     // Sets motor to run forward
 digitalWrite(_IN2, LOW);     // Sets motor to run forward
 
 digitalWrite(_IN3, HIGH);     // Sets motor to run forward
 digitalWrite(_IN4, LOW);     // Sets motor to run forward

 int i = 0;     // Set "i" to zero
 while(i < 255)     //Accelerate motor over time
 {
 i+=5;
 //Serial.println(i);       // Display current motor speed
 analogWrite(_ENA, i);     // Send current motor speed setting to motor
 analogWrite(_ENB, i);
 delay(30);     // Wait
 }
}

void MotorControl::driveForward()
{
 digitalWrite(_IN1, HIGH);     // Sets motor to run forward
 digitalWrite(_IN2, LOW);     // Sets motor to run forward
 analogWrite(_ENA, 255);      // Turns the motor on and sets speed to 255
 
 digitalWrite(_IN3, HIGH);     // Sets motor to run forward
 digitalWrite(_IN4, LOW);     // Sets motor to run forward
 analogWrite(_ENB, 255);      // Turns the motor on and sets speed to 255
} 

void MotorControl::accelerateBack()
{
 digitalWrite(_IN1, LOW);     // Sets motor to run forward
 digitalWrite(_IN2, HIGH);     // Sets motor to run forward
 
 digitalWrite(_IN3, LOW);     // Sets motor to run forward
 digitalWrite(_IN4, HIGH);     // Sets motor to run forward

int i = 0;     // Set "i" to zero
while(i < 255)     //Accelerate motor over time
 {
 i+=5;
 //Serial.println(i);       // Display current motor speed
 analogWrite(_ENA, i);     // Send current motor speed setting to motor
 analogWrite(_ENB, i);
 delay(30);     // Wait
 }
}

void MotorControl::driveBack()
{
   digitalWrite(_IN1, LOW);     // Sets motor to run forward
   digitalWrite(_IN2, HIGH);     // Sets motor to run forward
   analogWrite(_ENA, 255);      // Turns the motor on and sets speed to 255
   
   digitalWrite(_IN3, LOW);     // Sets motor to run forward
   digitalWrite(_IN4, HIGH);     // Sets motor to run forward
   analogWrite(_ENB, 255);      // Turns the motor on and sets speed to 255
}

void MotorControl::slowStop()
{
 if(analogRead(_ENA) != 0 || analogRead(_ENB) != 0 )
 {
   for(int i=255; i>=0; i-=5) 
   {
	 //Serial.println(i);
	 analogWrite(_ENA, i);
	 analogWrite(_ENB, i);
	 delay(30);
   }
 }
}

void MotorControl::halt()
{
 if(analogRead(_ENA) != 0 || analogRead(_ENB) != 0 )
 {
  analogWrite(_ENA, 0);
  analogWrite(_ENB, 0);
 }
}
