
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

#include "proc.h"
#include "i2c_a31g12x.h"
#include "sht3x.h"
#include "sgp30.h"
#include "sps30.h"
#include "disp.h"

#include "sensirion.h"

//----------------------------------
// define
//----------------------------------
//#define ADDR_SHT3x  0x44  // 온습도 센서 0x44, 0x45
//#define ADDR_SGP30  0x58  // GAS  세서
//#define ADDR_SPS30  0x69  // 먼지 센서

#define SGP_FAIL_PROBING       1
#define SGP_FAIL_SETVERSION    2
#define SGP_FAIL_SERIAL_ID     2
#define SGP_FAIL_READ_BLOCK    3
#define SGP_FAIL_IAQ_INIT      4
#define SGP_ERR_IAQ_READ      10
#define SGP_ERR_BAS_ELINE     11

#define SPS_FAIL_PROBING       1
#define SPS_FAIL_FIRMWARE      2
#define SPS_FAIL_SERIAL_ID     3
#define SPS_FAIL_START         4
#define SPS_ERR_READ_MEASURE  10

//----------------------------------
// variablee
//----------------------------------
int  sht3x_err;    // sht3x_상태
int  sgp30_err;    // gas 센서
int  sps30_err;    // 먼지센서

//------------------------
// SHT3x-DIS
float    temperature;
float    humidity;

const char  sht_drv_version_str[] = "SHT_DRV_VERSION_STR";
const char* SHT_DRV_VERSION_STR = sht_drv_version_str;

//------------------------
// SGP30
uint16_t tvoc_ppb;
uint16_t co2_eq_ppm;
uint32_t iaq_baseline;

//------------------------
// SPS30
uint8_t fw_major;
uint8_t fw_minor;
struct sps30_measurement m;


//----------------------------------
// prototype
//----------------------------------

//==============================================================================
// Sub Routines
//==============================================================================
uint16_t SHT3x_Init(void)
{
	int16_t  error;
	int      i;

	error = NO_ERROR;

	//------------------
	for(i=0;; i++){
		error = sht3x_probe();
		if (error == NO_ERROR) break;

		if(3<= i){
			sht3x_err = SGP_FAIL_PROBING;   // SHT3x_FAIL_PROBING이 없어 대체사용함
			return sht3x_err;
		}
		sleep(1000);
	}

	return error;
}

uint16_t SGP30_Init(void)
{
	int16_t  error;
	uint16_t feature_set_version;
	uint8_t  product_type;
	uint16_t ethanol_raw_signal, h2_raw_signal;
	int      i;

	//------------------
	for(i=0;; i++){
		error = sgp30_probe();
		if (error == NO_ERROR) break;

		if (error == SGP30_ERR_UNSUPPORTED_FEATURE_SET){
			//printf("Your sensor needs at least feature set version 1.0 (0x20)\n");
		}
		if(3<= i){
			sgp30_err = SGP_FAIL_PROBING;
			return sgp30_err;
		}
		sleep(1000);
	}

	//------------------
	error = sgp30_get_feature_set_version(&feature_set_version, &product_type);
	if (error == NO_ERROR) {
		//printf("Feature set version: %u\n", feature_set_version);
		//printf("Product type: %u\n", product_type);
	} else {
		//printf("sgp30_get_feature_set_version failed!\n");
		sgp30_err = SGP_FAIL_SETVERSION;
		return sgp30_err;
	}

	//------------------
	// Read gas raw signals
	uint64_t serial_id;
	error = sgp30_get_serial_id(&serial_id);
	if (error == NO_ERROR) {
		//printf("SerialID: %" PRIu64 "\n", serial_id);
	} else {
		//printf("sgp30_get_serial_id failed!\n");
		sgp30_err = SGP_FAIL_SERIAL_ID;
		return sgp30_err;
	}

	//------------------
    // Read gas raw signals
	error = sgp30_measure_raw_blocking_read(&ethanol_raw_signal, &h2_raw_signal);
	if (error == NO_ERROR) {
		// Print ethanol raw signal and h2 raw signal
		//printf("Ethanol raw signal: %u\n", ethanol_raw_signal);
		//printf("H2 raw signal: %u\n", h2_raw_signal);
	}
	else {
		//printf("error reading raw signals\n");
        sgp30_err = SGP_FAIL_READ_BLOCK;
		return sgp30_err;
	}

	//------------------
	// Consider the two cases (A) and (B):
	//  (A) If no baseline is available or the most recent baseline is more than
	//      one week old, it must discarded. A new baseline is found with
	//      sgp30_iaq_init()
	error = sgp30_iaq_init();
	if (error == NO_ERROR) {
        //printf("sgp30_iaq_init done\n");
		sgp30_err = NO_ERROR;
	}
	else {
        //printf("sgp30_iaq_init failed!\n");
		sgp30_err = SGP_FAIL_IAQ_INIT;
		return sgp30_err;
    }
	return sgp30_err;
}

