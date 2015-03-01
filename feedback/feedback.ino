#include "Arduino.h"

// Quadrature encoders

#define c_LeftEncoderPinA 7
#define c_LeftEncoderPinB 8

#define c_RightEncoderPinA 9
#define c_RightEncoderPinB 10

volatile bool _LeftEncoderASet;
volatile bool _LeftEncoderBSet;
volatile bool _LeftEncoderAPrev;
volatile bool _LeftEncoderBPrev;
volatile long _LeftEncoderTicks = 0;

volatile bool _RightEncoderASet;
volatile bool _RightEncoderBSet;
volatile bool _RightEncoderAPrev;
volatile bool _RightEncoderBPrev;
volatile long _RightEncoderTicks = 0;

void setup()
{
  // Quadrature encoders
  
  // Left encoder
  pinMode(c_LeftEncoderPinA, INPUT);      // sets pin A as input
  digitalWrite(c_LeftEncoderPinA, LOW);  // turn on pullup resistors
  pinMode(c_LeftEncoderPinB, INPUT);      // sets pin B as input
  digitalWrite(c_LeftEncoderPinB, LOW);  // turn on pullup resistors
  attachInterrupt(c_LeftEncoderPinA, HandleLeftMotorInterruptA, CHANGE);
  attachInterrupt(c_LeftEncoderPinB, HandleLeftMotorInterruptB, CHANGE);
  
  // Right encoder
  pinMode(c_RightEncoderPinA, INPUT);      // sets pin A as input
  digitalWrite(c_RightEncoderPinA, LOW);  // turn on pullup resistors
  pinMode(c_RightEncoderPinB, INPUT);      // sets pin B as input
  digitalWrite(c_RightEncoderPinB, LOW);  // turn on pullup resistors
  attachInterrupt(c_RightEncoderPinA, HandleRightMotorInterruptA, CHANGE);
  attachInterrupt(c_RightEncoderPinB, HandleRightMotorInterruptB, CHANGE);
}

void loop()
{ 
	Serial.print("L Encoder Ticks: ");
	Serial.print(_LeftEncoderTicks);
	Serial.print("  L Revolutions: ");
	Serial.print(_LeftEncoderTicks/8400.0);		// how to count: 64 counts/rev (PDR) * 131.25:1 (gear ratio) = 8400
	
	Serial.print("  R Encoder Ticks: ");
	Serial.print(_RightEncoderTicks);
	Serial.print("  R Revolutions: ");
	Serial.print(_RightEncoderTicks/8400.0);	// how to count: 64 counts/rev (PDR) * 131.25:1 (gear ratio) = 8400
	Serial.print("\n");
}

// Interrupt service routines for the left motor's quadrature encoder
void HandleLeftMotorInterruptA(){
  _LeftEncoderBSet = digitalRead(c_LeftEncoderPinB);
  _LeftEncoderASet = digitalRead(c_LeftEncoderPinA);
  
  _LeftEncoderTicks+=ParseLeftEncoder();
  
  _LeftEncoderAPrev = _LeftEncoderASet;
  _LeftEncoderBPrev = _LeftEncoderBSet;
}

void HandleLeftMotorInterruptB(){
  // Test transition;
  _LeftEncoderBSet = digitalRead(c_LeftEncoderPinB);
  _LeftEncoderASet = digitalRead(c_LeftEncoderPinA);
  
  _LeftEncoderTicks+=ParseLeftEncoder();
  
  _LeftEncoderAPrev = _LeftEncoderASet;
  _LeftEncoderBPrev = _LeftEncoderBSet;
}

// Interrupt service routines for the Right motor's quadrature encoder
void HandleRightMotorInterruptA(){
  _RightEncoderBSet = digitalRead(c_RightEncoderPinB);
  _RightEncoderASet = digitalRead(c_RightEncoderPinA);
  
  _RightEncoderTicks+=ParseRightEncoder();
  
  _RightEncoderAPrev = _RightEncoderASet;
  _RightEncoderBPrev = _RightEncoderBSet;
}

void HandleRightMotorInterruptB(){
  // Test transition;
  _RightEncoderBSet = digitalRead(c_RightEncoderPinB);
  _RightEncoderASet = digitalRead(c_RightEncoderPinA);
  
  _RightEncoderTicks+=ParseRightEncoder();
  
  _RightEncoderAPrev = _RightEncoderASet;
  _RightEncoderBPrev = _RightEncoderBSet;
}

int ParseLeftEncoder(){
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
