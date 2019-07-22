
#ifndef REGULATOR_H
#define REGULATOR_H

#include "Arduino.h"

typedef void ActivatorDelegate( byte onOff );

struct RegulatorParameters
{
	byte High;		// upper limit
	byte Low;		// lower limit
	byte MinOn;		// minimum minutes on per cycle
	byte MinOff;		// minimum minutes off per cycle
};

struct RegulatorParametersWithOverTempCutoff
{
	RegulatorParameters Parameters;

	byte OverTemp;		// over temp cutoff
	byte UnderTemp;	// resume normal operations
};

// abstract base class for two flavors of specific regulators

class Regulator
{
public:

	char* Name;
	bool Active;
	int PrevValue;

	void Print();

protected:

	RegulatorParameters* Parameters;
	ActivatorDelegate* Activator;

	Regulator( char* name, RegulatorParameters* parameters, ActivatorDelegate* activator ){
		Name = name;
		Activator = activator;
		Parameters = parameters;

		Active = false;
		ActivationTime = 0;
		DeactivationTime = 0;
	}

	unsigned long ActivationTime;
	unsigned long DeactivationTime;

	bool ActiveLongEnough();
	bool InactiveLongEnough();

	void Activate();
	void Deactivate();
};

// turn on device iff ambient exceeds upper limit value,
// try to maintain ambient maximum.
// e.g., air conditioner in summer or dehumidifier

class RegulatorMaximus : public Regulator
{
public:

	RegulatorMaximus( char* name, RegulatorParameters* parameters, ActivatorDelegate* activator )
		: Regulator( name, parameters, activator )
	{
    }

	void NewReading( byte value );
};

// turn on device iff ambient drops below lower limit value,
// try to maintain ambient minimum.
// e.g., heater in winter or lights when too hot

class RegulatorMinimus: public Regulator
{
public:

	RegulatorMinimus( char* name, RegulatorParameters* parameters, ActivatorDelegate* activator )
		: Regulator( name, parameters, activator )
	{
	}

	void NewReading( byte value );
};

// activate device if humidity exceeds upper limit value
// BUT NOT WHEN temperature exceeds a minimum threshold

class RegulatorMaximusOverTemp : public RegulatorMaximus
{
public:
	RegulatorParametersWithOverTempCutoff* Parameters;
	bool OverTemp;

	RegulatorMaximusOverTemp( char* name, RegulatorParametersWithOverTempCutoff* parameters, ActivatorDelegate* activator )
		: RegulatorMaximus( name, &parameters->Parameters, activator )
	{
		Parameters = parameters;
		OverTemp = false;
    }

	void NewReading( byte humidity, byte temperature );
};

// regulate ambient between two extremese,

#if UNUSED

class RegulatorMinMax: public RegulatorMaximus, public RegulatorMinimus
{
public:

	RegulatorMinMax(
			char* nameHi,
			char* nameLo,
			RegulatorParameters* parameters,
			ActivatorDelegate* activatorHi ,
			ActivatorDelegate* activatorLo )
		: RegulatorMaximus( nameHi, parameters, activatorHi )
		, RegulatorMinimus( nameLo, parameters, activatorLo )
	{
	}

	void NewReading( byte value );

	void PrintHi();
	void PrintLo();
};

#endif // UNUSED
#endif

