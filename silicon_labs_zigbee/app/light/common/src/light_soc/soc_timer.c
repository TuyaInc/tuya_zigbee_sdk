/*
 * @Author: jin lu
 * @email: jinlu@tuya.com
 * @Date: 2019-05-23 11:24:33
 * @file name: 
 * @Description: 
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 */
#include "zigbee_sdk.h"
#include "soc_timer.h"

#define TIMER_ID_BASE DEV_EVT_1

/**
 * @description: timer stop function
 * @param {timer_id} timer to stop
 * @return: none
 */
VOID vSocTimerHandler(IN UCHAR_T timer_id)
{
    UCHAR_T local_timer_id = timer_id - TIMER_ID_BASE; //zigbee timer id start from 25




}


/**
 * @description: timer stop function
 * @param {timer_id} timer to stop
 * @return: none
 */
VOID vSocTimerStop(IN UCHAR_T timer_id)
{
    dev_timer_stop(TIMER_ID_BASE + timer_id);
}

/**
 * @description: system timer start with timeout handler function
 * @param {timer_id} timer to start
 * @param {timeout_ms} timer run timeout time(ms)
 * @param {func} timer timeout handler function
 * @return: none
 */
VOID vSocTimerStart(IN UCHAR_T timer_id, IN UINT_T timeout_ms, IN timer_func_t func)
{
    dev_timer_start_with_callback(TIMER_ID_BASE + timer_id, timeout_ms, func);
}

/**
 * @description: Check if the timer is running
 * @param {timer_id} timer to be check
 * @return: BOOL_T: true: running, false : not running
 */
BOOL_T vSocTimerStartCheck(IN UCHAR_T timer_id)
{
    return dev_timer_get_valid_flag(DEV_EVT_1 + timer_id);
}