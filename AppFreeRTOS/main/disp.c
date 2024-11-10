
//----------------------------------
// include
//----------------------------------
#include <string.h>
#include <stdbool.h>

//#include "FreeRTOS.h"
//#include "task.h"

#include "proc.h"
#include "mc2102.h"
#include "sensirion.h"
#include "peri.h"
#include "disp.h"

#pragma   anon_unions

//----------------------------------
// define
//----------------------------------
#undef _BIT
// Set bit macro
#define _BIT( n )          (1 << (n))

#define  SPD_MASK_VAL     (~SPEED_TURBO)
#define  SPD_MASK_RGB     (~COLOR_WHITE)

#define  MODE_MASK_VAL    (~0x0F)
#define  MODE_MASK_RGB    (~COLOR_WHITE)

#define  GR_DIG100        2
#define  GR_DIG10         3
#define  GR_DIG1          4

#define  GR_SPD_L         0
#define  GR_MOD_L         1

#define  GR_RGB_1         0
#define  GR_RGB_2         1
#define  GR_RGB_3         2
#define  GR_RGB_4         3
#define  GR_RGB_5         4
#define  GR_RGB_6         5

#define  BIT_RED          0
#define  BIT_GRN          1
#define  BIT_BLU          2

#define  BIT_AUTO         0
#define  BIT_MAN1         1
#define  BIT_MAN2         2
#define  BIT_MAN3         3

#define  BIT_TEMP         0
#define  BIT_DUST         1
#define  BIT_GAS          2
#define  BIT_CO2          3

#define   led_all_rst()   do{int i; for(i=0;i<7;i++){mc2102_led[i][0] = 0x00; mc2102_led[i][1] = 0x00;}} while(0)  // 모두 지움
#define   led_all_set()   do{int i; for(i=0;i<7;i++){mc2102_led[i][0] = 0xff; mc2102_led[i][1] = 0xff;}} while(0)  // 모두 켜기

// 풍량 관련 
#define   led_spd_off()   do{mc2102_led[GR_SPD_L][0] &=     ~0x0f; } while(0)  // b0:Auto, b1:1단, b2:2단, b3:3단
#define   led_auto_set()  do{mc2102_led[GR_SPD_L][0] |=  _BIT(BIT_AUTO); } while(0)  // GR4  S1
#define   led_auto_rst()  do{mc2102_led[GR_SPD_L][0] &= ~_BIT(BIT_AUTO); } while(0)
#define   led_man1_set()  do{mc2102_led[GR_SPD_L][0] |=  _BIT(BIT_MAN1); } while(0)  // GR4  S2
#define   led_man1_rst()  do{mc2102_led[GR_SPD_L][0] &= ~_BIT(BIT_MAN1); } while(0)
#define   led_man2_set()  do{mc2102_led[GR_SPD_L][0] |=  _BIT(BIT_MAN2); } while(0)  // GR4  S3
#define   led_man2_rst()  do{mc2102_led[GR_SPD_L][0] &= ~_BIT(BIT_MAN2); } while(0)
#define   led_man3_set()  do{mc2102_led[GR_SPD_L][0] |=  _BIT(BIT_MAN3); } while(0)  // GR4  S4
#define   led_man3_rst()  do{mc2102_led[GR_SPD_L][0] &= ~_BIT(BIT_MAN3); } while(0)

// 구분 
#define   led_mode_off()  do{mc2102_led[GR_MOD_L][0] &=     ~0x0f; } while(0)  // GR2: b0:temp, b1:dust, b2:tvoc, b3:co2
#define   led_temp_set()  do{mc2102_led[GR_MOD_L][0] |=  _BIT(BIT_TEMP); } while(0)  //  S1
#define   led_temp_rst()  do{mc2102_led[GR_MOD_L][0] &= ~_BIT(BIT_TEMP); } while(0)  //      
#define   led_dust_set()  do{mc2102_led[GR_MOD_L][0] |=  _BIT(BIT_DUST); } while(0)  //  S2
#define   led_dust_rst()  do{mc2102_led[GR_MOD_L][0] &= ~_BIT(BIT_DUST); } while(0)  //      
#define   led_gas_set()   do{mc2102_led[GR_MOD_L][0] |=  _BIT(BIT_GAS ); } while(0)  //  S3
#define   led_gas_rst()   do{mc2102_led[GR_MOD_L][0] &= ~_BIT(BIT_GAS ); } while(0)  //      
#define   led_co2_set()   do{mc2102_led[GR_MOD_L][0] |=  _BIT(BIT_CO2 ); } while(0)  //  S4
#define   led_co2_rst()   do{mc2102_led[GR_MOD_L][0] &= ~_BIT(BIT_CO2 ); } while(0)

