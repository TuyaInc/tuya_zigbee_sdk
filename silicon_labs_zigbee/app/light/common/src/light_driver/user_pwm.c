/*
 * @Author: wls
 * @email: wuls@tuya.com
 * @LastEditors:jin lu
 * @file name: sm2135.c
 * @Description: SM2135 IIC driver program
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-04-16 11:44:21
 * @LastEditTime: 2019-05-28 15:21:35
 */
#include "user_pwm.h"



#define PWM_MAX_FREQ    20000           ///< PWM MAX Frequency 20K
#define PWM_MIN_FREQ    100             ///< PWM MIN Frequency 100
#define PWM_MAX_DUTY    1000            ///< PWM MAX Duty 1000 --> Precision 0.1%

STATIC USER_PWM_INIT_S PWM_PARM;        ///< PWM Settings
STATIC USHORT_T pwm_channel_num = -1;   ///< PWM channel num

/**
 * @berief: user pwm init
 * @param {IN USER_PWM_INIT_S *Init_PARM --> INIT Parm
 *          usFreq -> PWM Frequency unit Hz
 *          usDuty -> PWM Init duty unit 0.1% 
 *          *ucList -> RGBCW GPIO list
 *          Polarity -> PWM output polarity
 *              TRUE -> positive
 *              FAlse -> negative}
 * @attention *ucList parm set
 *              List order is always RGBCW !
 *              C       e.g List = {13}
 *              CW      e.g List = {13,14}
 *              RGB     e.g List = {7,8,9}
 *              RGBC    e.g List = {7,8,9,13}
 *              RGBCW   e.g List = {7,8,9,13,14}    
 * @return: none
 * @retval: none
 */
OPERATE_RET USER_PWM_Init(IN USER_PWM_INIT_S *Init_PARM)
{
    if( NULL == Init_PARM ) {
        PR_ERR("USER PWM init is invalid!");
        return OPRT_INVALID_PARM;
    }
  
    if( ( Init_PARM ->usFreq < PWM_MIN_FREQ ) || ( Init_PARM ->usFreq > PWM_MAX_FREQ ) ) {
        PR_ERR("USER PWM init is invalid!");
        return OPRT_INVALID_PARM;
    }

    if( Init_PARM ->usDuty > PWM_MAX_DUTY ) {
        PR_ERR("USER PWM init is invalid!");
        return OPRT_INVALID_PARM;
    }

    pwm_channel_num = Init_PARM->ucListNum;
    if( (pwm_channel_num <= 0) || (pwm_channel_num > 5) ){
        PR_ERR("USER PWM init is invalid!");
        return OPRT_INVALID_PARM;
    }
    
    memcpy(&PWM_PARM, Init_PARM, SIZEOF(USER_PWM_INIT_S));
   
    vSocPwmInit(PWM_PARM.usFreq, PWM_PARM.usDuty, Init_PARM->ucListNum, PWM_PARM.ucList);

    return OPRT_OK;
}

/**
 * @berief: pwm send data out
 * @param {IN USER_PWM_SendData -> PWM send data 
 *          R_Value -> R send duty,rang from 0 to 1000,unit 0.1%
 *          G_Value -> G send duty,rang from 0 to 1000,unit 0.1%
 *          B_Value -> B send duty,rang from 0 to 1000,unit 0.1%
 *          C_Value -> C send duty,rang from 0 to 1000,unit 0.1%
 *          W_Value -> W send duty,rang from 0 to 1000,unit 0.1%}
 * @return: OPERATE_RET
 * @retval: OPRT_INVALID_PARM -> invalid parm
 */
OPERATE_RET USER_PWM_SendOut(IN USER_PWM_SendData_S SendData)
{
    if( (SendData.R_Value > PWM_MAX_DUTY) || (SendData.G_Value > PWM_MAX_DUTY) || \
        (SendData.B_Value > PWM_MAX_DUTY) || (SendData.C_Value > PWM_MAX_DUTY) || \
        (SendData.W_Value > PWM_MAX_DUTY) ) {
        PR_ERR("USER PWM send data is invalid!");
        return OPRT_INVALID_PARM;
    }

    if( PWM_PARM.bPolarity == FALSE) {
        SendData.R_Value = PWM_MAX_DUTY - SendData.R_Value;
        SendData.G_Value = PWM_MAX_DUTY - SendData.G_Value;
        SendData.B_Value = PWM_MAX_DUTY - SendData.B_Value;
        SendData.C_Value = PWM_MAX_DUTY - SendData.C_Value;
        SendData.W_Value = PWM_MAX_DUTY - SendData.W_Value;
    }

    switch (pwm_channel_num)
    {
        case 1: /* 1 way -- C */           
            vSocPwmSetDuty(0,SendData.C_Value);  /* send C value */
            break;
        case 2: /* 2 way -- CW */
            vSocPwmSetDuty(0,SendData.C_Value);  /* send C value */
            vSocPwmSetDuty(1,SendData.W_Value);  /* send W value */
            break;
        case 3: /* 3 way -- RGB */
            vSocPwmSetDuty(0,SendData.R_Value);  /* send R value */
            vSocPwmSetDuty(1,SendData.G_Value);  /* send G value */
            vSocPwmSetDuty(2,SendData.B_Value);  /* send B value */
            break;
        case 4: /* 4 way -- RGBC */
            vSocPwmSetDuty(0,SendData.R_Value);  /* send R value */
            vSocPwmSetDuty(1,SendData.G_Value);  /* send G value */
            vSocPwmSetDuty(2,SendData.B_Value);  /* send B value */
            vSocPwmSetDuty(3,SendData.C_Value);  /* send C value */
            break;
        case 5: /* 5 way -- RGBCW */
            vSocPwmSetDuty(0,SendData.R_Value);  /* send R value */
            vSocPwmSetDuty(1,SendData.G_Value);  /* send G value */
            vSocPwmSetDuty(2,SendData.B_Value);  /* send B value */
            vSocPwmSetDuty(3,SendData.C_Value);  /* send C value */
            vSocPwmSetDuty(4,SendData.W_Value);  /* send w value */
            break;
        default:
            break;
    }
    
    return OPRT_OK;
}
