
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

#include "uart1n_a31g12x.h"

//******************************************************************************
// Constant
//******************************************************************************
//******************************************************************************
// Type
//******************************************************************************

//----------------------------------
// Private define
//----------------------------------
struct uart1n_buf_st{
	unsigned int head;    // Next In Index
	unsigned int tail;    // Next Out Index
	unsigned int cnt;     // buffer data 갯수
	unsigned int stat;    // Tx 인경우 0: idle 1:run  Rx인경우 0: ok, 1: Time Over -> 그런데 사용하느 것이 없다
	unsigned int size;    // buffer size
	char         *buf;    // Buffer Pointer
};

struct uart1n_rtx_st{
	struct uart1n_buf_st rx;
	struct uart1n_buf_st tx;
};

//******************************************************************************
// Variable
//******************************************************************************

//--------------------
// Private variables
//--------------------

USART1n_Type* const usartx[4] = {(USART1n_Type*)USART10, (USART1n_Type*)USART11, (USART1n_Type*)USART12, (USART1n_Type*)USART13};

struct uart1n_rtx_st com1n[4]= {
	{{0,0,0,0,0,0},{0,0,0,0,0,0}},
	{{0,0,0,0,0,0},{0,0,0,0,0,0}},
	{{0,0,0,0,0,0},{0,0,0,0,0,0}},
	{{0,0,0,0,0,0},{0,0,0,0,0,0}}
};

static __IO FlagStatus TxIntStat;
int tint, tint1;

//******************************************************************************
// Function
//******************************************************************************

//=============================================================================
// Private Routine
//=============================================================================
/*
void _UsartSendByte(int uart_no, USART1n_Type *UARTx, char c)
{
	volatile struct buf_st *p = &com1n[uart_no].tx;

	uart_no -=10;
	if(p->size==0) return;
	while(p->cnt==p->size);

	USART1n_IntConfig((USART1n_Type* )UARTx, USART1n_INTCFG_TXC, DISABLE );  //UART_Enable_Tx_Interrupt(UARTx, DISABLE);
	p->buf[p->head] = c;           // Add data to the transmit buffer.
	p->head++;
	if(p->size <= p->head) p->head =0;
	p->cnt++;
	USART1n_IntConfig(( USART1n_Type* )UARTx, USART1n_INTCFG_TXC, ENABLE );  //UART_Enable_Tx_Interrupt(UARTx, ENABLE);
}
*/
//=============================================================================
// Export Function
//=============================================================================
// port 초기화
#if 1

struct usart_port_t {
	Pn_Type*      Px;
	uint8_t       pin_rx;
	uint8_t       pin_tx;

};

struct usart_port_t  const usart_port_tbl[4] = {
	{(Pn_Type*)PB, 1, 0},
	{(Pn_Type*)PD, 3, 2},
	{(Pn_Type*)PE,13,12},
	{(Pn_Type*)PE, 9, 8}
};

