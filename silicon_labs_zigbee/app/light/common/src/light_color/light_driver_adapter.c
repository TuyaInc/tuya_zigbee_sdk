/*
 * @Author: wls
 * @email: wuls@tuya.com
 * @LastEditors:   
 * @file name: light_driver_adapter.c
 * @Description: light driver adapter proc
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-04-26 13:55:40
 * @LastEditTime: 2019-05-28 21:33:21
 */

#include "light_driver_adapter.h"
#include "light_types.h"
#include "user_pwm.h"
#include "sm2135.h"
#include "sm16726b.h"


#define LIGHT_SEND_VALUE_MAX                    1000
///SM2135 Gray Max Value
#define SM1235_VALUE_MAX                        255
///SM2135 change value to the IC range
#define SM2135_VALUE_RANGE_CONVERT(value)       (value/LIGHT_SEND_VALUE_MAX*SM1235_VALUE_MAX)

///SM16726B Gray Max Value
#define SM16726B_VALUE_MAX                      255
///SM16726B change value to th IC range
#define SM16726B_VALUE_RANGE_CONVERT(value)     (value/LIGHT_SEND_VALUE_MAX*SM16726B_VALUE_MAX)


/* enum need to define in json analysize & get file */
enum{
    Drive_PWM = 0,
    Drive_SM2135,
    Drive_SM16726B
};

typedef enum{
    SEND_MODE_NONE = 0,
    SEND_MODE_RGB = 1,
    SEND_MODE_CW = 2,
    SEND_MODE_RGBCW = 3
}LIGHT_SEND_MODE;



STATIC 


/**
 * @berief: get send mode before sm2135 send data 
 * @param {type} none
 * @return: none
 * @retval: LIGHT_SEND_MODE
 * @retval: SEND_MODE_NONE -> no transmission
 * @retval: SEND_MODE_RGB -> need send RGB data
 * @retval: SEND_MODE_CW -> need send CW data
 * @retval: SEND_MODE_RGBCW -> need send RGB & CW data
 */
STATIC LIGHT_SEND_MODE Get_Send_Mode(USHORT_T R_value, USHORT_T G_Value, USHORT_T B_Value, USHORT_T C_Value, USHORT_T W_value)
{
    LIGHT_SEND_MODE Send_Mode = SEND_MODE_NONE;
    STATIC USHORT_T Last_Value_R = 0;
    STATIC USHORT_T Last_Value_G = 0;
    STATIC USHORT_T Last_Value_B = 0;
    STATIC USHORT_T Last_Value_C = 0;
    STATIC USHORT_T Last_Value_W = 0;

    if( (R_value != 0) || (G_value != 0) || (B_value != 0) ) {
        Send_Mode |= SEND_MODE_RGB;
    }

    if( (C_Value != 0) || (W_value != 0)) {
        Send_Mode |= SEND_MODE_CW;
    }

    return Send_Mode;
}

VOID light_drive_adapter_init(mode)
{
    
}

/**
 * @berief: light send control data
 * @param {type} none
 * @return: none
 * @retval: none
 */
VOID vLight_Drive_Send(USHORT_T R_value, USHORT_T G_Value, USHORT_T B_Value, USHORT_T C_Value, USHORT_T W_value)
{
    OPERATE_RET ret = -1;
    UCHAR_T Drive_Mode = 0;
    BOOL_T CCT_Drive_Flag = FALSE;

    //Driver_mode = ;   /* get drive mode */
    
    //CCT_Driver_Flag = ;   /* get cct drive mode */
    switch(Drive_mode)
    {
        case Drive_PWM:
            {
                USER_PWM_SendData_S Send_buf;

                memset(&Send_buf, 0, SIZEOF(USER_PWM_SendData_S));    
                Send_buf.R_Value = R_Value;
                Send_buf.G_Value = G_Value;
                Send_buf.B_Value = B_Value;
                Send_buf.C_Value = C_Value;
                Send_buf.W_Value = W_Value;
                
                ret = USER_PWM_SendOut(Send_buf);     /* pwm send out */
                if( ret != OPRT_OK ) {
                    PR_ERR("PWM send data error!");
                }
            }
            break;

        case Drive_SM2135:
            {
                SM2135_SendData_S SM2135_Send_buf;
                LIGHT_SEND_MODE Send_Mode = SEND_MODE_NONE;

                memset(&SM2135_Send_buf, 0, SIZEOF(SM2135_SendData_S));
                SM2135_Send_buf.R_Value = SM2135_VALUE_RANGE_CONVERT(R_Value);
                SM2135_Send_buf.G_Value = SM2135_VALUE_RANGE_CONVERT(G_Value);
                SM2135_Send_buf.B_Value = SM2135_VALUE_RANGE_CONVERT(B_Value);
                SM2135_Send_buf.C_Value = SM2135_VALUE_RANGE_CONVERT(C_Value);
                SM2135_Send_buf.W_Value = SM2135_VALUE_RANGE_CONVERT(W_Value);

                Send_Mode = Get_Send_Mode(R_Value, G_Value, B_Value, C_Value, W_Value);     /* get current IIC send mode! */
                if( Send_Mode & SEND_MODE_CW ) {
                    Send_buf.Mode = SM2135_White_Mode;
                    ret = SM2135_SendOut(SM2135_Send_buf);
                    if( ret != OPRT_OK ) {
                        PR_ERR("SM2135 send data error!");
                    }
                }

                if( Send_Mode & SEND_MODE_RGB ) {
                    Send_buf.Mode = SM2135_White_Mode;
                    ret = SM2135_SendOut(SM2135_Send_buf);
                    if( ret != OPRT_OK ) {
                        PR_ERR("SM2135 send data error!");
                    }
                }
            }
            break;

        case Drive_SM16726B:
            {
                SM16726B_SendData_S SM16726B_Send_buf;
                LIGHT_SEND_MODE Send_Mode = SEND_MODE_NONE;
                USER_PWM_SendData_S PWM_Send_buf;

                memset(&SM16726B_Send_buf, 0, SIZEOF(SM16726B_SendData_S));
                SM16726B_Send_buf.R_Value = SM16726B_VALUE_RANGE_CONVERT(R_Value);
                SM16726B_Send_buf.G_Value = SM16726B_VALUE_RANGE_CONVERT(G_Value);
                SM16726B_Send_buf.B_Value = SM16726B_VALUE_RANGE_CONVERT(B_Value);

                memset(&PWM_Send_buf, 0, SIZEOF(USER_PWM_SendData_S));
                PWM_Send_buf.R_Value = 0;
                PWM_Send_buf.G_Value = 0;
                PWM_Send_buf.B_Value = 0;
                PWM_Send_buf.C_Value = C_Value;
                PWM_Send_buf.W_Value = W_Value;
            
                Send_Mode = Get_Send_Mode(R_Value, G_Value, B_Value, C_Value, W_Value);     /* get current IIC send mode! */
                if( Send_Mode & SEND_MODE_CW ) {
                    ret = USER_PWM_SendOut(PWM_Send_buf);     /* pwm send out */
                    if( ret != OPRT_OK ) {
                        PR_ERR("PWM send data error!");
                    }
                }

                if( Send_Mode & SEND_MODE_RGB ) {
                    ret = SM16726B_SendOut(SM16726B_Send_buf);
                    if( ret != OPRT_OK ) {
                        PR_ERR("SM16726B send data error!");
                    }
                }
            }
            break;
        default:
            break;
    }   

}