/***************************************************************************//**
 * @file          slib.h
 * @brief         Driver Header File for Serial Library
 * @version       1.0
 * @date          2019-09-01
 * @author        ABOV Application Team
 *
 * Copyright (c) 2019, ABOV Semiconductor. All rights reserved.
 *//***************************************************************************/

#ifndef _SLIB_H_
#define _SLIB_H_

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Function
//******************************************************************************

char* scani( char* s, unsigned int* result );
char* scans( char* s, char* result );

#ifdef __cplusplus
}
#endif

#endif   /* _SLIB_H_ */

