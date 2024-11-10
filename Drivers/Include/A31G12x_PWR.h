/***************************************************************************//**
 * @file          A31G12x_PWR.h
 * @brief         Driver Header File for Power Control
 * @version       1.0
 * @date          2019-09-01
 * @author        ABOV Application Team
 *
 * Copyright (c) 2019, ABOV Semiconductor. All rights reserved.
 *//***************************************************************************/

#ifndef _PWR_H_
#define _PWR_H_

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Function
//******************************************************************************

void PWR_Sleep( void );
void PWR_DeepSleep( void );

#ifdef __cplusplus
}
#endif

#endif   /*_PWR_H_ */

