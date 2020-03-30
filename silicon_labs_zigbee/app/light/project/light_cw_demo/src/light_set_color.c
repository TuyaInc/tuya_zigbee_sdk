/*
 * @Author: wls
 * @email: wuls@tuya.com
 * @LastEditors:
 * @file name: light_driver_adapter.c
 * @Description: light driver adapter proc
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-04-26 13:55:40
 * @LastEditTime: 2019-07-16 22:24:10
 */

#include "light_set_color.h"
#include "light_types.h"
#include "user_pwm.h"

OPERATE_RET opLightDriveInit(IN user_pwm_init_t *light_config)
{
    OPERATE_RET opRet = 0;
    opRet = opUserPWM_Init(light_config);
    if(opRet != OPRT_OK) {
        app_print("Light drive pwm init error!");
        return OPRT_INVALID_PARM;
    }
    return OPRT_OK;
}

/**
 * @berief: light send control data
 * @param {usR_value} red color, range 0~1000
 * @param {usG_Value} Green color, range 0~1000
 * @param {usB_Value} Blue color, range 0~1000
 * @param {usC_Value} cold white color, range 0~1000
 * @param {usW_Value} warm white color, range 0~1000
 * @return: OPERATE_RET
 * @retval: OPERATE_RET
 */
OPERATE_RET opLightSetRGBCW(IN USHORT_T usR_value, IN USHORT_T usG_value, IN USHORT_T usB_value, IN USHORT_T usC_value, IN USHORT_T usW_value)
{
    OPERATE_RET opRet = -1;
    //app_print("R G B C W: %x %x %x %x %x",usR_value,usG_value,usB_value,usC_value,usW_value);
    user_pwm_color_t pwm_color;
    //memset(&pwm_color, 0, SIZEOF(user_pwm_color_t));
    pwm_color.usR_Value = usR_value;   /* pwm color range 0 ~ 1000 */
    pwm_color.usG_Value = usG_value;
    pwm_color.usB_Value = usB_value;
    pwm_color.usC_Value = usC_value;
    pwm_color.usW_Value = usW_value;
    opRet = opUserPWM_SetRGBCW(&pwm_color); /* pwm set color */
    if(opRet != OPRT_OK) {
        app_print("PWM send data error!");
    }

    return OPRT_OK;
}

