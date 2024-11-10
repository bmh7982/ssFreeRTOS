#ifndef _DISP_MC2102_
#define _DISP_MC2102_

//-----------------------------
// defines
//-----------------------------
#define  CR_RED   4
#define  CR_GRN   2
#define  CR_BLU   1

typedef  enum
{
	COLOR_RED     = (CR_RED         ),
	COLOR_YELLOW  = (CR_RED + CR_GRN),
	COLOR_GREEN   = (CR_GRN         ),
	COLOR_CYAN    = (CR_GRN + CR_BLU),
	COLOR_BLUE    = (CR_BLU         ),
	COLOR_MAGENTA = (CR_BLU + CR_RED),

	COLOR_BLACK   = 0,
	COLOR_WHITE   = (CR_RED + CR_GRN + CR_BLU),  //7,
} rgb_color_t;

/*
typedef  enum
{
	COLOR_BLACK   = 0,
	COLOR_WHITE   = 7<<4,
	COLOR_YELLOW  = 6<<4,
	COLOR_MAGENTA = 5<<4,
	COLOR_RED     = 4<<4,
	COLOR_CYAN    = 3<<4,
	COLOR_GREEN   = 2<<4,
	COLOR_BLUE    = 1<<4
} rgb_color_t;
*/

#define  SPEED_OFF     0
#define  SPEED_AUTO    1
#define  SPEED_MAN1    2
#define  SPEED_MAN2    4
#define  SPEED_MAN3    8
#define  SPEED_TURBO  15  //SPEED_MAN1+SPEED_MAN2+SPEED_MAN3

#define  SEN_IDLE     0
#define  SEN_RUN      1
#define  SEN_ERR      2

//-----------------------------
// Public function prototypes
//-----------------------------
void DispLedTest(int delay);
void DispSegTest(int delay);

void set_disp_temp(int val, int stat);
void set_disp_dust(int val, int stat);
void set_disp_gas (int val, int stat);
void set_disp_co2 (int val, int stat);

void set_led_spd(int val);
void set_rgb_spd(rgb_color_t val);

void set_led_mode(int val);
void set_rgb_mode(rgb_color_t val);

//----------
// ÃÊ±âÈ­
void InitDisp(int term);
void ProcDisp(void);

#endif  // _DISP_MC2102_

