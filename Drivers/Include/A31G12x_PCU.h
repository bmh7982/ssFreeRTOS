/***************************************************************************//**
 * @file          A31G12x_PCU.h
 * @brief         Driver Header File for Port Control Unit & GPIO
 * @version       1.0
 * @date          2019-09-01
 * @author        ABOV Application Team
 *
 * Copyright (c) 2019, ABOV Semiconductor. All rights reserved.
 *//***************************************************************************/

#ifndef _PCU_H_
#define _PCU_H_

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Constant
//******************************************************************************

// Pin Mode & Pin Alternative Function Mask
/*
#define PCU_FUNC_Msk             (0x0FuL)
#define PCU_MODE_Msk             (0x03uL)
*/
#define MODEx_Msk                Pn_MOD_MODE0_Msk
#define AFSRx_Msk                Pn_AFSR1_AFSR0_Msk

// Pin Alternative Function Selection Control
/*
#define FUNC0                    0x0            // Alternative Function 0
#define FUNC1                    0x1            // Alternative Function 1
#define FUNC2                    0x2            // Alternative Function 2
#define FUNC3                    0x3            // Alternative Function 3
#define FUNC4                    0x4            // Alternative Function 4
// #define FUNC5                 0x5            // Alternative Function 5
// #define FUNC6                 0x6            // Alternative Function 6
// #define FUNC7                 0x7            // Alternative Function 7
*/
#define AFSRx_AF0                Pn_AFSR1_AFSR0_AF0
#define AFSRx_AF1                Pn_AFSR1_AFSR0_AF1
#define AFSRx_AF2                Pn_AFSR1_AFSR0_AF2
#define AFSRx_AF3                Pn_AFSR1_AFSR0_AF3
#define AFSRx_AF4                Pn_AFSR1_AFSR0_AF4
// #define AFSRx_AF5             Pn_AFSR1_AFSR0_AF5
// #define AFSRx_AF6             Pn_AFSR1_AFSR0_AF6
// #define AFSRx_AF7             Pn_AFSR1_AFSR0_AF7

// Pin Pull Up / Pull Down Resistor Enable Control
/*
#define DISPUPD                  0x0            // disable pu_pd
#define ENPU                     0x1            // enable pull up
#define ENPD                     0x2            // enable pull down
*/
#define PUPDx_Disable            Pn_PUPD_PUPD0_Disable
#define PUPDx_EnablePU           Pn_PUPD_PUPD0_EnablePU
#define PUPDx_EnablePD           Pn_PUPD_PUPD0_EnablePD

// Debounce Filter Sampling Clock Selection Control
/*
#define HCLK_1                   (0x0uL << 16)
#define HCLK_4                   (0x1uL << 16)
#define HCLK_16                  (0x2uL << 16)
#define HCLK_64                  (0x3uL << 16)
#define HCLK_256                 (0x4uL << 16)
#define HCLK_1024                (0x5uL << 16)
*/
#define DBCLK_HCLK1              (Pn_DBCR_DBCLK_HCLK1    << Pn_DBCR_DBCLK_Pos)   // HCLK/1
#define DBCLK_HCLK4              (Pn_DBCR_DBCLK_HCLK4    << Pn_DBCR_DBCLK_Pos)   // HCLK/4
#define DBCLK_HCLK16             (Pn_DBCR_DBCLK_HCLK16   << Pn_DBCR_DBCLK_Pos)   // HCLK/16
#define DBCLK_HCLK64             (Pn_DBCR_DBCLK_HCLK64   << Pn_DBCR_DBCLK_Pos)   // HCLK/64
#define DBCLK_HCLK256            (Pn_DBCR_DBCLK_HCLK256  << Pn_DBCR_DBCLK_Pos)   // HCLK/256
#define DBCLK_HCLK1024           (Pn_DBCR_DBCLK_HCLK1024 << Pn_DBCR_DBCLK_Pos)   // HCLK/1024

// Output Data Mask Control
#define OUTDMSKx_Unmask          Pn_OUTDMSK_OUTDMSK0_Unmask
#define OUTDMSKx_Mask            Pn_OUTDMSK_OUTDMSK0_Mask

//******************************************************************************
// Type
//******************************************************************************

//==============================================================================
// Enumeration
//==============================================================================

/**   Pin Mode and Pin Type Selection Control */
typedef enum
{
   INPUT = 0,
   PUSH_PULL_OUTPUT,
   ALTERN_FUNC,
   OPEN_DRAIN_OUTPUT
} PCU_PORT_MODE;

