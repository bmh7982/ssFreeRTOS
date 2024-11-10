/***************************************************************************//**
 * @file          A31G12x_TIMER3n.c
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
 * @brief         Initialize the TIMER3n peripheral with the specified parameters.
 * @param[in]     TIMER3x
 *                   Pointer to a target peripheral.
 *                   -  TIMER30
 * @param[in]     TIMER3n_Config
 *                   Pointer to the configuration information for the specified peripheral.
 * @return        none
 *//*-------------------------------------------------------------------------*/
void TIMER3n_Init( TIMER3n_Type* TIMER3x, TIMER3n_CFG_Type* TIMER3n_Config )
{
#if 1    // supported
   if( TIMER3x == ( TIMER3n_Type* )TIMER30 )
   {
      // enable peripheral clock
      SCUCG->PPCLKEN1_b.T30CLKE = 1;
   }
#endif

#if 0    // not supported
   if( TIMER3x == ( TIMER3n_Type* )TIMER31 )
   {
      // enable peripheral clock
      SCUCG->PPCLKEN1_b.T31CLKE = 1;
   }
#endif

   // CR
   TIMER3x->CR =  0
                  | ( TIMER3n_Config->T3nMS  << TIMER3n_CR_T3nMS_Pos )
                  | ( TIMER3n_Config->T3nCLK << TIMER3n_CR_T3nCLK_Pos )
                  | ( TIMER3n_Config->T3nECE << TIMER3n_CR_T3nECE_Pos )
                  | ( TIMER3n_Config->T3nCPOL << TIMER3n_CR_T3nCPOL_Pos )
                  ;

   // PDR
   TIMER3x->PDR = TIMER3n_Config->PDR;

   // ADR & BDR & CDR
   TIMER3x->ADR = TIMER3n_Config->ADR;
   TIMER3x->BDR = TIMER3n_Config->BDR;
   TIMER3x->CDR = TIMER3n_Config->CDR;

   // PREDR
   TIMER3x->PREDR = TIMER3n_Config->Prescaler;

   // clear counter and prescaler
   TIMER3n_ClearCounter( TIMER3x );
}

/*-------------------------------------------------------------------------*//**
 * @brief         Interrupt Control Register
 * @param[in]     TIMER3x
 *                   Pointer to a target peripheral.
 *                   -  TIMER30
 * @param[in]     NewState
 *                   Next State of Interrupt Operation
 *                   -  ENABLE, DISABLE
 * @param[in]     T30IntCfg
 *                   Interrupt Sourtce Setting
 * @return        none
 *//*-------------------------------------------------------------------------*/
void TIMER3n_IntConfig( TIMER3n_Type* TIMER3x, uint32_t NewState, uint32_t T30IntCfg )
{
   if( NewState == ENABLE )
   {
      TIMER3x->INTCR |= T30IntCfg;
   }
   else
   {
      TIMER3x->INTCR &= ( ~T30IntCfg ) & 0x7f;
   }
}

/*-------------------------------------------------------------------------*//**
 * @brief         Interrupt Flag Clear
 * @param[in]     TIMER3x
 *                   Pointer to a target peripheral.
 *                   -  TIMER30
 * @return        none
 *//*-------------------------------------------------------------------------*/
