/***************************************************************************//**
 * @file          debug_frmwrk.h
 * @brief         Driver Header File for Debugging through UART
 * @version       1.0
 * @date          2019-09-01
 * @author        ABOV Application Team
 *
 * Copyright (c) 2019, ABOV Semiconductor. All rights reserved.
 *//***************************************************************************/

#ifndef _DEBUG_FRMWRK_H_
#define _DEBUG_FRMWRK_H_

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Constant
//******************************************************************************

#define USED_UART_DEBUG_PORT     1

#if (USED_UART_DEBUG_PORT == 0)
#define DEBUG_UART_PORT          UART0
#elif (USED_UART_DEBUG_PORT == 1)
#define DEBUG_UART_PORT          UART1
#endif

//******************************************************************************
// Macro
//******************************************************************************

#define _DBG( x )       _db_msg( (UARTn_Type*)DEBUG_UART_PORT, x )
#define _DBG_( x )      _db_msg_( (UARTn_Type*)DEBUG_UART_PORT, x )
#define _DBC( x )       _db_char( (UARTn_Type*)DEBUG_UART_PORT, x )
#define _DBD( x )       _db_dec( (UARTn_Type*)DEBUG_UART_PORT, x )
#define _DBD16( x )     _db_dec_16( (UARTn_Type*)DEBUG_UART_PORT, x )
#define _DBD32( x )     _db_dec_32( (UARTn_Type*)DEBUG_UART_PORT, x )
#define _DBH( x )       _db_hex( (UARTn_Type*)DEBUG_UART_PORT, x )
#define _DBH16( x )     _db_hex_16( (UARTn_Type*)DEBUG_UART_PORT, x )
#define _DBH32( x )     _db_hex_32( (UARTn_Type*)DEBUG_UART_PORT, x )
#define _DG()           _db_get_char( (UARTn_Type*)DEBUG_UART_PORT )
#define _DG_( x )       _db_get_ch( (UARTn_Type*)DEBUG_UART_PORT, x )

//******************************************************************************
// Variable
//******************************************************************************

extern void ( *_db_msg )( UARTn_Type* UARTx, const void* s );
extern void ( *_db_msg_ )( UARTn_Type* UARTx, const void* s );
extern void ( *_db_char )( UARTn_Type* UARTx, uint8_t ch );
extern void ( *_db_dec )( UARTn_Type* UARTx, uint8_t decn );
extern void ( *_db_dec_16 )( UARTn_Type* UARTx, uint16_t decn );
extern void ( *_db_dec_32 )( UARTn_Type* UARTx, uint32_t decn );
extern void ( *_db_hex )( UARTn_Type* UARTx, uint8_t hexn );
extern void ( *_db_hex_16 )( UARTn_Type* UARTx, uint16_t hexn );
extern void ( *_db_hex_32 )( UARTn_Type* UARTx, uint32_t hexn );
extern uint8_t ( *_db_get_char )( UARTn_Type* UARTx );
extern uint8_t ( *_db_get_ch )( UARTn_Type* UARTx, uint8_t* ch );

//******************************************************************************
// Function
//******************************************************************************

void UARTPutChar( UARTn_Type* UARTx, uint8_t ch );
void UARTPuts( UARTn_Type* UARTx, const void* str );
void UARTPuts_( UARTn_Type* UARTx, const void* str );
void UARTPutDec( UARTn_Type* UARTx, uint8_t decnum );
void UARTPutDec16( UARTn_Type* UARTx, uint16_t decnum );
void UARTPutDec32( UARTn_Type* UARTx, uint32_t decnum );
void UARTPutHex( UARTn_Type* UARTx, uint8_t hexnum );
void UARTPutHex16( UARTn_Type* UARTx, uint16_t hexnum );
void UARTPutHex32( UARTn_Type* UARTx, uint32_t hexnum );
uint8_t UARTGetChar( UARTn_Type* UARTx );
uint8_t UARTGetCh( UARTn_Type* UARTx, uint8_t* ch );
void cprintf( const char* format, ... );
void debug_frmwrk_init( void );
uint8_t getstring( void );

#ifdef __cplusplus
}
#endif

#endif   /* _DEBUG_FRMWRK_H_ */

