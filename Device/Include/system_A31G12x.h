/***************************************************************************//**
 * @file     system_A31G12x.h
 * @brief    CMSIS Device System Header File for
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

#ifndef SYSTEM_A31G12x_H
#define SYSTEM_A31G12x_H

#ifdef __cplusplus
extern "C" {
#endif

//==============================================================================
// Define Debug mode or not
//==============================================================================
#define __ON_DEBUG__                 // This line should be defined on debug mode only.
 
//==============================================================================
// Define clocks
//==============================================================================
/* ToDo: add here your necessary defines for device initialization
         following is an example for different system frequencies */

#define __HIRC    (40000000uL)      /**<  Internal RC Oscillator Frequency */
#define __XMOSC   (16000000uL)      /**<  External Main Crystal Oscillator Frequency */
#define __XSOSC   (32768uL)         /**<  External Sub Crystal Oscillator Frequency */
#define __WDTRC   (40000uL)         /**<  Watch-Dog Timer RC Oscillator Frequency */

extern uint32_t mclk;               /**<  Main Clock Frequency (MCLK) */
extern uint32_t SystemCoreClock;    /**<  System Core Clock Frequency (Core Clock & HCLK) */
extern uint32_t SystemPeriClock;    /**<  System Peripheral Clock Frequency (PCLK) */

/*-------------------------------------------------------------------------*//**
 * @brief         Setup the microcontroller system.
 * @return        none
 * @details       Initialize the System and update the SystemCoreClock variable.
 *//*-------------------------------------------------------------------------*/
extern void SystemInit( void );

/*-------------------------------------------------------------------------*//**
 * @brief         Update SystemCoreClock variable.
 * @return        none
 * @details       Updates the SystemCoreClock with current core Clock retrieved from cpu registers.
 *//*-------------------------------------------------------------------------*/
extern void SystemCoreClockUpdate( void );

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_A31G12x_H */