void UsartInitPort(int usart_no)
{
	struct usart_port_t const *p;
	Pn_Type*      Px;
	uint8_t       rx;
	uint8_t       tx;

	usart_no -= 10;
	if(usart_no<0 || 3 < usart_no ) return;

	p = &usart_port_tbl[usart_no];
	Px = p->Px;
	rx = p->pin_rx;
	tx = p->pin_tx;

	// RX
	PCU_SetDirection       (Px, rx, ALTERN_FUNC);
	PCU_ConfigureFunction  (Px, rx, AFSRx_AF1 );
	PCU_ConfigurePullupdown(Px, rx, PUPDx_EnablePU );
	// TX
	PCU_SetDirection       (Px, tx, ALTERN_FUNC );
	PCU_ConfigureFunction  (Px, tx, AFSRx_AF1 );
}
/*
void UsartInitPort10()
{
	// Initialize USART10 pin connect
	// RX
	PCU_SetDirection       (( Pn_Type* )PB, 1, ALTERN_FUNC );
	PCU_ConfigureFunction  (( Pn_Type* )PB, 1, AFSRx_AF1 );
	PCU_ConfigurePullupdown(( Pn_Type* )PB, 1, PUPDx_EnablePU );
	// TX
	PCU_SetDirection       (( Pn_Type* )PB, 0, ALTERN_FUNC );
	PCU_ConfigureFunction  (( Pn_Type* )PB, 0, AFSRx_AF1 );
}

void UsartInitPort11()
{
	// Initialize USART11 pin connect
	// RX
	PCU_SetDirection       (( Pn_Type* )PD, 3, ALTERN_FUNC );
	PCU_ConfigureFunction  (( Pn_Type* )PD, 3, AFSRx_AF1 );
	PCU_ConfigurePullupdown(( Pn_Type* )PD, 3, PUPDx_EnablePU );
	// TX
	PCU_SetDirection       (( Pn_Type* )PD, 2, ALTERN_FUNC );
	PCU_ConfigureFunction  (( Pn_Type* )PD, 2, AFSRx_AF1 );
}

void UsartInitPort12()
{
	// Initialize USART11 pin connect
	// RX
	PCU_SetDirection       (( Pn_Type* )PE, 13, ALTERN_FUNC );
	PCU_ConfigureFunction  (( Pn_Type* )PE, 13, AFSRx_AF1 );
	PCU_ConfigurePullupdown(( Pn_Type* )PE, 13, PUPDx_EnablePU );
	// TX
	PCU_SetDirection       (( Pn_Type* )PE, 12, ALTERN_FUNC );
	PCU_ConfigureFunction  (( Pn_Type* )PE, 12, AFSRx_AF1 );
}

void UsartInitPort13()
{
	// Initialize USART11 pin connect
	// RX
	PCU_SetDirection       (( Pn_Type* )PE, 9, ALTERN_FUNC );
	PCU_ConfigureFunction  (( Pn_Type* )PE, 9, AFSRx_AF1 );
	PCU_ConfigurePullupdown(( Pn_Type* )PE, 9, PUPDx_EnablePU );
	// TX
	PCU_SetDirection       (( Pn_Type* )PE, 8, ALTERN_FUNC );
	PCU_ConfigureFunction  (( Pn_Type* )PE, 8, AFSRx_AF1 );
}
*/
//void UsartOpen(int uno, uint32_t Baud_rate, USART1n_DATA_BIT_Type Databits, USART1n_PARITY_BIT_Type Parity, USART1n_STOP_BIT_Type Stopbits)
void UsartOpen(int uno, uint32_t Baud_rate, USART_DATA_BIT_Type Databits, USART_PARITY_BIT_Type Parity, USART_STOP_BIT_Type Stopbits)
{
	USART1n_Type *USART1x;
	//ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
	//if     (uno==10) {USART1x = (USART1n_Type *) USART10;}
	//else if(uno==11) {USART1x = (USART1n_Type *) USART11;}
	//else if(uno==12) {USART1x = (USART1n_Type *) USART12;}
	//else if(uno==13) {USART1x = (USART1n_Type *) USART13;}
	//else return;
	//ttttttttttttttttttttttttttttttttttttttttttttttttttttttttttt
	if(uno<10||13<uno) return;
	USART1x = usartx[uno-10];
	//eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee

	USART1n_CFG_Type USART1n_Config;
		
	USART1n_UART_Mode_ConfigStructInit( &USART1n_Config );
	USART1n_Config.Baudrate = Baud_rate;
	USART1n_Config.Databits = (USART1n_DATA_BIT_Type  ) Databits;
	USART1n_Config.Parity   = (USART1n_PARITY_BIT_Type) Parity;
	USART1n_Config.Stopbits = (USART1n_STOP_BIT_Type  ) Stopbits;
	USART1n_Init(USART1x, &USART1n_Config);

	// Enable UART Rx interrupt
	USART1n_IntConfig( ( USART1n_Type* )USART10, USART1n_INTCFG_RXC, ENABLE );

	if     (USART1x==(USART1n_Type *)USART10) {NVIC_SetPriority( USART10_IRQn, 3 ); NVIC_EnableIRQ( USART10_IRQn ); EInt_MaskDisable( MSK_USART10 );}
	else if(USART1x==(USART1n_Type *)USART11) {NVIC_SetPriority( USART11_IRQn, 3 ); NVIC_EnableIRQ( USART11_IRQn ); EInt_MaskDisable( MSK_USART11 );}
	else if(USART1x==(USART1n_Type *)USART12) {NVIC_SetPriority( USART12_IRQn, 3 ); NVIC_EnableIRQ( USART12_IRQn ); EInt_MaskDisable( MSK_USART12 );}
	else if(USART1x==(USART1n_Type *)USART13) {NVIC_SetPriority( USART13_IRQn, 3 ); NVIC_EnableIRQ( USART13_IRQn ); EInt_MaskDisable( MSK_USART13 );}
	else return;

	USART1n_Enable((USART1n_Type*)USART1x, ENABLE );
}

