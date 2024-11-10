
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
#include "A31G12x_I2Cn_sg.h"
#include "peri.h"

#include "sensirion_common.h"
#include "sensirion_i2c.h"
#include "sensirion_i2c.h"

//----------------------------------
// define
//----------------------------------
#define I2Cx_SHT3x  (I2Cn_Type*) I2C0    //#define I2Cx_SHT3x  (I2Cn_Type*) I2C1    // TEMP
#define I2Cx_SGP30  (I2Cn_Type*) I2C0    // GAS
#define I2Cx_SPS30  (I2Cn_Type*) I2C0    //#define I2Cx_SPS30  (I2Cn_Type*) I2C2    // 먼지센서

#define SGP30_I2C_ADDRESS        0x58
#define SHT3x_I2C_ADDRESS        0x44
#define SHT31_I2C_ADDRESS        0x45
#define SPS30_I2C_ADDRESS        0x69

I2Cn_Type* GetI2c(uint8_t address)
{
	switch(address){
	case   SPS30_I2C_ADDRESS: return I2Cx_SPS30;
	case   SGP30_I2C_ADDRESS: return I2Cx_SGP30;
	case   SHT3x_I2C_ADDRESS:
	case   SHT31_I2C_ADDRESS: return I2Cx_SHT3x;
	}
	return NULL;
}

int8_t sensirion_i2c_read(uint8_t address, uint8_t* data, uint16_t count)
{
	I2Cn_M_SETUP_Type buf;
	I2Cn_Type* I2Cx;
	Status stat = ERROR;

	I2Cx = GetI2c(address);
	if(I2Cx == NULL) return stat;
	
	buf.sl_addr7bit = address;
	buf.rx_data     = data;
	buf.rx_length   = count;

	stat = I2Cn_MasterReceiveData(I2Cx, &buf, I2Cn_TRANSFER_POLLING);
	return !stat;
}

int8_t sensirion_i2c_write(uint8_t address, const uint8_t* data, uint16_t count)
{
	I2Cn_M_SETUP_Type buf;
	I2Cn_Type* I2Cx;
	Status stat = ERROR;

	I2Cx = GetI2c(address);
	if(I2Cx == NULL) return stat;

	buf.sl_addr7bit = address;
	buf.tx_data     = (uint8_t*) data;
	buf.tx_length   = count;

	stat = I2Cn_MasterTransmitData(I2Cx, &buf, I2Cn_TRANSFER_POLLING);
	return !stat;
}

void sensirion_sleep_usec(uint32_t useconds)
{
	DelayUs(useconds);
}
