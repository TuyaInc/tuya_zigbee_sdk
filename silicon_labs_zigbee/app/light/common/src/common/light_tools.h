/*
 * @Author: wls
 * @email: wuls@tuya.com
 * @LastEditors:   
 * @file name: light_toolkit.h
 * @Description: light common tool include file
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-04-26 13:55:40
 * @LastEditTime: 2019-05-28 15:24:01
 */

#ifndef __LIHGT_TOOLS_H__
#define __LIHGT_TOOLS_H__

#include "light_types.h"


#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

  
#define size_get(x) (sizeof(x)/sizeof(x[0]))
  
  
/**
 * @berief: get the max value in 5 numbers
 * @param {UINT_T a
 *         UINT_T b
 *         UINT c
 *         UINT_T d
 *         UINT_T e
 *         5 numbers} 
 * @return: Max value
 * @retval: none
 */
UINT_T uiLightTool_GetMaxValue(UINT_T a, UINT_T b, UINT_T c, UINT_T d, UINT_T e);

/**
 * @berief: get the absolute value
 * @param {value} 
 * @return: absolute value
 * @retval: none
 */
UINT_T uiLightTool_GetABSValue(INT_T value);

extern bool_t string_compare(char* str1,  char* str2);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif  /* __LIHGT_TOOLKIT_H__ */