// RGB spd
#define   rgb_all_rst()   do{int i; for(i=0;i<7;i++){mc2102_led[i][1] = 0x00;}} while(0)  // rgb 모두 지움

// RGB spd
#define   rgb_spd_set(cr) do{int i; for(i=0;i<3;i++){mc2102_led[i][1] = cr;  }} while(0)  // rgb 모두 지움
#define   rgb_spd_rst()   do{int i; for(i=0;i<3;i++){mc2102_led[i][1] = 0x00;}} while(0)  // rgb 모두 지움

#define   rgb1_red_set()  do{mc2102_led[GR_RGB_1][1] |=  _BIT(BIT_RED); } while(0)  // GR1  S9
#define   rgb1_red_rst()  do{mc2102_led[GR_RGB_1][1] &= ~_BIT(BIT_RED); } while(0)
#define   rgb1_grn_set()  do{mc2102_led[GR_RGB_1][1] |=  _BIT(BIT_GRN); } while(0)  // 
#define   rgb1_grn_rst()  do{mc2102_led[GR_RGB_1][1] &= ~_BIT(BIT_GRN); } while(0)
#define   rgb1_blu_set()  do{mc2102_led[GR_RGB_1][1] |=  _BIT(BIT_BLU); } while(0)  // 
#define   rgb1_blu_rst()  do{mc2102_led[GR_RGB_1][1] &= ~_BIT(BIT_BLU); } while(0)

#define   rgb2_red_set()  do{mc2102_led[GR_RGB_2][1] |=  _BIT(BIT_RED); } while(0)  // GR2  S9
#define   rgb2_red_rst()  do{mc2102_led[GR_RGB_2][1] &= ~_BIT(BIT_RED); } while(0)
#define   rgb2_grn_set()  do{mc2102_led[GR_RGB_2][1] |=  _BIT(BIT_GRN); } while(0)  // 
#define   rgb2_grn_rst()  do{mc2102_led[GR_RGB_2][1] &= ~_BIT(BIT_GRN); } while(0)
#define   rgb2_blu_set()  do{mc2102_led[GR_RGB_2][1] |=  _BIT(BIT_BLU); } while(0)  // 
#define   rgb2_blu_rst()  do{mc2102_led[GR_RGB_2][1] &= ~_BIT(BIT_BLU); } while(0)

#define   rgb3_red_set()  do{mc2102_led[GR_RGB_3][1] |=  _BIT(BIT_RED); } while(0)  // GR3  S9
#define   rgb3_red_rst()  do{mc2102_led[GR_RGB_3][1] &= ~_BIT(BIT_RED); } while(0)
#define   rgb3_grn_set()  do{mc2102_led[GR_RGB_3][1] |=  _BIT(BIT_GRN); } while(0)  // 
#define   rgb3_grn_rst()  do{mc2102_led[GR_RGB_3][1] &= ~_BIT(BIT_GRN); } while(0)
#define   rgb3_blu_set()  do{mc2102_led[GR_RGB_3][1] |=  _BIT(BIT_BLU); } while(0)  // 
#define   rgb3_blu_rst()  do{mc2102_led[GR_RGB_3][1] &= ~_BIT(BIT_BLU); } while(0)

// RGB mode
#define   rgb_mode_set(cr) do{int i; for(i=3;i<6;i++){mc2102_led[i][1] = cr;  }} while(0)  // rgb 모두 지움
#define   rgb_mode_rst()   do{int i; for(i=3;i<6;i++){mc2102_led[i][1] = 0x07;}} while(0)  // rgb 모두 지움

