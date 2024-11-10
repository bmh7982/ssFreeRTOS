/***************************************************************************//**
 * @file          A31G12x_TIMER2n.h
 * @brief         Driver Header File for Timer
 * @version       1.0
 * @date          2019-09-01
 * @author        ABOV Application Team
 *
 * Copyright (c) 2019, ABOV Semiconductor. All rights reserved.
 *//***************************************************************************/

#ifndef _TIMER2n_H_
#define _TIMER2n_H_

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Constant
//******************************************************************************

//--------------------------------------
// TIMER2n Control Register definitions
//--------------------------------------

/**   Counter/timer mode bits */
#define TIMER2n_CR_MODE_MASK        (0x3)

/**   counter clock select bits */
#define TIMER2n_CR_CLK_MASK         (TIMER2n_CR_T2nCLK_Msk)

/**   External Clock Edge Selection bit */
#define TIMER2n_CR_ECE_MASK         (TIMER2n_CR_T2nECE_Msk)

/**   T2nCAP signal selection bit */
#define TIMER2n_CR_CAPSEL_MASK      (0x03 << 9)

/**   T2nOUT Polarity Selection bit */
#define TIMER2n_CR_OPOL_MASK        (TIMER2n_CR_T2nOPOL_Msk)

/**   TIMER2n Capture Polarity Selection bit */
#define TIMER2n_CR_CPOL_MASK        (TIMER2n_CR_T2nCPOL_Msk)

/**   TIMER2n Match Interrupt Enable bit */
#define TIMER2n_CR_MIEN_MASK        (TIMER2n_CR_T2nMIEN_Msk)

/**   TIMER2n Capture Interrupt Enable bit */
#define TIMER2n_CR_CIEN_MASK        (TIMER2n_CR_T2nCIEN_Msk)

/**   TIMER2n Counter Temporary Pause Control bit */
#define TIMER2n_CR_PAU_MASK         (TIMER2n_CR_T2nPAU_Msk)

/**   TIMER2n Match Interrupt Flag */
#define TIMER2n_CR_MATCH_FLAG       (1 << 3)
#define TIMER2n_CR_CAPTURE_FLAG     (1 << 2)

/**   Clear Flag */
#define TIMER2n_CR_CLEAR_MATCHINT      (0x01 << TIMER2n_CR_T2nMIFLAG_Pos)
#define TIMER2n_CR_CLEAR_CAPTUREINT    (0x01 << TIMER2n_CR_T2nCIFLAG_Pos)
#define TIMER2n_CR_CLEAR_CNT_PRED      (0x01 << TIMER2n_CR_T2nCLR_Pos)

/**   TIMER2n A Data Register (ADR) */
#define TIMER2n_ADR_MASK         0xFFFFFFFF

/**   TIMER2n B Data Register (BDR) */
#define TIMER2n_BDR_MASK         0xFFFFFFFF

/**   TIMER2n Prescaler Data Register (PREDR) */
#define TIMER2n_PREDR_MASK       (0xFFF << 0)

/**   Timer/counter enable bit */
#define TIMER2n_ENABLE           (1)
#define TIMER2n_DISABLE          (0)

/**   Timer/counter reset bit */
#define TIMER2n_CLEAR            (1)

//******************************************************************************
// Type
//******************************************************************************

//==============================================================================
// Enumeration
//==============================================================================

/**   Timer operating mode */
typedef enum
{
   TIMER2n_PERIODIC_MODE = 0,    /**<  PERIODIC mode */
   TIMER2n_CAPTURE_MODE,         /**<  PWM mode */
   TIMER2n_ONESHOT_MODE,         /**<  ONE SHOT mode */
   TIMER2n_PWM_MODE              /**<  CAPTURE mode */
} TIMER2n_MODE_OPT;

/**   clear select when capture mode */
typedef enum
{
   TIMER2n_FALLING_EGDE = 0,     /**<  rising edge clear mode */
   TIMER2n_RISING_EGDE,          /**<  falling edge clear mode */
   TIMER2n_BOTH_EGDE,            /**<  both edge clear  mode */
   TIMER2n_NONE                  /**<  none clear mode */
} TIMER2n_CLR_MODE_OPT;

/**   Timer clock source select */
typedef enum
{
   TIMER2n_XSOSC_CLK = 0,        /**<  clock source from Internal */
   TIMER2n_PCLK_CLK,             /**<  clock source from External */
} TIMER2n_MASTER_CKSEL_OPT;

