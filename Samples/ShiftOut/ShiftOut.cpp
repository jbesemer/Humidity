/*	Copyright Lex Talionis, 2010
	
	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "WProgram.h"
#include "ShiftOut.h"

ShiftOut::ShiftOut(byte data, byte clock, byte latch)
{
  	pinMode(data, OUTPUT);				//3 output pins
	pinMode(latch, OUTPUT);
	pinMode(clock, OUTPUT);
	digitalWrite(data, LOW);		//everything idles low
	digitalWrite(latch, LOW); 
	digitalWrite(clock, LOW);
	_data = data;
	_latch = latch;
	_clock = clock;
}

void ShiftOut::shift(byte * myDataOut, byte count) //Writes data to the shift shield
{													//count is the number of shields, myDataOut is an array of count bytes
	pinMode(_clock, OUTPUT);
	pinMode(_data, OUTPUT);
	pinMode(_latch, OUTPUT);

	digitalWrite(_data, 0); // next relay state (High == on)
	digitalWrite(_clock, 0); // clock for relay states (TPIC6A596NE reads on upstroke of clock pin)
	digitalWrite(_latch, 0); // pushes state to the outputs	

	for (int j = count-1; j >=0; j--) 				//for each shift reg
	{
		for (int i=7; i >= 0; i--)					//send em out MSB first
		{
			digitalWrite(_clock, 0);				//zero the clock
			digitalWrite(_data, (myDataOut[j] & (1<<i)) && (1<<i));		//write our data to our datapin
			delayMicroseconds(SHIFT_DELAY);
			digitalWrite(_clock, 1);				//shifts bits on upstroke of clock pin  
			delayMicroseconds(SHIFT_DELAY);      
			digitalWrite(_data, 0);					//zero the data pin after shift to prevent bleed through
		}
	}
	
	digitalWrite(_clock, 0);					//stop shifting
	digitalWrite(_latch, HIGH);					//data is written when latch goes high
	delayMicroseconds(SHIFT_DELAY);	
	digitalWrite(_latch, LOW);					//latch idles low
}