#define   rgb4_red_set()  do{mc2102_led[GR_RGB_4][1] |=  _BIT(BIT_RED); } while(0)  // GR4  S9
#define   rgb4_red_rst()  do{mc2102_led[GR_RGB_4][1] &= ~_BIT(BIT_RED); } while(0)
#define   rgb4_grn_set()  do{mc2102_led[GR_RGB_4][1] |=  _BIT(BIT_GRN); } while(0)  // 
#define   rgb4_grn_rst()  do{mc2102_led[GR_RGB_4][1] &= ~_BIT(BIT_GRN); } while(0)
#define   rgb4_blu_set()  do{mc2102_led[GR_RGB_4][1] |=  _BIT(BIT_BLU); } while(0)  // 
#define   rgb4_blu_rst()  do{mc2102_led[GR_RGB_4][1] &= ~_BIT(BIT_BLU); } while(0)

#define   rgb5_red_set()  do{mc2102_led[GR_RGB_5][1] |=  _BIT(BIT_RED); } while(0)  // GR5  S9
#define   rgb5_red_rst()  do{mc2102_led[GR_RGB_5][1] &= ~_BIT(BIT_RED); } while(0)
#define   rgb5_grn_set()  do{mc2102_led[GR_RGB_5][1] |=  _BIT(BIT_GRN); } while(0)  // 
#define   rgb5_grn_rst()  do{mc2102_led[GR_RGB_5][1] &= ~_BIT(BIT_GRN); } while(0)
#define   rgb5_blu_set()  do{mc2102_led[GR_RGB_5][1] |=  _BIT(BIT_BLU); } while(0)  // 
#define   rgb5_blu_rst()  do{mc2102_led[GR_RGB_5][1] &= ~_BIT(BIT_BLU); } while(0)

#define   rgb6_red_set()  do{mc2102_led[GR_RGB_6][1] |=  _BIT(BIT_RED); } while(0)  // GR5  S9
#define   rgb6_red_rst()  do{mc2102_led[GR_RGB_6][1] &= ~_BIT(BIT_RED); } while(0)
#define   rgb6_grn_set()  do{mc2102_led[GR_RGB_6][1] |=  _BIT(BIT_GRN); } while(0)  // 
#define   rgb6_grn_rst()  do{mc2102_led[GR_RGB_6][1] &= ~_BIT(BIT_GRN); } while(0)
#define   rgb6_blu_set()  do{mc2102_led[GR_RGB_6][1] |=  _BIT(BIT_BLU); } while(0)  // 
#define   rgb6_blu_rst()  do{mc2102_led[GR_RGB_6][1] &= ~_BIT(BIT_BLU); } while(0)

//----------------------------------
// variable
//----------------------------------
#define  IDX_TEMP   0
#define  IDX_DUST   17
#define  IDX_GAS    2
#define  IDX_CO2    3
#define  IDX_MAX    3

static int disp_m_idx   =    0;  // 표시   0:온도, 1:먼지, 2:개스, 3:co2
static int disp_tm_term = 5000;  // 주기 5초
static int disp_tm_next ;       // 다음 기간

static int disp_temp   = 0;     // 온도
static int disp_dust   = 0;     // 먼지농도
static int disp_gas    = 0;     // GAS 값
static int disp_co2    = 0;     // CO2 농도

static int stat_temp   = 0;     // 온도
static int stat_dust   = 0;     // 먼지
static int stat_gas    = 0;     // GAS
static int stat_co2    = 0;     // CO2

static int disp_spd    = 0;     // 풍속
static int disp_mode   = 0;     // 모드

//====================================================================
// Display 7 Seg 
//====================================================================
#define  SEG_BLANK   10
#define  SEG_MINUS   11

#define  seg_a  0x01
#define  seg_b  0x02
#define  seg_c  0x04
#define  seg_d  0x08
#define  seg_e  0x10
#define  seg_f  0x20
#define  seg_g  0x40

