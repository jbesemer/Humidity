
typedef uint8_t byte;

//#include <wiring.h>


class Encoder
{
	byte PinA;
	byte PinB;
	byte State;
	byte A, B, AB;
	byte Counter;

public:

	Encoder( byte pinA, byte pinB );
	void Reset();
	byte GetCount();

	void InterruptHandler();

	virtual void Clockwise();
	virtual void CounterClockwise();
}
