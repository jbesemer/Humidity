
#include "UI_Table.h"
#include "EEPROM2.h"
#include "Sensor.h"
#include "CRC16.h"

extern Sensor TheSensor;

const int ReturnNormalTimeout = 30;  // seconds
const int DimDisplayTimeout = 60;    // seconds

int SelectedIndex;
int UIMode;
int PrevIndex;
int PrevMode;

unsigned long PreviousInputEvent;
unsigned long PreviousTickEvent;

const int* IntensityDimVariable = &Display.IntensityDim;

int InfoIndex = 0;

UITableEntry UITable[] = {
//////"1234567890123456"
	{ "Run",             0,  NULL,                           0,    0,  -1,  NULL, NULL },
	{ "Info",        	 RO, &InfoIndex,                 0,   InfoStringCount,  -1,  NULL, InfoStrings },

	{ "Humid High",      Hi, &HumidityParameters.Parameters.High,       0,   99,   2,  NULL, NULL },
	{ "Humid Low",       Lo, &HumidityParameters.Parameters.Low,        0,   99,   4,  NULL, NULL },
	{ "Humid Min On",    0,  &HumidityParameters.Parameters.MinOn,      0,  120,  10,  NULL, NULL },
	{ "Humid Min Off",   0,  &HumidityParameters.Parameters.MinOff,     0,  120,  12,  NULL, NULL },
	{ "Humid Hi Temp",   Hi, &HumidityParameters.OverTemp,   0,  120,  10,  NULL, NULL },
	{ "Humid Lo Temp",   Lo, &HumidityParameters.UnderTemp,  0,  120,  12,  NULL, NULL },

	{ "Heat High",       Hi, &HeatingParameters.High,   	33,  110,   6,  NULL, NULL },
	{ "Heat Low",        Lo, &HeatingParameters.Low,    	33,  110,   8,  NULL, NULL },
	{ "Heat Min On",     0,  &HeatingParameters.MinOn,  	 0,  120,  14,  NULL, NULL },
	{ "Heat Min Off",    0,  &HeatingParameters.MinOff, 	 0,  120,  16,  NULL, NULL },

	{ "Cool High",       Hi, &CoolingParameters.High,   	33,  110,  26,  NULL, NULL },
	{ "Cool Low",        Lo, &CoolingParameters.Low,    	33,  110,  28,  NULL, NULL },
	{ "Cool Min On",     0,  &CoolingParameters.MinOn,  	 0,  120,  30,  NULL, NULL },
	{ "Cool Min Off",    0,  &CoolingParameters.MinOff, 	 0,  120,  32,  NULL, NULL },

	{ "Light High",       Hi, &LightingParameters.High,   	33,  140,  26,  NULL, NULL },
	{ "Light Low",        Lo, &LightingParameters.Low,    	33,  140,  28,  NULL, NULL },
	{ "Light Min On",     0,  &LightingParameters.MinOn,  	 0,  120,  30,  NULL, NULL },
	{ "Light Min Off",    0,  &LightingParameters.MinOff, 	 0,  120,  32,  NULL, NULL },

	{ "Sensor Interval", 0,  &UpdateSensorInterval,          5,  300,  24,  NULL, NULL },
	{ "Bright Level",    0,  &Display.IntensityNormal,       0,   32,  18,  SetDisplayIntensity, NULL },
	{ "Dim Level",       0,  &Display.IntensityDim,          0,   32,  22,  SetDisplayIntensity, NULL },
#if PROTOTYPE
	{ "Contrast",        0,  &Display.Contrast,              0,   32,  20,  SetDisplayContrast, NULL },
#endif
};

int UITableEntryCount = sizeof( UITable) / sizeof( UITableEntry );

// forward refs

void EnterDimMode();
void ExitDimMode();
void UpdateSelection( int count );
void ShowCurrentValue();
void BeginEdit();
void EditSelection( int count );
void CancelEdit();
void AcceptEdit();
void ReturnToNormal();

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

