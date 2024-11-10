/***************************************************************************//**
 * @file          A31G12x_USART1n.c
 * @brief         Driver Source File for USART & SPI
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

uint32_t UsartBaseClock;

//******************************************************************************
// Function
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         Determines best dividers to get a target clock rate
 * @param[in]     USART1x
 *                   Pointer to a target peripheral.
 *                   -  USART10 ~ USART13
 * @param[in]     mode
 * @param[in]     baudrate
 *                   Desired baudrate.
 * @return        none
 *//*-------------------------------------------------------------------------*/
void usart_set_divisors( USART1n_Type* USART1x, uint32_t mode, uint32_t baudrate )
{
   uint32_t numerator;
   uint32_t denominator;
   uint32_t n;
   uint32_t bdr;

   if( mode == USART1n_UART_MODE )
   {
      //Baud Rate = PCLK/(16(USART1n_BDR+1))
      //USART1n_BDR = (PCLK/(16*Baud Rate))-1
      n = 16;
   }
   else
   {
      //Baud Rate = PCLK/(2(USART1n_BDR+1))
      //USART1n_BDR = (PCLK/(2*Baud Rate))-1
      n = 2;
   }


   //--------------------------------------
   // numerator & denominator
   //
   // bdr = (UsartBaseClock) / n / baudrate  - 1
   //--------------------------------------
   numerator = UsartBaseClock;
   denominator = baudrate;

   bdr = numerator / n / denominator - 1;

   USART1x->BDR = ( uint16_t )( bdr & 0xffff );
}

/*-------------------------------------------------------------------------*//**
 * @brief         Initialize the USART1n peripheral with the specified parameters.
 * @param[in]     USART1x
 *                   Pointer to a target peripheral.
 *                   -  USART10 ~ USART13
 * @param[in]     USART1n_Config
 *                   Pointer to a USART1n_CFG_Type structure
 *                   that contains the configuration information
 *                   for the specified peripheral.
 * @return        none
 *//*-------------------------------------------------------------------------*/
void USART1n_Init( USART1n_Type* USART1x, USART1n_CFG_Type* USART1n_Config )
{
   uint32_t tmp ;

   if( USART1x == ( USART1n_Type* )USART10 )
   {
      /* Set up peripheral clock for USART10 module */
      SCUCG_SetEachPeriClk2( PPCLKEN2_UST10CLKE, PPxCLKE_Enable );
      SCUCG_RstEachPeri2( 1 << 0 );
   }

   if( USART1x == ( USART1n_Type* )USART11 )
   {
      /* Set up peripheral clock for USART11 module */
      SCUCG_SetEachPeriClk2( PPCLKEN2_UST11CLKE, PPxCLKE_Enable );
      SCUCG_RstEachPeri2( 1 << 1 );
   }

   if( USART1x == ( USART1n_Type* )USART12 )
   {
      /* Set up peripheral clock for USART12 module */
      SCUCG_SetEachPeriClk2( PPCLKEN2_UST12CLKE, PPxCLKE_Enable );
      SCUCG_RstEachPeri2( 1 << 4 );
   }

   if( USART1x == ( USART1n_Type* )USART13 )
   {
      /* Set up peripheral clock for USART13 module */
      SCUCG_SetEachPeriClk2( PPCLKEN2_UST13CLKE, PPxCLKE_Enable );
      SCUCG_RstEachPeri2( 1 << 5 );
   }

   UsartBaseClock = SystemPeriClock;

   usart_set_divisors( USART1x, USART1n_Config->Mode, USART1n_Config->Baudrate );

   tmp = 0
         | ( ( USART1n_Config->Mode & 0x3 ) << USART1n_CR1_USTnMS_Pos )
         | ( ( USART1n_Config->Parity & 0x3 ) << USART1n_CR1_USTnP_Pos )
         | ( ( USART1n_Config->Databits & 0x7 ) << USART1n_CR1_USTnS_Pos )
         | ( ( USART1n_Config->Order & 0x1 ) << USART1n_CR1_ORDn_Pos )
         | ( ( USART1n_Config->ACK & 0x1 ) << USART1n_CR1_CPOLn_Pos )
         | ( ( USART1n_Config->Edge & 0x3 )  << USART1n_CR1_CPHAn_Pos )
         | ( 1 << USART1n_CR1_TXEn_Pos )  // Tx Enable
         | ( 1 << USART1n_CR1_RXEn_Pos )  // Rx Enable
         ;

   USART1x->CR1 = tmp;

   USART1x->CR2 &= ~( 1 << USART1n_CR2_USTnSB_Pos );     // USTnSB reset
   USART1x->CR2 |= ( ( USART1n_Config->Stopbits & 0x1 ) << USART1n_CR2_USTnSB_Pos );    // USTnSB
   USART1x->CR2 &= ~( 1 << USART1n_CR2_FXCHn_Pos );      // FXCHn reset
   // USART1x->CR2 |= (1<<USART1n_CR2_FXCHn_Pos);        // FXCHn

   USART1n_ClearStatus( USART1x, USART1n_STATUS_TXC );

   //Dummy Read
   tmp = USART1n_ReceiveByte( USART1x );
   tmp = USART1n_ReceiveByte( USART1x );
}

