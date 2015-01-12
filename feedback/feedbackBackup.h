
#include "Arduino.h"

// Quadrature encoders
// Left encoder

#define c_RightEncoderInterrupt 0
#define c_LeftEncoderInterrupt 1
#define c_RightEncoderPin 2
#define c_LeftEncoderPin 3

volatile long _LeftEncoderTicks = 0;
volatile long _RightEncoderTicks = 0;

volatile int leftPulseCount = 0;
volatile int rightPulseCount = 0;

volatile leftPulseRate = 0;
volatile rightPulseRate = 0;

volatile leftTempCount = 0;
volatile rightTempCount = 0;

volatile leftLastMicros = 0;
volatile rightLastMicros = 0;

void setup()
{
	Serial.begin(9600);

	// Quadrature encoders
	// Left encoder
	pinMode(c_LeftEncoderPin, INPUT);			// sets pin as input
	digitalWrite(c_LeftEncoderPin, LOW);		// turn on pullup resistors
	attachInterrupt(c_LeftEncoderInterrupt, HandleLeftMotorInterrupt, CHANGE);

	// Right encoder
	pinMode(c_RightEncoderPin, INPUT);			// sets pin as input
	digitalWrite(c_RightEncoderPin, LOW);		// turn on pullup resistors
	attachInterrupt(c_RightEncoderInterrupt, HandleRightMotorInterrupt, CHANGE);
}

void loop()
{ 
	Serial.print("Left Encoder Ticks: ");
	Serial.print(_LeftEncoderTicks);
	Serial.print("  Left Revolutions: ");
	Serial.print(_LeftEncoderTicks/4200.0);			// how to count: 64 counts/rev (PDR) * 131.25:1 (gear ratio) = 8400, Half resolution = 4200.
	
	Serial.print("  Right Encoder Ticks: ");
	Serial.print(_RightEncoderTicks);
	Serial.print("  Right Revolutions: ");
	Serial.print(_RightEncoderTicks/4200.0);			// how to count: 64 counts/rev (PDR) * 131.25:1 (gear ratio) = 8400, Half resolution = 4200.
	Serial.print("\n");
}

// Interrupt service routines for the left motor's quadrature encoder
void HandleLeftMotorInterrupt(){
	
	_LeftEncoderTicks++;
	leftPulseCount++;
	leftTempCount++;
	
	if leftPulseCount == 4) {
		leftPulseRate = leftTempCount / (micros() - leftLastMicros);		// leftPulseRate is speed in ticks per microsecond.
		leftPulseCount = 0;
		leftTempCount = 0;
		LeftLastMicros = micros();
	}
}

// Interrupt service routines for the right motor's quadrature encoder
void HandleRightMotorInterrupt(){
	
	_RightEncoderTicks++;
	rightPulseCount++;
	rightTempCount++;
	
	if rightPulseCount == 4) {
		rightPulseRate = rightTempCount / (micros() - rightLastMicros);		// leftPulseRate is speed in ticks per microsecond.
		rightPulseCount = 0;
		rightTempCount = 0;
		rightLastMicros = micros();
	}
}