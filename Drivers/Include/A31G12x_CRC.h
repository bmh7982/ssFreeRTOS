/***************************************************************************//**
 * @file          A31G12x_CRC.h
 * @brief         Driver Header File for CRC & Checksum
 * @version       1.0
 * @date          2019-09-01
 * @author        ABOV Application Team
 *
 * Copyright (c) 2019, ABOV Semiconductor. All rights reserved.
 *//***************************************************************************/

#ifndef _CRC_H_
#define _CRC_H_

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Constant
//******************************************************************************

//----------   CRC/Checksum User/Auto Mode Constant Definition   ----------
#define CRC_USER_M               (0x0uL << 7)
#define CRC_AUTO_M               (0x1uL << 7)

//----------   CRC/Checksum RLT Clear Constant Definition   ----------
#define CRC_RLTCLR               (0x1uL << 6)

//----------   CRC/Checksum Selection Constant Definition   ----------
#define CRC_CRC                  (0x0uL << 5)
#define CRC_CHECKSUM             (0x1uL << 5)

//----------   CRC Polynominal Constant Definition   ----------
#define CRC_CCITT                (0x0uL << 4)
#define CRC_16                   (0x1uL << 4)

//----------   CRC/Checksum Start Address Auto Increment Constant Definition   ----------
#define CRC_NOINC                (0x0uL << 3)
#define CRC_AUTOINC              (0x1uL << 3)

//----------   CRC/Checksum 1st Shifted-in Bit Constant Definition   ----------
#define CRC_MSB                  (0x0uL << 1)
#define CRC_LSB                  (0x1uL << 1)

//******************************************************************************
// Macro
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         CRC In Data
 * @param[in]     u32InData
 *                CRC/Checksum Input Data
 * @details       This macro puts input data for calculation
 *//*-------------------------------------------------------------------------*/
#define CRC_InData( u32InData )  (CRC->IN = u32InData)

/*-------------------------------------------------------------------------*//**
 * @brief         CRC/Checksum Run
 * @details       This macro starts CRC/Checksum calculation
 *//*-------------------------------------------------------------------------*/
#define CRCRun()                 { CRC->CR_b.CRCRUN = 1; NOP(); NOP(); NOP(); NOP(); }

/*-------------------------------------------------------------------------*//**
 * @brief         CRC/Checksum Stop
 * @details       This macro stops forcingly CRC/Checksum calculation
 *//*-------------------------------------------------------------------------*/
#define CRCStop()                (CRC->CR_b.CRCRUN = 0)

/*-------------------------------------------------------------------------*//**
 * @brief         CRC/Checksum Finish Check
 * @details       This macro checks CRC/Checksum finish
 *//*-------------------------------------------------------------------------*/
#define ChkCRCFinish()           (CRC->CR_b.CRCRUN)

//******************************************************************************
// Function
//******************************************************************************

void CRC_SetAddr( uint32_t u32SAdr, uint32_t u32EAdr, uint32_t u32IniD );
uint32_t CRC_AutoGet( uint32_t u32SEL, uint32_t u32POLY, uint32_t u32FirstBit );
void CRC_UserSet( uint32_t u32SEL, uint32_t u32POLY, uint32_t u32AdrInc, uint32_t u32FirstBit );
uint32_t CRC_UserInput( uint32_t u32Input );

#ifdef __cplusplus
}
#endif

#endif   /* _CRC_H_ */

