
#include "Sensor.h"


Sensor::Sensor()
	: Sensirion( SensorDataPin, SensorClockPin )
{
	pinMode( SensorPowerPin, OUTPUT );
	SetPower( true );

	Temp_c
		= Temp_f
		= Humidity
		= Prev_temp_f
		= Prev_humidity
		= DewPoint
		= 0;
}

void Sensor::GetMeasurement()
{
	SetPower( true );
	delay( 10 );

	// save prev values
	Prev_temp_f = Temp_f;
	Prev_humidity = Humidity;

	// Read values from the sensor
	int err = measure( &Temp_c, &Humidity, &DewPoint );
	Temp_f = 32 + 9./5. * Temp_c;

	SetPower( false );
}


void Sensor::SetPower( bool on )
{
	digitalWrite(
		SensorPowerPin,
		( on
			? HIGH
			: LOW ));
}

SensorTrend Sensor::Trend( float value, float prev_value )
{
	if( prev_value == 0 )
		return Unknown;

	if( value > prev_value )
		return Rising;

	else
		return Falling;
}

SensorTrend Sensor::TempTrend()
{
	return Trend( Temp_f, Prev_temp_f );
}

SensorTrend Sensor::HumidTrend()
{
	return Trend( Humidity, Prev_humidity );
}
