/***************************************************************************//**
 * @file          A31G12x_SCUCG.h
 * @brief         Driver Header File for SCU Clock Generation
 * @version       1.0
 * @date          2019-09-01
 * @author        ABOV Application Team
 *
 * Copyright (c) 2019, ABOV Semiconductor. All rights reserved.
 *//***************************************************************************/

#ifndef _SCUCG_H_
#define _SCUCG_H_

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Constant
//******************************************************************************

// MCLK Selection Control
/*
#define SCU_HIRC                 (0x0uL << 0)
#define SCU_XMOSC                (0x1uL << 0)
#define SCU_XSOSC                (0x2uL << 0)
#define SCU_WDTRC                (0x3uL << 0)
*/
#define MCLKSEL_HIRC             (SCUCG_SCCR_MCLKSEL_HIRC  << SCUCG_SCCR_MCLKSEL_Pos)
#define MCLKSEL_XMOSC            (SCUCG_SCCR_MCLKSEL_XMOSC << SCUCG_SCCR_MCLKSEL_Pos)
#define MCLKSEL_XSOSC            (SCUCG_SCCR_MCLKSEL_XSOSC << SCUCG_SCCR_MCLKSEL_Pos)
#define MCLKSEL_WDTRC            (SCUCG_SCCR_MCLKSEL_WDTRC << SCUCG_SCCR_MCLKSEL_Pos)

// HIRC Selection Control
/*
#define HIRC_40M                 (0x0uL << 12)
#define HIRC_20M                 (0x1uL << 12)
#define HIRC_10M                 (0x2uL << 12)
#define HIRC_5M                  (0x3uL << 12)
*/
#define HIRCSEL_HIRC1            (SCUCG_CLKSRCR_HIRCSEL_HIRC1 << SCUCG_CLKSRCR_HIRCSEL_Pos)  // 40MHz HIRC
#define HIRCSEL_HIRC2            (SCUCG_CLKSRCR_HIRCSEL_HIRC2 << SCUCG_CLKSRCR_HIRCSEL_Pos)  // 20MHz HIRC
#define HIRCSEL_HIRC4            (SCUCG_CLKSRCR_HIRCSEL_HIRC4 << SCUCG_CLKSRCR_HIRCSEL_Pos)  // 10MHz HIRC
#define HIRCSEL_HIRC8            (SCUCG_CLKSRCR_HIRCSEL_HIRC8 << SCUCG_CLKSRCR_HIRCSEL_Pos)  //  5MHz HIRC 

// XMFRNG Selection Control
/*
#define XTAL_XM                  (0x0uL << 8)
#define EXT_XM                   (0x1uL << 8)
*/
#define XMFRNG_Xtal              (SCUCG_CLKSRCR_XMFRNG_Xtal  << SCUCG_CLKSRCR_XMFRNG_Pos)
#define XMFRNG_Clock             (SCUCG_CLKSRCR_XMFRNG_Clock << SCUCG_CLKSRCR_XMFRNG_Pos)

// System Clock Source Enable Control
/*
#define EN_XSOSC                 (0x1uL << 0)
#define EN_XMOSC                 (0x1uL << 1)
#define EN_HIRC                  (0x1uL << 2)
#define EN_WDTRC                 (0x1uL << 3)
*/
#define CLKSRCR_WDTRCEN          (0x1uL << SCUCG_CLKSRCR_WDTRCEN_Pos)
#define CLKSRCR_HIRCEN           (0x1uL << SCUCG_CLKSRCR_HIRCEN_Pos)
#define CLKSRCR_XMOSCEN          (0x1uL << SCUCG_CLKSRCR_XMOSCEN_Pos)
#define CLKSRCR_XSOSCEN          (0x1uL << SCUCG_CLKSRCR_XSOSCEN_Pos)

// WT & LCD Clock Divider Selection Control (Divider 2)
/*
#define MCLK_64                  0
#define MCLK_128                 1
#define MCLK_256                 2
#define MCLK_512                 3
#define MCLK_1024                4
*/
#define WLDIV_MCLK64             (SCUCG_SCDIVR1_WLDIV_MCLK64   << SCUCG_SCDIVR1_WLDIV_Pos)   // MCLK/64
#define WLDIV_MCLK128            (SCUCG_SCDIVR1_WLDIV_MCLK128  << SCUCG_SCDIVR1_WLDIV_Pos)   // MCLK/128
#define WLDIV_MCLK256            (SCUCG_SCDIVR1_WLDIV_MCLK256  << SCUCG_SCDIVR1_WLDIV_Pos)   // MCLK/256
#define WLDIV_MCLK512            (SCUCG_SCDIVR1_WLDIV_MCLK512  << SCUCG_SCDIVR1_WLDIV_Pos)   // MCLK/512
#define WLDIV_MCLK1024           (SCUCG_SCDIVR1_WLDIV_MCLK1024 << SCUCG_SCDIVR1_WLDIV_Pos)   // MCLK/1024

