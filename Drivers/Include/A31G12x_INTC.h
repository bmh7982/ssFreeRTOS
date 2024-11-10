/***************************************************************************//**
 * @file          A31G12x_INTC.h
 * @brief         Driver Header File for Interrupt Controller
 * @version       1.0
 * @date          2019-09-01
 * @author        ABOV Application Team
 *
 * Copyright (c) 2019, ABOV Semiconductor. All rights reserved.
 *//***************************************************************************/

#ifndef _INTC_H_
#define _INTC_H_

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Constant
//******************************************************************************

// External Interrupt Port Number
// #define PORTA                 (0x0uL)  /**< Available EINT Pin: PB[11:0], PC[3:0], PE[3:0] */
#define PORTB                    (0x1uL)  /**< Available EINT Pin: PB[11:0], PC[3:0], PE[3:0] */
#define PORTC                    (0x2uL)  /**< Available EINT Pin: PB[11:0], PC[3:0], PE[3:0] */
// #define PORTD                 (0x3uL)  /**< Available EINT Pin: PB[11:0], PC[3:0], PE[3:0] */
#define PORTE                    (0x4uL)  /**< Available EINT Pin: PB[11:0], PC[3:0], PE[3:0] */
// #define PORTF                 (0x5uL)  /**< Available EINT Pin: PB[11:0], PC[3:0], PE[3:0] */

// External Interrupt Number
/*
#define EINT0_SEL                (0x0uL)
#define EINT1_SEL                (0x1uL)
#define EINT2_SEL                (0x2uL)
#define EINT3_SEL                (0x3uL)
*/
#define EINT0                    (0x0uL)
#define EINT1                    (0x1uL)
#define EINT2                    (0x2uL)
#define EINT3                    (0x3uL)

// External Interrupt Trigger Selection Control
/*
#define TRG_EDGE                 (0x0uL)
#define TRG_LEVEL                (0x1uL)
*/
#define ITRIGx_Edge              INTC_PBTRIG_ITRIG11_Edge
#define ITRIGx_Level             INTC_PBTRIG_ITRIG11_Level

// External Interrupt Enable Control
/*
#define EInt_DISABLE             (0x0uL)
#define EInt_LOW_LEVEL_INT       (0x1uL)
#define EInt_HIGH_LEVEL_INT      (0x2uL)
#define EInt_FALLING_EDGE_INT    (0x1uL)
#define EInt_RISING_EDGE_INT     (0x2uL)
#define EInt_BOTH_EDGE_INT       (0x3uL)
*/
#define INTCTLx_Disable          INTC_PBCR_INTCTL11_Disable
#define INTCTLx_LowLevel         INTC_PBCR_INTCTL11_FallingEdgeLowLevel
#define INTCTLx_HighLevel        INTC_PBCR_INTCTL11_RisingEdgeHighLevel
#define INTCTLx_FallingEdge      INTC_PBCR_INTCTL11_FallingEdgeLowLevel
#define INTCTLx_RisingEdge       INTC_PBCR_INTCTL11_RisingEdgeHighLevel
#define INTCTLx_BothEdge         INTC_PBCR_INTCTL11_BothEdgeNoLevel

