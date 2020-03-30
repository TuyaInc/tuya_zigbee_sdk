/*
 * @Author: jinlu
 * @email: jinlu@tuya.com
 * @LastEditors:   
 * @file name: soc_timer.h
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-05-06 10:00:26
 * @LastEditTime: 2019-05-27 18:07:01
 */

#ifndef __SOC_TIMER_H__
#define __SOC_TIMER_H__

#include "zigbee_sdk.h"
#include "light_types.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * @description: timer handler function type
 * @param {UCHAR_T} timer id has been triggered 
 * @return: none
 */
typedef void (*timer_func_t)(IN UCHAR_T);

/**
 * @description: timer stop function
 * @param {timer_id} timer to stop
 * @return: none
 */
VOID vSocTimerStop(IN UCHAR_T timer_id);

/**
 * @description: system timer start with timeout handler function
 * @param {timer_id} timer to start
 * @param {timeout_ms} timer run timeout time(ms)
 * @param {func} timer timeout handler function
 * @return: none
 */
VOID vSocTimerStart(IN UCHAR_T timer_id, IN UINT_T timeout_ms, IN timer_func_t func);

/**
 * @description: Check if the timer is running
 * @param {timer_id} timer to be check
 * @return: BOOL_T: true: running, false : not running
 */
BOOL_T vSocTimerStartCheck(IN UCHAR_T timer_id);



#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif  /* __SOC_PWM_H__ */