// HCLK Divider Selection Control (Divider 0)
/*
#define MCLK_16                  0
#define MCLK_8                   1
#define MCLK_4                   2
#define MCLK_2                   3
#define MCLK_1                   4
*/
#define HDIV_MCLK16              (SCUCG_SCDIVR1_HDIV_MCLK16 << SCUCG_SCDIVR1_HDIV_Pos)    // MCLK/16
#define HDIV_MCLK8               (SCUCG_SCDIVR1_HDIV_MCLK8  << SCUCG_SCDIVR1_HDIV_Pos)    // MCLK/8    
#define HDIV_MCLK4               (SCUCG_SCDIVR1_HDIV_MCLK4  << SCUCG_SCDIVR1_HDIV_Pos)    // MCLK/4
#define HDIV_MCLK2               (SCUCG_SCDIVR1_HDIV_MCLK2  << SCUCG_SCDIVR1_HDIV_Pos)    // MCLK/2
#define HDIV_MCLK1               (SCUCG_SCDIVR1_HDIV_MCLK1  << SCUCG_SCDIVR1_HDIV_Pos)    // MCLK/1

// SysTick Timer Clock Divider Selection Control (Divider 3)
/*
#define SCU_HCLK_1               0
#define SCU_HCLK_2               1
#define SCU_HCLK_4               2
#define SCU_HCLK_8               3
*/
#define SYSTDIV_HCLK1            (SCUCG_SCDIVR2_SYSTDIV_HCLK1 << SCUCG_SCDIVR2_SYSTDIV_Pos)  // HCLK/1
#define SYSTDIV_HCLK2            (SCUCG_SCDIVR2_SYSTDIV_HCLK2 << SCUCG_SCDIVR2_SYSTDIV_Pos)  // HCLK/2
#define SYSTDIV_HCLK4            (SCUCG_SCDIVR2_SYSTDIV_HCLK4 << SCUCG_SCDIVR2_SYSTDIV_Pos)  // HCLK/4
#define SYSTDIV_HCLK8            (SCUCG_SCDIVR2_SYSTDIV_HCLK8 << SCUCG_SCDIVR2_SYSTDIV_Pos)  // HCLK/8

// PCLK Divider Selection Control (Divider 1)
#define PDIV_HCLK1               (SCUCG_SCDIVR2_PDIV_HCLK1 << SCUCG_SCDIVR2_PDIV_Pos)  // HCLK/1
#define PDIV_HCLK2               (SCUCG_SCDIVR2_PDIV_HCLK2 << SCUCG_SCDIVR2_PDIV_Pos)  // HCLK/2
#define PDIV_HCLK4               (SCUCG_SCDIVR2_PDIV_HCLK4 << SCUCG_SCDIVR2_PDIV_Pos)  // HCLK/4
#define PDIV_HCLK8               (SCUCG_SCDIVR2_PDIV_HCLK8 << SCUCG_SCDIVR2_PDIV_Pos)  // HCLK/8

// Clock Output Enable/Disable Control
/*
#define DIS_CLKOUT               0
#define EN_CLKOUT                1
*/
#define CLKOEN_Disable           SCUCG_CLKOCR_CLKOEN_Disable
#define CLKOEN_Enable            SCUCG_CLKOCR_CLKOEN_Enable

// Clock Output Polarity Selection Control
/*
#define POL_L                    (0x0uL << 6)
#define POL_H                    (0x1uL << 6)
*/
#define POLSEL_Low               (SCUCG_CLKOCR_POLSEL_Low  << SCUCG_CLKOCR_POLSEL_Pos)    // Low level during disable  
#define POLSEL_High              (SCUCG_CLKOCR_POLSEL_High << SCUCG_CLKOCR_POLSEL_Pos)    // High level during disable

