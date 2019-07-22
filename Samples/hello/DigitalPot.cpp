#include "DigitalPot.h"

DigitalPot::DigitalPot( byte enablePin )
  : OutputExpander( enablePin )
{
}

const byte address = 0;
const byte UI_MAX = 32;
const byte HW_MAX = 128;


void DigitalPot::Set( byte value )
{
  DataCache = value * HW_MAX / UI_MAX;

  if(DataCache > HW_MAX )
    DataCache = HW_MAX;
  if( DataCache < 0 )
    DataCache = 0;

  Flush();
}

void DigitalPot::Flush()
{
  digitalWrite( EnablePin, EnableState );

  SPI.transfer( address );
  SPI.transfer( DataCache & 0xFF );

  digitalWrite( EnablePin, DisableState );
}

