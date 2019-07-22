#include "OutputExpander.h"


const byte CountMask = 0x3;  // low 2 bits

const byte HeaterBit = 2;
const byte DehumidifierBit = 3;



OutputExpander::OutputExpander( byte enablePin )
  : SPI_Expander( enablePin )
{
}

void OutputExpander::SetHeaterOrCooler( byte onOff )
{
  SetBit( HeaterBit, onOff );
  Flush();
}

void OutputExpander::SetDehumidifier( byte onOff )
{
  SetBit( DehumidifierBit, onOff );
  Flush();
}

void OutputExpander::SetCounter( byte bits )
{
  DataCache = ( DataCache & ~CountMask ) | ( bits & CountMask );
  Flush();
}