//******************************************************************************
// Macro
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         Get Port n Input Data
 * @param[in]     Px
 *                   Pointer to a target peripheral.
 *                   -  PA ~ PF
 * @return        Current value of GPIO port
 * @details       This macro gets input data of port n.
 * @remark        Available GPIO Pin: PA[11:0], PB[15:0], PC[12:0], PD[7:0], PE[15:0], PF[11:0]
 *//*-------------------------------------------------------------------------*/
#define GPIO_GetInData( Px )                 (Px->INDR)

/*-------------------------------------------------------------------------*//**
 * @brief         Get a bit of Port n
 * @param[in]     Px
 *                   Pointer to a target peripheral.
 *                   -  PA ~ PF
 * @return        Current bit value of GPIO port
 * @details       This macro gets a bit of port n.
 * @remark        Available GPIO Pin: PA[11:0], PB[15:0], PC[12:0], PD[7:0], PE[15:0], PF[11:0]
 *//*-------------------------------------------------------------------------*/
#define GPIO_GetBit0( Px )                   (Px->INDR_b.INDR0)
#define GPIO_GetBit1( Px )                   (Px->INDR_b.INDR1)
#define GPIO_GetBit2( Px )                   (Px->INDR_b.INDR2)
#define GPIO_GetBit3( Px )                   (Px->INDR_b.INDR3)
#define GPIO_GetBit4( Px )                   (Px->INDR_b.INDR4)
#define GPIO_GetBit5( Px )                   (Px->INDR_b.INDR5)
#define GPIO_GetBit6( Px )                   (Px->INDR_b.INDR6)
#define GPIO_GetBit7( Px )                   (Px->INDR_b.INDR7)
#define GPIO_GetBit8( Px )                   (Px->INDR_b.INDR8)
#define GPIO_GetBit9( Px )                   (Px->INDR_b.INDR9)
#define GPIO_GetBit10( Px )                  (Px->INDR_b.INDR10)
#define GPIO_GetBit11( Px )                  (Px->INDR_b.INDR11)
#define GPIO_GetBit12( Px )                  (Px->INDR_b.INDR12)
#define GPIO_GetBit13( Px )                  (Px->INDR_b.INDR13)
#define GPIO_GetBit14( Px )                  (Px->INDR_b.INDR14)
#define GPIO_GetBit15( Px )                  (Px->INDR_b.INDR15)

/*-------------------------------------------------------------------------*//**
 * @brief         Set Port n Output Data
 * @param[in]     Px
 *                   Pointer to a target peripheral.
 *                   -  PA ~ PF
 * @param[in]     u32OutData
 *                   Output Data of Port n
 * @return        none
 * @details       This macro sets output data of port n.
 * @remark        Available GPIO Pin: PA[11:0], PB[15:0], PC[12:0], PD[7:0], PE[15:0], PF[11:0]
 *//*-------------------------------------------------------------------------*/
#define GPIO_SetOutData( Px, u32OutData )    (Px->OUTDR = u32OutData)

/*-------------------------------------------------------------------------*//**
 * @brief         Set a bit of Port n
 * @param[in]     Px
 *                   Pointer to a target peripheral.
 *                   -  PA ~ PF
 * @return        none
 * @details       This macro sets a bit of port n.
 * @remark        Available GPIO Pin: PA[11:0], PB[15:0], PC[12:0], PD[7:0], PE[15:0], PF[11:0]
 *//*-------------------------------------------------------------------------*/
#define GPIO_SetBit0( Px )                   (Px->BSR_b.BSR0 = 1)
#define GPIO_SetBit1( Px )                   (Px->BSR_b.BSR1 = 1)
#define GPIO_SetBit2( Px )                   (Px->BSR_b.BSR2 = 1)
#define GPIO_SetBit3( Px )                   (Px->BSR_b.BSR3 = 1)
#define GPIO_SetBit4( Px )                   (Px->BSR_b.BSR4 = 1)
#define GPIO_SetBit5( Px )                   (Px->BSR_b.BSR5 = 1)
#define GPIO_SetBit6( Px )                   (Px->BSR_b.BSR6 = 1)
#define GPIO_SetBit7( Px )                   (Px->BSR_b.BSR7 = 1)
#define GPIO_SetBit8( Px )                   (Px->BSR_b.BSR8 = 1)
#define GPIO_SetBit9( Px )                   (Px->BSR_b.BSR9 = 1)
#define GPIO_SetBit10( Px )                  (Px->BSR_b.BSR10 = 1)
#define GPIO_SetBit11( Px )                  (Px->BSR_b.BSR11 = 1)
#define GPIO_SetBit12( Px )                  (Px->BSR_b.BSR12 = 1)
#define GPIO_SetBit13( Px )                  (Px->BSR_b.BSR13 = 1)
#define GPIO_SetBit14( Px )                  (Px->BSR_b.BSR14 = 1)
#define GPIO_SetBit15( Px )                  (Px->BSR_b.BSR15 = 1)

