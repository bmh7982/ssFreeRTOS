/***************************************************************************//**
 * @file          A31G12x_UARTn.h
 * @brief         Driver Header File for UART
 * @version       1.0
 * @date          2019-09-01
 * @author        ABOV Application Team
 *
 * Copyright (c) 2019, ABOV Semiconductor. All rights reserved.
 *//***************************************************************************/

#ifndef _UARTn_H_
#define _UARTn_H_

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Constant
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * UARTn time-out definitions in case of using Read() and Write function
 * with Blocking Flag mode
 *//*-------------------------------------------------------------------------*/
#define UARTn_BLOCKING_TIMEOUT      (0xFFFFFFFFuL)

//--------------------------------------
// Macro defines for UARTn interrupt enable register
//--------------------------------------
#define UARTn_IER_RBRINT_EN      ((uint8_t)(1 << 0))     /**<  RBR Interrupt enable */
#define UARTn_IER_THREINT_EN     ((uint8_t)(1 << 1))     /**<  THR Interrupt enable */
#define UARTn_IER_RLSINT_EN      ((uint8_t)(1 << 2))     /**<  RX line status interrupt enable */
#define UARTn_IER_TXE_EN         ((uint8_t)(1 << 3))     /**<  TXE interrupt enable */
#define UARTn_IER_BITMASK        ((uint8_t)(0x0F))       /**<  UART interrupt enable register bit mask */

//--------------------------------------
// Macro defines for UARTn interrupt identification register
//--------------------------------------
#define UARTn_IIR_INTSTAT_PEND   ((uint8_t)(1 << 0))     /**<  Interrupt Status - Active low */
#define UARTn_IIR_INTID_RLS      ((uint8_t)(3 << 1))     /**<  Interrupt identification: Receive line status */
#define UARTn_IIR_INTID_RDA      ((uint8_t)(2 << 1))     /**<  Interrupt identification: Receive data available */
#define UARTn_IIR_INTID_THRE     ((uint8_t)(1 << 1))     /**<  Interrupt identification: THRE interrupt */
#define UARTn_IIR_INTID_TXE      ((uint8_t)(1 << 4))     /**<  Interrupt identification: TXE interrupt */
#define UARTn_IIR_INTID_MASK     ((uint8_t)(7 << 1))     /**<  Interrupt identification: Interrupt ID mask */
#define UARTn_IIR_BITMASK        ((uint8_t)(0x1F))       /**<  UART interrupt identification register bit mask */

//--------------------------------------
// Macro defines for UARTn line control register
//--------------------------------------
#define UARTn_LCR_WLEN5          ((uint8_t)(0))          /**<  UART 5 bit data mode */
#define UARTn_LCR_WLEN6          ((uint8_t)(1 << 0))     /**<  UART 6 bit data mode */
#define UARTn_LCR_WLEN7          ((uint8_t)(2 << 0))     /**<  UART 7 bit data mode */
#define UARTn_LCR_WLEN8          ((uint8_t)(3 << 0))     /**<  UART 8 bit data mode */
#define UARTn_LCR_STOPBIT_SEL    ((uint8_t)(1 << 2))     /**<  UART Two Stop Bits Select */
#define UARTn_LCR_PARITY_EN      ((uint8_t)(1 << 3))     /**<  UART Parity Enable */
#define UARTn_LCR_PARITY_ODD     ((uint8_t)(0 << 4))     /**<  UART Odd Parity Select */
#define UARTn_LCR_PARITY_EVEN    ((uint8_t)(1 << 4))     /**<  UART Even Parity Select */
#define UARTn_LCR_PARITY_F_1     ((uint8_t)(2 << 4))     /**<  UART force 1 stick parity */
#define UARTn_LCR_PARITY_F_0     ((uint8_t)(3 << 4))     /**<  UART force 0 stick parity */
#define UARTn_LCR_BREAK_EN       ((uint8_t)(1 << 6))     /**<  UART Transmission Break enable */
#define UARTn_LCR_BITMASK        ((uint8_t)(0x7F))       /**<  UART line control bit mask */

//--------------------------------------
// Macro defines for UARTn data control register
//--------------------------------------
#define UARTn_DCR_TXINV       ((uint8_t)(1 << 2))     /**<  data control register: Tx Data Inversion */
#define UARTn_DCR_RXINV       ((uint8_t)(1 << 3))     /**<  data control register: Rx Data Inversion */
#define UARTn_DCR_LBON        ((uint8_t)(1 << 4))     /**<  data control register: Local loopback test mode */
#define UARTn_DCR_BITMASK     ((uint8_t)(0x7 << 2))   /**<  UART data control bit mask */

//--------------------------------------
// Macro defines for UARTn line status register
//--------------------------------------
#define UARTn_LSR_RDR         ((uint8_t)(1 << 0))     /**<  Line status register: Receive data ready */
#define UARTn_LSR_OE          ((uint8_t)(1 << 1))     /**<  Line status register: Overrun error */
#define UARTn_LSR_PE          ((uint8_t)(1 << 2))     /**<  Line status register: Parity error */
#define UARTn_LSR_FE          ((uint8_t)(1 << 3))     /**<  Line status register: Framing error */
#define UARTn_LSR_BI          ((uint8_t)(1 << 4))     /**<  Line status register: Break interrupt */
#define UARTn_LSR_THRE        ((uint8_t)(1 << 5))     /**<  Line status register: Transmit holding register empty */
#define UARTn_LSR_TEMT        ((uint8_t)(1 << 6))     /**<  Line status register: Transmitter empty */
#define UARTn_LSR_BITMASK     ((uint8_t)(0x7F))       /**<  UART Line status bit mask */

