
#include "WConstants.h"
#include "UI_Table.h"
#include "EEPROM2.h"

// display settings

int DisplayPwmNormal = 145;
int DisplayPwmDim = 145;
int DisplayContrast = 114;

// process settings

RegulatorParameters HumidityParameters = {
  34, 30, 0, 0,
};

RegulatorParameters TemperatureParameters = {
  70, 68, 0, 0,
};


extern void SetDisplayPwm( int pwm );
extern void SetDisplayContrast( int value );

UITableEntry UITable[] = {
  { "Run",             NULL,                              0,    0, -1,  NULL },
  { "Humid High",      &HumidityParameters.High,       0,   99,  2,  NULL },
  { "Humid Low",       &HumidityParameters.Low,      0,   99,  4,  NULL },
  { "Temp High",       &TemperatureParameters.High,      33,  110,  6,  NULL },
  { "Temp Low",        &TemperatureParameters.Low,       33,  110,  8,  NULL },
  { "Humid Min On",    &HumidityParameters.MinOn,         0,  120, 10,  NULL },
  { "Humid Min Off",   &HumidityParameters.MinOff,        0,  120, 12,  NULL },
  { "Temp Min On",     &TemperatureParameters.MinOn,      0,  120, 14,  NULL },
  { "Temp Min Off",    &TemperatureParameters.MinOff,     0,  120, 16,  NULL },
  { "Sensor Interval", &UpdateSensorInterval,              5,  300, 24,  NULL },
  { "Bright Level",    &DisplayPwmNormal,                  0,   32, 18,  SetDisplayPwm },
  { "Dim Level",       &DisplayPwmDim,                     0,   32, 22,  SetDisplayPwm },
  { "Contrast",        &DisplayContrast,                   0,   32, 20,  SetDisplayContrast },
};

int UITableEntryCount = sizeof( UITable) / sizeof( UITableEntry );



void UITableEntry::SaveSettingToEEPROM()
{
  if( Variable != NULL )
    EEPROM2.write_int( Address, *Variable );
}


void UITableEntry::LoadSettingFromEEPROM()
{
  int value = EEPROM2.read_int( Address );
  if( value != -1 && Variable != NULL )
    *Variable = value;
}