const unsigned char seg_digi[] = {
	(1*seg_a + 1*seg_b + 1*seg_c + 1*seg_d + 1*seg_e + 1*seg_f + 0*seg_g),  // 0 '0'
	(0*seg_a + 1*seg_b + 1*seg_c + 0*seg_d + 0*seg_e + 0*seg_f + 0*seg_g),  // 1 '1'
	(1*seg_a + 1*seg_b + 0*seg_c + 1*seg_d + 1*seg_e + 0*seg_f + 1*seg_g),  // 2 '2'
	(1*seg_a + 1*seg_b + 1*seg_c + 1*seg_d + 0*seg_e + 0*seg_f + 1*seg_g),  // 3 '3'
	(0*seg_a + 1*seg_b + 1*seg_c + 0*seg_d + 0*seg_e + 1*seg_f + 1*seg_g),  // 4 '4'
	(1*seg_a + 0*seg_b + 1*seg_c + 1*seg_d + 0*seg_e + 1*seg_f + 1*seg_g),  // 5 '5'
	(1*seg_a + 0*seg_b + 1*seg_c + 1*seg_d + 1*seg_e + 1*seg_f + 1*seg_g),  // 6 '6'
	(1*seg_a + 1*seg_b + 1*seg_c + 0*seg_d + 0*seg_e + 0*seg_f + 0*seg_g),  // 7 '7'
	(1*seg_a + 1*seg_b + 1*seg_c + 1*seg_d + 1*seg_e + 1*seg_f + 1*seg_g),  // 8 '8'
	(1*seg_a + 1*seg_b + 1*seg_c + 1*seg_d + 0*seg_e + 1*seg_f + 1*seg_g),  // 9 '9'
	(0*seg_a + 0*seg_b + 0*seg_c + 0*seg_d + 0*seg_e + 0*seg_f + 0*seg_g),  //10 BLANK 
	(0*seg_a + 0*seg_b + 0*seg_c + 0*seg_d + 0*seg_e + 0*seg_f + 1*seg_g),  //11 '-' (SEG_MINUS) 
	(0*seg_a + 0*seg_b + 0*seg_c + 0*seg_d + 0*seg_e + 0*seg_f + 0*seg_g)   // Dummy 
};

void seg_disp_off(void)
{
	mc2102_led[GR_DIG100][0] = 0;     // GR3 MASK
	mc2102_led[GR_DIG10 ][0] = 0;     // GR4
	mc2102_led[GR_DIG1  ][0] = 0;     // GR5
}

void seg_disp_val(uint8_t a, uint8_t b, uint8_t c)
{
	mc2102_led[GR_DIG100][0] = a;
	mc2102_led[GR_DIG10 ][0] = b;
	mc2102_led[GR_DIG10 ][0] = c;
}

// mode:0 숫자 0 1: blank
void seg_disp_digit(unsigned int val, bool blank)
{
	int val1 = (val%1000)/100;    // 100 단위
	int val2 = (val% 100)/ 10;    //  10 단위
	int val3 = (val%  10);        //   1 단위

	if(blank){
		if(val1==0){ val1=SEG_BLANK; if(val2==0 ){val2=SEG_BLANK;}}
	}

	seg_disp_off();
	mc2102_led[GR_DIG100][0] |=  seg_digi[val1];  // 100 단위
	mc2102_led[GR_DIG10 ][0] |=  seg_digi[val2];  //  10 단위
	mc2102_led[GR_DIG1  ][0] |=  seg_digi[val3];  //   1 단위
}

// "E00" 표기
void seg_disp_err(int val)
{
	int val3 = (val%  10);        //   1 단위

	seg_disp_off();
	mc2102_led[GR_DIG100][0] |= (1*seg_a + 0*seg_b + 0*seg_c + 1*seg_d + 1*seg_e + 1*seg_f + 1*seg_g); // E 
	mc2102_led[GR_DIG1  ][0] |=	seg_digi[val3];   // Err값 
}

