/*
	go_straight.ino is a sketch that drives both motors forward while trying to keep the robot perfectly straight
	using PID and Quadrature Feedback.
	
	Specifically, it drives one motor at a constant speed, and makes the other motor match it by trying to equalize
	the number of gear ticks from quadrature feedback.
*/

#include "Motor_Control.h"

IntervalTimer dataTimer;

// Variable for maximum speed of robot. This is a PWM value (0-255)

// Motor Pin Definitions
const int IN1 = 0;
const int IN2 = 1;
const int IN3 = 2;
const int IN4 = 3;
const int ENA = 4;
const int ENB = 5;

// Setting up the MotorControl object with the proper pins.
Motor rightMotor(ENA,IN1,IN2);
Motor leftMotor(ENB,IN3,IN4);

//Line following
int frontLeftPin = A1; //connected to analog 1
int frontMidPin = A2;
int frontRightPin = A3;
int colorChange = 512;
int fLeftVal = 0;
int fMidVal = 0;
int fRightVal = 0;

int action = 2;

int leftLed = 23;
int midLed = 22;
int rightLed = 21;

int leftSpeed = 40;
int rightSpeed = 40;

void setup()
{
  Serial.begin(9600);
  pinMode(rightLed, OUTPUT);
  pinMode(midLed, OUTPUT);
  pinMode(leftLed, OUTPUT);  
  dataTimer.begin(getAction, 1000);
}

void loop()
{	
    switch (action) {
      case 1:
        leftSpeed -= 1;
        rightSpeed += 1;
      case 2:
        leftSpeed = 64;
        rightSpeed = 64;
      case 3:
        leftSpeed -= 2;
        rightSpeed += 2;
      case 4:
        rightSpeed -= 2;
        leftSpeed += 2;
      case 6:
        rightSpeed -= 1;
        leftSpeed += 1;
      default:
        int sweepLength = 30;
        boolean isRight = true;
        while (action != 2) {
          double startTime = millis();
          if(isRight) {
            leftMotor.setForward();
            rightMotor.setReverse();
          } else {
            leftMotor.setReverse();
            rightMotor.setForward();
          }
          leftMotor.analogDrive(64);
          rightMotor.analogDrive(64);
          while(millis()<startTime+sweepLength || action == 2){;}
          isRight = !isRight;
        }
        leftMotor.signedDrive(0);
        rightMotor.signedDrive(0);
    }
    
    leftMotor.signedDrive(leftSpeed);
    rightMotor.signedDrive(rightSpeed);
    
    delay(20);
}

void getAction(void) {
  for (int i = 0; i < 10; i++) {
      fLeftVal += analogRead(frontLeftPin);
      fMidVal += analogRead(frontMidPin);
      fRightVal += analogRead(frontRightPin);
    }
    
    fLeftVal = fLeftVal / 10;
    fMidVal = fMidVal / 10;
    fRightVal = fRightVal / 10;
    
    fLeftVal = check_color(fLeftVal);
    fMidVal = check_color(fMidVal);
    fRightVal = check_color(fRightVal);
    
    action = ((fLeftVal << 2) | (fMidVal << 1) | fRightVal);
    
    digitalWrite(leftLed, fLeftVal);
    digitalWrite(midLed, fMidVal);
    digitalWrite(rightLed, fRightVal);
}

boolean check_color(int val){
  if(val>colorChange){
    return false;   //black
  }
  else{
    return true;  //white
  }
}


