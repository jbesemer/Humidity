#include "CRC16.h"


Crc16_Static Crc16::Static;

Crc16::Crc16()
{
	Reset();
}

void Crc16::Reset()
{
	Crc = 0;
}

// compute checksums -- for repeated computations

ushort Crc16::ComputeChecksum( int value )
{
	return ComputeChecksum( (byte*)&value, 0, sizeof( value ));
}

ushort Crc16::ComputeChecksum( byte bytes[], int count )
{
	return ComputeChecksum( bytes, 0, count );
}

ushort Crc16::ComputeChecksum( byte bytes[], int offset, int count )
{
	Reset();
	return AccumulateChecksum( bytes, offset, count );
}

// accumulate checksums -- for incremental computation

ushort Crc16::AccumulateChecksum( int value )
{
	return AccumulateChecksum( (byte*)&value, 0, sizeof( value ));
}

ushort Crc16::AccumulateChecksum( byte bytes[], int count )
{
	return AccumulateChecksum( bytes, 0, count );
}

ushort Crc16::AccumulateChecksum( byte bytes[], int offset, int count )
{
	for( ushort i = offset; i < count; ++i )
	{
		byte index = (byte)( Crc ^ bytes[ i ] );
		Crc = (ushort)( ( Crc >> 8 ) ^ Static.Table[ index ] );
	}

	return Crc;
}


// "static" class creates constant table

const int Crc16_Static::TableLength = 256;
const ushort Crc16_Static::Polynomial = 0xA001;

Crc16_Static::Crc16_Static()
{
	ushort value;
	ushort temp;

	for( ushort i = 0; i < TableLength; ++i )
	{
		value = 0;
		temp = i;
		for( byte j = 0; j < 8; ++j )
		{
			if( ( ( value ^ temp ) & 0x0001 ) != 0 )
			{
				value = (ushort)( ( value >> 1 ) ^ Polynomial );
			}
			else
			{
				value >>= 1;
			}
			temp >>= 1;
		}
		Table[ i ] = value;
	}
#if DUMP_CRC_TABLE && DEBUG
	using( TextWriter writer = File.CreateText( "table.txt" ) )
	{
		for( ushort i = 0; i < TableLength; i++ )
			writer.WriteLine( "Tab[{0}] = {1:X2}", i, Table[ i ]);
	}
#endif
}

