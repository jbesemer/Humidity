#ifndef SENSOR_H
#define SENSOR_H

#include <WProgram.h>
#include <Sensirion.h>

#include "Config.h"
#include "LcdDisplay.H"

enum SensorTrend {
	Unknown = 0,
	Falling = 1,
	Rising = 2,
};

class Sensor
	: public Sensirion
{
	SensorTrend Trend( float value, float prev_value );

public:

	float Temp_c;
	float Temp_f;
	float Humidity;
	float Prev_temp_f;
	float Prev_humidity;
	float DewPoint;

	SensorTrend TempTrend();
	SensorTrend HumidTrend();

	Sensor();

	void SetPower( bool on );

	void GetMeasurement();
};

extern Sensor TheSensor;


#endif
