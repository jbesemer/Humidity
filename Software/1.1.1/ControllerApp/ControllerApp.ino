/*
 * Humidity Controller -- Copyright (c) JJB 2011, 2012
 */

// TODO:
//


#include <Sensirion.h>
#include <LiquidCrystal.h>    // this shouldn't be necessary
#include <EEPROM.h>    		// this shouldn't be necessary
#include <SPI.h>

#include "EEPROM2.h"
#include "UI_Table.h"
#include "OutputExpander.h"
#include "DigitalPot.h"
#include "Regulator.h"
#include "RotaryEncoder.h"
#include "LcdDisplay.h"
#include "Sensor.h"

char* InfoStrings[] = {
	"Version 1.1.1",
	"Date 2-Feb-12",
};

int InfoStringCount = sizeof( InfoStrings ) / sizeof( char* ) - 1;

// Pin assignments and device declarations ///////////////////////////////////////

// const int DRED = A4;
// const int DGRN = A5;

// Temperature and Humidity sensor device

Sensor TheSensor;

// Display device

LcdDisplay Display;

// expansion shift register for 8 more outputs

const int ShiftSelectPin = 10;

OutputExpander ExpansionPort( ShiftSelectPin );

// Rotary Encoder for UI inputs

const int EncoderA = 3;
const int EncoderB = 2;

RotaryEncoder Encoder( EncoderA, EncoderB );

// temperature and humidity Regulators

RegulatorParameters HumidityParameters = {
  34, 30, 0, 0,
};

RegulatorParameters HeatingParameters = {
  70, 68, 0, 0,
};

RegulatorParameters CoolingParameters = {
  72, 70, 0, 0,
};

RegulatorMaximus
	HumidityRegulator(
		"Dehumidifier",
		&HumidityParameters,
		&HumidityActivator );

#if 0
RegulatorMinMax
	TemperatureRegulator(
		"Heater",
		"AC",
		&TemperatureParameters,
		&HeaterActivator,
		&CoolerActivator );

#else
RegulatorMinimus
	HeatingRegulator(
		"Heater",
		&HeatingParameters,
		&HeaterActivator );

RegulatorMaximus
	CoolingRegulator(
		"AC",
		&CoolingParameters,
		&CoolerActivator );

#endif

// setup //////////////////////////////////////////////////////////////////

void setup()
{
	Serial.begin(38400); // Open serial connection to report values to host

	Serial.print( InfoStrings[0]);
	Serial.print( " " );
	Serial.println( InfoStrings[1]);

	delay( 2000 );

	InitUI();

	attachInterrupt(0, EncoderInterruptDelegate, CHANGE);
	attachInterrupt(1, EncoderInterruptDelegate, CHANGE);

	Serial.println("Ready...");

	UpdateSensor();

}

//  loop ////////////////////////////////////////////////////////////////////////////

int UpdateSensorInterval = 5; // seconds

void loop()
{
	static unsigned long prevUpdateSensor = 0;

	// handle UI events
	AnimateUI();

	// periodically update sensor data
	if( SelectedIndex == 0 )
	{
		unsigned long ms = millis();

		if( ms - prevUpdateSensor > UpdateSensorInterval * 1000 ){
			prevUpdateSensor = ms;
			UpdateSensor();
		}

		// operate regulators

		HumidityRegulator
			.NewReading( TheSensor.Humidity );

#if 0
		TemperatureRegulator
			.NewReading( TheSensor.Temp_f );
#else
		HeatingRegulator
			.NewReading( TheSensor.Temp_f );

		CoolingRegulator
			.NewReading( TheSensor.Temp_f );
#endif
	}
}

// Helper Routines ////////////////////////////////////////////////////////


// refresh sensor data

void UpdateSensor()
{
	TheSensor.GetMeasurement();

	// Print the values to the serial port
	Serial.print( "Temperature: " );
	Serial.print( TheSensor.Temp_f );
	Serial.print( "F / " );
	//Serial.print( temp_c );
	Serial.print( " Humidity: " );
	Serial.print( TheSensor.Humidity );
	Serial.println( "%    " );

#if 0
	HumidityRegulator.Print();
	Serial.println();
	SelectedTemperatureRegulator()->Print();
	Serial.println();
	Serial.println();
#endif

	UpdateHomePage();
}

// update primary display page

void UpdateHomePage()
{
	//UpdateSensor();

	Display.ShowTitle("Temp   Humidity");

    Display.MoveTempPos();
    Display.print( TheSensor.Temp_f, 1 );
    Display.print( "F" );
    Display.print(
		UpOrDownArrow(
			TheSensor.TempTrend()));
    Display.print(
		ActiveIndicator(
			HeatingRegulator.Active,
			CoolingRegulator.Active ) );
    Display.print( " " );

    Display.MoveHumidPos();
    Display.print( TheSensor.Humidity, 1 );
    Display.print( "%" );
    Display.print(
		UpOrDownArrow(
			TheSensor.HumidTrend()));
    Display.print(
		ActiveIndicator(
			HumidityRegulator.Active ));
    Display.print( " " );

	// ShowUpdating( false );
}

char UpOrDownArrow( SensorTrend trend )
{
	switch( trend ){
	case Rising:	return uparrow1;
	case Falling:	return downarrow1;
	default:			return ' ';
	}
}

char ActiveIndicator( bool active )
{
	return
		active
			? '+'
			: ' '
			;

}

char ActiveIndicator( bool heaterActive, bool coolerActive )
{
	if( heaterActive and coolerActive ) return '*';  // "cannot happen"
	if( heaterActive ) return '+';
	if( coolerActive ) return '-';

	return ' ' ;
}

 // Misc delegates and helper functions

void EncoderInterruptDelegate()
{
	Encoder.Poll();
}

 void HumidityActivator( byte onOff )
{
	ExpansionPort.SetDehumidifier( onOff );
}

void HeaterActivator( byte onOff )
{
	ExpansionPort.SetHeater( onOff );
}

void CoolerActivator( byte onOff )
{
	ExpansionPort.SetCooler( onOff );
}

void SetShiftRegister( int value )
{
	ExpansionPort.SetCounter( value );
}


