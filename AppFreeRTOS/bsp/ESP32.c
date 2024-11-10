
//==============================================================================
// Include
//==============================================================================
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

//#include "A31G12x.h"
//#include "aa_types.h"

//#include "A31G12x_PCU.h"

#include "uart_a31g12x.h"
#include "uart1n_a31g12x.h"

#include "peri.h"
#include "ESP32.h"

//----------------------------------
// define
//----------------------------------
#define UART_NO_ESP32     1
#define UART_HOSTn       ((UARTn_Type*)UART1)

//----------------------------------
// variables
//----------------------------------
char rx_buf_esp32[20];
char tx_buf_esp32[500];

//----------------------------------
// protocol
//----------------------------------

//==============================================================================
// Sub Routines
//==============================================================================

//==============================================================================
// Call Back Function
//==============================================================================

// TEST(RS485 테스트용)

uint8_t   esp32_rx_buf[8];
uint32_t  esp32_rx_rtm=0;   // 진행시간

int  esp32_rx_idx=0;   // 버퍼 현재위치
int  esp32_rx_cnt=0;   // 받은 크기

void InitEsp32(void)
{
	UartInitPort1();
	UartInitBuff (UART_NO_ESP32, rx_buf_esp32, sizeof(rx_buf_esp32), tx_buf_esp32, sizeof(tx_buf_esp32));
	UartOpen(UART_NO_ESP32, 38400, UART_DATA_BIT_8, UART_PARITY_BIT_NONE, UART_STOP_BIT_1);

	//test
    UartSend(UART_NO_ESP32, "Hello World\r\n", 13 );  // UARTSend에서 자동으로 rs485_tx(), rs485_rx()로 전환되도록 한다.
}

//==============================================================================
// Function
//==============================================================================
volatile int c, cnt; 
void ProcEsp32(void)
{
	// 데이터를 받는다
	cnt=UartReceive(UART_NO_ESP32, (uint8_t *)&c, 1);
	if(cnt==0) return;

	UartSend(UART_NO_ESP32, (char*) &c, 1);
}

//==============================================================================
// Interrupt Handler
//==============================================================================
void  Uart1_TxE(void)
{
	// 외부 프로그램에서 해당포트를 hi 또는 low로 할것
	rs485_rx();
}