void LoadAllSettingsFromEEPROM()
{
	Crc16 checksum;
	int i;

	// should do checksum in 2 passes: 1. to validate data; 2. to update variables

	for( i=1; i < UITableEntryCount; i++ ){
		UITableEntry* entry = &UITable[ i ];

		if( entry->Address >= 0 ){
			int value = EEPROM2.read_int( entry->Address );
			checksum.AccumulateChecksum( value );

			if( value < entry->Min )
				value = entry->Min;
			if( value > entry->Max )
				value = entry->Max;
			if( entry->Flags == Lo ){
				// enforce hi/lo relationship
			}

			(*entry->Variable) = value;
		}
	}
}

void SaveAllSettingsToEEPROM()
{
	Crc16 checksum;
	int i;

	for( i=1; i < UITableEntryCount; i++ ){
		int value = UITable[ i ].SaveSettingToEEPROM();
		checksum.AccumulateChecksum( value );
	}
}


// set display intensity and contrast

void SetDisplayIntensity( int value )
{
	Display.SetIntensity( value );
}

void SetDisplayContrast( int value )
{
	Display.SetContrast( value );
}

void InitUI()
{
	UIMode = Selecting;
	SelectedIndex = 0;
	PrevIndex = -1;
	PrevMode = -1;

	LoadAllSettingsFromEEPROM();

	// have to do these after settings loaded from EEPROM
	Display.SetIntensityNormal();
	Display.SetContrast( Display.Contrast );

	pinMode(ButtonPin, INPUT);
	digitalWrite(ButtonPin, HIGH);
}

// main entrypoint -- to be called periodically from main loop

void AnimateUI()
{
	// process accumulated counts, if any

	unsigned long now = millis();

	int count = Encoder.GetCount();

	if( count != 0 ){
		PreviousInputEvent = now;

		switch( UIMode ){
		case DarkOps:
			ExitDimMode();  // and otherwise ignore input
			break;
		case Selecting:
			UpdateSelection( count );
			break;
		case Editing:
			EditSelection( count );
			break;
		}
	}

	// then process button-presses, if any

	bool Cancel = false;

	// button is active LOW!!

	int button = digitalRead(ButtonPin);
	if( button == 0 ){
		PreviousInputEvent = now;
		while( digitalRead(ButtonPin) == 0 )
		{
			// busy waiting
		}

		unsigned long elapsed = millis() - PreviousInputEvent;

		// Serial.print( "elapsed = ");
		// Serial.println( elapsed );

		Cancel = ( elapsed >= 500 );

		switch( UIMode ){
		case DarkOps:
		   ExitDimMode();  // and ignore input
		   break;

		case Selecting:
			if( Cancel ){
				//Serial.println( "cancel selecting");
				ReturnToNormal();
			}
			else
			{
				if( SelectedIndex < UITableEntryCount
				&& UITable[ SelectedIndex ].Variable != NULL )
				{
					UIMode = Editing;
					BeginEdit();
				}
			}
			break;

		case Editing:
			if( Cancel )
				CancelEdit();
			else
				AcceptEdit();

			UIMode = Selecting;
			UpdateSelection( 0 );

			// if exiting set dim intensity, restore to normal
			if( UITable[ SelectedIndex ].Variable == IntensityDimVariable )
				Display.SetIntensityNormal();
			break;
		}
	}

	// here, now-PreviousInputEvent == 0 if there was a button or encoder event ahove

	// revert to normal mode after 30 sec of inactivity.
	if( SelectedIndex != 0
	&& now - PreviousInputEvent > ReturnNormalTimeout * 1000 )
	{
		Serial.println( "returning to normal");
		ReturnToNormal();
	}

	// revert to normal mode after 30 sec of inactivity.
	if( UIMode != DarkOps
	&& now - PreviousInputEvent > (unsigned long)DimDisplayTimeout * 1000 )
	{
		EnterDimMode();
	}

	if( now - PreviousTickEvent > 500 )
	{
		static byte cnt = 0;
		PreviousTickEvent = now;
		SetShiftRegister( cnt++ );
	}
}

