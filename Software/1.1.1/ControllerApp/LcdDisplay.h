
#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include <WProgram.h>
#include <LiquidCrystal.h>

#include "DigitalPot.h"

#include "Config.h"

const int LCD_RW = -1;  // not in use -- wired to ground

// PWM pin wired to power LCD's backlight LED

const int LCD_PWM_Pin = 5;

// digi pot controls LCD's contrast

const int DigiPotSelectPin = 9;

// Screen dimensions

const int LCD_ROWS = 2;
const int LCD_COLUMNS = 16;

// Screen Position Class

struct Pos
{
	byte Row, Col;

	Pos( byte col, byte row )
		: Col(col), Row(row)
	{
	}
};

// Class def

class LcdDisplay
  : public LiquidCrystal
{
	DigitalPot DisplayContrastPot;

	void InitGlyphs( LcdDisplay* );

public:

	int IntensityNormal;
	int IntensityDim;
	int Contrast;

    LcdDisplay();

	void Move( Pos pos );
	void MoveTempPos();
	void MoveHumidPos();

	void ShowTitle( char* s );
	void ShowChange( bool changing );
	void ShowUpdating( bool updating );
	void ShowValue( char* s, bool wide = false );
	void ShowValue( int value, bool wide = false );

	void SetContrast( int contrast );
	void SetIntensity( int value );
	void SetIntensityNormal();
	void SetIntensityDim();
};


// Custom glyph char codes

const char bang1 = (char)2;
const char bang2 = (char)3;
const char uparrow1 = (char)0;
const char downarrow1 = (char)1;
const char dotdotdot = (char)6;
const char tinyplus = (char)7;

#endif
