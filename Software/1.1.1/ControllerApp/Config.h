
// Config.h -- global configuration switch and options

#ifndef INCLUDED_CONFIG_H
#define INCLUDED_CONFIG_H

#define PROTOTYPE 1  // true iff building for prototype; false for production

#define TRACE_ENCODER 0	// trace encoder interrupts

#if PROTOTYPE

const byte SensorDataPin = 6;  // 2.red
const byte SensorClockPin = 7; // 3.orange
const byte SensorPowerPin = 8;  // yellow / purple

#else // PRODUCTION

const byte SensorDataPin = 6;  // 2.red
const byte SensorClockPin = 7;  // 3.orange
const byte SensorPowerPin = 8;  // yellow / purple



#endif // PROTOTYPE

#endif // included this file

