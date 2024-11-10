/***************************************************************************//**
 * @file          A31G12x_FMC.h
 * @brief         Driver Header File for Flash Memory Controller
 * @version       1.0
 * @date          2019-09-01
 * @author        ABOV Application Team
 *
 * Copyright (c) 2019, ABOV Semiconductor. All rights reserved.
 *//***************************************************************************/

#ifndef _FMC_H_
#define _FMC_H_

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Constant
//******************************************************************************

//----------   Flash Memory Control Constants Definition   ----------
#define FLASH_ID1                0x08192A3B
#define FLASH_ID2                0x4C5D6E7F
#define FLASH_IDXOR              0x9C752FC2
#define FLASH_ADDR_CD0           0x5FFFFFFF
#define FLASH_ADDR_CD1           0x5F9A30D7
#define FLASH_ADDR_CDXOR         0xA498DF92
#define FLASH_ADDR_INIT          0x5FFFFF80
#define FLASH_CLR_PAGEBUF    0x6C930001
#define FLASH_CHIPER_WOPT        0xC1BE0555
#define FLASH_BULK_CODE          0x6C93A408
#define FLASH_MEM_PGM_CODE       0x6C93A400
#define FLASH_OPT_PGM_CODE       0x6C933800

#define FLASH_START_ADDR         (0x10000000)
#define FLASH_END_ADDR           (0x1000FFFF)
#define CFG_OPT_SADDR            (0x1FFFF200)
#define CFG_OPT_EADDR            (0x1FFFF7FF)
#define SECTOR_SIZE_BYTE         (0x80uL)

//----------   Flash Erase/Write Code Constanst Definition   ----------
#define FLASH_BULK_ERASE         0x9AB1E0F8
#define FLASH_PAGE_ERASE         0x190CD5A2
#define FLASH_PAGE_WRITE         0x54760F54

//----------   Flash Procedure Good/Fail Constant Definition   ----------
#define FLASH_PGM_GOOD           0x0uL
#define FLASH_PGM_FAIL           0x9uL

//******************************************************************************
// Variable
//******************************************************************************

extern uint32_t flash_id1_reg;
extern uint32_t flash_id2_reg;
extern uint32_t flash_addr_code0;
extern uint32_t flash_addr_code1;

//******************************************************************************
// Function
//******************************************************************************

void FMC_FlashEntry( void );
void FMC_FlashExit( void );
uint32_t FMC_FlashFunc( uint32_t u32FncSel, uint32_t u32Addr, uint32_t* u32Buf );

uint32_t Flash_BulkEr( uint32_t u32UserId );
uint32_t Flash_PageEr( uint32_t u32UserId, uint32_t u32Addr );
uint32_t Flash_PageWt( uint32_t u32UserId, uint32_t u32Addr, uint32_t* u32Buf );

#ifdef __cplusplus
}
#endif

#endif   /* _FMC_H_ */

