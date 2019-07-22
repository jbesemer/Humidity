
#ifndef REGULATOR_H
#define REGULATOR_H

#include "WConstants.h"

typedef void ActivatorFn( byte onOff );

struct RegulatorParameters
{
	int High;
	int Low;
	int MinOn;
	int MinOff;
};

// abstract class for two flavors of specific regulators

class Regulator
{
protected:

	RegulatorParameters* Parameters;
	ActivatorFn* Activator;
	bool Active;

	Regulator( RegulatorParameters* parameters, ActivatorFn* activator ){
		Parameters = parameters;
		Activator = activator;
		Active = false;
	}

public:

	virtual void NewReading( float value ){}
};

// regulate upper limit value, e.g., dehumidifier or air conditioner in summer

class RegulatorMaximus : public Regulator
{
public:
	RegulatorMaximus( RegulatorParameters* parameters, ActivatorFn* activator )
          : Regulator( parameters, activator )
        {
        }

	void NewReading( float value );
};

// regulate lower limit balue, e.g., heater in winter

class RegulatorMinimus: public Regulator
{
public:
	RegulatorMinimus( RegulatorParameters* parameters, ActivatorFn* activator )
          : Regulator( parameters, activator )
        {
        }

	void NewReading( float value );
};

#endif

