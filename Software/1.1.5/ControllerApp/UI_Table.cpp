
#include "UI_Table.h"
#include "EEPROM2.h"
#include "Sensor.h"
// #include "CRC16.h"
#include "UI.h"

const byte* IntensityDimVariable = &Display.IntensityDim;

char* InfoStrings[] = {
	"Version 1.1.5",
	"Date 9-March-12",
};

#define InfoStringMax ( sizeof( InfoStrings ) / sizeof( char* ) - 1 )


byte InfoIndex = 0;


// ui table in FLASH

UITableEntry UITable[] PROGMEM = {
//////"1234567890123456"
	{ 0,  NULL,                           0,    0,  -1 },
	{ RO|EN|0, &InfoIndex,                 0,   InfoStringMax,  -1 },

	{ HI, &HumidityParameters.Parameters.High,       0,   99,   2 },
	{ LO, &HumidityParameters.Parameters.Low,        0,   99,   4 },
	{ 0,  &HumidityParameters.Parameters.MinOn,      0,  120,  10 },
	{ 0,  &HumidityParameters.Parameters.MinOff,     0,  120,  12 },
	{ HI, &HumidityParameters.OverTemp,  33,  120,  42 },
	{ LO, &HumidityParameters.UnderTemp, 33,  120,  44 },

	{ HI, &HeatingParameters.High,   	33,  110,   6 },
	{ LO, &HeatingParameters.Low,    	33,  110,   8 },
	{ 0,  &HeatingParameters.MinOn,  	 0,  120,  14 },
	{ 0,  &HeatingParameters.MinOff, 	 0,  120,  16 },

	{ HI, &CoolingParameters.High,   	33,  110,  26 },
	{ LO, &CoolingParameters.Low,    	33,  110,  28 },
	{ 0,  &CoolingParameters.MinOn,  	 0,  120,  30 },
	{ 0,  &CoolingParameters.MinOff, 	 0,  120,  32 },

#if LIGHTING_FAIL_SAFE
	{ HI, &LightingParameters.High,   	33,  140,  34 },
	{ LO, &LightingParameters.Low,    	33,  140,  36 },
	{ 0,  &LightingParameters.MinOn,  	 0,  120,  38 },
	{ 0,  &LightingParameters.MinOff, 	 0,  120,  40 },
#endif

	{ 0,  &UpdateSensorInterval,          5,  300,  24 },
	{ UF|0,  &Display.IntensityNormal,       0,   32,  18 },
	{ UF|0,  &Display.IntensityDim,          0,   32,  22 },

#if PROTOTYPE
	{ UF|1,  &Display.Contrast,              0,   32,  20 },

#endif
};

int UITableEntryCount = sizeof( UITable) / sizeof( UITableEntry );


// strings in FLASH

char* StringTable[] = {

	"Run",
	"Info",

	"Humid High",
	"Humid Low",
	"Humid Min On",
	"Humid Min Off",
	"Humid Hi Temp",
	"Humid Lo Temp",

	"Heat High",
	"Heat Low",
	"Heat Min On",
	"Heat Min Off",

	"Cool High",
	"Cool Low",
	"Cool Min On",
	"Cool Min Off",

#if LIGHTING_FAIL_SAFE
	"Light High",
	"Light Low",
	"Light Min On",
	"Light Min Off",
#endif

	"Sensor Interval",
	"Bright Level",
	"Dim Level",

#if PROTOTYPE
	"Contrast",
#endif

};


UITableEntry CurrentUITableEntry;

char CurrentTitleString[ 16 ];

void UITableEntry::LoadCurrentUITableEntry( int index )
{
	// load the table entry
	memcpy_P(
		(void*)&CurrentUITableEntry,
		(PGM_VOID_P)&UITable[ index ],
		sizeof( UITableEntry ));

	// load the string
	strncpy(
		CurrentTitleString,
		StringTable[ index ],
		sizeof( CurrentTitleString ));
}

void UITableEntry::LoadCurrentUITableEntry()
{
	LoadCurrentUITableEntry( SelectedIndex );
}

char** UITableEntry::GetEnum( byte index )
{
	switch( index )
	{
	case 0:
		return InfoStrings;
	}

	return NULL;
}

UpdateFunctionDelegate* UITableEntry::GetUpdateFunction( byte index )
{
	switch( index )
	{
	case 0:
		return SetDisplayIntensity;
	case 1:
		return SetDisplayContrast;
	}

	return NULL;
}

// save/restore settings to/from EEPROM

int UITableEntry::SaveSettingToEEPROM()
{
	if( Variable != NULL ){
		EEPROM2.write_int( Address, *Variable );
		return *Variable;
	}
	else
		return NULL;
}


int UITableEntry::LoadSettingFromEEPROM()
{
	int value = EEPROM2.read_int( Address );

	if( Variable != NULL ){
		*Variable = value;
		return value;
	}
	else
		return NULL;
}

void UITableEntry::LoadAllSettingsFromEEPROM()
{
	// Crc16 checksum;
	int i;

	// should do checksum in 2 passes: 1. to validate data; 2. to update variables

	for( i=1; i < UITableEntryCount; i++ ){
		LoadCurrentUITableEntry( i );

		if( CurrentUITableEntry.Address >= 0 ){
			int value = EEPROM2.read_int( CurrentUITableEntry.Address );
			// checksum.AccumulateChecksum( value );

			if( value == 0 )
				continue;

			if( value < CurrentUITableEntry.Min )
				value = CurrentUITableEntry.Min;
			if( value > CurrentUITableEntry.Max )
				value = CurrentUITableEntry.Max;
			if(( CurrentUITableEntry.Flags & LO ) != 0 ){
				// enforce hi/lo relationship
			}

			(*CurrentUITableEntry.Variable) = value;
		}
	}
}

void UITableEntry::SaveAllSettingsToEEPROM()
{
	// Crc16 checksum;
	int i;

	for( i=1; i < UITableEntryCount; i++ ){
		LoadCurrentUITableEntry( i );
		CurrentUITableEntry.SaveSettingToEEPROM();

		// checksum.AccumulateChecksum( value );
	}
}

