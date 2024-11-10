/***************************************************************************//**
 * @file          A31G12x_INTC.c
 * @brief         Driver Source File for Interrupt Controller
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
 * @brief         Configure External Interrupt Trigger
 * @param[in]     u32Px
 *                   Port Number
 *                   -  PORTB ~ PORTC, PORTE
 * @param[in]     u32pin
 *                   Pin Number
 *                   -  0 ~ 11
 * @param[in]     u32Trig
 *                   Trigger Mode
 *                   -  ITRIGx_Edge, ITRIGx_Level
 * @param[in]     u32Con
 *                   Interrupt Mode
 *                   -  when Trigger Mode is ITRIGx_Edge
 *                      -  INTCTLx_Disable, INTCTLx_FallingEdge, INTCTLx_RisingEdge, INTCTLx_BothEdge
 *                   -  when Trigger Mode is ITRIGx_Level
 *                      -  INTCTLx_Disable, INTCTLx_LowLevel, INTCTLx_HighLevel
 * @return        none
 * @remark        Available EINT Pin: PB[11:0], PC[3:0], PE[3:0]
 * @code
 *                [Example]
 *
 *                // configure PB0 as a Falling Edge Trigger
 *                EIntPx_SetReg( PORTB, 0, ITRIGx_Edge, INTCTLx_FallingEdge );
 *
 *                // configure PB1 as a Falling Edge Trigger
 *                EIntPx_SetReg( PORTB, 1, ITRIGx_Edge, INTCTLx_RisingEdge );
 *
 *                // configure PB2 as a Falling Edge Trigger
 *                EIntPx_SetReg( PORTB, 2, ITRIGx_Edge, INTCTLx_BothEdge );
 *
 *                // configure PB3 as a Falling Edge Trigger
 *                EIntPx_SetReg( PORTB, 3, ITRIGx_Level, INTCTLx_LowLevel );
 * @endcode
 *//*-------------------------------------------------------------------------*/
void EIntPx_SetReg( uint32_t u32Px, uint32_t u32pin, uint32_t u32Trig, uint32_t u32Con )
{
   uint32_t    temp_reg;

#ifdef PORTA
   if( u32Px == PORTA )
   {
      temp_reg = INTC->PATRIG;
      temp_reg &= ~( 1 << u32pin );
      temp_reg |= ( u32Trig << u32pin );
      INTC->PATRIG = temp_reg;

      temp_reg = INTC->PACR;
      temp_reg &= ~( 3 << ( u32pin << 1 ) );
      temp_reg |= ( u32Con << ( u32pin << 1 ) );
      INTC->PACR = temp_reg;
   }
#endif

#ifdef PORTB
   if( u32Px == PORTB )
   {
      temp_reg = INTC->PBTRIG;
      temp_reg &= ~( 1 << u32pin );
      temp_reg |= ( u32Trig << u32pin );
      INTC->PBTRIG = temp_reg;

      temp_reg = INTC->PBCR;
      temp_reg &= ~( 3 << ( u32pin << 1 ) );
      temp_reg |= ( u32Con << ( u32pin << 1 ) );
      INTC->PBCR = temp_reg;
   }
#endif

#ifdef PORTC
   if( u32Px == PORTC )
   {
      temp_reg = INTC->PCTRIG;
      temp_reg &= ~( 1 << u32pin );
      temp_reg |= ( u32Trig << u32pin );
      INTC->PCTRIG = temp_reg;

      temp_reg = INTC->PCCR;
      temp_reg &= ~( 3 << ( u32pin << 1 ) );
      temp_reg |= ( u32Con << ( u32pin << 1 ) );
      INTC->PCCR = temp_reg;
   }
#endif

#ifdef PORTD
   if( u32Px == PORTD )
   {
      temp_reg = INTC->PDTRIG;
      temp_reg &= ~( 1 << u32pin );
      temp_reg |= ( u32Trig << u32pin );
      INTC->PDTRIG = temp_reg;

      temp_reg = INTC->PDCR;
      temp_reg &= ~( 3 << ( u32pin << 1 ) );
      temp_reg |= ( u32Con << ( u32pin << 1 ) );
      INTC->PDCR = temp_reg;
   }
#endif

#ifdef PORTE
   if( u32Px == PORTE )
   {
      temp_reg = INTC->PETRIG;
      temp_reg &= ~( 1 << u32pin );
      temp_reg |= ( u32Trig << u32pin );
      INTC->PETRIG = temp_reg;

      temp_reg = INTC->PECR;
      temp_reg &= ~( 3 << ( u32pin << 1 ) );
      temp_reg |= ( u32Con << ( u32pin << 1 ) );
      INTC->PECR = temp_reg;
   }
#endif

#ifdef PORTF
   if( u32Px == PORTF )
   {
      temp_reg = INTC->PFTRIG;
      temp_reg &= ~( 1 << u32pin );
      temp_reg |= ( u32Trig << u32pin );
      INTC->PFTRIG = temp_reg;

      temp_reg = INTC->PFCR;
      temp_reg &= ~( 3 << ( u32pin << 1 ) );
      temp_reg |= ( u32Con << ( u32pin << 1 ) );
      INTC->PFCR = temp_reg;
   }
#endif
}

