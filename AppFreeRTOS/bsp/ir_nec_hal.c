
//==============================================================================
// Include
//==============================================================================
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#include "A31G12x.h"
#include "aa_types.h"

#include "A31G12x_PCU.h"
#include "A31G12x_INTC.h"
#include "A31G12x_TIMER1n.h"

#include "ir_nec.h"
#include "ir_nec_hal.h"

//----------------------------------
// define
//----------------------------------

//----------------------------------
// variables
//----------------------------------
TIMER1n_CAPTURECFG_Type    TIMER1n_Config;


//----------------------------------
// protocol
//----------------------------------

//==============================================================================
// Sub Routines
//==============================================================================

//==============================================================================
// Function
//==============================================================================
#define IR_TMRx      ((TIMER1n_Type*)TIMER12)
#define IR_TMR       TIMER12
#define IR_Px        ((Pn_Type* )PA)
#define IR_Pn        5
#define MSK_IR_TMR   MSK_TIMER12
#define IR_TMR_IRQn  TIMER12_IRQn

// 타이머를 capure 기능으로 설정해야 한다.
static  void InitPortTn_CAP(void)
{
	// Initialize UART1 pin connect
	PCU_SetDirection       (IR_Px, IR_Pn, ALTERN_FUNC);
	PCU_ConfigureFunction  (IR_Px, IR_Pn, AFSRx_AF2);               // PA5  0: -, 1: T12OUT, 2: T12CAP, 3: AN5, 4: ----
	PCU_ConfigurePullupdown(IR_Px, IR_Pn, Pn_PUPD_PUPD0_EnablePU);
}

// 타이머 카운트 주기가 10 usec가 되도록 설정한다.
static void Init_TIMERn(void)
{
//	TIMER1n_CAPTURECFG_Type T1n_Config;
	
	TIMER1n_Config.CkSel = TIMER1n_PCLK;
	TIMER1n_Config.Prescaler = 160;
	TIMER1n_Config.ADR = 0xffff;
	TIMER1n_Config.ClrMode = TIMER1n_RISING_EGDE;
	TIMER1n_Config.ECE = TIMER1n_FALLING_EGDE;         // dummy parameter
	TIMER1n_Init(IR_TMRx, TIMER1n_CAPTURE_MODE, &TIMER1n_Config);

	TIMER1n_IntConfig(IR_TMRx, TIMER1n_INTCFG_CIE, ENABLE);
	T1nCapInt_ClrFg(IR_TMR);                          //T1nCapInt_ClrFg(TIMER12);

	NVIC_SetPriority(IR_TMR_IRQn, 3);
	NVIC_EnableIRQ(IR_TMR_IRQn);
	EInt_MaskDisable(MSK_IR_TMR);

	//타이머를 시작한다.	
	TIMER1n_ClrCnt(IR_TMR);
	TIMER1n_Cmd(IR_TMRx, ENABLE);
}
/*
// TIMER16 CAP: PB10
static  void InitPortT16_CAP(void)
{
	// Initialize UART1 pin connect
	PCU_SetDirection       (( Pn_Type* )PB, 10, ALTERN_FUNC);
	PCU_ConfigureFunction  (( Pn_Type* )PB, 10, AFSRx_AF2);               // PB10  0: SEG31, 1: T16OUT, 2: T16CAP, 3: ----, 4: ----
	PCU_ConfigurePullupdown(( Pn_Type* )PB, 10, Pn_PUPD_PUPD0_EnablePU);
}

// 타이머 카운트 주기가 10 usec가 되도록 설정한다.
static void Init_TIMER16(void)
{
//	TIMER1n_CAPTURECFG_Type T1n_Config;
	
	TIMER1n_Config.CkSel = TIMER1n_PCLK;
	TIMER1n_Config.Prescaler = 160;
	TIMER1n_Config.ADR = 0xffff;
	TIMER1n_Config.ClrMode = TIMER1n_RISING_EGDE;
	TIMER1n_Config.ECE = TIMER1n_FALLING_EGDE;         // dummy parameter
	TIMER1n_Init((TIMER1n_Type*)TIMER16, TIMER1n_CAPTURE_MODE, &TIMER1n_Config);

	TIMER1n_IntConfig((TIMER1n_Type*)TIMER16, TIMER1n_INTCFG_CIE, ENABLE);
	//TIMER1n_IntConfig((TIMER1n_Type*)TIMER16, TIMER1n_INTCFG_MIE, ENABLE);
	T1nCapInt_ClrFg(TIMER16);

	NVIC_SetPriority(TIMER16_IRQn, 3);
	NVIC_EnableIRQ(TIMER16_IRQn);
	EInt_MaskDisable(MSK_TIMER16);

	//타이머를 시작한다.	
	TIMER1n_ClrCnt(TIMER16);  //T1n_ClrCnt(TIMER16);
	TIMER1n_Cmd((TIMER1n_Type*)TIMER16, ENABLE);
}
*/
//==============================================================================
// Call Back Function
//==============================================================================
// ir_nec,c에서 호출함
void cbIR_TimerInit(void)
{
	InitPortTn_CAP();    //InitPortT16_CAP();
	Init_TIMERn();       //Init_TIMER16();
}

void cbIR_FallingEdge(void)
{
	uint16_t                   reg_val16;

	reg_val16   = IR_TMR->CR  & ~TIMER1n_CR_CPOL_MASK;
	TIMER16->CR = reg_val16   | TIMER1n_CR_CPOL_SET(TIMER1n_FALLING_EGDE);
}

void cbIR_RisingEdge(void)
{
	uint16_t                   reg_val16;

	reg_val16   = IR_TMR->CR  & ~TIMER1n_CR_CPOL_MASK;
	TIMER16->CR = reg_val16   | TIMER1n_CR_CPOL_SET(TIMER1n_RISING_EGDE);

}

/*
void cbIR_TimerInit(void)
{
	InitPortT16_CAP();    //InitPortT16_CAP();
	Init_TIMER16();       //Init_TIMER16();
}

void cbIR_FallingEdge(void)
{
	uint16_t                   reg_val16;

	reg_val16   = TIMER16->CR & ~TIMER1n_CR_CPOL_MASK;
	TIMER16->CR = reg_val16   | TIMER1n_CR_CPOL_SET(TIMER1n_FALLING_EGDE);
}

void cbIR_RisingEdge(void)
{
	uint16_t                   reg_val16;

	reg_val16   = TIMER16->CR & ~TIMER1n_CR_CPOL_MASK;
	TIMER16->CR = reg_val16   | TIMER1n_CR_CPOL_SET(TIMER1n_RISING_EGDE);

}
*/
//==============================================================================
// Interrupt Handler
//==============================================================================
void TIMER16_Handler(void)
{
	uint32_t status;
	uint32_t count;
	status = T1nCapInt_GetFg(TIMER16);  // TIMER1n Capture Interrupt Flag
	if(status == 1){
		T1nCapInt_ClrFg( TIMER16 );
		// CAPTURE 에서 읽은 값으로 IrNecProc()를 호출한다.
		count = TIMER1n_GetCapData(TIMER16);
		IrNecProc(count);  // 호출
	}
}
