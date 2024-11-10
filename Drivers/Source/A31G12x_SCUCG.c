/***************************************************************************//**
 * @file          A31G12x_SCUCG.c
 * @brief         Driver Source File for SCU Clock Generation
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
 * @brief         Configure Clock Monitoring
 * @param[in]     u32Acts
 *                   Clock Monitoring Action Selection
 *                   -  MACTS_FlagChk, MACTS_RstGen, MACTS_SysClkChg
 * @param[in]     u32Target
 *                   Clock Monitoring Target Selection
 *                   -  MONCS_MCLK, MONCS_HIRC, MONCS_XMOSC, MONCS_XSOSC
 * @return        none
 * @details       This function checks whether the target clock oscillates.
 *//*-------------------------------------------------------------------------*/
void SCUCG_SetMonClk( uint32_t u32Acts, uint32_t u32Target )
{
   if( SCUCG->CLKSRCR_b.WDTRCEN == 0 )
   {
      SCUCG->CLKSRCR = SCUCG->CLKSRCR
                       | ( ( uint32_t )SCUCG_CLKSRCR_WTIDKY_Value << SCUCG_CLKSRCR_WTIDKY_Pos )
                       | CLKSRCR_WDTRCEN;                   // The WDTRC should be enabled to use clock monitoring
   }
   SCUCG->CMONCR = ( 0x3uL << 2 ) | u32Acts | u32Target;    // Clear MONFLAG and NMINTFG, Set Monitoring Target and Monitoring Action
   SCUCG->CMONCR_b.MONEN = 1;                               // Clock Monitoring Enable
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set Clock Source, HIRC Frequency, and type of XMOSC
 * @param[in]     u32FreIRC
 *                   HIRC Frequency Selection
 *                   -  HIRCSEL_HIRC1, HIRCSEL_HIRC2, HIRCSEL_HIRC4, HIRCSEL_HIRC8
 * @param[in]     u32TypeXM
 *                   Main Oscillator Type and Frequency Range Selection
 *                   -  XMFRNG_Xtal, XMFRNG_Clock
 * @param[in]     u32ClkSrc
 *                   Clock Source
 *                   -  CLKSRCR_WDTRCEN | CLKSRCR_HIRCEN | CLKSRCR_XMOSCEN | CLKSRCR_XSOSCEN
 * @return        none
 * @details       This function sets clock source, HIRC frequency, and x-tal type of XMOSC.
 *                If target clock source is one of XMOSC and XSOSC,
 *                the x-tal pins should be set as alternative before this function call.
 *                To set alternative for x-tal, Use SCUCG_SetMainXtalPin() and SCUCG_SetSubXtalPin() functions.
 *//*-------------------------------------------------------------------------*/
void SCUCG_SetClkSrc( uint32_t u32FreIRC, uint32_t u32TypeXM, uint32_t u32ClkSrc )
{
   uint32_t    tmp;

   tmp = SCUCG->CLKSRCR & 0x0000000F;
   tmp |= ( ( ( uint32_t )SCUCG_CLKSRCR_WTIDKY_Value << SCUCG_CLKSRCR_WTIDKY_Pos )  // Write ID
            | u32FreIRC                                                             // HIRC Frequency
            | u32TypeXM                                                             // XMOSC type: x-tal or external clock
            | u32ClkSrc );
   SCUCG->CLKSRCR = tmp;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Enable Clock Source
 * @param[in]     u32ClkSrc
 *                   Clock Source
 *                   -  CLKSRCR_WDTRCEN | CLKSRCR_HIRCEN | CLKSRCR_XMOSCEN | CLKSRCR_XSOSCEN
 * @param[in]     u32HircDiv
 *                   HIRC Frequency Selection
 *                   -  HIRCSEL_HIRC1, HIRCSEL_HIRC2, HIRCSEL_HIRC4, HIRCSEL_HIRC8
 * @return        none
 * @details       This function is used to enable original source after system clock change.
 * @code
 *                [Example]
 *
 *                // eable HIRC, XMOSC, SXOSC, WDTRC and select HIRCSEL_HIRC1(40MHz HIRC)
 *                SCUCG_EnClkSrc( CLKSRCR_HIRCEN | CLKSRCR_XMOSCEN | CLKSRCR_XSOSCEN | CLKSRCR_WDTRCEN, HIRCSEL_HIRC1 );
 * @endcode
 *//*-------------------------------------------------------------------------*/
void SCUCG_EnClkSrc( uint32_t u32ClkSrc, uint32_t u32HircDiv )
{
#if 0    // before bug fix
   uint32_t    tmp;

   tmp = SCUCG->CLKSRCR & 0x0000FFFF;     // 0x00000fff°¡ ¿ÇÀ» µí...
   tmp |= ( ( uint32_t )SCUCG_CLKSRCR_WTIDKY_Value << SCUCG_CLKSRCR_WTIDKY_Pos );   // Write ID
   tmp |= u32ClkSrc;
   tmp |= u32HircDiv;
   SCUCG->CLKSRCR = tmp;
#else    // after bug fix
   SCUCG->CLKSRCR =  SCUCG->CLKSRCR
                     & ~( SCUCG_CLKSRCR_WTIDKY_Msk | SCUCG_CLKSRCR_HIRCSEL_Msk )
                     | ( ( uint32_t )SCUCG_CLKSRCR_WTIDKY_Value << SCUCG_CLKSRCR_WTIDKY_Pos )   // Write ID
                     | u32HircDiv
                     | u32ClkSrc
                     ;
#endif
}

/*-------------------------------------------------------------------------*//**
 * @brief         Disable Clock Source
 * @param[in]     u32ClkSrc
 *                   Clock Source
 *                   -  CLKSRCR_WDTRCEN | CLKSRCR_HIRCEN | CLKSRCR_XMOSCEN | CLKSRCR_XSOSCEN
 * @return        none
 * @details       This function is used to disable original source after system clock change.
 * @code
 *                [Example]
 *
 *                // disable XMOSC, SXOSC
 *                SCUCG_DisClkSrc( CLKSRCR_XMOSCEN | CLKSRCR_XSOSCEN );
 * @endcode
 *//*-------------------------------------------------------------------------*/
void SCUCG_DisClkSrc( uint32_t u32ClkSrc )
{
   uint32_t    tmp;

   tmp = SCUCG->CLKSRCR & 0x0000FFFF;
   tmp |= ( ( uint32_t )SCUCG_CLKSRCR_WTIDKY_Value << SCUCG_CLKSRCR_WTIDKY_Pos );   // Write ID
   tmp &= ~u32ClkSrc;
   SCUCG->CLKSRCR = tmp;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set XMOSC Pins for x-tal
 * @param[in]     u32XtalFilter
 *                   the filter of x-tal frequency
 *                   -  XRNS_LE4p5MHz, XRNS_LE6p5MHz, XRNS_LE8p5MHz
 *                   -  XRNS_LE10p5MHz, XRNS_LE12p5MHz, XRNS_LE16p5MHz
 * @return        none
 * @details       This function sets PF[1:0]'s alternative for x-tal of XMOSC.
 *//*-------------------------------------------------------------------------*/
void SCUCG_SetMainXtalPin( uint32_t u32XtalFilter )
{
   PF->AFSR1 &= 0xFFFFFF00;      // PF[1:0]: XIN/XOUT
   PF->PUPD  &= 0xFFFFF0uL;      // PF[1:0]: Pull-up/down resistors Disable
   PF->MOD   &= 0xFFFFF0uL;
   PF->MOD   |= 0x000AuL;        // PF[1:0]: Alternative Function
   SCUCG->XTFLSR = ( ( uint32_t )SCUCG_XTFLSR_WTIDKY_Value << SCUCG_XTFLSR_WTIDKY_Pos )   // Write ID
                   | u32XtalFilter;                                                       // x-tal filter value
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set XSOSC Pins for x-tal
 * @return        none
 * @details       This function sets PF[3:2]'s alternative for x-tal of XSOSC.
 *//*-------------------------------------------------------------------------*/
void SCUCG_SetSubXtalPin( void )
{
   PF->AFSR1 &= 0xFFFF00FF;      // PF[3:2]: SXIN/SXOUT
   PF->PUPD  &= 0xFFFF0FuL;      // PF[3:2]: Pull-up/down resistors Disable
   PF->MOD   &= 0xFFFF0FuL;
   PF->MOD   |= 0x00A0uL;        // PF[3:2]: Alternative Function
}

/*-------------------------------------------------------------------------*//**
 * @brief         Change System Clock
 * @param[in]     u32Target
 *                   Target Clock
 *                   -  MCLKSEL_HIRC, MCLKSEL_XMOSC, MCLKSEL_XSOSC, MCLKSEL_WDTRC
 * @return        none
 * @details       This function changes system clock to target source.
 *//*-------------------------------------------------------------------------*/
void SCUCG_ChangeSysClk( uint32_t u32Target )
{
   SCUCG->SCCR = ( SCUCG_SCCR_WTIDKY_Value << SCUCG_SCCR_WTIDKY_Pos )   // Write ID
                 | u32Target;                                           // Target Clock Source
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set System Clock Dividers, SCDIVR1 for WT and LCD Driver in case of using MCLK, SCDIVR2 for SysTick Timer and PCLK
 * @param[in]     u32Div02
 *                   Values for Divider 0 and 2
 *                   -  Clock Divide for HCLK (Divider 0)
 *                      -  HDIV_MCLK16, HDIV_MCLK8, HDIV_MCLK4, HDIV_MCLK2, HDIV_MCLK1
 *                   -  Clock Divide for Watch Timer and LCD Driver (Divider 2)
 *                      -  WLDIV_MCLK64, WLDIV_MCLK128, WLDIV_MCLK256, WLDIV_MCLK512, WLDIV_MCLK1024
 * @param[in]     u32Div13
 *                   Values for Divider 1 and 3
 *                   -  Clock Divide for PCLK (Divider 1)
 *                      -  PDIV_HCLK1, PDIV_HCLK2, PDIV_HCLK4, PDIV_HCLK8
 *                   -  Clock Divide for SysTick Timer (Divider 3)
 *                      -  SYSTDIV_HCLK1, SYSTDIV_HCLK2, SYSTDIV_HCLK4, SYSTDIV_HCLK8
 * @return        none
 * @details       This function changes system clock to target source.
 *//*-------------------------------------------------------------------------*/
void SCUCG_SetClkDiv( uint32_t u32Div02, uint32_t u32Div13 )
{
   // Divider 0 for HCLK:                 000/001/010/011/100: MCLK is divided by 16/8/4/2/1
   // Divider 2 for WT and LCD Driver:    000/001/010/011/100: MCLK is divided by 64/128/256/512/1024
   SCUCG->SCDIVR1 = u32Div02;

   // Divider 1 for PCLK:                 00/01/10/11: HCLK is divided by 1/2/4/8
   // Divider 3 for SysTick Timer:        00/01/10/11: HCLK is divided by 1/2/4/8
   SCUCG->SCDIVR2 = u32Div13;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set CLKO Pin for Clock Output
 * @return        none
 * @details       This function sets selected pin's alternative for CLKO.
 *//*-------------------------------------------------------------------------*/
void SCUCG_SetClkoPin( void )
{
   PF->AFSR1 &= 0xFFF0FFFF;   // PF4   CLKO     0: CLKO        1: ----        2: ----        3: ----        4: ----
   PF->PUPD  &= 0xFFFCFFuL;   // PF4   CLKO     0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
   PF->MOD   &= 0xFFFCFFuL;   // PF4   CLKO     0: Input Mode     1: Output Mode    2: Alternative Function Mode
   PF->MOD   |= 0x0200uL;     // PF4   CLKO     0: Input Mode     1: Output Mode    2: Alternative Function Mode
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set Configuration for Clock Output
 * @param[in]     u32ClkSrc
 *                   Clock to output
 *                   -  CLKOS_MCLK, CLKOS_WDTRC, CLKOS_HIRC, CLKOS_HCLK, CLKOS_PCLK
 * @param[in]     u32Level
 *                   Clock Output Polarity when Disable
 *                   -  POLSEL_Low, POLSEL_High
 * @param[in]     u32Div
 *                   Output Clock Divide
 *                   -  CLKODIV_SelectedClock1, CLKODIV_SelectedClock2
 *                   -  CLKODIV_SelectedClock4, CLKODIV_SelectedClock8
 *                   -  CLKODIV_SelectedClock16, CLKODIV_SelectedClock32
 *                   -  CLKODIV_SelectedClock64, CLKODIV_SelectedClock128
 * @return        none
 * @details       This function sets clock output related configuration.
 *//*-------------------------------------------------------------------------*/
void SCUCG_SetClkoCtrl( uint32_t u32ClkSrc, uint32_t u32Level, uint32_t u32Div )
{
   SCUCG->CLKOCR = 0
                   | ( 1 << SCUCG_CLKOCR_CLKOEN_Pos )    // CLKO Enable
                   | u32Level    // 0: Low Level   1: High Level
                   | u32Div      // 0: div_1       1: div_2       2: div_4    3: div_8    4: div_16   5: div_32   6: div_64   7: div_128
                   | u32ClkSrc   // 0: MCLK        1: WDTRC       2: HIRC     3: HCLK     4: PCLK
                   ;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set Peripheral Clock, The peripheral doesn't work if the corresponding bit is "0b".
 * @param[in]     u32PeriClk1
 *                   Values for TIMER20 ~ TIMER21, TIMER30, TIMER10 ~ TIMER16, PA ~ PF
 * @param[in]     u32PeriClk2
 *                   Values for the Others Peripheral
 * @return        none
 * @details       This function sets the peripheral clock.
 *                A peripheral works properly during the corresponding bit is set to "1b".
 *//*-------------------------------------------------------------------------*/
void SCUCG_SetPeriClk( uint32_t u32PeriClk1, uint32_t u32PeriClk2 )
{
   SCUCG->PPCLKEN1 = u32PeriClk1;   // Set peripheral clock of timers and ports
   SCUCG->PPCLKEN2 = u32PeriClk2;   // Set peripheral clock of the others
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set Each Peripheral Clock
 * @param[in]     u32PeriClk1
 *                   PeriClk1
 *                   -  PPCLKEN1_T20CLKE ~ PPCLKEN1_T21CLKE, PPCLKEN1_T30CLKE
 *                   -  PPCLKEN1_T10CLKE ~ PPCLKEN1_T16CLKE, PPCLKEN1_PACLKE ~ PPCLKEN1_PFCLKE
 * @param[in]     Ind
 *                   Enable/Disable Peripheral Clock.
 *                   -  PPxCLKE_Disable, PPxCLKE_Enable
 * @return        none
 * @details       This function sets each peripheral clock of timers and Ports.
 *//*-------------------------------------------------------------------------*/
void SCUCG_SetEachPeriClk1( uint32_t u32PeriClk1, uint32_t Ind )
{
   if( Ind )
   {
      SCUCG->PPCLKEN1 |= u32PeriClk1;     // Enable a peripheral clock of timers and ports
   }
   else
   {
      SCUCG->PPCLKEN1 &= ~u32PeriClk1;    // Disable a peripheral clock of timers and ports
   }
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set Each Peripheral Clock
 * @param[in]     u32PeriClk2
 *                   PeriClk2
 *                   -  PPCLKEN2_FMCLKE, PPCLKEN2_LVICLKE, PPCLKEN2_WDTCLKE, PPCLKEN2_WTCLKE
 *                   -  PPCLKEN2_LCDCLKE, PPCLKEN2_CRCLKE, PPCLKEN2_ADCLKE
 *                   -  PPCLKEN2_I2C0CLKE ~ PPCLKEN2_I2C2CLKE
 *                   -  PPCLKEN2_UT0CLKE ~ PPCLKEN2_UT1CLKE, PPCLKEN2_UST10CLKE ~ PPCLKEN2_UST13CLKE
 * @param[in]     u32Ind
 *                   Enable/Disable Peripheral Clock.
 *                   -  PPxCLKE_Disable, PPxCLKE_Enable
 * @return        none
 * @details       This function sets each peripheral clock of the others.
 *//*-------------------------------------------------------------------------*/
void SCUCG_SetEachPeriClk2( uint32_t u32PeriClk2, uint32_t u32Ind )
{
   if( u32Ind )
   {
      SCUCG->PPCLKEN2 |= u32PeriClk2;     // Enable a peripheral clock of others
   }
   else
   {
      SCUCG->PPCLKEN2 &= ~u32PeriClk2;    // Disable a peripheral clock of others
   }
}

/*-------------------------------------------------------------------------*//**
 * @brief         Reset Peripheral Block, The peripheral is reset if the corresponding bit is "1b".
 * @param[in]     u32PeriRst1
 *                   Values for TIMER20 ~ TIMER21, TIMER30, TIMER10 ~ TIMER16, PA ~ PF
 * @param[in]     u32PeriRst2
 *                   Values for the Others Peripheral
 * @return        none
 * @details       This function reset peripheral block during the corresponding bit is set to "1b".
                  After reset of a block, the corresponding bit should be cleared to "0b" for operation.
 *//*-------------------------------------------------------------------------*/
void SCUCG_RstPeri( uint32_t u32PeriRst1, uint32_t u32PeriRst2 )
{
   uint32_t    i;

   SCUCG->PPRST1 = u32PeriRst1;  // Reset peripheral block of timers and ports if the corresponding bit is "1b"
   SCUCG->PPRST2 = u32PeriRst2;  // Reset peripheral block of the others
   for( i = 0 ; i < 10 ; i++ )
   {
      NOP();
   }
   SCUCG->PPRST1 = 0x0uL;        // Clear the peripheral reset bits
   SCUCG->PPRST2 = 0x0uL;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set/Reset Each Peripheral Block Reset of PPRST1 Register
 * @param[in]     u32EachPeri1
 *                   Peri1
 *                   -  PPRST1_T20RST ~ PPRST1_T21RST, PPRST1_T30RST
 *                   -  PPRST1_T10RST ~ PPRST1_T16RST, PPRST1_PARST ~ PPRST1_PFRST
 * @details       This function resets each peripheral block.
*----------------------------------------------------------------------------*/
void SCUCG_RstEachPeri1( uint32_t u32EachPeri1 )
{
   uint32_t    i;

   SCUCG->PPRST1 = u32EachPeri1;    // Reset a peripheral block
   for( i = 0 ; i < 10 ; i++ )
   {
      NOP();
   }
   SCUCG->PPRST1 = 0;               // Clear the peripheral reset bit
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set/Reset Each Peripheral Block Reset of PPRST2 Register
 * @param[in]     u32EachPeri2
 *                   Peri2
 *                   -  PPRST2_FMCRST, PPRST2_LVIRST, PPRST2_WTRST, PPRST2_LCDRST
 *                   -  PPRST2_CRRST, PPRST2_ADRST, PPRST2_I2C0RST ~ PPRST2_I2C2RST
 *                   -  PPRST2_UT0RST ~ PPRST2_UT1RST, PPRST2_UST10RST ~ PPRST2_UST13RST
 * @details       This function resets each peripheral block.
*----------------------------------------------------------------------------*/
void SCUCG_RstEachPeri2( uint32_t u32EachPeri2 )
{
   uint32_t    i;

   SCUCG->PPRST2 = u32EachPeri2;    // Reset a peripheral block
   for( i = 0 ; i < 10 ; i++ )
   {
      NOP();
   }
   SCUCG->PPRST2 = 0;               // Clear the peripheral reset bit
}


/*-------------------------------------------------------------------------*//**
 * @brief         Peripheral Clock Selection of PPCLKSR Register
 * @param[in]     u32Peri
 *                   Peripheral Selection
 *                   -  PPCLKSR_T20CLK, PPCLKSR_T30CLK, PPCLKSR_LCDCLK, PPCLKSR_WTCLK, PPCLKSR_WDTCLK
 * @param[in]     u32ClkSrc
 *                   Peripheral Clock Selection
 *                   -  PPCLKSR_T20CLK:   T20CLK_XSOSC, T20CLK_PCLK
 *                   -  PPCLKSR_T30CLK:   T30CLK_MCLK, T30CLK_PCLK
 *                   -  PPCLKSR_LCDCLK:   LCDCLK_DividedMCLK, LCDCLK_XSOSC, LCDCLK_WDTRC
 *                   -  PPCLKSR_WTCLK:    WTCLK_DividedMCLK, WTCLK_XSOSC, WTCLK_WDTRC
 *                   -  PPCLKSR_WDTCLK:   WDTCLK_WDTRC, WDTCLK_PCLK
 * @details       This function resets each peripheral block.
*----------------------------------------------------------------------------*/
void SCUCG_PeriClockSel( uint32_t u32Peri, uint32_t u32ClkSrc )
{
   SCUCG->PPCLKSR &= ~u32Peri;
   SCUCG->PPCLKSR |= u32ClkSrc;
}