/*-------------------------------------------------------------------------*//**
 * @brief         Configure External Interrupt Group
 * @param[in]     u32TarIntNum
 *                   External Interrupt Number
 *                   -  EINT0 ~ EINT3
 * @param[in]     u32SrcPort
 *                   Port Number
 *                   -  CONFx_PB ~ CONFx_PC, CONFx_PE
 * @param[in]     u32SrcPin
 *                   Pin Number
 *                   -  0 ~ 11
 * @return        none
 * @details       This function configures the external interrupt group 0 to 3
 *                   -  If EINT0CONF1 == 0x01214211, The group 0 interrupts are [None:PB6:None:PB4:PE3:PC2:PB1:PB0]
 * @remark        Available EINT Pin: PB[11:0], PC[3:0], PE[3:0]
 *//*-------------------------------------------------------------------------*/
void EIntCfg( uint32_t u32TarIntNum, uint32_t u32SrcPort, uint32_t u32SrcPin )
{
   uint32_t    temp_reg;

   if( u32SrcPin < 8 )
   {
      if( u32TarIntNum == EINT0 )
      {
         temp_reg = INTC->EINT0CONF1;
         temp_reg &= ~( 0x0F << ( u32SrcPin * 4 ) );
         temp_reg |= ( u32SrcPort << ( u32SrcPin * 4 ) );
         INTC->EINT0CONF1 = temp_reg;
      }
      else if( u32TarIntNum == EINT1 )
      {
         temp_reg = INTC->EINT1CONF1;
         temp_reg &= ~( 0x0F << ( u32SrcPin * 4 ) );
         temp_reg |= ( u32SrcPort << ( u32SrcPin * 4 ) );
         INTC->EINT1CONF1 = temp_reg;
      }
      else if( u32TarIntNum == EINT2 )
      {
         temp_reg = INTC->EINT2CONF1;
         temp_reg &= ~( 0x0F << ( u32SrcPin * 4 ) );
         temp_reg |= ( u32SrcPort << ( u32SrcPin * 4 ) );
         INTC->EINT2CONF1 = temp_reg;
      }
      else if( u32TarIntNum == EINT3 )
      {
         temp_reg = INTC->EINT3CONF1;
         temp_reg &= ~( 0x0F << ( u32SrcPin * 4 ) );
         temp_reg |= ( u32SrcPort << ( u32SrcPin * 4 ) );
         INTC->EINT3CONF1 = temp_reg;
      }
   }
#if 1
   else
   {
      u32SrcPin -= 8;
      if( u32TarIntNum == EINT0 )
      {
         temp_reg = INTC->EINT0CONF2;
         temp_reg &= ~( 0x0F << ( u32SrcPin * 4 ) );
         temp_reg |= ( u32SrcPort << ( u32SrcPin * 4 ) );
         INTC->EINT0CONF2 = temp_reg;
      }
      else if( u32TarIntNum == EINT1 )
      {
         temp_reg = INTC->EINT1CONF2;
         temp_reg &= ~( 0x0F << ( u32SrcPin * 4 ) );
         temp_reg |= ( u32SrcPort << ( u32SrcPin * 4 ) );
         INTC->EINT1CONF2 = temp_reg;
      }
      else if( u32TarIntNum == EINT2 )
      {
         temp_reg = INTC->EINT2CONF2;
         temp_reg &= ~( 0x0F << ( u32SrcPin * 4 ) );
         temp_reg |= ( u32SrcPort << ( u32SrcPin * 4 ) );
         INTC->EINT2CONF2 = temp_reg;
      }
      else if( u32TarIntNum == EINT3 )
      {
         temp_reg = INTC->EINT3CONF2;
         temp_reg &= ~( 0x0F << ( u32SrcPin * 4 ) );
         temp_reg |= ( u32SrcPort << ( u32SrcPin * 4 ) );
         INTC->EINT3CONF2 = temp_reg;
      }
   }
#endif
}

