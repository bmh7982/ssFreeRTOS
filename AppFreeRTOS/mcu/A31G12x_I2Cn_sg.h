/***************************************************************************//**
 * @file          A31G12x_I2Cn_sg.h
 * @brief         Driver Header File for I2C
 * @version       1.0
 * @date          2019-09-01
 * @author        ABOV Application Team
 *
 * Copyright (c) 2019, ABOV Semiconductor. All rights reserved.
 *//***************************************************************************/

#ifndef _I2Cn_SG_H_
#define _I2Cn_SG_H_

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Constant
//******************************************************************************

/**   I2C Slave Address registers bit mask */
#define I2Cn_SLA_BITMASK      (0xff)

// I2C state handle return values
#define RECEIVE_MODE          1
#define TRANS_MODE            2
#define RECEIVE_DATA          3
#define TRANS_DATA            4
#define LOST_BUS              5
#define STOP_DECT             6

//******************************************************************************
// Type
//******************************************************************************

//==============================================================================
// Enumeration
//==============================================================================

/**   Transfer option type definitions */
typedef enum
{
   I2Cn_TRANSFER_POLLING = 0,    /**<  Transfer in polling mode */
   I2Cn_TRANSFER_INTERRUPT       /**<  Transfer in interrupt mode */
} I2Cn_TRANSFER_OPT_Type;

//==============================================================================
// Structure
//==============================================================================

/**   Master transfer setup data structure definitions */
typedef struct
{
   uint32_t          sl_addr7bit;   /**<  Slave address in 7bit mode */
   uint8_t*          tx_data;       /**<  Pointer to Transmit data - NULL if data transmit is not used */
   uint32_t          tx_length;     /**<  Transmit data length - 0 if data transmit is not used */
   uint32_t          tx_count;      /**<  Current Transmit data counter */
   uint8_t*          rx_data;       /**<  Pointer to Receive data - NULL if data receive is not used */
   uint32_t          rx_length;     /**<  Receive data length - 0 if data receive is not used */
   uint32_t          rx_count;      /**<  Current Receive data counter */
} I2Cn_M_SETUP_Type;

/**   Slave transfer setup data structure definitions */
typedef struct
{
   uint8_t*          tx_data;       /**<  Pointer to transmit data - NULL if data transmit is not used */
   uint32_t          tx_length;     /**<  Transmit data length - 0 if data transmit is not used */
   uint32_t          tx_count;      /**<  Current transmit data counter */
   uint8_t*          rx_data;       /**<  Pointer to receive data - NULL if data received is not used */
   uint32_t          rx_length;     /**<  Receive data length - 0 if data receive is not used */
   uint32_t          rx_count;      /**<  Current receive data counter */
} I2Cn_S_SETUP_Type;

//******************************************************************************
// Function
//******************************************************************************

int32_t I2Cn_getNum( I2Cn_Type* I2Cx );

void I2Cn_Init( I2Cn_Type* I2Cx, uint32_t clockrate );
void I2Cn_DeInit( I2Cn_Type* I2Cx );

Status I2Cn_MasterTransferData( I2Cn_Type* I2Cx, I2Cn_M_SETUP_Type* TransferCfg, I2Cn_TRANSFER_OPT_Type Opt );

Status I2Cn_SlaveTransferData( I2Cn_Type* I2Cx, I2Cn_S_SETUP_Type* TransferCfg, I2Cn_TRANSFER_OPT_Type Opt );
uint32_t I2Cn_MasterTransferComplete( I2Cn_Type* I2Cx );
uint32_t I2Cn_SlaveTransferComplete( I2Cn_Type* I2Cx );

void I2Cn_Interrupt_MasterHandler( I2Cn_Type*  I2Cx );
void I2Cn_Interrupt_SlaveHandler( I2Cn_Type*  I2Cx );

Status I2Cn_MasterTransmitData( I2Cn_Type* I2Cx, I2Cn_M_SETUP_Type* TransferCfg, I2Cn_TRANSFER_OPT_Type Opt );
Status I2Cn_MasterReceiveData( I2Cn_Type* I2Cx, I2Cn_M_SETUP_Type* TransferCfg, I2Cn_TRANSFER_OPT_Type Opt );
Status I2Cn_SlaveReceiveData( I2Cn_Type* I2Cx, I2Cn_S_SETUP_Type* TransferCfg, I2Cn_TRANSFER_OPT_Type Opt );

void I2Cn_SetOwnSlaveAddr1( I2Cn_Type* I2Cx, uint8_t SlaveAddr_7bit, uint8_t GeneralCallState );
void I2Cn_SetOwnSlaveAddr2( I2Cn_Type* I2Cx, uint8_t SlaveAddr_7bit, uint8_t GeneralCallState );

void I2Cn_IntCmd( I2Cn_Type* I2Cx, Bool NewState );

#ifdef __cplusplus
}
#endif

#endif   /* _I2Cn_SG_H_ */

