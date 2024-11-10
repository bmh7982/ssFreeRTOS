/***************************************************************************//**
 * @file          main.c
 * @brief         Example Source File
 * @version       1.0
 * @date          2019-09-01
 * @author        ABOV Application Team
 *
 * Copyright (c) 2019, ABOV Semiconductor. All rights reserved.
 *//***************************************************************************/

//******************************************************************************
// Include
//******************************************************************************

#include <stdarg.h>
#include <stdio.h>

// MCU includes.
#include "A31G12x.h"
#include "aa_types.h"

#include "A31G12x_ADC.h"
#include "A31G12x_CRC.h"
#include "A31G12x_FMC.h"
#include "A31G12x_I2Cn.h"
#include "A31G12x_INTC.h"
//#include "A31G12x_LCD.h"
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
//#include "debug_frmwrk.h"
//#include "slib.h"

// Scheduler includes.
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

// Demo app includes.
#include "death.h"
//#include "integer.h"
//#include "blocktim.h"
//#include "partest.h"
//#include "semtest.h"
//#include "PollQ.h"
//#include "flash.h"
//#include "comtest2.h"

//******************************************************************************
// Defines
//******************************************************************************
// Task priorities.
#define mainPROC_TASK_PRIORITY				( tskIDLE_PRIORITY + 1 )
#define mainESP32_TASK_PRIORITY             ( tskIDLE_PRIORITY + 1 )
#define mainCREATOR_TASK_PRIORITY           ( tskIDLE_PRIORITY + 3 )

//******************************************************************************
// Constant
//******************************************************************************

// The check task uses the sprintf function so requires a little more stack.
#define mainPROC_TASK_STACK_SIZE			( configMINIMAL_STACK_SIZE + 50 )
#define mainPROC_ESP32_STACK_SIZE			( configMINIMAL_STACK_SIZE + 50 )

// The number of nano seconds between each processor clock.
#define mainNS_PER_CLOCK ( ( unsigned long ) ( ( 1.0 / ( double ) configCPU_CLOCK_HZ ) * 1000000000.0 ) )

//******************************************************************************
// Variable
//******************************************************************************

//******************************************************************************
// Prototype
//******************************************************************************

//******************************************************************************
// Routines
//******************************************************************************
void proc(void *);
void task_main( void )
{
	__IO BaseType_t  ret;

	// Start the tasks defined within this file/specific to this demo.
	ret = xTaskCreate( proc, "proc", mainPROC_TASK_STACK_SIZE, NULL, mainPROC_TASK_PRIORITY, NULL );    //STACK(4*(128+50)
	//xTaskCreate( vLCDTask, "LCD", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );              //STACK(1*0x80) = 0x0080

	// The suicide tasks must be created last as they need to know how many 
	// tasks were running prior to their creation in order to ascertain whether
	//or not the correct/expected number of tasks are running at any given time.
	// TASK를 없애는 방안을 검토한다.
//	vCreateSuicidalTasks( mainCREATOR_TASK_PRIORITY );    // TASK "CREATOR"를 만든다                     //STACK(1*0x80) = 0x0080

	// Configure the timers used by the fast interrupt timer test.
	//vSetupTimerTest();  // 타이머를 초기화 한다.

	// Start the scheduler.
	vTaskStartScheduler();

	// Will only get here if there was not enough heap space to create the idle task.
	while(1);
}

