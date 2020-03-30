/*
 * @Author: wls
 * @email: wuls@tuya.com
 * @LastEditors:   
 * @file name: light_types.h
 * @Description: all typedef 
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-04-22 20:01:00
 * @LastEditTime: 2019-08-23 20:36:08
 */


#ifndef __LIHGT_TYPES_H__
#define __LIHGT_TYPES_H__

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#ifndef VOID
typedef void VOID;
#endif
typedef void *PVOID_T;

#ifndef bool_t
typedef unsigned char bool_t;
#endif

typedef bool_t BOOL_T;
typedef BOOL_T *PBOOL_T;

typedef char CHAR_T;
typedef signed char SCHAR_T;
typedef unsigned char UCHAR_T;
typedef char *PCHAR_T;

typedef short SHORT_T;
typedef unsigned short USHORT_T;
typedef short *PSHORT_T;

typedef signed int INT_T;
typedef int *PINT_T;
typedef unsigned int UINT_T;
typedef unsigned int *PUINT_T;

typedef long LONG_T;
typedef unsigned long ULONG_T;
typedef long *PLONG_T;

typedef unsigned short WORD_T;
typedef WORD_T *PWORD_T;
typedef unsigned int DWORD_T;
typedef DWORD_T *PDWORD_T;

typedef float FLOAT_T;
typedef FLOAT_T *PFLOAT_T;

typedef long long DLONG_T;
typedef DLONG_T *PDLONG_T;

typedef unsigned long long DDWORD_T;
typedef DDWORD_T *PDDWORD_T;

typedef int OPERATE_RET;

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef VOID
#define VOID void
#endif

#ifndef CONST
#define CONST const
#endif

#ifndef STATIC
#define STATIC static
#endif

#ifndef SIZEOF
#define SIZEOF sizeof
#endif

#ifndef INLINE
#define INLINE inline
#endif

#ifndef WEAK
#if defined (__IAR_SYSTEMS_ICC__)
#define WEAK __weak
#else
#define WEAK __attribute__((weak))
#endif
#endif

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif



#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#ifndef OPRT_OK
#define OPRT_OK            0
#endif
#ifndef OPRT_INVALID_PARM
#define OPRT_INVALID_PARM  1
#endif
#ifndef OPRT_COM_ERROR
#define OPRT_COM_ERROR     2
#endif

#define PIN_NOEXIST        0xFF  ///< default pin def



#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif  /* __LIHGT_TYPES_H__ */

