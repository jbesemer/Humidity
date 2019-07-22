#include <WProgram.h>

#include "Config.h"
#include "LcdDisplay.h"
#include "RotaryEncoder.h"
#include "Regulator.h"

const int ButtonPin = 4;

typedef void UpdateFunctionDelegate(int);

enum UI_Mode { DarkOps, Selecting, Editing };

enum UIEntryFlags {
	Lo = 1,	// low half of hi/low pair
	Hi = 2,	// high half of hi/low pair
	RO = 3,	// read-only
};

struct UITableEntry
{
	char* Title;	// name of this parameter
	int Flags;		// half of a high/low pair
	int* Variable;	// address of this parameter
	int Min, Max;	// min and max values
	int Address;  	// address in EEPROM -- do not change these capriciously
	UpdateFunctionDelegate* Func;	// func to call after changing, if non-null
	char** EnumText;	// spellings for enum type parameter, if non-null

	// load and save some or all parameters to EEPROM

	int SaveSettingToEEPROM();
	int LoadSettingFromEEPROM();
};

// exports
extern void InitUI();
extern void AnimateUI();

extern void SaveAllSettingsToEEPROM();
extern void LoadAllSettingsFromEEPROM();

extern UITableEntry UITable[];
extern int UITableEntryCount;

extern int SelectedIndex;

extern RegulatorParameters HumidityParameters;
extern RegulatorParameters HeatingParameters;
extern RegulatorParameters CoolingParameters;

extern void SetDisplayIntensity( int pwm );
extern void SetDisplayContrast( int value );

// imports

extern char* InfoStrings[] ;
extern int InfoStringCount ;

extern LcdDisplay Display;
extern RotaryEncoder Encoder;

extern void UpdateHomePage();
extern void SetShiftRegister( int value );

extern int UpdateSensorInterval;
extern int TemperatureOperatingMode;


