/***************************************************************************//**
 * @file          A31G12x_FMC.c
 * @brief         Driver Source File for Flash Memory Controller
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
// Variable
//******************************************************************************

uint32_t flash_id1_reg;
uint32_t flash_id2_reg;
uint32_t flash_addr_code0;
uint32_t flash_addr_code1;

//******************************************************************************
// Function
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         Entry of Flash Memory Control
 * @return        none
 * @details       This function sets flash IDs for erase/write
 *             The HIRC should be enabled before flash functions. So, Do enable the HIRC if disabled
 *             The LVR should be enabled and set over 2.28V level before flash functions. So, Do enable the LVR and Set 2.28V level over if disabled
 *             The global interrupt should be diabled by s/w. So, Diable global interrupt
 *//*-------------------------------------------------------------------------*/
void FMC_FlashEntry( void )
{
   uint32_t WDT_WINDR;
   flash_id1_reg = FLASH_ID1 ^ FLASH_IDXOR;
   flash_id2_reg = FLASH_ID2 ^ FLASH_IDXOR;
   flash_addr_code0 = FLASH_ADDR_CD0 ^ FLASH_ADDR_CDXOR;
   SCUCG->PPCLKEN2_b.FMCLKE = 1;       // Enable Flash Memory Control Clock
#if 0
   SCUCG->CLKSRCR_b.HIRCEN = 1;        // Enable HIRC
   NOP();
   NOP();
   NOP();

   SCULV->LVRCR_b.LVREN = 0x00;        // Enable LVR, the Level should be set 2.28V over in CONF_LVRCNFIG register of configure option page 1
   SCUCC->NMISRCR_b.NMICON = 0;        // Disable NMI
#endif
   WDT_WINDR = WDT->WINDR;
   if( WDT->CNT < WDT_WINDR )
   {
      WDT->CNTR_b.CNTR = 0x6a;         // Reload WDT Counter if WDT->CNT < WDT_WINDR
   }
   DI();                               // Disable global interrupt
}

/*-------------------------------------------------------------------------*//**
 * @brief         Exit of Flash Memory Control
 * @return        none
 * @details       This function clears flash IDs for erase/write
 *//*-------------------------------------------------------------------------*/
void FMC_FlashExit( void )
{
   flash_id1_reg = 0;
   flash_id2_reg = 0;
   flash_addr_code0 = 0;
   flash_addr_code1 = 0;
   SCUCG->PPCLKEN2_b.FMCLKE = 0;       // Disable Flash Memory Control Clock
   EI();                               // Enable global interrupt
}

/*-------------------------------------------------------------------------*//**
 * @brief         Flash Page Erase/Write and Bulk(Chip) Erase
 * @param[in]     u32FncSel
 *                Select Flash Function
 *                   -  FLASH_BULK_ERASE
 *                   -  FLASH_PAGE_ERASE
 *                   -  FLASH_PAGE_WRITE
 * @param[in]     u32Addr
 *                Address of erase/write
 *                   -  Flash Memory:  0x00000000 ~ 0x0000FFFF
 *                   -  Configuration Option Area Page 1 ~ 3
 * @param[in]     u32Buf
 *                Write Data
 * @return        none
 * @details       This function erases/writes flash memory and configure option area 1 to 3
 *//*-------------------------------------------------------------------------*/
