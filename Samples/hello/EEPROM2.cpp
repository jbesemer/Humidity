/*
 Extend eeprom for little-endian ints
*/

#include <avr/eeprom.h>
#include "WConstants.h"
#include "EEPROM2.h"
#include "UI_Table.h"

// little-endian ints

int EEPROM2Class::read_int(int address)
{
	return eeprom_read_byte((unsigned char *) address)
            | ( eeprom_read_byte((unsigned char *) address + 1 )
                  << 8 );
}

void EEPROM2Class::write_int(int address, int value)
{
	eeprom_write_byte((unsigned char *) address, value);
	eeprom_write_byte((unsigned char *) address + 1, value >> 8 );
}

void EEPROM2Class::LoadAllSettingsFromEEPROM()
{
  int i;
  
  for( i=1; i < UITableEntryCount; i++ ){
      UITable[ i ].LoadSettingFromEEPROM();
  }
}

void EEPROM2Class::SaveAllSettingsToEEPROM()
{
  int i;
  
  for( i=1; i < UITableEntryCount; i++ ){
      UITable[ i ].SaveSettingToEEPROM();
  }
}



EEPROM2Class EEPROM2;