// External Interrupt Flag Clear Control
/*
#define EFLAG0                   (0x1uL << 0)
#define EFLAG1                   (0x1uL << 1)
#define EFLAG2                   (0x1uL << 2)
#define EFLAG3                   (0x1uL << 3)
#define EFLAG4                   (0x1uL << 4)
#define EFLAG5                   (0x1uL << 5)
#define EFLAG6                   (0x1uL << 6)
#define EFLAG7                   (0x1uL << 7)
#define EFLAG8                   (0x1uL << 8)
#define EFLAG9                   (0x1uL << 9)
#define EFLAG10                  (0x1uL << 10)
#define EFLAG11                  (0x1uL << 11)
*/
// #define PnFLAG_FLAG15         (0x1uL << INTC_PBFLAG_FLAG15_Pos)
// #define PnFLAG_FLAG14         (0x1uL << INTC_PBFLAG_FLAG14_Pos)
// #define PnFLAG_FLAG13         (0x1uL << INTC_PBFLAG_FLAG13_Pos)
// #define PnFLAG_FLAG12         (0x1uL << INTC_PBFLAG_FLAG12_Pos)
#define PnFLAG_FLAG11            (0x1uL << INTC_PBFLAG_FLAG11_Pos)
#define PnFLAG_FLAG10            (0x1uL << INTC_PBFLAG_FLAG10_Pos)
#define PnFLAG_FLAG9             (0x1uL << INTC_PBFLAG_FLAG9_Pos)
#define PnFLAG_FLAG8             (0x1uL << INTC_PBFLAG_FLAG8_Pos)
#define PnFLAG_FLAG7             (0x1uL << INTC_PBFLAG_FLAG7_Pos)
#define PnFLAG_FLAG6             (0x1uL << INTC_PBFLAG_FLAG6_Pos)
#define PnFLAG_FLAG5             (0x1uL << INTC_PBFLAG_FLAG5_Pos)
#define PnFLAG_FLAG4             (0x1uL << INTC_PBFLAG_FLAG4_Pos)
#define PnFLAG_FLAG3             (0x1uL << INTC_PBFLAG_FLAG3_Pos)
#define PnFLAG_FLAG2             (0x1uL << INTC_PBFLAG_FLAG2_Pos)
#define PnFLAG_FLAG1             (0x1uL << INTC_PBFLAG_FLAG1_Pos)
#define PnFLAG_FLAG0             (0x1uL << INTC_PBFLAG_FLAG0_Pos)

// External Interrupt Configuration Selection Control
#ifdef PORTA
#define CONFx_PA                 INTC_EINT0CONF1_CONF0_PA   /**< Available EINT Pin: PB[11:0], PC[3:0], PE[3:0] */
#endif
#ifdef PORTB
#define CONFx_PB                 INTC_EINT0CONF1_CONF0_PB   /**< Available EINT Pin: PB[11:0], PC[3:0], PE[3:0] */
#endif
#ifdef PORTC
#define CONFx_PC                 INTC_EINT0CONF1_CONF0_PC   /**< Available EINT Pin: PB[11:0], PC[3:0], PE[3:0] */
#endif
#ifdef PORTD
#define CONFx_PD                 INTC_EINT0CONF1_CONF0_PD   /**< Available EINT Pin: PB[11:0], PC[3:0], PE[3:0] */
#endif
#ifdef PORTE
#define CONFx_PE                 INTC_EINT0CONF1_CONF0_PE   /**< Available EINT Pin: PB[11:0], PC[3:0], PE[3:0] */
#endif
#ifdef PORTF
#define CONFx_PF                 INTC_EINT0CONF1_CONF0_PF   /**< Available EINT Pin: PB[11:0], PC[3:0], PE[3:0] */
#endif

