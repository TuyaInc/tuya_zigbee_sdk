/*
 * @Author: jinlu
 * @email: jinlu@tuya.com
 * @LastEditors:
 * @file name: user_flash.h
 * @Description: light production flash read/write include file
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-04-26 13:55:40
 * @LastEditTime: 2019-06-26 16:08:44
 */

#ifndef __USER_FLASH_H__
#define __USER_FLASH_H__

#include "light_types.h"
#include "soc_flash.h"


#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */



#define LIGHT_SCENE_MAX_LENGTH    210

#pragma pack(1)

typedef enum
{
    LIGHT_MODE_WHITE = 0,
    LIGHT_MODE_COLOR,
    LIGHT_MODE_SCENE,
    LIGHT_MODE_MUSIC,
    LIGHT_MODE_MAX,
}LIGHT_MODE_T;

typedef struct
{
    USHORT_T usRed;         /* color R setting */
    USHORT_T usGreen;
    USHORT_T usBlue;
}color_rgb_t;

typedef struct
{
    USHORT_T usColorData1;
    USHORT_T usColorData2;
    USHORT_T usColorData3;
    UCHAR_T ucColorStr[13];
}color_origin_t;


/**
 * @brief Light save data structure
 */
typedef struct
{
    BOOL_T          bPower;
    LIGHT_MODE_T    Mode;
    USHORT_T        usBright;
    USHORT_T        usTemper;
    color_rgb_t     Color;
    color_origin_t  ColorOrigin;
    UCHAR_T         ucScene[LIGHT_SCENE_MAX_LENGTH + 1];
}light_app_data_flash_t;

/**
 * @brief Light save data structure
 */
typedef enum {
    FUC_TEST1 = 0,
    AGING_TEST,
    FUC_TEST2,
}TEST_MODE_T;

typedef struct {
    TEST_MODE_T  test_mode;
    USHORT_T     aging_tested_time;
}light_prod_test_data_flash_t;

/**
 * @berief: save light reset cnt
 * @param {IN UCHAR_T data -> save reset cnt to flash}
 * @return: OPERATE_RET
 * @retval: none
 */
OPERATE_RET opUserFlashWriteResetCnt(IN UCHAR_T data);

/**
 * @brief Light oem cfg data structure
 */
//@attention!!!

#pragma pack()

#define RESET_CNT_OFFSET          0
#define LIGHT_APP_DATA_OFFSET     (RESET_CNT_OFFSET + 2)
#define PROD_TEST_DATA_OFFSET     (LIGHT_APP_DATA_OFFSET + sizeof(light_app_data_flash_t) + 1)

/**
 * @berief: read light reset cnt
 * @param {OUT UCHAR_T *data -> reset cnt}
 * @return: OPERATE_RET
 * @retval: none
 */
OPERATE_RET opUserFlashReadResetCnt(OUT UCHAR_T *data);

/**
 * @berief: save light application data
 * @param {IN light_app_data_flash_t *data -> save data}
 * @return: OPERATE_RET
 * @retval: none
 */
OPERATE_RET opUserFlashWriteAppData(IN light_app_data_flash_t *data);

/**
 * @berief: save light product test data
 * @param {IN light_prod_test_data_flash_t *data -> prod test data}
 * @return: OPERATE_RET
 * @retval: none
 */
OPERATE_RET opUserFlashWriteProdData(IN light_prod_test_data_flash_t *data);

/**
 * @berief: save light application data
 * @param {IN light_app_data_flash_t *data -> save data}
 * @return: OPERATE_RET
 * @retval: none
 */
OPERATE_RET opUserFlashWriteOemCfgData(IN USHORT_T len, IN UCHAR_T *data);

/**
 * @berief: read light reset cnt
 * @param {OUT UCHAR_T *data -> reset cnt}
 * @return: OPERATE_RET
 * @retval: none
 */
OPERATE_RET opUserFlashReadRestCnt(OUT UCHAR_T *data);

/**
 * @berief: read light application data
 * @param {IN light_prod_test_data_flash_t *data -> prod test data}
 * @return: OPERATE_RET
 * @retval: none
 */
OPERATE_RET opUserFlashReadAppData(OUT light_app_data_flash_t *data);

/**
 * @berief: read light prod test data
 * @param {OUT light_prod_test_data_flash_t *data -> prod test data}
 * @return: OPERATE_RET
 * @retval: none
 */
OPERATE_RET opUserFlashReadProdData(OUT light_prod_test_data_flash_t *data);

/**
 * @berief: read oem cfg data
 * @param {OUT USHORT_T *len -> read data len}
 * @param {OUT UCHAR_T *data -> read data}
 * @return: OPERATE_RET
 * @retval: none
 */
OPERATE_RET opUserFlashReadOemCfgData(OUT USHORT_T *len, OUT UCHAR_T *data);

/**
 * @berief: erase all user flash data
 * @param {none}
 * @return: OPERATE_RET
 * @retval: none
 */
OPERATE_RET opUserFlashDataErase(VOID);



#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif  /* __USER_FLASH_H__ */
