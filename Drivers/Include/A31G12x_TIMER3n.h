/***************************************************************************//**
 * @file          A31G12x_TIMER3n.h
 * @brief         Driver Header File for Timer
 * @version       1.0
 * @date          2019-09-01
 * @author        ABOV Application Team
 *
 * Copyright (c) 2019, ABOV Semiconductor. All rights reserved.
 *//***************************************************************************/

#ifndef _TIMER3n_H_
#define _TIMER3n_H_

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Constant
//******************************************************************************

//==========   TIMER3n_CR   ========================================

//----------   TIMER3n Enable/Disable Definition   ----------
#define TIMER3n_DISABLE       (0x0uL << TIMER3n_CR_T3nEN_Pos)
#define TIMER3n_ENABLE        (0x1uL << TIMER3n_CR_T3nEN_Pos)

//----------   TIMER3n Clock Selection Definition   ----------
#define TIMER3n_CLKINT        (0x0uL << TIMER3n_CR_T3nCLK_Pos)
#define TIMER3n_CLKEXT        (0x1uL << TIMER3n_CR_T3nCLK_Pos)

//----------   TIMER3n Mode Selection Definition   ----------
#define TIMER3n_INVM          (0x0uL << TIMER3n_CR_T3nMS_Pos)
#define TIMER3n_CAPM          (0x1uL << TIMER3n_CR_T3nMS_Pos)
#define TIMER3n_BTOB          (0x2uL << TIMER3n_CR_T3nMS_Pos)

//----------   TIMER3n External Clock Edge Selection Definition   ----------
#define TIMER3n_FEDGE         (0x0uL << TIMER3n_CR_T3nECE_Pos)
#define TIMER3n_REDGE         (0x1uL << TIMER3n_CR_T3nECE_Pos)

//----------   TIMER3n Output Mode Selection Definition   ----------
#define TIMER3n_6CHMOD        (0x0uL << TIMER3n_CR_FORCA_Pos)
#define TIMER3n_FORAMOD       (0x1uL << TIMER3n_CR_FORCA_Pos)

//----------   TIMER3n Delay Time Insert En/Disable Definition   ----------
#define TIMER3n_DLYINSDIS     (0x0uL << TIMER3n_CR_DLYEN_Pos)
#define TIMER3n_DLYINSEN      (0x1uL << TIMER3n_CR_DLYEN_Pos)

//----------   TIMER3n Delay Timer Insertion Position Definition   ----------
#define TIMER3n_INSFRONT      (0x0uL << TIMER3n_CR_DLYPOS_Pos)
#define TIMER3n_INSBACK       (0x1uL << TIMER3n_CR_DLYPOS_Pos)

//----------   TIMER3n Capture Polarity Selection Definition   ----------
#define TIMER3n_CAPFALL       (0x0uL << TIMER3n_CR_T3nCPOL_Pos)
#define TIMER3n_CAPRISE       (0x1uL << TIMER3n_CR_T3nCPOL_Pos)
#define TIMER3n_CAPBOTH       (0x2uL << TIMER3n_CR_T3nCPOL_Pos)

//----------   TIMER3n Data Reload Time Selection Definition   ----------
#define TIMER3n_UPWRITE       (0x0uL << TIMER3n_CR_UPDT_Pos)
#define TIMER3n_UPMATCH       (0x1uL << TIMER3n_CR_UPDT_Pos)
#define TIMER3n_UPBOTTOM      (0x2uL << TIMER3n_CR_UPDT_Pos)

//----------   TIMER3n Period Match Interrupt Occurrence Selection Definition   ----------
#define TIMER3n_E1PERIOD      (0x00uL << TIMER3n_CR_PMOC_Pos)
#define TIMER3n_E2PERIOD      (0x01uL << TIMER3n_CR_PMOC_Pos)
#define TIMER3n_E3PERIOD      (0x02uL << TIMER3n_CR_PMOC_Pos)
#define TIMER3n_E4PERIOD      (0x03uL << TIMER3n_CR_PMOC_Pos)
#define TIMER3n_E5PERIOD      (0x04uL << TIMER3n_CR_PMOC_Pos)
#define TIMER3n_E6PERIOD      (0x05uL << TIMER3n_CR_PMOC_Pos)
#define TIMER3n_E7PERIOD      (0x06uL << TIMER3n_CR_PMOC_Pos)
#define TIMER3n_E8PERIOD      (0x07uL << TIMER3n_CR_PMOC_Pos)

