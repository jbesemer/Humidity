
PGM_P STR_Run  = "Run";
PGM_P STR_Info  = "Info";

PGM_P STR_HumidHigh  = "Humid High";
PGM_P STR_HumidLow  = "Humid Low";
PGM_P STR_HumidMinOn  = "Humid Min On";
PGM_P STR_HumidMinOff  = "Humid Min Off";
PGM_P STR_HumidHiTemp  = "Humid Hi Temp";
PGM_P STR_HumidLoTemp  = "Humid Lo Temp";

PGM_P STR_HeatHigh  = "Heat High";
PGM_P STR_HeatLow  = "Heat Low";
PGM_P STR_HeatMinOn  = "Heat Min On";
PGM_P STR_HeatMinOff  = "Heat Min Off";

PGM_P STR_CoolHigh  = "Cool High";
PGM_P STR_CoolLow  = "Cool Low";
PGM_P STR_CoolMinOn  = "Cool Min On";
PGM_P STR_CoolMinOff  = "Cool Min Off";

#if LIGHTING_FAIL_SAFE
PGM_P STR_LightHigh  = "Light High";
PGM_P STR_LightLow  = "Light Low";
PGM_P STR_LightMinOn  = "Light Min On";
PGM_P STR_LightMinOff  = "Light Min Off";
#endif

PGM_P STR_SensorInterval  = "Sensor Interval";
PGM_P STR_BrightLevel  = "Bright Level";
PGM_P STR_DimLevel  = "Dim Level";

#if PROTOTYPE
PGM_P STR_Contrast  = "Contrast";
#endif

// table of string addresses in FLASH

PGM_P StringTable[] PROGMEM = {
	STR_Run,
	STR_Info,

	STR_HumidHigh,
	STR_HumidLow,
	STR_HumidMinOn,
	STR_HumidMinOff,
	STR_HumidHiTemp,
	STR_HumidLoTemp,

	STR_HeatHigh,
	STR_HeatLow,
	STR_HeatMinOn,
	STR_HeatMinOff,

	STR_CoolHigh,
	STR_CoolLow,
	STR_CoolMinOn,
	STR_CoolMinOff,

#if LIGHTING_FAIL_SAFE
	STR_LightHigh,
	STR_LightLow,
	STR_LightMinOn,
	STR_LightMinOff,
#endif

	STR_SensorInterval,
	STR_BrightLevel,
	STR_DimLevel,
#if PROTOTYPE
	STR_Contrast,
#endif
};

