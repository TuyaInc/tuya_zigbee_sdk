/*
 * @Author: jinlu
 * @email: jinlu@tuya.com
 * @LastEditors:
 * @file name: light_init.c
 * @Description: light init process
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-04-28 14:35:54
 * @LastEditTime: 2019-08-19 16:22:49
 */
#include "light_types.h"
#include "light_control.h"
#include "tuya_zigbee_sdk.h"

void power_on_reset_data(void)
{
    opLightCtrlDataModeSet(0);
    opLightCtrlDataBrightSet(1000);
    opLightCtrlDataTemperatureSet(1000);
}

// C_PIN_OUT = Bright * Temper / MAX
// W_PIN_OUT = Bright - C = Bright * ( 1 - Temper/MAX )
/**
 * @berief: light hardware init
 *          get oem json set, and init hardware
 * @param {none}
 * @return: OPERATE_RET
 * @retval: OPERATE_RET
 */
STATIC OPERATE_RET opLightHardwareInit(VOID)
{
    OPERATE_RET opRet = -1;
    //opRet = opUserHWTimerStart(HW_TIMER_CYCLE_US,(VOID *)vLightCtrlHWTimerCB);//开启渐变定时器
     //if(opRet != OPRT_OK) {
     //   app_print("Light hardware timer init error!");
     //   return opRet;
    //}
    /* hardware reboot jude&proc */
    vLightCtrlHWRebootProc();   /* write recnt count! reload ctrl data! */

    return OPRT_OK;

}



/**
 * @berief: light init
 * @param {UINT_T HWTimercycle -> hardware time period,unit:us}
 * @return: OPERATE_RET
 * @retval: OPERATE_RET
 */
OPERATE_RET opLightInit(VOID)
{
    OPERATE_RET opRet = -1;

    opRet = opLightHardwareInit();

    opRet = opLightCtrlResetCntProcess(); //直接返回, 或者置CTRL参数为离网数据并保存后开始配网.
    if(opRet != OPRT_OK) {
        app_print("Light software init error!");
        //return OPRT_COM_ERROR;
    }
    power_on_reset_data();
    opLightCtrlProc();
    return OPRT_OK;
}


