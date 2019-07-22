
// This class works for the Rotary Encoder from SparkFun.com,
// sku: COM-09117
// http://www.sparkfun.com/products/9117
//
// I'm sure it's applicable to other similar devices.
//
// The encoder also has a "button" input -- which presently is NOT
// handled by this class.

#include "RotaryEncoder.h"

// Theory of operation
//
// The encoder when rotated outputs a sequence of grey codes on it's
// two output pins to represent the position of the knob as it advances.
// The phase relationship of the signals indicates the direction of
// rotation and the number of complete cycles indicates how many
// "clicks" the knob has advanced.
//
// Clockwise
//	    _______           _______________
//	A	       X_________X
//      ___________            __________
//	B	           X__________X
//
// Counter-Clockwise
//	    _____________           _________
//	A	             X_________X
//      _______            ______________
//	B	       X__________X
//
// Unfortunately, the contacts are rather noisy, so the signals must be
// "debounced".  The "X"s in the above diagrams illustrate the presence
// of noise in the signals -- that is places where the signal jitters
// rapidly between HIGH and LOW.  This greatly confounds recognition of
// the encoded information.
//
// Observe that, all told, there are only 4 possible states that the
// input signals may be in at any point in time, as follows:

const byte Phase0 = 0x3;	// HIGH HIGH
const byte Phase1 = 0x2;	// HIGH LOW
const byte Phase2 = 0x0;	// LOW  LOW
const byte Phase3 = 0x1;	// LOW  HIGH

// Furthermore, observe that these states appear in a definite sequence
// -- even in the presence of noise!  This suggests that we may employ
// a state machine to track the states of the encoder and thus determine
// each time a particular clockwise or counter-clockwise sequence is
// complete.
//
// We define 7 states, as follows (since clockwise and counter-clockwise
// both start with "C" we substitute R and L for right and left):

enum EncoderStates {
	Idle, 	// Phase0
	R1, 	// Phase1 following Phase0, begin clockwise
	R2, 	// Phase2 following Phase1	halfway clockwise
	R3, 	// Phase3 following Phase2, penultimate clockwise
	L1, 	// Phase3 following Phase0, begin counter-clockwise
	L2, 	// Phase2 following Phase3	halfway counter-clockwise
	L3 		// Phase1 following Phase2, penultimate counter-clockwise
};

// Most of the time the encoder is stationary at a detent, and the inputs
// show Phase0, which we call the Idle state.
//
// In normal operation the state machine follows the encoder through
// one of the following two possible sequences:
//
//		Idle -> R1 -> R2 -> R3 -> Idle/Accept
//		Idle -> L1 -> L2 -> L3 -> Idle/Accept
//
// States R3 and L3 are penultimate states, and a subsequent Phase0 input
// causes the state machine to recognize one complete clockwise or
// counter-clockwise cycle of the encoder.  The direction is determined
// by the state the machine was in just before it sees the Idle.
//
// Another important observation is that the phase relationship of the
// encoder signals means that noise appears only on one input at a time.
// Thus input A is stable while B is noisy and B is stable when A is
// noisy.  This reduces the possible inputs in each state.  The presence
// of noise on just one input causes the state machine to jitter back
// and forth between two states.  But in normal operation it will
// eventually settle into the new state well before it begins
// to see any transitions on the other input, leading to the next
// transition.
//
// Thus a more accurate depiction of the state diagram above is
// as follows:
//
//		Idle <-> R1 <-> R2 <-> R3 -> Idle/Accept
//		Idle <-> L1 <-> L2 <-> L3 -> Idle/Accept
//
// Now it is possible in practice to see encoder inputs that do not
// match either of the two main sequences.  E.g., you can rotate the
// encoder part way through a cycle then reverse direction.  If we see
// any inputs that do not match the expected normal patterns, then the
// state machine blithely ignores them, returning to the Idle state,
// and presumably eventually seeing a normal sequence.

// The class was designed with the expectation that the Poll method will
// be called from interrupt handlers connected to pins 2 and 3.  Thus it
// gets called for each transition on either of the input lines.  If you
// don't use interrupts, then you must call the Poll method frequently
// enough so that no important input states are missed.  See the example
// usage code at the bottom of this file for details.

// Finally, the encoder class maintains a count of accumulated rotations,
// positive for clockwise and negative for counter-clockwise.  Your app
// then may check on the counter at it's leisure from its main loop and
// respond to any accumulated encoder turns.  If polling occurrs rapidly
// enough then the counter will always be one of: +1, 0, or -1.

// Note that herein, "clockwise" and "counter-clockwise" are arbitrary.
// Actual behavior is entirely dependent on how you wire your encoder
// to the board.  If the net behavior is backwards from what you expect,
// options are (a) swap the encoder wires, (b) swap the pin assignments
// when instantiating the encoder, and (c) reverse your interpretation
// of the net count output from this class.

// Finally, the logic herein presumes that you enable the internal pull-ups
// in the intputs, and that the encoder switch contacts short those inputs
// to ground.  The state machine probably will need to be revised if you use
// some other technique (e.g., pull-down resistors, pulled up by encoder contacts).
// I also wire 100pF caps across both contact leads to reduce some of the noise.


