/*
 * @Author: wls
 * @email: wuls@tuya.com
 * @LastEditors:   
 * @file name: soc_pwm.c
 * @Description: Realtek 8710bn PWM proc
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-04-24 20:42:29
 * @LastEditTime: 2019-05-28 14:12:19
 */

#include "soc_pwm.h"


UCHAR_T g_light_channel = 0;
#define MAX_DUTY        1000            ///< PWM MAX Duty 1000 --> Precision 0.1%
/**
 * @berief: PWM SOC Init
 * @param {IN USHORT_T Frequency --> PWM Frequency}
 * @param {IN USHORT_T Duty --> PWM Init positive Duty}
 * @param {IN UCHAR_T channel_num --> PWM channel num}
 * @param {IN UCHART_T *GPIO_List --> PWM gpio set}
 * @return: none
 * @retval: none
 */

VOID vSocPwmInit(IN USHORT_T Frequency, IN UCHAR_T Positive_duty_cycle, IN UCHAR_T Channel_num, IN UCHAR_T *GPIO_List)
{
    UCHAR_T i=0;
    pwm_gpio_t *pwm_gpio_list = (pwm_gpio_t *)malloc(Channel_num * sizeof(pwm_gpio_t));
    memset(pwm_gpio_list,0,Channel_num * sizeof(pwm_gpio_t));

    g_light_channel = Channel_num;

    for(i=0;i<Channel_num;i++){
        UCHAR_T pin_num = *(GPIO_List+i);
        pwm_gpio_list[i].port = xSocGpioGetPort(pin_num);
        pwm_gpio_list[i].pin = xSocGpioGetPin(pin_num);
        pwm_gpio_list[i].lv = Positive_duty_cycle? 1:0;
    }

    switch (Channel_num){
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

    hal_pwm_init(pwm_gpio_list, Channel_num,  Frequency);
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
VOID vSocPwmSetDuty(IN UCHAR_T Channel, IN USHORT_T Duty)  
{
    if(g_light_channel == 2 || g_light_channel == 5)
    {
        if(Channel == g_light_channel - 1)
        {
            USHORT_T duty_revert = MAX_DUTY - Duty;
            set_pwm_duty(Channel, duty_revert, MAX_DUTY);
            return;
        }
    }
    set_pwm_duty(Channel,Duty,MAX_DUTY);
}




