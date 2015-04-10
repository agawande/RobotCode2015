#include "Arduino.h"

#ifndef APhotoResist_h
#define APhotoResist_h

class APhotoResist
{
	public:
		APhotoResist(int pin);
		int fetchValue();
	
	private:
		int _pin;
};

#endif
