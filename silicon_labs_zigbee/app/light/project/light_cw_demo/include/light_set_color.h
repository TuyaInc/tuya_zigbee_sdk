/*
 * @Author: wls
 * @email: wuls@tuya.com
 * @LastEditors:
 * @file name: light_set_color.h
 * @Description: light set color(choose drive mode)include file
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-04-26 13:55:40
 * @LastEditTime: 2019-08-22 20:12:30
 */

#ifndef __LIHGT_SET_COLOR_H__
#define __LIHGT_SET_COLOR_H__

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include "light_types.h"
#include "user_pwm.h"


OPERATE_RET opLightDriveInit(IN user_pwm_init_t *light_config);

/**
 * @berief: light send control data
 * @param {usR_value} red color, range 0~1000
 * @param {usG_Value} Green color, range 0~1000
 * @param {usB_Value} Blue color, range 0~1000
 * @param {usC_Value} cold white color, range 0~1000
 * @param {usW_Value} warm white color, range 0~1000
 * @return: OPERATE_RET
 * @retval: OPERATE_RET
 */
OPERATE_RET opLightSetRGBCW(IN USHORT_T usR_value, IN USHORT_T usG_value, IN USHORT_T usB_value, IN USHORT_T usC_value, IN USHORT_T usW_value);




#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif  /* __LIHGT_SET_COLOR_H__ */
