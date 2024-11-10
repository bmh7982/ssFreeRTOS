/***************************************************************************//**
 * @file          A31G12x_SCULV.h
 * @brief         Driver Header File for SCU Low Voltage Reset & Indicator
 * @version       1.0
 * @date          2019-09-01
 * @author        ABOV Application Team
 *
 * Copyright (c) 2019, ABOV Semiconductor. All rights reserved.
 *//***************************************************************************/

#ifndef _SCULV_H_
#define _SCULV_H_

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Constant
//******************************************************************************

// LVI Enable/Disable Control
/*
#define LVI_DIS                  (0x00uL << 7)
#define LVI_EN                   (0x01uL << 7)
*/
#define LVIEN_Disable            (SCULV_LVICR_LVIEN_Disable << SCULV_LVICR_LVIEN_Pos)
#define LVIEN_Enable             (SCULV_LVICR_LVIEN_Enable  << SCULV_LVICR_LVIEN_Pos)

// LVI Interrupt Enable/Disable Control
/*
#define LVI_INT_DIS              (0x00uL << 5)
#define LVI_INT_EN               (0x01uL << 5)
*/
#define LVINTEN_Disable          (SCULV_LVICR_LVINTEN_Disable << SCULV_LVICR_LVINTEN_Pos)
#define LVINTEN_Enable           (SCULV_LVICR_LVINTEN_Enable  << SCULV_LVICR_LVINTEN_Pos)

// LVI Voltage Selection Control
/*
#define LV_1DOT88                (0x03uL)
#define LV_2DOT00                (0x04uL)
#define LV_2DOT13                (0x05uL)
#define LV_2DOT28                (0x06uL)
#define LV_2DOT46                (0x07uL)
#define LV_2DOT67                (0x08uL)
#define LV_3DOT04                (0x09uL)
#define LV_3DOT20                (0x0AuL)
#define LV_3DOT55                (0x0BuL)
#define LV_3DOT75                (0x0CuL)
#define LV_3DOT99                (0x0DuL)
#define LV_4DOT25                (0x0EuL)
#define LV_4DOT55                (0x0FuL)
*/
#define LVIVS_2p00V              (SCULV_LVICR_LVIVS_2p00V << SCULV_LVICR_LVIVS_Pos)    // 2.00V                                                              */
#define LVIVS_2p13V              (SCULV_LVICR_LVIVS_2p13V << SCULV_LVICR_LVIVS_Pos)    // 2.13V                                                              */
#define LVIVS_2p28V              (SCULV_LVICR_LVIVS_2p28V << SCULV_LVICR_LVIVS_Pos)    // 2.28V                                                              */
#define LVIVS_2p46V              (SCULV_LVICR_LVIVS_2p46V << SCULV_LVICR_LVIVS_Pos)    // 2.46V                                                              */
#define LVIVS_2p67V              (SCULV_LVICR_LVIVS_2p67V << SCULV_LVICR_LVIVS_Pos)    // 2.67V                                                              */
#define LVIVS_3p04V              (SCULV_LVICR_LVIVS_3p04V << SCULV_LVICR_LVIVS_Pos)    // 3.04V                                                              */
#define LVIVS_3p20V              (SCULV_LVICR_LVIVS_3p20V << SCULV_LVICR_LVIVS_Pos)    // 3.20V                                                              */
#define LVIVS_3p55V              (SCULV_LVICR_LVIVS_3p55V << SCULV_LVICR_LVIVS_Pos)    // 3.55V                                                              */
#define LVIVS_3p75V              (SCULV_LVICR_LVIVS_3p75V << SCULV_LVICR_LVIVS_Pos)    // 3.75V                                                              */
#define LVIVS_3p99V              (SCULV_LVICR_LVIVS_3p99V << SCULV_LVICR_LVIVS_Pos)    // 3.99V                                                              */
#define LVIVS_4p25V              (SCULV_LVICR_LVIVS_4p25V << SCULV_LVICR_LVIVS_Pos)    // 4.25V                                                              */
#define LVIVS_4p55V              (SCULV_LVICR_LVIVS_4p55V << SCULV_LVICR_LVIVS_Pos)    // 4.55V                                                              */

//******************************************************************************
// Macro
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         Enable/Disable LVR
 * @details       This macro disables/enables LVR. If any value is written, the LVR register won't be changed until to POR.
 *//*-------------------------------------------------------------------------*/
#define SCULV_DisLVR()           (SCULV->LVRCR_b.LVREN = 0x55)
#define SCULV_EnLVR()            (SCULV->LVRCR_b.LVREN = 0x8A)

/*-------------------------------------------------------------------------*//**
 * @brief         Enable/Disable LVI Interrupt
 * @details       This macro sets the LVI interrupt enable bit.
 *//*-------------------------------------------------------------------------*/
#define SCULV_EnLviInt()         (SCULV->LVICR_b.LVINTEN = 1)
#define SCULV_DisLviInt()        (SCULV->LVICR_b.LVINTEN = 0)

/*-------------------------------------------------------------------------*//**
 * @brief         Get/Clear LVI Interrupt Flag
 * @details       This macro gets/clears the LVI interrupt flag.
 *//*-------------------------------------------------------------------------*/
#define SCULV_GetLviFlag()       (SCULV->LVICR_b.LVIFLAG)
#define SCULV_ClrLviFlag()       (SCULV->LVICR_b.LVIFLAG = 1)

//******************************************************************************
// Function
//******************************************************************************

void SCULV_SetLviBlk( uint32_t u32LviEnBit, uint32_t u32LviIntEnBit, uint32_t u32LviLevel );

#ifdef __cplusplus
}
#endif

#endif   /* _SCULV_H_ */