// Clock Output Divider Selection Control
/*
#define CLKODIV_1                (0x0uL << 3)
#define CLKODIV_2                (0x1uL << 3)
#define CLKODIV_4                (0x2uL << 3)
#define CLKODIV_8                (0x3uL << 3)
#define CLKODIV_16               (0x4uL << 3)
#define CLKODIV_32               (0x5uL << 3)
#define CLKODIV_64               (0x6uL << 3)
#define CLKODIV_128              (0x7uL << 3)
*/
#define CLKODIV_SelectedClock1   (SCUCG_CLKOCR_CLKODIV_SelectedClock1   << SCUCG_CLKOCR_CLKODIV_Pos)  // Selected Clock/1
#define CLKODIV_SelectedClock2   (SCUCG_CLKOCR_CLKODIV_SelectedClock2   << SCUCG_CLKOCR_CLKODIV_Pos)  // Selected Clock/2
#define CLKODIV_SelectedClock4   (SCUCG_CLKOCR_CLKODIV_SelectedClock4   << SCUCG_CLKOCR_CLKODIV_Pos)  // Selected Clock/4
#define CLKODIV_SelectedClock8   (SCUCG_CLKOCR_CLKODIV_SelectedClock8   << SCUCG_CLKOCR_CLKODIV_Pos)  // Selected Clock/8
#define CLKODIV_SelectedClock16  (SCUCG_CLKOCR_CLKODIV_SelectedClock16  << SCUCG_CLKOCR_CLKODIV_Pos)  // Selected Clock/16
#define CLKODIV_SelectedClock32  (SCUCG_CLKOCR_CLKODIV_SelectedClock32  << SCUCG_CLKOCR_CLKODIV_Pos)  // Selected Clock/32
#define CLKODIV_SelectedClock64  (SCUCG_CLKOCR_CLKODIV_SelectedClock64  << SCUCG_CLKOCR_CLKODIV_Pos)  // Selected Clock/64
#define CLKODIV_SelectedClock128 (SCUCG_CLKOCR_CLKODIV_SelectedClock128 << SCUCG_CLKOCR_CLKODIV_Pos)  // Selected Clock/128

// Clock Output Target Selection Control
/*
#define MCLK_OUT                 (0x0uL << 0)
#define WDTRC_OUT                (0x1uL << 0)
#define HIRC_OUT                 (0x2uL << 0)
#define HCLK_OUT                 (0x3uL << 0)
#define PCLK_OUT                 (0x4uL << 0)
*/
#define CLKOS_MCLK               (SCUCG_CLKOCR_CLKOS_MCLK  << SCUCG_CLKOCR_CLKOS_Pos)
#define CLKOS_WDTRC              (SCUCG_CLKOCR_CLKOS_WDTRC << SCUCG_CLKOCR_CLKOS_Pos)
#define CLKOS_HIRC               (SCUCG_CLKOCR_CLKOS_HIRC  << SCUCG_CLKOCR_CLKOS_Pos)
#define CLKOS_HCLK               (SCUCG_CLKOCR_CLKOS_HCLK  << SCUCG_CLKOCR_CLKOS_Pos)
#define CLKOS_PCLK               (SCUCG_CLKOCR_CLKOS_PCLK  << SCUCG_CLKOCR_CLKOS_Pos)

// Clock Monitoring Action Selection Control
/*
#define FLAG_CHK_M               (0x0uL << 5)
#define RST_GEN_M                (0x1uL << 5)
#define SYS_CHG_M                (0x2uL << 5)
*/
#define MACTS_FlagChk            (SCUCG_CMONCR_MACTS_FlagChk   << SCUCG_CMONCR_MACTS_Pos)
#define MACTS_RstGen             (SCUCG_CMONCR_MACTS_RstGen    << SCUCG_CMONCR_MACTS_Pos)
#define MACTS_SysClkChg          (SCUCG_CMONCR_MACTS_SysClkChg << SCUCG_CMONCR_MACTS_Pos)

// Clock Monitoring Target Selection Control
/*
#define MCLK_MON                 (0x0uL << 0)
#define HIRC_MON                 (0x1uL << 0)
#define XMOSC_MON                (0x2uL << 0)
#define XSOSC_MON                (0x3uL << 0)
*/
#define MONCS_MCLK               (SCUCG_CMONCR_MONCS_MCLK  << SCUCG_CMONCR_MONCS_Pos)
#define MONCS_HIRC               (SCUCG_CMONCR_MONCS_HIRC  << SCUCG_CMONCR_MONCS_Pos)
#define MONCS_XMOSC              (SCUCG_CMONCR_MONCS_XMOSC << SCUCG_CMONCR_MONCS_Pos)
#define MONCS_XSOSC              (SCUCG_CMONCR_MONCS_XSOSC << SCUCG_CMONCR_MONCS_Pos)

