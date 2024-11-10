
//==============================================================================
// Head::
//
//
//
//==============================================================================

//----------------------------------
// include
//----------------------------------
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "A31G12x.h"
#include "aa_types.h"

#include "A31G12x_PCU.h"
#include "A31G12x_I2Cn_sg.h"
#include "A31G12x_INTC.h"
#include "i2c_a31g12x.h"

//----------------------------------
// define
//----------------------------------
#define I2C_SENSOR    (I2Cn_Type*) I2C0
#define I2C_NO_SENSOR                 0
#define IRQ_I2C_SENSOR         IRQ_I2C0


//----------------------------------
// variablee
//----------------------------------



//----------------------------------
// prototype
//----------------------------------
void InitSample(void);
void ProcSample(void);


//==============================================================================
// Sub Routines
//==============================================================================

//==============================================================================
// Rrivate Function
//==============================================================================

//==============================================================================
// Public Function
//==============================================================================


//==============================================================================
// Public Function (√ ±‚»≠)
//==============================================================================
void I2C0_Init(void)
{
	//--------------------------
	// I2C0 PD0:SCL0, PD1:SDA0
	PCU_ConfigureFunction( ( Pn_Type* )PD, 0, AFSRx_AF1 );
	PCU_SetDirection( ( Pn_Type* )PD, 0, ALTERN_FUNC );

	//PCU_ConfigurePullupdown( ( Pn_Type* )PD, 0, Pn_PUPD_PUPD0_Disable );
	PCU_ConfigurePullupdown( ( Pn_Type* )PD, 0, Pn_PUPD_PUPD0_EnablePU );

	PCU_ConfigureFunction( ( Pn_Type* )PD, 1, AFSRx_AF1 );
	PCU_SetDirection( ( Pn_Type* )PD, 1, ALTERN_FUNC );
	//PCU_ConfigurePullupdown( ( Pn_Type* )PD, 1, Pn_PUPD_PUPD0_Disable );
	PCU_ConfigurePullupdown( ( Pn_Type* )PD, 1, Pn_PUPD_PUPD0_EnablePU );

	//I2Cn_DeInit((I2Cn_Type* )I2C0);
	I2Cn_Init((I2Cn_Type*) I2C0, 100000 );
	//I2Cn_SetOwnSlaveAddr1((I2Cn_Type*)I2C0, PM2008_I2C_ADDRESS, DISABLE );
	EInt_MaskDisable( MSK_I2C0);
}

//==============================================================================
// Interrupt Handler
//==============================================================================

