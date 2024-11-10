// ��ó :https://webnautes.tistory.com/967 
// :Ÿ�̸Ӹ� ���������� ������ ������ �Է½�ȣ�� int�� �޾� ��ȣ�� �ľ��ϴ� ������ ���δ�.
//--------------------------------------------
//                  +-----+     +
//  ���۾˸� : _____|     |_____|
//   13.5             9ms  4.5ms
//
//                  +-----+  +-+
//  �ݺ���ȣ : _____|     |__| |____________________________________________
//   108ms            9ms+2.25ms+0.56ms+ 96.19
//
//              +-+   +
//  data 0   : _| |___|  
//   1.125    0.56  0.565
//
//              +-+      +
//  data 1   : _| |______|  
//   2.25      0.56  1.69
//
//--------------------------------------------
// 1�ֱ� = 108 msec
//   ���۾˸� : 13.5 msec = 9+4.5
//   ADDRESS  : 27 msec   =  8*((0.56+0.565)+(0.56+1.69)) : 8bit * (data0 + data1)
//   DATA     : 27 msec   =  8*((0.56+0.565)+(0.56+1.69)) : 8bit * (data0 + data1)
//   ���     : 108 - (���۾˸�+ADDRESS+DATA)
//


//==================================
//  include
//==================================
#include <stdio.h>
#include <stdint.h>

#include "ir_nec.h"

//==================================
//  define
//==================================
//States
#define IR_VALIDATE_LEAD_HIGH 0   // 
#define IR_VALIDATE_LEAD_LOW  1
#define IR_RECEIVE_BITS       3
#define IR_WAIT_STOP_BIT      4
 
 
//Timer
#define IR_900         900    // Leader High 9000 uSec
#define IR_900_MAX     990
#define IR_900_MIN     810

#define IR_450         450    // Leader Low  4500 uSec
#define IR_450_MAX     495
#define IR_450_MIN     405

#define IR_225         215    // 225    // Leader Repeate Low 2250
#define IR_225_MAX     235    // 245
#define IR_225_MIN     195    // 205

#define IR_56           66    // 56    // Bit High, Leader Repeate Hi, Bit_0
#define IR_56_MAX       82    // 72
#define IR_56_MIN       50    // 40

#define IR_56D          45    // 56    // Bit High, Leader Repeate Hi, Bit_0
#define IR_56D_MAX      60    // 62
#define IR_56D_MIN      30    // 50

#define IR_169         160    //169    // Bit_1
#define IR_169_MAX     175    //186
#define IR_169_MIN     145    //152

#define QMAX             8    //Size of the Remote command buffer
#define RC_NONE        255    //This val is returned by GetRemoteCmd when no key is pressed

//==================================
// ���� ����
//==================================
// IR 
static IrCallback _getIR = NULL;  // ���������� ir�� ������ ȣ���ϴ� �Լ� ������

//Globals
volatile unsigned int Time;        //Main timer, stores time in 10us,
//Updated by ISR(TIMER0_COMP)
volatile unsigned int BitNo;        //Pos of next BIT
volatile unsigned int ByteNo;        //Pos of current Byte
 
         unsigned char IrData[4];    //The four data Bytes of Ir Packet
//2-Byte Address 2-Byte Data
volatile unsigned char IrCmdQ[QMAX];//Final Command Received (Buffer)
 
volatile unsigned int PrevCmd;        //Used for repeat
 
//Variables used for start repeating only after a key is pressed for certain time
 
volatile unsigned int Repeat;        //1=yes 0=no
volatile unsigned int RCount;        //Repeat count
 
volatile int QFront =-1;
volatile int QEnd   =-1;
volatile unsigned int State;        //State of receiver
volatile unsigned int Edge;         //Edge of interrupt [ RISING=1 OR FALLING=0 ]
volatile unsigned int stop;

/**********************************************************************************************/
/*                F U N C T I O N S   S T A R T S                                                  */
/**********************************************************************************************/

void ResetIR()
{
	char i;
	for(i=0;i<4;i++) IrData[i]=0;

	State = IR_VALIDATE_LEAD_HIGH;
	Edge=0;
 
	//INT ON FALLING EDGE
	cbIR_FallingEdge();
}

unsigned char GetRemoteCmd(char wait)
{
    unsigned char cmd;
 
    if(wait) while(QFront==-1);
    else     if(QFront==-1) return (RC_NONE);
 
    cmd = IrCmdQ[QFront];
 
    if(QFront==QEnd)
    QFront=QEnd=-1;
    else
    {
        if(QFront==(QMAX-1))
        QFront=0;
        else
        QFront++;
    }
 
    return cmd;
}
 
//cmd=GetRemoteCmd(1);

// �ð�����
// u:10usec
// ���۾˸�H : 900u  : 9    msec
// ���۾˸�L : 450u  : 4.5  msec
// repeat  L : 225u  : 2.25 msec
//           :  60u  : 0.56 msec
// 	��Ʈ 1 L : 169u  : 1.69 msec

// State :
// ����: IR_VALIDATE_LEAD_HIGH
// IR_VALIDATE_LEAD_HIGH
//   -> count�� 900u �̸� State=IR_VALIDATE_LEAD_LOW (���۾˸�H�� �Ϸ�Ȼ���)
// IR_VALIDATE_LEAD_LOW
//   -> count�� 450u �̸� State=IR_RECEIVE_BITS (���ۻ���)
//   -> count�� 225u �̸� ����Ʈ ���� (�м��� ���ؾ� �Ѵ�.)
// IR_RECEIVE_BITS : ��Ʈ���Ż���
//   -> Edge=H�̰� count��  60u �̸� Edge=0 (low������)
//   -> Edge=L�̰� count��  60u �̸� ��Ʈ��=0
//   -> Edge=L�̰� count�� 169u �̸� ��Ʈ��=1, ��Ʈ��8�̸� ����Ʈ�� 4�̸� State=IR_WAIT_STOP_BIT;
// IR_WAIT_STOP_BIT
//   Edge-1�̸� �м��� ��

