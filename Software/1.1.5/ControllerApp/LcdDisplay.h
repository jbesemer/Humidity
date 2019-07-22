
#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include "Config.h"
#include "LiquidCrystal.h"
#include "DigitalPot.h"


#define LCD_RW  -1  // not in use -- wired to ground

// PWM pin wired to power LCD's backlight LED

#define LCD_PWM_Pin  5

// digi pot controls LCD's contrast

#define DigiPotSelectPin  9

// Screen dimensions

#define LCD_ROWS  2
#define LCD_COLUMNS  16


// Class def

class LcdDisplay
  : public LiquidCrystal
{
	DigitalPot DisplayContrastPot;

	void InitGlyphs();
	void LoadGlyph(  byte code, byte* src );

public:

	byte IntensityNormal;
	byte IntensityDim;
	byte Contrast;

    LcdDisplay();

	void Move( byte pos );
	void MoveTempPos();
	void MoveHumidPos();

	void ShowTitle( char* s );
	void ShowChange( bool changing );
	void ShowUpdating( bool updating );
	void ShowValue( char* s, bool wide = false );
	void ShowValue( int value, bool wide = false );

	void SetContrast( byte contrast );
	void SetIntensity( byte value );
	void SetIntensityNormal();
	void SetIntensityDim();
};


// Custom glyph char codes

#define bang1		((char)2)
#define bang2		((char)3)
#define uparrow1	((char)0)
#define downarrow1	((char)1)
#define dotdotdot	((char)6)
#define tinyplus	((char)7)


// singleton instance

extern LcdDisplay Display;


#endif
