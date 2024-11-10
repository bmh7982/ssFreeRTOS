/***************************************************************************//**
 * @file          A31G12x_I2Cn.c
 * @brief         Driver Source File for I2C
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
#include "A31G12x_I2Cn_sg.h"   //#include "A31G12x_I2Cn.h"
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

#include "A31G12x_I2Cn_sg.h"
//******************************************************************************
// Constant
//******************************************************************************
#define ERROR_DECT           -1

#define I2Cn_BLOCKING_TIMEOUT    (0x000FFFFFUL)
#define I2Cn_MAX                 3

//******************************************************************************
// Type
//******************************************************************************
#pragma anon_unions

/**   I2Cn device configuration structure type */
typedef struct
{
   union
   {
      I2Cn_M_SETUP_Type  txrx_setup_master;      /**<  Transmission setup */
      I2Cn_S_SETUP_Type  txrx_setup_slave;       /**<  Transmission setup */
   };
   int32_t     dir;        /* Current direction phase, 0 - write, 1 - read */
} I2Cn_CFG_T;

//******************************************************************************
// Variable
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         I2Cn driver data
 *//*-------------------------------------------------------------------------*/

static I2Cn_CFG_T    i2cdat[I2Cn_MAX];
Bool          I2Cn_MasterComplete[I2Cn_MAX];
Bool          I2Cn_SlaveComplete[I2Cn_MAX];

//******************************************************************************
// Function
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         Convert from I2C peripheral to number
 * @param[in]     I2Cx
 *                   Pointer to a target peripheral.
 *                   -  I2C0 ~ I2C2
 * @return        I2C number or error code, could be:
 *                   -  0:       I2C0
 *                   -  1:       I2C1
 *                   -  2:       I2C2
 *                   -  (-1):    Error
 *//*-------------------------------------------------------------------------*/
