/*
 * @Author: jinlu
 * @email: jinlu@tuya.com
 * @LastEditors:   
 * @file name: soc_timer.h
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-05-06 10:00:26
 * @LastEditTime: 2019-06-19 13:26:25
 */

#ifndef __SOC_TIMER_H__
#define __SOC_TIMER_H__

#include "light_types.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */


/**
 * @berief: SOC hardware time start
 * @param {IN UINT_T cycle_us -> cycle time,unit:us}
 * @param {IN VOID* callback -> callback}
 * @return: OPERATE_RET
 * @retval: none
 */
OPERATE_RET opSocHWTimerStart(IN UINT_T cycle_us, IN VOID* callback);

/**
 * @berief: SOC hardware time stop
 * @param {none} 
 * @return: OPERATE_RET
 * @retval: none
 */
OPERATE_RET opSocHWTimerStop(VOID);

/**
 * @berief: SOC software time start
 * @param {IN UCHAR_T timer_id -> timer id}
 * @param {IN UINT_T cycle_ms -> cycle time,unit:ms}
 * @param {IN VOID* callback -> callback}
 * @attention: timer_id must < SOFTWARE_TIMER_MAX(5)
 * @return: OPERATE_RET
 * @retval: none
 */
OPERATE_RET opSocSWTimerStart(IN UCHAR_T timer_id, IN UINT_T cycle_ms, IN VOID* callback);

/**
 * @berief: SOC software time stop
 * @param {IN UCHAR_T timer_id -> timer id} 
 * @return: OPERATE_RET
 * @retval: none
 */
OPERATE_RET opSocSWTimerStop(IN UCHAR_T timer_id);

/**
 * @description: Check if the timer is running
 * @param {timer_id} timer to be check
 * @return: BOOL_T: true: running, false : not running
 */
BOOL_T bSocSWTimerStartCheck(IN UCHAR_T timer_id);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif  /* __SOC_TIMER_H__ */
