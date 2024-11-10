
//==================================================================
// Include 
//==================================================================
#include "mc2012_hal.h"
#include "MC2102.h"

//==================================================================
// define
//==================================================================
#define _BIT(n)	(1<<(n))


/* 
  command
   : 0000_xxbb: DisplayMode set : b1..b0: 00:4Grid  13 Seg, 
   : 0100_0bbb: Data Setting    : b2: Fix/Inc addr, b1..b0: 00:Write data, 10:ReadKey
   : 10xx_bbbb: Display Control : b4: Display on/off b2..b0 : Dimming value
   : 11xx_bbbb: Addr Setting    : 

*/

//==================================================================
// variable
//==================================================================

unsigned char mc2102_led[7][2];  // seg:: 0:seg[1-8], 1:seg[9-14]
unsigned char mc2102_bak[7][2];  // seg:: 0:seg[1-8], 1:seg[9-14]
volatile unsigned char mc2102_key0, mc2102_key1, mc2102_key2, mc2102_key3, mc2102_key4, mc2102_key5;     // 
int mc2102_dimm=14;
int mc2102_grid=5;   // &&& 기본값 5로 세팅

#if 0
#define MC2102dly()   do{__NOP();}while(0)
#else
// 65MHz 에서 동작할 때
void MC2102dly()
{
	int i;
	for(i=0; i<30; i++){
	  __nop();
	}
}
#endif

//=============================================================================
// Sub Routine
//=============================================================================
void MC2102SetBuf(unsigned char val)
{
	int c;

	for(c=0;c<mc2102_grid; c++){
	  mc2102_led[c][0] = val;
	  mc2102_led[c][1] = val;
	}
}

void MC2102PushBuf()
{
	int c;

	for(c=0;c<mc2102_grid; c++){
	  mc2102_bak[c][0] = mc2102_led[c][0];
	  mc2102_bak[c][1] = mc2102_led[c][1];
	}
}

void MC2102PullBuf()
{
	int c;

	for(c=0;c<mc2102_grid; c++){
	  mc2102_led[c][0] = mc2102_bak[c][0];
	  mc2102_led[c][1] = mc2102_bak[c][1];
	}
}

void MC2102Write(unsigned char val) // Command Write Led
{
	int cnt = 8;
	for(;0 < cnt; cnt--){
	  if(val & 0x01) { MC2102_dio_on(); } else { MC2102_dio_off();}
	  val >>= 1;
	  MC2102_clk_off();
	  MC2102dly();
	  MC2102_clk_on();
	  MC2102dly();
	}
	MC2102_clk_on();
}

void	MC2102CMD(char cmd, char stb)	
{
	MC2102_dio_output();
	MC2102_stb_off();
	MC2102Write(cmd);   // Grid(5), Segment(12)	
	if(stb) MC2102_stb_on();
}

// COMMAND 1 : DISPLAY MODE SETTING COMMAND
void	MC2102SetGrid(unsigned char grid)
{
	grid -=4;
	if(3<grid) grid = 0;
	MC2102CMD(grid,1);   // Grid(5), Segment(12)
}

#define AddInc    0
#define AddFix    1
#define DataWrite 0
#define DataRead  2

// COMMAND 2 : DATA SETTING COMMAND
void	MC2102SetData(unsigned char inc, unsigned char rd)
{
	unsigned char c;
	c  = 0x40;   // command
	if(inc==AddFix)   c += 4;  // Address     0:increment, 4:Fixes
	if(rd ==DataRead) c += 2;  // Read/Write 00:Write,    10:REad
	MC2102CMD(c,1);
	MC2102dly();
}

//------------------------------------
// COMMAND 3 : ADDRESS SETTING COMMAND
void	MC2102SetAddr(unsigned char addr)
{
	addr = 0xC0 + (0x0f & addr);
	MC2102CMD(addr,1);	
}

// COMMAND 4 : DISPLAY CONTROL COMMANDS
void	MC2102WriteDimm(unsigned char duty)
{
	if      (duty == 0) { duty = 0x80;}
	else if (duty == 1) { duty = 0x88;}
	else if (duty == 2) { duty = 0x89;}
	else if (duty <= 4) { duty = 0x8a;}
	else if (duty <=10) { duty = 0x8b;}
	else if (duty <=11) { duty = 0x8c;}
	else if (duty <=12) { duty = 0x8d;}
	else if (duty <=13) { duty = 0x8e;}
	else                { duty = 0x8f;}

	MC2102_dio_output();
	MC2102_stb_off();
	MC2102Write(duty);   // Grid(5), Segment(12)	
	MC2102_stb_on();
}
// MC2102가 분리되었다가 다시 연결되어도 표시가 가능하도록
// 데이터를 쓸때마다 MC2102를 새로 초기화 한다.
void MC2102_Reset()
{
	MC2102SetGrid(mc2102_grid);
	MC2102dly();

	MC2102WriteDimm(mc2102_dimm);
	MC2102dly();
}

