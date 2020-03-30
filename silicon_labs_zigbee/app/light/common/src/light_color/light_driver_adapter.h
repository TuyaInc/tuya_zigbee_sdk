/*
 * @Author: wls
 * @email: wuls@tuya.com
 * @LastEditors: wls
 * @file name: light_driver_adapter.h
 * @Description: light driver adapter include file
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-04-26 13:55:40
 * @LastEditTime: 2019-05-15 16:19:32
 */

#ifndef __LIHGT_DRIVER_ADAPTER_H__
#define __LIHGT_DRIVER_ADAPTER_H__


#include "sm2135.h"




#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

typedef struct 
{
    USHORT_T R_Aims;        ///< RGBCW Target value
    USHORT_T G_Aims;
    USHORT_T B_Aims;
    USHORT_T C_Aims;
    USHORT_T W_Aims;

    USHORT_T R_Cur;         ///< RGBCW Current value
    USHORT_T G_Cur;
    USHORT_T B_Cur;
    USHORT_T C_Cur;
    USHORT_T W_Cur;
    
}LIGHT_DRIVE_SENDDATA_S;


#define LIGHT_MAX_CTL_VAL 1000

/**
 * @berief: light send control data
 * @param {type} none
 * @return: none
 * @retval: none
 */
VOID vLight_Drive_Send(USHORT_T R_value, USHORT_T G_Value, USHORT_T B_Value, USHORT_T C_Value, USHORT_T W_value);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif  /* __USER_PWM_H__ */
