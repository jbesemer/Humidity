


 int GetCount()
 {
   noInterrupts();
   
   // each call to GetCount reports the accumulated changes since the last call
   int count = Counter;
   
   Counter = 0;
   
   interrupts();
   
   return count;
 }
 
enum EncoderStates { Idle, R1, R2, R3, L1, L2, L3 };

const byte Phase0 = 0x3;
const byte Phase1 = 0x2;
const byte Phase2 = 0x0;
const byte Phase3 = 0x1;

unsigned long EncoderStart;
unsigned long EncoderFinish;

void EncoderInterruptHandler()
{
	static byte state = Idle;

        if( state == Idle ){
          //EncoderStart = micros();
        }
          
	byte eA = digitalRead(EncoderA);
	byte eB = digitalRead(EncoderB);

	byte AB = ( eA << 1 ) | eB;

		switch( AB ){
		case Phase0:
			switch(state){
                        case R1:
                        case L1: 
                          break;
			case R3:	
			  Counter--;	
                          //state = Idle;
                          //EncoderFinish = micros();
                          break;
			case L3:	
			  Counter++;	
                          //EncoderFinish = micros();
                           //state = Idle;
                            break;
			}
                        state = Idle;
			break;

		case Phase1:
			switch(state){
                        case R1:    break;
                        case R2:    break;
                        case L3:    break;
			case Idle:	state = R1;	break;
			case L2:	state = L3;	break;
			default:	state = Idle;	break;
			}
			break;

		case Phase2:
			switch(state){
                        case R2:    break;
                        case R3:    break;
                        case L2:    break;
                        case L3:    break;
			case R1:	state = R2;	break;
			case L1:	state = L2;	break;
			default:	state = Idle;	break;
			}
			break;

		case Phase3:
			switch(state){
                        case L1:    break;
                        case L2:    break;
                        case R3:    break;
			case Idle:	state = L1;	break;
			case R2:	state = R3;	break;
			default:	state = Idle;	break;
			}
			break;
		}


}


