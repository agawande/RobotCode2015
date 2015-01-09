/*
	AphotoResist.cpp - Library for using photoresistors.
	Created by Shawn Westcott, Jan 09, 2014
*/

#include "Arduino.h"
#include "AphotoResist.h"

AphotoResist::AphotoResist(int pin)
{
	_pin = pin;
}

void AphotoResist::fetchValue()
{
	
}