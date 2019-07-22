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

	Much thanks to the original ShiftOut: http://www.arduino.cc/en/Tutorial/ShiftOut
*/


#include <ShiftOut.h>

ShiftOut shifty(13, 12, 8);

byte shieldCount = 1;
byte relayState[] = {0};

void setup()
{

}
void loop()
{
  for(byte i = 0; i != 8; i++)
  {
    relayState[0] = 1<<i;
    shifty.shift(relayState, 1);
    delay(500);
  }
}
