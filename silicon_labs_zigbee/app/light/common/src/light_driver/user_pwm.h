/*
 * @Author: wls
 * @email: wuls@tuya.com
 * @LastEditors:jin lu   
 * @file name: user_pwm.h
 * @Description: pwm send out include file
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-04-22 20:01:00
 * @LastEditTime: 2019-05-28 11:23:34
 */


#ifndef __USER_PWM_H__
#define __USER_PWM_H__


#include "light_types.h"
#include "light_printf.h"
#include "light_tools.h"
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
    UCHAR_T *ucList;            ///< GPIO List 
    UCHAR_T ucListNum;          ///< GPIO List length
    BOOL_T bPolarity;            ///< PWM output polarity
}USER_PWM_INIT_S;


/**
 * @brief pwm send data structure
 * pwm send data structure
 */
typedef struct 
{
    USHORT_T R_Value;       ///< R value,rang from 0 to 1000
    USHORT_T G_Value;       ///< G value,rang from 0 to 1000
    USHORT_T B_Value;       ///< B value,rang from 0 to 1000
    USHORT_T C_Value;       ///< C value,rang from 0 to 1000
    USHORT_T W_Value;       ///< W value,rang from 0 to 1000
}USER_PWM_SendData_S;

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
 *              C       e.g List = {13}
 *              CW      e.g List = {13,14}
 *              RGB     e.g List = {7,8,9}
 *              RGBC    e.g List = {7,8,9,13}
 *              RGBCW   e.g List = {7,8,9,13,14}    
 * @return: none
 * @retval: none
 */
OPERATE_RET USER_PWM_Init(IN USER_PWM_INIT_S *Init_PARM);

/**
 * @berief: pwm send data out
 * @param {IN USER_PWM_SendData -> PWM send data 
 *          R_Value -> R send duty,rang from 0 to 1000,unit 0.1%
 *          G_Value -> G send duty,rang from 0 to 1000,unit 0.1%
 *          B_Value -> B send duty,rang from 0 to 1000,unit 0.1%
 *          C_Value -> C send duty,rang from 0 to 1000,unit 0.1%
 *          W_Value -> W send duty,rang from 0 to 1000,unit 0.1%}
 * @return: OPERATE_RET
 * @retval: OPRT_INVALID_PARM -> invalid parm
 */
OPERATE_RET USER_PWM_SendOut(IN USER_PWM_SendData_S SendData);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif  /* __USER_PWM_H__ */
