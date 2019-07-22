
#ifndef UI_TABLE_H_INCLUDED
#define UI_TABLE_H_INCLUDED

#include "Config.h"
#include "LcdDisplay.h"
#include "RotaryEncoder.h"
#include "Regulator.h"


typedef void UpdateFunctionDelegate(byte);

enum UI_Mode { DarkOps, Selecting, Editing };

#define LO 0x80	// low half of hi/low pair
#define HI  0x40	// high half of hi/low pair
#define RO 0x20	// read-only
#define EN 0x10 // enum
#define UF 0x08 // has update function
#define INDX 0x07 // low-order bits index into aux tables for EN and UF


struct UITableEntry
{
	byte Flags;		// half of a high/low pair
	byte* Variable;	// address of this parameter
	byte Min, Max;	// min and max values
	char Address;  	// address in EEPROM -- do not change these capriciously
	// UpdateFunctionDelegate* Func;	// func to call after changing, if non-null
	// char** EnumText;	// spellings for enum type parameter, if non-null

	static void LoadCurrentUITableEntry( int index );
	static void LoadCurrentUITableEntry();

	static char** GetEnum( byte index );
	static UpdateFunctionDelegate* GetUpdateFunction( byte index );

	// load and save some or all parameters to EEPROM

	int SaveSettingToEEPROM();
	int LoadSettingFromEEPROM();
	static void LoadAllSettingsFromEEPROM();
	static void SaveAllSettingsToEEPROM();
};

extern const byte* IntensityDimVariable;
extern UITableEntry CurrentUITableEntry;
extern int UITableEntryCount;
extern char CurrentTitleString[ 16 ];


#endif
