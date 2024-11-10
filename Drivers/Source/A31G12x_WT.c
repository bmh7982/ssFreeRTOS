/***************************************************************************//**
 * @file          A31G12x_WT.c
 * @brief         Driver Source File for Watch Timer
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
 * @brief         Set WT CR Register
 * @param[in]     u32WTSet
 *                      Watch Timer Control Register Setting Data
 * @return        none
 * @details       This function sets the mode, external clock edge, Timer out polarity,
 *                Capture Polarity and Timer match/capture interrupt.
 *//*-------------------------------------------------------------------------*/

void WT_SetReg( uint32_t u32WTSet )
{
   WT->CR = u32WTSet;      // Setting Timer 1n Control Register
}

//uint32_t _WTCLK;

/*-------------------------------------------------------------------------*//**
 * @brief         Initialize the WT peripheral with the specified parameters.
 * @param[in]     WT_Config
 *                   Pointer to a WT_CFG_Type structure
 *                   that contains the configuration information for the specified peripheral.
 * @return        none
 *//*-------------------------------------------------------------------------*/
void WT_Init( WT_CFG_Type WT_Config )
{
   WT->CR = 0
            | ( ( WT_Config.wtClkDiv & 0x03 ) << 4 )   // WTINTV[1:0]
            | WT_CR_WTCLR
            ;
   WT->DR = ( WT_Config.wtTmrConst & 0xFFF );
}

/*-------------------------------------------------------------------------*//**
 * @brief         deinit for Watch Timer function
 * @return        none
 *//*-------------------------------------------------------------------------*/
void WT_DeInit( void )
{
   WT->CR = 0;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Enable WT activity
 * @param[in]     ctrl
 *                   Next State of Functional Operation
 *                   -  ENABLE, DISABLE
 * @return        none
 *//*-------------------------------------------------------------------------*/
void WT_Start( FunctionalState ctrl )
{
   uint32_t tmp_reg;

   tmp_reg = WT->CR & 0xFF;
   tmp_reg &= ~( 0x1 << 7 );  // Disable watch-dog timer counter

   if( ctrl == ENABLE )
   {
      tmp_reg |= ( 0x1 << 7 );   // Enable watch timer counter
   }

   tmp_reg |= WT_CR_WTCLR;    // clear the counter and divider

   WT->CR = tmp_reg;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Get the current value of WT
 * @return        current value of WT
 *//*-------------------------------------------------------------------------*/
uint32_t WT_GetCurrentCount( void )
{
   return ( WT->CNT & 0xFFF );
}

/*-------------------------------------------------------------------------*//**
 * @brief         Get the timer status register of WT
 * @return        the status register of WT
 *//*-------------------------------------------------------------------------*/
uint32_t WT_GetStatus( void )
{
   return ( WT->CR & WT_STATUS_BITMASK );
}

/*-------------------------------------------------------------------------*//**
 * @brief         Clear the timer status register of WT
 * @return        none
 *//*-------------------------------------------------------------------------*/
void WT_ClearStatus( void )
{
   WT->CR |= WT_CR_WTIFLAG;   // (1<<1):  clear for WTIFLAG
}

/*-------------------------------------------------------------------------*//**
 * @brief         Configure the peripheral interrupt.
 * @param[in]     NewState
 *                   Next State of Interrupt Operation
 *                   -  ENABLE, DISABLE
 * @return        none
 *//*-------------------------------------------------------------------------*/
void WT_IntConfig( FunctionalState NewState )
{
   uint32_t reg_val = 0;

   reg_val = ( WT->CR & 0xFF );

   if( NewState == ENABLE )
   {
      reg_val |= WT_CR_WTIEN;          // WTIEN bit
   }
   else
   {
      reg_val &= ~WT_CR_WTIEN;
   }

   WT->CR = reg_val | WT_CR_WTIFLAG;   // (1<<1):  clear for WTIFLAG
}

