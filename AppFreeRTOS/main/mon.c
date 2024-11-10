
//==============================================================================
// include
//==============================================================================
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "uart1n_a31g12x.h"
#include "uart_a31g12x.h"
#include "mon.h"

#define USART10
//#define UART1

//----------------------------------
// define
//----------------------------------
#ifdef USART10
#define  UartMon     10
#define  cputs(a)    do{UsartSendStr(UartMon, a); } while(0)
#else
#define  UartMon     10
#define  cputs(a)    do{UartSend(UartMon, a, strlen(a)); } while(0)
#endif
//----------------------------------
// variablee
//----------------------------------
char rx_buf_dbg[20];
char tx_buf_dbg[200];

//==============================================================================
// Sub Routines
//==============================================================================

//==============================================================================
// Function
//==============================================================================
void InitMon(void)
{
#ifdef USART10
	UsartInitPort(UartMon);    //Usartn10InitPort(ENABLE);
	UsartInitBuff(UartMon, rx_buf_dbg, sizeof(rx_buf_dbg), tx_buf_dbg ,  sizeof(tx_buf_dbg ));
	UsartOpen(UartMon,  38400, USART_DATA_BIT_8, USART_PARITY_BIT_NONE, USART_STOP_BIT_1); //UsartOpen((USART1n_Type *)USART10,  38400, USART1n_DATA_BIT_8, USART1n_PARITY_BIT_NONE, USART1n_STOP_BIT_1);
#else
	UartInitPort1();
	UartInitBuff (UartMon, rx_buf_dbg, sizeof(rx_buf_dbg), tx_buf_dbg ,  sizeof(tx_buf_dbg ));
	UartOpen(UartMon,  38400, UART_DATA_BIT_8, UART_PARITY_BIT_NONE, UART_STOP_BIT_1);
#endif
	cputs("!!! Start !!!\r\n");
}

void ProcMon(void)
{
//	  volatile int c, cnt; 

//	  do{
//	    cnt=UsartReceive(UartMon, (uint8_t *)&c, 1);
//	    if(cnt)
//		  UsartSendChar(UartMon, 0xff&c);
//	  }while(cnt);
}

