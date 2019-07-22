#include "OutputExpander.h"


const byte CountMask = 0x3;  // low 2 bits of count
const byte CountBits = 4;    // high nibble

const byte HeaterBit = 0;
const byte CoolerBit = 1;
const byte DehumidifierBit = 2;



OutputExpander::OutputExpander( byte enablePin )
  : SPI_Expander( enablePin )
{
}

void OutputExpander::SetHeater( byte onOff )
{
  SetBit( CoolerBit, 0 );
  SetBit( HeaterBit, onOff );
  Flush();
}

void OutputExpander::SetCooler( byte onOff )
{
  SetBit( HeaterBit, 0 );
  SetBit( CoolerBit, onOff );
  Flush();
}

void OutputExpander::SetDehumidifier( byte onOff )
{
  SetBit( DehumidifierBit, onOff );
  Flush();
}

void OutputExpander::SetCounter( byte bits )
{
  int mask = CountMask << CountBits;

  DataCache = ( DataCache & ~mask ) | (( bits & CountMask ) << CountBits );
  Flush();
}