#else
void Usartn10InitPort(FunctionalState IntState)
{
	// Initialize USART10 pin connect
	PCU_SetDirection       (( Pn_Type* )PB, 1, ALTERN_FUNC );
	PCU_ConfigureFunction  (( Pn_Type* )PB, 1, AFSRx_AF1 );
	PCU_ConfigurePullupdown(( Pn_Type* )PB, 1, PUPDx_EnablePU );
	PCU_SetDirection       (( Pn_Type* )PB, 0, ALTERN_FUNC );
	PCU_ConfigureFunction  (( Pn_Type* )PB, 0, AFSRx_AF1 );

	if(IntState != ENABLE){
		USART1n_Enable( ( USART1n_Type* )USART10, DISABLE );
		return;
	}

	/* Enable UART Rx interrupt */
	USART1n_IntConfig(( USART1n_Type* ) USART10, USART1n_INTCFG_RXC, ENABLE );

	NVIC_SetPriority( USART10_IRQn, 3 );
	NVIC_EnableIRQ  ( USART10_IRQn );
	EInt_MaskDisable( MSK_USART10 );

	USART1n_Enable( ( USART1n_Type* )USART10, ENABLE );
}

void Usartn11InitPort(FunctionalState IntState)
{
	// Initialize USART11 pin connect
	PCU_SetDirection( ( Pn_Type* )PD, 3, ALTERN_FUNC );
	PCU_ConfigureFunction( ( Pn_Type* )PD, 3, AFSRx_AF1 );
	PCU_ConfigurePullupdown( ( Pn_Type* )PD, 3, PUPDx_EnablePU );

	PCU_SetDirection( ( Pn_Type* )PD, 2, ALTERN_FUNC );
	PCU_ConfigureFunction( ( Pn_Type* )PD, 2, AFSRx_AF1 );

	if(IntState != ENABLE){
		USART1n_Enable( ( USART1n_Type* )USART11, DISABLE );
		return;
	}

	/* Enable UART Rx interrupt */
	USART1n_IntConfig( ( USART1n_Type* )USART11, USART1n_INTCFG_RXC, ENABLE );
	NVIC_SetPriority( USART11_IRQn, 3 );
	NVIC_EnableIRQ( USART11_IRQn );
	EInt_MaskDisable( MSK_USART11 );

	USART1n_Enable( ( USART1n_Type* )USART11, ENABLE );

	TxIntStat = RESET;
}

