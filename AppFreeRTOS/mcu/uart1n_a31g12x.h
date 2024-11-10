#ifndef  _USART_A31G12x_H
#define  _USART_A31G12x_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

//----------------------------------
// define
//----------------------------------
typedef enum
{
   USART_DATA_BIT_5 = 0,       /**<  5 Data Bits */
   USART_DATA_BIT_6,           /**<  6 Data Bits */
   USART_DATA_BIT_7,           /**<  7 Data Bits */
   USART_DATA_BIT_8,           /**<  8 Data Bits */
   USART_DATA_BIT_9 = 7        /**<  9 Data Bits */
} USART_DATA_BIT_Type;

typedef enum
{
   USART_STOP_BIT_1 = 0,       /**<  1 Stop Bits */
   USART_STOP_BIT_2            /**<  2 Stop Bits */
} USART_STOP_BIT_Type;

typedef enum
{
   USART_PARITY_BIT_NONE = 0,     /**<  No parity */
   USART_PARITY_BIT_EVEN = 2,     /**<  Even parity */
   USART_PARITY_BIT_ODD  = 3      /**<  Odd parity */
} USART_PARITY_BIT_Type;

//--------------------
// prototype
//--------------------

void UsartInitPort(int usart_no);
void UsartInitBuff(int uart_no, char *rx_buf, int rx_size, char *tx_buf, int tx_size);
void UsartOpen    (int uno, uint32_t Baud_rate, USART_DATA_BIT_Type Databits, USART_PARITY_BIT_Type Parity, USART_STOP_BIT_Type Stopbits);

void UsartSendBytes(int uart_no, char *buf, int len);
void UsartSendStr  (int uart_no, char *buf);
void UsartSendChar (int uart_no, char  c);

uint32_t UsartReceive(int usart_no, uint8_t* rxbuf, int buflen );

#endif  //  _USART_A31G12x_H

