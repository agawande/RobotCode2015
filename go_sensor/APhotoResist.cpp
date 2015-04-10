/*
	AphotoResist.cpp - Library for using photoresistors.
	Created by Shawn Westcott, Jan 09, 2014
*/

#include "Arduino.h"
#include "AphotoResist.h"

APhotoResist::APhotoResist(int pin)
{
	_pin = pin;
}

int APhotoResist::fetchValue()
{
  return analogRead(_pin);
}
