
//--------------------
// Includes
//--------------------
#include <string.h>
#include <stdbool.h>

#include "A31G12x.h"
#include "aa_types.h"

#include "A31G12x_PCU.h"
#include "A31G12x_UARTn.h"
#include "A31G12x_INTC.h"

#include "uart_a31g12x.h"

//******************************************************************************
// Constant
//******************************************************************************
#if (BUF_TYPE == 0)
  /* buffer size definition */
  #define UART_RING_BUFSIZE 32

  /* Buf mask */
  #define __BUF_MASK                     (UART_RING_BUFSIZE-1)
  /* Check buf is full or not */
  #define __BUF_IS_FULL(head, tail)      ((tail&__BUF_MASK)==((head+1)&__BUF_MASK))
  /* Check buf will be full in next receiving or not */
  #define __BUF_WILL_FULL(head, tail)    ((tail&__BUF_MASK)==((head+2)&__BUF_MASK))
  /* Check buf is empty */
  #define __BUF_IS_EMPTY(head, tail)     ((head&__BUF_MASK)==(tail&__BUF_MASK))
  /* Reset buf */
  #define __BUF_INCR(bufidx)             (bufidx=(bufidx+1)&__BUF_MASK)
#else
  #define __BUF_IS_FULL(head, tail, size)      ((tail==(head+1)) || ((tail==0)&&(head==size)))
  #define __BUF_IS_EMPTY(head, tail)           ( head==tail)
  #define __BUF_INCR(bufidx, size)           do{if(size<=++bufidx) bufidx = 0;}while(0)
#endif
//******************************************************************************
// Type
//******************************************************************************
//
#if (BUF_TYPE == 0)
  typedef struct
  {
     uint32_t tx_head;                /**<  UART Tx ring buffer head index */
     uint32_t tx_tail;                /**<  UART Tx ring buffer tail index */
     uint32_t rx_head;                /**<  UART Rx ring buffer head index */
     uint32_t rx_tail;                /**<  UART Rx ring buffer tail index */
     uint8_t  tx[UART_RING_BUFSIZE];  /**<  UART Tx data ring buffer */
     uint8_t  rx[UART_RING_BUFSIZE];  /**<  UART Rx data ring buffer */
  } UART_RING_BUFFER_T;
#else
  struct uart_buf_st {
	unsigned int head;    // Next In Index
	unsigned int tail;    // Next Out Index
	unsigned int cnt;     // buffer data 갯수
	unsigned int stat;    // Tx 인경우 0: idle 1:run  Rx인경우 0: ok, 1: Time Over -> 그런데 사용하느 것이 없다
	unsigned int size;    // buffer size
	char         *buf;    // Buffer Pointer
  };

  struct uart_rtx_st{
	struct uart_buf_st rx;
	struct uart_buf_st tx;
  };
#endif

//******************************************************************************
// Variable
//******************************************************************************

// UART Ring buffer
#if BUF_TYPE == 0
  UART_RING_BUFFER_T rb;
#else 
  volatile struct uart_rtx_st rb = {
	{0,0,0,0,0,0},{0,0,0,0,0,0}
  };
#endif

// Current Tx Interrupt enable state
static __IO FlagStatus TxIntStat = RESET;

/* Interrupt service routines */
void UART_IntErr( uint8_t bLSErrType );
void UART_IntTransmit( void );
void UART_IntReceive( void );

uint32_t UARTReceive( UARTn_Type* UARTx, uint8_t* rxbuf, uint8_t buflen );
//=============================================================================
// Export Function
//=============================================================================
// port 초기화
#define  Px         ((Pn_Type     *)PB)
#define  UARTx      ((UARTn_Type  *)UART1)
#define  IRQn       ( UART1_IRQn )
#define  MSK_UARTx  ( MSK_UART1  )
void UartInitPort1(void)
{
	UARTn_DeInit(UARTx);

	// Initialize USART10 pin connect
	PCU_SetDirection       (Px, 7, ALTERN_FUNC    );
	PCU_ConfigureFunction  (Px, 7, AFSRx_AF1      );
	PCU_ConfigurePullupdown(Px, 7, PUPDx_EnablePU );
	PCU_SetDirection       (Px, 6, ALTERN_FUNC );
	PCU_ConfigureFunction  (Px, 6, AFSRx_AF1 );

	UARTn_IntConfig(UARTx, UARTn_INTCFG_RBR, ENABLE );   // Enable UART Rx interrupt
  //UARTn_IntConfig(UARTx, UARTn_INTCFG_RLS, ENABLE );   // Enable UART line status interrupt

	NVIC_SetPriority(IRQn, 3   );
	NVIC_EnableIRQ  (IRQn      );
	EInt_MaskDisable(MSK_UARTx );

	TxIntStat = RESET;
}
#undef  Px
#undef  UARTx
#undef  IRQn
#undef  MSK_UARTx