#ifdef PORTA
/*-------------------------------------------------------------------------*//**
 * @brief         Get PA Interrupt Flag
 * @return        Pn Interrput Flag
 * @remark        Available EINT Pin: PB[11:0], PC[3:0], PE[3:0]
 *//*-------------------------------------------------------------------------*/
uint32_t EIntPA_GetIntStatus( void )
{
   return INTC->PAFLAG;
}
#endif

#ifdef PORTB
/*-------------------------------------------------------------------------*//**
 * @brief         Get PB Interrupt Flag
 * @return        Pn Interrput Flag
 * @remark        Available EINT Pin: PB[11:0], PC[3:0], PE[3:0]
 *//*-------------------------------------------------------------------------*/
uint32_t EIntPB_GetIntStatus( void )
{
   return INTC->PBFLAG;
}
#endif

#ifdef PORTC
/*-------------------------------------------------------------------------*//**
 * @brief         Get PC Interrupt Flag
 * @return        Pn Interrput Flag
 * @remark        Available EINT Pin: PB[11:0], PC[3:0], PE[3:0]
 *//*-------------------------------------------------------------------------*/
uint32_t EIntPC_GetIntStatus( void )
{
   return INTC->PCFLAG;
}
#endif

#ifdef PORTD
/*-------------------------------------------------------------------------*//**
 * @brief         Get PD Interrupt Flag
 * @return        Pn Interrput Flag
 * @remark        Available EINT Pin: PB[11:0], PC[3:0], PE[3:0]
 *//*-------------------------------------------------------------------------*/
uint32_t EIntPD_GetIntStatus( void )
{
   return INTC->PDFLAG;
}
#endif

#ifdef PORTE
/*-------------------------------------------------------------------------*//**
 * @brief         Get PE Interrupt Flag
 * @return        Pn Interrput Flag
 * @remark        Available EINT Pin: PB[11:0], PC[3:0], PE[3:0]
 *//*-------------------------------------------------------------------------*/
uint32_t EIntPE_GetIntStatus( void )
{
   return INTC->PEFLAG;
}

#endif

#ifdef PORTF
/*-------------------------------------------------------------------------*//**
 * @brief         Get PF Interrupt Flag
 * @return        Pn Interrput Flag
 * @remark        Available EINT Pin: PB[11:0], PC[3:0], PE[3:0]
 *//*-------------------------------------------------------------------------*/
uint32_t EIntPF_GetIntStatus( void )
{
   return INTC->PFFLAG;
}
#endif

#ifdef PORTA
/*-------------------------------------------------------------------------*//**
 * @brief         Clear PA Interrupt Flag
 * @param[in]     u32Value
 *                   Pn Interrupt Flag Mask
 *                   -  PnFLAG_FLAG0 ~ PnFLAG_FLAG0
 * @return        none
 * @remark        Available EINT Pin: PB[11:0], PC[3:0], PE[3:0]
 *//*-------------------------------------------------------------------------*/
void EIntPA_ClearIntStatus( uint32_t u32Value )
{
   INTC->PAFLAG = u32Value;
}
#endif

