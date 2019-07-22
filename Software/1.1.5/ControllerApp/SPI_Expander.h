
#ifndef SPI_EXPANDER_h
#define SPI_EXPANDER_h

#include "Arduino.h"
#include <SPI.h>

// driver for SPI connection to TPIC6B595 8 bit register, or equivalant

class SPI_Expander
{
protected:

	byte EnablePin;
	byte EnableState;
	byte DisableState;

 public:

	int DataCache;  // 1/0 => active/inactive, whatever that means in the hardware

	SPI_Expander(
		byte enablePin, 				// pin connected to register's enable line
		int InitialData=0, 			// initial data cache (only relevant if using SetBit, GetBit)
		byte EnableState=LOW );	// whether enable is active low (default) or high

	virtual void Send( byte data);
	void SetBit( byte pin, byte onOff );
	byte GetBit( byte pin );
	virtual void Flush();
};

#endif

