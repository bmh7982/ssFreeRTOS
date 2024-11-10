#ifndef  _UART_A31G12x_H
#define  _UART_A31G12x_H

#include <stdint.h> 
#include <stdbool.h> 
#include <string.h> 

//----------------------------------
// define
//----------------------------------
typedef enum
{
   UART_DATA_BIT_5 = 0,      /**<  5 Data Bits */
   UART_DATA_BIT_6,          /**<  6 Data Bits */
   UART_DATA_BIT_7,          /**<  7 Data Bits */
   UART_DATA_BIT_8           /**<  8 Data Bits */
} UART_DATA_BIT_Type;

typedef enum
{
   UART_PARITY_BIT_NONE = 0,    /**<  No parity */
   UART_PARITY_BIT_ODD,         /**<  Odd parity */
   UART_PARITY_BIT_EVEN,        /**<  Even parity */
   UART_PARITY_BIT_SP_1,        /**<  Forced "1" stick parity */
   UART_PARITY_BIT_SP_0         /**<  Forced "0" stick parity */
} UART_PARITY_BIT_Type;

typedef enum
{
   UART_STOP_BIT_1 = 0,      /**<  1 Stop Bits */
   UART_STOP_BIT_2           /**<  2 Stop Bits */
} UART_STOP_BIT_Type;

//--------------------
// Private define
//--------------------
//#define BUF_TYPE 0       // UART_RING_BUFFER_T
#define BUF_TYPE 1       // struct uart_rtx_st

//-----------------------------------------
/*
아래 함수 3개를 
void UartInitPort1(void);
void UartInitBuff (int uart_no, char *rx_buf, int rx_size, char *tx_buf, int tx_size);
void UartOpen(int uart_no, uint32_t Baud_rate, UART_DATA_BIT_Type Databits, UART_PARITY_BIT_Type Parity, UART_STOP_BIT_Type Stopbits);

아래와 같이 수정하면 더 좋을 것 같다.
주의점은 UART_DATA_BIT_5는 0 인데 적용은 5로 되어 있어 mcu마다 함수에세 새로 작성해야하는 번거로움이 있으나

typedef enum
{
   NONE = 0,    // No parity
   ODD,         // Odd parity
   EVEN         // Even parity
}UART_PARITY_Type;

typedef enum
{
   PARITY_NONE = 0,    // No parity
   PARITY_ODD,         // Odd parity
   PARITY_EVEN         // Even parity
}UART_PARITY_Type;

typedef struct
{
	uint32_t          Baudrate;    //  Baud Rate
	uint32_t          Databits;    //  5,6,7,8
	UART_PARITY_Type  Parity;      //  NONE, ODD, EVEN
	uint32_t          Stopbits;    //  1, 2

	char             *rx_buf;
	int               rx_size;
	char             *tx_buf;
	int               tx_size;
} UART_CFG_Type;

usage:
char  uart_rx_buf[32];
char  uart_tx_buf[32];
UART_CFG_Type const uct = {38400,8,NONE,1, uart_rx_buf, sizeof(uart_rx_buf), uart_tx_buf,sixeof(uart_tx_buf)};
void UartInit(int uart_no, &uct);
*/
//-----------------------------------------
void UartInitPort1(void);
void UartInitBuff (int uart_no, char *rx_buf, int rx_size, char *tx_buf, int tx_size);
void UartOpen(int uart_no, uint32_t Baud_rate, UART_DATA_BIT_Type Databits, UART_PARITY_BIT_Type Parity, UART_STOP_BIT_Type Stopbits);

//* Interrupt service routines */
uint32_t UartSend(int uart_no, char* txbuf, uint8_t buflen );        //uint32_t UARTSend( UARTn_Type* UARTx, uint8_t* txbuf, uint8_t buflen );
uint32_t UartReceive(int uart_no, uint8_t* rxbuf, uint8_t buflen );     //uint32_t UARTReceive( UARTn_Type* UARTx, uint8_t* rxbuf, uint8_t buflen );


//----------------------------------------------------------------------------------------------------------------------------------------------------
#endif  //  _UART_A31G12x_H

