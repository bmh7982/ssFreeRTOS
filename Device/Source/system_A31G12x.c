/***************************************************************************//**
 * @file     system_A31G12x.c
 * @brief    CMSIS Device System Source File for
 *           A31G12x Device Series
 * @version  V5.00
 * @date     10. January 2018
 *//***************************************************************************/
/*
 * Copyright (c) 2009-2018 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "A31G12x.h"

//==============================================================================
// System Core Clock Variable
//==============================================================================
/* ToDo: initialize SystemCoreClock with the system core clock frequency value
         achieved after system intitialization.
         This means system core clock frequency after call to SystemInit() */
uint32_t    mclk;                /**<  Main Clock Frequency (MCLK) */
uint32_t    SystemCoreClock;     /**<  System Core Clock Frequency (Core Clock & HCLK) */
uint32_t    SystemPeriClock;     /**<  System Peripheral Clock Frequency (PCLK) */
uint32_t    ClkSrcTbl[] = { __HIRC, __XMOSC, __XSOSC, __WDTRC };

/*-------------------------------------------------------------------------*//**
 * @brief         Update SystemCoreClock variable.
 * @return        none
 * @details       Updates the SystemCoreClock with current core Clock retrieved from cpu registers.
 * @note          This function should be executed whenever the clock is changed.
 *//*-------------------------------------------------------------------------*/
void SystemCoreClockUpdate( void )      /* Get Core Clock Frequency */
{
   mclk = ClkSrcTbl[SCUCG->SCCR_b.MCLKSEL];
   if( SCUCG->SCCR_b.MCLKSEL == 0 ) mclk >>= SCUCG->CLKSRCR_b.HIRCSEL;  // MCLK

   if( SCUCG->SCDIVR1_b.HDIV > 4 ) SCUCG->SCDIVR1_b.HDIV = 4;
   SystemCoreClock = mclk >> (4 - SCUCG->SCDIVR1_b.HDIV);               // HCLK
   SystemPeriClock = SystemCoreClock >> SCUCG->SCDIVR2_b.PDIV;          // PCLK
}

/*-------------------------------------------------------------------------*//**
 * @brief         Setup the microcontroller system.
 * @return        none
 * @details       Initialize the System and update the SystemCoreClock variable.
 *//*-------------------------------------------------------------------------*/
void SystemInit( void )
{
   /* ToDo: add code to initialize the system
            do not use global variables because this function is called before
            reaching pre-main. RW section maybe overwritten afterwards. */

   while( SCUCC->SRSTVR_b.VALID != 0x55 ) {}

   // disable WDT
   WDT->CR  =  0
               | ( 0x5A69uL             << WDT_CR_WTIDKY_Pos )
               | ( WDT_CR_RSTEN_Disable << WDT_CR_RSTEN_Pos )     // added by lms
               | ( WDT_CR_CNTEN_Disable << WDT_CR_CNTEN_Pos )
               ;

#ifndef __ON_DEBUG__
   WDT->SR_b.DBGCNTEN = 0 << WDT_SR_DBGCNTEN_Pos;
#endif
}

