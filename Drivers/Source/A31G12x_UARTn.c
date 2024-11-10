/***************************************************************************//**
 * @file          A31G12x_UARTn.c
 * @brief         Driver Source File for UART
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

static uint32_t   UartBaseClock;

char       InData[80];
int        InFlag;
int        InCount;

//******************************************************************************
// Function
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         Determines best dividers to get a target clock rate
 * @param[in]     UARTx
 *                   Pointer to a target peripheral.
 *                   -  UART0 ~ UART1
 * @param[in]     baudrate
 *                   Desired UART baud rate.
 * @return        none
 *//*-------------------------------------------------------------------------*/
void uart_set_divisors( UARTn_Type* UARTx, uint32_t baudrate )
{
   uint32_t numerator;
   uint32_t denominator;
   uint32_t bdr, bfr;
   uint32_t fd;

   //--------------------------------------
   // numerator & denominator
   //
   // bdr = (UartBaseClock) / (16 * baudrate * 2)
   //--------------------------------------
   numerator = UartBaseClock;
   denominator = 16 * baudrate * 2;

   bdr = numerator / denominator;

   //--------------------------------------
   // fraction = numerator - bdr x denominator
   //
   // bfr = INT (fraction * 256) = ((f x d ) << 8) / d
   //--------------------------------------

   fd = numerator - ( bdr * denominator );
   bfr = ( fd * 256 ) / denominator;
   UARTx->BDR = ( uint16_t )( bdr & 0xffff );
   UARTx->BFR = ( uint8_t )( bfr & 0xff );
}

/*-------------------------------------------------------------------------*//**
 * @brief         Initialize the UARTn peripheral with the specified parameters.
 * @param[in]     UARTx
 *                   Pointer to a target peripheral.
 *                   -  UART0 ~ UART1
 * @param[in]     UARTn_Config
 *                   Pointer to a UARTn_CFG_Type structure
 *                   that contains the configuration information for the specified UART peripheral.
 * @return        none
 *//*-------------------------------------------------------------------------*/
void UARTn_Init( UARTn_Type* UARTx, UARTn_CFG_Type* UARTn_Config )
{
   uint8_t tmp;

   if( UARTx == ( UARTn_Type* )UART0 )
   {
      /* Set up peripheral clock for UART0 module */
      SCUCG_SetEachPeriClk2( PPCLKEN2_UT0CLKE, PPxCLKE_Enable );
   }

   if( UARTx == ( UARTn_Type* )UART1 )
   {
      /* Set up peripheral clock for UART1 module */
      SCUCG_SetEachPeriClk2( PPCLKEN2_UT1CLKE, PPxCLKE_Enable );
   }

   // Dummy reading
   while( UARTx->LSR & UARTn_LSR_RDR )
   {
      tmp = UARTx->RBR;
   }
   // Wait for current transmit complete
   while( !( UARTx->LSR & UARTn_LSR_THRE ) );

   // Disable interrupt
   UARTx->IER = 0;
   // Set LCR, DCR to default state
   UARTx->LCR = 0;
   UARTx->DCR = 0;
   // Dummy reading
   tmp = UARTx->LSR;
   tmp = UARTx->IIR;

   // uart clock set
   // SCU->MCCR7 &=0xffff0000;
   // SCU->MCCR7 |=((4<<8)|(2)); // MCLK
   UartBaseClock = SystemPeriClock;

   // Set Line Control register ----------------------------
   uart_set_divisors( UARTx, ( UARTn_Config->Baudrate ) );

   tmp = ( UARTx->LCR & UARTn_LCR_BREAK_EN ) & UARTn_LCR_BITMASK;

   switch( UARTn_Config->Databits )
   {
      case UARTn_DATA_BIT_5:
         tmp |= UARTn_LCR_WLEN5;
         break;
      case UARTn_DATA_BIT_6:
         tmp |= UARTn_LCR_WLEN6;
         break;
      case UARTn_DATA_BIT_7:
         tmp |= UARTn_LCR_WLEN7;
         break;
      case UARTn_DATA_BIT_8:
      default:
         tmp |= UARTn_LCR_WLEN8;
         break;
   }

   if( UARTn_Config->Parity == UARTn_PARITY_BIT_NONE )
   {
      // Do nothing...
   }
   else
   {
      tmp |= UARTn_LCR_PARITY_EN;
      switch( UARTn_Config->Parity )
      {
         case UARTn_PARITY_BIT_ODD:
            tmp |= UARTn_LCR_PARITY_ODD;
            break;

         case UARTn_PARITY_BIT_EVEN:
            tmp |= UARTn_LCR_PARITY_EVEN;
            break;

         case UARTn_PARITY_BIT_SP_1:
            tmp |= UARTn_LCR_PARITY_F_1;
            break;

         case UARTn_PARITY_BIT_SP_0:
            tmp |= UARTn_LCR_PARITY_F_0;
            break;
         default:
            break;
      }
   }

   switch( UARTn_Config->Stopbits )
   {
      case UARTn_STOP_BIT_2:
         tmp |= UARTn_LCR_STOPBIT_SEL;
         break;
      case UARTn_STOP_BIT_1:
      default:
         // Do no thing
         break;
   }

   UARTx->LCR = ( uint8_t )( tmp & UARTn_LCR_BITMASK );
}

