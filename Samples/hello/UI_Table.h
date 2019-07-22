
#include "Regulator.h"

typedef void UpdateFunction(int);

struct UITableEntry
{
  char* Title;
  int* Variable;
  int Min, Max;
  int Address;  // address in EEPROM -- do not change these capriciously
  UpdateFunction* Func;

  void SaveSettingToEEPROM();
  void LoadSettingFromEEPROM();
};

extern RegulatorParameters HumidityParameters;
extern RegulatorParameters TemperatureParameters;

extern int UpdateSensorInterval;
extern int DisplayPwmNormal;
extern int DisplayPwmDim;
extern int DisplayContrast;

extern UITableEntry UITable[];
extern int UITableEntryCount;
