/*
 * @Author: wls
 * @email: wuls@tuya.com
 * @LastEditors:   
 * @file name: sm2135.h
 * @Description: sm2135 IIC drive include file
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-04-16 15:11:22
 * @LastEditTime: 2019-05-28 21:27:28
 */

#ifndef __SM2135_H__
#define __SM2135_H__


#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */
#include "light_types.h"

/**
 * @brief sm2135 work mode enum
 * sm2135 work mode 
 */
enum{
    SM2135_White_Mode = 0,
    SM2135_RGB_Mode ,
};


/**
 * @brief sm2135 init gpio pin structure
 * sm2135 init gpio structure
 */
typedef struct 
{
    UCHAR_T cSDA_IO;            ///< SDA pin
    UCHAR_T cSCL_IO;            ///< SCL pin
    UCHAR_T ucList[5];          ///< RGBCW output channel index config 
    UCHAR_T ucRGB_Current;      ///< RGB mode max current settings
    UCHAR_T ucCW_Current;       ///< CW mode max current settings
}SM2135_GPIO_S;

/**
 * @brief sm2135 send data structure
 * sm2135 send data structure
 */
typedef struct 
{
    USHORT_T usR_Value;       ///< R value,rang from 0 to 256
    USHORT_T usG_Value;       ///< G value,rang from 0 to 256
    USHORT_T usB_Value;       ///< B value,rang from 0 to 256
    USHORT_T usC_Value;       ///< C value,rang from 0 to 256
    USHORT_T usW_Value;       ///< W value,rang from 0 to 256
    UCHAR_T  ucMode;           ///< output mode
}SM2135_ColoerData_S;

/**
 * @berief: sm3125 init
 * @param {IN SM3125_GPIO_S Init_GPIO -> INIT Parm
 *          CHAR_T cSDA_IO -> SDA IO
 *          CHAR_T cSCL_IO -> SCL IO
 *          UCHAR_T uclist[5] -> RGBCW output channel config list
 *          UCHAR_T ucRGB_Current -> RGB mode max current settings
 *          UCHAR_T ucCW_Current -> CW mode max current settings}
 * @attention ucList[5] parm set
 *              RGB configurable output channel is 0~2
 *              CW configurable output channel is 3~4
* @attention ucRGB_Current parm set
 *              RGB_Current parm configurable max current <= 45mA
 * @attention ucCW_Current parm set
 *               CW_Current parm configurable max current <= 60mA
 * @return: OPERATE_RET
 * @retval: none
 */
OPERATE_RET opSM2135_Init(IN SM2135_GPIO_S Init_GPIO);

/**
 * @berief: sm2135 send data
 * @param {IN SM2135_SendData -> need write data structure to sm2135
 *          usR_Value -> R value,rang from 0 to 256
 *          usG_Value -> G value,rang from 0 to 256
 *          usB_Value -> B value,rang from 0 to 256
 *          usC_Value -> C value,rang from 0 to 256
 *          usW_Value -> W value,rang from 0 to 256
 *          ucMode -> output mode}
 * @attention sm2135 send mode must be SM2135_White_Mode or SM2135_RGB_Mode 
 *              at same moment 
 * @return: OPERATE_RET
 * @retval: OPRT_INVALID_PARM -> invalid parm
 * @retval: OPRT_COM_ERROR  -> send data error
 */
OPERATE_RET opSM2135_SendOut(IN SM2135_ColoerData_S SendData);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif  /* __SM2135_H__ */
