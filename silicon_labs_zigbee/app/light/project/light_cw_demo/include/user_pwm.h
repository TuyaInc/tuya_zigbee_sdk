/*
 * @Author: wls
 * @email: wuls@tuya.com
 * @LastEditors: wls
 * @file name: user_pwm.h
 * @Description: pwm send out include file
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-04-22 20:01:00
 * @LastEditTime: 2019-05-10 17:41:15
 */


#ifndef __USER_PWM_H__
#define __USER_PWM_H__


#include "light_types.h"
#include "soc_pwm.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * @brief pwm init data structure
 * pwm send data structure
 */
typedef struct
{
    USHORT_T usFreq;            ///< PWM Frequency
    USHORT_T usDuty;            ///< PWM Init duty
    UCHAR_T ucList[5];          ///< GPIO List
    UCHAR_T ucChannel_num;      ///< GPIO List length
    BOOL_T bPolarity;           ///< PWM output polarity
}user_pwm_init_t;

/**
 * @brief pwm send data structure
 * pwm send data structure
 */
typedef struct
{
    USHORT_T usR_Value;       ///< R value,rang from 0 to 1000
    USHORT_T usG_Value;       ///< G value,rang from 0 to 1000
    USHORT_T usB_Value;       ///< B value,rang from 0 to 1000
    USHORT_T usC_Value;       ///< C value,rang from 0 to 1000
    USHORT_T usW_Value;       ///< W value,rang from 0 to 1000
}user_pwm_color_t;

/**
 * @berief: user pwm init
 * @param {IN USER_PWM_INIT_S *Init_PARM --> INIT Parm
 *          Freq -> PWM Frequency unit Hz
 *          Duty -> PWM Init duty unit 0.1%
 *          *List -> RGBCW GPIO list
 *          Polarity -> PWM output polarity
 *              TRUE -> positive
 *              FAlse -> negative}
 * @attention *List parm set
 *              List order is always RGBCW !
 * @return: none
 * @retval: none
 */
OPERATE_RET opUserPWM_Init(IN user_pwm_init_t *pwm_init);

/**
 * @berief: pwm send data out
 * @param {IN user_pwm_color_t *color_data -> PWM send data
 *          R_Value -> R send duty,rang from 0 to 1000,unit 0.1%
 *          G_Value -> G send duty,rang from 0 to 1000,unit 0.1%
 *          B_Value -> B send duty,rang from 0 to 1000,unit 0.1%
 *          C_Value -> C send duty,rang from 0 to 1000,unit 0.1%
 *          W_Value -> W send duty,rang from 0 to 1000,unit 0.1%}
 * @return: OPERATE_RET
 * @retval: OPRT_INVALID_PARM -> invalid parm
 */
OPERATE_RET opUserPWM_SetRGBCW(IN user_pwm_color_t *color_data);



#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif  /* __USER_PWM_H__ */
