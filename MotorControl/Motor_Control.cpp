/*
MotorControl.cpp Library for controlling a two wheel robot (i.e. control two motors)
Created by Ashlesh, Dec 19, 2014
*/

#include "Arduino.h"
#include "Motor_Control.h"


Motor::Motor(int ENA, int IN1, int IN2)
{
	_ENA = ENA;
	_IN1 = IN1;
	_IN2 = IN2;

	pinMode(_ENA, OUTPUT);
	pinMode(_IN1, OUTPUT);
	pinMode(_IN2, OUTPUT);

	analogWrite(_ENA, 0);
}

void Motor::setForward()
{
	// This function sets the motor to drive forward by setting the H bridge pins L-H
	
	digitalWrite(_IN1, LOW);
	digitalWrite(_IN2, HIGH);
}

void Motor::setReverse()
{
	// This function sets the motor to drive in reverse by setting the H bridge pins H-L
	
	digitalWrite(_IN1, HIGH);
	digitalWrite(_IN2, LOW);
}

void Motor::halt()
{
	// A hard/emergency stop. Immediately disable both motors and set speeds to 0.
	// Because of how this stops the motor, you will need to manually set a direction (or use analogDrive) to resume normal function.
	
	analogWrite(_ENA, 0);
	digitalWrite(_IN1, LOW);
	digitalWrite(_IN2, LOW);
}

void Motor::analogDrive(int amt)
{
	// Drives this motor with PWM given by amt. Any value greater than 255 is set to 255, and values under 0 are set to 0.
	
	amt = (amt > 255) ? 255 : amt;
	amt = (amt < 0)   ? 0   : amt;
	
	analogWrite(_ENA, amt);
}

void Motor::signedDrive(int amt)
{
	// Drives this motor at the amount given by amt. amt is an integer from -255 to 255.
	// signedDrive handles directionality on its own and will set the direction of the motor according to the sign.
	// Old direction will be retained on speed of 0.
	
	if (amt > 0) {
		this->setForward();
	} 
	else if (amt < 0) {
		this->setReverse();
		amt = -amt;
	}
	
	this->analogDrive(amt);
	
}
