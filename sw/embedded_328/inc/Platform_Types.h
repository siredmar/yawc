#ifndef PLATFORM_TYPES_H
#define PLATFORM_TYPES_H

#ifndef NULL
#define NULL ((void*)(0))
#endif
#ifndef NULL_PTR
#define NULL_PTR ((void*)(0))
#endif

#define FALSE 			0
#define TRUE 			1

#define MASK_8BIT_UI8		0xFF
#define MASK_8BIT_HI_UI8	0xF0
#define MASK_8BIT_LO_UI8	0x0F
#define MASK_16BIT_LO_UI16	0x00FF
#define MASK_16BIT_HI_UI16	0xFF00
#define MASK_32BIT_1BYTE_UI32	0x000000FF
#define MASK_32BIT_2BYTE_UI32	0x0000FF00
#define MASK_32BIT_3BYTE_UI32	0x00FF0000
#define MASK_32BIT_4BYTE_UI32	0xFF000000

#define CHECK_BIT(var, pos) ((var) & (pos))
#define GET_BIT(var, pos) (((var) >> (pos)) & 0x01u)
#define SET_BIT(var, pos) ((var) |= (1 << (pos)))
#define CLEAR_BIT(var, pos) ((var) &= ~(1 << (pos)))
#define TOGGLE_BIT(var, pos) ((var) ^= (1 << (pos)))
#define CLEAR_BITMASK(var, mask) ((var) &= ~(mask))
#define SET_BITMASK(var, mask) ((var) = (var) | (mask))
#define CHECK_BITMASK(var, mask) ((var) & (mask) == (mask))

#define BYTE0 (0u)
#define BYTE1 (1u)
#define BYTE2 (2u)
#define BYTE3 (3u)
#define GET_BYTE_FROM_4BYTE(val, byte) (((val) >> ((byte)*8u)) & 0xFFu)
#define GET_BYTE_FROM_2BYTE(val, byte) (((val) >> ((byte)*8u)) & 0xFFu)

#define BUILD_16BIT_FROM_2BYTE(msb, lsb) (((msb) << 8u) & 0xFF00u | (lsb) & 0x00FFu)
#define BUILD_32IT_FROM_4BYTE(mmsb, mlsb, lmsb, llsb) (((((uint32)(mmsb)) << 24u) & 0xFF000000u) | ((((uint32)(mlsb)) << 16u) & 0x00FF0000u) | ((((uint32)(lmsb)) << 8u) & 0x0000FF00u) | (((uint32)(llsb)) & 0x000000FFu))

typedef unsigned char      boolean;
typedef boolean            bool;
typedef signed char        sint8;
typedef unsigned char      uint8;
typedef signed short       sint16;
typedef unsigned short     uint16;
typedef signed long        sint32;
typedef unsigned long      uint32;
typedef signed long long   sint64;
typedef unsigned long long uint64;
typedef float              float32;
typedef double             float64;



#endif /* PLATFORM_TYPES_H */