#if BUF_TYPE == 0
  void UartInitBuff (int uart_no, char *rx_buf, int rx_size, char *tx_buf, int tx_size)
  {
	rb.rx_head = 0;
	rb.rx_tail = 0;
	rb.tx_head = 0;
	rb.tx_tail = 0;
  }  
#else 
  void UartInitBuff (int uart_no, char *rx_buf, int rx_size, char *tx_buf, int tx_size)
  {
	volatile struct uart_buf_st *p;

	p= &rb.rx;
	p->head = 0;
	p->tail = 0;
	p->cnt  = 0;
	p->stat = 0;
	p->size = rx_size;
	p->buf  = rx_buf;

	p= &rb.tx;
	p->head = 0;
	p->tail = 0;
	p->cnt  = 0;
	p->stat = 0;
	p->size = tx_size;
	p->buf  = tx_buf;
  }
#endif


void UartOpen(int uart_no, uint32_t Baud_rate, UART_DATA_BIT_Type Databits, UART_PARITY_BIT_Type Parity, UART_STOP_BIT_Type Stopbits)
{
	UARTn_CFG_Type UARTn_Config;
	UARTn_Type  *UARTx;

	UARTx = (UARTn_Type  *) UART1;
	if ( uart_no != 1) return;

	TxIntStat = RESET;

	UARTn_ConfigStructInit( &UARTn_Config );  // 참고용
	UARTn_Config.Baudrate = Baud_rate;
	UARTn_Config.Databits = (UARTn_DATA_BIT_Type) Databits;
	UARTn_Config.Parity   = (UARTn_PARITY_BIT_Type) Parity;
	UARTn_Config.Stopbits = (UARTn_STOP_BIT_Type) Stopbits;
	UARTn_Init(UARTx, &UARTn_Config);
}

/*-------------------------------------------------------------------------*//**
 * @brief         UART transmit function for interrupt mode (using ring buffers)
 * @param[in]     UARTx
 *                   Pointer to a target peripheral.
 *                   -  UART1
 * @param[in]     txbuf
 *                   Pointer to Transmit buffer
 * @param[in]     buflen
 *                   Length of Transmit buffer
 * @return        Number of bytes actually sent to the ring buffer
 *//*-------------------------------------------------------------------------*/

