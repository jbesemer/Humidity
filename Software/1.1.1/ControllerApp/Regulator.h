
#ifndef REGULATOR_H
#define REGULATOR_H

#include <WProgram.h>

typedef void ActivatorDelegate( byte onOff );

struct RegulatorParameters
{
	int High;		// upper limit
	int Low;		// lower limit
	int MinOn;		// minimum minutes on per cycle
	int MinOff;		// minimum minutes off per cycle
};

// abstract base class for two flavors of specific regulators

class Regulator
{
public:

	char* Name;
	bool Active;
	int PrevValue;

	virtual void NewReading( int value ) = 0;
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

// regulate ambient below upper limit value,
// e.g., dehumidifier or air conditioner in summer

class RegulatorMaximus : public Regulator
{
public:

	RegulatorMaximus( char* name, RegulatorParameters* parameters, ActivatorDelegate* activator )
		: Regulator( name, parameters, activator )
	{
    }

	virtual void NewReading( int value );
};

// regulate ambient above lower limit value,
// e.g., heater in winter

class RegulatorMinimus: public Regulator
{
public:

	RegulatorMinimus( char* name, RegulatorParameters* parameters, ActivatorDelegate* activator )
		: Regulator( name, parameters, activator )
	{
	}

	virtual void NewReading( int value );
};

// regulate ambient above lower limit value,
// e.g., heater in winter

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

	virtual void NewReading( int value );

	void PrintHi();
	void PrintLo();
};
#endif

