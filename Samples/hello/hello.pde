/*
** Humidity Controller -- JJB -- Q4 2011
*/

#include <LiquidCrystal.h>
#include <Sensirion.h>
#include <EEPROM.h>    // dunno why this is necessary
#include <SPI.h>

#include "EEPROM2.h"
#include "Glyphs.h"
#include "UI_Table.h"
#include "DigitalPot.h"
#include "OutputExpander.h"
#include "Regulator.h"

// Pin assignments and device declarations ///////////////////////////////////////

// const int DRED = A4;
// const int DGRN = A5;

// Sensor device

const byte SensorDataPin = 7;  // 2.red
const byte SensorClockPin = 6;  // 3.orange
const byte SensorPowerPin = 8;  // yellow / purple

Sensirion Sensor(SensorDataPin, SensorClockPin);


// LCD display

const byte LCD_Enable = A5;
const byte LCD_RS = A4;

const byte LCD_D3 = A3;
const byte LCD_D2 = A2;
const byte LCD_D1 = A1;
const byte LCD_D0 = A0;

const int LCD_RW = -1;  // not in use -- wired to ground

const int LCD_ROwS = 2;
const int LCD_COLUMNS = 16;

const int LCD_PWM_Pin = 5;    // PWM pin wired to power LCD's backlight LED

LiquidCrystal lcd(LCD_RS, LCD_Enable, LCD_D0, LCD_D1, LCD_D2, LCD_D3 );


// digi pot controls LCD's contrast

const int DigiPotSelectPin = 9;   

DigitalPot DisplayContrastPot( DigiPotSelectPin );

// expansion shift register for 8 more outputs

const int ShiftSelectPin = 10;    

OutputExpander ExpansionPort( ShiftSelectPin );

// Rotary Encoder

const int EncoderA = 3;
const int EncoderB = 2;
const int Button = 4;

// temperature and humidity Regulation

void HumidityActivator( byte onOff )
{
  ExpansionPort.SetDehumidifier( onOff );
}

RegulatorMaximus HumidityRegulator( &HumidityParameters, &HumidityActivator );

void TemperatureActivator( byte onOff )
{
  ExpansionPort.SetHeaterOrCooler( onOff );
}

RegulatorMaximus TemperatureRegulatorCooling( &TemperatureParameters, &HumidityActivator );

RegulatorMinimus TemperatureRegulatorHeating( &TemperatureParameters, &HumidityActivator );


// Globals ////////////////////////////////////////////////////////////////////////////////

// encoder counts

volatile char Counter = 0;  // need signed arithmetic
char PrevCounter = 0;

// timeouts and elapsed times

int UpdateSensorInterval = 5; // seconds

const int ReturnNormalTimeout = 30;  // seconds
const int DimDisplayTimeout = 60;    // seconds

unsigned long prevUpdateSensor = -(unsigned long)UpdateSensorInterval*1000;  // force refresh at startup
unsigned long PreviousInputEvent;
unsigned long PreviousTickEvent;

// UI states

enum UI_Mode { DarkOps, Selecting, Editing };

int SelectedIndex;
int UIMode;
int PrevIndex;
int PrevMode;


// setup //////////////////////////////////////////////////////////////////

void setup() {
  
    Serial.begin(38400); // Open serial connection to report values to host
    lcd.begin(LCD_COLUMNS,LCD_ROwS);
    SPI.begin();
    
    EEPROM2.LoadAllSettingsFromEEPROM();
    
    InitGlyphs( &lcd );
  
    UIMode = Selecting;
    SelectedIndex = 0;
    PrevIndex = -1;
    PrevMode = -1;
  
    pinMode(EncoderA, INPUT);
    digitalWrite(EncoderA, HIGH);
    pinMode(EncoderB, INPUT);
    digitalWrite(EncoderB, HIGH);
    pinMode(Button, INPUT);
    digitalWrite(Button, HIGH);

    pinMode (LCD_PWM_Pin, OUTPUT);

    pinMode (SensorPowerPin, OUTPUT);
    digitalWrite(SensorPowerPin,HIGH);

    SetDisplayPwm( DisplayPwmNormal );
    DisplayContrastPot.Set( DisplayContrast );

    attachInterrupt(0, EncoderInterruptHandler, CHANGE);
    attachInterrupt(1, EncoderInterruptHandler, CHANGE);

//      pinMode( DRED, OUTPUT);
//      pinMode( DGRN, OUTPUT);
//	digitalWrite(DRED, LOW);
//	digitalWrite(DGRN, LOW);
  
    Serial.println("Ready...");
    lcd.print( "Initializing..." );
}

//  loop ////////////////////////////////////////////////////////////////////////////


void loop() {
#if 0
   if( PrevIndex != SelectedIndex){
     Serial.print( "Index: ");
     Serial.print( PrevIndex );
     Serial.print(" -> ");
     Serial.println( SelectedIndex);
     PrevIndex = SelectedIndex;
   }
   
   if( PrevMode != UIMode ){
     Serial.print( "Mode: ");
     Serial.print( PrevMode );
     Serial.print(" -> ");
     Serial.println( UIMode);
     PrevMode = UIMode;
   }
#endif
   
   AnimateUI();
     
   if( SelectedIndex == 0 )
     NormalRunMode();
 }
 
 
 // Misc helper functions
 
 void SetDisplayContrast( int value )
{
  DisplayContrastPot.Set( value );
}

void SetShiftRegister( int value )
{
  ExpansionPort.SetCounter( value );
}

void SetDisplayPwm( int pwm )
{
  const byte UI_MAX = 32;
  const byte HW_MAX = 255;
  
    pwm *= ( HW_MAX + 1 ) / 32;
  
  if(pwm > HW_MAX )
    pwm = HW_MAX;

  if( pwm <= 0 )
    lcd.noDisplay();
  else
    lcd.display();

   analogWrite(LCD_PWM_Pin, pwm);
}