//==========   TIMER3n_OUTCR   ========================================

//----------   TIMER3n PWM30xB Output Polarity Selection Definition   ----------
#define TIMER3n_OUT_BPOLOW       (0x0uL << TIMER3n_OUTCR_POLB_Pos)
#define TIMER3n_OUT_BPOHIGH      (0x1uL << TIMER3n_OUTCR_POLB_Pos)

//----------   TIMER3n PWM30xA Output Polarity Selection Definition   ----------
#define TIMER3n_OUT_APOLOW       (0x0uL << TIMER3n_OUTCR_POLA_Pos)
#define TIMER3n_OUT_APOHIGH      (0x1uL << TIMER3n_OUTCR_POLA_Pos)

//----------   TIMER3n PWM30AB Output En/Disable Definition   ----------
#define TIMER3n_OUT_PWMABDIS     (0x0uL << TIMER3n_OUTCR_PABOE_Pos)
#define TIMER3n_OUT_PWMABEN      (0x1uL << TIMER3n_OUTCR_PABOE_Pos)

//----------   TIMER3n PWM30BB Output En/Disable Definition   ----------
#define TIMER3n_OUT_PWMBBDIS     (0x0uL << TIMER3n_OUTCR_PBBOE_Pos)
#define TIMER3n_OUT_PWMBBEN      (0x1uL << TIMER3n_OUTCR_PBBOE_Pos)

//----------   TIMER3n PWM30CB Output En/Disable Definition   ----------
#define TIMER3n_OUT_PWMCBDIS     (0x0uL << TIMER3n_OUTCR_PCBOE_Pos)
#define TIMER3n_OUT_PWMCBEN      (0x1uL << TIMER3n_OUTCR_PCBOE_Pos)

//----------   TIMER3n PWM30AA Output En/Disable Definition   ----------
#define TIMER3n_OUT_PWMAADIS     (0x0uL << TIMER3n_OUTCR_PAAOE_Pos)
#define TIMER3n_OUT_PWMAAEN      (0x1uL << TIMER3n_OUTCR_PAAOE_Pos)

//----------   TIMER3n PWM30BA Output En/Disable Definition   ----------
#define TIMER3n_OUT_PWMBADIS     (0x0uL << TIMER3n_OUTCR_PBAOE_Pos)
#define TIMER3n_OUT_PWMBAEN      (0x1uL << TIMER3n_OUTCR_PBAOE_Pos)

//----------   TIMER3n PWM30CA Output En/Disable Definition   ----------
#define TIMER3n_OUT_PWMCADIS     (0x0uL << TIMER3n_OUTCR_PCAOE_Pos)
#define TIMER3n_OUT_PWMCAEN      (0x1uL << TIMER3n_OUTCR_PCAOE_Pos)

//----------   TIMER3n PWM30AB Output When Disable   ----------
#define TIMER3n_OUT_ABLOW        (0x0uL << TIMER3n_OUTCR_LVLAB_Pos)
#define TIMER3n_OUT_ABHIGH       (0x1uL << TIMER3n_OUTCR_LVLAB_Pos)

//----------   TIMER3n PWM30BB Output When Disable   ----------
#define TIMER3n_OUT_BBLOW        (0x0uL << TIMER3n_OUTCR_LVLBB_Pos)
#define TIMER3n_OUT_BBHIGH       (0x1uL << TIMER3n_OUTCR_LVLBB_Pos)

//----------   TIMER3n PWM30CB Output When Disable   ----------
#define TIMER3n_OUT_CBLOW        (0x0uL << TIMER3n_OUTCR_LVLCB_Pos)
#define TIMER3n_OUT_CBHIGH       (0x1uL << TIMER3n_OUTCR_LVLCB_Pos)

//----------   TIMER3n PWM30AA Output When Disable   ----------
#define TIMER3n_OUT_AALOW        (0x0uL << TIMER3n_OUTCR_LVLAA_Pos)
#define TIMER3n_OUT_AAHIGH       (0x1uL << TIMER3n_OUTCR_LVLAA_Pos)

