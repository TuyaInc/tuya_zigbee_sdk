/*
 * @Author: wls
 * @email: wuls@tuya.com
 * @LastEditors:
 * @file name: soc_pwm.h
 * @Description: Realtek 8710bn PWM include file
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-04-24 20:42:29
 * @LastEditTime: 2019-06-14 09:56:51
 */

#ifndef __SOC_PWM_H__
#define __SOC_PWM_H__

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */
#include "light_types.h"
#include "tuya_zigbee_sdk.h"


/**
 * @berief: PWM SOC Init
 * @param {IN USHORT_T frequency --> PWM frequency,unit:HZ}
 * @param {IN USHORT_T positive_duty_cycle --> PWM Init Duty(positive duty),range:0 ~ 1000}
 * @param {IN UCHAR_T channel_num --> PWM channel num}
 * @param {IN UCHART_T *gpio_list --> PWM gpio set}
 * @return: OPERATE_RET
 * @retval: OPERATE_RET
 */
OPERATE_RET opSocPwmInit(IN USHORT_T frequency, IN UCHAR_T positive_duty_cycle, IN UCHAR_T channel_num, IN UCHAR_T *gpio_list);

/**
 * @berief: PWM SOC SET Duty
 * @param {IN UCHAR_T channel -> pwm send out channel num}
 * @param {IN USHORT_T duty ->  pwm send duty,rang: 0~1000}
 * @attention channel --> according to the initlize order
 * @return: none
 * @retval: none
 */
VOID vSocPwmSetDuty(IN UCHAR_T channel, IN USHORT_T duty);



#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif  /* __SOC_PWM_H__ */
