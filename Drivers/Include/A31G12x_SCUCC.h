/***************************************************************************//**
 * @file          A31G12x_SCUCC.h
 * @brief         Driver Header File for SCU Chip Configuration
 * @version       1.0
 * @date          2019-09-01
 * @author        ABOV Application Team
 *
 * Copyright (c) 2019, ABOV Semiconductor. All rights reserved.
 *//***************************************************************************/

#ifndef _SCUCC_H_
#define _SCUCC_H_

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Constant
//******************************************************************************

// Boot Pin Function Selection Control
/*
#define B_INCLUDE_RSTB           2
#define B_POR_ONLY               3
*/
#define BFIND_PORorEXTR          SCUCC_BTPSCR_BFIND_PORorEXTR
#define BFIND_POR                SCUCC_BTPSCR_BFIND_POR

// HIRC Fine Trim One Step Change Control
#define HIRC_UP_ONESTEP          0
#define HIRC_DOWN_ONESTEP        1

// WDTRC Fine Trim One Step Change Control
#define WDTRC_UP_ONESTEP         0
#define WDTRC_DOWN_ONESTEP       1

//******************************************************************************
// Macro
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         Get "Vendor ID"/"Chip ID"/Revision Number"
 * @details       This macro gets vendor ID, chip ID, and revision number.
 *//*-------------------------------------------------------------------------*/
#define SCUCC_GetVendorID()         (SCUCC->VENDORID)
#define SCUCC_GetChipID()           (SCUCC->CHIPID)
#define SCUCC_GetRevNo()            (SCUCC->REVNR)

/*-------------------------------------------------------------------------*//**
 * @brief         Set Boot Pin Function
 * @param[in]     rst_src
 *                   Reset sources to check boot pin
 *                   -  BFIND_PORorEXTR, BFIND_POR
 * @details       This macro sets boot pin function to check when reset occurs.
 *//*-------------------------------------------------------------------------*/
#define SCUCC_SetBtFnc(rst_src)     (SCUCC->BTPSCR_b.BFIND = rst_src)

/*-------------------------------------------------------------------------*//**
 * @brief         Get Boot Pin Status
 * @details       This macro gets boot pin status.
 *//*-------------------------------------------------------------------------*/
#define SCUCC_GetBtPinSt()          (SCUCC->BTPSCR_b.BTPSTA)

/*-------------------------------------------------------------------------*//**
 * @brief         Enable/Disable NMI Interrupt
 * @details       This macro sets the NMI interrupt control bit.
 *//*-------------------------------------------------------------------------*/
#define SCUCC_EnNMI()               (SCUCC->NMISRCR_b.NMICON = 1)
#define SCUCC_DisNMI()              (SCUCC->NMISRCR_b.NMICON = 0)

/*-------------------------------------------------------------------------*//**
 * @brief         Generate Software Reset
 * @details       This macro generates software reset.
 *//*-------------------------------------------------------------------------*/
#define SCUCC_GenSwRst()            (SCUCC->SWRSTR = ((uint32_t)SCUCC_SWRSTR_WTIDKY_Value << SCUCC_SWRSTR_WTIDKY_Pos) | 0x2DuL)

/*-------------------------------------------------------------------------*//**
 * @brief         Enable/Disable Wake-Up Timer Interrupt
 * @details       This macro sets the wake-up timer interrupt enable bit.
 *//*-------------------------------------------------------------------------*/
#define SCUCC_EnWutInt()            (SCUCC->WUTCR_b.WUTIEN = 1)
#define SCUCC_DisWutInt()           (SCUCC->WUTCR_b.WUTIEN = 0)

/*-------------------------------------------------------------------------*//**
 * @brief         Get/Clear Wake-Up Timer Interrupt Flag
 * @details       This macro gets/clears the wake-up timer interrupt flag.
 *//*-------------------------------------------------------------------------*/
#define SCUCC_GetWutFlag()          (SCUCC->WUTCR_b.WUTIFLAG)
#define SCUCC_ClrWutFlag()          (SCUCC->WUTCR_b.WUTIFLAG = 1)

/*-------------------------------------------------------------------------*//**
 * @brief         Reload Counter of Wake-Up Timer
 * @details       This macro reloads wake-up timer counter to re-count.
 *//*-------------------------------------------------------------------------*/
#define SCUCC_ReloadWut()           (SCUCC->WUTCR_b.CNTRLD = 1)

//******************************************************************************
// Function
//******************************************************************************

uint32_t SCUCC_GetRstSrc( void );
void SCUCC_SetNmi( uint32_t u32NmiCon );
void SCUCC_ChkValidRst( void );
void SCUCC_SetWutData( uint32_t u32Data );
void SCUCC_ChgHircFreq( uint32_t u32Ind );
void SCUCC_ChgWdtrcFreq( uint32_t u32Ind );

#ifdef __cplusplus
}
#endif

#endif   /* _SCUCC_H_ */