// Peripheral Clock Enable Control 1
/*
#define PERI_PA                  (0x1uL << 0)
#define PERI_PB                  (0x1uL << 1)
#define PERI_PC                  (0x1uL << 2)
#define PERI_PD                  (0x1uL << 3)
#define PERI_PE                  (0x1uL << 4)
#define PERI_PF                  (0x1uL << 5)
#define PERI_T13                 (0x1uL << 8)
#define PERI_T14                 (0x1uL << 9)
#define PERI_T15                 (0x1uL << 10)
#define PERI_T16                 (0x1uL << 11)
#define PERI_T10                 (0x1uL << 16)
#define PERI_T11                 (0x1uL << 17)
#define PERI_T12                 (0x1uL << 18)
#define PERI_T30                 (0x1uL << 19)
#define PERI_T20                 (0x1uL << 20)
#define PERI_T21                 (0x1uL << 21)
*/
#define PPCLKEN1_T21CLKE         (0x1uL << SCUCG_PPCLKEN1_T21CLKE_Pos)
#define PPCLKEN1_T20CLKE         (0x1uL << SCUCG_PPCLKEN1_T20CLKE_Pos)
#define PPCLKEN1_T30CLKE         (0x1uL << SCUCG_PPCLKEN1_T30CLKE_Pos)
#define PPCLKEN1_T12CLKE         (0x1uL << SCUCG_PPCLKEN1_T12CLKE_Pos)
#define PPCLKEN1_T11CLKE         (0x1uL << SCUCG_PPCLKEN1_T11CLKE_Pos)
#define PPCLKEN1_T10CLKE         (0x1uL << SCUCG_PPCLKEN1_T10CLKE_Pos)
#define PPCLKEN1_T16CLKE         (0x1uL << SCUCG_PPCLKEN1_T16CLKE_Pos)
#define PPCLKEN1_T15CLKE         (0x1uL << SCUCG_PPCLKEN1_T15CLKE_Pos)
#define PPCLKEN1_T14CLKE         (0x1uL << SCUCG_PPCLKEN1_T14CLKE_Pos)
#define PPCLKEN1_T13CLKE         (0x1uL << SCUCG_PPCLKEN1_T13CLKE_Pos)
#define PPCLKEN1_PFCLKE          (0x1uL << SCUCG_PPCLKEN1_PFCLKE_Pos)
#define PPCLKEN1_PECLKE          (0x1uL << SCUCG_PPCLKEN1_PECLKE_Pos)
#define PPCLKEN1_PDCLKE          (0x1uL << SCUCG_PPCLKEN1_PDCLKE_Pos)
#define PPCLKEN1_PCCLKE          (0x1uL << SCUCG_PPCLKEN1_PCCLKE_Pos)
#define PPCLKEN1_PBCLKE          (0x1uL << SCUCG_PPCLKEN1_PBCLKE_Pos)
#define PPCLKEN1_PACLKE          (0x1uL << SCUCG_PPCLKEN1_PACLKE_Pos)

// Peripheral Clock Enable Control 2
/*
#define PERI_UST10               (0x1uL << 0)
#define PERI_UST11               (0x1uL << 1)
#define PERI_UT0                 (0x1uL << 2)
#define PERI_UT1                 (0x1uL << 3)
#define PERI_UST12               (0x1uL << 4)
#define PERI_UST13               (0x1uL << 5)
#define PERI_I2C0                (0x1uL << 6)
#define PERI_I2C1                (0x1uL << 7)
#define PERI_I2C2                (0x1uL << 8)
#define PERI_ADC                 (0x1uL << 10)
#define PERI_CRC                 (0x1uL << 12)
#define PERI_LCD                 (0x1uL << 13)
#define PERI_WT                  (0x1uL << 16)
#define PERI_WDT                 (0x1uL << 17)
#define PERI_LVI                 (0x1uL << 18)
#define PERI_FMC                 (0x1uL << 19)
*/
#define PPCLKEN2_FMCLKE          (0x1uL << SCUCG_PPCLKEN2_FMCLKE_Pos)
#define PPCLKEN2_LVICLKE         (0x1uL << SCUCG_PPCLKEN2_LVICLKE_Pos)
#define PPCLKEN2_WDTCLKE         (0x1uL << SCUCG_PPCLKEN2_WDTCLKE_Pos)
#define PPCLKEN2_WTCLKE          (0x1uL << SCUCG_PPCLKEN2_WTCLKE_Pos)
#define PPCLKEN2_LCDCLKE         (0x1uL << SCUCG_PPCLKEN2_LCDCLKE_Pos)
#define PPCLKEN2_CRCLKE          (0x1uL << SCUCG_PPCLKEN2_CRCLKE_Pos)
#define PPCLKEN2_ADCLKE          (0x1uL << SCUCG_PPCLKEN2_ADCLKE_Pos)
#define PPCLKEN2_I2C2CLKE        (0x1uL << SCUCG_PPCLKEN2_I2C2CLKE_Pos)
#define PPCLKEN2_I2C1CLKE        (0x1uL << SCUCG_PPCLKEN2_I2C1CLKE_Pos)
#define PPCLKEN2_I2C0CLKE        (0x1uL << SCUCG_PPCLKEN2_I2C0CLKE_Pos)
#define PPCLKEN2_UST13CLKE       (0x1uL << SCUCG_PPCLKEN2_UST13CLKE_Pos)
#define PPCLKEN2_UST12CLKE       (0x1uL << SCUCG_PPCLKEN2_UST12CLKE_Pos)
#define PPCLKEN2_UT1CLKE         (0x1uL << SCUCG_PPCLKEN2_UT1CLKE_Pos)
#define PPCLKEN2_UT0CLKE         (0x1uL << SCUCG_PPCLKEN2_UT0CLKE_Pos)
#define PPCLKEN2_UST11CLKE       (0x1uL << SCUCG_PPCLKEN2_UST11CLKE_Pos)
#define PPCLKEN2_UST10CLKE       (0x1uL << SCUCG_PPCLKEN2_UST10CLKE_Pos)