//----------   TIMER3n PWM30BA Output When Disable   ----------
#define TIMER3n_OUT_BALOW        (0x0uL << TIMER3n_OUTCR_LVLBA_Pos)
#define TIMER3n_OUT_BAHIGH       (0x1uL << TIMER3n_OUTCR_LVLBA_Pos)

//----------   TIMER3n PWM30CA Output When Disable   ----------
#define TIMER3n_OUT_CALOW        (0x0uL << TIMER3n_OUTCR_LVLCA_Pos)
#define TIMER3n_OUT_CAHIGH       (0x1uL << TIMER3n_OUTCR_LVLCA_Pos)

//==========   TIMER3n_INTCR   ========================================

//----------   TIMER3n High-Impedance Interrupt EN/Disable Definition   ----------
#define TIMER3n_INT_HIZDIS       (0x0uL << TIMER3n_INTCR_HIZIEN_Pos)
#define TIMER3n_INT_HIZEN        (0x1uL << TIMER3n_INTCR_HIZIEN_Pos)

//----------   TIMER3n Capture Interrupt EN/Disable Definition   ----------
#define TIMER3n_INT_CAPDIS       (0x0uL << TIMER3n_INTCR_T3nCIEN_Pos)
#define TIMER3n_INT_CAPEN        (0x1uL << TIMER3n_INTCR_T3nCIEN_Pos)

//----------   TIMER3n Bottom Interrupt EN/Disable Definition   ----------
#define TIMER3n_INT_BOTDIS       (0x0uL << TIMER3n_INTCR_T3nBTIEN_Pos)
#define TIMER3n_INT_BOTEN        (0x1uL << TIMER3n_INTCR_T3nBTIEN_Pos)

//----------   TIMER3n Period Match Interrupt EN/Disable Definition   ----------
#define TIMER3n_INT_PMATDIS      (0x0uL << TIMER3n_INTCR_T3nPMIEN_Pos)
#define TIMER3n_INT_PMATEN       (0x1uL << TIMER3n_INTCR_T3nPMIEN_Pos)

//----------   TIMER3n A Match Interrupt EN/Disable Definition   ----------
#define TIMER3n_INT_AMATDIS      (0x0uL << TIMER3n_INTCR_T3nAMIEN_Pos)
#define TIMER3n_INT_AMATEN       (0x1uL << TIMER3n_INTCR_T3nAMIEN_Pos)

//----------   TIMER3n B Match Interrupt EN/Disable Definition   ----------
#define TIMER3n_INT_BMATDIS      (0x0uL << TIMER3n_INTCR_T3nBMIEN_Pos)
#define TIMER3n_INT_BMATEN       (0x1uL << TIMER3n_INTCR_T3nBMIEN_Pos)

//----------   TIMER3n C Match Interrupt EN/Disable Definition   ----------
#define TIMER3n_INT_CMATDIS      (0x0uL << TIMER3n_INTCR_T3nCMIEN_Pos)
#define TIMER3n_INT_CMATEN       (0x1uL << TIMER3n_INTCR_T3nCMIEN_Pos)

//==========   TIMER3n_HIZCR   ========================================

//----------   TIMER3n PWM Output High-Impedance En/Disable Definition   ----------
#define TIMER3n_HIZ_DISABLE      (0x0uL << TIMER3n_HIZCR_HIZEN_Pos)
#define TIMER3n_HIZ_ENABLE       (0x1uL << TIMER3n_HIZCR_HIZEN_Pos)

//----------   TIMER3n High-Impedance(BLNK) Edge Definition   ----------
#define TIMER3n_HIZ_BLNKFALL     (0x0uL << TIMER3n_HIZCR_HEDGE_Pos)
#define TIMER3n_HIZ_BLNKRISE     (0x1uL << TIMER3n_HIZCR_HEDGE_Pos)

//==========   TIMER3n_ADTCR   ========================================

//----------   TIMER3n Bottom for A/DC Trigger Signal Generator EN/Disable Definition   ----------
#define TIMER3n_ADT_BTTGDIS      (0x0uL << TIMER3n_ADTCR_T3nBTTG_Pos)
#define TIMER3n_ADT_BTTGEN       (0x1uL << TIMER3n_ADTCR_T3nBTTG_Pos)