/*-------------------------------------------------------------------------*//**
 * @brief         Deinitialize the USART1n peripheral registers to their default reset values.
 * @param[in]     USART1x
 *                   Pointer to a target peripheral.
 *                   -  USART10 ~ USART13
 * @return        none
 *//*-------------------------------------------------------------------------*/
void USART1n_DeInit( USART1n_Type* USART1x )
{
   if( USART1x == ( USART1n_Type* )USART10 )
   {
      /* Set up peripheral clock for USART10 module */
      SCUCG_RstEachPeri2( 1 << 0 );
      SCUCG_SetEachPeriClk2( PPCLKEN2_UST10CLKE, PPxCLKE_Disable );
   }

   if( USART1x == ( USART1n_Type* )USART11 )
   {
      /* Set up peripheral clock for USART11 module */
      SCUCG_RstEachPeri2( 1 << 1 );
      SCUCG_SetEachPeriClk2( PPCLKEN2_UST11CLKE, PPxCLKE_Disable );
   }

   if( USART1x == ( USART1n_Type* )USART12 )
   {
      /* Set up peripheral clock for USART12 module */
      SCUCG_RstEachPeri2( 1 << 4 );
      SCUCG_SetEachPeriClk2( PPCLKEN2_UST12CLKE, PPxCLKE_Disable );
   }

   if( USART1x == ( USART1n_Type* )USART13 )
   {
      /* Set up peripheral clock for USART13 module */
      SCUCG_RstEachPeri2( 1 << 5 );
      SCUCG_SetEachPeriClk2( PPCLKEN2_UST13CLKE, PPxCLKE_Disable );
   }
}

/*-------------------------------------------------------------------------*//**
 * @brief         USART1n enable control
 * @param[in]     USART1x
 *                   Pointer to a target peripheral.
 *                   -  USART10 ~ USART13
 * @param[in]     state
 *                   ENABLE / DISABLE
 * @return        none
 *//*-------------------------------------------------------------------------*/
void USART1n_Enable( USART1n_Type* USART1x, FunctionalState state )
{
   if( state == ENABLE )
   {
      USART1x->CR2 |= ( 1 << USART1n_CR2_USTnEN_Pos );    // USTnEN
   }
   else
   {
      USART1x->CR2 &= ~( 1 << USART1n_CR2_USTnEN_Pos );   // USTnEN
   }
}

/*-------------------------------------------------------------------------*//**
 * @brief         Fills each USART_InitStruct member with its default value:
 *                   -  38400 bps
 *                   -  8 Data Bit
 *                   -  No Parity Bit
 *                   -  1 Stop Bit
 * @param[out]    USART1n_Config
 *                   Pointer to a USART1n_CFG_Type structure which will be initialized.
 * @return        none
 *//*-------------------------------------------------------------------------*/
