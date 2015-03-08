#include "Motor_Control.h"
#include "PID_v1.h"
#include <Encoder.h>

// Pin Definitions
const int IN1 = 0;
const int IN2 = 1;
const int IN3 = 2;
const int IN4 = 3;
const int ENA = 4;
const int ENB = 5;

// Setting up the MotorControl object with the proper pins.
Motor rightMotor(ENA,IN1,IN2);
Motor leftMotor(ENB,IN3,IN4);

//----------------For-PID--------------------

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

//Define Variables we'll be connecting to
double Setpoint, Input, Output;
double SetpointL, InputL, OutputL;
double SetpointR, InputR, OutputR;

//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint,0.5,0.5,0.5, DIRECT);

//------------------------------------PID------------------------------------------

void setup()
{
 Serial.begin(9600);
 Serial.println("Motor Test");     // Ready statement.
 Setpoint = 0;
 Input = _LeftEncoderTicks - _RightEncoderTicks;
 //turn the PID on
  myPID.SetOutputLimits(-255, 255);
  myPID.SetMode(AUTOMATIC);
  myPID.SetSampleTime(1);
  
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
  
  //Start driving left motor, PID in the loop will make sure that the right motor follows it
  leftMotor.signedDrive(125);
}

long positionLeft  = 0;
long positionRight = 0;

void loop()
{  
  if (_LeftEncoderTicks != _RightEncoderTicks) {
    // If the right motor has moved
        Input = _LeftEncoderTicks - _RightEncoderTicks;
	myPID.Compute();
        Serial.print("Output: ");
        Serial.println((int) Output);
        rightMotor.signedDrive((int) Output);
	//rightMotor.analogDrive(Output);
        /*Serial.print(" L Encoder Ticks: ");
        Serial.print(_LeftEncoderTicks);
        Serial.print("  L Revolutions: ");
        Serial.print(_LeftEncoderTicks/8400.0);		// how to count: 64 counts/rev (PDR) * 131.25:1 (gear ratio) = 8400
      
        Serial.print("  R Encoder Ticks: ");
        Serial.print(_RightEncoderTicks);
        Serial.print("  R Revolutions: ");
        Serial.print(_RightEncoderTicks/8400.0);	// how to count: 64 counts/rev (PDR) * 131.25:1 (gear ratio) = 8400
        Serial.print("\n");*/
  }
  
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
    if(!_LeftEncoderASet && _LeftEncoderBSet) return -1;
    if(_LeftEncoderASet && !_LeftEncoderBSet) return 1;
  }else if(!_LeftEncoderAPrev && _LeftEncoderBPrev){
    if(!_LeftEncoderASet && !_LeftEncoderBSet) return -1;
    if(_LeftEncoderASet && _LeftEncoderBSet) return 1;
  }else if(!_LeftEncoderAPrev && !_LeftEncoderBPrev){
    if(_LeftEncoderASet && !_LeftEncoderBSet) return -1;
    if(!_LeftEncoderASet && _LeftEncoderBSet) return 1;
  }else if(_LeftEncoderAPrev && !_LeftEncoderBPrev){
    if(_LeftEncoderASet && _LeftEncoderBSet) return -1;
    if(!_LeftEncoderASet && !_LeftEncoderBSet) return 1;
  }
}

int ParseRightEncoder(){
  if(_RightEncoderAPrev && _RightEncoderBPrev){
    if(!_RightEncoderASet && _RightEncoderBSet) return -1;
    if(_RightEncoderASet && !_RightEncoderBSet) return 1;
  }else if(!_RightEncoderAPrev && _RightEncoderBPrev){
    if(!_RightEncoderASet && !_RightEncoderBSet) return -1;
    if(_RightEncoderASet && _RightEncoderBSet) return 1;
  }else if(!_RightEncoderAPrev && !_RightEncoderBPrev){
    if(_RightEncoderASet && !_RightEncoderBSet) return -1;
    if(!_RightEncoderASet && _RightEncoderBSet) return 1;
  }else if(_RightEncoderAPrev && !_RightEncoderBPrev){
    if(_RightEncoderASet && _RightEncoderBSet) return -1;
    if(!_RightEncoderASet && !_RightEncoderBSet) return 1;
  }
}