// Peripheral Clock Enable/Disable Control
/*
#define DIS_PERICLK              0
#define EN_PERICLK               1
*/
#define PPxCLKE_Disable          SCUCG_PPCLKEN1_PACLKE_Disable
#define PPxCLKE_Enable           SCUCG_PPCLKEN1_PACLKE_Enable

// Timer/Counter 20 Clock Selecion Control
/*
#define SCUCG_T20CLK             (SCUCG_PPCLKSR_T20CLK_Msk)
#define SCUCG_T20CLK_XSOSC       (0x0uL << SCUCG_PPCLKSR_T20CLK_Pos)
#define SCUCG_T20CLK_PCLK        (0x1uL << SCUCG_PPCLKSR_T20CLK_Pos)
*/
#define PPCLKSR_T20CLK           (SCUCG_PPCLKSR_T20CLK_Msk)
#define T20CLK_XSOSC             (SCUCG_PPCLKSR_T20CLK_XSOSC << SCUCG_PPCLKSR_T20CLK_Pos)
#define T20CLK_PCLK              (SCUCG_PPCLKSR_T20CLK_PCLK  << SCUCG_PPCLKSR_T20CLK_Pos)

// Timer/Counter 30 Clock Selection Control
/*
#define SCUCG_T30CLK             (SCUCG_PPCLKSR_T30CLK_Msk)
#define SCUCG_T30CLK_MCLK        (0x0uL << SCUCG_PPCLKSR_T30CLK_Pos)
#define SCUCG_T30CLK_PCLK        (0x1uL << SCUCG_PPCLKSR_T30CLK_Pos)
*/
#define PPCLKSR_T30CLK           (SCUCG_PPCLKSR_T30CLK_Msk)
#define T30CLK_MCLK              (SCUCG_PPCLKSR_T30CLK_MCLK << SCUCG_PPCLKSR_T30CLK_Pos)
#define T30CLK_PCLK              (SCUCG_PPCLKSR_T30CLK_PCLK << SCUCG_PPCLKSR_T30CLK_Pos)

// LCD Driver Clock Selection Control
/*
#define SCUCG_LCDCLK             (SCUCG_PPCLKSR_LCDCLK_Msk)
#define SCUCG_LCDCLK_MCLK        (0x0uL << SCUCG_PPCLKSR_LCDCLK_Pos)
#define SCUCG_LCDCLK_XSOSC       (0x1uL << SCUCG_PPCLKSR_LCDCLK_Pos)
#define SCUCG_LCDCLK_WDTRC       (0x2uL << SCUCG_PPCLKSR_LCDCLK_Pos)
*/
#define PPCLKSR_LCDCLK           (SCUCG_PPCLKSR_LCDCLK_Msk)
#define LCDCLK_DividedMCLK       (SCUCG_PPCLKSR_LCDCLK_DividedMCLK << SCUCG_PPCLKSR_LCDCLK_Pos)
#define LCDCLK_XSOSC             (SCUCG_PPCLKSR_LCDCLK_XSOSC       << SCUCG_PPCLKSR_LCDCLK_Pos)
#define LCDCLK_WDTRC             (SCUCG_PPCLKSR_LCDCLK_WDTRC       << SCUCG_PPCLKSR_LCDCLK_Pos)