void UsartOpen(USART1n_Type *USART1x, uint32_t Baud_rate, USART1n_DATA_BIT_Type Databits, USART1n_PARITY_BIT_Type Parity, USART1n_STOP_BIT_Type Stopbits)
{
	USART1n_CFG_Type USART1n_Config;
		
	USART1n_UART_Mode_ConfigStructInit( &USART1n_Config );
	USART1n_Config.Baudrate = Baud_rate;
	USART1n_Config.Databits = Databits;
	USART1n_Config.Parity   = Parity;
	USART1n_Config.Stopbits = Stopbits;
	USART1n_Init(USART1x, &USART1n_Config);

	// Enable UART Rx interrupt
	USART1n_IntConfig( ( USART1n_Type* )USART10, USART1n_INTCFG_RXC, ENABLE );

	if     (USART1x==(USART1n_Type *)USART10) {NVIC_SetPriority( USART10_IRQn, 3 ); NVIC_EnableIRQ( USART10_IRQn ); EInt_MaskDisable( MSK_USART10 );}
	else if(USART1x==(USART1n_Type *)USART11) {NVIC_SetPriority( USART11_IRQn, 3 ); NVIC_EnableIRQ( USART11_IRQn ); EInt_MaskDisable( MSK_USART11 );}
	else if(USART1x==(USART1n_Type *)USART12) {NVIC_SetPriority( USART12_IRQn, 3 ); NVIC_EnableIRQ( USART12_IRQn ); EInt_MaskDisable( MSK_USART12 );}
	else if(USART1x==(USART1n_Type *)USART13) {NVIC_SetPriority( USART13_IRQn, 3 ); NVIC_EnableIRQ( USART13_IRQn ); EInt_MaskDisable( MSK_USART13 );}
	else return;

	USART1n_Enable((USART1n_Type*)USART1x, ENABLE );
}
#endif

// buffer 초기화
void UsartInitBuff (int uart_no, char *rx_buf, int rx_size, char *tx_buf, int tx_size)
{
	volatile struct uart1n_buf_st *p;

	uart_no -=10;

	p= &com1n[uart_no].rx;
	p->head = 0;
	p->tail = 0;
	p->cnt  = 0;
	p->stat = 0;
	p->size = rx_size;
	p->buf  = rx_buf;

	p= &com1n[uart_no].tx;
	p->head = 0;
	p->tail = 0;
	p->cnt  = 0;
	p->stat = 0;
	p->size = tx_size;
	p->buf  = tx_buf;
}

//tttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttt
#define __BUF_IS_FULL(head, tail, size)      ((tail==(head+1)) || ((tail==0)&&(head==size)))
#define __BUF_IS_EMPTY(head, tail)           ( head==tail)
#define __BUF_INCR(bufidx, size)         do{if(size<=++bufidx) bufidx=0;} while(0)

void UST_IntTransmit(int uno, USART1n_Type *USARTx);

//uint32_t USTSend( USART1n_Type* USARTx, int usart_no, uint8_t* txbuf, uint8_t buflen )
uint32_t USTSend(int usart_no, uint8_t* txbuf, uint8_t buflen )
{
	int uno;
	struct uart1n_buf_st *p;
	USART1n_Type *USARTx;

	uno =  usart_no - 10;

	tint  = usart_no;
	tint1 = *txbuf;

	if(uno<0||3<uno) return 0;
	USARTx = usartx[uno];

//	USART1n_Type *USARTx;
//	if     (uno==10) {USARTx = (USART1n_Type *) USART10;}
//	else if(uno==11) {USARTx = (USART1n_Type *) USART11;}
//	else if(uno==12) {USARTx = (USART1n_Type *) USART12;}
//	else if(uno==13) {USARTx = (USART1n_Type *) USART13;}
//	else return 0;

	p= &com1n[uno].tx;
	uint8_t* data = (uint8_t*) txbuf;
	uint32_t bytes = 0;

   /* Temporarily lock out UART transmit interrupts during this
      read so the UART transmit interrupt won't cause problems
      with the index values */
	USART1n_IntConfig(USARTx, USART1n_INTCFG_DR, DISABLE );   //USART1n_IntConfig(USART1x, USART1n_INTCFG_TXC, DISABLE );

   /* Loop until transmit run buffer is full or until n_bytes
      expires */
	while((0<buflen) && ( !__BUF_IS_FULL( p->head, p->tail, p->size)))
	//while((0<buflen) && !((p->tail == p->head+1) || ((p->tail==0)&&(p->head==p->size))))
   {
      /* Write data from buffer into ring buffer */
      p->buf[p->head] = *data;
      data++;

      /* Increment head pointer */
      __BUF_INCR(p->head, p->size);

      /* Increment data count and decrement buffer size count */
      bytes++;
      buflen--;
   }

   /*
    * Check if current Tx interrupt enable is reset,
    * that means the Tx interrupt must be re-enabled
    * due to call UST_IntTransmit() function to trigger
    * this interrupt type
    */
   if( TxIntStat == RESET )
   {
      UST_IntTransmit(uno, USARTx);
   }
   /*
    * Otherwise, re-enables Tx Interrupt
    */
   else
   {
	  USART1n_IntConfig(USARTx, USART1n_INTCFG_DR, ENABLE );  //USART1n_IntConfig( USART1x, USART1n_INTCFG_TXC, ENABLE );
   }

   return bytes;
}
//eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee

//----------------------------------
// Uart Send
void _UsartSendByte(int uart_no, USART1n_Type *UARTx, char c)
{
	volatile struct uart1n_buf_st *p = &com1n[uart_no].tx;

	if(p->size==0) return;
	while(p->cnt==p->size);

//	//USART1n_IntConfig((USART1n_Type* )UARTx, USART1n_INTCFG_TXC, DISABLE );  //UART_Enable_Tx_Interrupt(UARTx, DISABLE);
//	USART1n_IntConfig((USART1n_Type* )UARTx, USART1n_INTCFG_DR, DISABLE );  //UART_Enable_Tx_Interrupt(UARTx, DISABLE);
	p->buf[p->head] = c;           // Add data to the transmit buffer.
	p->head++;
	if(p->size <= p->head) p->head =0;
	p->cnt++;
	if(p->cnt==1){
		USART1n_IntConfig(( USART1n_Type* )UARTx, USART1n_INTCFG_DR, ENABLE );  //UART_Enable_Tx_Interrupt(UARTx, ENABLE);
	}
//	USART1n_IntConfig(( USART1n_Type* )UARTx, USART1n_INTCFG_DR, ENABLE );  //UART_Enable_Tx_Interrupt(UARTx, ENABLE);
//	//USART1n_IntConfig(( USART1n_Type* )UARTx, USART1n_INTCFG_TXC, ENABLE );  //UART_Enable_Tx_Interrupt(UARTx, ENABLE);
}

void UsartSendBytes(int usart_no, char *buf, int len)
{
	USART1n_Type    *USARTx;
	volatile struct uart1n_buf_st *p;
	int uno = usart_no - 10;

	if(uno<0||3<uno) return;
	USARTx = usartx[uno];
	//if      (usart_no == 10) USARTx = (USART1n_Type*)USART10;
	//else if (usart_no == 11) USARTx = (USART1n_Type*)USART11;
	//else if (usart_no == 12) USARTx = (USART1n_Type*)USART12;
	//else if (usart_no == 13) USARTx = (USART1n_Type*)USART13;
	//else return;

	//ttttttttttttttttttttttttttttttttttttttttttttttttt
	//for(; 0<len; len--){
	//	  _UsartSendByte(uart_no, UARTx, *(buf++));
	//}
	//ooooooooooooooooooooooooooooooooooooooooooooooo
	p = &com1n[uno].tx;
	if(p->size==0) return;
	USART1n_IntConfig(USARTx, USART1n_INTCFG_DR, DISABLE );   //USART1n_IntConfig( USART1x, USART1n_INTCFG_TXC, DISABLE );
	for(; 0<len; len--){
		if(p->cnt==p->size){
			USART1n_IntConfig(USARTx, USART1n_INTCFG_DR, ENABLE );
			while(p->cnt==p->size);
			USART1n_IntConfig(USARTx, USART1n_INTCFG_DR, DISABLE );
		}
		p->buf[p->head] = *(buf++);           // Add data to the transmit buffer.
		p->head++;
		if(p->size <= p->head) p->head =0;
		p->cnt++;
	}
	USART1n_IntConfig(USARTx, USART1n_INTCFG_DR, ENABLE );
  //UART_Enable_Tx_Interrupt(UARTx, ENABLE);
	//eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee
}
#if 0
  void UsartSendStr(int uart_no, char *buf)
  {
	UsartSendBytes(uart_no, buf, strlen(buf));
  }

  void UsartSendChar(int uart_no, char c)
  {
	UsartSendBytes(uart_no, &c, 1);
  }