void USART1n_UART_Mode_ConfigStructInit( USART1n_CFG_Type* USART1n_Config )
{
   USART1n_Config->Mode = USART1n_UART_MODE;
   USART1n_Config->Baudrate = 38400;
   USART1n_Config->Databits = USART1n_DATA_BIT_8;
   USART1n_Config->Parity = USART1n_PARITY_BIT_NONE;
   USART1n_Config->Stopbits = USART1n_STOP_BIT_1;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Fills each USART_InitStruct member with its default value:
 *                   -  38400 bps
 *                   -  8 Data Bit
 *                   -  No Parity Bit
 *                   -  1 Stop Bit
 * @param[out]    USART1n_Config
 *                   Pointer to a USART1n_CFG_Type structure which will be initialized.
 * @return        none
 *//*-------------------------------------------------------------------------*/
void USART1n_SPI_Mode_ConfigStructInit( USART1n_CFG_Type* USART1n_Config )
{
   USART1n_Config->Mode = USART1n_SPI_MODE;
   USART1n_Config->Baudrate = 38400;
   USART1n_Config->Databits = USART1n_DATA_BIT_8;
   USART1n_Config->Parity = USART1n_PARITY_BIT_NONE;
   USART1n_Config->Stopbits = USART1n_STOP_BIT_1;

   //only SPI & Sync. Mode
   USART1n_Config->Order = USART1n_SPI_LSB;
#if 1 // CPOLn : 0, CPHAn : 0 (X)
   USART1n_Config->ACK = USART1n_SPI_TX_RISING;
   USART1n_Config->Edge = USART1n_SPI_TX_LEADEDGE_SAMPLE;
#endif
#if 0 // CPOLn : 0, CPHAn : 1 (O)
   USART1n_Config->ACK = USART1n_SPI_TX_RISING;
   USART1n_Config->Edge = USART1n_SPI_TX_LEADEDGE_SETUP;
#endif

#if 0 // CPOLn : 1, CPHAn : 0 (X)
   USART1n_Config->ACK = USART1n_SPI_TX_FALLING;
   USART1n_Config->Edge = USART1n_SPI_TX_LEADEDGE_SAMPLE;
#endif

#if 0 // CPOLn : 1, CPHAn : 1 (O)
   USART1n_Config->ACK = USART1n_SPI_TX_FALLING;
   USART1n_Config->Edge = USART1n_SPI_TX_LEADEDGE_SETUP;
#endif
}

/*-------------------------------------------------------------------------*//**
 * @brief         Fills each USART_InitStruct member with its default value:
 *                   -  38400 bps
 *                   -  8 Data Bit
 *                   -  No Parity Bit
 *                   -  1 Stop Bit
 * @param[out]    USART1n_Config
 *                   Pointer to a USART1n_CFG_Type structure which will be initialized.
 * @return        none
 *//*-------------------------------------------------------------------------*/
void USART1n_USRT_Mode_ConfigStructInit( USART1n_CFG_Type* USART1n_Config )
{
   USART1n_Config->Mode = USART1n_USRT_MODE;
   USART1n_Config->Baudrate = 38400;
   USART1n_Config->Databits = USART1n_DATA_BIT_8;
   USART1n_Config->Parity = USART1n_PARITY_BIT_NONE;
   USART1n_Config->Stopbits = USART1n_STOP_BIT_1;

   //only SPI & Sync. Mode
   USART1n_Config->Order = USART1n_SPI_LSB;
#if 0 // CPOLn : 0, CPHAn : 0 (X)
   USART1n_Config->ACK = USART1n_SPI_TX_RISING;
   USART1n_Config->Edge = USART1n_SPI_TX_LEADEDGE_SAMPLE;
#endif
#if 1 // CPOLn : 0, CPHAn : 1 (O)
   USART1n_Config->ACK = USART1n_SPI_TX_RISING;
   USART1n_Config->Edge = USART1n_SPI_TX_LEADEDGE_SETUP;
#endif

#if 0 // CPOLn : 1, CPHAn : 0 (X)
   USART1n_Config->ACK = USART1n_SPI_TX_FALLING;
   USART1n_Config->Edge = USART1n_SPI_TX_LEADEDGE_SAMPLE;
#endif

#if 0 // CPOLn : 1, CPHAn : 1 (O)
   USART1n_Config->ACK = USART1n_SPI_TX_FALLING;
   USART1n_Config->Edge = USART1n_SPI_TX_LEADEDGE_SETUP;
#endif
}

/* USART Send/Recieve functions -------------------------------------------------*/
/*-------------------------------------------------------------------------*//**
 * @brief         Transmit a single data through USART peripheral
 * @param[in]     USART1x
 *                   Pointer to a target peripheral.
 *                   -  USART10 ~ USART13
 * @param[in]     Data
 *                   Data to transmit (must be 8-bit long)
 * @return        none
 *//*-------------------------------------------------------------------------*/
void USART1n_SendByte( USART1n_Type* USART1x, uint8_t Data )
{
   USART1x->DR = Data;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Receive a single data from USART peripheral
 * @param[in]     USART1x
 *                   Pointer to a target peripheral.
 *                   -  USART10 ~ USART13
 * @return        Data     received
 *//*-------------------------------------------------------------------------*/
uint8_t USART1n_ReceiveByte( USART1n_Type* USART1x )
{
   return USART1x->DR;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Send a block of data via USART peripheral
 * @param[in]     USART1x
 *                   Pointer to a target peripheral.
 *                   -  USART10 ~ USART13
 * @param[in]     txbuf
 *                   Pointer to Transmit buffer
 * @param[in]     buflen
 *                   Length of Transmit buffer
 * @param[in]     flag
 *                   Flag used in  USART transfer
 *                   -  NONE_BLOCKING
 *                   -  BLOCKING
 * @return        Number of bytes sent.
 * @note          when using USART in BLOCKING mode, a time-out condition is used
 *                via defined symbol USART_BLOCKING_TIMEOUT.
 *//*-------------------------------------------------------------------------*/
uint32_t USART1n_Send( USART1n_Type* USART1x, uint8_t* txbuf, uint32_t buflen, TRANSFER_BLOCK_Type flag )
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

         USART1n_SendByte( USART1x, ( *pChar++ ) );

         timeOut = USART1n_BLOCKING_TIMEOUT;
         // Wait for THR empty with timeout
         while( !( USART1x->ST & USART1n_SR_TXC ) )
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
         USART1n_ClearStatus( USART1x, USART1n_STATUS_TXC );
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
         if( !( USART1x->ST & USART1n_SR_DRE ) )
         {
            break;
         }
         USART1n_SendByte( USART1x, ( *pChar++ ) );
         bToSend--;
         bSent++;
      }
   }
   return bSent;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Receive a block of data via USART peripheral
 * @param[in]     USART1x
 *                   Pointer to a target peripheral.
 *                   -  USART10 ~ USART13
 * @param[out]    rxbuf
 *                   Pointer to Received buffer
 * @param[in]     buflen
 *                   Length of Received buffer
 * @param[in]     flag
 *                   Flag mode
 *                   -  NONE_BLOCKING
 *                   -  BLOCKING
 * @return        Number of bytes received
 * @note          when using USART in BLOCKING mode, a time-out condition is used
 *                via defined symbol USART_BLOCKING_TIMEOUT.
 *//*-------------------------------------------------------------------------*/
uint32_t USART1n_Receive( USART1n_Type* USART1x, uint8_t* rxbuf, uint32_t buflen, TRANSFER_BLOCK_Type flag )
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
         timeOut = USART1n_BLOCKING_TIMEOUT;
         while( !( USART1x->ST & USART1n_SR_RXC ) )
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
         ( *pChar++ ) = USART1n_ReceiveByte( USART1x );

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
         if( !( USART1x->ST & USART1n_SR_RXC ) )
         {
            break;
         }
         else
         {
            ( *pChar++ ) = USART1n_ReceiveByte( USART1x );
            bRecv++;
            bToRecv--;
         }
      }
   }
   return bRecv;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Configure the peripheral interrupt.
 * @param[in]     USART1x
 *                   Pointer to a target peripheral.
 *                   -  USART10 ~ USART13
 * @param[in]     USTIntCfg
 *                   Specifies the interrupt flag
 *                   -  USART1n_INTCFG_DR:      DR Interrupt enable
 *                   -  USART1n_INTCFG_TXC:     TXC Interrupt enable
 *                   -  USART1n_INTCFG_RXC:     RXC interrupt enable
 *                   -  USART1n_INTCFG_WAKE:    WAKE Interrupt enable
 * @param[in]     NewState
 *                   Next State of Interrupt Operation
 *                   -  ENABLE, DISABLE
 * @return        none
 *//*-------------------------------------------------------------------------*/
