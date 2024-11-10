/***************************************************************************//**
 * @file          A31G12x_CRC.c
 * @brief         Driver Source File for CRC & Checksum
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
 * @brief         Set CRC/Checksum Address
 * @param[in]     u32SAdr
 *                   CRC/Checksum Start Address
 *                   -  0x10000000 ~ 0x10007FFF
 * @param[in]     u32EAdr
 *                   CRC/Checksum End Address
 *                   -  0x10000000 ~ 0x10007FFF
 * @param[in]     u32IniD
 *                   CRC/Checksum Initial Data
 * @return        none
 * @details       This function sets the start/end address for range and initial data for calculation
 *//*-------------------------------------------------------------------------*/
void CRC_SetAddr( uint32_t u32SAdr, uint32_t u32EAdr, uint32_t u32IniD )
{
   SCUCG->PPCLKEN2_b.CRCLKE = 1;    // CRC/Checksum Clock Enable

   CRC->SADR = u32SAdr;             // Set start address
   CRC->EADR = u32EAdr;             // Set end address
   CRC->INIT = u32IniD;             // Set initial data for CRC/Checksum
}

/*-------------------------------------------------------------------------*//**
 * @brief         CRC/Checksum Auto Mode Start and Result
 * @param[in]     u32SEL
 *                   CRC/Checksum Selection
 *                   -  CRC_CRC
 *                   -  CRC_CHECKSUM
 * @param[in]     u32POLY
 *                   CRC Polynomial Selection
 *                   -  CRC_CCITT
 *                   -  CRC_16
 * @param[in]     u32FirstBit
 *                   CRC 1st Shifted-in bit
 *                   -  CRC_MSB
 *                   -  CRC_LSB
 * @return        CRC/Checksum Result
 * @details       This function starts CRC/Checksum calculation and gets result
 *//*-------------------------------------------------------------------------*/
uint32_t CRC_AutoGet( uint32_t u32SEL, uint32_t u32POLY, uint32_t u32FirstBit )
{
   uint32_t    imgPRIMASK;

   if( SystemCoreClock > 20000000uL )
   {
      SCUCG->SCDIVR1_b.HDIV = 3;          // HCLK should be less than or equal to 20MHz during CRC/Checksum auto mode
   }

   CRC->CR = 0
             | ( 0x1uL << CRC_CR_RLTCLR_Pos )   // CRC/Checksum Result Data Register (CRC_RLT) Initialization
             | CRC_AUTO_M                       // User/Auto Mode Selection
             | u32SEL                           // CRC/Checksum Selection
             | u32POLY                          // Polynomial Selection (CRC only)
             | CRC_NOINC                        // CRC/Checksum Start Address Auto Increment Control (User mode only)
             | u32FirstBit;                     // First Shifted-in Selection (CRC only)

   imgPRIMASK = __get_PRIMASK();          // backup PRIMASK (current global interrupt configuration)
   DI();                                  // disable global interrupt
   CRCRun();
   while( ChkCRCFinish() ) {}             // Check if CRC/Checksum finishes or not
   __set_PRIMASK( imgPRIMASK );           // restore PRIMASK

   SCUCG->PPCLKEN2_b.CRCLKE = 0;          // CRC/Checksum Clock Disable

   if( SystemCoreClock > 20000000uL )
   {
      SCUCG->SCDIVR1_b.HDIV = 4;          // HCLK should be set with original frequency
   }

   return ( CRC->RLT & 0xffff );
}

/*-------------------------------------------------------------------------*//**
 * @brief         CRC/Checksum User Mode Start
 * @param[in]     u32SEL
 *                   CRC/Checksum Selection
 *                   -  CRC_CRC
 *                   -  CRC_CHECKSUM
 * @param[in]     u32POLY
 *                   CRC Polynomial Selection
 *                   -  CRC_CCITT
 *                   -  CRC_16
 * @param[in]     u32AdrInc
 *                   Auto Increment of Start Address
 *                   -  CRC_NOINC
 *                   -  CRC_AUTOINC
 * @param[in]     u32FirstBit
 *                   CRC 1st Shifted-in bit
 *                   -  CRC_MSB, CRC_LSB
 * @return        none
 * @details       This function sets the control register for CRC/Checksum User mode
 *//*-------------------------------------------------------------------------*/
void CRC_UserSet( uint32_t u32SEL, uint32_t u32POLY, uint32_t u32AdrInc, uint32_t u32FirstBit )
{
   CRC->CR = 0
             | ( 0x1uL << CRC_CR_RLTCLR_Pos )   // CRC/Checksum Result Data Register (CRC_RLT) Initialization
             | CRC_USER_M                       // User/Auto Mode Selection
             | u32SEL                           // CRC/Checksum Selection
             | u32POLY                          // Polynomial Selection (CRC only)
             | u32AdrInc                        // CRC/Checksum Start Address Auto Increment Control (User mode only)
             | u32FirstBit;                     // First Shifted-in Selection (CRC only)
   CRCRun();
}

/*-------------------------------------------------------------------------*//**
 * @brief         CRC/Checksum Input on User Mode
 * @param[in]     u32Input
 *                   CRC/Checksum Input
 * @return        CRC/Checksum Result with finish Indicator
 *                   -  0x8a29xxxx if on run
 *                   -  0x0000xxxx if Finish
 * @details       This function calculates up to finish.
 *                The CRC_SADR register should be incremented by 4 every input on CRC_NOINC
 *//*-------------------------------------------------------------------------*/
uint32_t CRC_UserInput( uint32_t u32Input )
{
   uint32_t u32Result = 0x8a290000uL;
   uint32_t CRC_EADR;

   CRC_InData( u32Input );
   CRC_EADR = CRC->EADR;
   if( !ChkCRCFinish() || ( CRC->SADR > CRC_EADR ) )                // "Auto"/"User" Increment of Start Address
   {
      CRCStop();                                                            // Stop forcingly on User Increment of Start Address
      SCUCG->PPCLKEN2_b.CRCLKE = 0;                                         // CRC/Checksum Clock Disable
      u32Result = ( CRC->RLT & 0xffff );
   }
   return u32Result;
}

