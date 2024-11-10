/***************************************************************************//**
 * @file          A31G12x_TIMER2n.c
 * @brief         Driver Source File for Timer
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
 * @brief         Initialize the TIMER2n peripheral with the specified parameters.
 *                   -  Set Clock frequency for Timer
 *                   -  Set initial configuration for Timer
 * @param[in]     TIMER2x
 *                   Pointer to a target peripheral.
 *                   -  TIMER20 ~ TIMER21
 * @param[in]     TimerCounterMode
 *                   Timer Counter Mode
 *                   -  TIMER2n_PERIODIC_MODE:  Timer mode
 *                   -  TIMER2n_PWM_MODE:       Counter rising mode
 *                   -  TIMER2n_ONESHOT_MODE:   Counter falling mode
 *                   -  TIMER2n_CAPTURE_MODE:   Counter on both edges
 * @param[in]     TIMER2n_Config
 *                   Pointer to TIM_TIMERCFG_Type
 *                   that contains the configuration information
 *                   for the specified peripheral.
 * @return        none
 *//*-------------------------------------------------------------------------*/
void TIMER2n_Init( TIMER2n_Type* TIMER2x, TIMER2n_MODE_OPT TimerCounterMode, void* TIMER2n_Config )
{
   TIMER2n_PERIODICCFG_Type* pTimeCfg;
   TIMER2n_PWMCFG_Type* pPwmOneshotCfg;
   TIMER2n_CAPTURECFG_Type* pCaptureCfg;
   uint16_t reg_val16;

   //set power
   if( TIMER2x == ( TIMER2n_Type* )TIMER20 )
   {
      /* Set up peripheral clock for TIMER0 module */
      SCUCG->PPCLKEN1_b.T20CLKE = 1;
   }
   else if( TIMER2x == ( TIMER2n_Type* )TIMER21 )
   {
      /* Set up peripheral clock for TIMER1 module */
      SCUCG->PPCLKEN1_b.T21CLKE = 1;
   }

   if( TimerCounterMode == TIMER2n_PERIODIC_MODE )
   {
      pTimeCfg = ( TIMER2n_PERIODICCFG_Type* )TIMER2n_Config;

      SCUCG->PPCLKSR_b.T20CLK = pTimeCfg->TCLK_SEL;
      reg_val16 = 0
                  | ( TimerCounterMode << 12 )
                  | TIMER2n_CR_CLK_SET( pTimeCfg->CkSel )
                  | TIMER2n_CR_ECE_SET( pTimeCfg->ExtCkEdge )
                  | TIMER2n_CR_OPOL_SET( pTimeCfg->StartLevel )
                  | TIMER2n_CR_CPOL_SET( pTimeCfg->CapEdge )
                  ;
      TIMER2x->CR = reg_val16;
      TIMER2x->PREDR = ( ( pTimeCfg->Prescaler - 1 ) & TIMER2n_PREDR_MASK );
      TIMER2x->ADR = pTimeCfg->ADR;
      TIMER2x->BDR = pTimeCfg->BDR;
   }
   else if( ( TimerCounterMode == TIMER2n_PWM_MODE ) || ( TimerCounterMode == TIMER2n_ONESHOT_MODE ) )
   {
      pPwmOneshotCfg = ( TIMER2n_PWMCFG_Type* )TIMER2n_Config;

      SCUCG->PPCLKSR_b.T20CLK = pPwmOneshotCfg->TCLK_SEL;
      reg_val16 = 0
                  | ( TimerCounterMode << 12 )
                  | TIMER2n_CR_CLK_SET( pPwmOneshotCfg->CkSel )
                  | TIMER2n_CR_ECE_SET( pPwmOneshotCfg->ExtCkEdge )
                  | TIMER2n_CR_OPOL_SET( pPwmOneshotCfg->StartLevel )
                  | TIMER2n_CR_CPOL_SET( pPwmOneshotCfg->CapEdge )
                  ;
      TIMER2x->CR = reg_val16;
      TIMER2x->PREDR = ( ( pPwmOneshotCfg->Prescaler - 1 ) & TIMER2n_PREDR_MASK );
      TIMER2x->ADR = pPwmOneshotCfg->ADR;
      TIMER2x->BDR = pPwmOneshotCfg->BDR;
   }
   else if( TimerCounterMode == TIMER2n_CAPTURE_MODE )
   {
      pCaptureCfg = ( TIMER2n_CAPTURECFG_Type* )TIMER2n_Config;

      SCUCG->PPCLKSR_b.T20CLK = pCaptureCfg->TCLK_SEL;
      reg_val16 = 0
                  | ( TimerCounterMode << 12 )
                  | TIMER2n_CR_CLK_SET( pCaptureCfg->CkSel )
                  | TIMER2n_CR_CPOL_SET( pCaptureCfg->ClrMode )
                  | TIMER2n_CR_CAPSEL_SET( pCaptureCfg->CAPCkSel )
                  ;
      TIMER2x->CR = reg_val16;
      TIMER2x->PREDR = ( ( pCaptureCfg->Prescaler - 1 ) & TIMER2n_PREDR_MASK );
   }

   TIMER2x->CR_b.T2nCLR = TIMER2n_CLEAR;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Close Timer/Counter device
 * @param[in]     TIMER2x
 *                   Pointer to a target peripheral.
 *                   -  TIMER20 ~ TIMER21
 * @return        none
 *//*-------------------------------------------------------------------------*/
void TIMER2n_DeInit( TIMER2n_Type* TIMER2x )
{
   // Disable timer/counter
   TIMER2x->CR_b.T2nEN = 0;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Clear Timer Status
 * @param[in]     TIMER2x
 *                   Pointer to a target peripheral.
 *                   -  TIMER20 ~ TIMER21
 * @param[in]     value
 *                   clear value
 * @return        none
 *//*-------------------------------------------------------------------------*/
void TIMER2n_ClearStatus( TIMER2n_Type* TIMER2x, uint8_t value )
{
   TIMER2x->CR |= ( 0xD & value );
}

/*-------------------------------------------------------------------------*//**
 * @brief         Get Timer Status
 * @param[in]     TIMER2x
 *                   Pointer to a target peripheral.
 *                   -  TIMER20 ~ TIMER21
 * @return        Value of status register
 *//*-------------------------------------------------------------------------*/
uint8_t TIMER2n_GetStatus( TIMER2n_Type* TIMER2x )
{
   return TIMER2x->CR;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Start/Stop Timer/Counter device
 * @param[in]     TIMER2x
 *                   Pointer to a target peripheral.
 *                   -  TIMER20 ~ TIMER21
 * @param[in]     NewState
 *                   Next State of Functional Operation
 *                   -  ENABLE, DISABLE
 * @return        none
 *//*-------------------------------------------------------------------------*/
void TIMER2n_Cmd( TIMER2n_Type* TIMER2x, FunctionalState NewState )
{
   if( NewState == ENABLE )
   {
      TIMER2x->CR_b.T2nEN = TIMER2n_ENABLE;
   }
   else
   {
      TIMER2x->CR_b.T2nEN = TIMER2n_DISABLE;
   }
}

/*-------------------------------------------------------------------------*//**
 * @brief         Clear Timer/Counter device,
 * @param[in]     TIMER2x
 *                   Pointer to a target peripheral.
 *                   -  TIMER20 ~ TIMER21
 * @return        none
 *//*-------------------------------------------------------------------------*/
void TIMER2n_ClearCounter( TIMER2n_Type* TIMER2x )
{
   TIMER2x->CR_b.T2nCLR |= TIMER2n_CLEAR;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Update value
 * @param[in]     TIMER2x
 *                   Pointer to a target peripheral.
 *                   -  TIMER20 ~ TIMER21
 * @param[in]     CountCh
 *                   -  0 = GRA
 *                   -  1 = GRB
 * @param[in]     Value
 *                   updated match value
 * @return        none
 *//*-------------------------------------------------------------------------*/
void TIMER2n_UpdateCountValue( TIMER2n_Type* TIMER2x, uint8_t CountCh, uint16_t Value )
{
   switch( CountCh )
   {
      case 0:
         TIMER2x->ADR = Value;
         break;
      case 1:
         TIMER2x->BDR = Value;
         break;
   }
}

/*-------------------------------------------------------------------------*//**
 * @brief         Read value of capture register in timer/counter device
 * @param[in]     TIMER2x
 *                   Pointer to a target peripheral.
 *                   -  TIMER20 ~ TIMER21
 * @return        Value of count register
 *//*-------------------------------------------------------------------------*/
uint16_t TIMER2n_GetCountValue( TIMER2n_Type* TIMER2x )
{
   return TIMER2x->CAPDR ;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Configure the peripheral interrupt.
 * @param[in]     TIMER2x
 *                   Pointer to a target peripheral.
 *                   -  TIMER20 ~ TIMER21
 * @param[in]     TIMER2ntCfg
 *                   Specifies the interrupt flag
 *                   -  TIMER2n_CR_MATCH_INTR:     Match interrupt enable
 *                   -  TIMER2n_CR_CAPTURE_INTR:   Capture interrupt enable
 * @param[in]     NewState
 *                   Next State of Interrupt Operation
 *                   -  ENABLE, DISABLE
 * @return        none
 *//*-------------------------------------------------------------------------*/
void TIMER2n_IntConfig( TIMER2n_Type* TIMER2x, TIMER2n_INT_Type TIMER2ntCfg, FunctionalState NewState )
{
   switch( TIMER2ntCfg )
   {
      case TIMER2n_CR_MATCH_INTR:
         if( NewState == ENABLE )
         {
            TIMER2x->CR_b.T2nMIEN = 1;
         }
         else if( NewState == DISABLE )
         {
            TIMER2x->CR_b.T2nMIEN = 0;
         }
         break;
      case TIMER2n_CR_CAPTURE_INTR:
         if( NewState == ENABLE )
         {
            TIMER2x->CR_b.T2nCIEN = 1;
         }
         else if( NewState == DISABLE )
         {
            TIMER2x->CR_b.T2nCIEN = 0;
         }
         break;
   }
}

