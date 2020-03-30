/*
 * @Author: wls
 * @email: wuls@tuya.com
 * @LastEditors: wls
 * @file name: user_pwm.c
 * @Description: PWM driver program
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-04-16 11:44:21
 * @LastEditTime: 2019-06-12 17:20:52
 */

#include "user_pwm.h"



#define PWM_MAX_FREQ    20000           ///< PWM MAX Frequency 20K
#define PWM_MIN_FREQ    100             ///< PWM MIN Frequency 100
#define PWM_MAX_DUTY    1000            ///< PWM MAX Duty 1000 --> Precision 0.1%

STATIC user_pwm_init_t g_pwm_init;      ///< PWM Settings
STATIC BOOL_T user_pwm_init_flag = FALSE;

/**
 * @berief: user pwm init
 * @param {IN USER_PWM_INIT_S *Init_PARM --> INIT Parm
 *          usFreq -> PWM Frequency unit Hz
 *          usDuty -> PWM Init duty unit 0.1%
 *          ucList[5] -> RGBCW GPIO list
 *          ucChannel_num -> channel num
 *          bPolarity -> PWM output polarity
 *              TRUE -> positive
 *              FAlse -> negative}
 * @attention ucList[5] parm set
 *              List order is always RGBCW !
 * @return: none
 * @retval: none
 */
OPERATE_RET opUserPWM_Init(IN user_pwm_init_t *pwm_init)
{
    OPERATE_RET opRet = -1;
    UCHAR_T ucListTemp[5] = {0};

    if( NULL == pwm_init ) {
        app_print("USER PWM init is invalid!");
        return OPRT_INVALID_PARM;
    }

    if( ( pwm_init ->usFreq < PWM_MIN_FREQ ) || ( pwm_init ->usFreq > PWM_MAX_FREQ ) ) {
        app_print("USER PWM init is invalid!");
        return OPRT_INVALID_PARM;
    }

    if( pwm_init ->usDuty > PWM_MAX_DUTY ) {
        app_print("USER PWM init is invalid!");
        return OPRT_INVALID_PARM;
    }

    if( (pwm_init->ucChannel_num <= 0) || (pwm_init->ucChannel_num > 5) ){
        app_print("USER PWM init is invalid!");
        return OPRT_INVALID_PARM;
    }

    memcpy(&g_pwm_init, pwm_init, SIZEOF(user_pwm_init_t));

    switch(g_pwm_init.ucChannel_num)
    {
        case 1: /* 1 way -- C */
            ucListTemp[0] = g_pwm_init.ucList[3];
            memcpy(g_pwm_init.ucList, ucListTemp, SIZEOF(g_pwm_init.ucList));
            break;

        case 2: /* 2 way -- CW */
            ucListTemp[0] = g_pwm_init.ucList[3];
            ucListTemp[1] = g_pwm_init.ucList[4];
            memcpy(g_pwm_init.ucList, ucListTemp, SIZEOF(g_pwm_init.ucList));
            break;

        case 3: /* 3 way -- RGB */
        case 4: /* 4 way -- RGBC */
        case 5: /* 5 way -- RGBCW */
            /* don't need to process **/
            break;

        default:
            break;
    }

    opRet = opSocPwmInit(g_pwm_init.usFreq, g_pwm_init.usDuty, pwm_init->ucChannel_num, g_pwm_init.ucList);
    if(opRet != OPRT_OK) {
        app_print("PWM soc init error!");
        return OPRT_COM_ERROR;
    }

    app_print("pwm init ok!");
    user_pwm_init_flag = TRUE;

    return OPRT_OK;
}

/**
 * @berief: pwm send data out
 * @param {IN user_pwm_color_t *color_data -> PWM send data
 *          R_Value -> R send duty,rang from 0 to 1000,unit 0.1%
 *          G_Value -> G send duty,rang from 0 to 1000,unit 0.1%
 *          B_Value -> B send duty,rang from 0 to 1000,unit 0.1%
 *          C_Value -> C send duty,rang from 0 to 1000,unit 0.1%
 *          W_Value -> W send duty,rang from 0 to 1000,unit 0.1%}
 * @return: OPERATE_RET
 * @retval: OPRT_INVALID_PARM -> invalid parm
 */
OPERATE_RET opUserPWM_SetRGBCW(IN user_pwm_color_t *color_data)
{
    if(user_pwm_init_flag != TRUE){
        app_print("user pwm not init!");
        return OPRT_INVALID_PARM;
    }

    if( (color_data ->usR_Value > PWM_MAX_DUTY) || (color_data ->usG_Value > PWM_MAX_DUTY) || \
        (color_data ->usB_Value > PWM_MAX_DUTY) || (color_data -> usC_Value > PWM_MAX_DUTY) || \
        (color_data ->usW_Value > PWM_MAX_DUTY) ) {
        app_print("USER PWM data is invalid!");
        return OPRT_INVALID_PARM;
    }

    if( g_pwm_init.bPolarity == FALSE) {
        color_data ->usR_Value = PWM_MAX_DUTY - color_data ->usR_Value;
        color_data ->usG_Value = PWM_MAX_DUTY - color_data ->usG_Value;
        color_data ->usB_Value = PWM_MAX_DUTY - color_data ->usB_Value;
        color_data ->usC_Value = PWM_MAX_DUTY - color_data ->usC_Value;
        color_data ->usW_Value = PWM_MAX_DUTY - color_data ->usW_Value;
    }

    switch(g_pwm_init.ucChannel_num)
    {
        case 1: /* 1 way -- C */
            vSocPwmSetDuty(0, color_data ->usC_Value);  /* send C value */
            break;
        case 2: /* 2 way -- CW */
            vSocPwmSetDuty(0, color_data ->usC_Value);  /* send C value */
            vSocPwmSetDuty(1, color_data ->usW_Value);  /* send W value */
            break;
        case 3: /* 3 way -- RGB */
            vSocPwmSetDuty(0, color_data ->usR_Value);  /* send R value */
            vSocPwmSetDuty(1, color_data ->usG_Value);  /* send G value */
            vSocPwmSetDuty(2, color_data ->usB_Value);  /* send B value */
            break;
        case 4: /* 4 way -- RGBC */
            vSocPwmSetDuty(0, color_data ->usR_Value);  /* send R value */
            vSocPwmSetDuty(1, color_data ->usG_Value);  /* send G value */
            vSocPwmSetDuty(2, color_data ->usB_Value);  /* send B value */
            vSocPwmSetDuty(3, color_data ->usC_Value);  /* send C value */
            break;
        case 5: /* 5 way -- RGBCW */
            vSocPwmSetDuty(0, color_data ->usR_Value);  /* send R value */
            vSocPwmSetDuty(1, color_data ->usG_Value);  /* send G value */
            vSocPwmSetDuty(2, color_data ->usB_Value);  /* send B value */
            vSocPwmSetDuty(3, color_data ->usC_Value);  /* send C value */
            vSocPwmSetDuty(4, color_data ->usW_Value);  /* send w value */
            break;
        default:
            app_print("pwm drive channel set error!!");
            break;
    }
    return OPRT_OK;
}