#if BUF_TYPE == 0
  //if BUF_TYPE == 0 -------------------------------------------------------------------
  uint32_t UARTSend( UARTn_Type* UARTx, char* txbuf, uint8_t buflen )
  {
	uint8_t* data = ( uint8_t* ) txbuf;
	uint32_t bytes = 0;

	/* Temporarily lock out UART transmit interrupts during this
	  read so the UART transmit interrupt won't cause problems
	  with the index values */
	UARTn_IntConfig( UARTx, UARTn_INTCFG_THRE, DISABLE );

	/* Loop until transmit run buffer is full or until n_bytes
	  expires */
	while( ( buflen > 0 ) && ( !__BUF_IS_FULL( rb.tx_head, rb.tx_tail ) ) ){
	  /* Write data from buffer into ring buffer */
	  rb.tx[rb.tx_head] = *data;
	  data++;

	  /* Increment head pointer */
	  __BUF_INCR( rb.tx_head );

	  /* Increment data count and decrement buffer size count */
	  bytes++;
	  buflen--;
	}

	/*
	* Check if current Tx interrupt enable is reset,
	* that means the Tx interrupt must be re-enabled
	* due to call UART_IntTransmit() function to trigger
	* this interrupt type
	*/
	if( TxIntStat == RESET ){
	  UART_IntTransmit();
	}
	else {
	  // Otherwise, re-enables Tx Interrupt
	  UARTn_IntConfig( UARTx, UARTn_INTCFG_THRE, ENABLE );
	}

	return bytes;
}

  /*-------------------------------------------------------------------------*//**
   * @brief         UART read function for interrupt mode (using ring buffers)
   * @param[in]     UARTx
   *                   Pointer to a target peripheral.
   *                   -  UART1
   * @param[out]    rxbuf
   *                   Pointer to Received buffer
   * @param[in]     buflen
   *                   Length of Received buffer
   * @return        Number of bytes actually read from the ring buffer
   *//*-------------------------------------------------------------------------*/
  uint32_t UARTReceive( UARTn_Type* UARTx, uint8_t* rxbuf, uint8_t buflen )
  {
   uint8_t* data = ( uint8_t* ) rxbuf;
   uint32_t bytes = 0;

   /* Temporarily lock out UART receive interrupts during this
      read so the UART receive interrupt won't cause problems
      with the index values */
   UARTn_IntConfig( UARTx, UARTn_INTCFG_RBR, DISABLE );

   /* Loop until receive buffer ring is empty or
      until max_bytes expires */
   while( ( buflen > 0 ) && ( !( __BUF_IS_EMPTY( rb.rx_head, rb.rx_tail ) ) ) )
   {
      /* Read data from ring buffer into user buffer */
      *data = rb.rx[rb.rx_tail];
      data++;

      /* Update tail pointer */
      __BUF_INCR( rb.rx_tail );

      /* Increment data count and decrement buffer size count */
      bytes++;
      buflen--;
   }

   /* Re-enable UART interrupts */
   UARTn_IntConfig( UARTx, UARTn_INTCFG_RBR, ENABLE );

   return bytes;
  }

#else 
  //else // if BUF_TYPE == 0 -------------------------------------------------------------------
  uint32_t UARTSend( UARTn_Type* UARTx, char* txbuf, uint8_t buflen )
  {
	uint8_t* data = ( uint8_t* ) txbuf;
	uint32_t bytes = 0;

	/* Temporarily lock out UART transmit interrupts during this
	  read so the UART transmit interrupt won't cause problems
	  with the index values */
	UARTn_IntConfig( UARTx, UARTn_INTCFG_THRE, DISABLE );

	/* Loop until transmit run buffer is full or until n_bytes
	  expires */
	while( ( buflen > 0 ) && ( !__BUF_IS_FULL( rb.tx.head, rb.tx.tail, rb.tx.size ) ) )
	{
	  /* Write data from buffer into ring buffer */
	  rb.tx.buf[rb.tx.head] = *data;
	  data++;

	  /* Increment head pointer */
	  __BUF_INCR( rb.tx.head, rb.tx.size );

	  /* Increment data count and decrement buffer size count */
	  bytes++;
	  buflen--;
	}

   /*
	* Check if current Tx interrupt enable is reset,
	* that means the Tx interrupt must be re-enabled
	* due to call UART_IntTransmit() function to trigger
	* this interrupt type
	*/
	if( TxIntStat == RESET ) {
	  UART_IntTransmit();
	}
   /*
	* Otherwise, re-enables Tx Interrupt
	*/
	else{
	  UARTn_IntConfig( UARTx, UARTn_INTCFG_THRE, ENABLE );
	}
	return bytes;
}

uint32_t UartSend(int uart_no, char* txbuf, uint8_t buflen )
{
	if(uart_no != 1) return 0;
	return UARTSend((UARTn_Type*) UART1, txbuf, buflen);
}

/*-------------------------------------------------------------------------*//**
 * @brief         UART read function for interrupt mode (using ring buffers)
 * @param[in]     UARTx
 *                   Pointer to a target peripheral.
 *                   -  UART1
 * @param[out]    rxbuf
 *                   Pointer to Received buffer
 * @param[in]     buflen
 *                   Length of Received buffer
 * @return        Number of bytes actually read from the ring buffer
 *//*-------------------------------------------------------------------------*/