// Watch Timer Clock Selection Control
/*
#define SCUCG_WTCLK              (SCUCG_PPCLKSR_WTCLK_Msk)
#define SCUCG_WTCLK_MCLK         (0x0uL << SCUCG_PPCLKSR_WTCLK_Pos)
#define SCUCG_WTCLK_XSOSC        (0x1uL << SCUCG_PPCLKSR_WTCLK_Pos)
#define SCUCG_WTCLK_WDTRC        (0x2uL << SCUCG_PPCLKSR_WTCLK_Pos)
*/
#define PPCLKSR_WTCLK            (SCUCG_PPCLKSR_WTCLK_Msk)
#define WTCLK_DividedMCLK        (SCUCG_PPCLKSR_WTCLK_DividedMCLK << SCUCG_PPCLKSR_WTCLK_Pos)
#define WTCLK_XSOSC              (SCUCG_PPCLKSR_WTCLK_XSOSC       << SCUCG_PPCLKSR_WTCLK_Pos)
#define WTCLK_WDTRC              (SCUCG_PPCLKSR_WTCLK_WDTRC       << SCUCG_PPCLKSR_WTCLK_Pos)

// Watch-Dog Timer Clock Selection Control
/*
#define SCUCG_WDTCLK             (SCUCG_PPCLKSR_WDTCLK_Msk)
#define SCUCG_WDTCLK_WDTRC       (0x0uL << SCUCG_PPCLKSR_WDTCLK_Pos)
#define SCUCG_WDTCLK_PCLK        (0x1uL << SCUCG_PPCLKSR_WDTCLK_Pos)
*/
#define PPCLKSR_WDTCLK           (SCUCG_PPCLKSR_WDTCLK_Msk)
#define WDTCLK_WDTRC             (SCUCG_PPCLKSR_WDTCLK_WDTRC << SCUCG_PPCLKSR_WDTCLK_Pos)
#define WDTCLK_PCLK              (SCUCG_PPCLKSR_WDTCLK_PCLK  << SCUCG_PPCLKSR_WDTCLK_Pos)

// Peripheral Reset Control 1
#define PPRST1_T21RST            (0x1uL << SCUCG_PPRST1_T21RST_Pos)
#define PPRST1_T20RST            (0x1uL << SCUCG_PPRST1_T20RST_Pos)
#define PPRST1_T30RST            (0x1uL << SCUCG_PPRST1_T30RST_Pos)
#define PPRST1_T12RST            (0x1uL << SCUCG_PPRST1_T12RST_Pos)
#define PPRST1_T11RST            (0x1uL << SCUCG_PPRST1_T11RST_Pos)
#define PPRST1_T10RST            (0x1uL << SCUCG_PPRST1_T10RST_Pos)
#define PPRST1_T16RST            (0x1uL << SCUCG_PPRST1_T16RST_Pos)
#define PPRST1_T15RST            (0x1uL << SCUCG_PPRST1_T15RST_Pos)
#define PPRST1_T14RST            (0x1uL << SCUCG_PPRST1_T14RST_Pos)
#define PPRST1_T13RST            (0x1uL << SCUCG_PPRST1_T13RST_Pos)
#define PPRST1_PFRST             (0x1uL << SCUCG_PPRST1_PFRST_Pos)
#define PPRST1_PERST             (0x1uL << SCUCG_PPRST1_PERST_Pos)
#define PPRST1_PDRST             (0x1uL << SCUCG_PPRST1_PDRST_Pos)
#define PPRST1_PCRST             (0x1uL << SCUCG_PPRST1_PCRST_Pos)
#define PPRST1_PBRST             (0x1uL << SCUCG_PPRST1_PBRST_Pos)
#define PPRST1_PARST             (0x1uL << SCUCG_PPRST1_PARST_Pos)

