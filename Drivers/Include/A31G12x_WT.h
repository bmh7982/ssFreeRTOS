/***************************************************************************//**
 * @file          A31G12x_WT.h
 * @brief         Driver Header File for Watch Timer
 * @version       1.0
 * @date          2019-09-01
 * @author        ABOV Application Team
 *
 * Copyright (c) 2019, ABOV Semiconductor. All rights reserved.
 *//***************************************************************************/

#ifndef _WT_H_
#define _WT_H_

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Constant
//******************************************************************************

//==========   WT_CR   ========================================

//----------   WT Interval Selection   ----------
#define WT_DIV2E7          (0x0uL << 4)
#define WT_DIV2E13         (0x1uL << 4)
#define WT_DIV2E14         (0x2uL << 4)
#define WT_DIV2E14DR       (0x3uL << 4)

//----------   WT Interrupt En/Disable   ----------
#define WT_INTDIS          (0x0uL << 3)
#define WT_INTEN           (0x1uL << 3)

/**   WT_CR interrupt enable bit */
#define WT_CR_WTIEN              ((uint32_t)(1 << 3))    // WT Interrupt Enable bit
#define WT_INTERRUPT_BITMASK     0x0008

/**   WT_CR interrupt status flag bit */
#define WT_CR_WTIFLAG            ((uint32_t)(1 << 0))    // WT Interrupt Flag bit
#define WT_STATUS_BITMASK        0x0002

#define WT_CR_WTCLR              ((uint32_t)(1 << 0))    // WT counter and divider clear bit

//******************************************************************************
// Type
//******************************************************************************

//==============================================================================
// Enumeration
//==============================================================================

enum
{
   WT_DIV_2_7 = 0,
   WT_DIV_2_13,
   WT_DIV_2_14,
   WT_DIV_2_14_MUL_DR
};

//==============================================================================
// Structure
//==============================================================================

typedef struct Wt_Config
{
   uint32_t    wtClkDiv;      /**<  wtClkDiv */
   uint32_t    wtTmrConst;    /**<  Set Watch Timer Data Register */
} WT_CFG_Type;

//******************************************************************************
// Macro
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         Watch Timer Enable/Disable
 * @details       This macro Enable Watch Timer Block
 *//*-------------------------------------------------------------------------*/
#define WT_EN()      (WT->CR_b.WTEN = 1)
#define WT_DIS()     (WT->CR_b.WTEN = 0)

/*-------------------------------------------------------------------------*//**
 * @brief         Get Watch Timer Counter Register
 * @details       This macro gets Watch Timer Counter Register
 *//*-------------------------------------------------------------------------*/
#define WT_GetCnt()     (WT->CNT)

/*-------------------------------------------------------------------------*//**
 * @brief         Set Watch Timer Data Register
 * @param[in]     u32WTData
 *                   Data of WT_DR
 * @details       This macro sets Watch Timer Data Register
 *//*-------------------------------------------------------------------------*/
#define WT_SetWT_DR( u32WTData )    (WT->DR = u32WTData)

/*-------------------------------------------------------------------------*//**
 * @brief         Get flags of Watch Timer Interrupt
 * @details       This macro gets interrupt flag of Watch Timer Interrupt
 *//*-------------------------------------------------------------------------*/
#define WTInt_GetFg()      (WT->CR_b.WTIFLAG)

/*-------------------------------------------------------------------------*//**
 * @brief         Clear flags of Watch Timer Interrupt
 * @details       This macro clears interrupt flag of Watch Timer Interrupt
 *//*-------------------------------------------------------------------------*/
#define WTInt_ClrFg()      (WT->CR_b.WTIFLAG = 1)

//******************************************************************************
// Function
//******************************************************************************

void WT_SetReg( uint32_t u32WTSet );
void WT_Init( WT_CFG_Type WT_Config );
void WT_DeInit( void );
void WT_Start( FunctionalState ctrl );
uint32_t WT_GetCurrentCount( void );
uint32_t WT_GetStatus( void );
void WT_ClearStatus( void );
void WT_IntConfig( FunctionalState NewState );

#ifdef __cplusplus
}
#endif

#endif   /*_WT_H_ */

