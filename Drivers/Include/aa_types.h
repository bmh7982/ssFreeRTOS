/***************************************************************************//**
 * @file          aa_types.h
 * @brief         Driver Header File for the ABOV typedefs for C standard types.
 *                It is intended to be used in ISO C conforming development environments
 *                and checks for this insofar as it is possible to do so.
 * @version       1.0
 * @date          2019-09-01
 * @author        ABOV Application Team
 *
 * Copyright (c) 2019, ABOV Semiconductor. All rights reserved.
 *//***************************************************************************/

#ifndef AA_TYPES_H
#define AA_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Constant
//******************************************************************************

/* NULL pointer */
#ifndef NULL
#define NULL            ((void*) 0)
#endif

//******************************************************************************
// Type
//******************************************************************************

//==============================================================================
// Enumeration
//==============================================================================

/**   Boolean Type definition */
typedef enum
{
   FALSE = 0,
   TRUE = !FALSE
} Bool;

/**   Flag Status and Interrupt Flag Status type definition */
typedef enum
{
   RESET = 0,
   SET = !RESET
} FlagStatus, IntStatus, SetState;

/**   Functional State Definition */
typedef enum
{
   DISABLE = 0,
   ENABLE = !DISABLE
} FunctionalState;

/**   Status type definition */
typedef enum
{
   ERROR = 0,
   SUCCESS = !ERROR
} Status;

/**   Read/Write transfer type mode (Block or non-block) */
typedef enum
{
   NONE_BLOCKING = 0,   /**<  None Blocking type */
   BLOCKING,            /**<  Blocking type */
} TRANSFER_BLOCK_Type;

//==============================================================================
// Generic
//==============================================================================

typedef unsigned            char    b8;      // boolean type
typedef                     char    c8;      // character type
typedef unsigned            char    u8;      //  8-bit unsigned integer type
typedef signed              char    s8;      //  8-bit   signed integer type
typedef unsigned short      int     u16;     // 16-bit unsigned integer type
typedef signed   short      int     s16;     // 16-bit   signed integer type
// typedef unsigned            int     u32;     // 32-bit unsigned integer type
// typedef signed              int     s32;     // 32-bit   signed integer type
typedef unsigned long       int     u32;     // 32-bit unsigned integer type
typedef signed   long       int     s32;     // 32-bit   signed integer type
typedef unsigned long  long int     u64;     // 64-bit unsigned integer type
typedef signed   long  long int     s64;     // 64-bit   signed integer type
typedef                     float   f32;     // 32-bit floating point type
typedef                     double  f64;     // 64-bit floating point type

typedef volatile unsigned            char    vb8;      // boolean type
typedef volatile                     char    vc8;      // character type
typedef volatile unsigned            char    vu8;      //  8-bit unsigned integer type
typedef volatile signed              char    vs8;      //  8-bit   signed integer type
typedef volatile unsigned short      int     vu16;     // 16-bit unsigned integer type
typedef volatile signed   short      int     vs16;     // 16-bit   signed integer type
// typedef volatile unsigned            int     vu32;     // 32-bit unsigned integer type
// typedef volatile signed              int     vs32;     // 32-bit   signed integer type
typedef volatile unsigned long       int     vu32;     // 32-bit unsigned integer type
typedef volatile signed   long       int     vs32;     // 32-bit   signed integer type
typedef volatile unsigned long  long int     vu64;     // 64-bit unsigned integer type
typedef volatile signed   long  long int     vs64;     // 64-bit   signed integer type
typedef volatile                     float   vf32;     // 32-bit floating point type
typedef volatile                     double  vf64;     // 64-bit floating point type

//******************************************************************************
// Macro
//******************************************************************************

//==============================================================================
// bit operation
//==============================================================================

#define bitm( p )             ((u32)1<<(p))
#define bitp( r, p, v )       {                                   \
                                 r &= ~bitm( p );                 \
                                 r |= (((v) & bitm(0)) << (p));   \
                              }
