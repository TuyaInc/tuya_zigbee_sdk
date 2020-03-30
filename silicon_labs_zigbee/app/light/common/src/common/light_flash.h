/*
 * @Author: wls
 * @email: wuls@tuya.com
 * @LastEditors: wls
 * @file name: light_flash.h
 * @Description: light production flash read/write include file
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-04-26 13:55:40
 * @LastEditTime: 2019-05-06 14:37:13
 */

#ifndef __LIHGT_FLASH_H__
#define __LIHGT_FLASH_H__


#include "soc_flash.h"


#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */


/**
 * @berief: read flash data type
 * @param {type} none
 * @return: none
 * @retval: none
 */
enum{
    DTYPE_OEM = 0,
    DTYPE_APP,
    DTYPE_RESTCNT,
    DTYPED_PORD,
};




#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif  /* __LIHGT_FLASH_H__ */
