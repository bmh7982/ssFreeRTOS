
#ifndef _IR_NEC_H
#define _IR_NEC_H

#include <stdint.h>
// 외부에서 준비해주어야 할것
typedef void (*IrCallback)(unsigned char *IrData);

void cbIR_FallingEdge(void);
void cbIR_RisingEdge(void);
void cbIR_TimerInit(void);

void IrNecProc(uint32_t count); //ir_nec_hal.c에서 호출
void InitIrNec(IrCallback fp);

#endif  // _IR_NEC_H

