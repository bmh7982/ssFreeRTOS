/***************************************************************************//**
 * @file          A31G12x_WTIDKY.h
 * @brief         Driver Header File for Write Identification Key
 * @version       1.0
 * @date          2019-09-01
 * @author        ABOV Application Team
 *
 * @deprecated    This file is deprecated. Use the WTIDKY Value defined in A31G12x.h instead.
 *
 * Copyright (c) 2019, ABOV Semiconductor. All rights reserved.
 *//***************************************************************************/

#ifndef _WTIDKY_H_
#define _WTIDKY_H_

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Constant
//******************************************************************************

// Write ID Key Definition
#define SCU_SCCRWTIDKY     0x570AuL       // 0     SCUCG_SCCR_WTIDKY_Value
#define SCU_CLKSRCRWTIDKY  0xA507uL       // 1     SCUCG_CLKSRCR_WTIDKY_Value
#define XTFWTIDKY          0x9B37uL       // 2     SCUCG_XTFLSR_WTIDKY_Value
#define HIRCWTIDKY         0xA6B5uL       // 3     SCUCC_HIRCTRM_WTIDKY_Value
#define WDTRCWTIDKY        0x4C3DuL       // 4     SCUCC_WDTRCTRM_WTIDKY_Value
#define REMWTIDKY          0xE2F1uL       // 5     SCUCC_PMREMAP_WTIDKY_Value
#define WDTWTIDKY          0x5A69uL       // 6     WDT_CR_WTIDKY_Value
#define T30OUTWTIDKY       0xE06CuL       // 7     TIMER30_T30_OUTCR_WTIDKY_Value
#define FMC_BCRWTIDKY      0xC1BEuL       // 8     FMC_BCR_WTIDKY_Value
#define FMC_CRWTIDKY       0x6C93uL       // 9     FMC_CR_WTIDKY_Value
#define SWRSTWTIDKY        0x9EB3uL       // 10    SCUCC_SWRSTR_WTIDKY_Value
#define VECTWTIDKY         0x05FAuL       // 11

#ifdef __cplusplus
}
#endif

#endif   /* _WTIDKY_H_ */

