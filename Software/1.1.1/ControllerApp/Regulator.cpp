#include "Regulator.h"

int ElapsedMinutesSince( unsigned long start );

// regulate ambient below upper limit value,
// e.g., air conditioner in summer or dehumidifier

void RegulatorMaximus::NewReading( int value )
{
	PrevValue = value;

	if( Active )
	{
		if( value < Parameters->Low  && ActiveLongEnough())
			Deactivate();
	}
	else
	{
		if( value > Parameters->High && InactiveLongEnough())
			Activate();
	}
}

// regulate ambient above lower limit value,
// e.g., heater in winter

void RegulatorMinimus::NewReading( int value )
{
	PrevValue = value;

	if( Active )
	{
		if( value > Parameters->High && ActiveLongEnough())
			Deactivate();
	}
	else
	{
		if( value < Parameters->Low && InactiveLongEnough())
			Activate();
	}
}

void Regulator::Activate()
{
	Serial.print( "Activate: " );
	Serial.print( Name );
	Serial.print( " at " );
	Serial.println( PrevValue );

	Active = true;
	(*Activator)( Active );
	ActivationTime = millis();
}

void Regulator::Deactivate()
{
	Serial.print( "Deactivate: " );
	Serial.print( Name );
	Serial.print( " at " );
	Serial.println( PrevValue );

	Active = false;
	(*Activator)( Active );
	DeactivationTime = millis();
}

bool Regulator::ActiveLongEnough()
{
	return Parameters->MinOn == 0
		|| ElapsedMinutesSince( ActivationTime ) >= Parameters->MinOn;
}

bool Regulator::InactiveLongEnough()
{
	return Parameters->MinOff == 0
		|| ElapsedMinutesSince( DeactivationTime ) >= Parameters->MinOff;
}

// regulate ambient between upper/lower limits,
// e.g., heater in winter and AC in summer.

void RegulatorMinMax::NewReading( int value )
{
	RegulatorMaximus::PrevValue
	  = RegulatorMinimus::PrevValue
          = value;

	// Maximus => enforce maximum, e.g., AC

	if( RegulatorMaximus::Active )
	{
		if( value < RegulatorMaximus::Parameters->High
		&& RegulatorMaximus::ActiveLongEnough())
			RegulatorMaximus::Deactivate();
	}
	else
	{
		if( value > RegulatorMaximus::Parameters->High
		&& RegulatorMaximus::InactiveLongEnough())
			RegulatorMaximus::Activate();
	}

	// ActiveLo => enforce minimum, e.g., Heater

	if( RegulatorMinimus::Active )
	{
		if( value > RegulatorMinimus::Parameters->Low
		&& RegulatorMinimus::ActiveLongEnough())
			RegulatorMinimus::Deactivate();
	}
	else
	{
		if( value < RegulatorMinimus::Parameters->Low
		&& RegulatorMinimus::InactiveLongEnough())
			RegulatorMinimus::Activate();
	}
}

int ElapsedMinutesSince( unsigned long start )
{
	const unsigned long UInt32Max = 0xFFFFFFFFUL;
	unsigned long stop = millis();
	unsigned long delta;

	if( stop < start )
	{
		// the counter wrapped around, we'll ASSUME just once (less than 49 days),
		// in which case start-stop is the amount less than Int32.MaxInt that has elapsed.

		delta = UInt32Max + start - stop;
	}
	else
	{
		delta = stop - start;
	}

	return delta / 60000;	// ms to minutes
}


void Regulator::Print()
{
	Serial.print( Name );
	Serial.print( " A: " );
	Serial.print( Active );
	Serial.print( " P:" );
	Serial.print( PrevValue );
	Serial.print( " H: " );
	Serial.print( Parameters->High );
	Serial.print( " L: " );
	Serial.print( Parameters->Low );
	Serial.print( " " );
}