/**   counter clock source select */
typedef enum
{
   TIMER2n_INTERNAL_CLK = 0,     /**<  clock source from Internal */
   TIMER2n_EXTERNAL_CLK,         /**<  clock source from External */
} TIMER2n_CKSEL_MODE_OPT;

/**   capture clock source select */
typedef enum
{
   TIMER2n_CAP_EXTERNAL_CLK = 0,    /**<  clock source from Externl */
   TIMER2n_CAP_XSOSC_CLK,           /**<  clock source from XSOSC */
   TIMER2n_CAP_WDTRC_CLK,           /**<  Clock Source from WDTRC */
} TIMER2n_CAP_CKSEL_OPT;

/**   start default level select: initial output value. */
typedef enum
{
   TIMER2n_START_HIGH = 0,    /**<  clock source from pclk div 2 */
   TIMER2n_START_LOW          /**<  clock source from pclk div 4 */
} TIMER2n_STARTLVL_OPT;

/**   TIMER Interrupt Type definitions */
typedef enum
{
   TIMER2n_CR_MATCH_INTR = 0,    /**<  OVIE Interrupt enable*/
   TIMER2n_CR_CAPTURE_INTR,      /**<  MBIE Interrupt enable*/
} TIMER2n_INT_Type;

//==============================================================================
// Structure
//==============================================================================

/**   Configuration structure in TIMER mode */
typedef struct
{
   uint16_t    ADR;
   uint16_t    BDR;
   uint8_t     TCLK_SEL;         /**<  Timer Clock Source, should be:
                                       -  XSOSC
                                       -  PCLK
                                 */
   uint16_t    Prescaler;        /**<  Timer Prescaler(TnPRS), should be:
                                       -  0~1023  value range
                                 */
   uint8_t     CkSel;            /**<  Counter clock source select, should be:
                                       -  Internal
                                       -  EXT : EXT,  clock source
                                 */
   uint8_t     ExtCkEdge;        /**<  Set External Clock Edge Selection bit:
                                       -  Falling Edge
                                       -  Rising Edge
                                 */
   uint8_t     StartLevel;       /**<  set initial output value, should be:
                                       -  TIMER2n_START_LOW
                                       -  TIMER2n_START_HIGH
                                 */
   uint8_t     CapEdge;          /**<  Set Capture Edge, should be:
                                       -  Falling Edge
                                       -  Rising Edge
                                       -  Both Edge
                                 */
} TIMER2n_PERIODICCFG_Type;

/**   Configuration structure in COUNTER mode */
typedef struct
{
   uint16_t    ADR;
   uint16_t    BDR;
   uint8_t     TCLK_SEL;         /**<  Timer Clock Source, should be:
                                       -  XSOSC
                                       -  PCLK
                                 */
   uint16_t    Prescaler;        /**<  Timer Prescaler(TnPRS), should be:
                                       -  0~1023  value range
                                 */
   uint8_t     CkSel;            /**<  Counter clock source select, should be:
                                       -  Internal
                                       -  EXT : EXT,  clock source
                                 */
   uint8_t     ExtCkEdge;        /**<  Set External Clock Edge Selection bit:
                                       -  Falling Edge
                                       -  Rising Edge
                                 */
   uint8_t     StartLevel;       /**<  set initial output value, should be:
                                       -  TIMER2n_START_LOW
                                       -  TIMER2n_START_HIGH
                                 */
   uint8_t     CapEdge;          /**<  Set Capture Edge, should be:
                                       -  Falling Edge
                                       -  Rising Edge
                                       -  Both Edge
                                 */
} TIMER2n_PWMCFG_Type, TIMER2n_ONESHOTCFG_Type;

/**   Capture Input configuration structure */
typedef struct
{
   uint8_t     TCLK_SEL;         /**<  Timer Clock Source, should be:
                                       -  XSOSC
                                       -  PCLK
                                 */
   uint16_t    Prescaler;        /**<  Timer Prescaler(TnPRS), should be:
                                       -  0~1023  value range
                                 */
   uint8_t     ClrMode;          /**<  clear select when capture, should be:
                                       -  TIMER2n_RISING_EGDE
                                       -  TIMER2n_FALLING_EGDE
                                       -  TIMER2n_BOTH_EGDE
                                       -  TIMER2n_NONE
                                 */
   uint8_t     CkSel;            /**<  Counter clock source select, should be:
                                       -  PCLK_2:     PCLK / 2
                                       -  PCLK_4:     PCLK / 4
                                       -  PCLK_16:    PCLK / 16
                                       -  PCLK_64:    PCLK / 64
                                       -  EXT = 4:    EXT, clock source from MCCR3(TEXT) clock
                                       -  TnC
                                 */
   uint8_t     CAPCkSel;         /**<  Capture clock source select, should be:
                                       -  External Capture Signal
                                       -  XSOSC
                                       -  WDTRC
                                 */
   uint8_t     Reserved[2];      /**<  Reserved */
} TIMER2n_CAPTURECFG_Type;

