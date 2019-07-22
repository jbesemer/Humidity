
#ifndef INCLUDED_UI_H
#define INCLUDED_UI_H

#include "Config.h"
#include "LcdDisplay.h"
#include "RotaryEncoder.h"
#include "Regulator.h"

#define ButtonPin	4

// exports
extern void InitUI();
extern void AnimateUI();

extern void SaveAllSettingsToEEPROM();
extern void LoadAllSettingsFromEEPROM();

extern char SelectedIndex;

extern RegulatorParametersWithOverTempCutoff HumidityParameters;
extern RegulatorParameters HeatingParameters;
extern RegulatorParameters CoolingParameters;
#if LIGHTING_FAIL_SAFE
extern RegulatorParameters LightingParameters;
#endif

extern void SetDisplayIntensity( byte pwm );
extern void SetDisplayContrast( byte value );

// imports

extern char* InfoStrings[] ;

extern LcdDisplay Display;
extern RotaryEncoder Encoder;

extern void UpdateHomePage();
extern void SetShiftRegister( int value );

extern byte UpdateSensorInterval;
//extern int TemperatureOperatingMode;


#endif
