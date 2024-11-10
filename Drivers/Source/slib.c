/***************************************************************************//**
 * @file          slib.c
 * @brief         Driver Source File for Serial Library
 * @version       1.0
 * @date          2019-09-01
 * @author        ABOV Application Team
 *
 * Copyright (c) 2019, ABOV Semiconductor. All rights reserved.
 *//***************************************************************************/

//******************************************************************************
// Include
//******************************************************************************

#include <stdarg.h>
#include <stdio.h>

#include "A31G12x.h"
#include "aa_types.h"

#include "A31G12x_ADC.h"
#include "A31G12x_CRC.h"
#include "A31G12x_FMC.h"
#include "A31G12x_I2Cn.h"
#include "A31G12x_INTC.h"
#include "A31G12x_LCD.h"
#include "A31G12x_PCU.h"
#include "A31G12x_PWR.h"
#include "A31G12x_SCUCC.h"
#include "A31G12x_SCUCG.h"
#include "A31G12x_SCULV.h"
#include "A31G12x_TIMER1n.h"
#include "A31G12x_TIMER2n.h"
#include "A31G12x_TIMER3n.h"
#include "A31G12x_UARTn.h"
#include "A31G12x_USART1n.h"
#include "A31G12x_WDT.h"
#include "A31G12x_WT.h"
#include "A31G12x_WTIDKY.h"
#include "debug_frmwrk.h"
#include "slib.h"

//******************************************************************************
// Function
//******************************************************************************

char* scani( char* s, unsigned int* result )
{
   uint32_t    n = 0;
   int         i;

   *result = 0;
   while( *s != 0 && *s == ' ' )
   {
      s++;
   }
   if( *s == 0 )
   {
      return NULL;
   }
   else
      while( ( i = *s ) != 0 )
      {
         s++;
         if( i == ',' )
         {
            break;
         }

         if( i >= '0' && i <= '9' )
         {
            i -= '0';
         }
         else if( i >= 'A' && i <= 'F' )
         {
            i -= 'A' - 10;
         }
         else if( i >= 'a' && i <= 'f' )
         {
            i -= 'a' - 10;
         }
         else
         {
            break;
         }
         n = ( n << 4 ) + i;
      }
   *result = n;
   return s;
}

char* scans( char* s, char* result )
{
   int   i;
   while( *s != 0 && *s == ' ' )
   {
      s++;
   }
   while( ( i = *s ) != 0 )
   {
      s++;
      if( /*i==' '||*/i == ',' )
      {
         break;
      }
      *result++ = i;
   }
   *result = 0;
   return s;
}

