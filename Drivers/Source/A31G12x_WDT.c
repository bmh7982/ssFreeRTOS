/***************************************************************************//**
 * @file          A31G12x_WDT.c
 * @brief         Driver Source File for Watch-Dog Timer
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
// Variable
//******************************************************************************

uint32_t _WDTCLK;

//******************************************************************************
// Function
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         Initialize the WDT peripheral with the specified parameters.
 *                   Pointer to a WDT_CFG_Type structure
 *                   that contains the configuration information
 *                   for the specified peripheral.
 * @return        none
 *//*-------------------------------------------------------------------------*/
void WDT_Init( WDT_CFG_Type WDT_Config )
{
   uint32_t reg_val = 0;

   WDT->DR = ( WDT_Config.wdtTmrConst & 0x00FFFFFF );
   WDT->WINDR = ( WDT_Config.wdtWTmrConst & 0x00FFFFFF );

   reg_val = WDT_Config.wdtClkDiv;

   if( WDT_Config.wdtResetEn == ENABLE )
   {
      reg_val &= ~( 0x3f << WDT_CR_RSTEN_Pos );
   }
   else
   {
      reg_val |= ( 0x25 << WDT_CR_RSTEN_Pos );
   }

   WDT->CR = ( 0x5A69 << WDT_CR_WTIDKY_Pos ) | ( 0x1a << WDT_CR_CNTEN_Pos ) | reg_val;    // /w Write Identification Key
}

/*-------------------------------------------------------------------------*//**
 * @brief         Close ADC
 * @return        none
 *//*-------------------------------------------------------------------------*/
void WDT_DeInit( void )
{
   WDT->CR = 0
             | ( 0x5A69 << WDT_CR_WTIDKY_Pos )  // Write Identification Key
             | ( 0x25  << WDT_CR_RSTEN_Pos )    // Disable watch-dog timer reset
             | ( 0x1A << WDT_CR_CNTEN_Pos )     // Disable watch-dog timer counter
             ;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Reload WDT counter
 * @return        none
 *//*-------------------------------------------------------------------------*/
void WDT_ReloadTimeCounter( void )
{
   WDT->CNTR = 0x6a;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Enable WDT activity
 * @param[in]     ctrl
 *                   -  DISABLE:  wdt enable
 *                   -  ENABLE:   wdt disable
 * @return        none
 *//*-------------------------------------------------------------------------*/
void WDT_Start( FunctionalState ctrl )
{
   uint32_t tmp_reg;

   tmp_reg = WDT->CR & 0xFFFF;
   tmp_reg |= ( 0x1a << WDT_CR_CNTEN_Pos );  // Disable watch-dog timer counter

   if( ctrl == ENABLE )
   {
      tmp_reg &= ~( 0x3f << WDT_CR_CNTEN_Pos );    // Enable watch-dog timer counter,
   }

   WDT->CR = ( 0x5A69 << WDT_CR_WTIDKY_Pos ) | tmp_reg;  // Write Identification Key 0x5A69
}

/*-------------------------------------------------------------------------*//**
 * @brief         Get the current value of WDT
 * @return        current value of WDT
 *//*-------------------------------------------------------------------------*/
uint32_t WDT_GetCurrentCount( void )
{
   return WDT->CNT;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Get the timer status register of WDT
 * @return        the status register of WDT
 *//*-------------------------------------------------------------------------*/
uint32_t WDT_GetStatus( void )
{
   return WDT->SR;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Clear the timer status register of WDT
 * @param[in]     clrbit
 *                   -  WDT_SR_UNFIFLAG:     UNFIFLAG Interrupt flag
 *                   -  WDT_SR_WINMIFLAG:    WINMIFLAG Interrupt flag
 * @return        none
 *//*-------------------------------------------------------------------------*/
void WDT_ClearStatus( uint32_t clrbit )
{
   WDT->SR = clrbit;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Configure the peripheral interrupt.
 * @param[in]     WDTIntCfg
 *                   Specifies the interrupt flag
 *                   -  WDT_INTCFG_UNFIEN:   UNFIEN Interrupt enable
 *                   -  WDT_INTCFG_WINMIEN:  WINMIEN Interrupt enable
 * @param[in]     NewState
 *                   Next State of Interrupt Operation
 *                   -  ENABLE, DISABLE
 * @return        none
 *//*-------------------------------------------------------------------------*/
void WDT_IntConfig( WDT_INT_Type WDTIntCfg, FunctionalState NewState )
{
#if 0    // before bug fix
   uint32_t reg_val = 0;
   uint32_t tmp = 0;

   reg_val = ( WDT->CR & 0xFFFF );

   switch( WDTIntCfg )
   {
      case WDT_INTCFG_UNFIEN:
         tmp = WDT_CR_UNFIEN;
         break;
      case WDT_INTCFG_WINMIEN:
         tmp = WDT_CR_WINMIEN;
         break;
   }

   if( NewState == ENABLE )
   {
      reg_val |= ( tmp & WDT_INTERRUPT_BITMASK );
   }
   else
   {
      reg_val &= ( ( ~tmp ) & WDT_INTERRUPT_BITMASK );   // reg_val &= ~tmp;°¡ ¿ÇÀ» µí...
   }

   WDT->CR = ( 0x5A69 << WDT_CR_WTIDKY_Pos ) | reg_val;  // Write Identification Key 0x5A69
#else    // after bug fix
   uint32_t    reg_val = 0;
   uint32_t    mask = 0;

   switch( WDTIntCfg )
   {
      case WDT_INTCFG_UNFIEN:
         mask = WDT_CR_UNFIEN_Msk;
         break;
      case WDT_INTCFG_WINMIEN:
         mask = WDT_CR_WINMIEN_Msk;
         break;
   }

   reg_val =   WDT->CR
               & ~( WDT_CR_WTIDKY_Msk )
               | ( ( uint32_t )WDT_CR_WTIDKY_Value << WDT_CR_WTIDKY_Pos )
               ;
   if( NewState == ENABLE )
   {
      reg_val |= mask;
   }
   else
   {
      reg_val &= ~mask;
   }

   WDT->CR = reg_val;
#endif
}

