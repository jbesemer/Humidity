
#ifndef SPI_EXPANDER_h
#define SPI_EXPANDER_h

#include <WProgram.h>
#include <SPI.h>

class SPI_Expander
{
protected:

	byte EnablePin;
	byte EnableState;
	byte DisableState;

 public:

	int DataCache;  // 1/0 => active/inactive, whatever that means in the hardware

	SPI_Expander( byte enablePin, int InitialData=0, byte EnableState=LOW );

	virtual void Send( byte data);
	void SetBit( byte pin, byte onOff );
	byte GetBit( byte pin );
	virtual void Flush();
};

#endif

