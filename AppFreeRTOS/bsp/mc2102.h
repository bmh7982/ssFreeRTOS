#ifndef _MC2012_H_
#define _MC2012_H_

//--------------------
// include
//--------------------

//--------------------
// IO PORT Define
//--------------------

//--------------------
// Public define
//--------------------

//-----------------------------
// Public function prototypes
//-----------------------------
//MC2102SetDimm();
#define  MC2102_DIMM_HI   14
#define  MC2102_DIMM_13   13
#define  MC2102_DIMM_12   12
#define  MC2102_DIMM_11   11
#define  MC2102_DIMM_10   10
#define  MC2102_DIMM_4     4
#define  MC2102_DIMM_2     2
#define  MC2102_DIMM_1     1
#define  MC2102_DIMM_0     0   // Off

//MC2102SetGrid();
#define  MC2102_GRID_LO  4
#define  MC2102_GRID_4   4
#define  MC2102_GRID_5   5  // 기본값
#define  MC2102_GRID_6   6
#define  MC2102_GRID_7   7
#define  MC2102_GRID_HI  7

typedef struct {
	unsigned short  k1;
	unsigned short  k2;
} MC2102_KEY_t;

//-----------------------------
// Public function prototypes
//-----------------------------
void MC2102Init(unsigned int grid, unsigned int dimm);
void MC2102SetGrid(unsigned char grid);
void MC2102SetDimm(int duty);

void MC2102WriteData(void);          //디스플레이 버퍼를 MC2102에 쓴다(표시)
void MC2102getKey(MC2102_KEY_t *key);

void MC2102SetBuf(unsigned char c);  //디스플레이 버퍼를 모두 동일한 값으로 채운다 (초기화 & 테슽용)
void MC2102PushBuf(void);
void MC2102PullBuf(void);

extern unsigned char mc2102_led[7][2];  // seg:: 0:seg[1-8], 1:seg[9-14]
//volatile extern unsigned char mc2102_key0, mc2102_key1, mc2102_key2, mc2102_key3, mc2102_key4, mc2102_key5;     // 

#endif  // _MC2012_H_
