/*
 * @Author: jinlu
 * @email: jinlu@tuya.com
 * @LastEditors:
 * @file name: light_flash.c
 * @Description: light production flash data read/write proc
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-05-06 10:57:08
 * @LastEditTime: 2019-08-09 14:30:49
 */

#include "soc_flash.h"
#include "user_flash.h"
#include "light_types.h"
#include "tuya_zigbee_sdk.h"

/**
 * @berief: save light reset cnt
 * @param {IN UCHAR_T data -> save reset cnt to flash}
 * @return: OPERATE_RET
 * @retval: none
 */
OPERATE_RET opUserFlashWriteResetCnt(IN UCHAR_T data)
{
    OPERATE_RET opRet = -1;

    opRet = opSocFlashWrite(SAVE_TYP1, RESET_CNT_OFFSET, &data, 1);
    if(opRet != OPRT_OK) {
        app_print("Reset cnt write error %d!", opRet);
        return OPRT_COM_ERROR;
    }
    return OPRT_OK;
}

/**
 * @berief: save light application data
 * @param {IN light_app_data_flash_t *data -> save data}
 * @return: OPERATE_RET
 * @retval: none
 */
OPERATE_RET opUserFlashWriteAppData(IN light_app_data_flash_t *data)
{
    OPERATE_RET opRet;

    opRet = opSocFlashWrite(SAVE_TYP2, LIGHT_APP_DATA_OFFSET, (UCHAR_T *)data, SIZEOF(light_app_data_flash_t));
    if(opRet != OPRT_OK) {
        app_print("Application data write error %d!", opRet);
        return OPRT_COM_ERROR;
    }
    return OPRT_OK;
}

/**
 * @berief: save light product test data
 * @param {IN light_prod_test_data_flash_t *data -> prod test data}
 * @return: OPERATE_RET
 * @retval: none
 */
OPERATE_RET opUserFlashWriteProdData(IN light_prod_test_data_flash_t *data)
{
    OPERATE_RET opRet = -1;

    opRet = opSocFlashWrite(SAVE_TYP3, PROD_TEST_DATA_OFFSET, (UCHAR_T *)data, SIZEOF(light_prod_test_data_flash_t));
    if(opRet != OPRT_OK) {
        app_print("Production test data write error %d!", opRet);
        return OPRT_COM_ERROR;
    }
    return OPRT_OK;
}

/**
 * @berief: save light application data
 * @param {IN light_app_data_flash_t *data -> save data}
 * @return: OPERATE_RET
 * @retval: none
 */
OPERATE_RET opUserFlashWriteOemCfgData(IN USHORT_T len, IN UCHAR_T *data)
{
    OPERATE_RET opRet = -1;

    opRet = opSocOemCfgWrite(len, data);
    if(opRet != OPRT_OK) {
        app_print("oem cfg move to uf file error!");
        return OPRT_COM_ERROR;
    }

    return OPRT_OK;
}

/**
 * @berief: read light reset cnt
 * @param {OUT UCHAR_T *data -> reset cnt}
 * @return: OPERATE_RET
 * @retval: none
 */
OPERATE_RET opUserFlashReadResetCnt(OUT UCHAR_T *data)
{
    INT_T uiReadCnt = 0;

    uiReadCnt = uiSocFlashRead(SAVE_TYP1, RESET_CNT_OFFSET, 1, (UCHAR_T *)data);
    if(uiReadCnt <= 0) {
        app_print("user flash reset cnt read error!");
        return OPRT_COM_ERROR;
    }

    return OPRT_OK;
}

/**
 * @berief: read light application data
 * @param {IN light_prod_test_data_flash_t *data -> prod test data}
 * @return: OPERATE_RET
 * @retval: none
 */
OPERATE_RET opUserFlashReadAppData(OUT light_app_data_flash_t *data)
{
    INT_T uiReadCnt = 0;

    uiReadCnt = uiSocFlashRead(SAVE_TYP2, LIGHT_APP_DATA_OFFSET, SIZEOF(light_app_data_flash_t), (UCHAR_T *)data);
    if(uiReadCnt <= 0) {
        app_print("Application data read error!");
        return OPRT_COM_ERROR;
    }

    return OPRT_OK;
}

/**
 * @berief: read light prod test data
 * @param {OUT light_prod_test_data_flash_t *data -> prod test data}
 * @return: OPERATE_RET
 * @retval: none
 */
OPERATE_RET opUserFlashReadProdData(OUT light_prod_test_data_flash_t *data)
{
    INT_T uiReadCnt = 0;

    uiReadCnt = uiSocFlashRead(SAVE_TYP3, PROD_TEST_DATA_OFFSET, SIZEOF(light_prod_test_data_flash_t), (UCHAR_T *)data);
    if(uiReadCnt <= 0) {
        app_print("Production data read error!");
        return OPRT_COM_ERROR;
    }

    return OPRT_OK;
}

/**
 * @berief: read oem cfg data
 * @param {OUT USHORT_T *len -> read data len}
 * @param {OUT UCHAR_T *data -> read data}
 * @return: OPERATE_RET
 * @retval: none
 */
OPERATE_RET opUserFlashReadOemCfgData(OUT USHORT_T *len, OUT UCHAR_T *data)
{
    OPERATE_RET opRet = -1;

    opRet = opSocOemCfgRead(len, data);
    if(opRet != OPRT_OK) {
        app_print("oem cfg data read error!");
        return OPRT_COM_ERROR;
    }

    return OPRT_OK;
}

/**
 * @berief: erase all user flash data
 * @param {none}
 * @return: OPERATE_RET
 * @retval: none
 */
OPERATE_RET opUserFlashDataErase(VOID)
{
    OPERATE_RET opRet = opSocFlashErase();

    if(opRet!= OPRT_OK){
        return OPRT_COM_ERROR;
    }
    return OPRT_OK;
}


