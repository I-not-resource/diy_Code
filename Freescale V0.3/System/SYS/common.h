/*
 * File:        common.h
 * Purpose:     File to be included by all project files
 *
 * Notes:
 */

#ifndef _COMMON_H_
#define _COMMON_H_

/********************************************************************/

/*
 * Debug prints ON (#define) or OFF (#undef)
 */
#define DEBUG
#define DEBUG_PRINT

/*****************************设置使用到的数据类型CSL*****************************/  
typedef       unsigned     char          U8;   //无符号型   
typedef       unsigned     short int    U16;   
typedef       unsigned     long   int    U32; 

typedef                char         S8;    //有符号型    
typedef                short int   S16;    
typedef                long  int   S32;


/*****************************For My Old DRIVER CSL*****************/
/*
 * The basic data types
 */
typedef unsigned char		uint8;  /*  8 bits */
typedef unsigned short int	uint16; /* 16 bits */
typedef unsigned long int	uint32; /* 32 bits */

typedef char			    int8;   /*  8 bits */
typedef short int	        int16;  /* 16 bits */
typedef int		            int32;  /* 32 bits */

typedef volatile int8		vint8;  /*  8 bits */
typedef volatile int16		vint16; /* 16 bits */
typedef volatile int32		vint32; /* 32 bits */

typedef volatile uint8		vuint8;  /*  8 bits */
typedef volatile uint16		vuint16; /* 16 bits */
typedef volatile uint32		vuint32; /* 32 bits */
/********************************************************************/

#include "MK60D10.h"

#endif /* _COMMON_H_ */