void USART1n_IntConfig( USART1n_Type* USART1x, USART1n_INT_Type USTIntCfg, FunctionalState NewState )
{
   uint32_t tmp = 0;


   switch( USTIntCfg )
   {
      case USART1n_INTCFG_WAKE :
         tmp = USART1n_IER_WAKEINT_EN;
         break;
      case USART1n_INTCFG_RXC:
         tmp = USART1n_IER_RXCINT_EN;
         break;
      case USART1n_INTCFG_TXC:
         tmp = USART1n_IER_TXCINT_EN;
         break;
      case USART1n_INTCFG_DR:
         tmp = USART1n_IER_DR_EN;
         break;
   }

   if( NewState == ENABLE )
   {
      USART1x->CR1 |= tmp;
   }
   else
   {
      USART1x->CR1 &= ~( tmp & USART1n_IER_BITMASK );
   }
}

/*-------------------------------------------------------------------------*//**
 * @brief         Get current value of Line Status register in USART peripheral.
 * @param[in]     USART1x
 *                   Pointer to a target peripheral.
 *                   -  USART10 ~ USART13
 * @return        Current value of Status register in USART peripheral.
 *//*-------------------------------------------------------------------------*/
uint8_t USART1n_GetStatus( USART1n_Type* USART1x )
{
   return ( ( USART1x->ST ) & USART1n_SR_BITMASK );
}

