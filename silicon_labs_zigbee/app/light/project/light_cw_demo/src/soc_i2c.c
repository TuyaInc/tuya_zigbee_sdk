/*
 * @Author: wls
 * @email: wuls@tuya.com
 * @LastEditors:   
 * @file name:  soc_i2c.c
 * @Description: Realtek 8710bn IIC gpio proc
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-04-16 16:46:43
 * @LastEditTime: 2019-08-23 12:57:19
 */
#include "soc_i2c.h"
#include "soc_gpio.h"
#include "tuya_zigbee_sdk.h"
#include "light_printf.h"

i2c_pin_t g_i2c_gpio;


OPERATE_RET opSocI2CInit(IN i2c_pin_t *i2c_pin)
{
    memset(&g_i2c_gpio,0,sizeof(i2c_pin_t));
    memcpy(&g_i2c_gpio, i2c_pin,sizeof(i2c_pin_t));
    
    if(OPRT_OK != opSocGpioOutPutInit(i2c_pin->ucSDA_IO, 0)){
        app_print("I2C init ERR");
        return OPRT_INVALID_PARM;
    }
    
    if(OPRT_OK != opSocGpioOutPutInit(i2c_pin->ucSCL_IO, 0)){
        app_print("I2C init ERR");
        return OPRT_INVALID_PARM;
    }
    if(i2c_pin->ucCTRL_IO != PIN_NOEXIST)
    {
        if(OPRT_OK != opSocGpioOutPutInit(i2c_pin->ucCTRL_IO, 0)){
            app_print("I2C init ERR");
            return OPRT_INVALID_PARM;
        }
    }

    return OPRT_OK;
}

/**
 * @berief: SOC i2c SDA reset proc
 * @param {none}
 * @return: none
 * @retval: none
 */
VOID vI2CSDAReset(VOID)
{
    vSocGpioOutPutWrite(g_i2c_gpio.ucSDA_IO, 0);
}

/**
 * @berief: SOC i2c SDA set proc
 * @param {none}
 * @return: none
 * @retval: none
 */
VOID vI2CSDASet(VOID)
{
    vSocGpioOutPutWrite(g_i2c_gpio.ucSDA_IO, 1);
}

/**
 * @berief: SOC i2c SCL set proc
 * @param {none}
 * @return: OPERATE_RET
 * @retval: none
 */
VOID vI2CSCLReset(VOID)
{  
    vSocGpioOutPutWrite(g_i2c_gpio.ucSCL_IO, 0);
}

/**
 * @berief: SOC i2c SCL set proc
 * @param {none}
 * @return: none
 * @retval: none
 */
VOID vI2CSCLSet(VOID)
{  
    vSocGpioOutPutWrite(g_i2c_gpio.ucSCL_IO, 1);
}

/**
 * @berief: SOC i2c ctrl Reset proc
 * @param {none}
 * @return: none
 * @retval: none
 */
VOID vI2CCtrlReset(VOID)
{ 
    vSocGpioOutPutWrite(g_i2c_gpio.ucCTRL_IO, 0);
}

/**
 * @berief: SOC i2c ctrl set proc
 * @param {none}
 * @return: none
 * @retval: none
 */
VOID vI2CCtrlSet(VOID)
{
    vSocGpioOutPutWrite(g_i2c_gpio.ucCTRL_IO, 1);
}

/**
 * @berief: SOC i2c SDA(output) read
 * @param {none}
 * @return: OPERATE_RET
 * @retval: none
 */
UCHAR_T ucI2CSDAOutputRead(VOID)
{   
    return ucSocGpioOutPutRead(g_i2c_gpio.ucSDA_IO);
}
/**
 * @berief: SOC i2c SDA(input) read
 * @param {none}
 * @return: OPERATE_RET
 * @retval: none
 */
UCHAR_T ucI2CSDAInputRead(VOID)
{
    /*
    UCHAR_T data;
    //opSocGpioInPutInit(g_i2c_gpio.ucSDA_IO,1);
    opSocGpioInPutInitNoPull(g_i2c_gpio.ucSDA_IO);
    data = ucSocGpioInPutRead(g_i2c_gpio.ucSDA_IO);
    opSocGpioOutPutInit(g_i2c_gpio.ucSDA_IO,0);
    return data;
    */
   return FALSE;
}

















