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

#ifndef SNATCH59_DEBUGTRACE_H
#define SNATCH59_DEBUGTRACE_H

enum eLog {OFF, ON};
enum eLogTarget {TO_SERIAL, TO_FILE};

class DebugTrace
{
public:
    DebugTrace(eLog on, eLogTarget mode );

    void traceOut(const char* text );
    void traceOut(const char* text, float num );
    void traceOut(int num);
    void traceOut(int num, int format);
	void traceOutAddress( byte* address);

private:
    eLog enabled;
    eLogTarget logMode;
};

#endif