// 매뉴얼 14/24 참고
// 최대 14 byte
void MC2102WriteDataA()
{
	char i;

	MC2102SetAddr(0);
	MC2102CMD(0x40,0);   // // COMMAND 2 : DATA SETTING COMMAND ,Write ,increment 

	for(i=0; i<mc2102_grid; i++){
	   MC2102Write(mc2102_led[i][0]);
	   MC2102Write(mc2102_led[i][1]);
	}
	MC2102_stb_on();
}

void MC2102WriteData()
{
	MC2102_Reset();
	MC2102WriteDataA();
}

unsigned char MC2102Read() // Command Write Led
{
	char cnt = 8;
	char val = 0;

	for(;0 < cnt; cnt--){
	  MC2102_clk_off();
	  val >>= 1;
	  MC2102dly();
	  MC2102_clk_on();
	  MC2102dly();
	  if(is_MC2102_dio()) { val |= 0x80; }
	}
	cnt = val & 0x1b;
	return cnt ;
}

//====================================================================
//  MC2102 초기화
//====================================================================
// 최초 세팅
void MC2102Init(unsigned int grid, unsigned int dimm)
{
	MC2102InitIO();

	if(grid < MC2102_GRID_LO) grid = MC2102_GRID_LO;
	if(grid > MC2102_GRID_HI) grid = MC2102_GRID_HI;
	mc2102_grid = grid;

	if(dimm > MC2102_DIMM_HI) dimm = MC2102_DIMM_HI;
	mc2102_dimm = dimm;

	// clear led buff
	MC2102SetBuf(0);
	MC2102WriteData();
}

void MC2102SetDimm(int val)
{
	mc2102_dimm = val;
	MC2102_Reset();
}

//====================================================================
//  Key 처리 루틴
//====================================================================
// 매뉴얼 19/24 참고
// b0,b3:K1, b1,b4:K2, b2,b3:Don't Care, b6:0, b7:1
void MC2102ReadKey()
{
	volatile int tmp;
	MC2102_Reset();
	MC2102SetAddr(0);

	MC2102CMD(0x42,0);
	MC2102dly();

	MC2102_dio_input();
	mc2102_key0 = MC2102Read();
	mc2102_key1 = MC2102Read();
	mc2102_key2 = MC2102Read();
	mc2102_key3 = MC2102Read();
	mc2102_key4 = MC2102Read();
	mc2102_key5 = MC2102Read();
	MC2102_dio_output();
	MC2102_stb_on();
}

void MC2102getKey(MC2102_KEY_t *key)
{
	MC2102ReadKey();
	key->k1=0;
	if(mc2102_key0 & _BIT(0)) key->k1 |= 0x0001;   // SEG1
	if(mc2102_key0 & _BIT(3)) key->k1 |= 0x0002;   // SEG2
	if(mc2102_key1 & _BIT(0)) key->k1 |= 0x0004;   // SEG3
	if(mc2102_key1 & _BIT(3)) key->k1 |= 0x0008;   // SEG4
	if(mc2102_key2 & _BIT(0)) key->k1 |= 0x0010;   // SEG5
	if(mc2102_key2 & _BIT(3)) key->k1 |= 0x0020;   // SEG6
	if(mc2102_key3 & _BIT(0)) key->k1 |= 0x0040;   // SEG7
	if(mc2102_key3 & _BIT(3)) key->k1 |= 0x0080;   // SEG8
	if(mc2102_key4 & _BIT(0)) key->k1 |= 0x0100;   // SEG9
	if(mc2102_key4 & _BIT(3)) key->k1 |= 0x0200;   // SEG10
	if(mc2102_key5 & _BIT(0)) key->k1 |= 0x0400;   // SEG11

	key->k2=0;
	if(mc2102_key0 & _BIT(1)) key->k2 |= 0x0001;   // SEG1
	if(mc2102_key0 & _BIT(4)) key->k2 |= 0x0002;   // SEG2
	if(mc2102_key1 & _BIT(1)) key->k2 |= 0x0004;   // SEG3
	if(mc2102_key1 & _BIT(4)) key->k2 |= 0x0008;   // SEG4
	if(mc2102_key2 & _BIT(1)) key->k2 |= 0x0010;   // SEG5
	if(mc2102_key2 & _BIT(4)) key->k2 |= 0x0020;   // SEG6
	if(mc2102_key3 & _BIT(1)) key->k2 |= 0x0040;   // SEG7
	if(mc2102_key3 & _BIT(4)) key->k2 |= 0x0080;   // SEG8
	if(mc2102_key4 & _BIT(1)) key->k2 |= 0x0100;   // SEG9
	if(mc2102_key4 & _BIT(4)) key->k2 |= 0x0200;   // SEG10
	if(mc2102_key5 & _BIT(1)) key->k2 |= 0x0400;   // SEG11
}