#else
  void UsartSendStr(int usart_no, char *buf)
  {
	USTSend(usart_no, (uint8_t *)buf, strlen(buf));

  //	int uno = usart_no -10;
  //	if(uno<0||3<uno) return;
  //
  //	USTSend(usartx[uno], uno, (uint8_t *)buf, strlen(buf));
  //
  //	//if     (uno==10) {USTSend((USART1n_Type *) USART10, uno, (uint8_t *)buf, strlen(buf));}
  //	//else if(uno==11) {USTSend((USART1n_Type *) USART11, uno, (uint8_t *)buf, strlen(buf));}
  //	//else if(uno==12) {USTSend((USART1n_Type *) USART12, uno, (uint8_t *)buf, strlen(buf));}
  //	//else if(uno==13) {USTSend((USART1n_Type *) USART13, uno, (uint8_t *)buf, strlen(buf));}
  //	//else return;
  }

  void UsartSendChar(int usart_no, char c)
  {
	USTSend(usart_no, (uint8_t *)&c, 1);
  //	int uno = usart_no -10;
  //	if(uno<0||3<uno) return;
  //
  //	USTSend(usartx[uno], uno, (uint8_t *)&c, 1);
  //
  //	//if     (uno==10) {USTSend((USART1n_Type *) USART10, uno, (uint8_t *)&c, 1);}
  //	//else if(uno==11) {USTSend((USART1n_Type *) USART11, uno, (uint8_t *)&c, 1);}
  //	//else if(uno==12) {USTSend((USART1n_Type *) USART12, uno, (uint8_t *)&c, 1);}
  //	//else if(uno==13) {USTSend((USART1n_Type *) USART13, uno, (uint8_t *)&c, 1);}
  //	//else return;
  }
#endif
//=============================================================================
// Driver Routine
//=============================================================================
/*-------------------------------------------------------------------------*//**
 * @brief         USART receive function (ring buffer used)
 * @return        none
 *//*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*//**
 * @brief         UART read function for interrupt mode (using ring buffers)
 * @param[in]     USART1x
 *                   Pointer to a target peripheral.
 *                   -  UART1
 * @param[out]    rxbuf
 *                   Pointer to Received buffer
 * @param[in]     buflen
 *                   Length of Received buffer
 * @return        Number of bytes actually read from the ring buffer
 *//*-------------------------------------------------------------------------*/
uint32_t USTReceive( USART1n_Type* USART1x, int uno, uint8_t* rxbuf, int buflen )
{
	uint32_t bytes = 0;
   uint8_t* data = ( uint8_t* ) rxbuf;
	volatile struct uart1n_buf_st *p;
	p = &com1n[uno].rx;

	// Temporarily lock out UART receive interrupts during this
	//   read so the UART receive interrupt won't cause problems
	//   with the index values
	USART1n_IntConfig( USART1x, USART1n_INTCFG_RXC, DISABLE );

	// Loop until receive buffer ring is empty or until max_bytes expires
	while((0<buflen) && ( !(__BUF_IS_EMPTY( p->head, p->tail ) ) ) ) {
      // Read data from ring buffer into user buffer
      *data = p->buf[p->tail];
      data++;

      // Update tail pointer
      __BUF_INCR( p->tail, p->size );

      // Increment data count and decrement buffer size count
      bytes++;
      buflen--;
   }

   // Re-enable UART interrupts
   USART1n_IntConfig( USART1x, USART1n_INTCFG_RXC, ENABLE );

   return bytes;
}

uint32_t UsartReceive(int usart_no, uint8_t* buf, int buflen )
{
	int uno = usart_no-10;

	{
	  if     (usart_no==10) {return USTReceive((USART1n_Type *) USART10, uno, (uint8_t *)buf, buflen);}
	  else if(usart_no==11) {return USTReceive((USART1n_Type *) USART11, uno, (uint8_t *)buf, buflen);}
	  else if(usart_no==12) {return USTReceive((USART1n_Type *) USART12, uno, (uint8_t *)buf, buflen);}
	  else if(usart_no==13) {return USTReceive((USART1n_Type *) USART13, uno, (uint8_t *)buf, buflen);}
	  else return 0;
    }
}
/*-------------------------------------------------------------------------*//**
 * @brief         UART transmit function (ring buffer used)
 * @return        none
 *//*-------------------------------------------------------------------------*/