char serial_number[SPS30_MAX_SERIAL_LEN];
uint16_t SPS30_Init(void)
{
	int16_t  error;
//	uint16_t feature_set_version;
//	uint8_t  product_type;
//	uint16_t ethanol_raw_signal, h2_raw_signal;
	int      i;

	//------------------
	for(i=0;; i++){
		error = sps30_probe();
		if (error == NO_ERROR) break;
		if(3<= i){
			sps30_err = SPS_FAIL_PROBING;
			return sps30_err;
		}
		sleep(1000);
	}

	//------------------
	error = sps30_read_firmware_version(&fw_major, &fw_minor);
	if (error) {
		//printf("error reading firmware version\n");
		sps30_err = SPS_FAIL_FIRMWARE;
		return sps30_err;
	}
	else {
		//printf("FW: %u.%u\n", fw_major, fw_minor);
    }

	//------------------
	// Read serial_number
	// 모니터링을 위하여 바깥으로 이동함 char serial_number[SPS30_MAX_SERIAL_LEN];
	error = sps30_get_serial(serial_number);
	if (error) {
		//printf("error reading serial number\n");
		sps30_err = SPS_FAIL_SERIAL_ID;
		return sps30_err;
	}
	else {
		//printf("Serial Number: %s\n", serial_number);
	}

	//------------------
	// Start Mesurment
	error = sps30_start_measurement();
	if (error < 0){
		//printf("error starting measurement\n");
		sps30_err = SPS_FAIL_START;
		return sps30_err;
	}
	else{
		//printf("measurements started\n");
		sps30_err = NO_ERROR;
	}
	return sps30_err;
}

//==============================================================================
// Public Function
//==============================================================================
#define SENSOR_IDLE    0  // 대기 상태
#define SENSOR_SHT3x  10  // 온습도 센서 동작 상태
#define SENSOR_SHT3xD 11  // 온습도 센서 종료된 상태
#define SENSOR_SGP30  20  // GAS  세서   동작 상태
#define SENSOR_SGP30D 21  // GAS  세서   종료된 상태
#define SENSOR_SPS30  30  // 먼지 센서   동작 상태
#define SENSOR_SPS30D 31  // 먼지 센서   종료된 상태

uint32_t  sensor_mode = SENSOR_IDLE;
uint32_t  next_time;
uint32_t  term_time;

void ProcSensorIdle(void)
{
	uint32_t cur_time;

	cur_time = GetTime();
	if(next_time < cur_time){
		next_time = cur_time + term_time;
		sensor_mode = SENSOR_SHT3x;
	}
}

// addr: 0x44, 0x45
//struct SHT3x_BUFF_t sh3_buf;
uint8_t tx_buf[20];
void ProcSensorSHT3x(void)
{
//	SHT3xError error;

	int32_t temp, humi;
	sensor_mode = SENSOR_SGP30;

	if(sht3x_err==true){
		// sht3x가 에러이면 새로 살려야 한다.
		return;
	}

//	error = SHT3X_ReadMeasurementBuffer(&temperature, &humidity);
//	if(error != NO_ERROR) sht3x_err = true;
//
//	set_disp_temp((int) temperature, stat_humi);
//	set_disp_humi((int) humidity);

	int8_t ret = sht3x_measure_blocking_read(&temp, &humi);
	if (ret == NO_ERROR) {
		//printf("measured temperature: %0.2f degreeCelsius, "
		//  "measured humidity: %0.2f percentRH\n",
		//  temperature / 1000.0f, humidity / 1000.0f);
		temperature = temp/1000.0f;
		humidity    = humi/1000.f;
	}
	else {
		//err
		//printf("error reading measurement\n");
	}
}

