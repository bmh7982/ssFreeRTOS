/***************************************************************************//**
 * @file          A31G12x_SCUCC.c
 * @brief         Driver Source File for SCU Chip Configuration
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
 * @brief         Get Reset Source Status
 * @return        Reset Source
 * @details       This function gets reset source status and clear the register.
 *//*-------------------------------------------------------------------------*/
uint32_t SCUCC_GetRstSrc( void )
{
   uint32_t    tmp;

   tmp = SCUCC->RSTSSR;       // Get reset source status
   SCUCC->RSTSSR = 0x3FuL;    // Clear all reset source status
   return tmp;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set Non-Maskable Interrupt(NMI) Source Selection Register
 * @param[in]     u32NmiCon
 *                   Values for NMISRCR register
 * @return        none
 * @details       This function sets NMISRCR register.
 *//*-------------------------------------------------------------------------*/
void SCUCC_SetNmi( uint32_t u32NmiCon )
{
   SCUCC->NMISRCR = u32NmiCon;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Check whether system reset ok or not. Generate s/w reset if a weak reset
 * @return        none
 * @details       This function checks system reset validation and Generate s/w reset if a weak reset.
 *//*-------------------------------------------------------------------------*/
void SCUCC_ChkValidRst( void )
{
   if( SCUCC->SRSTVR_b.VALID != 0x55 )
   {
      SCUCC_GenSwRst();    // Generate S/W reset on invalid reset
   }
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set Wake-Up Timer Data
 * @param[in]     u32Data
 * @return        none
 * @details       This function sets wake-up timer data to wait for release of deep sleep mode.
 *                Its value should be set to be at least more than 150usec.
 *//*-------------------------------------------------------------------------*/
void SCUCC_SetWutData( uint32_t u32Data )
{
   SCUCC->WUTDR = u32Data;    // On HCLK=40MHz, (150us x 40)/32 = 187.5. So, the data should be more than 187
}

/*-------------------------------------------------------------------------*//**
 * @brief         Change fine trim value of HIRC by one step
 * @param[in]     u32Ind
 *                   Indicator for +/- one step
 *                   -  HIRC_UP_ONESTEP, HIRC_DOWN_ONESTEP
 * @return        none
 * @details       This function changes fine trim value by one step.
 *                If the u32Ind is HIRC_UP_ONESTEP, HIRC frequency is changed up by about 140kHz.
 *                If the u32Ind is HIRC_DOWN_ONESTEP, HIRC frequency is changed down by about 140kHz.
 *//*-------------------------------------------------------------------------*/
void SCUCC_ChgHircFreq( uint32_t u32Ind )
{
   uint32_t    tmp, ntrim;

   tmp = ( SCUCC->HIRCTRM ) & 0x001fuL;   // Read current fine trim value of HIRC
   if( u32Ind == HIRC_UP_ONESTEP )        // Increment by one step(about 140kHz)
   {
      if( tmp != 0x0f )
      {
         tmp++;
      }
   }
   else     // Decrement by one step(about 140kHz)
   {
      if( tmp != 0x10 )
      {
         tmp--;
      }
   }
   tmp &= 0x1f;                                 // Fine trim value is only 5-bits
   tmp |= ( ( SCUCC->HIRCTRM ) & 0x00E0uL );    // Read Coarse trim value
   ntrim = ( tmp << 8 ) ^ 0x0000FF00;           // Make write complement key
   SCUCC->HIRCTRM = ( ( uint32_t )SCUCC_HIRCTRM_WTIDKY_Value << SCUCC_HIRCTRM_WTIDKY_Pos )   // Write new HIRC trim value with write ID and complement key
                    | ntrim
                    | tmp;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Change fine trim value of WDTRC by one step
 * @param[in]     u32Ind
 *                   Indicator for +/- one step
 *                   -  WDTRC_UP_ONESTEP, WDTRC_DOWN_ONESTEP
 * @return        none
 * @details       This function changes fine trim value by one step.
 *                If the u32Ind is WDTRC_UP_ONESTEP, WDTRC frequency is changed up by about 1.1kHz.
 *                If the u32Ind is WDTRC_DOWN_ONESTEP, WDTRC frequency is changed down by about 1.1kHz.
 *//*-------------------------------------------------------------------------*/
void SCUCC_ChgWdtrcFreq( uint32_t u32Ind )
{
   uint32_t    ctmp, ftmp, ntrim;

   ftmp = ( SCUCC->WDTRCTRM ) & 0x0007uL;    // Read current fine trim value of WDTRC
   ctmp = ( SCUCC->WDTRCTRM ) & 0x00F0uL;    // Read current coarse trim value of WDTRC
   if( u32Ind == WDTRC_UP_ONESTEP )          // Increment by one step(about 1.1kHz)
   {
      ftmp++;
      ftmp &= 0x07uL;                        // Fine trim value is only 3-bits
      if( ftmp == 0x04 )
      {
         if( ctmp != 0x70 )
         {
            ctmp += 0x10uL;
         }
         else
         {
            ftmp = 0x03uL;
         }
      }
   }
   else     // Decrement by one step(about 1.1kHz)
   {
      ftmp--;
      ftmp &= 0x07uL;      // Fine trim value is only 3-bits
      if( ftmp == 0x03 )
      {
         if( ctmp != 0x80 )
         {
            ctmp -= 0x10uL;
         }
         else
         {
            ftmp = 0x04uL;
         }
      }
   }
   ctmp &= 0x00f0uL;                      // Coarse trim value is only 4-bits
   ctmp |= ftmp;
   ntrim = ( ctmp << 8 ) ^ 0x0000FF00;    // Make write complement key
   SCUCC->WDTRCTRM   = ( SCUCC_WDTRCTRM_WTIDKY_Value << SCUCC_WDTRCTRM_WTIDKY_Pos )    // Write new WDTRC trim value with write ID and complement key
                       | ntrim
                       | ctmp;
}