// Interrupt Source Mask Control
/*
#define LVI_MASK                 (0x1uL << 0)
#define WUT_MASK                 (0x1uL << 1)
#define WDT_MASK                 (0x1uL << 2)
#define EINT0_MASK               (0x1uL << 3)
#define EINT1_MASK               (0x1uL << 4)
#define EINT2_MASK               (0x1uL << 5)
#define EINT3_MASK               (0x1uL << 6)
#define TIMER10_MASK             (0x1uL << 7)
#define TIMER11_MASK             (0x1uL << 8)
#define TIMER12_MASK             (0x1uL << 9)
#define I2C0_MASK                (0x1uL << 10)
#define USART10_MASK             (0x1uL << 11)
#define WT_MASK                  (0x1uL << 12)
#define TIMER30_MASK             (0x1uL << 13)
#define I2C1_MASK                (0x1uL << 14)
#define TIMER20_MASK             (0x1uL << 15)
#define TIMER21_MASK             (0x1uL << 16)
#define USART11_MASK             (0x1uL << 17)
#define ADC_MASK                 (0x1uL << 18)
#define UART0_MASK               (0x1uL << 19)
#define UART1_MASK               (0x1uL << 20)
#define TIMER13_MASK             (0x1uL << 21)
#define TIMER14_MASK             (0x1uL << 22)
#define TIMER15_MASK             (0x1uL << 23)
#define TIMER16_MASK             (0x1uL << 24)
#define I2C2_MASK                (0x1uL << 25)
#define UST12_MASK               (0x1uL << 26)
#define UST13_MASK               (0x1uL << 27)
*/
#define MSK_IMSK31               (0x1uL << INTC_MSK_IMSK31_NULL_Pos)
#define MSK_IMSK30               (0x1uL << INTC_MSK_IMSK30_NULL_Pos)
#define MSK_IMSK29               (0x1uL << INTC_MSK_IMSK29_NULL_Pos)
#define MSK_IMSK28               (0x1uL << INTC_MSK_IMSK28_NULL_Pos)
#define MSK_USART13              (0x1uL << INTC_MSK_IMSK27_USART13_Pos)
#define MSK_USART12              (0x1uL << INTC_MSK_IMSK26_USART12_Pos)
#define MSK_I2C2                 (0x1uL << INTC_MSK_IMSK25_I2C2_Pos)
#define MSK_TIMER16              (0x1uL << INTC_MSK_IMSK24_TIMER16_Pos)
#define MSK_TIMER15              (0x1uL << INTC_MSK_IMSK23_TIMER15_Pos)
#define MSK_TIMER14              (0x1uL << INTC_MSK_IMSK22_TIMER14_Pos)
#define MSK_TIMER13              (0x1uL << INTC_MSK_IMSK21_TIMER13_Pos)
#define MSK_UART1                (0x1uL << INTC_MSK_IMSK20_UART1_Pos)
#define MSK_UART0                (0x1uL << INTC_MSK_IMSK19_UART0_Pos)
#define MSK_ADC                  (0x1uL << INTC_MSK_IMSK18_ADC_Pos)
#define MSK_USART11              (0x1uL << INTC_MSK_IMSK17_USART11_Pos)
#define MSK_TIMER21              (0x1uL << INTC_MSK_IMSK16_TIMER21_Pos)
#define MSK_TIMER20              (0x1uL << INTC_MSK_IMSK15_TIMER20_Pos)
#define MSK_I2C1                 (0x1uL << INTC_MSK_IMSK14_I2C1_Pos)
#define MSK_TIMER30              (0x1uL << INTC_MSK_IMSK13_TIMER30_Pos)
#define MSK_WT                   (0x1uL << INTC_MSK_IMSK12_WT_Pos)
#define MSK_USART10              (0x1uL << INTC_MSK_IMSK11_USART10_Pos)
#define MSK_I2C0                 (0x1uL << INTC_MSK_IMSK10_I2C0_Pos)
#define MSK_TIMER12              (0x1uL << INTC_MSK_IMSK9_TIMER12_Pos)
#define MSK_TIMER11              (0x1uL << INTC_MSK_IMSK8_TIMER11_Pos)
#define MSK_TIMER10              (0x1uL << INTC_MSK_IMSK7_TIMER10_Pos)
#define MSK_EINT3                (0x1uL << INTC_MSK_IMSK6_EINT3_Pos)
#define MSK_EINT2                (0x1uL << INTC_MSK_IMSK5_EINT2_Pos)
#define MSK_EINT1                (0x1uL << INTC_MSK_IMSK4_EINT1_Pos)
#define MSK_EINT0                (0x1uL << INTC_MSK_IMSK3_EINT0_Pos)
#define MSK_WDT                  (0x1uL << INTC_MSK_IMSK2_WDT_Pos)
#define MSK_WUT                  (0x1uL << INTC_MSK_IMSK1_WUT_Pos)
#define MSK_LVI                  (0x1uL << INTC_MSK_IMSK0_LVI_Pos)

//******************************************************************************
// Macro
//******************************************************************************

#ifdef PORTA
/*-------------------------------------------------------------------------*//**
 * @brief         Get PA Interrupt Flag
 * @return        Pn Interrput Flag
 * @remark        Available EINT Pin: PB[11:0], PC[3:0], PE[3:0]
 *//*-------------------------------------------------------------------------*/
