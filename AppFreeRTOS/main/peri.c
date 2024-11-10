
//******************************************************************************
// Include
//******************************************************************************

#include "A31G12x.h"
#include "aa_types.h"

#include "A31G12x_ADC.h"
#include "A31G12x_CRC.h"
#include "A31G12x_FMC.h"
#include "A31G12x_I2Cn.h"
#include "A31G12x_INTC.h"
#include "A31G12x_LCD.h"
#include "A31G12x_PCU.h"
#include "A31G12x_PWR.h"
#include "A31G12x_SCUCC.h"
#include "A31G12x_SCUCG.h"
#include "A31G12x_SCULV.h"
#include "A31G12x_TIMER1n.h"
#include "A31G12x_TIMER2n.h"
#include "A31G12x_TIMER3n.h"
#include "A31G12x_UARTn.h"
#include "A31G12x_USART1n.h"
#include "A31G12x_WDT.h"
#include "A31G12x_WT.h"
#include "A31G12x_WTIDKY.h"

#include "uart_a31g12x.h"
#include "uart1n_a31g12x.h"

//#include "FreeRTOS.h"
//#include "task.h"

#include "proc.h"
#include "ir_nec.h"
#include "disp.h"
#include "peri.h"

//----------------------------------
// define
//----------------------------------
#define UART_NO_ESP32     1

//----------------------------------
// variable
//----------------------------------
int  co2_err;      // co2 센서먼지센서

//==============================================================================
// Sub Routines
//==============================================================================

//==============================================================================
// Public Function GPIO
//==============================================================================
//RS485
void rs485_rx(void)
{
	GPIO_ClearValue((Pn_Type*)PD, 7);
}

void rs485_tx(void)
{
	GPIO_SetValue  ((Pn_Type*)PD, 7);
}

// FAN
void fan_on()
{
	GPIO_SetValue ((Pn_Type*)PE, 0);
}

void fan_off()
{
	GPIO_ClearValue ((Pn_Type*)PE, 0);
}

// TOUCH
bool is_touch()
{
	return (GPIO_ReadValue((Pn_Type*)PB) & _BIT( 8)) ? true : false;       // TOUCH센서
}

//PIR
bool is_pir()
{
	return (GPIO_ReadValue((Pn_Type*)PE) & _BIT( 2)) ? true : false;       // 인체감지 센서
}

// 회로도 : TEMP nRESET
void temp_reset(void)
{
	GPIO_ClearValue((Pn_Type*)PA,11);
}

void temp_run(void)
{
	GPIO_SetValue  ((Pn_Type*)PA,11);
}

// 회로도 : DUST_ON
void dust_on(void)
{
	GPIO_SetValue  ((Pn_Type*)PC, 4);
}

void dust_off(void)
{
	GPIO_ClearValue((Pn_Type*)PC, 4);
}

//==============================================================================
// 리모콘 Function
//==============================================================================
// 리모콘에서 호출하는 Call Back 함수 
void GetRemoteCB(unsigned char *IrData)
{
	char buf[100];
	sprintf(buf,"\r\nR:%02x, %02x, %02x, %02x ", IrData[0], IrData[1], IrData[2], IrData[3]);

	// UARTSend에서 자동으로 rs485_tx(), rs485_rx()로 전환되도록 한다.
	if(IrData[0] == 0 & IrData[1] == 0){
		UsartSendStr(10, "-");
	}
	else {
		UsartSendStr(10, buf);
	}
}

void InitRemote(void)
{
	// 리모콘값 초기화
	// 
	InitIrNec(GetRemoteCB);
	//InitIrNec(NULL);
}

//==============================================================================
// Public Function Co2
//==============================================================================
#define UARTn_Co2        1          //#define USARTn_Co2       10          //
#ifdef UARTn_Co2
	#define Co2InitPort()        do{UartInitPort1();} while(0)
	#define Co2InitBuff()        do{UartInitBuff(UARTn_Co2, uart_rx_buf_co2, sizeof(uart_rx_buf_co2), NULL ,  0 );} while(0)
	#define Co2Open()            do{UartOpen(UARTn_Co2,  38400, UART_DATA_BIT_8, UART_PARITY_BIT_NONE, UART_STOP_BIT_1);} while(0)
	#define Co2Receive(pbuf,len) UartReceive(UARTn_Co2, (uint8_t *)pbuf, len)
#else
	#define Co2InitPort()        do{UsartInitPort(USARTn_Co2);} while(0)
	#define Co2InitBuff()        do{UsartInitBuff(USARTn_Co2, uart_rx_buf_co2, sizeof(uart_rx_buf_co2), NULL ,  0 );} while(0)
	#define Co2Open()            do{UsartOpen(USARTn_Co2,  38400, USART_DATA_BIT_8, USART_PARITY_BIT_NONE, USART_STOP_BIT_1);} while(0)
	#define Co2Receive(pbuf,len) UsartReceive(USARTn_Co2, (uint8_t *)pbuf, len)
#endif

int      cur_co2;      // CO2 농도
bool     co2_rcvd;

