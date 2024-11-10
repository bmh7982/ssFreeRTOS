
//==============================================================================
// Include
//==============================================================================
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#include "A31G12x.h"
#include "aa_types.h"
#include "A31G12x_PCU.h"

#include "mc2012_hal.h"


//----------------------------------
// Define
//----------------------------------

//----------------------------------
// IO PORT Define
//----------------------------------
#define  MC2102_PORT_STB  PA
#define  MC2102_PORT_CLK  PA
#define  MC2102_PORT_DIO  PA

#define  MC2102_PIO_STB   PA
#define  MC2102_PIO_CLK   PA
#define  MC2102_PIO_DIO   PA

#define  MC2102_BIT_STB   14
#define  MC2102_BIT_CLK   15
#define  MC2102_BIT_DIO   13


//==============================================================================
// Sub Routines
//==============================================================================

//==============================================================================
// Function
//==============================================================================
void  MC2102_stb_on(void) {GPIO_SetValue  (( Pn_Type* )MC2102_PIO_STB, _BIT(MC2102_BIT_STB));}   // STB
void  MC2102_stb_off(void){GPIO_ClearValue(( Pn_Type* )MC2102_PIO_STB, _BIT(MC2102_BIT_STB));}

void  MC2102_clk_on(void) {GPIO_SetValue  (( Pn_Type* )MC2102_PIO_CLK, _BIT(MC2102_BIT_CLK));}    // CLK
void  MC2102_clk_off(void){GPIO_ClearValue(( Pn_Type* )MC2102_PIO_CLK, _BIT(MC2102_BIT_CLK));}

void  MC2102_dio_on(void) {GPIO_SetValue  (( Pn_Type* )MC2102_PIO_DIO, _BIT(MC2102_BIT_DIO));}    // DIO
void  MC2102_dio_off(void){GPIO_ClearValue(( Pn_Type* )MC2102_PIO_DIO, _BIT(MC2102_BIT_DIO));}

void  MC2102_dio_output(void){PCU_SetDirection(( Pn_Type* )MC2102_PORT_DIO, MC2102_BIT_DIO, PUSH_PULL_OUTPUT);}    // DIO
void  MC2102_dio_input(void) {PCU_SetDirection(( Pn_Type* )MC2102_PORT_DIO, MC2102_BIT_DIO, INPUT           );}

bool  is_MC2102_dio(void){ return ((GPIO_ReadValue (( Pn_Type* )MC2102_PIO_DIO)&_BIT(MC2102_BIT_DIO)) ? 1:0);}

void  MC2102InitIO(void)
{
	MC2102_stb_on();
	MC2102_clk_on();
	MC2102_dio_on();

	PCU_SetDirection(( Pn_Type* )MC2102_PORT_STB, MC2102_BIT_STB, PUSH_PULL_OUTPUT);  // STB
	PCU_SetDirection(( Pn_Type* )MC2102_PORT_CLK, MC2102_BIT_CLK, PUSH_PULL_OUTPUT);  // CLK
	PCU_SetDirection(( Pn_Type* )MC2102_PORT_DIO, MC2102_BIT_DIO, PUSH_PULL_OUTPUT);  // DIO
}

//==============================================================================
// Interrupt Handler
//==============================================================================