//----------   TIMER3n Period Match for A/DC Trigger Signal Generator EN/Disable Definition   ----------
#define TIMER3n_ADT_PMTGDIS      (0x0uL << TIMER3n_ADTCR_T3nPMTG_Pos)
#define TIMER3n_ADT_PMTGEN       (0x1uL << TIMER3n_ADTCR_T3nPMTG_Pos)

//----------   TIMER3n A-ch Match for A/DC Trigger Signal Generator EN/Disable Definition   ----------
#define TIMER3n_ADT_AMTGDIS      (0x0uL << TIMER3n_ADTCR_T3nAMTG_Pos)
#define TIMER3n_ADT_AMTGEN       (0x1uL << TIMER3n_ADTCR_T3nAMTG_Pos)

//----------   TIMER3n B-ch Match for A/DC Trigger Signal Generator EN/Disable Definition   ----------
#define TIMER3n_ADT_BMTGDIS      (0x0uL << TIMER3n_ADTCR_T3nBMTG_Pos)
#define TIMER3n_ADT_BMTGEN       (0x1uL << TIMER3n_ADTCR_T3nBMTG_Pos)

//----------   TIMER3n C-ch Match for A/DC Trigger Signal Generator EN/Disable Definition   ----------
#define TIMER3n_ADT_CMTGDIS      (0x0uL << TIMER3n_ADTCR_T3nCMTG_Pos)
#define TIMER3n_ADT_CMTGEN       (0x1uL << TIMER3n_ADTCR_T3nCMTG_Pos)

//******************************************************************************
// Type
//******************************************************************************

//==============================================================================
// Structure
//==============================================================================

typedef struct
{
   // TIMER3n.CR
   TIMER3n_CR_T3nMS_Enum         T3nMS;      // TIMER3n Operation Mode Selection
   TIMER3n_CR_T3nCLK_Enum        T3nCLK;     // TIMER3n Clock Selection
   TIMER3n_CR_T3nECE_Enum        T3nECE;     // TIMER3n External Clock Edge Selection
   TIMER3n_CR_T3nCPOL_Enum       T3nCPOL;    // TIMER3n Capture Polarity Selection

   // TIMER3n.PDR
   uint16_t    PDR;

   // TIMER3n.ADR
   uint16_t    ADR;

   // TIMER3n.BDR
   uint16_t    BDR;

   // TIMER3n.CDR
   uint16_t    CDR;

   // TIMER3n.PREDR
   uint16_t    Prescaler;
} TIMER3n_CFG_Type;

//******************************************************************************
// Macro
//******************************************************************************

// Control
/*-------------------------------------------------------------------------*//**
 * @brief         TIMER3n Enable/Disable
 * @details       This macro Enable TIMER3n Block
 *//*-------------------------------------------------------------------------*/
#define TIMER3n_EN()                      (TIMER30->CR_b.T3nEN = 1)
#define TIMER3n_DIS()                     (TIMER30->CR_b.T3nEN = 0)
#define TIMER3n_EnableTimer( TIMER3x )    (TIMER3x->CR_b.T3nEN = 1)
#define TIMER3n_DisableTimer( TIMER3x )   (TIMER3x->CR_b.T3nEN = 0)

/*-------------------------------------------------------------------------*//**
 * @brief         TIMER3n Counter and Prescaler clear
 * @details       This macro clears Counter and Prescalrer of TIMER3n
 *//*-------------------------------------------------------------------------*/
#define TIMER3n_ClrCnt()                  (TIMER30->CR_b.T3nCLR = 1)
#define TIMER3n_ClearCounter( TIMER3x )   (TIMER3x->CR_b.T3nCLR = 1)

/*-------------------------------------------------------------------------*//**
 * @brief         Get TIMER3n Counter Register
 * @details       This macro gets TIMER3n Counter Register
 *//*-------------------------------------------------------------------------*/
#define TIMER3n_GetCnt()               (TIMER30->CNT)