/*-------------------------------------------------------------------------*//**
 * @brief         Clear a bit of Port n
 * @param[in]     Px
 *                   Pointer to a target peripheral.
 *                   -  PA ~ PF
 * @return        none
 * @details       This macro clears a bit of port n.
 * @remark        Available GPIO Pin: PA[11:0], PB[15:0], PC[12:0], PD[7:0], PE[15:0], PF[11:0]
 *//*-------------------------------------------------------------------------*/
#define GPIO_ClrBit0( Px )                   (Px->BCR_b.BCR0 = 1)
#define GPIO_ClrBit1( Px )                   (Px->BCR_b.BCR1 = 1)
#define GPIO_ClrBit2( Px )                   (Px->BCR_b.BCR2 = 1)
#define GPIO_ClrBit3( Px )                   (Px->BCR_b.BCR3 = 1)
#define GPIO_ClrBit4( Px )                   (Px->BCR_b.BCR4 = 1)
#define GPIO_ClrBit5( Px )                   (Px->BCR_b.BCR5 = 1)
#define GPIO_ClrBit6( Px )                   (Px->BCR_b.BCR6 = 1)
#define GPIO_ClrBit7( Px )                   (Px->BCR_b.BCR7 = 1)
#define GPIO_ClrBit8( Px )                   (Px->BCR_b.BCR8 = 1)
#define GPIO_ClrBit9( Px )                   (Px->BCR_b.BCR9 = 1)
#define GPIO_ClrBit10( Px )                  (Px->BCR_b.BCR10 = 1)
#define GPIO_ClrBit11( Px )                  (Px->BCR_b.BCR11 = 1)
#define GPIO_ClrBit12( Px )                  (Px->BCR_b.BCR12 = 1)
#define GPIO_ClrBit13( Px )                  (Px->BCR_b.BCR13 = 1)
#define GPIO_ClrBit14( Px )                  (Px->BCR_b.BCR14 = 1)
#define GPIO_ClrBit15( Px )                  (Px->BCR_b.BCR15 = 1)

/*-------------------------------------------------------------------------*//**
 * @brief         Set Port n Output Data Mask
 * @param[in]     Px
 *                   Pointer to a target peripheral.
 *                   -  PA ~ PF
 * @param[in]     u32Msk
 *                   Output Data Mask of Port n
 *                   -  OUTDMSKx_Unmask, OUTDMSKx_Mask
 * @return        none
 * @details       This macro sets mask or unmask for output data of port n.
 * @remark        Available GPIO Pin: PA[11:0], PB[15:0], PC[12:0], PD[7:0], PE[15:0], PF[11:0]
 *//*-------------------------------------------------------------------------*/
#define GPIO_OutMsk( Px, u32Msk )            (Px->OUTDMSK = u32Msk)

//******************************************************************************
// Function
//******************************************************************************

void PCU_SetReg( Pn_Type* Px, uint32_t u32Mode, uint32_t u32Type, uint32_t u32Afsr1, uint32_t u32Afsr2, uint32_t u32PuPd );

void PCU_SetDebounce( Pn_Type* Px, uint32_t u32Pins, uint32_t u32Debnc );

void GPIO_SetValue( Pn_Type* Px, uint16_t bitValue );
void GPIO_ClearValue( Pn_Type* Px, uint16_t bitValue );
uint16_t GPIO_ReadValue( Pn_Type* Px );
void GPIO_WriteValue( Pn_Type* Px, uint16_t Value );

void PCU_ConfigureFunction( Pn_Type* Px, uint8_t pin_no, uint32_t func );
void PCU_SetDirection( Pn_Type* Px, uint8_t pin_no, PCU_PORT_MODE dir_type );
void PCU_ConfigurePullupdown( Pn_Type* Px, uint8_t pin_no, uint8_t pullupdown );
void PCU_ConfigureOutDataMask( Pn_Type* Px, uint8_t pin_no, FunctionalState maskctrl );

#ifdef __cplusplus
}
#endif

#endif   /* _PCU_H_ */