void ProcSensorSGP30(void)
{
    uint16_t i = 0;
	int16_t err;

	sensor_mode = SENSOR_SPS30;

	if(sgp30_err!=NO_ERROR){
		// 에러이면 새로 살려야 한다.
		return;
	}

	err = sgp30_measure_iaq_blocking_read(&tvoc_ppb, &co2_eq_ppm);
	if (err == NO_ERROR) {
		//printf("tVOC  Concentration: %dppb\n", tvoc_ppb);
		//printf("CO2eq Concentration: %dppm\n", co2_eq_ppm);
		set_disp_gas((int) tvoc_ppb, sps30_err);
	}
	else {
		//printf("error reading IAQ values\n");
		sps30_err = SGP_ERR_IAQ_READ;
	}

	// Persist the current baseline every hour
	if (++i % 3600 == 3599) {
		err = sgp30_get_iaq_baseline(&iaq_baseline);
		if (err == NO_ERROR) {
		}
		else{
			sps30_err = SGP_ERR_BAS_ELINE;
		}
	}

	// The IAQ measurement must be triggered exactly once per second (SGP30)
	//   to get accurate values.
}

void ProcSensorSPS30(void)
{
	int16_t err;

	sensor_mode = SENSOR_IDLE;

	if(sps30_err!=NO_ERROR){
		// 에러이면 새로 살려야 한다.
		return;
	}

	err = sps30_read_measurement(&m);
	if (err != NO_ERROR) {
		//printf("error reading measurement\n");
		sps30_err = SGP_ERR_BAS_ELINE;
	}
	else {
		set_disp_dust((int) m.mc_2p5, sps30_err);

		if(0) printf("measured values:\n"
                   "\t%0.2f pm1.0\n"
                   "\t%0.2f pm2.5\n"
                   "\t%0.2f pm4.0\n"
                   "\t%0.2f pm10.0\n"
                   "\t%0.2f nc0.5\n"
                   "\t%0.2f nc1.0\n"
                   "\t%0.2f nc2.5\n"
                   "\t%0.2f nc4.5\n"
                   "\t%0.2f nc10.0\n"
                   "\t%0.2f typical particle size\n\n",
                   m.mc_1p0, m.mc_2p5, m.mc_4p0, m.mc_10p0, m.nc_0p5, m.nc_1p0,
                   m.nc_2p5, m.nc_4p0, m.nc_10p0, m.typical_particle_size);
	}
}

void ProcSensirion(void)
{
//	if(sensor_mode == SENSOR_IDLE){
//		ProcSensorIdle();
//		return;
//	}
//	ProcSensorIdle(); 
//	ProcSensorSHT3x();
//	ProcSensorSGP30();
//	ProcSensorSPS30();

	switch(sensor_mode){
		case   SENSOR_IDLE:  ProcSensorIdle();   break;
		case   SENSOR_SHT3x: ProcSensorSHT3x();  break;
		case   SENSOR_SGP30: ProcSensorSGP30();  break;
		case   SENSOR_SPS30: ProcSensorSPS30();  break;
		default            : sensor_mode = SENSOR_IDLE; break;
	}
}

void InitSensirion(uint32_t term)
{
	term_time = term;
	next_time = GetTime() + term;

	// I2C0 초기화
	I2C0_Init();

	sht3x_err = true;    // sht3x_상태
	sgp30_err = true;    // gas 센서
	sps30_err = true;    // 먼지센서

	// SHT3X 초기화
	sht3x_err  = SHT3x_Init();

	// SGP30 초기화
	sgp30_err = SGP30_Init();
	if(sgp30_err != NO_ERROR){
		//printf("SGP sensor probing failed\n");
	}

	//---------------------------
	// 먼지센서(SPS30) 초기화
	sps30_err = SPS30_Init();
	if(sps30_err != NO_ERROR){
		//printf("SGP sensor probing failed\n");
	}

	sensor_mode = SENSOR_IDLE;
}
