/***************************************************************************//**
 * @file          A31G12x_ADC.c
 * @brief         Driver Source File for ADC
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
 * @brief         Initialize the ADC peripheral with the specified parameters.
 * @param[in]     ADC_Config
 *                   Pointer to a ADC_CFG_Type structure
 *                   that contains the configuration information
 *                   for the specified peripheral.
 * @return        none
 *//*-------------------------------------------------------------------------*/
void ADC_Init( ADC_CFG_Type* ADC_Config )
{
   uint32_t tempreg;

   SCUCG_SetEachPeriClk2( PPCLKEN2_ADCLKE, PPxCLKE_Enable );

   ADC->CR = ( 1 << ADC_CR_ADCEN_Pos ); // ADCEN;

   tempreg = 0
             | ( 1 << ADC_CR_ADCEN_Pos ) // ADCEN
             | ( ( ( ADC_Config->TrgSel ) & 7 ) << ADC_CR_TRIG_Pos ) // TRGSRC
             | ( ( ( ADC_Config->RefSel ) & 1 ) << ADC_CR_REFSEL_Pos )
             | ( 1 << ADC_CR_ADCIFLAG_Pos ) // clear flag
             ;
   ADC->CR = tempreg;

   ADC->PREDR = ( ADC_Config->InClkDiv & ADC_PREDR_PRED_Msk );
}

/*-------------------------------------------------------------------------*//**
 * @brief         Close ADC
 * @return        none
 *//*-------------------------------------------------------------------------*/
void ADC_DeInit( void )
{
   ADC->CR = 0;
   SCUCG_SetEachPeriClk2( PPCLKEN2_ADCLKE, PPxCLKE_Disable );
}

/*-------------------------------------------------------------------------*//**
 * @brief         Get Result conversion from A/D data register
 * @return        Result of conversion
 *//*-------------------------------------------------------------------------*/
uint16_t ADC_GetData( void )
{
   uint16_t adc_value;

   adc_value = ADC->DR;
   return ADC_DR_RESULT( adc_value );
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set start mode for ADC
 * @return        none
*********************************************************************/
void ADC_StartCmd( void )
{
   ADC->CR |= ( 1 << ADC_CR_ADST_Pos );
}

/*-------------------------------------------------------------------------*//**
 * @brief         ADC interrupt configuration
 * @param[in]     NewState
 *                   Next State of Interrupt Operation
 *                   -  ENABLE, DISABLE
 * @return        none
 *//*-------------------------------------------------------------------------*/
void ADC_IntConfig( FunctionalState NewState )
{
   uint32_t tempreg;

   tempreg = ADC->CR;
   tempreg &= ~( 1 << ADC_CR_ADCIEN_Pos );

   if( NewState )
   {
      tempreg |= ( 1 << ADC_CR_ADCIEN_Pos );
   }
   ADC->CR = tempreg;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Select ADC Channel Number
 * @param[in]     Channel
 *                   Channel Number
 * @return        none
 *//*-------------------------------------------------------------------------*/
void ADC_ChannelSel( uint32_t Channel )
{
   uint32_t temp_reg;

   temp_reg = ADC->CR & ( uint32_t )( ~ADC_CR_ADSEL_Msk );
   temp_reg |= ( uint32_t )( Channel & ADC_CR_ADSEL_Msk );
   ADC->CR = temp_reg;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Get ADC channel status
 * @return        ADC status register
 *//*-------------------------------------------------------------------------*/
uint32_t ADC_GetStatus( void )
{
   uint32_t tempreg;

   tempreg = ADC->CR;
   tempreg &= ( 1 << ADC_CR_ADCIFLAG_Pos );

   return tempreg;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Clear ADC channel status
 * @return        none
 *//*-------------------------------------------------------------------------*/
void ADC_ClearStatus( void )
{
   uint32_t tempreg;

   tempreg = ADC->CR;
   tempreg |= ( 1 << ADC_CR_ADCIFLAG_Pos );

   ADC->CR = tempreg;  // clear flag;
}

