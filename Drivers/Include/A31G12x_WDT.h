/***************************************************************************//**
 * @file          A31G12x_WDT.h
 * @brief         Driver Header File for Watch-Dog Timer
 * @version       1.0
 * @date          2019-09-01
 * @author        ABOV Application Team
 *
 * Copyright (c) 2019, ABOV Semiconductor. All rights reserved.
 *//***************************************************************************/

#ifndef _WDT_H_
#define _WDT_H_

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Constant
//******************************************************************************

// WDT_CR interrupt enable bit
/* Deprecated
#define WDT_CR_UNFIEN            ((uint32_t)(1 << 2))    // WDT Underflow Interrupt Enable bit
#define WDT_CR_WINMIEN           ((uint32_t)(1 << 3))    // WDT Window Match Interrupt Enable bit
#define WDT_INTERRUPT_BITMASK    0x000c
*/

/**   WDT_SR interrupt enable bit */
#define WDT_SR_UNFIFLAG          ((uint32_t)(1 << 0))    // WDT Underflow Interrupt Flag bit
#define WDT_SR_WINMIFLAG         ((uint32_t)(1 << 1))    // WDT Window Match Interrupt Flag bit

#define WDT_STATUS_BITMASK       0x0003

//******************************************************************************
// Type
//******************************************************************************

//==============================================================================
// Enumeration
//==============================================================================

enum
{
   WDT_DIV_4 = 0,
   WDT_DIV_16,
   WDT_DIV_64,
   WDT_DIV_256
};

typedef enum
{
   WDT_INTCFG_UNFIEN = 0,     /**<  UNFIEN Interrupt enable */
   WDT_INTCFG_WINMIEN,        /**<  WINMIEN Interrupt enable */
} WDT_INT_Type;

//==============================================================================
// Structure
//==============================================================================

typedef struct Wdt_Config
{
   uint8_t     wdtResetEn;       /**<  if ENABLE -> the Reset bit is enabled */
   uint16_t    wdtClkDiv;        /**<  wdtClkDiv */
   uint32_t    wdtTmrConst;      /**<  Set Watch-Dog Timer Data Register */
   uint32_t    wdtWTmrConst;     /**<  Set Watch-Dog Timer Window Data Register */
} WDT_CFG_Type;

//******************************************************************************
// Function
//******************************************************************************

void WDT_Init( WDT_CFG_Type WDT_Config );
void WDT_DeInit( void );
void WDT_ReloadTimeCounter( void );
void WDT_Start( FunctionalState ctrl );
uint32_t WDT_GetCurrentCount( void );
uint32_t WDT_GetStatus( void );
void WDT_ClearStatus( uint32_t clrbit );
void WDT_IntConfig( WDT_INT_Type WDTIntCfg, FunctionalState NewState );

#ifdef __cplusplus
}
#endif

#endif   /* _WDT_H_ */

