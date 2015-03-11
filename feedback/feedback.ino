#include "Arduino.h"

// Quadrature encoders
#define c_LeftEncoderPinA 7
#define c_LeftEncoderPinB 8
#define c_RightEncoderPinA 9
#define c_RightEncoderPinB 10

// Left Encoder Variables
volatile bool _LeftEncoderASet;
volatile bool _LeftEncoderBSet;
volatile bool _LeftEncoderAPrev;
volatile bool _LeftEncoderBPrev;
volatile long _LeftEncoderTicks = 0;

// Right Encoder Variables
volatile bool _RightEncoderASet;
volatile bool _RightEncoderBSet;
volatile bool _RightEncoderAPrev;
volatile bool _RightEncoderBPrev;
volatile long _RightEncoderTicks = 0;

void setup()
{
	// Left encoder setup
	pinMode(c_LeftEncoderPinA, INPUT);			// set pins A and B as input
	pinMode(c_LeftEncoderPinB, INPUT);
	digitalWrite(c_LeftEncoderPinA, LOW);	 	// turn on pullup resistors
	digitalWrite(c_LeftEncoderPinB, LOW);
	
	// Right encoder
	pinMode(c_RightEncoderPinA, INPUT);			// set pins A and B as input
	pinMode(c_RightEncoderPinB, INPUT);
	digitalWrite(c_RightEncoderPinA, LOW);		// turn on pullup resistors
	digitalWrite(c_RightEncoderPinB, LOW);
	
	// Attach interrupts to the encoder pins.
	// This code is for Teensy 3.x systems. A different format will be used for other controllers
	attachInterrupt(c_LeftEncoderPinA, HandleLeftMotorInterruptA, CHANGE);
	attachInterrupt(c_LeftEncoderPinB, HandleLeftMotorInterruptB, CHANGE);
	attachInterrupt(c_RightEncoderPinA, HandleRightMotorInterruptA, CHANGE);
	attachInterrupt(c_RightEncoderPinB, HandleRightMotorInterruptB, CHANGE);
}

void loop()
{ 
	// Send the data over serial for monitoring.
	Serial.print("L Encoder Ticks: ");
	Serial.print(_LeftEncoderTicks);
	Serial.print("	L Revolutions: ");
	Serial.print(_LeftEncoderTicks/8400.0);		// how to count: 64 counts/rev (PDR) * 131.25:1 (gear ratio) = 8400
	
	Serial.print("	R Encoder Ticks: ");
	Serial.print(_RightEncoderTicks);
	Serial.print("	R Revolutions: ");
	Serial.print(_RightEncoderTicks/8400.0);
	Serial.print("\n");
}

void HandleLeftMotorInterruptA(){
	// Handle the left motor's A interrupt.
	
	// Fetch both pins' states.
	_LeftEncoderBSet = digitalRead(c_LeftEncoderPinB);
	_LeftEncoderASet = digitalRead(c_LeftEncoderPinA);
	
	// Increment this by 1 or -1 based on how the state parses.
	_LeftEncoderTicks+=ParseLeftEncoder();
	
	// Set the old values to these ones.
	_LeftEncoderAPrev = _LeftEncoderASet;
	_LeftEncoderBPrev = _LeftEncoderBSet;
}

void HandleLeftMotorInterruptB(){
	// Handle the Left motor's B interrupt.
	// This works the same as the A interrupt.
	
	_LeftEncoderBSet = digitalRead(c_LeftEncoderPinB);
	_LeftEncoderASet = digitalRead(c_LeftEncoderPinA);
	
	_LeftEncoderTicks+=ParseLeftEncoder();
	
	_LeftEncoderAPrev = _LeftEncoderASet;
	_LeftEncoderBPrev = _LeftEncoderBSet;
}

void HandleRightMotorInterruptA(){
	// The right motor interrupts also work in the same manner.
	_RightEncoderBSet = digitalRead(c_RightEncoderPinB);
	_RightEncoderASet = digitalRead(c_RightEncoderPinA);
	
	_RightEncoderTicks+=ParseRightEncoder();
	
	_RightEncoderAPrev = _RightEncoderASet;
	_RightEncoderBPrev = _RightEncoderBSet;
}

void HandleRightMotorInterruptB(){
	// More of the same here.
	_RightEncoderBSet = digitalRead(c_RightEncoderPinB);
	_RightEncoderASet = digitalRead(c_RightEncoderPinA);
	
	_RightEncoderTicks+=ParseRightEncoder();
	
	_RightEncoderAPrev = _RightEncoderASet;
	_RightEncoderBPrev = _RightEncoderBSet;
}

int ParseLeftEncoder(){
	// The left encoder is parsed with some binary magic.
	// For more information about how this works, look into quadrature encoder mechanics.
	// If you find that your values are increasing when they should decrease, swap all -1's for 1s and vice versa.
	
	if(_LeftEncoderAPrev && _LeftEncoderBPrev){
		if(!_LeftEncoderASet && _LeftEncoderBSet) return 1;
		if(_LeftEncoderASet && !_LeftEncoderBSet) return -1;
	}else if(!_LeftEncoderAPrev && _LeftEncoderBPrev){
		if(!_LeftEncoderASet && !_LeftEncoderBSet) return 1;
		if(_LeftEncoderASet && _LeftEncoderBSet) return -1;
	}else if(!_LeftEncoderAPrev && !_LeftEncoderBPrev){
		if(_LeftEncoderASet && !_LeftEncoderBSet) return 1;
		if(!_LeftEncoderASet && _LeftEncoderBSet) return -1;
	}else if(_LeftEncoderAPrev && !_LeftEncoderBPrev){
		if(_LeftEncoderASet && _LeftEncoderBSet) return 1;
		if(!_LeftEncoderASet && !_LeftEncoderBSet) return -1;
	}
}

int ParseRightEncoder(){
	// The left encoder is parsed with some binary magic.
	// For more information about how this works, look into quadrature encoder mechanics.
	// If you find that your values are increasing when they should decrease, swap all -1's for 1s and vice versa.
	
	if(_RightEncoderAPrev && _RightEncoderBPrev){
		if(!_RightEncoderASet && _RightEncoderBSet) return 1;
		if(_RightEncoderASet && !_RightEncoderBSet) return -1;
	}else if(!_RightEncoderAPrev && _RightEncoderBPrev){
		if(!_RightEncoderASet && !_RightEncoderBSet) return 1;
		if(_RightEncoderASet && _RightEncoderBSet) return -1;
	}else if(!_RightEncoderAPrev && !_RightEncoderBPrev){
		if(_RightEncoderASet && !_RightEncoderBSet) return 1;
		if(!_RightEncoderASet && _RightEncoderBSet) return -1;
	}else if(_RightEncoderAPrev && !_RightEncoderBPrev){
		if(_RightEncoderASet && _RightEncoderBSet) return 1;
		if(!_RightEncoderASet && !_RightEncoderBSet) return -1;
	}
}