#define bits( r, p )          r |= bitm( p )
#define bitr( r, p )          r &= ~bitm( p )
#define bitc( r, p )          r ^= bitm( p )
#define bitg( r, p )          ((r&bitm( p )) >> (p))
#define bitt( r, p, v )       ((r&bitm( p )) == ((v)<<(p)))

#define bitsm( p, s )         ((((u64)1<<(s))-1) << (p))
#define bitsp( r, p, s, v )   {                                      \
                                 r &= ~bitsm( p, s );                \
                                 r |= (((v) & bitsm(0,s)) << (p));   \
                              }
#define bitss( r, p, s )      r |= bitsm( p, s )
#define bitsr( r, p, s )      r &= ~bitsm( p, s )
#define bitsc( r, p, s )      r ^= bitsm( p, s )
#define bitsg( r, p, s )      ((r&bitsm( p, s )) >> (p))
#define bitst( r, p, s, v )   ((r&bitsm( p, s )) == ((v)<<(p)))

//==============================================================================
// bit operation
//==============================================================================

/*-------------------------------------------------------------------------*//**
 * _BIT( n ) sets the bit at position "n"
 * _BIT( n ) is intended to be used in "OR" and "AND" expressions:
 * e.g., "(_BIT(3) | _BIT(7))".
 *//*-------------------------------------------------------------------------*/
#undef _BIT
// Set bit macro
#define _BIT( n )          (1 << (n))

/*-------------------------------------------------------------------------*//**
 * _SBF( f, v ) sets the bit field starting at position "f" to value "v".
 * _SBF( f, v ) is intended to be used in "OR" and "AND" expressions:
 * e.g., "((_SBF(5,7) | _SBF(12,0xF)) & 0xFFFF)"
 *//*-------------------------------------------------------------------------*/
#undef _SBF
// Set bit field macro
#define _SBF( f, v )       ((v) << (f))

/*-------------------------------------------------------------------------*//**
 * _BITMASK constructs a symbol with 'field_width' least significant
 * bits set.
 * e.g., _BITMASK(5) constructs '0x1F', _BITMASK(16) == 0xFFFF
 * The symbol is intended to be used to limit the bit field width
 * thusly:
 * <a_register> = (any_expression) & _BITMASK(x), where 0 < x <= 32.
 * If "any_expression" results in a value that is larger than can be
 * contained in 'x' bits, the bits above 'x - 1' are masked off.  When
 * used with the _SBF example above, the example would be written:
 * a_reg = ((_SBF(5,7) | _SBF(12,0xF)) & _BITMASK(16))
 * This ensures that the value written to a_reg is no wider than
 * 16 bits, and makes the code easier to read and understand.
 *//*-------------------------------------------------------------------------*/
#undef _BITMASK
// Bitmask creation macro
#define _BITMASK( field_width )        (_BIT(field_width) - 1)

//==============================================================================
// array
//==============================================================================

// Number of elements in an array
#define NELEMENTS( array )             (sizeof (array) / sizeof (array[0]))

//==============================================================================
// max/min
//==============================================================================

// max
#if !defined( MAX )
#define MAX( a, b )     (((a) > (b)) ? (a) : (b))
#endif

// min
#if !defined( MIN )
#define MIN( a, b )     (((a) < (b)) ? (a) : (b))
#endif

//==============================================================================
// instruction
//==============================================================================

#define SYS_RESET()              NVIC_SystemReset()

#define NOP()                    __NOP()
#define WFI()                    __WFI()
#define WFE()                    __WFE()
#define DI()                     __disable_irq()   // reset value is "interrupt enable".
#define EI()                     __enable_irq()    // reset value is "interrupt enable".

#define APB_NOP                  {unsigned long tmp = CIDR;}

//==============================================================================
// directive
//==============================================================================

// Static data/function define
#define STATIC          static

// External data/function define
#define EXTERN          extern

// inline function
#ifdef __CC_ARM
#define INLINE          __inline
#else
#define INLINE          inline
#endif

#ifdef __cplusplus
}
#endif

#endif   /* AA_TYPES_H */

