#ifndef _MC2012_HAL_H_
#define _MC2012_HAL_H_

#include <stdbool.h>

void  MC2102_stb_on(void);
void  MC2102_stb_off(void);

void  MC2102_clk_on(void);
void  MC2102_clk_off(void);

void  MC2102_dio_on(void);
void  MC2102_dio_off(void);
void  MC2102_dio_output(void);
void  MC2102_dio_input(void);

bool  is_MC2102_dio(void);

void  MC2102InitIO(void);

#endif   // _MC2012_HAL_H_

