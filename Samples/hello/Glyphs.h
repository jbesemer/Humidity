
#ifndef Glyphs_h
#define Glyphs_h

#include "WProgram.h"
#include <LiquidCrystal.h>

 
const char bang1 = (char)2;
const char bang2 = (char)3;
const char uparrow1 = (char)0;
const char downarrow1 = (char)1;
const char dotdotdot = (char)6;
const char tinyplus = (char)7;

// const char uparrow2 = (char)2;
// const char leftarrow1 = (char)4;

extern void InitGlyphs(LiquidCrystal* lcd);

#endif

