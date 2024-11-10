/***************************************************************************//**
 * @file          A31G12x_TIMER1n.h
 * @brief         Driver Header File for Timer
 * @version       1.0
 * @date          2019-09-01
 * @author        ABOV Application Team
 *
 * Copyright (c) 2019, ABOV Semiconductor. All rights reserved.
 *//***************************************************************************/

#ifndef _TIMER1n_H_
#define _TIMER1n_H_

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Constant
//******************************************************************************

//----------   TIMER1n Enable/Disable Definition   ----------
#define TIMER1n_DISABLE          (0x0uL << 15)
#define TIMER1n_ENABLE           (0x1uL << 15)

//----------   TIMER1n Clock Selection Definition   ----------
#define TIMER1n_CLKINT           (0x0uL << 14)
#define TIMER1n_CLKEXT           (0x1uL << 14)

//----------   TIMER1n Mode Selection Definition   ----------
#define TIMER1n_CNTM             (0x0uL << 12)
#define TIMER1n_CAPM             (0x1uL << 12)
#define TIMER1n_PPGONEM          (0x2uL << 12)
#define TIMER1n_PPGREM           (0x3uL << 12)

//----------   TIMER1n External Clock Edge Selection Definition   ----------
#define TIMER1n_FEDGE            (0x0uL << 11)
#define TIMER1n_REDGE            (0x1uL << 11)

//----------   TIMER1n OUT Polarity Selection Definition   ----------
#define TIMER1n_STHIGH           (0x0uL << 8)
#define TIMER1n_STLOW            (0x1uL << 8)

//----------   TIMER1n Capture Polarity Selection Definition   ----------
#define TIMER1n_CAPFALL          (0x0uL << 6)
#define TIMER1n_CAPRISE          (0x1uL << 6)
#define TIMER1n_CAPBOTH          (0x2uL << 6)

//----------   TIMER1n Match Interrupt Definition   ----------
#define TIMER1n_MATINTEN         (0x1uL << 5)
#define TIMER1n_MATINTDIS        (0x0uL << 5)

//----------   TIMER1n Capture Interrupt Definition   ----------
#define TIMER1n_CAPINTEN         (0x1uL << 4)
#define TIMER1n_CAPINTDIS        (0x0uL << 4)

#define TIMER1n_PRS_MASK         0x0FFF

//******************************************************************************
// Type
//******************************************************************************

//==============================================================================
// Enumeration
//==============================================================================

/**   counter clock source select */
typedef enum
{
   TIMER1n_PCLK = 0,             /**<  clock source from pclk */
   TIMER1n_ECn = 1               /**<  clock source from ECn pin input. before setting, have to set ECn pin mode */
} TIMER1n_CKSEL_MODE_OPT;

typedef enum
{
   TIMER1n_PERIODIC_MODE = 0,    /**<  PERIODIC mode */
   TIMER1n_CAPTURE_MODE,         /**<  CAPTURE mode */
   TIMER1n_ONESHOT_MODE,         /**<  ONE SHOT mode */
   TIMER1n_PWM_MODE              /**<  PWM mode */
} TIMER1n_MODE_OPT;

typedef enum
{
   TIMER1n_FALLING_EGDE = 0,     /**<  falling edge clear mode */
   TIMER1n_RISING_EGDE,          /**<  rising edge clear mode */
   TIMER1n_BOTH_EGDE,            /**<  both edge clear  mode */
   TIMER1n_NONE                  /**<  none clear mode */
} TIMER1n_CLR_MODE_OPT;

/**   start default level select: initial output value. */
typedef enum
{
   TIMER1n_START_HIGH = 0,
   TIMER1n_START_LOW
} TIMER1n_STARTLVL_OPT;

/**   TIMER Interrupt Type definitions */
typedef enum
{
   TIMER1n_INTCFG_MIE = 0,    /**<  Match Interrupt enable*/
   TIMER1n_INTCFG_CIE,        /**<  Capture Interrupt enable*/
} TIMER1n_INT_Type;

//==============================================================================
// Structure
//==============================================================================