uint32_t TIMER3n_GetStatus( TIMER3n_Type* TIMER3x )
{
   return TIMER3x->INTFLAG;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Interrupt Flag Clear
 * @param[in]     TIMER3x
 *                   Pointer to a target peripheral.
 *                   -  TIMER30
 * @param[in]     T30IntCfg
 *                   Select clear interrupt
 * @return        none
 *//*-------------------------------------------------------------------------*/
void TIMER3n_ClearStatus( TIMER3n_Type* TIMER3x, uint32_t T30IntCfg )
{
   TIMER3x->INTFLAG = T30IntCfg;
}

/*-------------------------------------------------------------------------*//**
 * @brief         TIMER3n PWM Mode Setting (Initial : Back to Back Mode, Internal Clock, 6channel Mode)
 * @param[in]     TIMER3x
 *                   Pointer to a target peripheral.
 *                   -  TIMER30
 * @param[in]     updatedata
 *                   PWM Duty Update Timing
 * @param[in]     intcount
 *                   Period Match Interrupt Occurence Seletion
 * @return        none
 *//*-------------------------------------------------------------------------*/
void TIMER3n_MPWMCmd( TIMER3n_Type* TIMER3x, uint32_t updatedata, uint32_t intcount )
{
   TIMER3x->CR = 0
                 | TIMER3n_CLKINT
                 | TIMER3n_BTOB
                 | TIMER3n_6CHMOD
                 | updatedata
                 | intcount
                 | 1             // Clear TIMER3n & Precaler
                 ;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Enable or Disable PWM start
 * @param[in]     TIMER3x
 *                   Pointer to a target peripheral.
 *                   -  TIMER30
 * @param[in]     NewState
 *                   Next State of Functional Operation
 *                   -  ENABLE, DISABLE
 * @return        none
 *//*-------------------------------------------------------------------------*/
void TIMER3n_Start( TIMER3n_Type* TIMER3x, uint32_t NewState )
{
   if( NewState == TIMER3n_ENABLE )
   {
      TIMER3x->CR |= TIMER3n_ENABLE;
   }
   else
   {
      TIMER3x->CR &= ~TIMER3n_ENABLE;
   }
}

/*-------------------------------------------------------------------------*//**
 * @brief         PWM Output Port Control Register (Initial : 6channel enable, output low)
 * @param[in]     TIMER3x
 *                   Pointer to a target peripheral.
 *                   -  TIMER30
 * @param[in]     NewState
 *                   Next State of Functional Operation
 *                   -  ENABLE, DISABLE
 * @param[in]     pwmApol
 *                   Timer 30xA Output Polarity Selection
 * @param[in]     pwmBpol
 *                   Timer 30xB Output Polarity Selection
 * @return        none
 *//*-------------------------------------------------------------------------*/
void TIMER3n_OutputCtrl( TIMER3n_Type* TIMER3x,  uint32_t NewState, uint32_t pwmApol, uint32_t pwmBpol )
{
   uint32_t temp;
   temp = 0
          | ( ( uint32_t )TIMER30_T30_OUTCR_WTIDKY_Value << TIMER30_OUTCR_WTIDKY_Pos )
          | ( pwmApol | pwmBpol )
          ;
   if( NewState == ENABLE )
   {
      temp |= ( 0x3f << 8 );
   }

   TIMER3x->OUTCR = temp;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set Prescaler data.
 * @param[in]     TIMER3x
 *                   Pointer to a target peripheral.
 *                   -  TIMER30
 * @param[in]     prescale
 *                   Timer30 Prescaler Value
 * @return        none
 *//*-------------------------------------------------------------------------*/
void TIMER3n_ClockPrescaler( TIMER3n_Type* TIMER3x, uint32_t prescale )
{
   TIMER3x->PREDR = ( prescale & 0x0fff ); // period. it sould be larger than 0x10
}


/*-------------------------------------------------------------------------*//**
 * @brief         Set period data.
 * @param[in]     TIMER3x
 *                   Pointer to a target peripheral.
 *                   -  TIMER30
 * @param[in]     period
 *                   MPWM period data
 * @return        none
 *//*-------------------------------------------------------------------------*/
void TIMER3n_SetPeriod( TIMER3n_Type* TIMER3x, uint32_t period )
{
   TIMER3x->PDR = ( period & 0xffff ); // period. it sould be larger than 0x10
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set duty A data.
 * @param[in]     TIMER3x
 *                   Pointer to a target peripheral.
 *                   -  TIMER30
 * @param[in]     aduty
 *                   Timer30 Aduty data
 * @return        none
 *//*-------------------------------------------------------------------------*/
void TIMER3n_SetADuty( TIMER3n_Type* TIMER3x, uint32_t aduty )
{
   TIMER3x->ADR = ( ( aduty ) & 0xffff ); // if using I/O control function, set period data
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set duty B data.
 * @param[in]     TIMER3x
 *                   Pointer to a target peripheral.
 *                   -  TIMER30
 * @param[in]     bduty
 *                   Timer30 Bduty data
 * @return        none
 *//*-------------------------------------------------------------------------*/
void TIMER3n_SetBDuty( TIMER3n_Type* TIMER3x, uint32_t bduty )
{
   TIMER3x->BDR = ( ( bduty ) & 0xffff ); // if using I/O control function, set period data
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set duty C data.
 * @param[in]     TIMER3x
 *                   Pointer to a target peripheral.
 *                   -  TIMER30
 * @param[in]     cduty
 *                   Timer30 Cduty data
 * @return        none
 *//*-------------------------------------------------------------------------*/
void TIMER3n_SetCDuty( TIMER3n_Type* TIMER3x, uint32_t cduty )
{
   TIMER3x->CDR = ( ( cduty ) & 0xffff ); // if using I/O control function, set period data
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set dead time (delay time)
 * @param[in]     TIMER3x
 *                   Pointer to a target peripheral.
 *                   -  TIMER30
 * @param[in]     dten
 *                   dead time enable
 * @param[in]     dtpos
 *                   dead timer position (front or back)
 * @param[in]     clkdata
 *                   dead time
 * @return        none
 *//*-------------------------------------------------------------------------*/
void TIMER3n_SetDelayTime( TIMER3n_Type* TIMER3x, uint32_t dten, uint32_t dtpos, uint32_t clkdata )
{
   uint32_t temp;

   temp = TIMER3x->CR;
   temp &= ~( 0x03 << 8 );
   temp |= dten | dtpos;
   TIMER3x->CR = temp;

   TIMER3x->DLY = clkdata & 0x03ff;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set HIZCR Register
 * @param[in]     TIMER3x
 *                   Pointer to a target peripheral.
 *                   -  TIMER30
 * @param[in]     u32T30HizSet
 *                   Timer 30 Output High-Impedance Setting Data
 * @return        none
 * @details       This function sets the Timer 30 HIZ/ADT Control Register
 *//*-------------------------------------------------------------------------*/
void TIMER3n_SetHizReg( TIMER3n_Type* TIMER3x, uint32_t u32T30HizSet )
{
   TIMER3x->HIZCR = u32T30HizSet;  //Setting Timer 30 High-Impedance Control Register
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set ADC Tirgger Source & Timing
 * @param[in]     TIMER3x
 *                   Pointer to a target peripheral.
 *                   -  TIMER30
 * @param[in]     u32triggerpoint
 *                   Timer 30 Output High-Impedance Setting Data
 * @param[in]     u32triggertime
 *                   Timer 30 A/DC Trigger Setting Data
 * @return        none
 * @details       This function sets the Timer 30 HIZ/ADT Control Register
 *//*-------------------------------------------------------------------------*/
void TIMER3n_SetADCTrigger( TIMER3n_Type* TIMER3x, uint32_t u32triggerpoint, uint32_t u32triggertime )
{
   TIMER3x->ADTCR = u32triggerpoint;  //Setting Timer 30 A/DC Trigger Control Register
   TIMER3x->ADTDR = u32triggertime;
}

