
#include "SPI_Expander.h"

SPI_Expander::SPI_Expander( byte enablePin, int InitialData, byte enableState )
{
	SPI.begin();

	EnablePin = enablePin;

	DataCache = InitialData;

	EnableState = enableState;

	DisableState
		= (( enableState == HIGH )
			? LOW
			: HIGH );

	pinMode( EnablePin, OUTPUT );
	digitalWrite( EnablePin, HIGH );
}

void SPI_Expander::SetBit( byte pin, byte onOff )
{
	int mask = 1 << pin;

	if( onOff )
		DataCache |= mask;
	else
		DataCache &= ~mask;
 }

byte SPI_Expander::GetBit( byte pin )
{
	return ( DataCache >> pin ) & 1;
}

 void SPI_Expander::Flush()
 {
	Send( DataCache );
 }

 void SPI_Expander::Send( byte data )
 {
	digitalWrite( EnablePin, EnableState );

	SPI.transfer( data );

	digitalWrite( EnablePin, DisableState );
}