#define EIntPA_GetFg()                 (INTC->PAFLAG)
#endif

#ifdef PORTB
/*-------------------------------------------------------------------------*//**
 * @brief         Get PB Interrupt Flag
 * @return        Pn Interrput Flag
 * @remark        Available EINT Pin: PB[11:0], PC[3:0], PE[3:0]
 *//*-------------------------------------------------------------------------*/
#define EIntPB_GetFg()                 (INTC->PBFLAG)
#endif

#ifdef PORTC
/*-------------------------------------------------------------------------*//**
 * @brief         Get PC Interrupt Flag
 * @return        Pn Interrput Flag
 * @remark        Available EINT Pin: PB[11:0], PC[3:0], PE[3:0]
 *//*-------------------------------------------------------------------------*/
#define EIntPC_GetFg()                 (INTC->PCFLAG)
#endif

#ifdef PORTD
/*-------------------------------------------------------------------------*//**
 * @brief         Get PD Interrupt Flag
 * @return        Pn Interrput Flag
 * @remark        Available EINT Pin: PB[11:0], PC[3:0], PE[3:0]
 *//*-------------------------------------------------------------------------*/
#define EIntPD_GetFg()                 (INTC->PDFLAG)
#endif

#ifdef PORTE
/*-------------------------------------------------------------------------*//**
 * @brief         Get PE Interrupt Flag
 * @return        Pn Interrput Flag
 * @remark        Available EINT Pin: PB[11:0], PC[3:0], PE[3:0]
 *//*-------------------------------------------------------------------------*/
#define EIntPE_GetFg()                 (INTC->PEFLAG)
#endif

#ifdef PORTF
/*-------------------------------------------------------------------------*//**
 * @brief         Get PF Interrupt Flag
 * @return        Pn Interrput Flag
 * @remark        Available EINT Pin: PB[11:0], PC[3:0], PE[3:0]
 *//*-------------------------------------------------------------------------*/
#define EIntPF_GetFg()                 (INTC->PFFLAG)
#endif

#ifdef PORTA
/*-------------------------------------------------------------------------*//**
 * @brief         Clear PA Interrupt Flag
 * @param[in]     u32Bit
 *                   Pn Interrupt Flag Mask
 *                   -  PnFLAG_FLAG0 ~ PnFLAG_FLAG0
 * @return        none
 * @remark        Available EINT Pin: PB[11:0], PC[3:0], PE[3:0]
 *//*-------------------------------------------------------------------------*/
#define EIntPA_ClrFgBits( u32Bit )     (INTC->PAFLAG = u32Bit)
#endif

#ifdef PORTB
/*-------------------------------------------------------------------------*//**
 * @brief         Clear PB Interrupt Flag
 * @param[in]     u32Bit
 *                   Pn Interrupt Flag Mask
 *                   -  PnFLAG_FLAG0 ~ PnFLAG_FLAG11
 * @return        none
 * @remark        Available EINT Pin: PB[11:0], PC[3:0], PE[3:0]
 *//*-------------------------------------------------------------------------*/
#define EIntPB_ClrFgBits( u32Bit )     (INTC->PBFLAG = u32Bit)
#endif

#ifdef PORTC
/*-------------------------------------------------------------------------*//**
 * @brief         Clear PC Interrupt Flag
 * @param[in]     u32Bit
 *                   Pn Interrupt Flag Mask
 *                   -  PnFLAG_FLAG0 ~ PnFLAG_FLAG3
 * @return        none
 * @remark        Available EINT Pin: PB[11:0], PC[3:0], PE[3:0]
 *//*-------------------------------------------------------------------------*/
#define EIntPC_ClrFgBits( u32Bit )     (INTC->PCFLAG = u32Bit)
#endif

#ifdef PORTD
/*-------------------------------------------------------------------------*//**
 * @brief         Clear PD Interrupt Flag
 * @param[in]     u32Bit
 *                   Pn Interrupt Flag Mask
 *                   -  PnFLAG_FLAG0 ~ PnFLAG_FLAG0
 * @return        none
 * @remark        Available EINT Pin: PB[11:0], PC[3:0], PE[3:0]
 *//*-------------------------------------------------------------------------*/