void IrNecProc(uint32_t count)
{
	switch(State){
		case IR_VALIDATE_LEAD_HIGH:{
			if(Edge){
				// rising�϶�
				//Rising
				if((IR_900_MIN<count) && (count<(IR_900_MAX))){
					//Lead High Correct
					State=IR_VALIDATE_LEAD_LOW;
					//INT ON FALLING EDGE
					cbIR_FallingEdge();
					Edge=0;
				}
				else{
					ResetIR();
				}
			}
            else {
				//Falling
				//Set INT on Rising Edge
				cbIR_RisingEdge();
				Edge=1;
			}
			break;
		}
		case IR_VALIDATE_LEAD_LOW:
		{
			if((IR_450_MIN < count) && (count<IR_450_MAX)){
                //Got a valid leader
                State=IR_RECEIVE_BITS;
                BitNo  = 0;
                ByteNo = 0;
                Edge=1;
				cbIR_RisingEdge();
				break;
            }
            else if((IR_225_MIN<count) && (count<IR_225_MAX))
            {
				//tttttttttttttttttttttttttttt
				if(_getIR != NULL){
					IrData[2]=0; IrData[3]=0;
					_getIR(IrData);
				}
				//oooooooooooooooooooooooooooo
                /*
				if(Repeat)
                {
                    //Got a repeat pulse
                    if((QEnd==(QMAX-1) && QFront==0)||((QEnd+1)==QFront))
                    {
                        QFront++;
                        if(QFront==(QMAX))
                        QFront=0;
                    }
 
                    if(QEnd==(QMAX-1))
                    QEnd=0;
                    else
                    QEnd++;
 
                    IrCmdQ[QEnd]=PrevCmd;
 
                    if(QFront==-1) QFront=0;
                }
                else
                {
                    RCount++;
                    if(RCount==4) Repeat=1;
                }
				*/
				ResetIR();
				//eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee
            }
			else{
				ResetIR();
			}
			break;
        }
        case IR_RECEIVE_BITS:
        {
			if(Edge){
                //Rising Lead
				if((IR_56_MIN<count) && (count<IR_56_MAX)){
					//Correct Beg of BIT found
					//INT ON FALLING EDGE
					Edge=0;
					cbIR_FallingEdge();
                }
                else{
                    ResetIR();
                }
			}
			else{
                //Falling  - Tail
                if((IR_56D_MIN<count) && (count<IR_56D_MAX)){  // ������ 0b
					//We got a '0' here
					BitNo++;
					if(BitNo==8){
						BitNo=0;
						ByteNo++;
						if(ByteNo==4){
							State=IR_WAIT_STOP_BIT;
						}
					}
					Edge=1;
					cbIR_RisingEdge();
				}
				else if((IR_169_MIN<count) && (count<IR_169_MAX)) {  // 1b
					//We Have got a '1' here
					IrData[ByteNo]|=(1<<BitNo);
					BitNo++;
					if(BitNo==8){
						BitNo=0;
						ByteNo++;
						if(ByteNo==4){
							State=IR_WAIT_STOP_BIT;
						}
					}
					cbIR_RisingEdge();
					//TIMER1n_Config.ClrMode = TIMER1n_RISING_EGDE;
					//TIMER1n_Init((TIMER1n_Type*)TIMER16, TIMER1n_CAPTURE_MODE, &TIMER1n_Config);
					Edge=1;
				}
				else{
					ResetIR();
				}
			}
			break;
		}
        case IR_WAIT_STOP_BIT:
        {
            if(Edge)
            {
				//ttttttttttttttttttttttttttttttttt
				if(_getIR != NULL) _getIR(IrData);
				//ooooooooooooooooooooooooooooooooo
				/*  
				//Check for integrity
				if(IrData[2]==((unsigned char)~IrData[3]))
				{
					//Now We Have Got a packet
					//Add its  Cmd to Queue
                    
					//Step1:Check of Q full
					if((QEnd==(QMAX-1) && QFront==0)||((QEnd+1)==QFront))
					{
                        QFront++;
                        if(QFront==(QMAX))
                        QFront=0;
					}
 
					if(QEnd==(QMAX-1))
					QEnd=0;
					else
					QEnd++;
 
					IrCmdQ[QEnd]=IrData[2];
					PrevCmd=IrData[2];
					//----------------
					// Ű���� �Ѱ���
					if(_getIR != NULL) _getIR(PrevCmd);
					//----------------

					if(QFront==-1) QFront=0;
					//Prevent repeating immediatly
					Repeat=0;//It will be enabled after 4 repeat pulses
					RCount=0;
				}
				*/
				//eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee
			}
		}
		ResetIR();
		break;
	}
}

//=============================================================================
// Public Function
//=============================================================================
void InitIrNec(IrCallback fp)
{
	cbIR_TimerInit();
	_getIR = fp;

	ResetIR();
	  //State=IR_VALIDATE_LEAD_HIGH;
	  //Edge=0;
	  //cbIR_FallingEdge();
	  //Repeat=0;
	stop=0;
}