// Peripheral Reset Control 2
#define PPRST2_FMCRST            (0x1uL << SCUCG_PPRST2_FMCRST_Pos)
#define PPRST2_LVIRST            (0x1uL << SCUCG_PPRST2_LVIRST_Pos)
#define PPRST2_WTRST             (0x1uL << SCUCG_PPRST2_WTRST_Pos)
#define PPRST2_LCDRST            (0x1uL << SCUCG_PPRST2_LCDRST_Pos)
#define PPRST2_CRRST             (0x1uL << SCUCG_PPRST2_CRRST_Pos)
#define PPRST2_ADRST             (0x1uL << SCUCG_PPRST2_ADRST_Pos)
#define PPRST2_I2C2RST           (0x1uL << SCUCG_PPRST2_I2C2RST_Pos)
#define PPRST2_I2C1RST           (0x1uL << SCUCG_PPRST2_I2C1RST_Pos)
#define PPRST2_I2C0RST           (0x1uL << SCUCG_PPRST2_I2C0RST_Pos)
#define PPRST2_UST13RST          (0x1uL << SCUCG_PPRST2_UST13RST_Pos)
#define PPRST2_UST12RST          (0x1uL << SCUCG_PPRST2_UST12RST_Pos)
#define PPRST2_UT1RST            (0x1uL << SCUCG_PPRST2_UT1RST_Pos)
#define PPRST2_UT0RST            (0x1uL << SCUCG_PPRST2_UT0RST_Pos)
#define PPRST2_UST11RST          (0x1uL << SCUCG_PPRST2_UST11RST_Pos)
#define PPRST2_UST10RST          (0x1uL << SCUCG_PPRST2_UST10RST_Pos)

// External Main Oscillator Filter Selection Control
/*
#define XTAL_4DOT5MHZ            (0x0 << 0)
#define XTAL_6DOT5MHZ            (0x1 << 0)
#define XTAL_8DOT5MHZ            (0x2 << 0)
#define XTAL_10DOT5MHZ           (0x3 << 0)
#define XTAL_12DOT5MHZ           (0x4 << 0)
#define XTAL_16DOT5MHZ           (0x5 << 0)
*/
#define XRNS_LE4p5MHz            (SCUCG_XTFLSR_XRNS_LE4p5MHz  << SCUCG_XTFLSR_XRNS_Pos)   //            x-tal LE 4.5MHz
#define XRNS_LE6p5MHz            (SCUCG_XTFLSR_XRNS_LE6p5MHz  << SCUCG_XTFLSR_XRNS_Pos)   //  4.5MHz GT x-tal LE 6.5MHz
#define XRNS_LE8p5MHz            (SCUCG_XTFLSR_XRNS_LE8p5MHz  << SCUCG_XTFLSR_XRNS_Pos)   //  6.5MHz GT x-tal LE 8.5MHz
#define XRNS_LE10p5MHz           (SCUCG_XTFLSR_XRNS_LE10p5MHz << SCUCG_XTFLSR_XRNS_Pos)   //  8.5MHz GT x-tal LE 10.5MHz
#define XRNS_LE12p5MHz           (SCUCG_XTFLSR_XRNS_LE12p5MHz << SCUCG_XTFLSR_XRNS_Pos)   // 10.5MHz GT x-tal LE 12.5MHz
#define XRNS_LE16p5MHz           (SCUCG_XTFLSR_XRNS_LE16p5MHz << SCUCG_XTFLSR_XRNS_Pos)   // 12.5MHz GT x-tal LE 16.5MHz

//******************************************************************************
// Macro
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         Set Divider 0/1/2/3
 * @param[in]     scu_hdiv
 *                   -  scu_hdiv:      HDIV_MCLK16, HDIV_MCLK8, HDIV_MCLK4, HDIV_MCLK2, HDIV_MCLK1
 *                   -  scu_pdiv:      PDIV_HCLK1, PDIV_HCLK2, PDIV_HCLK4, PDIV_HCLK8
 *                   -  scu_wldiv:     WLDIV_MCLK64, WLDIV_MCLK128, WLDIV_MCLK256, WLDIV_MCLK512, WLDIV_MCLK1024
 *                   -  scu_systdiv:   SYSTDIV_HCLK1, SYSTDIV_HCLK2, SYSTDIV_HCLK4, SYSTDIV_HCLK8
 * @details       This macro sets dividers.
 *//*-------------------------------------------------------------------------*/
#define SCUCG_SetHCLK( scu_hdiv )            (SCUCG->SCDIVR1_b.HDIV = scu_hdiv)
#define SCUCG_SetPCLK( scu_pdiv )            (SCUCG->SCDIVR2_b.PDIV = scu_pdiv)
#define SCUCG_SetWtLcd( scu_wldiv )          (SCUCG->SCDIVR1_b.WLDIV = scu_wldiv)
#define SCUCG_SetSysTick( scu_systdiv )      (SCUCG->SCDIVR2_b.SYSTDIV = scu_systdiv)

/*-------------------------------------------------------------------------*//**
 * @brief         Set Clock Output Control Register (CLKOCR)
 * @param[in]     u32Clko
 *                   Values for CLKO frequency, Polarity, and Divider
 * @details       This macro sets clock output control register for CLKO.
 *//*-------------------------------------------------------------------------*/
#define SCUCG_SetClkOutReg( u32Clko )        (SCUCG->CLKOCR = u32Clko)

