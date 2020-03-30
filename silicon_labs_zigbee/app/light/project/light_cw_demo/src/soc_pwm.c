/*
 * @Author: wls
 * @email: wuls@tuya.com
 * @LastEditors:
 * @file name: soc_pwm.c
 * @Description: Realtek 8710bn PWM proc
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-04-24 20:42:29
 * @LastEditTime: 2019-06-28 18:36:45
 */

#include "soc_pwm.h"
#include "light_types.h"
#include "soc_gpio.h"
#include "tuya_zigbee_sdk.h"


UCHAR_T g_light_channel = 0;
#define MAX_DUTY        1000              ///< PWM MAX Duty 1000 --> Precision 0.1%
/**
 * @berief: PWM SOC Init
 * @param {IN USHORT_T frequency --> PWM frequency}
 * @param {IN USHORT_T Duty --> PWM Init positive Duty}
 * @param {IN UCHAR_T channel_num --> PWM channel num}
 * @param {IN UCHART_T *gpio_list --> PWM gpio set}
 * @return: none
 * @retval: none
 */

OPERATE_RET opSocPwmInit(IN USHORT_T frequency, IN UCHAR_T positive_duty_cycle, IN UCHAR_T channel_num, IN UCHAR_T *gpio_list)
{
    UCHAR_T i=0;
    OPERATE_RET ret = OPRT_OK;
    pwm_gpio_t *pwm_gpio_list = (pwm_gpio_t *)malloc(channel_num * sizeof(pwm_gpio_t));
    if(pwm_gpio_list == NULL){
        app_print("malloc ERR");
        ret =  OPRT_COM_ERROR;
        goto RETURN;
    }
    memset(pwm_gpio_list,0,channel_num * sizeof(pwm_gpio_t));

    g_light_channel = channel_num;

    for(i=0;i<channel_num;i++){
        UCHAR_T pin_num = *(gpio_list+i);
        pwm_gpio_list[i].port = xSocGpioGetPort(pin_num);
         app_print("list[%d].port = %d,%d",i,pwm_gpio_list[i].port,pin_num);
        if(pwm_gpio_list[i].port == PIN_NUM_ERROR){
            app_print("SOC PWM init ERR PORT");
            ret =  OPRT_INVALID_PARM;
            goto RETURN;
        }
        pwm_gpio_list[i].pin = xSocGpioGetPin(pin_num);
        if(pwm_gpio_list[i].pin == PIN_NUM_ERROR){
            app_print("SOC PWM init ERR PIN");
            ret =  OPRT_INVALID_PARM;
            goto RETURN;
        }
        pwm_gpio_list[i].lv = positive_duty_cycle? GPIO_LEVEL_HIGH:GPIO_LEVEL_LOW;
    }
    switch (channel_num){
        case 1:
        break;
        case 2:
            pwm_gpio_list[1].invert = true;
        break;
        case 3:
        break;
        case 4:
        break;
        case 5:
            pwm_gpio_list[4].invert = true;
        break;
    }

    if(!hal_pwm_init(pwm_gpio_list, channel_num,  frequency)){
        app_print("SOC PWM init ERR");
        ret =  OPRT_INVALID_PARM;
        goto RETURN;
    }

RETURN:
    if(pwm_gpio_list != NULL){
        free(pwm_gpio_list);
    }
    return ret;
}

/**
 * @berief: pwm soc set positive duty
 * @param {USHORT_T duty --> pwm send duty}
 * @param {UCHAR_T channel --> pwm send out channel}
 * @attention channel --> according to the initlize order
 * @return: none
 * @retval: none
 */

// C,CW,RGB,RGBC,RGBCW
VOID vSocPwmSetDuty(IN UCHAR_T channel, IN USHORT_T duty)
{
    if(duty > MAX_DUTY) {
        app_print("PWM SET DUTY ERR");
        return ;
    }
    if(channel >= g_light_channel)
    {
        app_print("PWM SET DUTY CHANNEL ERR");\
        return;
    }
    if(g_light_channel == 2 || g_light_channel == 5) {
        if(channel == g_light_channel - 1) {
            if(!set_pwm_duty(channel, MAX_DUTY - duty, MAX_DUTY)){
                app_print("PWM SET DUTY ERR");
            }
        } else {
            if(!set_pwm_duty(channel, duty, MAX_DUTY)){
                app_print("PWM SET DUTY ERR");
            }
        }
    }else{
        if(!set_pwm_duty(channel, duty, MAX_DUTY)){
            app_print("PWM SET DUTY ERR");
        }
    }
}




