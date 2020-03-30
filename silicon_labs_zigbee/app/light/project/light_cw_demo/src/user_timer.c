/*
 * @Author: wls
 * @email: wuls@tuya.com
 * @LastEditors:
 * @file name: sm2135.c
 * @Description: SM2135 IIC driver program
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-04-16 11:44:21
 * @LastEditTime: 2019-08-07 16:25:27
 */

#include "user_timer.h"
#include "soc_timer.h"
#include "light_types.h"
#include "tuya_zigbee_sdk.h"

/**
 * @berief: user hardware time start
 * @param {IN UINT_T cycle -> hardware timer period, unit:us}
 * @param {IN VOID* callback -> callback proc}
 * @return: OPERATE_RET
 * @retval: OPERATE_RET
 */
OPERATE_RET opUserHWTimerStart(IN UINT_T cyclye_us, IN VOID* callback)
{
    OPERATE_RET opRet = -1;

    if(NULL == callback) {
        app_print("user hardware time callback can't be null!");
        return OPRT_INVALID_PARM;
    }

    opRet = opSocHWTimerStart(cyclye_us, callback);
    if(opRet != OPRT_OK) {
        app_print("User hardware start error!");
        return opRet;
    }

    return OPRT_OK;
}

/**
 * @berief: user hardware time stop
 * @param {none}
 * @return: OPERATE_RET
 * @retval: OPERATE_RET
 */
OPERATE_RET opUserHWTimerStop(VOID)
{
    OPERATE_RET opRet = -1;

    opRet = opSocHWTimerStop();

    return opRet;
}

/**
 * @berief: user software time start
 * @param {IN UCHAR_T timer_id -> software timer ID}
 * @param {IN UINT_T cyclye_ms -> timerout time,unit:ms}
 * @param {IN VOID* callback -> timerout handler}
 * @attention: soft time must can reload in anytime.
 * @retval: OPERATE_RET
 */
OPERATE_RET opUserSWTimerStart(IN UCHAR_T timer_id, IN UINT_T cyclye_ms, IN VOID* callback)
{
    OPERATE_RET opRet = -1;

    if(NULL == callback) {
        app_print("user software time callback can't be null!");
        return OPRT_INVALID_PARM;
    }

    opRet = opSocSWTimerStart(timer_id, cyclye_ms, callback);
    if(opRet != OPRT_OK) {
        app_print("User software start error!");
        return opRet;
    }

    return OPRT_OK;
}

/**
 * @berief: user software time stop
 * @param {IN UCHAR_T timer_id -> software timer_id}
 * @return: OPERATE_RET
 * @retval: none
 */
OPERATE_RET opUserSWTimerStop(IN UCHAR_T timer_id)
{
    OPERATE_RET opRet = -1;

    opRet = opSocSWTimerStop(timer_id);
    if(opRet != OPRT_OK) {
        app_print("User software stop error!");
        return opRet;
    }

    return OPRT_OK;
}

/**
 * @berief: user hardware time start
 * @param {IN UINT_T cycle -> hardware timer period, unit:us}
 * @param {IN VOID* callback -> callback proc}
 * @return: OPERATE_RET
 * @retval: OPERATE_RET
 */
BOOL_T bUserSoftTimerCheckValid(IN UCHAR_T timer_id)
{
    return bSocSWTimerStartCheck(timer_id);
}


