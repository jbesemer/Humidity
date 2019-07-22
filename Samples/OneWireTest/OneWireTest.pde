
////////////////////////////////////////////////////////////////////
// Test code to read temperature from a Maxim DS18B20 or DS18S20
// 1-wire device
////////////////////////////////////////////////////////////////////

//#include <WProgram.h>
#include "DS18S20.h"
#include "DS18B20.h"
#include "OneWireDefs.h"

// #define THERMOMETER DS18S20
#define THERMOMETER DS18B20

const int OneWirePin = 5;

// device( crcOn, useAddress, parasitic, mbed pin )
THERMOMETER* device;

void setup()
{
  Serial.begin(38400);

  Serial.println( "Initializing...");

  device = &THERMOMETER(true, true, false, OneWirePin );

    while (!device->initialize());    // keep calling until it works

  Serial.println( "Ready...");
}

void loop()
{
    device->readTemperature();
    delay(2000);
}






