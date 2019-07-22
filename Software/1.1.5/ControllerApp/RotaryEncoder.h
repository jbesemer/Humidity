
#ifndef ROTARY_ENCODER_h
#define ROTARY_ENCODER_h

#include "Arduino.h"

#include "Config.h"

class RotaryEncoder
{
	byte PinA;
	byte PinB;

	byte State;

	int Counter;

	unsigned long EncoderStart;
	unsigned long EncoderFinish;

public:
	RotaryEncoder( byte pinA, byte pinB );
	int GetCount();
	void Poll();
	void Poll2();
};

#endif
