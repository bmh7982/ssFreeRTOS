/***************************************************************************//**
 * @file          A31G12x_TIMER1n.c
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
 * @brief         Set TIMER1n CR/PREDR Registers
 * @param[in]     TIMER1x
 *                   Pointer to a target peripheral.
 *                   -  TIMER10 ~ TIMER16
 * @param[in]     u32T1nSet
 *                   TIMER1n Control Register Setting Data
 * @param[in]     u32T1nClk
 *                   TIMER1n Prescaler Data
 * @return        none
 * @details       This function sets the mode, external clock edge, Timer out polarity,
 *                Capture Polarity and Timer match/capture interrupt.
 *//*-------------------------------------------------------------------------*/

void TIMER1n_SetReg( TIMER1n_Type* TIMER1x, uint32_t u32T1nSet, uint32_t u32T1nClk )
{
   TIMER1x->CR = u32T1nSet;      // Setting TIMER1n Control Register

   TIMER1x->PREDR = u32T1nClk;   // Setting TIMER1n Prescaler data
}

/*-------------------------------------------------------------------------*//**
 * @brief         Initialize the TIMER1n peripheral with the specified parameters.
 *                   -  Set Clock frequency for Timer
 *                   -  Set initial configuration for Timer
 * @param[in]     TIMER1x
 *                   Pointer to a target peripheral.
 *                   -  TIMER10 ~ TIMER16
 * @param[in]     TimerCounterMode
 *                   Timer counter mode
 *                   -  TIMER1n_PERIODIC_MODE:  Timer mode
 *                   -  TIMER1n_PWM_MODE:       Counter rising mode
 *                   -  TIMER1n_ONESHOT_MODE:   Counter falling mode
 *                   -  TIMER1n_CAPTURE_MODE:   Counter on both edges
 * @param[in]     TIMER1n_Config
 *                   Pointer to TIM_TIMERCFG_Type
 *                   that contains the configuration information
 *                   for the specified peripheral.
 * @return        none
 *//*-------------------------------------------------------------------------*/
void TIMER1n_Init( TIMER1n_Type* TIMER1x, TIMER1n_MODE_OPT TimerCounterMode, void* TIMER1n_Config )
{
   TIMER1n_PERIODICCFG_Type*  pTimeCfg;
   TIMER1n_PWMCFG_Type*       pPwmOneshotCfg;
   TIMER1n_CAPTURECFG_Type*   pCaptureCfg;
   uint16_t                   reg_val16;

   //set power
   if( TIMER1x == ( TIMER1n_Type* )TIMER10 )
   {
      SCUCG_SetEachPeriClk1( PPCLKEN1_T10CLKE, PPxCLKE_Enable );
   }
   else if( TIMER1x == ( TIMER1n_Type* )TIMER11 )
   {
      SCUCG_SetEachPeriClk1( PPCLKEN1_T11CLKE, PPxCLKE_Enable );
   }
   else if( TIMER1x == ( TIMER1n_Type* )TIMER12 )
   {
      SCUCG_SetEachPeriClk1( PPCLKEN1_T12CLKE, PPxCLKE_Enable );
   }
   else if( TIMER1x == ( TIMER1n_Type* )TIMER13 )
   {
      SCUCG_SetEachPeriClk1( PPCLKEN1_T13CLKE, PPxCLKE_Enable );
   }
   else if( TIMER1x == ( TIMER1n_Type* )TIMER14 )
   {
      SCUCG_SetEachPeriClk1( PPCLKEN1_T14CLKE, PPxCLKE_Enable );
   }
   else if( TIMER1x == ( TIMER1n_Type* )TIMER15 )
   {
      SCUCG_SetEachPeriClk1( PPCLKEN1_T15CLKE, PPxCLKE_Enable );
   }
   else if( TIMER1x == ( TIMER1n_Type* )TIMER16 )
   {
      SCUCG_SetEachPeriClk1( PPCLKEN1_T16CLKE, PPxCLKE_Enable );
   }

   if( TimerCounterMode == TIMER1n_PERIODIC_MODE )
   {
      pTimeCfg = ( TIMER1n_PERIODICCFG_Type* )TIMER1n_Config;

      reg_val16 = 0
                  | TIMER1n_CR_CKSEL_SET( pTimeCfg->CkSel )
                  | TIMER1n_CR_MODE_SET( TimerCounterMode )
                  | TIMER1n_CR_STARTLVL_SET( pTimeCfg->StartLevel )
                  ;
      if( pTimeCfg->CkSel == 1 )
      {
         reg_val16 = reg_val16 | TIMER1n_CR_ECE_SET( pTimeCfg->ECE );
      }
      TIMER1x->CR = reg_val16;

      TIMER1x->PREDR = ( ( pTimeCfg->Prescaler - 1 ) & TIMER1n_PRS_MASK );
      TIMER1x->ADR = pTimeCfg->ADR;
   }
   else if( ( TimerCounterMode == TIMER1n_PWM_MODE ) || ( TimerCounterMode == TIMER1n_ONESHOT_MODE ) )
   {
      pPwmOneshotCfg = ( TIMER1n_PWMCFG_Type* )TIMER1n_Config;

      reg_val16 = 0
                  | TIMER1n_CR_CKSEL_SET( pPwmOneshotCfg->CkSel )
                  | TIMER1n_CR_MODE_SET( TimerCounterMode )
                  | TIMER1n_CR_STARTLVL_SET( pPwmOneshotCfg->StartLevel )
                  ;
      if( pPwmOneshotCfg->CkSel == 1 )
      {
         reg_val16 = reg_val16 | TIMER1n_CR_ECE_SET( pPwmOneshotCfg->ECE );
      }
      TIMER1x->CR = reg_val16;

      TIMER1x->PREDR = ( ( pPwmOneshotCfg->Prescaler - 1 ) & TIMER1n_PRS_MASK );
      TIMER1x->ADR = pPwmOneshotCfg->ADR;
      TIMER1x->BDR = pPwmOneshotCfg->BDR;
   }
   else if( TimerCounterMode == TIMER1n_CAPTURE_MODE )
   {
      pCaptureCfg = ( TIMER1n_CAPTURECFG_Type* )TIMER1n_Config;

      reg_val16 = 0
                  | TIMER1n_CR_CKSEL_SET( pCaptureCfg->CkSel )
                  | TIMER1n_CR_MODE_SET( TimerCounterMode )
                  | TIMER1n_CR_CPOL_SET( pCaptureCfg->ClrMode )
                  ;
      if( pCaptureCfg->CkSel == 1 )
      {
         reg_val16 = reg_val16 | TIMER1n_CR_ECE_SET( pCaptureCfg->ECE );
      }
      TIMER1x->CR = reg_val16;

      TIMER1x->PREDR = ( ( pCaptureCfg->Prescaler - 1 ) & TIMER1n_PRS_MASK );
      TIMER1x->ADR = pCaptureCfg->ADR;
   }
   TIMER1x->CR |= 0x1; //  timer counter clear
}

