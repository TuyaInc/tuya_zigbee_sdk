/*
 * @Author: wls
 * @email: wuls@tuya.com
 * @LastEditors:   
 * @file name: soc_pwm.h
 * @Description: Realtek 8710bn PWM include file
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-04-24 20:42:29
 * @LastEditTime: 2019-05-28 10:54:22
 */

#ifndef __SOC_PWM_H__
#define __SOC_PWM_H__

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include "light_types.h"
#include "zigbee_sdk.h"
#include "soc_gpio.h"
#include "light_printf.h"

/**
 * @berief: PWM SOC Init
 * @param {IN USHORT_T Frequency --> PWM Frequency}
 * @param {IN USHORT_T Duty --> PWM Init Duty}
 * @param {IN UCHAR_T channel_num --> PWM channel num}
 * @param {IN UCHART_T *GPIO_List --> PWM gpio set}
 * @return: none
 * @retval: none
 */
VOID vSocPwmInit(IN USHORT_T Frequency, IN UCHAR_T duty, IN UCHAR_T Channel_num, IN UCHAR_T *GPIO_List);

/**
 * @berief: PWM SOC SET Duty
 * @param {USHORT_T duty --> pwm send duty}
 * @param {UCHAR_T channel --> pwm send out channel}
 * @attention channel --> according to the initlize order
 * @return: none
 * @retval: none
 */
VOID vSocPwmSetDuty(IN UCHAR_T Channel, IN USHORT_T Duty);



#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif  /* __SOC_PWM_H__ */