/*-------------------------------------------------------------------------*//**
 * @brief         Set TIMER3n Period Match Interrupt Occurrence
 * @details       This macro sets Period Match Interrupt Occurrence
 *//*-------------------------------------------------------------------------*/
#define TIMER3n_SetPMOC( u32PMOC )     (TIMER30->CR_b.PMOC = u32PMOC)

/*-------------------------------------------------------------------------*//**
 * @brief         TIMER3n PWM Output High-Impedance Enable/Disable
 * @details       This macro Enable TIMER3n PWM Output High-Impedance
 *//*-------------------------------------------------------------------------*/
#define TIMER3n_HIZEN()                (TIMER30->HIZCR_b.T3nEN = 1)
#define TIMER3n_HIZDIS()               (TIMER30->HIZCR_b.T3nEN = 0)

/*-------------------------------------------------------------------------*//**
 * @brief         Set TIMER3n PWM Output High-Impedance by Software
 * @details       This macro sets TIMER3n PWM Output High-Impedance by Software
 *//*-------------------------------------------------------------------------*/
#define TIMER3n_SetHIZSW()             (TIMER30->HIZCR_b.HIZSW = 1)

/*-------------------------------------------------------------------------*//**
 * @brief         TIMER3n High-Impedance Output Clear
 * @details       This macro clears TIMER3n High-Impedance Output
 *//*-------------------------------------------------------------------------*/
#define TIMER3n_ClrHIZ()               (TIMER30->HIZCR_b.HIZCLR = 1)

/*-------------------------------------------------------------------------*//**
 * @brief         Get TIMER3n High-Impedance Status
 * @details       This macro gets TIMER3n High-Impedance Status
 *//*-------------------------------------------------------------------------*/
#define TIMER3n_GetHIZStaus()          (TIMER30->HIZCR_b.HIZSTA)

// Set & Get Data Register
/*-------------------------------------------------------------------------*//**
 * @brief         Set TIMER3n Period Data Register
 * @param[in]     u32PData
 *                   Period Data of TIMER3n
 * @details       This macro sets TIMER3n Period Data Register
 *//*-------------------------------------------------------------------------*/
#define TIMER3n_SetPeData( u32PData )     (TIMER30->PDR = u32PData)

/*-------------------------------------------------------------------------*//**
 * @brief         Set TIMER3n A Data Register
 * @param[in]     u32AData
 *                   A Data of TIMER3n
 * @details       This macro sets TIMER3n A Data Register
 *//*-------------------------------------------------------------------------*/
#define TIMER3n_SetAData( u32AData )      (TIMER30->ADR = u32AData)

/*-------------------------------------------------------------------------*//**
 * @brief         Set TIMER3n B Data Register
 * @param[in]     u32BData
 *                   B Data of TIMER3n
 * @details       This macro sets TIMER3n B Data Register
 *//*-------------------------------------------------------------------------*/
#define TIMER3n_SetBData( u32BData )      (TIMER30->BDR = u32BData)

/*-------------------------------------------------------------------------*//**
 * @brief         Set TIMER3n C Data Register
 * @param[in]     u32CData
 *                   C Data of TIMER3n
 * @details       This macro sets TIMER3n C Data Register
 *//*-------------------------------------------------------------------------*/
#define TIMER3n_SetCData( u32CData )      (TIMER30->CDR = u32CData)

/*-------------------------------------------------------------------------*//**
 * @brief         Get TIMER3n Capture Data Register
 * @details       This macro gets TIMER3n Capture Data Register
 *//*-------------------------------------------------------------------------*/
#define TIMER3n_GetCapData()                 (TIMER30->CAPDR)
#define TIMER3n_GetCaptureData( TIMER3x )    (TIMER3x->CAPDR)

/*-------------------------------------------------------------------------*//**
 * @brief         Set TIMER3n PWM Output Delay Data Register
 * @param[in]     u32DelayData
 *                   PWM Output Delay Data of TIMER3n
 * @details       This macro sets TIMER3n PWM Output Delay Data Register
 *//*-------------------------------------------------------------------------*/
#define TIMER3n_SetDelayData( u32DelayData )    (TIMER30->DLY = u32DelayData)