// "---" 표기
void seg_disp_none(void)
{
	seg_disp_off();
	mc2102_led[GR_DIG100][0] |=  seg_digi[SEG_MINUS];  // 100 단위
	mc2102_led[GR_DIG10 ][0] |=  seg_digi[SEG_MINUS];  //  10 단위
	mc2102_led[GR_DIG1  ][0] |=  seg_digi[SEG_MINUS];  //   1 단위
}

const unsigned char seg_rotate[] = {
	(1*seg_a + 0*seg_b + 0*seg_c + 0*seg_d + 0*seg_e + 0*seg_f + 0*seg_g),  // a
	(0*seg_a + 1*seg_b + 0*seg_c + 0*seg_d + 0*seg_e + 0*seg_f + 0*seg_g),  // b
	(0*seg_a + 0*seg_b + 1*seg_c + 0*seg_d + 0*seg_e + 0*seg_f + 0*seg_g),  // c
    (0*seg_a + 0*seg_b + 0*seg_c + 1*seg_d + 0*seg_e + 0*seg_f + 0*seg_g),  // d
	(0*seg_a + 0*seg_b + 0*seg_c + 0*seg_d + 1*seg_e + 0*seg_f + 0*seg_g),  // e
	(0*seg_a + 0*seg_b + 0*seg_c + 0*seg_d + 0*seg_e + 1*seg_f + 0*seg_g),  // f
    (0*seg_a + 0*seg_b + 0*seg_c + 0*seg_d + 0*seg_e + 0*seg_f + 1*seg_g)   // g
};


//====================================================================
// Public Function (test)
//====================================================================
void sleep(unsigned int cnt);
// auto -> 수동 1단, 수동2, 수동3, 온도, 먼지, 개스, co2 순으로 1회전한다
void DispLedTest(int delay)
{
	led_all_rst();
	led_auto_set();  // S9,GR4 필터 버튼 백색
	MC2102WriteData();
	sleep(delay);

	led_auto_rst();
	led_man1_set();
	MC2102WriteData();
	sleep(delay);

	led_man1_rst();
	led_man2_set();
	MC2102WriteData();
	sleep(delay);

	led_man2_rst();
	led_man3_set();
	MC2102WriteData();
	sleep(delay);

	led_man3_rst();
	rgb1_red_set();
	MC2102WriteData();
	sleep(delay);

	rgb1_red_rst();
	rgb1_grn_set();
	MC2102WriteData();
	sleep(delay);

	rgb1_grn_rst();
	rgb1_blu_set();
	MC2102WriteData();
	sleep(delay);

	rgb1_blu_rst();
	rgb2_red_set();
	led_temp_set();
	MC2102WriteData();
	sleep(delay);

	rgb2_red_rst();
	rgb2_grn_set();
	MC2102WriteData();
	sleep(delay);

	rgb2_grn_rst();
	rgb2_blu_set();
	MC2102WriteData();
	sleep(delay);

	rgb2_blu_rst();
	rgb3_red_set();
	led_temp_set();
	MC2102WriteData();
	sleep(delay);

	rgb3_red_rst();
	rgb3_grn_set();
	MC2102WriteData();
	sleep(delay);

	rgb3_grn_rst();
	rgb3_blu_set();
	MC2102WriteData();
	sleep(delay);

	rgb3_blu_rst();
	led_temp_set();
	MC2102WriteData();
	sleep(delay);

	led_temp_rst();
	led_dust_set();
	MC2102WriteData();
	sleep(delay);

	led_dust_rst();
	led_gas_set();
	MC2102WriteData();
	sleep(delay);

	led_gas_rst();
	led_co2_set();
	MC2102WriteData();
	sleep(delay);

	led_co2_rst();
	rgb4_red_set();
	led_temp_set();
	MC2102WriteData();
	sleep(delay);

	rgb4_red_rst();
	rgb4_grn_set();
	MC2102WriteData();
	sleep(delay);

	rgb4_grn_rst();
	rgb4_blu_set();
	MC2102WriteData();
	sleep(delay);

	rgb4_blu_rst();
	rgb5_red_set();
	led_temp_set();
	MC2102WriteData();
	sleep(delay);

	rgb5_red_rst();
	rgb5_grn_set();
	MC2102WriteData();
	sleep(delay);

	rgb5_grn_rst();
	rgb5_blu_set();
	MC2102WriteData();
	sleep(delay);

	rgb5_blu_rst();
	rgb6_red_set();
	led_temp_set();
	MC2102WriteData();
	sleep(delay);

	rgb6_red_rst();
	rgb6_grn_set();
	MC2102WriteData();
	sleep(delay);

	rgb6_grn_rst();
	rgb6_blu_set();
	MC2102WriteData();
	sleep(delay);

	rgb6_blu_rst();
	rgb2_red_set();
	led_temp_set();
	MC2102WriteData();
	sleep(delay);
}

