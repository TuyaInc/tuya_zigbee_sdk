/**
 * @author Leon
 * @par email:
 *      zhangpeng@tuya.com
 * @file type_def.h
 * @brief common type define files
 * @copyright HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @par company
 *      http://www.tuya.com
 * @date 2020-03-20 17:29:33
 */

#ifndef  __TYPE_DEF_H__
#define  __TYPE_DEF_H__

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined (__IAR_SYSTEMS_ICC__) //for IAR
    #define VIRTUAL_FUNC __weak
#else //for gcc
    #define VIRTUAL_FUNC __attribute__((weak))
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

#ifndef STATIC
#define STATIC static
#endif

#ifndef CONST
#define CONST const
#endif

#ifndef SIZEOF
#define SIZEOF sizeof
#endif

#ifndef INLINE
#define INLINE inline
#endif


#ifndef NULL
    #ifdef __cplusplus
    #define NULL 0
    #else
    #define NULL ((void *)0)
    #endif
#endif

#ifndef bool_t
typedef uint8_t bool_t;
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

/**
 * @brief EUI 64-bit ID (an IEEE address).
 */
typedef uint8_t Device_Mac_64[8];

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
