/*
* DebugTrace. Allows dumping debug messages/values to serial or
* to file.
*
* Copyright (C) <2009> Petras Saduikis <petras@petras.co.uk>
*
* This file is part of DebugTrace.
*
* DebugTrace is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* DebugTrace is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with DebugTrace.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <WProgram.h>
#include "DebugTrace.h"

DebugTrace::DebugTrace(eLog on, eLogTarget mode ) :
  enabled(on),
  logMode(mode)
{
}

void DebugTrace::traceOut(const char* text )
{
    if (enabled)
    {
	   Serial.print( text );
    }
}

void DebugTrace::traceOut(const char* text, float num )
{
    if (enabled)
    {
		Serial.print( text );
		Serial.println( num );
    }
}

void DebugTrace::traceOut(int num)
{
    if (enabled)
    {
		Serial.print( num );
    }
}

void DebugTrace::traceOut(int num, int format)
{
    if (enabled)
    {
		Serial.print( num, format );
    }
}

void DebugTrace::traceOutAddress(byte* address)
{
    if (enabled)
    {
		int i;
		for( i=0; i<8; i++ ){
			int num = address[ i ];
			Serial.print(( num >> 4 ) & 0xF, HEX);
			Serial.print( num & 0xF, HEX);
			Serial.print( " " );
		}
    }
}