void UST_IntReceive(USART1n_Type* USART1x, int uno)
{
	uint8_t tmpc;
	uint32_t rLen;
	volatile struct uart1n_buf_st *p;
	p = &com1n[uno].rx;

	while(1){
      // Call UART read function in UART driver
      rLen = USART1n_Receive(USART1x, &tmpc, 1, NONE_BLOCKING );
      // If data received
      if(rLen) {
         /* Check if buffer is more space
          * If no more space, remaining character will be trimmed out
          */
         if(!__BUF_IS_FULL(p->head, p->tail, p->size)) {
            p->buf[p->head] = tmpc;
            __BUF_INCR(p->head, p->size);
         }
      }
      else {
	    // no more data
	    break;
      }
   }

/*
 	char                           c;
	volatile struct uart1n_buf_st *p;

	p =  &com1n[uno].rx;
	c = USART1n_ReceiveByte(USART1x);
	if(p->size<=p->cnt) return;
	p->cnt++;

	p->buf[p->head] = c;
	p->head++;
	if(p->size<=p->head) p->head=0;
*/
}

void UST_IntTransmit(int uno, USART1n_Type* USARTx)
{
	volatile struct uart1n_buf_st *p;

	tint = uno;
	if(uno<0 || 3<uno) return;

	p = &com1n[uno].tx;

	// Disable THRE interrupt
	USART1n_IntConfig(USARTx, USART1n_INTCFG_DR, DISABLE );   //USART1n_IntConfig(USART1x, USART1n_INTCFG_TXC, DISABLE );

	/* Wait until THR empty */
	while( USART1n_CheckBusy(USARTx) == SET );

	while(!__BUF_IS_EMPTY( p->head, p->tail)){
	  //if(p->size<=p->head)
	  // Move a piece of data into the transmit FIFO
      if( USART1n_Send(USARTx, ( uint8_t* )&p->buf[p->tail], 1, NONE_BLOCKING ) ) {
         __BUF_INCR(p->tail, p->size);   // Update transmit ring FIFO tail pointer
      }
      else
      {
         break;
      }
   }

   /* If there is no more data to send, disable the transmit
      interrupt - else enable it or keep it enabled */
   if( __BUF_IS_EMPTY( p->head, p->tail ) )
   {
      USART1n_IntConfig(USARTx, USART1n_INTCFG_TXC, DISABLE );
      // Reset Tx Interrupt state
      TxIntStat = RESET;
   }
   else
   {
      // Set Tx Interrupt state
      TxIntStat = SET;
      USART1n_IntConfig(USARTx, USART1n_INTCFG_DR, ENABLE );
   }
}


void USART1n_Handler(USART1n_Type* USARTx, int uno)
{
   uint32_t intsrc, tmp;

   /* Determine the interrupt source */
   intsrc = USART1n_GetStatus(USARTx);
   tmp = intsrc & USART1n_SR_BITMASK;

   // Receive Data Available or Character time-out
   if( ( tmp & USART1n_SR_RXC ) == USART1n_SR_RXC ){
      UST_IntReceive(USARTx, uno);
   }

   // Transmit Holding Empty
   //if( ( tmp & USART1n_SR_TXC ) == USART1n_SR_TXC ){
   if( ( tmp & USART1n_SR_DRE ) == USART1n_SR_DRE ){
      UST_IntTransmit(uno, USARTx);
   }
}

void USART10_Handler( void )
{
	USART1n_Handler((USART1n_Type*) USART10, 0);
}

void USART11_Handler( void )
{
	USART1n_Handler((USART1n_Type*) USART11, 1);
}

void USART12_Handler( void )
{
	USART1n_Handler((USART1n_Type*) USART12, 2);
}

void USART13_Handler( void )
{
	USART1n_Handler((USART1n_Type*) USART13, 3);
}