/**   Configuration structure in TIMER mode */
typedef struct
{
   uint16_t    ADR;
   uint16_t    BDR;
   uint16_t    Prescaler;
   uint8_t     StartLevel;    /**<  set initial output value
                                    -  TIMER1n_START_LOW, TIMER1n_START_HIGH
                              */
   uint8_t     CkSel;         /**<  Counter clock source select
                                    -  PCLK, ECn
                              */
   uint8_t     ECE;
} TIMER1n_PERIODICCFG_Type;

/**   Configuration structure in COUNTER mode */
typedef struct
{
   uint16_t    ADR;
   uint16_t    BDR;
   uint16_t    Prescaler;
   uint8_t     StartLevel;    /**<  set initial output value
                                    -  TIMER1n_START_LOW, TIMER1n_START_HIGH
                              */
   uint8_t     CkSel;         /**<  Counter clock source select
                                    -  PCLK, ECn
                              */
   uint8_t     ECE;
} TIMER1n_PWMCFG_Type, TIMER1n_ONESHOTCFG_Type;

/**   Capture Input configuration structure */
typedef struct
{
   uint16_t    ADR;
   uint16_t    Prescaler;
   uint8_t     ClrMode;
   uint8_t     CkSel;
   uint8_t     ECE;
} TIMER1n_CAPTURECFG_Type;

//******************************************************************************
// Macro
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         TIMER1n Enable/Disable
 * @param[in]     TIMER1x
 *                   -  TIMER10 ~ TIMER16
 * @details       This macro Enable TIMER1n Block
 *//*-------------------------------------------------------------------------*/
#define TIMER1n_EN( TIMER1x )        (TIMER1x->CR_b.T1nEN = 1)
#define TIMER1n_DIS( TIMER1x )       (TIMER1x->CR_b.T1nEN = 0)

/*-------------------------------------------------------------------------*//**
 * @brief         TIMER1n Continue Counting/Temporary Pause
 * @param[in]     TIMER1x
 *                   -  TIMER10 ~ TIMER16
 * @details       This macro select TIMER1n Continue Counting/Temporary Pause
 *//*-------------------------------------------------------------------------*/
#define TIMER1n_ConCnt( TIMER1x )       (TIMER1x->CR_b.T1nPAU = 0)
#define TIMER1n_TempPau( TIMER1x )      (TIMER1x->CR_b.T1nPAU = 1)

/*-------------------------------------------------------------------------*//**
 * @brief         TIMER1n Counter and Prescaler clear
 * @param[in]     TIMER1x
 *                   -  TIMER10 ~ TIMER16
 * @details       This macro clears Counter and Prescalrer of TIMER1n
 *//*-------------------------------------------------------------------------*/
#define TIMER1n_ClrCnt( TIMER1x )       (TIMER1x->CR_b.T1nCLR = 1)

/*-------------------------------------------------------------------------*//**
 * @brief         Get TIMER1n Counter Register
 * @param[in]     TIMER1x
 *                   -  TIMER10 ~ TIMER16
 * @details       This macro gets TIMER1n Counter Register
 *//*-------------------------------------------------------------------------*/
#define TIMER1n_GetCnt( TIMER1x )       (TIMER1x->CNT)

/*-------------------------------------------------------------------------*//**
 * @brief         Set TIMER1n A Data Register
 * @param[in]     TIMER1x
 *                   -  TIMER10 ~ TIMER16
 * @param[in]     u32AData
 *                   A Data of TIMER1n
 * @details       This macro sets TIMER1n A Data Register
 *//*-------------------------------------------------------------------------*/
#define TIMER1n_SetAData( TIMER1x, u32AData )    (TIMER1x->ADR = u32AData)

/*-------------------------------------------------------------------------*//**
 * @brief         Set TIMER1n B Data Register
 * @param[in]     TIMER1x
 *                   -  TIMER10 ~ TIMER16
 * @param[in]     u32BData
 *                   B Data of TIMER1n
 * @details       This macro sets TIMER1n B Data Register
 *//*-------------------------------------------------------------------------*/
#define TIMER1n_SetBData( TIMER1x, u32BData )    (TIMER1x->BDR = u32BData)

/*-------------------------------------------------------------------------*//**
 * @brief         Get TIMER1n Capture Data Register
 * @param[in]     TIMER1x
 *                   -  TIMER10 ~ TIMER16
 * @details       This macro gets TIMER1n Capture Data Register
 *//*-------------------------------------------------------------------------*/