/*-------------------------------------------------------------------------*//**
 * @brief         Close Timer/Counter device
 * @param[in]     TIMER1x
 *                   Pointer to a target peripheral.
 *                   -  TIMER10 ~ TIMER16
 * @return        none
 *//*-------------------------------------------------------------------------*/
void TIMER1n_DeInit( TIMER1n_Type* TIMER1x )
{
   // Disable timer/counter
   TIMER1x->CR = 0x00;

   if( TIMER1x == ( TIMER1n_Type* )TIMER10 )
   {
      SCUCG_SetEachPeriClk1( PPCLKEN1_T10CLKE, PPxCLKE_Disable );
   }
   else if( TIMER1x == ( TIMER1n_Type* )TIMER11 )
   {
      SCUCG_SetEachPeriClk1( PPCLKEN1_T11CLKE, PPxCLKE_Disable );
   }
   else if( TIMER1x == ( TIMER1n_Type* )TIMER12 )
   {
      SCUCG_SetEachPeriClk1( PPCLKEN1_T12CLKE, PPxCLKE_Disable );
   }
   else if( TIMER1x == ( TIMER1n_Type* )TIMER13 )
   {
      SCUCG_SetEachPeriClk1( PPCLKEN1_T13CLKE, PPxCLKE_Disable );
   }
   else if( TIMER1x == ( TIMER1n_Type* )TIMER14 )
   {
      SCUCG_SetEachPeriClk1( PPCLKEN1_T14CLKE, PPxCLKE_Disable );
   }
   else if( TIMER1x == ( TIMER1n_Type* )TIMER15 )
   {
      SCUCG_SetEachPeriClk1( PPCLKEN1_T15CLKE, PPxCLKE_Disable );
   }
   else if( TIMER1x == ( TIMER1n_Type* )TIMER16 )
   {
      SCUCG_SetEachPeriClk1( PPCLKEN1_T16CLKE, PPxCLKE_Disable );
   }
}

/*-------------------------------------------------------------------------*//**
 * @brief         Start/Stop Timer/Counter device
 * @param[in]     TIMER1x
 *                   Pointer to a target peripheral.
 *                   -  TIMER10 ~ TIMER16
 * @param[in]     NewState
 *                   Next State of Functional Operation
 *                   -  ENABLE, DISABLE
 * @return        none
 *//*-------------------------------------------------------------------------*/
void TIMER1n_Cmd( TIMER1n_Type* TIMER1x, FunctionalState NewState )
{
   if( NewState == ENABLE )
   {
      TIMER1x->CR |=  TIMER1n_ENABLE;
   }
   else
   {
      TIMER1x->CR &= ~TIMER1n_ENABLE;
   }
}

/*-------------------------------------------------------------------------*//**
 * @brief         Configure the peripheral interrupt.
 * @param[in]     TIMER1x
 *                   Pointer to a target peripheral.
 *                   -  TIMER10 ~ TIMER16
 * @param[in]     TIMERIntCfg
 *                   Specifies the interrupt flag
 *                   -  TIMER1n_INTCFG_MIE:  OVIE Interrupt enable
 *                   -  TIMER1n_INTCFG_CIE:  MBIE Interrupt enable
 * @param[in]     NewState
 *                   Next State of Interrupt Operation
 *                   -  ENABLE, DISABLE
 * @return        none
 *//*-------------------------------------------------------------------------*/
void TIMER1n_IntConfig( TIMER1n_Type* TIMER1x, TIMER1n_INT_Type TIMERIntCfg, FunctionalState NewState )
{
   uint8_t tmp;

   switch( TIMERIntCfg )
   {
      case TIMER1n_INTCFG_MIE:
         tmp = TIMER1n_MATINTEN;
         break;
      case TIMER1n_INTCFG_CIE:
         tmp = TIMER1n_CAPINTEN;
         break;
   }

   if( NewState == ENABLE )
   {
      TIMER1x->CR |= tmp;
   }
   else
   {
      TIMER1x->CR &= ( ~tmp );
   }
}

