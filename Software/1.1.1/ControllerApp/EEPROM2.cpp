/*
 Extend eeprom for little-endian ints
*/

#include <avr/eeprom.h>
#include "WConstants.h"
#include "EEPROM2.h"

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



EEPROM2Class EEPROM2;
