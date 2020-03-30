/*
 * @Author: wls
 * @email: wuls@tuya.com
 * @LastEditors:
 * @file name: user_timer.h
 * @Description: set timer include file
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-04-22 20:01:00
 * @LastEditTime: 2019-07-01 11:38:17
 */


#ifndef __USER_TIMER_H__
#define __USER_TIMER_H__


#include "light_types.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * @berief: user hardware time start
 * @param {IN UINT_T cycle -> hardware timer period, unit:us}
 * @param {IN VOID* callback -> callback proc}
 * @return: OPERATE_RET
 * @retval: OPERATE_RET
 */
OPERATE_RET opUserHWTimerStart(IN UINT_T cyclye_us, IN VOID* callback);
/**
 * @berief: user hardware time stop
 * @param {none}
 * @return: OPERATE_RET
 * @retval: OPERATE_RET
 */
OPERATE_RET opUserHWTimeStop(VOID);

/**
 * @berief: user software time start
 * @param {IN UCHAR_T timer_id -> software timer ID}
 * @param {IN UINT_T cyclye_ms -> timerout time,unit:ms}
 * @param {IN VOID* callback -> timerout handler}
 * @attention: soft time must can reload in anytime.
 * @retval: OPERATE_RET
 */
OPERATE_RET opUserSWTimerStart(IN UCHAR_T timer_id, IN UINT_T cyclye_ms, IN VOID* callback);

/**
 * @berief: user software time stop
 * @param {IN UCHAR_T timer_id -> software timer_id}
 * @return: OPERATE_RET
 * @retval: none
 */
OPERATE_RET opUserSWTimerStop(IN UCHAR_T timer_id);

/**
 * @berief: check if user software timer valid
 * @param {IN UCHAR_T timer_id -> software timer_id}
 * @return: true/false
 */
BOOL_T bUserSoftTimerCheckValid(IN UCHAR_T timer_id);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif  /* __USER_TIMER_H__ */