/*-------------------------------------------------------------------------*//**
 * @brief         Deinitialize the UARTn peripheral registers
 *                to their default reset values.
 * @param[in]     UARTx
 *                   Pointer to a target peripheral.
 *                   -  UART0 ~ UART1
 * @return        none
 *//*-------------------------------------------------------------------------*/
void UARTn_DeInit( UARTn_Type* UARTx )
{
   if( UARTx == ( UARTn_Type* )UART0 )
   {
      /* Set up peripheral clock for UART0 module */
      SCUCG_RstEachPeri2( 1 << 2 );
      SCUCG_SetEachPeriClk2( PPCLKEN2_UT0CLKE, PPxCLKE_Disable );
   }

   if( UARTx == ( UARTn_Type* )UART1 )
   {
      /* Set up peripheral clock for UART1 module */
      SCUCG_RstEachPeri2( 1 << 3 );
      SCUCG_SetEachPeriClk2( PPCLKEN2_UT1CLKE, PPxCLKE_Disable );
   }
}

/*-------------------------------------------------------------------------*//**
 * @brief         Fills each UARTn_Config member with its default value:
 *                   -  38400 bps
 *                   -  8 Data Bit
 *                   -  No Parity Bit
 *                   -  1 Stop Bit
 * @param[out]    UARTn_Config
 *                   Pointer to a UARTn_CFG_Type structure which will be initialized.
 * @return        none
 *//*-------------------------------------------------------------------------*/
void UARTn_ConfigStructInit( UARTn_CFG_Type* UARTn_Config )
{
   UARTn_Config->Baudrate = 38400;
   UARTn_Config->Databits = UARTn_DATA_BIT_8;
   UARTn_Config->Parity = UARTn_PARITY_BIT_NONE;
   UARTn_Config->Stopbits = UARTn_STOP_BIT_1;
}

/* UART Send/Recieve functions -------------------------------------------------*/
/*-------------------------------------------------------------------------*//**
 * @brief         Transmit a single data through UART peripheral
 * @param[in]     UARTx
 *                   Pointer to a target peripheral.
 *                   -  UART0 ~ UART1
 * @param[in]     Data
 *                   Data to transmit (must be 8-bit long)
 * @return        none
 *//*-------------------------------------------------------------------------*/
