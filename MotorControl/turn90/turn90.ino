/*
	turn90.ino is a sketch that makes a two-wheeled robot turn in place 90 degrees using PID and Quadrature Feedback.
	
	Specifically, it sets two pid loops whose setpoint is 0 and the input is the difference between that motor's
	number of gear ticks and the number of gear ticks needed to turn 90 degrees. One motor will drive forward, 
	and the other in reverse.
*/

#include "Motor_Control.h"
#include "PID_v1.h"
#include <Encoder.h>

// Variable for maximum speed of robot. This is a PWM value (0-255)
double maxSpeed = 64;

// Variable defining number of ticks for a 90 degree turn needed from each motor.
int turnTicks = 5500;

// Define the direction of the turn
bool turnLeft = true;

// Motor Pin Definitions
const int IN1 = 0;
const int IN2 = 1;
const int IN3 = 2;
const int IN4 = 3;
const int ENA = 4;
const int ENB = 5;

// PID Parameter Input Definitions
int knobKp = A1;
int knobKi = A2;
int knobKd = A3;

// Initial PID values
double Kp = 0.1;
double Ki = 0.1;
double Kd = 0.1;

// Quadrature Feedback Pin Definitions
#define c_LeftEncoderPinA 7
#define c_LeftEncoderPinB 8
#define c_RightEncoderPinA 9
#define c_RightEncoderPinB 10

// Initializing Quadrature Feedback Variables.
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

long positionLeft = 0;
long positionRight = 0;

// Setting up the MotorControl object with the proper pins.
Motor rightMotor(ENA,IN1,IN2);
Motor leftMotor(ENB,IN3,IN4);

//Define Variables we'll be connecting to
double SetpointL, InputL, OutputL;
double SetpointR, InputR, OutputR;

//Specify the links and initial tuning parameters
// DIRECT means an increase here should increase our output. REVERSE means an increase here decreases our output.
PID leftPID(&InputL, &OutputL, &SetpointL, Kp, Ki, Kd, DIRECT);
PID rightPID(&InputR, &OutputR, &SetpointL, Kp, Ki, Kd, DIRECT);

void setup()
{
	// Open serial communications
	Serial.begin(9600);

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

        // Our signed drive algorithm allows negative PWM values, which are treated as reverse driving.
        rightPID.SetOutputLimits(-maxSpeed,maxSpeed);
        leftPID.SetOutputLimits(-maxSpeed,maxSpeed);
	
	// This sketch works by driving both motors with the goal of making the difference between the number of ticks and the goal number of ticks 0.
	SetpointL = 0;
	SetpointR = 0;
	
	// Set our initial inputs. 
	// To drive a motor backwards, we remove the goal ticks from the current ticks. This means we have to drive it backwards to bring our state down to make the difference 0.
	// To drive it forward, we remove our current ticks from the goal ticks. This means we must drive the motor forward to bring the state up to make the difference 0.
	if (turnLeft) {
		InputL = _LeftEncoderTicks - turnTicks;
		InputR = turnTicks - _RightEncoderTicks;
	} else {
		InputL = turnTicks - _LeftEncoderTicks;
		InputR = _RightEncoderTicks - turnTicks;
	}
	
	// Set our sample times. This is 1ms because these measurements are very prone to sudden changes.
	leftPID.SetSampleTime(1);
	rightPID.SetSampleTime(1);
	
	leftPID.SetMode(AUTOMATIC);
	rightPID.SetMode(AUTOMATIC);
}

void loop()
{
	FetchPIDConstants();
	
	// Recalculate our inputs.
	if (turnLeft) {
		InputL = _LeftEncoderTicks - turnTicks;
		InputR = turnTicks - _RightEncoderTicks;
	} else {
		InputL = turnTicks - _LeftEncoderTicks;
		InputR = _RightEncoderTicks - turnTicks;
	}
	
	// Compute PID values
	leftPID.Compute();
	rightPID.Compute();
	
	// Drive the motors.
	leftMotor.signedDrive((int)OutputL);
	rightMotor.signedDrive((int)OutputR);
	
}

void FetchPIDConstants() {
	// Note the old values.
	double oldKp = Kp;
	double oldKi = Ki;
	double oldKd = Kd;
	
	// Read our knob values
	Kp = analogRead(knobKp);
	Ki = analogRead(knobKi);
	Kd = analogRead(knobKd);
	
	// Map these to actual PID constants.
	Kp = map(Kp, 0, 1023, 0.01, 5);
	Ki = map(Ki, 0, 1023, 0.01, 5);
	Kd = map(Kd, 0, 1023, 0.01, 5);
	
	// Alter the tunings.
	leftPID.SetTunings(Kp, Ki, Kd);
	rightPID.SetTunings(Kp, Ki, Kd);
	
	// Show the current P, I, and D constants only if they're changed from the old ones.
	if ((oldKp != Kp) || (oldKi != Ki) || (oldKd != Kd)) {
		Serial.println('Kp: ' + Kp + ' ');
		Serial.println('Ki: ' + Ki + ' ');
		Serial.println('Kd: ' + Kd);
		Serial.println('\n');
	}
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
