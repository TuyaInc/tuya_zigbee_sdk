/*
 * @Author: wls
 * @email: wuls@tuya.com
 * @LastEditors:   
 * @file name: sm16726b.h
 * @Description: SM16726B IIC driver include file
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-04-17 20:49:35
 * @LastEditTime: 2019-05-28 21:41:22
 */

#ifndef __SM16726B_H__
#define __SM16726B_H__

#include "light_types.h"


#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */


typedef enum
{
    COLOR_MODE_WHITE = 0,
    COLOR_MODE_RGB  = 1,
};
/**
 * @brief sm16726b init gpio pin structure
 * sm16726b init gpio structure
 */
typedef struct 
{
    UCHAR_T uSDA_IO;                 ///< SDA pin
    UCHAR_T uSCL_IO;                 ///< SCL pin
    UCHAR_T uCTRL_IO;                ///< CTRL pin parm
    BOOL_T  bCTRL_LV;                ///< Enable level
    UCHAR_T ucList[3];              ///< channel list
}SM16726B_GPIO_S;

typedef struct
{
    USHORT_T usR_Value;       ///< R value,rang from 0 to 256
    USHORT_T usG_Value;       ///< G value,rang from 0 to 256
    USHORT_T usB_Value;       ///< B value,rang from 0 to 256
    USHORT_T usC_Value;       ///< B value,rang from 0 to 256
    USHORT_T usW_Value;       ///< B value,rang from 0 to 256
    UCHAR_T  ucMode;          ///< output mode
}SM16726B_ColoerData_S;

/**
 * @berief: sm16726B init
 * @param {IN SM16726B_GPIO_S Init_GPIO -> INIT Parm
 *          CHAR_T uSDA_IO -> SDA IO
 *          CHAR_T uSCL_IO -> SCL IO
 *          SM16726B_CTRL_S CTRL_PARM -> CTRL IO Parm
 *              CHAR_T cCTRL_IO -> CTRL IO
 *              BOOL_T bLEVEL -> HIGH/LOW LEVEL
 *          UCHAR_T *uclist -> RGB output channel config list}
 * @attention bLEVEL setting
 *              TRUE    --> HIGH LEVEL effective
 *              FALSE   --> LOW LEVEL effective
 * @attention *ucList parm set
 *              RGB defalut output channel is R->0 G->1 B->2
 * @return: none
 * @retval: none
 */
OPERATE_RET opSM16726B_Init(IN SM16726B_GPIO_S Init_GPIO);

/**
 * @berief: sm16726b send data
 * @param {IN SM16726B_SendData -> need write data structure to sm16726B
 *          SendData.usR_Value -> R value,rang from 0 to 256
 *          SendData.usG_Value -> G value,rang from 0 to 256
 *          SendData.usB_Value -> B value,rang from 0 to 256}
 * @return: OPERATE_RET
 * @retval: OPRT_INVALID_PARM -> invalid parm
 * @retval: OPRT_OK ->operate ok
 */
OPERATE_RET opSM16726B_SetColor(IN SM16726B_ColoerData_S SendData);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif  /* __SM16726B_H__ */
