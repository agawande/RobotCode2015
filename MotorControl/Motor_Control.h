#include "Arduino.h"

#ifndef Motor_h
#define Motor_h

class Motor
{
  public:
    Motor(int ENA, int IN1, int IN2);
      // Set the motor's direction to forward.
      void setForward();
      
      // Set the motor's direction to reverse.
      void setReverse();
      
      // Halt all motor movement immediately.
	  // A Halt removes directionality, so you'll need to select the direction again after, or use signedDrive.
      void halt();
      
      // Drive the motor using PWM. 0-255.
      void analogDrive(int amt);
      
      // This also drives the motor with PWM, but accepts values from -255 to 255.
      // Negative values are assumed to be in reverse.
      void signedDrive(int amt);
	
  private:
    int _ENA;
    int _IN1;
    int _IN2; 
};

#endif