//******************************************************************************
// Type
//******************************************************************************

//==============================================================================
// Enumeration
//==============================================================================

/**   UARTn Data Bit type definitions */
typedef enum
{
   UARTn_DATA_BIT_5 = 0,      /**<  5 Data Bits */
   UARTn_DATA_BIT_6,          /**<  6 Data Bits */
   UARTn_DATA_BIT_7,          /**<  7 Data Bits */
   UARTn_DATA_BIT_8           /**<  8 Data Bits */
} UARTn_DATA_BIT_Type;

/**   UARTn Stop Bit type definitions */
typedef enum
{
   UARTn_STOP_BIT_1 = 0,      /**<  1 Stop Bits */
   UARTn_STOP_BIT_2           /**<  2 Stop Bits */
} UARTn_STOP_BIT_Type;

/**   UARTn Parity Bit type definitions */
typedef enum
{
   UARTn_PARITY_BIT_NONE = 0,    /**<  No parity */
   UARTn_PARITY_BIT_ODD,         /**<  Odd parity */
   UARTn_PARITY_BIT_EVEN,        /**<  Even parity */
   UARTn_PARITY_BIT_SP_1,        /**<  Forced "1" stick parity */
   UARTn_PARITY_BIT_SP_0         /**<  Forced "0" stick parity */
} UARTn_PARITY_BIT_Type;

/**   UARTn Interrupt Type definitions */
typedef enum
{
   UARTn_INTCFG_RBR = 0,    /**<  RBR Interrupt enable */
   UARTn_INTCFG_THRE,       /**<  THR Interrupt enable */
   UARTn_INTCFG_RLS,        /**<  RX line status interrupt enable */
   UARTn_INTCFG_TXE         /**<  TXE interrupt */
} UARTn_INT_Type;

/**   UARTn Data Control type definition */
typedef enum
{
   UARTn_DATA_CONTROL_LOOPBACK = 0,     /**<  Loop back mode select */
   UARTn_DATA_CONTROL_RXINV,
   UARTn_DATA_CONTROL_TXINV,
   UARTn_DATA_CONTROL_RTXINV,
} UARTn_DATA_CONTROL_Type;

//==============================================================================
// Structure
//==============================================================================

/**   UARTn Configuration Structure definition */
typedef struct
{
   uint32_t                Baudrate;     /**<  Baud Rate */
   UARTn_DATA_BIT_Type     Databits;      /**<  Number of Data Bits, should be:
                                                -  UARTn_DATA_BIT_5:    5 Data Bits
                                                -  UARTn_DATA_BIT_6:    6 Data Bits
                                                -  UARTn_DATA_BIT_7:    7 Data Bits
                                                -  UARTn_DATA_BIT_8:    8 Data Bits
                                          */
   UARTn_PARITY_BIT_Type   Parity;        /**<  Number of Parity Bits, should be:
                                                -  UARTn_PARITY_BIT_NONE:  No Parity
                                                -  UARTn_PARITY_BIT_ODD:   Odd Parity
                                                -  UARTn_PARITY_BIT_EVEN:  Even Parity
                                                -  UARTn_PARITY_BIT_SP_1:  Forced "1" Stick Parity
                                                -  UARTn_PARITY_BIT_SP_0:  Forced "0" Stick Parity
                                          */
   UARTn_STOP_BIT_Type     Stopbits;      /**<  Number of Stop Bits, should be:
                                                -  UARTn_STOP_BIT_1:    1 Stop Bits
                                                -  UARTn_STOP_BIT_2:    2 Stop Bits
                                          */
} UARTn_CFG_Type;

//******************************************************************************
// Variable
//******************************************************************************

extern char     InData[80];
extern int      InFlag;
extern int      InCount;

//******************************************************************************
// Function
//******************************************************************************

void UARTn_Init( UARTn_Type* UARTx, UARTn_CFG_Type* UARTn_Config );
void UARTn_DeInit( UARTn_Type* UARTx );
void UARTn_ConfigStructInit( UARTn_CFG_Type* UARTn_Config );

void UARTn_SendByte( UARTn_Type* UARTx, uint8_t Data );
uint8_t UARTn_ReceiveByte( UARTn_Type* UARTx );
uint32_t UARTn_Send( UARTn_Type* UARTx, uint8_t* txbuf, uint32_t buflen, TRANSFER_BLOCK_Type flag );
uint32_t UARTn_Receive( UARTn_Type* UARTx, uint8_t* rxbuf, uint32_t buflen, TRANSFER_BLOCK_Type flag );

void UARTn_IntConfig( UARTn_Type* UARTx, UARTn_INT_Type UARTIntCfg, FunctionalState NewState );
uint8_t UARTn_GetLineStatus( UARTn_Type* UARTx );
FlagStatus UARTn_CheckBusy( UARTn_Type* UARTx );
void UARTn_ForceBreak( UARTn_Type* UARTx );
void UARTn_DataControlConfig( UARTn_Type* UARTx, UARTn_DATA_CONTROL_Type Mode, FunctionalState NewState );
void UARTn_IFDelayConfig( UARTn_Type* UARTx, uint8_t waitval );

#ifdef __cplusplus
}
#endif

#endif   /* _UARTn_H_ */