//******************************************************************************
// Macro
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         TIMER2n Counter and Prescaler clear
 * @param[in]     TIMER2x
 *                   -  TIMER20 ~ TIMER21
 * @details       This macro clears Counter and Prescalrer of TIMER2n
 *//*-------------------------------------------------------------------------*/
#define TIMER2n_ClrCnt( TIMER2x )       (TIMER2x->CR_b.T2nCLR = 1)

/*-------------------------------------------------------------------------*//**
 * @brief         Set TIMER2n A Data Register
 * @param[in]     TIMER1x
 *                   -  TIMER20 ~ TIMER21
 * @param[in]     u32AData
 *                   A Data of TIMER2n
 * @details       This macro sets TIMER2n A Data Register
 *//*-------------------------------------------------------------------------*/
#define TIMER2n_SetAData( TIMER2x, u32AData )    (TIMER2x->ADR = u32AData)

/*-------------------------------------------------------------------------*//**
 * @brief         Set TIMER2n B Data Register
 * @param[in]     TIMER1x
 *                   -  TIMER20 ~ TIMER21
 * @param[in]     u32BData
 *                   B Data of TIMER2n
 * @details       This macro sets TIMER2n B Data Register
 *//*-------------------------------------------------------------------------*/
#define TIMER2n_SetBData( TIMER2x, u32BData )    (TIMER2x->BDR = u32BData)

/**   counter clock select bits */
#define TIMER2n_CR_CLK_SET( n )        (n << TIMER2n_CR_T2nCLK_Pos)

/**   External Clock Edge Selection bit */
#define TIMER2n_CR_ECE_SET( n )        (n << TIMER2n_CR_T2nECE_Pos)

/**   T2nCAP signal selection bit */
#define TIMER2n_CR_CAPSEL_SET( n )     (n << 9)

/**   T2nOUT Polarity Selection bit */
#define TIMER2n_CR_OPOL_SET( n )       (n << TIMER2n_CR_T2nOPOL_Pos)

/**   TIMER2n Capture Polarity Selection bit */
#define TIMER2n_CR_CPOL_SET( n )       (n << TIMER2n_CR_T2nCPOL_Pos)

/**   TIMER2n Match Interrupt Enable bit */
#define TIMER2n_CR_MIEN_SET( n )       (n << TIMER2n_CR_T2nMIEN_Pos)

/**   TIMER2n Capture Interrupt Enable bit */
#define TIMER2n_CR_CIEN_SET( n )       (n << TIMER2n_CR_T2nCIEN_Pos)

/**   TIMER2n Counter Temporary Pause Control bit */
#define TIMER2n_CR_PAU_SET( n )        (n << TIMER2n_CR_T2nPAU_Pos)

#define TIMER2n_PREDR_SET( n )         (n << 0)

//******************************************************************************
// Function
//******************************************************************************

void TIMER2n_Init( TIMER2n_Type* TIMER2x, TIMER2n_MODE_OPT TimerCounterMode, void* TIMER2n_Config );
void TIMER2n_DeInit( TIMER2n_Type* TIMER2x );

void TIMER2n_ClearStatus( TIMER2n_Type* TIMER2x, uint8_t value );
uint8_t TIMER2n_GetStatus( TIMER2n_Type* TIMER2x );
void TIMER2n_Cmd( TIMER2n_Type* TIMER2x, FunctionalState NewState );
void TIMER2n_ClearCounter( TIMER2n_Type* TIMER2x );
void TIMER2n_UpdateCountValue( TIMER2n_Type* TIMER2x, uint8_t CountCh, uint16_t Value );
uint16_t TIMER2n_GetCountValue( TIMER2n_Type* TIMER2x );
void TIMER2n_IntConfig( TIMER2n_Type* TIMER2x, TIMER2n_INT_Type TIMER2ntCfg, FunctionalState NewState );

#ifdef __cplusplus
}
#endif

#endif   /* _TIMER2n_H_ */

