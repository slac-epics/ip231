
#ifndef EDT_TYPES_H
#define EDT_TYPES_H

/****************************************/
/* Macros which define types and type combinations */
/****************************************/
 
#define TYPE_NULL 0

#define TYPE_CHAR 1
#define TYPE_BYTE 2
#define TYPE_SHORT 3
#define TYPE_USHORT 4
#define TYPE_LONG 7
#define TYPE_ULONG 8
#define TYPE_FLOAT 9
#define TYPE_DOUBLE 10

/* Shortcuts for type = byte, n = 3 or 4 */

#define TYPE_RGB	11
#define TYPE_BGR	12
#define TYPE_RGBA	13
#define	TYPE_BGRA	14
#define TYPE_RGB15  15
#define TYPE_RGB16  16

#define TYPE_RGB48	17
#define TYPE_BGR48	18

#define TYPE_MONO	19


/* Defines as shorthand for combining type values into constants */

/* Monadic functions - one input, one output */
#define FUNC_MONAD(type1, type2) ((type2 << 8) | type1)

/* Dyadic functions - two inputs, one output */
#define FUNC_DYAD(type1, type2, type3) ((type3 << 16) | (type2 << 8) | type1)


#define FUNC_BYTE_BYTE FUNC_MONAD(TYPE_BYTE, TYPE_BYTE)
#define FUNC_BYTE_USHORT FUNC_MONAD(TYPE_BYTE, TYPE_USHORT)
#define FUNC_USHORT_BYTE FUNC_MONAD(TYPE_USHORT, TYPE_BYTE)
#define FUNC_USHORT_USHORT FUNC_MONAD(TYPE_USHORT, TYPE_USHORT)

#define FUNC_RGB_BGR FUNC_MONAD(TYPE_RGB, TYPE_BGR)
#define FUNC_BGR_BGR FUNC_MONAD(TYPE_BGR, TYPE_BGR)
#define FUNC_BYTE_BGR FUNC_MONAD(TYPE_BYTE, TYPE_BGR)
#define FUNC_USHORT_BGR FUNC_MONAD(TYPE_USHORT, TYPE_BGR)

#define FUNC_RGB_RGB FUNC_MONAD(TYPE_RGB, TYPE_RGB)
#define FUNC_BGR_RGB FUNC_MONAD(TYPE_BGR, TYPE_RGB)
#define FUNC_BYTE_RGB FUNC_MONAD(TYPE_BYTE, TYPE_RGB)
#define FUNC_USHORT_RGB FUNC_MONAD(TYPE_USHORT, TYPE_RGB)

#define FUNC_RGB_RGBA FUNC_MONAD(TYPE_RGB, TYPE_RGBA)
#define FUNC_BGR_RGBA FUNC_MONAD(TYPE_BGR, TYPE_RGBA)
#define FUNC_BYTE_RGBA FUNC_MONAD(TYPE_BYTE, TYPE_RGBA)
#define FUNC_USHORT_RGBA FUNC_MONAD(TYPE_USHORT, TYPE_RGBA)

#define FUNC_BGR_BGRA FUNC_MONAD(TYPE_BGR, TYPE_BGRA)
#define FUNC_BYTE_BGRA FUNC_MONAD(TYPE_BYTE, TYPE_BGRA)
#define FUNC_USHORT_BGRA FUNC_MONAD(TYPE_USHORT, TYPE_BGRA)

#define FUNC_BGR_RGB15 FUNC_MONAD(TYPE_BGR, TYPE_RGB15)
#define FUNC_BYTE_RGB15 FUNC_MONAD(TYPE_BYTE, TYPE_RGB15)
#define FUNC_USHORT_RGB15 FUNC_MONAD(TYPE_USHORT, TYPE_RGB15)

#define FUNC_BGR_RGB16 FUNC_MONAD(TYPE_BGR, TYPE_RGB16)
#define FUNC_BYTE_RGB16 FUNC_MONAD(TYPE_BYTE, TYPE_RGB16)
#define FUNC_USHORT_RGB16 FUNC_MONAD(TYPE_USHORT, TYPE_RGB16)


typedef unsigned char byte;

#ifdef WIN32
typedef unsigned short ushort;
#endif

/* Macro to return whether type is a signed / unsigned type */

#define IsSigned(type) (((type) &1) || ((type) >= TYPE_FLOAT))

#define TypeSize(type) ((type < TYPE_FLOAT)?((type+1) >> 1) : \
(type == TYPE_DOUBLE)? 8 : \
(type == TYPE_BGR || type == TYPE_RGB) ? 3 : \
(type == TYPE_RGB15 || type == TYPE_RGB16)? 2 : \
(type == TYPE_MONO)? 0.125 : 4)

#define EDT_MAX_COLOR_PLANES 4

/* Color tag values - pixel color order  */

#define COLOR_MONO 0
#define COLOR_RGB	1
#define COLOR_RGBA	2
#define COLOR_BGR	3
#define COLOR_BGRA	4

#endif