#define EIntPD_ClrFgBits( u32Bit )     (INTC->PDFLAG = u32Bit)
#endif

#ifdef PORTE
/*-------------------------------------------------------------------------*//**
 * @brief         Clear PE Interrupt Flag
 * @param[in]     u32Bit
 *                   Pn Interrupt Flag Mask
 *                   -  PnFLAG_FLAG0 ~ PnFLAG_FLAG3
 * @return        none
 * @remark        Available EINT Pin: PB[11:0], PC[3:0], PE[3:0]
 *//*-------------------------------------------------------------------------*/
#define EIntPE_ClrFgBits( u32Bit )     (INTC->PEFLAG = u32Bit)
#endif

#ifdef PORTF
/*-------------------------------------------------------------------------*//**
 * @brief         Clear PF Interrupt Flag
 * @param[in]     u32Bit
 *                   Pn Interrupt Flag Mask
 *                   -  PnFLAG_FLAG0 ~ PnFLAG_FLAG0
 * @return        none
 * @remark        Available EINT Pin: PB[11:0], PC[3:0], PE[3:0]
 *//*-------------------------------------------------------------------------*/
#define EIntPF_ClrFgBits( u32Bit )     (INTC->PFFLAG = u32Bit)
#endif

/*-------------------------------------------------------------------------*//**
 * @brief         Mask Interrupt Source
 * @param[in]     u32Msk
 *                   Interrupt Source Mask
 *                   -  MSK_LVI, MSK_WUT, MSK_WDT, MSK_EINT0, ...
 * @return        none
 * @details       This macro masks an interrupt vector
 *//*-------------------------------------------------------------------------*/
#define Int_Mask( u32Msk )             (INTC->MSK = (INTC->MSK) & ~u32Msk)

/*-------------------------------------------------------------------------*//**
 * @brief         Unmask Interrupt Source
 * @param[in]     u32UnMsk
 *                   Interrupt Source Mask
 *                   -  MSK_LVI, MSK_WUT, MSK_WDT, MSK_EINT0, ...
 * @return        none
 * @details       This macro unmasks an interrupt vector
 *//*-------------------------------------------------------------------------*/
#define Int_UnMask( u32UnMsk )         (INTC->MSK = (INTC->MSK) | u32UnMsk)

//******************************************************************************
// Function
//******************************************************************************

void EIntPx_SetReg( uint32_t u32Px, uint32_t u32pin, uint32_t u32Trig, uint32_t u32Con );
void EIntCfg( uint32_t u32TarIntNum, uint32_t u32SrcPort, uint32_t u32SrcPin );
#ifdef PORTA
uint32_t EIntPA_GetIntStatus( void );
#endif
#ifdef PORTB
uint32_t EIntPB_GetIntStatus( void );
#endif
#ifdef PORTC
uint32_t EIntPC_GetIntStatus( void );
#endif
#ifdef PORTD
uint32_t EIntPD_GetIntStatus( void );
#endif
#ifdef PORTE
uint32_t EIntPE_GetIntStatus( void );
#endif
#ifdef PORTF
uint32_t EIntPF_GetIntStatus( void );
#endif
#ifdef PORTA
void EIntPA_ClearIntStatus( uint32_t u32Value );
#endif
#ifdef PORTB
void EIntPB_ClearIntStatus( uint32_t u32Value );
#endif
#ifdef PORTC
void EIntPC_ClearIntStatus( uint32_t u32Value );
#endif
#ifdef PORTD
void EIntPD_ClearIntStatus( uint32_t u32Value );
#endif
#ifdef PORTE
void EIntPE_ClearIntStatus( uint32_t u32Value );
#endif
#ifdef PORTF
void EIntPF_ClearIntStatus( uint32_t u32Value );
#endif
void EInt_MaskEnable( uint32_t u32Src );
void EInt_MaskDisable( uint32_t u32Src );

#ifdef __cplusplus
}
#endif

#endif   /* _INTC_H_ */