/*-------------------------------------------------------------------------*//**
 * @brief         Set TIMER3n A/DC Trigger Generator Data Register
 * @param[in]     u32ADTData
 *                   A/DC Trigger Generator Data of TIMER3n
 * @details       This macro sets TIMER3n A/DC Trigger Generator Data Register
 *//*-------------------------------------------------------------------------*/
#define TIMER3n_SetADTData( u32ADTData )        (TIMER30->ADTDR = u32ADTData)

// Get & Clear Interrupt Flag
/*-------------------------------------------------------------------------*//**
 * @brief         Get flags of TIMER3n All Interrupt
 * @details       This macro gets interrupt flag of TIMER3n All Interrupt
 *//*-------------------------------------------------------------------------*/
#define TIMER3n_AllInt_GetFg()                  (TIMER30->INTFLAG)

/*-------------------------------------------------------------------------*//**
 * @brief         Get flags of TIMER3n C-ch Match Interrupt
 * @details       This macro gets interrupt flag of TIMER3n C-ch Match Interrupt
 *//*-------------------------------------------------------------------------*/
#define TIMER3n_CMaInt_GetFg()         (TIMER30->INTFLAG_b.T30CMIFLAG)

/*-------------------------------------------------------------------------*//**
 * @brief         Get flags of TIMER3n B-ch Match Interrupt
 * @details       This macro gets interrupt flag of TIMER3n B-ch Match Interrupt
 *//*-------------------------------------------------------------------------*/
#define TIMER3n_BMaInt_GetFg()         (TIMER30->INTFLAG_b.T30BMIFLAG)

/*-------------------------------------------------------------------------*//**
 * @brief         Get flags of TIMER3n A-ch Match Interrupt
 * @details       This macro gets interrupt flag of TIMER3n A-ch Match Interrupt
 *//*-------------------------------------------------------------------------*/
#define TIMER3n_AchMaInt_GetFg()       (TIMER30->INTFLAG_b.T30AMIFLAG)

/*-------------------------------------------------------------------------*//**
 * @brief         Get flags of TIMER3n Period Match Interrupt
 * @details       This macro gets interrupt flag of TIMER3n Period Match Interrupt
 *//*-------------------------------------------------------------------------*/
#define TIMER3n_PeMaInt_GetFg()        (TIMER30->INTFLAG_b.T30PMIFLAG)

/*-------------------------------------------------------------------------*//**
 * @brief         Get flags of TIMER3n Bottom Interrupt
 * @details       This macro gets interrupt flag of TIMER3n Bottom Interrupt
 *//*-------------------------------------------------------------------------*/
#define TIMER3n_BotMaInt_GetFg()       (TIMER30->INTFLAG_b.T30BTIFLAG)

/*-------------------------------------------------------------------------*//**
 * @brief         Get flags of TIMER3n Capture Interrupt
 * @details       This macro gets interrupt flag of TIMER3n Capture Interrupt
 *//*-------------------------------------------------------------------------*/
#define TIMER3n_CapInt_GetFg()         (TIMER30->INTFLAG_b.T30CIFLAG)

/*-------------------------------------------------------------------------*//**
 * @brief         Get flags of TIMER3n High-Impedance Interrupt
 * @details       This macro gets interrupt flag of TIMER3n High-Impedance Interrupt
 *//*-------------------------------------------------------------------------*/
#define TIMER3n_HIZInt_GetFg()         (TIMER30->INTFLAG_b.HIZIFLAG)

/*-------------------------------------------------------------------------*//**
 * @brief         Clear flags of TIMER3n All Interrupt
 * @details       This macro Clears interrupt flag of TIMER3n All Interrupt
 *//*-------------------------------------------------------------------------*/
#define TIMER3n_AllInt_ClrFg()         (TIMER30->INTFLAG = 0x7F)

/*-------------------------------------------------------------------------*//**
 * @brief         Clear flags of TIMER3n C-ch Match Interrupt
 * @details       This macro clears interrupt flag of TIMER3n C-ch Match Interrupt
 *//*-------------------------------------------------------------------------*/
#define TIMER3n_CchMaInt_ClrFg()       (TIMER30->INTFLAG_b.T30CMIFLAG = 1)

