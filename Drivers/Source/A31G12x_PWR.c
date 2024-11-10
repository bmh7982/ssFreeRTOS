/***************************************************************************//**
 * @file          A31G12x_PWR.c
 * @brief         Driver Source File for Power Control
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
#include "debug_frmwrk.h"
#include "slib.h"

//******************************************************************************
// Function
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         Enter Sleep mode with co-operated instruction by the Cortex-M3.
 * @return        none
 *//*-------------------------------------------------------------------------*/
void PWR_Sleep( void )
{
   /* Sleep Mode, clear SLEEPDEEP bit */
   SCB->SCR = 0;

   /* Sleep Mode*/
   WFI();
   NOP();
   NOP();
   NOP();
   NOP();
}

/*-------------------------------------------------------------------------*//**
 * @brief         Enter Deep Sleep mode with co-operated instruction by the Cortex-M3.
 * @return        none
 *//*-------------------------------------------------------------------------*/
void PWR_DeepSleep( void )
{
   /* Deep-Sleep Mode, set SLEEPDEEP bit */
   SCB->SCR = 0x4;

   /* Deep Sleep Mode*/
   WFI();
   NOP();
   NOP();
   NOP();
   NOP();
}

