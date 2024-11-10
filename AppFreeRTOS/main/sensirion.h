#ifndef _SENSIRION_H_
#define _SENSIRION_H_

//----------------------------------
// define
//----------------------------------


//----------------------------------
// variable
//----------------------------------
extern int cur_temp;     // 온도
extern int cur_dust;     // 먼지농도
extern int cur_gas;      // GAS 값

//----------------------------------
// protocol type
//----------------------------------
void InitSensirion(uint32_t term);
void ProcSensirion(void);

#endif   // _SENSIRION_H_
