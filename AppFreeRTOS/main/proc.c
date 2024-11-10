
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
//int disp_temp  = 0;     // �µ�
//int disp_dust  = 0;     // ������
//int disp_gas   = 0;     // GAS ��
//int disp_co2   = 0;     // CO2 ��

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

//---------< display ���� >----
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

	// ����ð�
	++run_time;

	// SYSTEM ����
	if(CNT_10MS<=++cnt_10ms) {
	  cnt_10ms = 0;
	  b_10ms = true;
	}

	// FND ����
	if(disp_cnt) {
	  --disp_cnt;
	}

	// tick ����
	if(tick_cnt) {
	  if((--tick_cnt)==0) {
	    tick_cnt = tick_set;
	    btick = true;
	    // ��׶��� ���α׷��� ���⼭ ó���Ϸ��� 
	    // ���ͷ�Ʈ ������ ���� ó���簣�� ��� ��� ���ŵ�� ������ ������ �ִ�.
	    // �׷���� ������ ���� Ÿ�̸Ӹ� ����ϴ� ���� �ٶ��� �ϴ�.
	  }
	}

	// sleep ����
	if(sleep_cnt){
	  if((--sleep_cnt)==0)
	  bsleep = true;
	}

	// I2C ����
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

//  �����ð��� ������ �Ǹ� ������ ����
//  TimerSet()�� �ϰ� istimer�� Ȯ���� �� �ֵ��� �Ѵ�
// ���⿡���� touch���� ����ϱ� ������ �ٸ� �������� ����ϸ� �ȵȴ�.
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

//  ������ ���
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

	// uart ���
	InitMon();
//	InitCo2();
	InitSensirion(1000);  // I2C ���, ����3�� �ʱ�ȭ

//	InitEsp32();
//	InitModBus();


	// Test ��
	// 500msec ����
//	DispLedTest(300);

//	SetTick(500);      // 500mSec
//	WaitTick();

	//-----------------------------
	while(1){
	  ProcMon();
//	  ProcRemocon();  // ����������
//	  ProcCo2();
	  ProcSensirion();

//	  ProcEsp32();
//	  ProcModBus();
//	  ProcPir();      // PIR����
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
