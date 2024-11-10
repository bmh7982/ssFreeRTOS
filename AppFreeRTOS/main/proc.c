
//==================================================================
// Include 
//==================================================================
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

//#include "A31G12x.h"
//#include "aa_types.h"

//#include "A31G12x_PCU.h"
//#include "A31G12x_UARTn.h"
//#include "A31G12x_USART1n.h"

#include "uart_a31g12x.h"
#include "uart1n_a31g12x.h"

#include "FreeRTOS.h"
#include "task.h"

#include "peri.h"
#include "ir_nec.h"
#include "disp.h"

#include "ESP32.h"
#include "sensirion.h"
#include "modbus.h"
#include "mon.h"

#include "proc.h"

//----------------------------------
// define
//----------------------------------

//----------------------------------
// variable
//----------------------------------
//int disp_temp  = 0;     // 온도
//int disp_dust  = 0;     // 먼지농도
//int disp_gas   = 0;     // GAS 값
//int disp_co2   = 0;     // CO2 농도

//----------------------------------
// Tick
//----------------------------------
// Tick : 1 mSec
/*
bool  sys_tick  = false;

volatile unsigned int run_time=0;



#define CNT_10MS      10
volatile int cnt_10ms  = CNT_10MS;
bool    b_10ms  = false;



unsigned int  sleep_cnt =  0;
bool          bsleep    = false;

unsigned int  tick_set  =  0;
unsigned int  tick_cnt  =  0;
volatile bool btick     = false;

//---------< display 관련 >----
unsigned int  disp_cnt = 0;

//---------< i2c >----
unsigned int  i2c0_timer_cnt = 0;
bool          i2c0_btimer    = false;

unsigned int  i2c1_timer_cnt = 0;
bool          i2c1_btimer    = false;

//---------< i2c >----

void SysTick_Handler(void)
{
	sys_tick = true;

	// 진행시간
	++run_time;

	// SYSTEM 관련
	if(CNT_10MS<=++cnt_10ms) {
	  cnt_10ms = 0;
	  b_10ms = true;
	}

	// FND 관련
	if(disp_cnt) {
	  --disp_cnt;
	}

	// tick 관련
	if(tick_cnt) {
	  if((--tick_cnt)==0) {
	    tick_cnt = tick_set;
	    btick = true;
	    // 백그라운드 프로그램을 여기서 처리하려면 
	    // 인터럽트 순위가 높아 처리사간이 길면 통신 수신등에서 문제가 있을수 있다.
	    // 그럴경우 순위가 낮은 타이머를 사용하는 것이 바랍직 하다.
	  }
	}

	// sleep 관련
	if(sleep_cnt){
	  if((--sleep_cnt)==0)
	  bsleep = true;
	}

	// I2C 관련
	if(i2c0_timer_cnt ){
	  if((--i2c0_timer_cnt )==0) i2c0_btimer = true;
	}

	if(i2c1_timer_cnt ){
	  if((--i2c1_timer_cnt )==0) i2c1_btimer = true;
	}
}

void I2cTimerSet(int i2c_no, unsigned int cnt)
{
	if     (i2c_no==0){i2c0_timer_cnt = cnt; i2c0_btimer = false;}
	else if(i2c_no==1){i2c1_timer_cnt = cnt; i2c1_btimer = false;}
}

bool isI2cTimer(int i2c_no)
{
	if     (i2c_no==0){return i2c0_btimer;}
	else if(i2c_no==1){return i2c1_btimer;}
	return false;
}

void wait_rtn()
{
}

void wait_tick()
{
	while(sys_tick==false) wait_rtn();
	sys_tick = false;
}

//  일정시간내 조건이 되면 나가기 위해
//  TimerSet()을 하고 istimer로 확인할 수 있도록 한다
// 여기에서는 touch에서 사용하기 때문에 다른 곳에서는 사용하면 안된다.
void SetTick(unsigned int cnt)
{
	tick_set = tick_cnt = cnt;
	btick    = false;
}

void WaitTick(void)
{
	while(!btick) wait_rtn();
	btick = false;
}

//  무조건 대기
void sleep(unsigned int cnt)
{
	bsleep    = false;
	sleep_cnt = cnt;
	while(bsleep==false) wait_rtn();
}
*/
//=============================================================================
// Sub Routine
//=============================================================================


//=============================================================================
// Main Routine
//=============================================================================
#define UART_HOST  0

//char rx_buf_modbus[20];
//char tx_buf_modbus[100];

//char rx_buf_co2[20];
//char tx_buf_co2[100];

unsigned int PM2008_I2C_Read_Int(void);
void proc(void)
{
	// configure SysTick to 1ms interrupt

	InitPeri();
	InitDisp(5000);

	// uart 사용
	InitMon();
//	InitCo2();
	InitSensirion(1000);  // I2C 사용, 센서3종 초기화

//	InitEsp32();
//	InitModBus();


	// Test 용
	// 500msec 지연
//	DispLedTest(300);

//	SetTick(500);      // 500mSec
//	WaitTick();

	//-----------------------------
	while(1){
	  ProcMon();
//	  ProcRemocon();  // 리모콘제어
//	  ProcCo2();
	  ProcSensirion();

//	  ProcEsp32();
//	  ProcModBus();
//	  ProcPir();      // PIR센서
//	  ProcDisp();
	  taskYIELD();
	}
}

//=============================================================================
// Public Funciton
//=============================================================================

uint32_t GetTime(void)
{
	return  xTaskGetTickCount();
	//return run_time;
}

void sleep(unsigned int cnt)
{
	vTaskDelay(cnt);
}