uint32_t UARTReceive( UARTn_Type* UARTx, uint8_t* rxbuf, uint8_t buflen )
{
   uint8_t* data = ( uint8_t* ) rxbuf;
   uint32_t bytes = 0;

   /* Temporarily lock out UART receive interrupts during this
      read so the UART receive interrupt won't cause problems
      with the index values */
   UARTn_IntConfig( UARTx, UARTn_INTCFG_RBR, DISABLE );

   /* Loop until receive buffer ring is empty or
      until max_bytes expires */
   while( ( buflen > 0 ) && ( !( __BUF_IS_EMPTY( rb.rx.head, rb.rx.tail))))
   {
      /* Read data from ring buffer into user buffer */
      *data = rb.rx.buf[rb.rx.tail];
      data++;

      /* Update tail pointer */
      __BUF_INCR(rb.rx.tail, rb.rx.size);

      /* Increment data count and decrement buffer size count */
      bytes++;
      buflen--;
   }

   /* Re-enable UART interrupts */
   UARTn_IntConfig( UARTx, UARTn_INTCFG_RBR, ENABLE );

   return bytes;
}

uint32_t UartReceive(int uart_no, uint8_t* rxbuf, uint8_t buflen )
{
	if(uart_no != 1) return 0;
	return UARTReceive((UARTn_Type*) UART1, rxbuf, buflen);
}

#endif  //end  // if BUF_TYPE == 0 -------------------------------------------------------------------

//=============================================================================
// IRQHandler
//=============================================================================

#if BUF_TYPE == 0

/*-------------------------------------------------------------------------*//**
 * @brief         UART receive function (ring buffer used)
 * @return        none
 *//*-------------------------------------------------------------------------*/
void UART_IntReceive( void )
{
   uint8_t tmpc;
   uint32_t rLen;

   while( 1 )
   {
      // Call UART read function in UART driver
      rLen = UARTn_Receive( ( UARTn_Type* )UART1, &tmpc, 1, NONE_BLOCKING );
      // If data received
      if( rLen )
      {
         /* Check if buffer is more space
          * If no more space, remaining character will be trimmed out
          */
         if( !__BUF_IS_FULL( rb.rx_head, rb.rx_tail ) )
         {
            rb.rx[rb.rx_head] = tmpc;
            __BUF_INCR( rb.rx_head );
         }
      }
      // no more data
      else
      {
         break;
      }
   }
}

/*-------------------------------------------------------------------------*//**
 * @brief         UART transmit function (ring buffer used)
 * @return        none
 *//*-------------------------------------------------------------------------*/
void UART_IntTransmit( void )
{
   // Disable THRE interrupt
   UARTn_IntConfig( ( UARTn_Type* )UART1, UARTn_INTCFG_THRE, DISABLE );

   /* Wait until THR empty */
   while( UARTn_CheckBusy( ( UARTn_Type* )UART1 ) == SET );

   while( !__BUF_IS_EMPTY( rb.tx_head, rb.tx_tail ) )
   {
      /* Move a piece of data into the transmit FIFO */
      if( UARTn_Send( ( UARTn_Type* )UART1, ( uint8_t* )&rb.tx[rb.tx_tail], 1, NONE_BLOCKING ) )
      {
         /* Update transmit ring FIFO tail pointer */
         __BUF_INCR( rb.tx_tail );
      }
      else
      {
         break;
      }
   }

   /* If there is no more data to send, disable the transmit
      interrupt - else enable it or keep it enabled */
   if( __BUF_IS_EMPTY( rb.tx_head, rb.tx_tail ) )
   {
      UARTn_IntConfig( ( UARTn_Type* )UART1, UARTn_INTCFG_THRE, DISABLE );
      // Reset Tx Interrupt state
      TxIntStat = RESET;
   }
   else
   {
      // Set Tx Interrupt state
      TxIntStat = SET;
      UARTn_IntConfig( ( UARTn_Type* )UART1, UARTn_INTCFG_THRE, ENABLE );
   }
}

#else 
/*-------------------------------------------------------------------------*//**
 * @brief         UART receive function (ring buffer used)
 * @return        none
 *//*-------------------------------------------------------------------------*/
