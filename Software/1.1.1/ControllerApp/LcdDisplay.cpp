#include "LcdDisplay.h"
#include "Sensor.h"

// LCD Pin assignments

const byte LCD_Enable = A5;
const byte LCD_RS = A4;

const byte LCD_D7 = A3;
const byte LCD_D6 = A2;
const byte LCD_D5 = A1;
const byte LCD_D4 = A0;

// layout positions

const int ValueColumn = 8;
const int statColumn = 15;
const int statRow = 0;

const Pos TitleRow( 0, 0 );
const Pos ValueRow( 0, 1 );
const Pos StatPos( statColumn, statRow );
const Pos ValuePos( ValueColumn, 1 );
const Pos TempPos( 0, 1 );
const Pos HumidPos( 8, 1 );

char UpOrDownArrow( SensorTrend trend );

LcdDisplay::LcdDisplay()
	: LiquidCrystal( LCD_RS, LCD_Enable, LCD_D4, LCD_D5, LCD_D6, LCD_D7  )
	, DisplayContrastPot( DigiPotSelectPin )
{
    begin( LCD_COLUMNS, LCD_ROWS );

	InitGlyphs( this );

	IntensityNormal = 24;
	IntensityDim = 5;
	Contrast = 28;

	pinMode( LCD_PWM_Pin, OUTPUT );
}

void LcdDisplay::Move( Pos pos )
{
	setCursor( pos.Col, pos.Row );
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

void LcdDisplay::SetContrast( int contrast )
{
    DisplayContrastPot.Set( contrast );
}


void LcdDisplay::SetIntensity( int value )
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

static uint8_t uparrow1_glyph[]={ 0x4, 0xE, 0x1F, 0x4, 0x4, 0x4, 0x4, 0x4 };
static uint8_t downarrow1_glyph[]={ 0x4, 0x4, 0x4, 0x4, 0x4, 0x1F, 0xE, 0x4 };
static uint8_t tinyplus_glyph[]={ 0, 0, 0x4, 0xE, 0x4, 0, 0, 0 };
static uint8_t bang1_glyph[]={ 0x0, 0x8, 0x8, 0x8, 0x0, 0x8, 0x0, 0x0 };
static uint8_t bang2_glyph[]={ 0x0, 0xA, 0xA, 0xA, 0x0, 0xA, 0x0, 0x0 };
static uint8_t dotdotdot_glyph[]={ 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x15 };
// static uint8_t uparrow2_glyph[]={ 0x4, 0x4, 0xE, 0xE, 0x1F, 0x1F,0x4, 0x4 };
// static uint8_t leftarrow1_glyph[]={ 0, 0x2, 0xC, 0xE, 0xC, 0x2,0x0, 0x0 };


void LcdDisplay::InitGlyphs(LcdDisplay* lcd)
{
    lcd->createChar( uparrow1, uparrow1_glyph );
    lcd->createChar( downarrow1, downarrow1_glyph );
    lcd->createChar( tinyplus, tinyplus_glyph );
    lcd->createChar( dotdotdot, dotdotdot_glyph );
    lcd->createChar( bang1, bang1_glyph );
    lcd->createChar( bang2, bang2_glyph );
}