uint32_t FMC_FlashFunc( uint32_t u32FncSel, uint32_t u32Addr, uint32_t* u32Buf )
{
   uint32_t             i;
   volatile uint32_t*   pagebuffer;

   pagebuffer = &FMC->PAGEBUF;
   FMC->ADR = flash_addr_code0 ^ FLASH_ADDR_CDXOR;    // Write 0x5FFFFFFF to FMC_ADR during the register is equal to 0x5FFFFF80;
   FMC->IDR1 = flash_id1_reg ^ FLASH_IDXOR;           // Identification Value 0
   FMC->IDR2 = flash_id2_reg ^ FLASH_IDXOR;           // Identification Value 1

   FMC->CR = FLASH_CLR_PAGEBUF;                       // Clear page buffer
   for( i = 0 ; i < SECTOR_SIZE_BYTE / 4 ; i++ )
   {
      if( u32FncSel == FLASH_PAGE_WRITE )
      {
         *pagebuffer++ = *u32Buf++;                   // To page write
      }
      else if( ( u32FncSel == FLASH_PAGE_ERASE ) || ( u32FncSel == FLASH_BULK_ERASE ) )
      {
         *pagebuffer++ = 0xFFFFFFFF;                  // To page or bulk erase
      }
      else
      {
         return FLASH_PGM_FAIL;
      }
   }

   if( ( u32FncSel == FLASH_PAGE_WRITE ) || ( u32FncSel == FLASH_PAGE_ERASE ) )  // Page Erase/Write
   {
      if( u32Addr < FLASH_START_ADDR )
      {
         FMC->ADR = u32Addr + FLASH_START_ADDR;          // Flash Page Address to be erased or written
      }
      else
      {
         FMC->ADR = u32Addr;
      }
   }
   else if( u32FncSel == FLASH_BULK_ERASE )              // Bulk(Chip) Erase
   {
      FMC->BCR = FLASH_CHIPER_WOPT;                      // For bulk erase including Configure Option Page 1/2/3
      FMC->ADR = flash_addr_code1 ^ FLASH_ADDR_CDXOR;    // Identification Address for bulk erase
   }
   else
   {
      return FLASH_PGM_FAIL;
   }

   if( FMC->IDR1 != FLASH_ID1 )
   {
      return FLASH_PGM_FAIL;        // Check whether ID0 is ok or not
   }
   if( FMC->IDR2 != FLASH_ID2 )
   {
      return FLASH_PGM_FAIL;        // Check whether ID1 is ok or not
   }

   if( ( u32FncSel == FLASH_PAGE_WRITE ) || ( u32FncSel == FLASH_PAGE_ERASE ) )     // Page Erase/Write
   {
      if( ( FMC->ADR >= FLASH_START_ADDR ) && ( FMC->ADR <= FLASH_END_ADDR ) )
      {
         FMC->CR = FLASH_MEM_PGM_CODE | ( u32FncSel & 0x0000000F );     // Start flash page erase/write from here
      }
      else if( ( FMC->ADR >= CFG_OPT_SADDR ) && ( FMC->ADR <= CFG_OPT_EADDR ) )
      {
         FMC->CR = FLASH_OPT_PGM_CODE | ( u32FncSel & 0x0000000F );     // Start configure page erase/write from here
      }
      else
      {
         return FLASH_PGM_FAIL;
      }
   }
   else if( u32FncSel == FLASH_BULK_ERASE )  // Bulk(Chip) Erase
   {
      if( FMC->ADR != FLASH_ADDR_CD1 )
      {
         return FLASH_PGM_FAIL;
      }
      FMC->CR = FLASH_BULK_CODE;             // Start bulk(chip) erase from here
   }
   else
   {
      return FLASH_PGM_FAIL;
   }

   while( FMC->CR_b.FMBUSY ) {}              // Check whether the busy bit. If time over, goes out with error.
   if( !FMC->ERFLAG_b.FMOPFLAG )
   {
      return FLASH_PGM_GOOD;     // Success
   }
   return FLASH_PGM_FAIL;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Flash Bulk(Chip) Erase Function
 * @param[in]     u32UserId
 *                Check value for real chip erase       User ID to check flash memory bulk erase
 * @return        Result of FMC
 *                   -  Success:    0
 *                   -  Fail:       9
 * @details       This function erases flash memory area with configure option area 1 to 3
 *//*-------------------------------------------------------------------------*/
uint32_t Flash_BulkEr( uint32_t u32UserId )
{
   uint32_t result;

   FMC_FlashEntry();
   flash_addr_code1 = FLASH_ADDR_CD1 ^ FLASH_ADDR_CDXOR;
   if( u32UserId == 0x90E832CF )                         // Ex) 0x90E832CF, The user ID may be changed by programmer
   {
      result = FMC_FlashFunc( FLASH_BULK_ERASE, 0, 0 );
   }
   else
   {
      result = FLASH_PGM_FAIL;
   }

   if( result )                  // If fail
   {
      FMC->ERFLAG = 0x03uL;      // Clear FMC related flag
   }
   FMC_FlashExit();
   return result;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Flash Page Erase Function
 * @param[in]     u32UserId
 *                Check value for real page erase       User ID to check flash memory page erase
 * @param[in]     u32Addr
 *                Target Address
 *                   -  Flash Memory:  0x00000000 ~ 0x0000FFFF
 *                   -  Configuration Option Area Page 1 ~ 3
 * @return        Result of FMC
 *                   -  Success:    0
 *                   -  Fail:       9
 * @details       This function erases flash memory area and configure option area
 *//*-------------------------------------------------------------------------*/
uint32_t Flash_PageEr( uint32_t u32UserId, uint32_t u32Addr )
{
   uint32_t result;

   FMC_FlashEntry();
   if( u32UserId == 0xA901358F )    // Ex) 0xA901358F, The user ID may be changed by programmer
   {
      result = FMC_FlashFunc( FLASH_PAGE_ERASE, u32Addr, 0 );
   }
   else
   {
      result = FLASH_PGM_FAIL;
   }

   if( result )                     // If fail
   {
      FMC->ERFLAG = 0x03uL;         // Clear FMC related flag
   }
   FMC_FlashExit();
   return result;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Flash Page Write Function
 * @param[in]     u32UserId
 *                Check value for real page write       User ID to check flash memory page write
 * @param[in]     u32Addr
 *                Target Address
 *                   -  Flash Memory:  0x00000000 ~ 0x0000FFFF
 *                   -  Configuration Option Area Page 1 ~ 3
 * @param[in]     u32Buf
 *                Write Data
 * @return        Result of FMC
 *                   -  Success:    0
 *                   -  Fail:       9
 * @details       This function writes flash memory area and configure option area
 *//*-------------------------------------------------------------------------*/
uint32_t Flash_PageWt( uint32_t u32UserId, uint32_t u32Addr, uint32_t* u32Buf )
{
   uint32_t result;

   FMC_FlashEntry();
   if( u32UserId == 0x4F17DC86 )       // Ex) 0x4F17DC86, The user ID may be changed by programmer
   {
      result = FMC_FlashFunc( FLASH_PAGE_WRITE, u32Addr, u32Buf );
   }
   else
   {
      result = FLASH_PGM_FAIL;
   }

   if( result )                        // If fail
   {
      FMC->ERFLAG = 0x03uL;            // Clear FMC related flag
   }
   FMC_FlashExit();
   return result;
}

