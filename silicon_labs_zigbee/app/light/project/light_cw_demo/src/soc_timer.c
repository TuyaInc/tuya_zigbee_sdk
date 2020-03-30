/*
 * @Author: jin lu
 * @email: jinlu@tuya.com
 * @Date: 2019-05-23 11:24:33
 * @file name:
 * @Description:
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 */
#include "tuya_zigbee_sdk.h"
#include "soc_timer.h"
#include "light_types.h"

STATIC BOOL_T hw_timer_enabled = FALSE;

#define SHADE_TIMER_ID  1
#define TIMER_ID_BASE   2

/**
 * @berief: SOC hardware time start
 * @param {none}
 * @return: OPERATE_RET
 * @retval: OPERATE_RET
 */
/*
OPERATE_RET opSocHWTimerStart(UINT_T cycle_us, VOID* callback)
{
    UINT_T cycle_100us = 0;

    if(callback == NULL){
         app_print("SW timer start ERROR");
        return OPRT_INVALID_PARM;
    }

    if(hw_timer_enabled != TRUE){
        hardware_timer_enable();
    }

    cycle_100us = cycle_us/100;
    shade_control_timer_id = timer_hardware_start_100us(cycle_100us, HARDWARE_TIMER_AUTO_RELOAD_ENABLE, (hardware_timer_func_t)callback);
    if(shade_control_timer_id==V_TIMER_ERR){
         app_print("SW timer start ERROR");
        return OPRT_COM_ERROR;
    }

    hw_timer_enabled = TRUE;

    return OPRT_OK;
}
*/
OPERATE_RET opSocHWTimerStart(UINT_T cycle_us, VOID* callback)
{
    UINT_T cycle_100us = 0;

    if(callback == NULL){
         app_print("SW timer start ERROR");
        return OPRT_INVALID_PARM;
    }

    if(hw_timer_enabled != TRUE){
        hardware_timer_enable();
        hw_timer_enabled = TRUE;
    }

    cycle_100us = cycle_us/100;
    timer_hardware_start_with_id(SHADE_TIMER_ID, cycle_100us, HARDWARE_TIMER_AUTO_RELOAD_ENABLE, (hardware_timer_func_t)callback);

    return OPRT_OK;
}




/**
 * @berief: SOC hardware time stop
 * @param {none}
 * @return: OPERATE_RET
 * @retval: OPERATE_RET
 */
OPERATE_RET opSocHWTimerStop(VOID)
{
    timer_hardware_stop_100us(SHADE_TIMER_ID);
    return OPRT_OK;
}


/**
 * @description: timer stop function
 * @param {timer_id} timer to stop
 * @return: none
 */
OPERATE_RET opSocSWTimerStop(IN UCHAR_T timer_id)
{
    timer_hardware_stop_100us(TIMER_ID_BASE + timer_id);
    return OPRT_OK;
}

/**
 * @description: system timer start with timeout handler function
 * @param {timer_id} timer to start
 * @param {timeout_ms} timer run timeout time(ms)
 * @param {func} timer timeout handler function
 * @return: none
 */
OPERATE_RET opSocSWTimerStart(IN UCHAR_T timer_id, IN UINT_T timeout_ms, IN VOID* func)
{
    UINT_T cycle_100us = 0;

    if(func == NULL){
         app_print("SW timer start ERROR");
        return OPRT_INVALID_PARM;
    }
    if(hw_timer_enabled != TRUE){
        hardware_timer_enable();
        hw_timer_enabled = TRUE;
    }
    cycle_100us = timeout_ms*10;
    timer_hardware_start_with_id(TIMER_ID_BASE+timer_id, cycle_100us, HARDWARE_TIMER_AUTO_RELOAD_DISABLE, (hardware_timer_func_t)func);

    return OPRT_OK;
}

/**
 * @description: Check if the timer is running
 * @param {timer_id} timer to be check
 * @return: BOOL_T: true: running, false : not running
 */
BOOL_T bSocSWTimerStartCheck(IN UCHAR_T timer_id)
{
    return timer_hardware_is_active(TIMER_ID_BASE + timer_id);
}
