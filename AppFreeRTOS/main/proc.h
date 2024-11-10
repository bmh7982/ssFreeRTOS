
#ifndef _PROC_H
#define _PROC_H

#include <stdint.h>
#include <stdbool.h>
//#include "A31G12x.h"
//#include "aa_types.h"

//----------------------------------
// define
//----------------------------------
#define  VER       '1'

//----------------------------------
// variable
//----------------------------------

//----------------------------------
// protocol type
//----------------------------------
inline uint32_t GetTime(void);
inline void sleep(unsigned int cnt);

//

#endif  // _PROC_H

