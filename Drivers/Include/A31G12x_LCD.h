/***************************************************************************//**
 * @file          A31G12x_LCD.h
 * @brief         Driver Header File for LCD Driver
 * @version       1.0
 * @date          2019-09-01
 * @author        ABOV Application Team
 *
 * Copyright (c) 2019, ABOV Semiconductor. All rights reserved.
 *//***************************************************************************/

#ifndef _LCD_H_
#define _LCD_H_

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Constant
//******************************************************************************

//==========   LCD_CR   ========================================

//----------   Internal LCD Bias Dividing Resistor Selection   ----------
#define LCD_RLCD3       (0x0uL << 6)
#define LCD_RLCD1       (0x1uL << 6)
#define LCD_RLCD2       (0x2uL << 6)
#define LCD_RLCD4       (0x3uL << 6)

//----------   LCD Duty and Bias Selection   ----------
#define LCD_8D4B        (0x0uL << 3)
#define LCD_6D4B        (0x1uL << 3)
#define LCD_5D3B        (0x2uL << 3)
#define LCD_4D3B        (0x3uL << 3)
#define LCD_3D3B        (0x4uL << 3)
#define LCD_3D2B        (0x5uL << 3)

//----------   LCD Clock Selection   ----------
#define LCD_CLK128      (0x0uL << 1)
#define LCD_CLK256      (0x1uL << 1)
#define LCD_CLK512      (0x2uL << 1)
#define LCD_CLK1024     (0x3uL << 1)

//==========   LCD_BCCR   ========================================

//----------   LCD Automatic Bias Control En/Disable   ----------
#define LCD_ABCDIS      (0x0uL << 12)
#define LCD_ABCEN       (0x1uL << 12)

//----------   "Bias Mode A" time Selection   ----------
#define LCD_BIA1CLK     (0x0uL << 8)
#define LCD_BIA2CLK     (0x1uL << 8)
#define LCD_BIA3CLK     (0x2uL << 8)
#define LCD_BIA4CLK     (0x3uL << 8)
#define LCD_BIA5CLK     (0x4uL << 8)
#define LCD_BIA6CLK     (0x5uL << 8)
#define LCD_BIA7CLK     (0x6uL << 8)
#define LCD_BIA8CLK     (0x7uL << 8)

//----------   LCD Contrast Control En/Disable   ----------
#define LCD_CONTDIS     (0x0uL << 5)
#define LCD_CONTEN      (0x1uL << 5)

//----------   VLC0 Voltage Control   ----------
#define LCD_VLC31       (0x0uL << 0)
#define LCD_VLC30       (0x1uL << 0)
#define LCD_VLC29       (0x2uL << 0)
#define LCD_VLC28       (0x3uL << 0)
#define LCD_VLC27       (0x4uL << 0)
#define LCD_VLC26       (0x5uL << 0)
#define LCD_VLC25       (0x6uL << 0)
#define LCD_VLC24       (0x7uL << 0)
#define LCD_VLC23       (0x8uL << 0)
#define LCD_VLC22       (0x9uL << 0)
#define LCD_VLC21       (0xAuL << 0)
#define LCD_VLC20       (0xBuL << 0)
#define LCD_VLC19       (0xCuL << 0)
#define LCD_VLC18       (0xDuL << 0)
#define LCD_VLC17       (0xEuL << 0)
#define LCD_VLC16       (0xFuL << 0)

//----------   Constant   ----------
#define LCDBufSize      28

//******************************************************************************
// Type
//******************************************************************************

//==============================================================================
// Structure
//==============================================================================

typedef struct
{
   /* LCD_CR */
   uint32_t    Bias;          /**<  RestartEn = ENABLE or DISABLE */
   uint32_t    Duty;
   uint32_t    Clk;

   /* LCD_BCCR */
   uint32_t    AutoBiasEn;
   uint32_t    BiasTime;
   uint32_t    Contrast;
   uint32_t    ContrastStep;
} LCD_CFG_Type;

//******************************************************************************
// Macro
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         LCD Enable
 * @details       This macro Enable LCD Block
 *//*-------------------------------------------------------------------------*/
#define LCDON()         (LCD->CR_b.DISP = 1)

/*-------------------------------------------------------------------------*//**
 * @brief         LCD Disable
 * @details       This macro Disable LCD Block
 *//*-------------------------------------------------------------------------*/
#define LCDOFF()        (LCD->CR_b.DISP = 0)

//******************************************************************************
// Function
//******************************************************************************

void LCD_Init( LCD_CFG_Type* LCD_Config );
void LCD_SetReg( uint32_t u32LCD_CR, uint32_t u32LCD_BCCR );
void LCD_ClearDspRam( void );
void LCD_WriteDspRam( uint8_t* write_buf, uint32_t u32Index, uint32_t size );

#ifdef __cplusplus
}
#endif

#endif   /* _LCD_H_ */