/*-------------------------------------------------------------------------*//**
 * @brief         Clear Status register in USART peripheral.
 * @param[in]     USART1x
 *                   Pointer to a target peripheral.
 *                   -  USART10 ~ USART13
 * @param[in]     Status
 * @return        none
 *//*-------------------------------------------------------------------------*/
void USART1n_ClearStatus( USART1n_Type* USART1x, USART1n_STATUS_Type Status )
{
   uint32_t tmp;

   switch( Status )
   {
      case USART1n_STATUS_WAKE:
         tmp = USART1n_SR_WAKE;
         break;
      case USART1n_STATUS_RXC:
         tmp = USART1n_SR_RXC;
         break;
      case USART1n_STATUS_TXC:
         tmp = USART1n_SR_TXC;
         break;
      case USART1n_STATUS_DRE:
         tmp = USART1n_SR_DRE;
         break;
      default:
         return;
   }

   USART1x->ST = tmp;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Check whether if USART is busy or not
 * @param[in]     USART1x
 *                   Pointer to a target peripheral.
 *                   -  USART10 ~ USART13
 * @return        RESET if USART is not busy, otherwise return SET.
 *//*-------------------------------------------------------------------------*/
FlagStatus USART1n_CheckBusy( USART1n_Type* USART1x )
{
   if( USART1x->ST & USART1n_SR_DRE )
   {
      return RESET;
   }
   else
   {
      return SET;
   }
}

/*-------------------------------------------------------------------------*//**
 * @brief         Configure Data Control mode for USART peripheral
 * @param[in]     USART1x
 *                   Pointer to a target peripheral.
 *                   -  USART10 ~ USART13
 * @param[in]     Mode
 *                   Data Control Mode
 *                   -  UST_CR_USTEN:     Activate USARTn Block by supplying.
 *                   -  UST_CR_DBLS:      Selects receiver sampling rate. (only UART mode)
 *                   -  UST_CR_MASTER:    Selects master or slave in SPIn or Synchronous mode and controls the direction of SCKn pin.
 *                   -  UST_CR_LOOPS:     Control the Loop Back mode of USARTn for test mode.
 *                   -  UST_CR_DISSCK:    In synchronous mode operation, selects the waveform of SCKn output.
 *                   -  UST_CR_USTSSEN:   This bit controls the SSn pin operation. (only SPI mode)
 *                   -  UST_CR_FXCH:      SPIn port function exchange control bit. (only SPI mode)
 *                   -  UST_CR_USTSB:     Selects the length of stop bit in Asynchronous or Synchronous mode.
 *                   -  UST_CR_USTTX8:    The ninth bit of data frame in Asynchronous or Synchronous mode of operation. Write this bit first before loading the USARTn_DR register.
 *                   -  UST_CR_USTRX8:    The ninth bit of data frame in Asynchronous or Synchronous mode of operation. Read this bit first before reading the receive buffer (only UART mode)
 * @param[in]     NewState
 *                   Next State of Functional Operation
 *                   -  ENABLE, DISABLE
 * @return        none
 *//*-------------------------------------------------------------------------*/
void USART1n_DataControlConfig( USART1n_Type* USART1x, USART1n_CONTROL_Type Mode, FunctionalState NewState )
{
   uint16_t    tmp = 0;

   switch( Mode )
   {
      case USART1n_CONTROL_USTRX8:
         tmp = USART1n_CR2_USTnRX8;
         break;
      case USART1n_CONTROL_USTTX8:
         tmp = USART1n_CR2_USTnTX8;
         break;
      case USART1n_CONTROL_USTSB:
         tmp = USART1n_CR2_USTnSB;
         break;
      case USART1n_CONTROL_FXCH:
         tmp = USART1n_CR2_FXCHn;
         break;
      case USART1n_CONTROL_USTSSEN:
         tmp = USART1n_CR2_USTnSSEN;
         break;
      case USART1n_CONTROL_DISSCK:
         tmp = USART1n_CR2_DISSCKn;
         break;
      case USART1n_CONTROL_LOOPS:
         tmp = USART1n_CR2_LOOPSn;
         break;
      case USART1n_CONTROL_MASTER:
         tmp = USART1n_CR2_MASTERn;
         break;
      case USART1n_CONTROL_DBLS:
         tmp = USART1n_CR2_DBLSn;
         break;
      case USART1n_CONTROL_USTEN:
         tmp = USART1n_CR2_USTnEN;
         break;
      default:
         break;
   }

   if( NewState == ENABLE )
   {
      USART1x->CR2 |= tmp;
   }
   else
   {
      USART1x->CR2 &= ~( tmp & USART1n_CR2_BITMASK );
   }
}

