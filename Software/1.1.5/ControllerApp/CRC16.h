#include "Arduino.h"

// #define DUMP_CRC_TABLE

#ifndef CRC16_H_INCLUDED
#define CRC16_H_INCLUDED


typedef unsigned int ushort;

class Crc16_Static
{
public:

	static const ushort Polynomial;
	static const int TableLength;

	ushort Table[ 256 ];

	// static constructor creates constant table

	Crc16_Static();
};


	// static constructor creates constant table
class Crc16
{
protected:

	static Crc16_Static Static;

public:

	ushort Crc;		// previous result

	Crc16();			// constructor

	void Reset();	// reset accumulated results

	// compute checksums -- for repeated computations

	ushort ComputeChecksum( int value );
	ushort ComputeChecksum( byte bytes[], int count );
	ushort ComputeChecksum( byte bytes[], int offset, int count );

	// accumulate checksums -- for incremental computation

	ushort AccumulateChecksum( int value );
	ushort AccumulateChecksum( byte bytes[], int count );
	ushort AccumulateChecksum( byte bytes[], int offset, int count );
};


#endif // CRC16_H_INCLUDED