char     uart_rx_buf_co2[20];
uint8_t  t300_buf[20];
uint32_t co2_comm_time;
int      co2_idx;

void InitCo2(void)
{
	Co2InitPort();
	Co2InitBuff();
	Co2Open();

	co2_idx = 0;
	co2_comm_time = GetTime();
	co2_rcvd = true;
}

bool Co2Parse(void)
{
	unsigned char ch;
	unsigned int val;

	if(t300_buf[4] != ' ' || t300_buf[5] != 'p' || t300_buf[6] != 'p' || t300_buf[7] != 'm' || t300_buf[8] != '\r'){
		cur_co2 = 0;
		return false;
	}

	val = 0;

	ch = 0xff & t300_buf[0];
	if     (('0'<=ch)&&(ch <='9')){ val = 1000 * (ch - '0');}
	else if((ch!=' ')||(val!= 0 )){	cur_co2 = 0; return false;}

	ch = 0xff & t300_buf[1];
	if     (('0'<=ch)&&(ch <='9')){ val += 100  * (ch - '0');}
	else if((ch!=' ')||(val!= 0 )){ cur_co2 = 0; return false;}

	ch = 0xff & t300_buf[2];
	if     (('0'<=ch)&&(ch <='9')){ val += 10   * (ch - '0');}
	else if((ch!=' ')||(val!= 0 )){ cur_co2 = 0; return false;}

	ch = 0xff & t300_buf[3];
	if     (('0'<=ch)&&(ch <='9')){ val += 1    * (ch - '0');}
	else                          { cur_co2 = 0; return false;}

	cur_co2  = val;
	co2_rcvd = true;
	return  true;
}

void ProcCo2(void)
{
	int c;
//	uint32_t cur_time; 

//	cnt = Co2Receive(&c, 1);     // cnt = UsartReceive(USARTn_Co2, (uint8_t *)&c, 1);
//	cur_time = GetTime();
//	if(!cnt){
//		// 대기시간이 넘으면 에러를 세트하고 종료함
//		if(	co2_comm_time + 1500 < cur_time){
//			co2_rcvd = false;
//			cur_co2 = 0;
//		}
//		return;
//	}

	c &= 0xff;
	co2_comm_time = GetTime();
	while(Co2Receive(&c, 1)){
		if((sizeof(t300_buf)-1) <= co2_idx){
			co2_idx = 0;
			continue;
		}

		t300_buf[co2_idx++] = c;
		if(c=='\n'){
			if(co2_idx!=10){
				co2_idx = 0;
			}
			else{
				if(Co2Parse()){set_disp_co2(cur_co2, false);}
				else          {set_disp_co2(      0, true );}
			}
			co2_idx = 0;
		}
	}
}

//==============================================================================
// Public Function DelayMicroSec
//==============================================================================
#define TimerDelay  ((TIMER1n_Type*) TIMER14)

void DelayUs(uint32_t usec)
{
	uint16_t i, hi, lo;

	hi = usec / 0x10000;
	lo = usec % 0x10000;

	if(hi !=0){
		TimerDelay->ADR = 0xffff;
		TIMER1n_Cmd(TimerDelay, ENABLE );   // timer start & clear
		for(i=0; i<hi; i++){
			while( T1nMaInt_GetFg( TIMER14 ) != 1 );
			T1nMaInt_ClrFg( TIMER14 );
			TIMER1n_Cmd(TimerDelay, ENABLE );   // timer start & clear
		}
	}

	if(lo !=0){
		TimerDelay->ADR = lo-1;
		TIMER1n_Cmd(TimerDelay, ENABLE );   // timer start & clear
		while( T1nMaInt_GetFg( TIMER14 ) != 1 );
		T1nMaInt_ClrFg( TIMER14 );
	}
}

void InitTmrUs(void)
{
	TIMER1n_PERIODICCFG_Type   TIMER1n_Config;

	TIMER1n_Config.CkSel     = TIMER1n_PCLK;             // PCLK = 40MHz
	TIMER1n_Config.Prescaler = SystemPeriClock/1000000;  // fTIMER = PCLK(40,000,000) / 1,000,000 = 1MHz
	TIMER1n_Config.ADR       = (1000);                   // Period  = ADR / fTIMER = 1000us
	TIMER1n_Config.StartLevel = TIMER1n_START_LOW;
	TIMER1n_Config.BDR = 0;                            // Duty(B) = BDR / fTIMER =    0us  // dummy parameter
	TIMER1n_Config.ECE = TIMER1n_FALLING_EGDE;         // dummy parameter

	TIMER1n_Init((TIMER1n_Type*)TIMER14, TIMER1n_ONESHOT_MODE, &TIMER1n_Config );    //TIMER1n_Init((TIMER1n_Type*)TIMER14, TIMER1n_PERIODIC_MODE, &TIMER1n_Config );
}

//==============================================================================
// Public Function (초기화)
//==============================================================================
void InitPeri(void)
{
	InitRemote();

//	RgbSpdSet (COLOR_BLACK); 
//	RgbDustSet(COLOR_BLACK); 

	InitTmrUs();       //
}


