#include "Regulator.h"


void RegulatorMaximus::NewReading( float value )
{
	if( value >= Parameters->High && !Active ){
		Active = true;
		(*Activator)( Active );
	}

	if( value <= Parameters->Low && Active ){
		Active = false;
		(*Activator)( Active );
	}
}

void RegulatorMinimus::NewReading( float value )
{
	if( value >= Parameters->High && Active ){
		Active = false;
		(*Activator)( Active );
	}

	if( value <= Parameters->Low && !Active ){
		Active = true;
		(*Activator)( Active );
	}
}