void UART_IntReceive( void )
{
   uint8_t tmpc;
   uint32_t rLen;

   while( 1 )
   {
      // Call UART read function in UART driver
      rLen = UARTn_Receive( ( UARTn_Type* )UART1, &tmpc, 1, NONE_BLOCKING );
      // If data received
      if( rLen )
      {
         /* Check if buffer is more space
          * If no more space, remaining character will be trimmed out
          */
         if( !__BUF_IS_FULL( rb.rx.head, rb.rx.tail, rb.rx.size ) )
         {
            rb.rx.buf[rb.rx.head] = tmpc;
            __BUF_INCR(rb.rx.head, rb.rx.size);
         }
      }
      // no more data
      else
      {
         break;
      }
   }
}

/*-------------------------------------------------------------------------*//**
 * @brief         UART transmit function (ring buffer used)
 * @return        none
 *//*-------------------------------------------------------------------------*/
void UART_IntTransmit( void )
{
   // Disable THRE interrupt
   UARTn_IntConfig( ( UARTn_Type* )UART1, UARTn_INTCFG_THRE, DISABLE );

   /* Wait until THR empty */
   while( UARTn_CheckBusy( ( UARTn_Type* )UART1 ) == SET );

   while( !__BUF_IS_EMPTY( rb.tx.head, rb.tx.tail ) )
   {
      /* Move a piece of data into the transmit FIFO */
      if( UARTn_Send( ( UARTn_Type* )UART1, ( uint8_t* )&rb.tx.buf[rb.tx.tail], 1, NONE_BLOCKING ) )
      {
         /* Update transmit ring FIFO tail pointer */
         __BUF_INCR( rb.tx.tail, rb.tx.size );
      }
      else
      {
         break;
      }
   }

   /* If there is no more data to send, disable the transmit
      interrupt - else enable it or keep it enabled */
   if( __BUF_IS_EMPTY( rb.tx.head, rb.tx.tail ) )
   {
      UARTn_IntConfig( ( UARTn_Type* )UART1, UARTn_INTCFG_THRE, DISABLE );
      // Reset Tx Interrupt state
      TxIntStat = RESET;
   }
   else
   {
      // Set Tx Interrupt state
      TxIntStat = SET;
      UARTn_IntConfig( ( UARTn_Type* )UART1, UARTn_INTCFG_THRE, ENABLE );
   }
}
#endif

/*-------------------------------------------------------------------------*//**
 * @brief         UART Line Status Error
 * @param[in]     bLSErrType
 *                UART Line Status Error Type
 * @return        none
 *//*-------------------------------------------------------------------------*/
// 무한루프가 되어서는 안된다. xxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void UART_IntErr( uint8_t bLSErrType )
{
   uint8_t test;
   // Loop forever
   while( 0 )    //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
   {
      // For testing purpose
      test = bLSErrType;
      test = test;
   }
}

/*----------------- INTERRUPT SERVICE ROUTINES ----------------------------
 * @brief         UART1 interrupt handler sub-routine
 * @return        none
 *-------------------------------------------------------------------------*/
void UART1_Handler( void )
{
   uint32_t intsrc, tmp, tmp1;

   /* Determine the interrupt source */
   intsrc = UART1->IIR;
   tmp = intsrc & UARTn_IIR_INTID_MASK;

   // Receive Line Status
   if( tmp == UARTn_IIR_INTID_RLS )
   {
      // Check line status
      tmp1 = UART1->LSR;
      // Mask out the Receive Ready and Transmit Holding empty status
      tmp1 &= ( UARTn_LSR_OE | UARTn_LSR_PE | UARTn_LSR_FE | UARTn_LSR_BI );
      // If any error exist
      if( tmp1 )
      {
         UART_IntErr( tmp1 );
      }
   }

   // Receive Data Available or Character time-out
   if( tmp == UARTn_IIR_INTID_RDA )
   {
      UART_IntReceive();
   }

   // Transmit Holding Empty
   if( tmp == UARTn_IIR_INTID_THRE )
   {
      UART_IntTransmit();
   }
}