/*-------------------------------------------------------------------------*//**
 * @brief         Disable Clock Monitoring
 * @details       This macro stops clock monitoring.
 *//*-------------------------------------------------------------------------*/
#define SCUCG_StopMonClk()                   (SCUCG->CMONCR_b.MONEN = 0)

/*-------------------------------------------------------------------------*//**
 * @brief         Get MONFLAG of Clock Monitoring
 * @details       This macro reads MONFLAG of Clock Monitoring.
 *//*-------------------------------------------------------------------------*/
#define SCUCG_GetMonFlag()                   (SCUCG->CMONCR_b.MONFLAG)

/*-------------------------------------------------------------------------*//**
 * @brief         Set Timer/Counter 20 Clock
 * @param[in]     clk
 *                   -  T20CLK_XSOSC, T20CLK_PCLK
 * @details       This macro sets Timer/Counter 20 clock.
 *//*-------------------------------------------------------------------------*/
#define SCUCG_SetT20ClkSrc( clk )            (SCUCG->PPCLKSR_b.T20CLK = clk)

/*-------------------------------------------------------------------------*//**
 * @brief         Set Timer/Counter 30 Clock
 * @param[in]     clk
 *                   -  T30CLK_MCLK, T30CLK_PCLK
 * @details       This macro sets Timer/Counter 30 clock.
 *//*-------------------------------------------------------------------------*/
#define SCUCG_SetT30ClkSrc( clk )            (SCUCG->PPCLKSR_b.T30CLK = clk)

/*-------------------------------------------------------------------------*//**
 * @brief         Set LCD Driver Clock
 * @param[in]     clk
 *                   -  LCDCLK_DividedMCLK, LCDCLK_XSOSC, LCDCLK_WDTRC
 * @details       This macro sets LCD Driver clock.
 *//*-------------------------------------------------------------------------*/
#define SCUCG_SetLcdClk( clk )               (SCUCG->PPCLKSR_b.LCDCLK = clk)

/*-------------------------------------------------------------------------*//**
 * @brief         Set Watch Timer Clock
 * @param[in]     clk
 *                   -  WTCLK_DividedMCLK, WTCLK_XSOSC, WTCLK_WDTRC
 * @details       This macro sets Watch Timer clock.
 *//*-------------------------------------------------------------------------*/
#define SCUCG_SetWtClk( clk )                (SCUCG->PPCLKSR_b.WTCLK = clk)

/*-------------------------------------------------------------------------*//**
 * @brief         Set Watch-Dog Timer Clock
 * @param[in]     clk
 *                   -  WDTCLK_WDTRC, WDTCLK_PCLK
 * @details       This macro sets Watch-Dog Timer clock.
 *//*-------------------------------------------------------------------------*/
#define SCUCG_SetWdtClk( clk )               (SCUCG->PPCLKSR_b.WDTCLK = clk)

//******************************************************************************
// Function
//******************************************************************************

void SCUCG_SetMonClk( uint32_t u32Acts, uint32_t u32Target );
void SCUCG_SetClkSrc( uint32_t u32FreIRC, uint32_t u32TypeXM, uint32_t u32ClkSrc );
void SCUCG_EnClkSrc( uint32_t u32ClkSrc, uint32_t u32HircDiv );
void SCUCG_DisClkSrc( uint32_t u32ClkSrc );
void SCUCG_ChangeSysClk( uint32_t u32Target );
void SCUCG_SetMainXtalPin( uint32_t u32XtalFilter );
void SCUCG_SetSubXtalPin( void );
void SCUCG_SetClkDiv( uint32_t u32Div02, uint32_t u32Div13 );
void SCUCG_SetClkoPin( void );
void SCUCG_SetClkoCtrl( uint32_t u32ClkSrc, uint32_t u32Level, uint32_t u32Div );
void SCUCG_SetPeriClk( uint32_t u32PeriClk1, uint32_t u32PeriClk2 );
void SCUCG_SetEachPeriClk1( uint32_t u32PeriClk1, uint32_t Ind );
void SCUCG_SetEachPeriClk2( uint32_t u32PeriClk2, uint32_t u32Ind );
void SCUCG_RstPeri( uint32_t u32PeriRst1, uint32_t u32PeriRst2 );
void SCUCG_RstEachPeri1( uint32_t u32EachPeri1 );
void SCUCG_RstEachPeri2( uint32_t u32EachPeri2 );
void SCUCG_PeriClockSel( uint32_t u32Peri, uint32_t u32ClkSrc );

#ifdef __cplusplus
}
#endif

#endif   /* _SCUCG_H_ */

