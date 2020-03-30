/*
 * @Author: wls
 * @email: wuls@tuya.com
 * @LastEditors:
 * @file name: soc_flash.c
 * @Description: Realtek 8710bn flash proc
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-05-06 10:00:26
 * @LastEditTime: 2019-08-05 21:34:42
 */
#include "soc_flash.h"
#include "tuya_zigbee_sdk.h"

#define FLASH_DATA_START_ADDRESS   0
#define FLASH_DATA_SIZE_MAX        250
#define FLASH_DATA_MAX_ADDRESS     (FLASH_DATA_START_ADDRESS + FLASH_DATA_SIZE_MAX)

STATIC  UCHAR_T WRITE_DATA[FLASH_DATA_SIZE_MAX] = {0};
STATIC  UCHAR_T READ_DATA[FLASH_DATA_SIZE_MAX] = {0};


/**
 * @berief: soc data save
 * @param {IN SOC_FLASH_SAVE_TYPE_E DataType -> save type(meaning data kind)}
 * @param {IN UINT_T addr -> this type data address offset}
 * @param {IN UCHAR_T *data -> save data}
 * @param {IN USHORT_T len -> save data len}
 * @return: OPERATE_RET
 * @retval: none
 */
OPERATE_RET opSocFlashWrite(IN SOC_FLASH_SAVE_TYPE_E DataType, IN UINT_T addr, IN UCHAR_T *data, IN USHORT_T len)
{
    UCHAR_T ret = 0;
    USHORT_T i=0;

    //check space
    if((addr + len) >= FLASH_DATA_MAX_ADDRESS || addr < FLASH_DATA_START_ADDRESS) {
        app_print("write data size out range !");
        return FALSE;
    }
    //read out firt
    user_flash_data_read(WRITE_DATA, sizeof(WRITE_DATA));

    //set data valid flag
    WRITE_DATA[addr] = 0x5A;

    //fill write buffer from addr + 1
    for(i=0;i<len;i++){
        WRITE_DATA[addr+i+1] = *(data+i);
    }
    ret = user_flash_data_write(WRITE_DATA, sizeof(WRITE_DATA));
    if(ret != sizeof(WRITE_DATA)){
        app_print("write data error !");
        return OPRT_COM_ERROR;
    }
    return OPRT_OK;
}

/**
 * @berief: soc flash save data read
 * @param {IN SOC_FLASH_SAVE_TYPE_E DataType -> read data type(meaning data kind)}
 * @param {IN UINT_T addr -> this type data address offset}
 * @param {IN USHORT_T len -> read data len}
 * @param {OUT UCHAR_T *data -> read data}
 * @return: INT_T
 * @retval: read data cnt
 */
USHORT_T uiSocFlashRead(IN SOC_FLASH_SAVE_TYPE_E DataType, IN UINT_T addr, IN USHORT_T len, OUT UCHAR_T *data)
{
    USHORT_T ret=0;
    if((addr + len ) >= FLASH_DATA_MAX_ADDRESS||addr < FLASH_DATA_START_ADDRESS) {
        app_print("read data size out range !");
        return 0;
    }
    memset(READ_DATA,0,sizeof(READ_DATA));
    ret = user_flash_data_read(READ_DATA, sizeof(READ_DATA));
    if(!ret)
    {
        return 0;
    }
    if(READ_DATA[addr] == 0x5A)
    {
        memcpy(data,&READ_DATA[addr+1],len);
        return ret;
    } else {
        return 0;
    }
}



/**
 * @berief: soc flash oem cfg data read
 * @param {IN USHORT_T len -> write data len}
 * @param {IN UCHAR_T *data -> write data}
 * @return: OPERATE_RET
 * @retval: none
 */
OPERATE_RET opSocOemCfgWrite(IN USHORT_T len, IN UCHAR_T *data)
{
    // do nothing
    return OPRT_OK;
}
/**
 * @berief: soc flash oem cfg data read
 * @param {OUT USHORT_T *len -> read data len}
 * @param {OUT UCHAR_T *data -> read data}
 * @return: OPERATE_RET
 * @retval: none
 */
OPERATE_RET opSocOemCfgRead(INOUT USHORT_T *len, OUT UCHAR_T *data)
{
    UCHAR_T *data_pp = NULL;
    USHORT_T data_len;
    BOOL_T ret = oem_config_load(&data_len, &data_pp);
    if(!ret){
        return OPRT_COM_ERROR;
    }
	if(data_len>(*len)){
		return OPRT_COM_ERROR;
	}
     *len = data_len;
    memcpy(data,data_pp,data_len);

    return OPRT_OK;
}




OPERATE_RET opSocFlashErase(VOID)
{
   USHORT_T ret=0;
    memset(WRITE_DATA,0, sizeof(WRITE_DATA));
    ret = user_flash_data_write(WRITE_DATA, sizeof(WRITE_DATA));
    if(ret != sizeof(WRITE_DATA)){
        app_print("write data error !");
        return OPRT_COM_ERROR;
    }
    return OPRT_OK;
}





