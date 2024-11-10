#ifndef _PERIPHERAL_H_
#define _PERIPHERAL_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

//----------------------------------
// define
//----------------------------------
//typedef  enum
//{
//	COLOR_BLACK   = 0,
//	COLOR_WHITE   = 7,
//	COLOR_YELLOW  = 6,
//	COLOR_MAGENTA = 5,
//	COLOR_RED     = 4,
//	COLOR_CYAN    = 3,
//	COLOR_GREEN   = 2,
//	COLOR_BLUE    = 1
//} rgb_color_t;

//#define	COLOR_BLACK    0
//#define COLOR_WHITE    7
//#define COLOR_YELLOW   6
//#define COLOR_MAGENTA  5
//#define COLOR_RED      4
//#define COLOR_CYAN     3
//#define COLOR_GREEN    2
//#define COLOR_BLUE     1

//----------------------------------
// variable
//----------------------------------
//extern int temp;     // 온도
//extern int dust;     // 먼지농도
extern int cur_gas;      // GAS 값
extern int cur_co2;      // CO2 농도

extern int curspd;      // 풍속
extern int curiaq;      // 공기질 상태

//----------------------------------
// protocol type
//----------------------------------
void InitPeri(void);
void InitUartEsp32(void);
void InitUartDust(void);
void InitUartMon(void);
void InitUartModBus(void);

//void RgbSpdSet(rgb_color_t cr);
//void RgbDustSet(rgb_color_t cr);

void rs485_rx(void);
void rs485_tx(void);

void DelayUs(uint32_t usec);

inline void fan_on(void);
inline void fan_off(void);

inline bool is_touch(void);
inline bool is_pir(void);

inline void sht3x_reset_lo(void);
inline void sht3x_reset_hi(void);

#endif   // _PERIPHERAL_H_

