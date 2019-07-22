
#include "UI_Table.h"
#include "UI.h"
#include "EEPROM2.h"
#include "Sensor.h"
// #include "CRC16.h"


#define ReturnNormalTimeout	30  // seconds
#define DimDisplayTimeout		60    // seconds

char SelectedIndex;
byte UIMode;
char PrevIndex;
byte PrevMode;

unsigned long PreviousInputEvent;
unsigned long PreviousTickEvent;


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


// set display intensity and contrast

void SetDisplayIntensity( byte value )
{
	Display.SetIntensity( value );
}

void SetDisplayContrast( byte value )
{
	Display.SetContrast( value );
}

void InitUI()
{
	UIMode = Selecting;
	SelectedIndex = 0;
	PrevIndex = -1;
	PrevMode = -1;

	UITableEntry::LoadAllSettingsFromEEPROM();
	UITableEntry::LoadCurrentUITableEntry();

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
				if( CurrentUITableEntry.Variable != NULL )		// RO is dealt with below...
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
			if( CurrentUITableEntry.Variable == IntensityDimVariable )
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

	UITableEntry::LoadCurrentUITableEntry();

#if TRACE_SELECTED_INDEX_CHANGE
	Serial.print("Index=");
	Serial.print( SelectedIndex );
	Serial.print(" ");
	Serial.println( CurrentTitleString );
#endif

	Display.clear();

	if( SelectedIndex == 0 ){
		UpdateHomePage();
		return;
	}

	Display.ShowTitle( CurrentTitleString );
	ShowCurrentValue();
}

void ShowCurrentValue()
{
	int value = *( CurrentUITableEntry.Variable );
	byte flags = CurrentUITableEntry.Flags;

	if( flags & EN ){
		char** enumText = UITableEntry::GetEnum( flags & INDX );
		Display.ShowValue( enumText[ value ], flags & RO );
	} else
		Display.ShowValue( value, flags & RO  );

	Display.print( "   " );
}

void BeginEdit()
{
#if TRACE_EDIT_VALUE
	Serial.println( "BeginEdit ");  //Serial.println(SelectedIndex);
#endif

	Display.ShowTitle( CurrentTitleString );

	Display.ShowChange(( CurrentUITableEntry.Flags & RO  ) == 0);

	ShowCurrentValue();

	// if changing dim intensity, set display to dim
	if( CurrentUITableEntry.Variable == IntensityDimVariable )
		Display.SetIntensityDim();
}

void EditSelection( int count )
{
  // Serial.print( "EditSelection ");Serial.println(count);

	int value = *( CurrentUITableEntry.Variable );
	value += count;

	if( value > CurrentUITableEntry.Max )
	value = CurrentUITableEntry.Max;

	if( value < CurrentUITableEntry.Min )
	value = CurrentUITableEntry.Min;

	*(CurrentUITableEntry.Variable) = value;

	ShowCurrentValue();

	byte flags = CurrentUITableEntry.Flags;

#if TRACE_EDIT_VALUE
	Serial.print("Value = ");
	if( flags & EN ){
          char** enumText = GetEnum( flags & INDX );
		Serial.println( enumText[ value ]);
	} else
		Serial.println( value );
#endif

	if( flags & UF )
	{
		UpdateFunctionDelegate* fn
			= UITableEntry::GetUpdateFunction( flags & INDX );

		if( fn != NULL )
			(*fn)( value );
  }
}


void CancelEdit()
{
#if TRACE_EDIT_VALUE
	Serial.println( "CancelEdit ");
#endif
	//*(CurrentUITableEntry.Variable) = SavedValue;
	CurrentUITableEntry.LoadSettingFromEEPROM();	// easier than caching a saved value
}

void AcceptEdit()
{
	int value = *( CurrentUITableEntry.Variable );

	CurrentUITableEntry.SaveSettingToEEPROM();

#if TRACE_EDIT_VALUE
	Serial.println( "AcceptEdit: " );
	Serial.println( value );
#endif

	// enforce constraint for HiLoPairs

	switch( CurrentUITableEntry.Flags & ( HI | LO )){
	case HI:
		// selected is Hi half, make Low half match
		UITableEntry::LoadCurrentUITableEntry( SelectedIndex + 1 );
		if( value < *( CurrentUITableEntry.Variable ))
			*( CurrentUITableEntry.Variable ) = value;
		UITableEntry::LoadCurrentUITableEntry();
		break;

	case LO:
		// selected is Low half, make High half match
		UITableEntry::LoadCurrentUITableEntry( SelectedIndex - 1 );
		if( value > *( CurrentUITableEntry.Variable ))
			*( CurrentUITableEntry.Variable ) = value;
		UITableEntry::LoadCurrentUITableEntry();
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