/*-------------------------------------------------------------------------*//**
 * @brief         Clear flags of TIMER3n B-ch Match Interrupt
 * @details       This macro clears interrupt flag of TIMER3n B-ch Match Interrupt
 *//*-------------------------------------------------------------------------*/
#define TIMER3n_BchMaInt_ClrFg()       (TIMER30->INTFLAG_b.T30BMIFLAG = 1)

/*-------------------------------------------------------------------------*//**
 * @brief         Clear flags of TIMER3n A-ch Match Interrupt
 * @details       This macro clears interrupt flag of TIMER3n A-ch Match Interrupt
 *//*-------------------------------------------------------------------------*/
#define TIMER3n_AchMaInt_ClrFg()       (TIMER30->INTFLAG_b.T30AMIFLAG = 1)

/*-------------------------------------------------------------------------*//**
 * @brief         Clear flags of TIMER3n Period Match Interrupt
 * @details       This macro clears interrupt flag of TIMER3n Period Match Interrupt
 *//*-------------------------------------------------------------------------*/
#define TIMER3n_PeMaInt_ClrFg()        (TIMER30->INTFLAG_b.T30PMIFLAG = 1)

/*-------------------------------------------------------------------------*//**
 * @brief         Clear flags of TIMER3n Bottom Interrupt
 * @details       This macro clears interrupt flag of TIMER3n Bottom Interrupt
 *//*-------------------------------------------------------------------------*/
#define TIMER3n_BotMaInt_ClrFg()       (TIMER30->INTFLAG_b.T30BTIFLAG = 1)

/*-------------------------------------------------------------------------*//**
 * @brief         Clear flags of TIMER3n Capture Interrupt
 * @details       This macro clears interrupt flag of TIMER3n Capture Interrupt
 *//*-------------------------------------------------------------------------*/
#define TIMER3n_CapInt_ClrFg()         (TIMER30->INTFLAG_b.T30CIFLAG = 1)

/*-------------------------------------------------------------------------*//**
 * @brief         Clear flags of TIMER3n High-Impedance Interrupt
 * @details       This macro clears interrupt flag of TIMER3n High-Impedance Interrupt
 *//*-------------------------------------------------------------------------*/
#define TIMER3n_HIZInt_ClrFg()         (TIMER30->INTFLAG_b.HIZIFLAG = 1)

//******************************************************************************
// Function
//******************************************************************************

void TIMER3n_Init( TIMER3n_Type* TIMER3x, TIMER3n_CFG_Type* TIMER3n_Config );
void TIMER3n_ClockPrescaler( TIMER3n_Type* TIMER3x, uint32_t prescale );
void TIMER3n_SetPeriod( TIMER3n_Type* TIMER3x, uint32_t period );
void TIMER3n_SetADuty( TIMER3n_Type* TIMER3x, uint32_t aduty );
void TIMER3n_SetBDuty( TIMER3n_Type* TIMER3x, uint32_t bduty );
void TIMER3n_SetCDuty( TIMER3n_Type* TIMER3x, uint32_t cduty );
void TIMER3n_SetDelayTime( TIMER3n_Type* TIMER3x, uint32_t dten, uint32_t dtpos, uint32_t clkdata );
void TIMER3n_Start( TIMER3n_Type* TIMER3x, uint32_t NewState );
void TIMER3n_OutputCtrl( TIMER3n_Type* TIMER3x,  uint32_t NewState, uint32_t pwmApol, uint32_t pwmBpol );
void TIMER3n_IntConfig( TIMER3n_Type* TIMER3x, uint32_t NewState, uint32_t T30IntCfg );
void TIMER3n_ClearStatus( TIMER3n_Type* TIMER3x, uint32_t T30IntCfg );
uint32_t TIMER3n_GetStatus( TIMER3n_Type* TIMER3x );
void TIMER3n_MPWMCmd( TIMER3n_Type* TIMER3x, uint32_t updatedata, uint32_t intcount );
void TIMER3n_SetHizReg( TIMER3n_Type* TIMER3x, uint32_t u32T30HizSet );
void TIMER3n_SetADCTrigger( TIMER3n_Type* TIMER3x, uint32_t u32triggerpoint, uint32_t u32triggertime );

#ifdef __cplusplus
}
#endif

#endif   /* _TIMER3n_H_ */