int32_t I2Cn_getNum( I2Cn_Type* I2Cx )
{
   if(I2Cx == ( I2Cn_Type* )I2C0 ) { return 0; }
   if(I2Cx == ( I2Cn_Type* )I2C1 ) { return 1; }
   if(I2Cx == ( I2Cn_Type* )I2C2 ) { return 2; }
   return -1;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Initialize the I2Cn peripheral with the specified parameters.
 * @param[in]     I2Cx
 *                   Pointer to a target peripheral.
 *                   -  I2C0 ~ I2C2
 * @param[in]     clockrate
 *                   Target Clock Rate (Hz)
 * @return        none
 *//*-------------------------------------------------------------------------*/
void I2Cn_Init( I2Cn_Type* I2Cx, uint32_t clockrate )
{
	if( I2Cx == ( I2Cn_Type* )I2C0 ) { SCUCG->PPCLKEN2_b.I2C0CLKE = 1; SCUCG_RstEachPeri2( 1 << 6 ); } // Set up clock for I2C0 module
	if( I2Cx == ( I2Cn_Type* )I2C1 ) { SCUCG->PPCLKEN2_b.I2C1CLKE = 1; SCUCG_RstEachPeri2( 1 << 7 ); } // Set up clock for I2C1 module
	if( I2Cx == ( I2Cn_Type* )I2C2 ) { SCUCG->PPCLKEN2_b.I2C2CLKE = 1; SCUCG_RstEachPeri2( 1 << 8 ); } // Set up clock for I2C2 module

	I2Cx->CR_b.I2CnEN = 1;     // I2C Block Active
	I2Cx->CR_b.I2CnIEN = 1;    // I2C Interrupt Enable

	I2Cx->SCLR = ( SystemPeriClock/clockrate - 4 )/8;      // freq = PCLK / ((4*SCLL+2) + (4*SCLH+2))
	I2Cx->SCHR = ( SystemPeriClock/clockrate - 4 )/8;      // ex) 100k = 10M / ((4*12+2) + (4*12+2)), if PCLK : 10MHz
	I2Cx->SDHR = 1;            // default value 1

	I2Cx->CR_b.ACKnEN = 1;     // ACK Signal Enable
}

/*-------------------------------------------------------------------------*//**
 * @brief         Deinitialize the I2Cn peripheral registers to their default reset values.
 * @param[in]     I2Cx
 *                   Pointer to a target peripheral.
 *                   -  I2C0 ~ I2C2
 * @return        none
 *//*-------------------------------------------------------------------------*/
void I2Cn_DeInit( I2Cn_Type* I2Cx )
{
   /* Disable I2C control */
   I2Cx->CR = 0;     // I2C Block Disable

   if(I2Cx == (I2Cn_Type*)I2C0) { SCUCG->PPCLKEN2_b.I2C0CLKE = 0; } // Set up clock for I2C0 module
   if(I2Cx == (I2Cn_Type*)I2C1) { SCUCG->PPCLKEN2_b.I2C1CLKE = 0; } // Set up clock for I2C1 module
   if(I2Cx == (I2Cn_Type*)I2C2) { SCUCG->PPCLKEN2_b.I2C2CLKE = 0; } // Set up clock for I2C2 module
}

/*-------------------------------------------------------------------------*//**
 * @brief         Enable/Disable interrupt for I2C peripheral
 * @param[in]     I2Cx
 *                   Pointer to a target peripheral.
 *                   -  I2C0 ~ I2C2
 * @param[in]     NewState
 *                   Next State of Interrupt Operation
 *                   -  ENABLE, DISABLE
 * @return        none
 *//*-------------------------------------------------------------------------*/
void I2Cn_IntCmd( I2Cn_Type* I2Cx, Bool NewState )
{
   if( NewState )   {
      if( I2Cx == (I2Cn_Type*)I2C0) { NVIC_ClearPendingIRQ(I2C0_IRQn); NVIC_EnableIRQ(I2C0_IRQn); }
      if( I2Cx == (I2Cn_Type*)I2C1) { NVIC_ClearPendingIRQ(I2C1_IRQn); NVIC_EnableIRQ(I2C1_IRQn); }
      if( I2Cx == (I2Cn_Type*)I2C2) { NVIC_ClearPendingIRQ(I2C2_IRQn); NVIC_EnableIRQ(I2C2_IRQn); }
   }
   else   {
      if( I2Cx == (I2Cn_Type*)I2C0) { NVIC_DisableIRQ(I2C0_IRQn); }
      if( I2Cx == (I2Cn_Type*)I2C1) { NVIC_DisableIRQ(I2C1_IRQn); }
      if( I2Cx == (I2Cn_Type*)I2C2) { NVIC_DisableIRQ(I2C2_IRQn); }
   }
}

/*-------------------------------------------------------------------------*//**
 * @brief         General Master Interrupt handler for I2C peripheral
 * @param[in]     I2Cx
 *                   Pointer to a target peripheral.
 *                   -  I2C0 ~ I2C2
 * @return        none
 *//*-------------------------------------------------------------------------*/
void I2Cn_Interrupt_MasterHandler( I2Cn_Type* I2Cx )
{
   int32_t              tmp;
   I2Cn_M_SETUP_Type*   txrx_setup;
   uint32_t             status;

   tmp = I2Cn_getNum( I2Cx );
   txrx_setup = ( I2Cn_M_SETUP_Type* )&i2cdat[tmp].txrx_setup_master;

   status = I2Cx->ST;

   switch( status )
   {
      case 0x87: // transmit mode - addr ACK
         if( txrx_setup->tx_count < txrx_setup->tx_length )
         {
            I2Cx->DR = txrx_setup->tx_data[txrx_setup->tx_count];
            txrx_setup->tx_count++;
         }
         else
         {
            I2Cx->CR = 0
                       | ( 1 << 7 )    // Enable I2C Block
                       | ( 1 << 5 )    // Interrupt Enable
                       | ( 1 << 1 );   // STOP
         }
         break;

      case 0x47: // transmit mode - data ACK
         if( txrx_setup->tx_count < txrx_setup->tx_length )
         {
            I2Cx->DR = txrx_setup->tx_data[txrx_setup->tx_count];
            txrx_setup->tx_count++;
         }
         else
         {
            if( txrx_setup->rx_count < txrx_setup->rx_length )
            {
               I2Cx->DR = ( ( txrx_setup->sl_addr7bit << 1 ) | 0x01 ); // 0:write, 1:read
               I2Cx->CR |= ( 1 << 0 );    // reSTART
            }
            else
            {
               I2Cx->CR = 0
                          | ( 1 << 7 )    // Enable I2C Block
                          | ( 1 << 5 )    // Interrupt Enable
                          | ( 1 << 1 );   // STOP
            }
         }
         break;

      case 0x85: // receive mode - addr ACK
         if( txrx_setup->rx_count < txrx_setup->rx_length )
         {
            if( ( txrx_setup->rx_length > 1 ) && ( txrx_setup->rx_count < ( txrx_setup->rx_length - 1 ) ) )
            {
               NOP();
            }
            else
            {
               I2Cx->CR_b.ACKnEN = 0;     // disable ACKEN
            }
         }
         else
         {
            I2Cx->CR = 0
                       | ( 1 << 7 )    // Enable I2C Block
                       | ( 1 << 5 )    // Interrupt Enable
                       | ( 1 << 1 );   // STOP
         }
         break;

      case 0x45: // receive mode - data ACK
         if( txrx_setup->rx_count < txrx_setup->rx_length )
         {
            txrx_setup->rx_data[txrx_setup->rx_count] = I2Cx->DR;
            txrx_setup->rx_count++;

            if( ( txrx_setup->rx_length > 1 ) && ( txrx_setup->rx_count < ( txrx_setup->rx_length - 1 ) ) )
            {
               NOP();
            }
            else
            {
               I2Cx->CR_b.ACKnEN = 0;     // disable ACKEN
            }
         }
         break;

      case 0x44: // receive mode - data NOACK
         if( txrx_setup->rx_count < txrx_setup->rx_length )
         {
            txrx_setup->rx_data[txrx_setup->rx_count] = I2Cx->DR;
            txrx_setup->rx_count++;

            I2Cx->CR = 0
                       | ( 1 << 7 )    // Enable I2C Block
                       | ( 1 << 5 )    // Interrupt Enable
                       | ( 1 << 1 );   // STOP
         }
         break;

      case 0x20: // receive mode
      case 0x22: // transmit mode - stop receive
         I2Cx->CR = 0
                    | ( 1 << 7 )    // Enable I2C Block
                    | ( 1 << 5 )    // Interrupt Enable
                    | ( 1 << 3 );   // ACK Signal Enable
         goto s_int_end;

      default:
         if( status & 0x08 )     // mastership lost
         {

         }
         break;
   }

   I2Cx->ST = 0xff;     // flag clear and SCL go to HIGH
   return;

s_int_end:

   I2Cx->ST = 0xff;     // flag clear and SCL go to HIGH

   // Disable interrupt
   I2Cn_IntCmd( I2Cx, FALSE );

   I2Cn_MasterComplete[tmp] = TRUE;
}

/*-------------------------------------------------------------------------*//**
 * @brief         General Slave Interrupt handler for I2C peripheral
 * @param[in]     I2Cx
 *                   Pointer to a target peripheral.
 *                   -  I2C0 ~ I2C2
 * @return        none
 *//*-------------------------------------------------------------------------*/
void I2Cn_Interrupt_SlaveHandler( I2Cn_Type* I2Cx )
{
   int32_t              tmp;
   I2Cn_S_SETUP_Type*   txrx_setup;
   uint32_t             status;

   tmp = I2Cn_getNum( I2Cx );
   txrx_setup = ( I2Cn_S_SETUP_Type* )&i2cdat[tmp].txrx_setup_slave;

   status = I2Cx->ST;
   switch( status )
   {
      case 0x15: // receive mode - slave select + ACK
      case 0x45: // receive mode - data ACK
         if( ( txrx_setup->rx_count < txrx_setup->rx_length ) && ( txrx_setup->rx_data != NULL ) )
         {
            txrx_setup->rx_data[txrx_setup->rx_count] = I2Cx->DR;
            txrx_setup->rx_count++;
         }
         break;

      case 0x20: // receive mode
      case 0x22: // transmit mode - stop receive
         goto s_int_end;

      case 0x17: // transmit mode - slave select + ACK
      case 0x46: // transmit mode - data NOACK
      case 0x47: // transmit mode - data ACK
         if( ( txrx_setup->tx_count < txrx_setup->tx_length ) && ( txrx_setup->tx_data != NULL ) )
         {
            I2Cx->DR = txrx_setup->tx_data[txrx_setup->tx_count];
            txrx_setup->tx_count++;
         }
         break;
   }

   I2Cx->ST = 0xff;     // flag clear and SCL go to HIGH
   return;

s_int_end:

   I2Cx->ST = 0xff;     // flag clear and SCL go to HIGH

   // Disable interrupt
   I2Cn_IntCmd( I2Cx, FALSE );

   I2Cn_SlaveComplete[tmp] = TRUE;
}

/*-------------------------------------------------------------------------*//**
 * @brief         wait and return status in master mode
 * @param[in]     I2Cx
 *                   Pointer to a target peripheral.
 *                   -  I2C0 ~ I2C2
 * @return        Status
 *//*-------------------------------------------------------------------------*/
int32_t I2Cn_MWait( I2Cn_Type* I2Cx )
{
	uint32_t    tmp;
	int32_t     ret = 0;

	tmp = 0;
	while( 1 )     // Interrupt Status Check
	{
		if( ( I2Cx->CR & I2Cn_CR_I2CnIFLAG_Msk ) != 0 ) {
			break;
		}
		// Ω√∞£√ ∞˙∏¶ ∏∑±‚¿ß«— ¿”Ω√ πÊ∆Ì
		if(70000<++tmp){
			return ERROR_DECT;
		}
	}

	tmp = I2Cx->ST;
	I2Cx->ST = 0xFF;

	switch( tmp )
	{
		// Transmitter mode
		case 0x87:
			ret = TRANS_MODE;
			break;
		// Receive mode
		case 0x85:
			ret = RECEIVE_MODE;
			break;

		// Transed Data
		case 0x47:
			ret = TRANS_DATA;
			break;

		// Received Data
		case 0x44:
		case 0x45:
			ret = RECEIVE_DATA;
			break;

		default:
			if     ( ( I2Cx->ST ) & 0x08 ){ ret = LOST_BUS; }   // lost
			else if( ( I2Cx->ST ) & 0x20 ){ ret = STOP_DECT;}   // stop
			else                          { ret = -1;       }
			break;
	}
		return ret;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Transmit and Receive data in master mode
 * @param[in]     I2Cx
 *                   Pointer to a target peripheral.
 *                   -  I2C0 ~ I2C2
 * @param[in]     TransferCfg
 *                   Pointer to a I2Cn_M_SETUP_Type structure
 *                   that contains specified information
 *                   about the configuration for master transfer.
 * @param[in]     Opt
 *                   I2Cn_TRANSFER_OPT_Type type
 *                   that selected for interrupt or polling mode.
 * @return        Result
 *                   -  SUCCESS, ERROR
 *//*-------------------------------------------------------------------------*/
void I2C_Error(I2Cn_Type* I2Cx)
{
	I2Cn_DeInit(I2Cx);

   uint32_t SCLR = I2Cx->SCLR;
   uint32_t SCHR = I2Cx->SCHR;
   uint32_t SDHR = I2Cx->SDHR;

   if     (I2Cx==(I2Cn_Type*)I2C0) {SCUCG->PPCLKEN2_b.I2C0CLKE = 1; SCUCG_RstEachPeri2( 1 << 6 );} /* Set up clock for I2C0 module */
   else if(I2Cx==(I2Cn_Type*)I2C1) {SCUCG->PPCLKEN2_b.I2C1CLKE = 1; SCUCG_RstEachPeri2( 1 << 7 );} /* Set up clock for I2C1 module */
   else if(I2Cx==(I2Cn_Type*)I2C2) {SCUCG->PPCLKEN2_b.I2C2CLKE = 1; SCUCG_RstEachPeri2( 1 << 8 );} /* Set up clock for I2C2 module */

   I2Cx->CR_b.I2CnEN = 1;     // I2C Block Active
   I2Cx->CR_b.I2CnIEN = 1;    // I2C Interrupt Enable

   I2Cx->SCLR = SCLR;
   I2Cx->SCHR = SCHR;
   I2Cx->SDHR = SDHR;
   I2Cx->CR_b.ACKnEN = 1;     // ACK Signal Enable
}
int test;
Status I2Cn_MasterTransferData( I2Cn_Type* I2Cx, I2Cn_M_SETUP_Type* TransferCfg, I2Cn_TRANSFER_OPT_Type Opt )
{
   int32_t     tmp;
   uint32_t    exitflag;
   int32_t     Ret;

   // Reset I2C setup value to default state
   TransferCfg->tx_count = 0;
   TransferCfg->rx_count = 0;

	// busy check
	//TimerSet(2);
	tmp = 0;
   while(I2Cx->ST&0x04){     // busy check
		if(70000<++tmp){       // ÌÖåÏä§Ìä∏Ïö©
			//I2C_Error(I2Cx);
			break;
		}
		//if(!istimer()) continue;
		//I2C_Error(I2Cx);
		//break;
	}

   if( Opt == I2Cn_TRANSFER_POLLING )
   {
      /* First Start condition -------------------------------------------------------------- */
      // Reset I2C setup value to default state
      TransferCfg->tx_count = 0;
      TransferCfg->rx_count = 0;

      if( TransferCfg->tx_count < TransferCfg->tx_length)
      {
         I2Cx->DR = ( TransferCfg->sl_addr7bit << 1 ); //write

         // Start command
         I2Cx->CR |= ( 1 << 0 );    // START
         Ret = I2Cn_MWait( I2Cx );

         if( ( Ret != TRANS_MODE ) )
         {
            I2Cx->CR |= ( 1 << 1 );    // STOP
            I2Cn_MWait( I2Cx );
            I2Cx->ST = 0xFF;
            I2Cx->CR = 0
                       | ( 1 << 7 )    // I2C Block Enable
                       | ( 1 << 5 )    // Interrupt Enable
                       | ( 1 << 3 );   // ACK Signal Enable
            return ERROR;
         }

         exitflag = 1;
         while( exitflag )
         {
            if( TransferCfg->tx_count < TransferCfg->tx_length )
            {
               I2Cx->DR = TransferCfg->tx_data[TransferCfg->tx_count];
               TransferCfg->tx_count++;
               I2Cx->ST = 0xFF;
               Ret = I2Cn_MWait( I2Cx );

               //test
               if(TransferCfg->tx_count == TransferCfg->tx_length && TransferCfg->rx_length==0){
                  I2Cx->CR |= ( 1 << 1 );    // STOP
               }
               //test

              if( ( Ret != TRANS_DATA ) )
               {
                  I2Cx->CR |= ( 1 << 1 );    // STOP
                  I2Cn_MWait( I2Cx );
                  I2Cx->ST = 0xFF;
                  I2Cx->CR = 0
                             | ( 1 << 7 )    // I2C Block Enable
                             | ( 1 << 5 )    // Interrupt Enable
                             | ( 1 << 3 );   // ACK Signal Enable
                  return ERROR;
               }
            }
            else
            {
               if( TransferCfg->rx_count >= TransferCfg->rx_length )
               {
                  I2Cx->CR |= ( 1 << 1 );    // STOP

                  I2Cx->ST = 0xFF;
                  I2Cn_MWait( I2Cx );

                  I2Cx->ST = 0xFF;

                  I2Cx->CR = 0
                             | ( 1 << 7 )    // I2C Block Enable
                             | ( 1 << 5 )    // Interrupt Enable
                             | ( 1 << 3 );   // ACK Signal Enable
                  return SUCCESS;
               }
               else
               {
                  exitflag = 0;
               }
            }
         }
      }

      if( TransferCfg->rx_count < TransferCfg->rx_length )
      {
         I2Cx->DR = ( ( TransferCfg->sl_addr7bit << 1 ) | 0x01 );
         I2Cx->CR |= ( 1 << 0 );    // START

         I2Cx->ST = 0xFF;
         Ret = I2Cn_MWait( I2Cx );

         if( ( Ret != RECEIVE_MODE ) )
         {
            I2Cx->CR |= ( 1 << 1 );    // STOP
            I2Cn_MWait( I2Cx );
            I2Cx->ST = 0xFF;
            I2Cx->CR = 0
                       | ( 1 << 7 )    // I2C Block Enable
                       | ( 1 << 5 )    // Interrupt Enable
                       | ( 1 << 3 );   // ACK Signal Enable
            return ERROR;
         }

         exitflag = 1;
         while( exitflag )
         {
            if( ( TransferCfg->rx_length > 1 ) && ( TransferCfg->rx_count < ( TransferCfg->rx_length - 1 ) ) )
            {

               I2Cx->ST = 0xFF;
               Ret = I2Cn_MWait( I2Cx );

               if( ( Ret != RECEIVE_DATA ) )
               {
                  I2Cx->CR |= ( 1 << 1 );    // STOP
                  I2Cn_MWait( I2Cx );
                  I2Cx->ST = 0xFF;
                  I2Cx->CR = 0
                             | ( 1 << 7 )    // I2C Block Enable
                             | ( 1 << 5 )    // Interrupt Enable
                             | ( 1 << 3 );   // ACK Signal Enable
                  return ERROR;
               }
            }
            else    // the next byte is the last byte, send NACK instead.
            {
               I2Cx->CR &= ~( 1 << 3 );   // ACK Signal Disable

               I2Cx->ST = 0xFF;
               Ret = I2Cn_MWait( I2Cx );

               if( ( Ret != RECEIVE_DATA ) )
               {
                  I2Cx->CR |= ( 1 << 1 );    // STOP
                  I2Cn_MWait( I2Cx );
                  I2Cx->CR = 0
                             | ( 1 << 7 )    // I2C Block Enable
                             | ( 1 << 5 )    // Interrupt Enable
                             | ( 1 << 3 );   // ACK Signal Enable
                  return ERROR;
               }
            }
            TransferCfg->rx_data[TransferCfg->rx_count] = I2Cx->DR;
            TransferCfg->rx_count++;
            if( TransferCfg->rx_count == TransferCfg->rx_length )
            {
               exitflag = 0;
               // commented by kth  return SUCCESS;
            }
         }

         I2Cx->CR |= ( 1 << 1 );    // STOP

         I2Cx->ST = 0xFF;

         I2Cn_MWait( I2Cx );

         I2Cx->ST = 0xFF;

         I2Cx->CR = 0
                    | ( 1 << 7 )    // I2C Block Enable
                    | ( 1 << 5 )    // Interrupt Enable
                    | ( 1 << 3 );   // ACK Signal Enable
         return SUCCESS;
      }
   }
   else if( Opt == I2Cn_TRANSFER_INTERRUPT )
   {
      tmp = I2Cn_getNum( I2Cx );
      I2Cn_MasterComplete[tmp] = FALSE;

      // Setup tx_rx data, callback and interrupt handler
      i2cdat[tmp].txrx_setup_master = *TransferCfg;

      // Set direction phase, write first
      i2cdat[tmp].dir = 0;

      /* First Start condition -------------------------------------------------------------- */
      I2Cn_IntCmd( I2Cx, TRUE );
      if( TransferCfg->tx_count < TransferCfg->tx_length )
      {
         I2Cx->DR = ( TransferCfg->sl_addr7bit << 1 ); //write
      }
      else if( TransferCfg->rx_count < TransferCfg->rx_length )
      {
         I2Cx->DR = ( ( TransferCfg->sl_addr7bit << 1 ) | 0x01 );
      }

      // Start command
      I2Cx->CR |= ( 1 << 0 );    // START

      return SUCCESS;
   }

   return ERROR;
}

/*-------------------------------------------------------------------------*//**
 * @brief         wait and return status in slave mode
 * @param[in]     I2Cx
 *                   Pointer to a target peripheral.
 *                   -  I2C0 ~ I2C2
 * @return        Status
 *//*-------------------------------------------------------------------------*/
int32_t I2Cn_SWait( I2Cn_Type* I2Cx )
{
   uint32_t    tmp;
   int32_t     ret = 0;

   while( 1 )     // Interrupt Status Check
   {
      if( ( I2Cx->CR & I2Cn_CR_I2CnIFLAG_Msk ) != 0 )
      {
         break;
      }
   }

   tmp = I2Cx->ST;
   I2Cx->ST = 0xFF;

   switch( tmp )
   {
      // Receive mode
      case 0x15:
      case 0x95:
         ret = RECEIVE_MODE;
         break;

      // Transmitter mode
      case 0x17:
      case 0x97:
         ret = TRANS_MODE;
         break;

      // Received Data
      case 0x45:
         ret = RECEIVE_DATA;
         break;

      // Transed Data
      case 0x47:
         ret = TRANS_DATA;
         break;
      default:
         if( tmp & 0x08 )
         {
            ret = LOST_BUS;   // lost
         }
         else if( tmp & 0x20 )
         {
            ret = STOP_DECT;   // stop
         }
         else
         {
            ret = -1;
         }
         break;
   }

   return ret;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Receive and Transmit data in slave mode
 * @param[in]     I2Cx
 *                   Pointer to a target peripheral.
 *                   -  I2C0 ~ I2C2
 * @param[in]     TransferCfg
 *                   Pointer to a I2Cn_S_SETUP_Type structure
 *                   that contains specified information
 *                   about the configuration for master transfer.
 * @param[in]     Opt
 *                   I2Cn_TRANSFER_OPT_Type type
 *                   that selected for interrupt or polling mode.
 * @return        Result
 *                   -  SUCCESS, ERROR
 *//*-------------------------------------------------------------------------*/
Status I2Cn_SlaveTransferData( I2Cn_Type* I2Cx, I2Cn_S_SETUP_Type* TransferCfg, I2Cn_TRANSFER_OPT_Type Opt )
{
   int32_t     tmp;
   int32_t     Ret;

   // Reset I2C setup value to default state
   TransferCfg->tx_count = 0;
   TransferCfg->rx_count = 0;

   // Polling option
   if( Opt == I2Cn_TRANSFER_POLLING )
   {
      while( 1 )
      {
         Ret = I2Cn_SWait( I2Cx );      // Start
         switch( Ret )
         {
            case RECEIVE_MODE:
            case RECEIVE_DATA:
               if( ( TransferCfg->rx_count < TransferCfg->rx_length ) && ( TransferCfg->rx_data != NULL ) )
               {
                  TransferCfg->rx_data[TransferCfg->rx_count] = I2Cx->DR;
                  TransferCfg->rx_count++;
               }
               break;
            case TRANS_MODE:
            case TRANS_DATA:
               if( ( TransferCfg->tx_count < TransferCfg->tx_length ) && ( TransferCfg->tx_data != NULL ) )
               {
                  I2Cx->DR = TransferCfg->tx_data[TransferCfg->tx_count];
                  TransferCfg->tx_count++;
               }
               break;
            case STOP_DECT:
               goto s_end_stage;
            case 0:
               break;
            default:
               goto s_error;
         }
      }

s_end_stage:
      I2Cx->ST = 0xFF;
      return SUCCESS;

s_error:
      I2Cx->ST = 0xFF;
      return ERROR;
   }

   else if( Opt == I2Cn_TRANSFER_INTERRUPT )
   {
      tmp = I2Cn_getNum( I2Cx );
      I2Cn_SlaveComplete[tmp] = FALSE;

      // Setup tx_rx data, callback and interrupt handler
      i2cdat[tmp].txrx_setup_slave = *TransferCfg;

      // Set direction phase, read first
      i2cdat[tmp].dir = 1;

      I2Cn_IntCmd( I2Cx, TRUE );

      return SUCCESS;
   }

   return ERROR;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Transmit an array of bytes in Master mode
 * @param[in]     I2Cx
 *                   Pointer to a target peripheral.
 *                   -  I2C0 ~ I2C2
 * @param[in]     TransferCfg
 *                   Pointer to a I2Cn_M_SETUP_Type structure
 *                   that contains specified information
 *                   about the configuration for master transfer.
 * @param[in]     Opt
 *                   I2Cn_TRANSFER_OPT_Type type
 *                   that selected for interrupt or polling mode.
 * @return        Result
 *                   -  SUCCESS, ERROR
 *//*-------------------------------------------------------------------------*/
Status I2Cn_MasterTransmitData( I2Cn_Type* I2Cx, I2Cn_M_SETUP_Type* TransferCfg, I2Cn_TRANSFER_OPT_Type Opt )
{
   TransferCfg->rx_data = NULL;
   TransferCfg->rx_length = 0;
   TransferCfg->tx_count = 0;
   TransferCfg->rx_count = 0;

   return I2Cn_MasterTransferData( I2Cx, TransferCfg, Opt );
}

/*-------------------------------------------------------------------------*//**
 * @brief         Receive an array of bytes in Master mode
 * @param[in]     I2Cx
 *                   Pointer to a target peripheral.
 *                   -  I2C0 ~ I2C2
 * @param[in]     TransferCfg
 *                   Pointer to a I2Cn_M_SETUP_Type structure
 *                   that contains specified information
 *                   about the configuration for master transfer.
 * @param[in]     Opt
 *                   I2Cn_TRANSFER_OPT_Type type
 *                   that selected for interrupt or polling mode.
 * @return        Result
 *                   -  SUCCESS, ERROR
 *//*-------------------------------------------------------------------------*/
Status I2Cn_MasterReceiveData( I2Cn_Type* I2Cx, I2Cn_M_SETUP_Type* TransferCfg, I2Cn_TRANSFER_OPT_Type Opt )
{
   TransferCfg->tx_data = NULL;
   TransferCfg->tx_length = 0;
   TransferCfg->tx_count = 0;
   TransferCfg->rx_count = 0;

   return I2Cn_MasterTransferData( I2Cx, TransferCfg, Opt );
}

/*-------------------------------------------------------------------------*//**
 * @brief         Receive an array of bytes in Slave mode
 * @param[in]     I2Cx
 *                   Pointer to a target peripheral.
 *                   -  I2C0 ~ I2C2
 * @param[in]     TransferCfg
 *                   Pointer to a I2Cn_S_SETUP_Type structure
 *                   that contains specified information
 *                   about the configuration for slave transfer.
 * @param[in]     Opt
 *                   I2Cn_TRANSFER_OPT_Type type
 *                   that selected for interrupt or polling mode.
 * @return        Result
 *                   -  SUCCESS, ERROR
 *//*-------------------------------------------------------------------------*/
Status I2Cn_SlaveReceiveData( I2Cn_Type* I2Cx, I2Cn_S_SETUP_Type* TransferCfg, I2Cn_TRANSFER_OPT_Type Opt )
{
   TransferCfg->tx_data = NULL;
   TransferCfg->tx_length = 0;
   TransferCfg->tx_count = 0;
   TransferCfg->rx_count = 0;

   return I2Cn_SlaveTransferData( I2Cx, TransferCfg, Opt );
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set Own slave address in I2C peripheral corresponding to
 *                parameter specified in OwnSlaveAddrConfigStruct.
 * @param[in]     I2Cx
 *                   Pointer to a target peripheral.
 *                   -  I2C0 ~ I2C2
 * @param[in]     SlaveAddr_7bit
 *                   own slave address
 * @param[in]     GeneralCallState
 *                   -  ENABLE, DISABLE
 * @return        none
 *//*-------------------------------------------------------------------------*/
void I2Cn_SetOwnSlaveAddr1( I2Cn_Type* I2Cx, uint8_t SlaveAddr_7bit, uint8_t GeneralCallState )
{
   I2Cx->SAR1 = ( ( ( uint32_t )( SlaveAddr_7bit << 1 ) ) | ( ( GeneralCallState == ENABLE ) ? 0x01 : 0x00 ) ) & I2Cn_SLA_BITMASK;
}

void I2Cn_SetOwnSlaveAddr2( I2Cn_Type* I2Cx, uint8_t SlaveAddr_7bit, uint8_t GeneralCallState )
{
   I2Cx->SAR2 = ( ( ( uint32_t )( SlaveAddr_7bit << 1 ) ) | ( ( GeneralCallState == ENABLE ) ? 0x01 : 0x00 ) ) & I2Cn_SLA_BITMASK;
}


/*-------------------------------------------------------------------------*//**
 * @brief         Get Status of Master Transfer
 * @param[in]     I2Cx
 *                   Pointer to a target peripheral.
 *                   -  I2C0 ~ I2C2
 * @return        Status of Master Transfer
 *                   -  TRUE, FALSE
 *//*-------------------------------------------------------------------------*/
uint32_t I2Cn_MasterTransferComplete( I2Cn_Type* I2Cx )
{
   uint32_t    retval, tmp;

   tmp = I2Cn_getNum( I2Cx );
   retval = I2Cn_MasterComplete[tmp];
   I2Cn_MasterComplete[tmp] = FALSE;

   return retval;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Get Status of Slave Transfer
 * @param[in]     I2Cx
 *                   Pointer to a target peripheral.
 *                   -  I2C0 ~ I2C2
 * @return        Status of Slave Transfer
 *                   -  TRUE, FALSE
 *//*-------------------------------------------------------------------------*/
uint32_t I2Cn_SlaveTransferComplete( I2Cn_Type* I2Cx )
{
   uint32_t    retval, tmp;

   tmp = I2Cn_getNum( I2Cx );
   retval = I2Cn_SlaveComplete[tmp];
   I2Cn_SlaveComplete[tmp] = FALSE;

   return retval;
}