void DispSegTest(int delay)
{
	//int delay = 400;
	int i;
	uint8_t c;

	for(i=0;i<7;i++){
		c = seg_rotate[i];
		seg_disp_val(c,c,c);
		MC2102WriteData();
		sleep(delay);
	}
}

void DispFndCo2(void)      // fnd제어
{
	if(disp_co2 ==0) {seg_disp_none();                }
	else             {seg_disp_digit(disp_co2, true);}
}

// 일정주기 마다 온도, 먼지, 개스, co2순서로 이동하면서 표시한다.
void DispFnd(void)      // fnd제어
{
	led_mode_off();
	switch(disp_m_idx){
		case  IDX_TEMP:  led_temp_set(); seg_disp_digit(disp_temp, stat_temp); break;
		case  IDX_DUST:  led_dust_set(); seg_disp_digit(disp_dust, stat_dust); break;
		case  IDX_GAS:   led_gas_set();  seg_disp_digit(disp_gas,  stat_gas ); break;
		case  IDX_CO2:   led_co2_set();  seg_disp_digit(disp_co2,  stat_co2 ); break;
		default: break;
	}
	MC2102WriteData();
}

//====================================================================
// Public Function
//====================================================================
void DispSpd(void)      // fnd제어
{
	led_spd_off();
	if((disp_spd & SPEED_AUTO) == SPEED_AUTO){led_auto_set();}
	if((disp_spd & SPEED_MAN1) == SPEED_MAN1){led_man1_set();}
	if((disp_spd & SPEED_MAN2) == SPEED_MAN2){led_man2_set();}
	if((disp_spd & SPEED_MAN3) == SPEED_MAN3){led_man3_set();}

	MC2102WriteData();
}

//====================================================================
// Public Function (Call)
//====================================================================
void set_disp_temp(int val, int stat)
{
	disp_temp = val;      // 온도
	stat_temp = stat;     // 상태
	if(disp_m_idx == IDX_TEMP) DispFnd();
}

/*
void set_disp_humi(int val, int stat)
{
	disp_humi = val;      // 온도
	stat_humi = stat;     // 상태
	// 확장용
}
*/

void set_disp_dust(int val, int stat)
{
	disp_dust = val;      // 먼지
	stat_dust = stat;     // 상태
	if(disp_m_idx == IDX_DUST) DispFnd();
}

void set_disp_gas(int val, int stat)
{
	disp_gas = val;      // 개스
	stat_gas = stat;     // 상태
	if(disp_m_idx == IDX_GAS) DispFnd();
}

void set_disp_co2(int val, int stat)
{
	disp_co2 = val;      // co2
	stat_co2 = stat;     // 상태
	if(disp_m_idx == IDX_CO2) DispFnd();
}

void set_led_spd(int val)
{
	disp_spd &= SPD_MASK_VAL;
	disp_spd |= val;      // 풍속
	DispFnd();
}

void set_led_mode(int val)
{
	disp_mode &= MODE_MASK_VAL;
	disp_mode |= val;      // 풍속	
	DispFnd();
}