void UARTn_SendByte( UARTn_Type* UARTx, uint8_t Data )
{
   UARTx->THR = Data;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Receive a single data from UART peripheral
 * @param[in]     UARTx
 *                   Pointer to a target peripheral.
 *                   -  UART0 ~ UART1
 * @return        Data received
 *//*-------------------------------------------------------------------------*/
uint8_t UARTn_ReceiveByte( UARTn_Type* UARTx )
{
   return UARTx->RBR;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Send a block of data via UART peripheral
 * @param[in]     UARTx
 *                   Pointer to a target peripheral.
 *                   -  UART0 ~ UART1
 * @param[in]     txbuf
 *                   Pointer to Transmit buffer
 * @param[in]     buflen
 *                   Length of Transmit buffer
 * @param[in]     flag
 *                   Flag used in  UART transfer
 *                   -  NONE_BLOCKING
 *                   -  BLOCKING
 * @return        Number of bytes sent.
 * @note          when using UART in BLOCKING mode,
 *                a time-out condition is used via defined symbol UARTn_BLOCKING_TIMEOUT.
 *//*-------------------------------------------------------------------------*/
uint32_t UARTn_Send( UARTn_Type* UARTx, uint8_t* txbuf, uint32_t buflen, TRANSFER_BLOCK_Type flag )
{
   uint32_t bToSend, bSent, timeOut;
   uint8_t* pChar = txbuf;

   bToSend = buflen;

   // blocking mode
   if( flag == BLOCKING )
   {
      bSent = 0;
      while( bToSend )
      {
         timeOut = UARTn_BLOCKING_TIMEOUT;
         // Wait for THR empty with timeout
         while( !( UARTx->LSR & UARTn_LSR_THRE ) )
         {
            if( timeOut == 0 )
            {
               break;
            }
            timeOut--;
         }
         // Time out!
         if( timeOut == 0 )
         {
            break;
         }
         UARTn_SendByte( UARTx, ( *pChar++ ) );
         bToSend--;
         bSent++;
      }
   }
   // None blocking mode
   else
   {
      bSent = 0;
      while( bToSend )
      {
         if( !( UARTx->LSR & UARTn_LSR_THRE ) )
         {
            //continue;
            break;
         }
         UARTn_SendByte( UARTx, ( *pChar++ ) );
         bToSend--;
         bSent++;
      }
   }
   return bSent;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Receive a block of data via UART peripheral
 * @param[in]     UARTx
 *                   Pointer to a target peripheral.
 *                   -  UART0 ~ UART1
 * @param[out]    rxbuf
 *                   Pointer to Received buffer
 * @param[in]     buflen
 *                   Length of Received buffer
 * @param[in]     flag
 *                   Flag mode
 *                   -  NONE_BLOCKING
 *                   -  BLOCKING
 * @return        Number of bytes received
 * @note          when using UART in BLOCKING mode,
 *                a time-out condition is used via defined symbol UARTn_BLOCKING_TIMEOUT.
 *//*-------------------------------------------------------------------------*/
uint32_t UARTn_Receive( UARTn_Type* UARTx, uint8_t* rxbuf, uint32_t buflen, TRANSFER_BLOCK_Type flag )
{
   uint32_t bToRecv, bRecv, timeOut;
   uint8_t* pChar = rxbuf;

   bToRecv = buflen;

   // Blocking mode
   if( flag == BLOCKING )
   {
      bRecv = 0;
      while( bToRecv )
      {
         timeOut = UARTn_BLOCKING_TIMEOUT;
         while( !( UARTx->LSR & UARTn_LSR_RDR ) )
         {
            if( timeOut == 0 )
            {
               break;
            }
            timeOut--;
         }
         // Time out!
         if( timeOut == 0 )
         {
            break;
         }
         // Get data from the buffer
         ( *pChar++ ) = UARTn_ReceiveByte( UARTx );
         bToRecv--;
         bRecv++;
      }
   }
   // None blocking mode
   else
   {
      bRecv = 0;
      while( bToRecv )
      {
         if( !( UARTx->LSR & UARTn_LSR_RDR ) )
         {
            break;
         }
         else
         {
            ( *pChar++ ) = UARTn_ReceiveByte( UARTx );
            bRecv++;
            bToRecv--;
         }
      }
   }
   return bRecv;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Force BREAK character on UART line, output pin UARTn TXD is
 *                   forced to logic 0.
 * @param[in]     UARTx
 *                   Pointer to a target peripheral.
 *                   -  UART0 ~ UART1
 * @return        none
 *//*-------------------------------------------------------------------------*/
void UARTn_ForceBreak( UARTn_Type* UARTx )
{
   UARTx->LCR |= UARTn_LCR_BREAK_EN;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Configure the peripheral interrupt.
 * @param[in]     UARTx
 *                   Pointer to a target peripheral.
 *                   -  UART0 ~ UART1
 * @param[in]     UARTIntCfg
 *                   Specifies the interrupt flag
 *                   -  UARTn_INTCFG_RBR:    RBR Interrupt enable
 *                   -  UARTn_INTCFG_THRE:   THR Interrupt enable
 *                   -  UARTn_INTCFG_RLS:    RX line status interrupt enable
 *                   -  UARTn_INTCFG_DRX:    THR Interrupt enable
 *                   -  UARTn_INTCFG_DTX:    RX line status interrupt enable
 * @param[in]     NewState
 *                   Next State of Interrupt Operation
 *                   -  ENABLE, DISABLE
 * @return        none
 *//*-------------------------------------------------------------------------*/
void UARTn_IntConfig( UARTn_Type* UARTx, UARTn_INT_Type UARTIntCfg, FunctionalState NewState )
{
   uint32_t tmp;

   switch( UARTIntCfg )
   {
      case UARTn_INTCFG_RBR:
         tmp = UARTn_IER_RBRINT_EN;
         break;
      case UARTn_INTCFG_THRE:
         tmp = UARTn_IER_THREINT_EN;
         break;
      case UARTn_INTCFG_RLS:
         tmp = UARTn_IER_RLSINT_EN;
         break;
      case UARTn_INTCFG_TXE:
         tmp = UARTn_IER_TXE_EN;
         break;
   }

   if( NewState == ENABLE )
   {
      UARTx->IER |= tmp;
   }
   else
   {
      UARTx->IER &= ( ~tmp ) & UARTn_IER_BITMASK;
   }
}


/*-------------------------------------------------------------------------*//**
 * @brief         Get current value of Line Status register in UART peripheral.
 * @param[in]     UARTx
 *                   Pointer to a target peripheral.
 *                   -  UART0 ~ UART1
 * @return        Current value of Line Status register in UART peripheral.
 * @note          The return value of this function must be ANDed with each member in
 *                UART_LS_Type enumeration to determine current flag status
 *                corresponding to each Line status type. Because some flags in
 *                Line Status register will be cleared after reading, the next reading
 *                Line Status register could not be correct. So this function used to
 *                read Line status register in one time only, then the return value
 *                used to check all flags.
 *//*-------------------------------------------------------------------------*/
uint8_t UARTn_GetLineStatus( UARTn_Type* UARTx )
{
   return ( ( UARTx->LSR ) & UARTn_LSR_BITMASK );
}

/*-------------------------------------------------------------------------*//**
 * @brief         Check whether if UART is busy or not
 * @param[in]     UARTx
 *                   Pointer to a target peripheral.
 *                   -  UART0 ~ UART1
 * @return        RESET if UART is not busy, otherwise return SET.
 *//*-------------------------------------------------------------------------*/
FlagStatus UARTn_CheckBusy( UARTn_Type* UARTx )
{
   if( UARTx->LSR & UARTn_LSR_TEMT )
   {
      return RESET;
   }
   else
   {
      return SET;
   }
}

/*-------------------------------------------------------------------------*//**
 * @brief         Configure Data Control mode for UART peripheral
 * @param[in]     UARTx
 *                   Pointer to a target peripheral.
 * @param[in]     Mode
 *                   Data Control Mode
 *                   -  UARTn_DATA_CONTROL_LOOPBACK:  Loop back mode.
 *                   -  UARTn_DATA_CONTROL_RXINV:     RX Data inversion mode.
 *                   -  UARTn_DATA_CONTROL_TXINV:     TX Data inversion mode.
 *                   -  UARTn_DATA_CONTROL_RTXINV:    TX RX Data inversion mode.
 * @param[in]     NewState
 *                   Next State of Functional Operation
 *                   -  ENABLE, DISABLE
 * @return        none
 *//*-------------------------------------------------------------------------*/
void UARTn_DataControlConfig( UARTn_Type* UARTx, UARTn_DATA_CONTROL_Type Mode, FunctionalState NewState )
{
   uint8_t tmp;

   switch( Mode )
   {
      case UARTn_DATA_CONTROL_LOOPBACK:
         tmp = UARTn_DCR_LBON;
         break;
      case UARTn_DATA_CONTROL_RXINV:
         tmp = UARTn_DCR_RXINV;
         break;
      case UARTn_DATA_CONTROL_TXINV:
         tmp = UARTn_DCR_TXINV;
         break;
      case UARTn_DATA_CONTROL_RTXINV:
         tmp = UARTn_DCR_RXINV | UARTn_DCR_TXINV;
         break;
      default:
         break;
   }

   if( NewState == ENABLE )
   {
      UARTx->DCR |= tmp;
   }
   else
   {
      UARTx->DCR &= ( ~tmp ) & UARTn_DCR_BITMASK;
   }
}

/*-------------------------------------------------------------------------*//**
 * @brief         Configure inter-frame delay time for UART peripheral
 * @param[in]     UARTx
 *                   Pointer to a target peripheral.
 * @param[in]     waitval
 *                   inter-frame delay time: 1 bit time  unit (0~7)
 * @return        none
 *//*-------------------------------------------------------------------------*/
void UARTn_IFDelayConfig( UARTn_Type* UARTx, uint8_t waitval )
{
   if( waitval < 8 )
   {
      UARTx->IDTR = waitval;
   }
}

