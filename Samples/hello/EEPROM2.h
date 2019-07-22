/*

*/

#ifndef EEPROM2_h
#define EEPROM2_h

#include <EEPROM.h>
#include <inttypes.h>

class EEPROM2Class : public EEPROMClass
{
  public:
    uint8_t read(int);
    void write(int, uint8_t);

    int read_int(int);
    void write_int(int, int);
    
    void LoadAllSettingsFromEEPROM();
    void SaveAllSettingsToEEPROM();
};

extern EEPROM2Class EEPROM2;

#endif

