#include "LcdDisplay.h"
#include "Sensor.h"

// LCD Pin assignments

#define LCD_Enable  A5
#define LCD_RS  A4

#define LCD_D7  A3
#define LCD_D6  A2
#define LCD_D5  A1
#define LCD_D4  A0

// layout positions

#define ValueColumn  8
#define statColumn  15
#define statRow  0

#define POS( COL, ROW ) ( COL | ( ROW << 6 ))
#define COL( POS ) ( ( POS ) & 0x3F )
#define ROW( POS ) ( ( ( POS ) >> 6 ) & 0x3 )

#define TitleRow	POS( 0, 0 )
#define ValueRow	POS( 0, 1 )
#define StatPos	POS( statColumn, statRow )
#define ValuePos	POS( ValueColumn, 1 )
#define TempPos	POS( 0, 1 )
#define HumidPos	POS( 8, 1 )

char UpOrDownArrow( SensorTrend trend );

// singleton instance

LcdDisplay Display;

// implementation

LcdDisplay::LcdDisplay()
	: LiquidCrystal( LCD_RS, LCD_Enable, LCD_D4, LCD_D5, LCD_D6, LCD_D7  )
	, DisplayContrastPot( DigiPotSelectPin )
{
    begin( LCD_COLUMNS, LCD_ROWS );

	InitGlyphs();

	IntensityNormal = 24;
	IntensityDim = 5;
	Contrast = 28;

	pinMode( LCD_PWM_Pin, OUTPUT );
}

void LcdDisplay::Move( byte pos )
{
	setCursor( COL( pos ), ROW( pos ));
}

void LcdDisplay::MoveTempPos()
{
	Move( TempPos );
}

void LcdDisplay::MoveHumidPos()
{
	Move( HumidPos );
}


void LcdDisplay::ShowTitle( char* s )
{
	Move( TitleRow );
	print( s );
}

void LcdDisplay::ShowChange( bool changing )
{
	Move( ValueRow );
	if( changing )
		print( "CHANGE" );
	else
		print( "       " );
}

void LcdDisplay::ShowUpdating( bool updating )
{
	Move( StatPos );
	if( updating )
		print(dotdotdot);
	else
		print( " " );
}

void LcdDisplay::ShowValue( char* s, bool wide )
{
	if( wide )
		Move( TempPos );
	else
		Move( ValuePos );

	print( s );
}

void LcdDisplay::ShowValue( int value, bool wide )
{
	if( wide )
		Move( TempPos );
	else
		Move( ValuePos );

	print( value );
}

// set contrast and intensity

void LcdDisplay::SetContrast( byte contrast )
{
    DisplayContrastPot.Set( contrast );
}


void LcdDisplay::SetIntensity( byte value )
{
	// Serial.print( "SetIntensity=");
	// Serial.println( value );

  	const byte UI_MAX = 32;
	const byte HW_MAX = 255;

	int pwm = constrain(( value * HW_MAX + 1 ) / UI_MAX, 0, HW_MAX );

	if( pwm <= 0 )
		noDisplay();
	else
		display();

	analogWrite( LCD_PWM_Pin, pwm );
}

void LcdDisplay::SetIntensityNormal()
{
	SetIntensity( IntensityNormal );
}

void LcdDisplay::SetIntensityDim()
{
	SetIntensity( IntensityDim );
}


// custom glyph bitmps

static uint8_t uparrow1_glyph[] PROGMEM = { 0x4, 0xE, 0x1F, 0x4, 0x4, 0x4, 0x4, 0x4 };
static uint8_t downarrow1_glyph[] PROGMEM = { 0x4, 0x4, 0x4, 0x4, 0x4, 0x1F, 0xE, 0x4 };
static uint8_t tinyplus_glyph[] PROGMEM = { 0, 0, 0x4, 0xE, 0x4, 0, 0, 0 };
static uint8_t bang1_glyph[] PROGMEM = { 0x0, 0x8, 0x8, 0x8, 0x0, 0x8, 0x0, 0x0 };
static uint8_t bang2_glyph[] PROGMEM = { 0x0, 0xA, 0xA, 0xA, 0x0, 0xA, 0x0, 0x0 };
static uint8_t dotdotdot_glyph[] PROGMEM = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x15 };
// static uint8_t uparrow2_glyph[]={ 0x4, 0x4, 0xE, 0xE, 0x1F, 0x1F,0x4, 0x4 };
// static uint8_t leftarrow1_glyph[]={ 0, 0x2, 0xC, 0xE, 0xC, 0x2,0x0, 0x0 };


void LcdDisplay::LoadGlyph( byte code, byte* src )
{
	uint8_t glyph[ 8 ];

	memcpy_P(
		(void*)glyph,
		(PGM_VOID_P)src,
		sizeof( glyph ));

    createChar( code, glyph );
}


void LcdDisplay::InitGlyphs()
{
    LoadGlyph( uparrow1, uparrow1_glyph );
    LoadGlyph( downarrow1, downarrow1_glyph );
    LoadGlyph( tinyplus, tinyplus_glyph );
    LoadGlyph( dotdotdot, dotdotdot_glyph );
    LoadGlyph( bang1, bang1_glyph );
    LoadGlyph( bang2, bang2_glyph );
}
