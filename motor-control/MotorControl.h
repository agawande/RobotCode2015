#include "Arduino.h"

#ifndef MotorControl_h
#define MotorControl_h

class MotorControl
{
  public:
    MotorControl(int ENA, int ENB, int IN1, int IN2, int IN3, int IN4);
	void accelerateForward();
	void accelerateBack();
	void driveForward();
	void driveBack();
	void slowStop();
	void halt();	
        void driveLeft();
        void driveRight();
        //void drive(int degrees, float theta);
        //void stop (bool isHardStop)
	
  private:
    int _ENA;
    int _ENB;
    int _IN1;
    int _IN2; 
    int _IN3;
    int _IN4;
};

#endif
