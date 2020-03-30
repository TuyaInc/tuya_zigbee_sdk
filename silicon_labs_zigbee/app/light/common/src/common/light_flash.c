/*
 * @Author: wls
 * @email: wuls@tuya.com
 * @LastEditors: wls
 * @file name: light_flash.c
 * @Description: light production flash data read/write proc
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-05-06 10:57:08
 * @LastEditTime: 2019-05-06 16:50:33
 */

#include "light_types.h"
#include "light_flash.h"


/**
 * @berief: read light flash data
 * @param {UCHAR_T ReadData_Type -> read data type
 *         CHAR_T *cData -> read data} 
 * @attention all data is stroaged by value directly
 * @return: none
 * @retval: none
 */
VOID vUserFlash_Read(UCHAR_T ReadData_Type, SHORT_T *sData)
{
    switch (ReadData_Type)
    {
        case DTYPE_OEM:
            break;

        case DTYPE_APP:
            break;

        case DTYPE_RESTCNT:

            break;

        case DTYPED_PORD:   /* read production factory data */
            if( ( NULL == cData ) || ( strlen(sData) < 2 ) ){
                PR_ERR("Read Production data poniter space error");
                break;
            }

            vSOCFlash_Read(ReadData_Type, cData);
            /* code */
            break;
    
        default:
            break;
    }

}


OPERATE_RET opUserFlash_Write(UCHAR_T ReadData_Type, SHORT_T *sData)
{

    switch (ReadData_Type)
    {
        case DTYPE_OEM:
            break;

        case DTYPE_APP:
            break;

        case DTYPE_RESTCNT:

            break;

        case DTYPED_PORD:   /* read production factory data */
            if( ( NULL == cData ) || ( strlen(sData) < 2 ) ){
                PR_ERR("Write Production data poniter space error");
                break;
            }

            vSocFlash_Read(ReadData_Type, cData);
            /* code */
            break;
    
        default:
            break;
    }
}