#define TIMER1n_GetCapData( TIMER1x )      (TIMER1x->CAPDR)

/*-------------------------------------------------------------------------*//**
 * @brief         Get flags of TIMER1n Match Interrupt
 * @param[in]     TIMER1x
 *                   -  TIMER10 ~ TIMER16
 * @details       This macro gets interrupt flag of TIMER1n Match Interrupt
 *//*-------------------------------------------------------------------------*/
#define T1nMaInt_GetFg( TIMER1x )          (TIMER1x->CR_b.T1nMIFLAG)

/*-------------------------------------------------------------------------*//**
 * @brief         Clear flags of TIMER1n Match Interrupt
 * @param[in]     TIMER1x
 *                   -  TIMER10 ~ TIMER16
 * @details       This macro clears interrupt flag of TIMER1n Match Interrupt
 *//*-------------------------------------------------------------------------*/
#define T1nMaInt_ClrFg( TIMER1x )          (TIMER1x->CR_b.T1nMIFLAG = 1)

/*-------------------------------------------------------------------------*//**
 * @brief         Get flags of TIMER1n Capture Interrupt
 * @param[in]     TIMER1x
 *                   -  TIMER10 ~ TIMER16
 * @details       This macro gets interrupt flag of TIMER1n Capture Interrupt
 *//*-------------------------------------------------------------------------*/
#define T1nCapInt_GetFg( TIMER1x )         (TIMER1x->CR_b.T1nCIFLAG)

/*-------------------------------------------------------------------------*//**
 * @brief         Clear flags of TIMER1n Capture Interrupt
 * @param[in]     TIMER1x
 *                   -  TIMER10 ~ TIMER16
 * @details       This macro clears interrupt flag of TIMER1n Capture Interrupt
 *//*-------------------------------------------------------------------------*/
#define T1nCapInt_ClrFg( TIMER1x )         (TIMER1x->CR_b.T1nCIFLAG = 1)

/*-------------------------------------------------------------------------*//***
 * @brief         Timer device enumeration
 *//*-------------------------------------------------------------------------*/
/* Timer n Control register  */
#define TIMER1n_CR_CKSEL_MASK          (TIMER1n_CR_T1nCLK_Msk)
#define TIMER1n_CR_CKSEL_SET( n )      (n << TIMER1n_CR_T1nCLK_Pos)

#define TIMER1n_CR_MODE_MASK           (TIMER1n_CR_T1nMS_Msk)
#define TIMER1n_CR_MODE_SET( n )       (n << TIMER1n_CR_T1nMS_Pos)

#define TIMER1n_CR_ECE_MASK            (TIMER1n_CR_T1nECE_Msk)
#define TIMER1n_CR_ECE_SET( n )        (n << TIMER1n_CR_T1nECE_Pos)

#define TIMER1n_CR_STARTLVL_MASK       (TIMER1n_CR_T1nOPOL_Msk)
#define TIMER1n_CR_STARTLVL_SET( n )   (n << TIMER1n_CR_T1nOPOL_Pos)

#define TIMER1n_CR_CPOL_MASK           (TIMER1n_CR_T1nCPOL_Msk)
#define TIMER1n_CR_CPOL_SET( n )       (n << TIMER1n_CR_T1nCPOL_Pos)

//******************************************************************************
// Function
//******************************************************************************

void TIMER1n_SetReg( TIMER1n_Type* TIMER1x, uint32_t u32T1nSet, uint32_t u32T1nClk );

void TIMER1n_Init( TIMER1n_Type* TIMER1x, TIMER1n_MODE_OPT TimerCounterMode, void* TIMER1n_Config );
void TIMER1n_DeInit( TIMER1n_Type* TIMER1x );

void TIMER1n_Cmd( TIMER1n_Type* TIMER1x, FunctionalState NewState );
void TIMER1n_IntConfig( TIMER1n_Type* TIMER1x, TIMER1n_INT_Type TIMERIntCfg, FunctionalState NewState );

#ifdef __cplusplus
}
#endif

#endif   /*_TIMER1n_H_ */

