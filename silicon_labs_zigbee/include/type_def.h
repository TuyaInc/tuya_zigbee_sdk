/*
 * @Author: Leon
 * @email: zhangpeng@tuya.com
 * @LastEditors:   
 * @file name: com_def.h
 * @Description: common micro define files
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-03-25 21:41:17
 * @LastEditTime: 2019-06-10 17:31:13
 */

#ifndef  __TYPE_DEF_H__
#define  __TYPE_DEF_H__

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
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