//RGB
void set_rgb_spd(rgb_color_t cr)
{
	rgb_spd_rst();
	rgb_spd_set(cr);
	MC2102WriteData();

/*
	uint8_t val;
	disp_spd &= SPD_MASK_RGB;
	disp_spd |= cr;

	rgb_spd_rst();
	switch(cr){
		case COLOR_WHITE:    val = 0x07; //ff_ff_ff
		                     rgb1_red_set(); rgb1_grn_set(); rgb1_blu_set();  //ff_ff_ff
		                     break;
		case COLOR_BLACK:    rgb1_red_rst(); rgb1_grn_rst(); rgb1_blu_rst(); break; //00_00_00
		case COLOR_RED:      rgb1_red_set(); rgb1_grn_rst(); rgb1_blu_rst(); break; //ff_00_00
		case COLOR_YELLOW:   rgb1_red_set(); rgb1_grn_set(); rgb1_blu_rst(); break; //ff_ff_00
		case COLOR_GREEN:    rgb1_red_rst(); rgb1_grn_set(); rgb1_blu_rst(); break; //00_ff_00
		case COLOR_CYAN:     rgb1_red_rst(); rgb1_grn_set(); rgb1_blu_set(); break; //00_ff_ff
		case COLOR_BLUE:     rgb1_red_rst(); rgb1_grn_rst(); rgb1_blu_set(); break; //00_00_ff
		case COLOR_MAGENTA:  rgb1_red_set(); rgb1_grn_rst(); rgb1_blu_set(); break; //ff_00_ff
	}
	MC2102WriteData();
*/
}

void set_rgb_mode(rgb_color_t cr)
{
	rgb_mode_rst();
	rgb_mode_set(cr);
	MC2102WriteData();

/*
	disp_mode &= MODE_MASK_RGB;
	disp_mode |= cr;
	switch(cr){
		case COLOR_WHITE:    led_mred_set(); led_mgrn_set(); led_mblu_set(); break; //ff_ff_ff
		case COLOR_BLACK:    led_mred_rst(); led_mgrn_rst(); led_mblu_rst(); break; //00_00_00
		case COLOR_RED:      led_mred_set(); led_mgrn_rst(); led_mblu_rst(); break; //ff_00_00
		case COLOR_YELLOW:   led_mred_set(); led_mgrn_set(); led_mblu_rst(); break; //ff_ff_00
		case COLOR_GREEN:    led_mred_rst(); led_mgrn_set(); led_mblu_rst(); break; //00_ff_00
		case COLOR_CYAN:     led_mred_rst(); led_mgrn_set(); led_mblu_set(); break; //00_ff_ff
		case COLOR_BLUE:     led_mred_rst(); led_mgrn_rst(); led_mblu_set(); break; //00_00_ff
		case COLOR_MAGENTA:  led_mred_set(); led_mgrn_rst(); led_mblu_set(); break; //ff_00_ff
	}
	MC2102WriteData();
*/
}

//====================================================================
// Public Function (Proc)
//====================================================================
void ProcDisp()
{
	int32_t tm_current;
	tm_current = disp_tm_next - GetTime();
	if(0 < tm_current) return;
	disp_tm_next += disp_tm_term;
	if(IDX_MAX<++disp_m_idx) disp_m_idx =0;
	DispFnd();
}

//====================================================================
// Public Function ( 초기화)
//====================================================================
void InitDisp(int term)
{
	disp_tm_term = term;
	disp_tm_next = (GetTime()+500)/1000 + term;

	// LED드라이버 초기화
	MC2102Init(MC2102_GRID_6, MC2102_DIMM_HI);

	// 모든 DISPLAY LED OFF
	led_all_rst();
	MC2102WriteData();

	// FND초기화
	disp_m_idx = IDX_MAX;

	disp_m_idx   = IDX_MAX;  // 표시   0:온도, 1:먼지, 2:개스, 3:co2
	disp_tm_term = 5000;     // 주기 5초

	disp_temp   = -999;     // 온도
	disp_dust   = -999;     // 먼지농도
	disp_gas    = -999;     // GAS 값
	disp_co2    = -999;     // CO2 농도

	stat_temp   = 0;     // 온도
	stat_dust   = 0;     // 먼지
	stat_gas    = 0;     // GAS
	stat_co2    = 0;     // CO2

	disp_spd    = 0;     // 풍속
	disp_mode   = 0;     // 모드

}

