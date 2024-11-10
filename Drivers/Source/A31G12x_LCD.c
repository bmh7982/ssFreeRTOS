/***************************************************************************//**
 * @file          A31G12x_LCD.c
 * @brief         Driver Source File for LCD Driver
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
 * @brief         Initialize the LCD peripheral with the specified parameters.
 * @param[in]     LCD_Config
 *                   Pointer to LCD_CFG_Type
 *                   that contains the configuration information
 *                   for the specified peripheral.
 * @return        none
 * @details       This function sets the LCD Bias Resistor, LCD Duty,
 *                LCD Clock, Automatic Bias Control, and Contrast Control Data.
 *//*-------------------------------------------------------------------------*/
void LCD_Init( LCD_CFG_Type* LCD_Config )
{
   LCD->CR = 0
             | LCD_Config->Bias
             | LCD_Config->Duty
             | LCD_Config->Clk
             ;

   LCD->BCCR = 0
               | LCD_Config->AutoBiasEn
               | LCD_Config->BiasTime
               | LCD_Config->Contrast
               | LCD_Config->ContrastStep
               ;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set LCD LCD_CR/LCD_BCCR Registers
 * @param[in]     u32LCD_CR
 *                   LCD Driver Control Register Setting Data
 * @param[in]     u32LCD_BCCR
 *                   LCD Automatic bias and Contrast Control Register Setting Data
 * @return        none
 * @details       This function sets the LCD Bias Resistor, LCD Duty,
 *                LCD Clock, Automatic Bias Control, and Contrast Control Data.
 *//*-------------------------------------------------------------------------*/
void LCD_SetReg( uint32_t u32LCD_CR, uint32_t u32LCD_BCCR )
{
   LCD->CR = u32LCD_CR;       // Set LCD Driver Control Register
   LCD->BCCR = u32LCD_BCCR;   // Set LCD Driver Automatic Bias and Contrast Control Register
}

/*-------------------------------------------------------------------------*//**
 * @brief         Clear LCD Buffer
 * @return        none
 * @details       This function clear Display Data RAM.
 *//*-------------------------------------------------------------------------*/
void LCD_ClearDspRam( void )
{
   uint8_t              u32Index;
   volatile uint8_t*    lcd_dsp_ram;

   lcd_dsp_ram = &LCD->DR0;
   for( u32Index = 0; u32Index < LCDBufSize; u32Index ++ )
   {
      lcd_dsp_ram[u32Index] = 0;
   }
}

/*-------------------------------------------------------------------------*//**
 * @brief         Write LCD Buffer
 * @param[in]     write_buf
 *                   Font Data Buffer Address
 * @param[in]     u32Index
 *                   Font Data Buffer Index
 * @param[in]     size
 *                   Font Data Buffer Size
 * @return        none
 * @details       This function writes display data to Display Data RAM.
 *//*-------------------------------------------------------------------------*/
void LCD_WriteDspRam( uint8_t* write_buf, uint32_t u32Index, uint32_t size )
{
   volatile uint8_t*    lcd_dsp_ram;
   uint32_t             i;

   if( ( u32Index + size ) > LCDBufSize )    // Check Buffer size
   {
      size = size - ( ( u32Index + size ) - LCDBufSize );
   }

   lcd_dsp_ram = &LCD->DR0;
   for( i = 0; i < size; i ++ )
   {
      lcd_dsp_ram[u32Index + i] = write_buf[i];
   }
}

