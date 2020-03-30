/*
 * @Author: wls
 * @email: wuls@tuya.com
 * @LastEditors:
 * @file name: soc_flash.h
 * @Description: Realtek 8710bn flash include file
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-05-06 10:00:26
 * @LastEditTime: 2019-06-10 16:49:13
 */

#ifndef __SOC_FLASH_H__
#define __SOC_FLASH_H__


#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include "light_types.h"

/**
 * @brief wifi(realtek 8710bn) soc flash save type enum
 */
typedef enum {
    SAVE_TYP1 = 0,
    SAVE_TYP2,
    SAVE_TYP3,
    SAVE_TYP4,
    SAVE_TYP_MAX,
}SOC_FLASH_SAVE_TYPE_E;
/**
 * @berief: soc data save
 * @param {IN SOC_FLASH_SAVE_TYPE_E DataType -> save type(meaning data kind)}
 * @param {IN UINT_T addr -> this type data address offset}
 * @param {IN UCHAR_T *data -> save data}
 * @param {IN USHORT_T len -> save data len}
 * @return: OPERATE_RET
 * @retval: none
 */
OPERATE_RET opSocFlashWrite(IN SOC_FLASH_SAVE_TYPE_E DataType, IN UINT_T addr, IN UCHAR_T *data, IN USHORT_T len);
/**
 * @berief: soc flash save data read
 * @param {IN SOC_FLASH_SAVE_TYPE_E DataType -> read data type(meaning data kind)}
 * @param {IN UINT_T addr -> this type data address offset}
 * @param {IN USHORT_T len -> read data len}
 * @param {OUT UCHAR_T *data -> read data}
 * @return: INT_T
 * @retval: read data cnt
 */
USHORT_T uiSocFlashRead(IN SOC_FLASH_SAVE_TYPE_E DataType, IN UINT_T addr, IN USHORT_T len, OUT UCHAR_T *data);
/**
 * @berief: soc data save
 * @param {IN SOC_FLASH_SAVE_TYPE_E DataType -> save type(meaning data kind)}
 * @param {IN UINT_T addr -> this type data address offset}
 * @param {IN UCHAR_T *data -> save data}
 * @param {IN USHORT_T len -> save data len}
 * @return: OPERATE_RET
 * @retval: none
 */
OPERATE_RET opSocFlashWrite(IN SOC_FLASH_SAVE_TYPE_E DataType, IN UINT_T addr, IN UCHAR_T *data, IN USHORT_T len);

/**
 * @berief: soc flash save data read
 * @param {IN SOC_FLASH_SAVE_TYPE_E DataType -> read data type(meaning data kind)}
 * @param {IN UINT_T addr -> this type data address offset}
 * @param {IN USHORT_T len -> read data len}
 * @param {OUT UCHAR_T *data -> read data}
 * @return: USHORT_T
 * @retval: read data cnt
 */
USHORT_T usSocFlashRead(IN SOC_FLASH_SAVE_TYPE_E DataType, IN UINT_T addr, IN USHORT_T len, OUT UCHAR_T *data);

/**
 * @berief: soc flash oem cfg data read
 * @param {IN USHORT_T len -> write data len}
 * @param {IN UCHAR_T *data -> write data}
 * @return: OPERATE_RET
 * @retval: none
 */
OPERATE_RET opSocOemCfgWrite(IN USHORT_T len, IN UCHAR_T *data);

/**
 * @berief: soc flash oem cfg data read
 * @param {OUT USHORT_T *len -> read data len}
 * @param {OUT UCHAR_T *data -> read data}
 * @return: OPERATE_RET
 * @retval: none
 */
OPERATE_RET opSocOemCfgRead(OUT USHORT_T *len, OUT UCHAR_T *data);

/**
 * @berief: soc flash erase all
 * @param {none}
 * @return: OPERATE_RET
 * @retval: none
 */
OPERATE_RET opSocFlashErase(VOID);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif  /* __SOC_FLASH_H__ */
