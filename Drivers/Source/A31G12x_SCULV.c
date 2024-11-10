/***************************************************************************//**
 * @file          A31G12x_SCULV.c
 * @brief         Driver Source File for SCU Low Voltage Reset & Indicator
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
 * @brief         Set LVI Block Enable/Disable and Voltage Level
 * @param[in]     u32LviEnBit
 *                   LVI Enable/Disable Control
 *                   -  LVIEN_Disable, LVIEN_Enable
 * @param[in]     u32LviIntEnBit
 *                   LVI Interrupt Enable/Disable Control
 *                   -  LVINTEN_Disable, LVINTEN_Enable
 * @param[in]     u32LviLevel
 *                   LVI Voltage Level
 *                   -  LVIVS_2p00V    // 2.00V
 *                   -  LVIVS_2p13V    // 2.13V
 *                   -  LVIVS_2p28V    // 2.28V
 *                   -  LVIVS_2p46V    // 2.46V
 *                   -  LVIVS_2p67V    // 2.67V
 *                   -  LVIVS_3p04V    // 3.04V
 *                   -  LVIVS_3p20V    // 3.20V
 *                   -  LVIVS_3p55V    // 3.55V
 *                   -  LVIVS_3p75V    // 3.75V
 *                   -  LVIVS_3p99V    // 3.99V
 *                   -  LVIVS_4p25V    // 4.25V
 *                   -  LVIVS_4p55V    // 4.55V
 * @return        none
 * @details       This function sets LVI voltage level.
 *//*-------------------------------------------------------------------------*/
void SCULV_SetLviBlk( uint32_t u32LviEnBit, uint32_t u32LviIntEnBit, uint32_t u32LviLevel )
{
   SCULV->LVICR = ( u32LviEnBit | u32LviIntEnBit | u32LviLevel );
}