#ifdef PORTB
/*-------------------------------------------------------------------------*//**
 * @brief         Clear PB Interrupt Flag
 * @param[in]     u32Value
 *                   Pn Interrupt Flag Mask
 *                   -  PnFLAG_FLAG0 ~ PnFLAG_FLAG11
 * @return        none
 * @remark        Available EINT Pin: PB[11:0], PC[3:0], PE[3:0]
 *//*-------------------------------------------------------------------------*/
void EIntPB_ClearIntStatus( uint32_t u32Value )
{
   INTC->PBFLAG = u32Value;
}
#endif

#ifdef PORTC
/*-------------------------------------------------------------------------*//**
 * @brief         Clear PC Interrupt Flag
 * @param[in]     u32Value
 *                   Pn Interrupt Flag Mask
 *                   -  PnFLAG_FLAG0 ~ PnFLAG_FLAG3
 * @return        none
 * @remark        Available EINT Pin: PB[11:0], PC[3:0], PE[3:0]
 *//*-------------------------------------------------------------------------*/
void EIntPC_ClearIntStatus( uint32_t u32Value )
{
   INTC->PCFLAG = u32Value;
}
#endif

#ifdef PORTD
/*-------------------------------------------------------------------------*//**
 * @brief         Clear PD Interrupt Flag
 * @param[in]     u32Value
 *                   Pn Interrupt Flag Mask
 *                   -  PnFLAG_FLAG0 ~ PnFLAG_FLAG0
 * @return        none
 * @remark        Available EINT Pin: PB[11:0], PC[3:0], PE[3:0]
 *//*-------------------------------------------------------------------------*/
void EIntPD_ClearIntStatus( uint32_t u32Value )
{
   INTC->PDFLAG = u32Value;
}
#endif

#ifdef PORTE
/*-------------------------------------------------------------------------*//**
 * @brief         Clear PE Interrupt Flag
 * @param[in]     u32Value
 *                   Pn Interrupt Flag Mask
 *                   -  PnFLAG_FLAG0 ~ PnFLAG_FLAG3
 * @return        none
 * @remark        Available EINT Pin: PB[11:0], PC[3:0], PE[3:0]
 *//*-------------------------------------------------------------------------*/
void EIntPE_ClearIntStatus( uint32_t u32Value )
{
   INTC->PEFLAG = u32Value;
}
#endif

#ifdef PORTF
/*-------------------------------------------------------------------------*//**
 * @brief         Clear PF Interrupt Flag
 * @param[in]     u32Value
 *                   Pn Interrupt Flag Mask
 *                   -  PnFLAG_FLAG0 ~ PnFLAG_FLAG0
 * @return        none
 * @remark        Available EINT Pin: PB[11:0], PC[3:0], PE[3:0]
 *//*-------------------------------------------------------------------------*/
void EIntPF_ClearIntStatus( uint32_t u32Value )
{
   INTC->PFFLAG = u32Value;
}
#endif

/*-------------------------------------------------------------------------*//**
 * @brief         Enable Interrupt Source Mask
 * @param[in]     u32Src
 *                   Interrupt Source Mask
 *                   -  MSK_LVI | MSK_WUT | MSK_WDT | MSK_EINT0 | ...
 * @return        none
 * @code
 *                [Example]
 *
 *                // mask LVI, WUT, WDT interrupt
 *                EInt_MaskEnable( MSK_LVI | MSK_WUT | MSK_WDT );
 * @endcode
 *//*-------------------------------------------------------------------------*/
void EInt_MaskEnable( uint32_t u32Src )
{
   INTC->MSK &= ~u32Src;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Disable Interrupt Source Mask
 * @param[in]     u32Src
 *                   Interrupt Source Mask
 *                   -  MSK_LVI | MSK_WUT | MSK_WDT | MSK_EINT0 | ...
 * @return        none
 * @code
 *                [Example]
 *
 *                // unmask LVI, WUT, WDT interrupt
 *                EInt_MaskDisable( MSK_LVI | MSK_WUT | MSK_WDT );
 * @endcode
 *//*-------------------------------------------------------------------------*/
void EInt_MaskDisable( uint32_t u32Src )
{
   INTC->MSK |= u32Src;
}

