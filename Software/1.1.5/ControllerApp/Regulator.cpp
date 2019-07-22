#include "Regulator.h"

int ElapsedMinutesSince( unsigned long start );

// turn on device iff ambient exceeds upper limit value,
// try to maintain ambient maximum.
// e.g., air conditioner in summer or dehumidifier

void RegulatorMaximus::NewReading( byte value )
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

// turn on device iff ambient drops below lower limit value,
// try to maintain ambient minimum.
// e.g., heater in winter or lights when too hot

void RegulatorMinimus::NewReading( byte value )
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

// activate device if humidity exceeds upper limit value
// BUT NOT WHEN temperature exceeds a minimum threshold

void RegulatorMaximusOverTemp::NewReading( byte humidity, byte temperature )
{
	PrevValue = humidity;

	if( OverTemp )
	{
		if( temperature < Parameters->UnderTemp )
			OverTemp = false;
	}
	else
	{
		if( temperature > Parameters->OverTemp ){
			OverTemp = true;
			Deactivate();
		}
	}

	if( !OverTemp ){
		RegulatorMaximus::NewReading( humidity );
	}
}

// regulate ambient between upper/lower limits,
// e.g., heater in winter and AC in summer.

#if UNUSED	// never tested/debugged

void RegulatorMinMax::NewReading( byte value )
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
#endif // UNUSED

// common base class methods //////////////////////////////////////

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