RotaryEncoder::RotaryEncoder( byte pinA, byte pinB )
{
	// remember pin assignments
	PinA = pinA;
	PinB = pinB;

	// configure input pins
	pinMode(PinA, INPUT);
	digitalWrite(PinA, HIGH);
	pinMode(PinB, INPUT);
	digitalWrite(PinB, HIGH);
	// pinMode(Button, INPUT);
	// digitalWrite(Button, HIGH);

	// init counter and state machine
	Counter = 0;
	State = Idle;
}

// each call to GetCount reports the net accumulated rotations
// since the previous call

int RotaryEncoder::GetCount()
 {
	// interrupts are suspended briefly so no counts get lost between
	// reading the counter and clearing it.
	noInterrupts();

	int count = Counter;

	Counter = 0;

	interrupts();

	return count;
 }


// read inputs and advance state machine

void RotaryEncoder::Poll2()
{

	if( State == Idle ){
	  //EncoderStart = micros();
	}

	byte eA = digitalRead(PinA);
	byte eB = digitalRead(PinB);

	byte AB = ( eA << 1 ) | eB;

	switch( AB ){
	case Phase0:
		switch(State){
		case R1:
		case L1:
			break;
		case R3:
			Counter--;
			//EncoderFinish = micros();
			break;
		case L3:
			Counter++;
			//EncoderFinish = micros();
			break;
		}
		State = Idle;
		break;	// end case Phase0

	case Phase1:
		switch(State){
		case R1:
		case R2:
		case L3:
			break;
		case Idle:	State = R1;	break;
		case L2:	State = L3;	break;
		default:		State = Idle;	break;
		}
		break;	// end case Phase1

	case Phase2:
		switch(State){
		case R2:
		case R3:
		case L2:
		case L3:
			break;
		case R1:	State = R2;	break;
		case L1:	State = L2;	break;
		default:	State = Idle;	break;
		}
		break;	// end case Phase2

	case Phase3:
		switch(State){
		case L1:
		case L2:
		case R3:
			break;
		case Idle:	State = L1;	break;
		case R2:	State = R3;	break;
		default:	State = Idle;	break;
		}
		break;	// end case Phase3
	}
}


void RotaryEncoder::Poll()
{
	int eA = digitalRead(PinA);
	int eB = digitalRead(PinB);

	byte AB = ( eA << 1 ) | eB;

#if TRACE_ENCODER
	Serial.print("Encoder: ");
	Serial.print( eA );
	Serial.println( eB );
#endif

	// successful cases return.
	// fall-thru cases enter the idle state

	switch( State ){
	case Idle:
		//EncoderStart = micros();
		switch( AB ){
		case Phase1:	State = R1;		return;
		case Phase3:	State = L1;		return;
		}
		break;	// remain Idle

	case R1:
		switch( AB ){
		// case Phase0:	State = Idle;		return;
		case Phase2:	State = R2;			return;
		case Phase1:	/*stay in R1*/	return;
		}
		break;	// become Idle

	case R2:
		switch( AB ){
		case Phase1:	State = R2;			return;
		case Phase3:	State = R3;			return;
		case Phase2:	/*stay in R2*/	return;
		}
		break;	// become Idle

	case R3:
		switch( AB ){
		case Phase2:	State = R2;		return;
		case Phase3:	/*stay in R3*/	return;
		case Phase0:	// ACCEPT CLOCKWISE
			Counter--;
			//EncoderFinish = micros();
			// Fall-Thru to IDLE
		}
		break;	// become Idle

	case L1:
		switch( AB ){
		// case Phase0:	State = Idle;	return;
		case Phase3:	/*stay in L1*/	return;
		case Phase2:	State = L2;		return;
		}
		break;	// become Idle

	case L2:
		switch( AB ){
		case Phase3:	State = L1;		return;
		case Phase2:	/*stay in L2*/	return;
		case Phase1:	State = L3;		return;
		}
		break;	// become Idle

	case L3:
		switch( AB ){
		case Phase2:	State = L2;		return;
		case Phase1:	/*stay in L3*/	return;
		case Phase0:	// ACCEPT COUNTER-CLOCKWISE
			Counter++;
			//EncoderFinish = micros();
			// Fall-Thru to IDLE
		}
		break;	// become Idle
	}

	// cases not handled above fall-thru to here and thus become Idle state

	State = Idle;
}


// EXAMPLE USAGE ////////////////////////////////////

#if false

// include class definition
#include "RotaryEncoder.h"

// define encoder pins
const int EncoderA = 3;
const int EncoderB = 2;

// instantiate Encoder
RotaryEncoder Encoder( EncoderA, EncoderB );

void setup(){
	// setup encoder interrupts
    attachInterrupt( 0, EncoderInterruptDelegate, CHANGE );
    attachInterrupt( 1, EncoderInterruptDelegate, CHANGE );
}

// Redirect interrupt function call to Encoder instance method call
void EncoderInterruptDelegate()
{
	Encoder.Poll();
}

#endif