void EnterDimMode()
{
	ReturnToNormal();  // should already be normal
	UIMode = DarkOps;
	Display.SetIntensityDim();
	Serial.println( "Dim....." );
}

void ExitDimMode()
{
	UIMode = Selecting;
	Display.SetIntensityNormal();
	Serial.println( "Bright..." );
}

void UpdateSelection( int count )
{
	// Serial.print( "Update selection ");Serial.println(count);

	SelectedIndex += count;

	if( SelectedIndex < 0 )
		SelectedIndex = 0;

	if( SelectedIndex >= UITableEntryCount )
		SelectedIndex = UITableEntryCount - 1;

	Serial.print("Index=");
	Serial.print( SelectedIndex );
	Serial.print(" ");
	Serial.println( UITable[ SelectedIndex ].Title );

	Display.clear();

	if( SelectedIndex == 0 ){
		UpdateHomePage();
		return;
	}

	Display.ShowTitle( UITable[ SelectedIndex ].Title );
	ShowCurrentValue();
}

void ShowCurrentValue()
{
	UITableEntry* entry = &UITable[ SelectedIndex ];
	int value = *( entry->Variable );

	if( entry->EnumText != NULL )
		Display.ShowValue( entry->EnumText[ value ], entry->Flags == RO );
	else
		Display.ShowValue( value, entry->Flags == RO  );

	Display.print( "   " );
}

// int SavedValue;

void BeginEdit()
{
	UITableEntry* entry = &UITable[ SelectedIndex ];

	Serial.println( "BeginEdit ");  //Serial.println(SelectedIndex);

	Display.ShowTitle( entry->Title );

	Display.ShowChange( entry->Flags != RO );

	ShowCurrentValue();

	// if changing dim intensity, set display to dim
	if( UITable[ SelectedIndex ].Variable == IntensityDimVariable )
		Display.SetIntensityDim();
}

void EditSelection( int count )
{
  // Serial.print( "EditSelection ");Serial.println(count);

	UITableEntry* entry = &UITable[ SelectedIndex ];
	int value = *( entry->Variable );
	value += count;

	if( value > entry->Max )
	value = entry->Max;

	if( value < entry->Min )
	value = entry->Min;

	*(entry->Variable) = value;

	ShowCurrentValue();

	Serial.print("Value = ");
	if( entry->EnumText != NULL )
		Serial.println( entry->EnumText[ value ]);
	else
		Serial.println( value );

	UpdateFunctionDelegate* fn = UITable[ SelectedIndex ].Func;

	if( fn != NULL )
		(*fn)( value );
}


void CancelEdit()
{
	Serial.println( "CancelEdit ");
	//*(UITable[ SelectedIndex ].Variable) = SavedValue;
	UITable[ SelectedIndex ].LoadSettingFromEEPROM();
}

void AcceptEdit()
{
	UITableEntry* entry = &UITable[ SelectedIndex ];
	int value = *( entry->Variable );

	// SaveAllSettingsToEEPROM();
	UITable[ SelectedIndex ].SaveSettingToEEPROM();

	Serial.println( "AcceptEdit: " );
	Serial.println( value );

	// enforce constraint for HiLoPairs

	switch( entry->Flags ){
	case Hi:
		// selected is Hi half, make Low half match
		entry = &UITable[ SelectedIndex + 1 ];
		if( value < *( entry->Variable ))
			*( entry->Variable ) = value;
		break;

	case Lo:
		// selected is Low half, make High half match
		entry = &UITable[ SelectedIndex - 1 ];
		if( value > *( entry->Variable ))
			*( entry->Variable ) = value;
		break;
	}
}

void ReturnToNormal()
{
	if( UIMode == Editing )
		CancelEdit();

	UIMode = Selecting;
	UpdateSelection( -SelectedIndex );  // back to 0 and update display
}


