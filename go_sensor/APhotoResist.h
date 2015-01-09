#include "Arduino.h"

#ifndef APhotoResist_h
#define APhotoResist_h

class APhotoResist
{
	public:
		APhotoResist(int readPin);
		void fetchValue();
	
	private:
		int _pin;
};

#endif
