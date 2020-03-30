/*
 * @Author: wls
 * @email: wuls@tuya.com
 * @LastEditors:   
 * @file name:  soc_i2c.c
 * @Description: Realtek 8710bn IIC gpio proc
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-04-16 16:46:43
 * @LastEditTime: 2019-05-28 20:04:21
 */
#include "soc_i2c.h"

i2c_pin_t g_i2c_gpio;

VOID vSocI2CInit(IN i2c_pin_t i2c_pin)
{
    BOOL_T ret = 0;
    
    memset(&g_i2c_gpio,0,sizeof(g_i2c_gpio));
    memcpy(&g_i2c_gpio, &i2c_pin,sizeof(g_i2c_gpio));
    if(i2c_pin.sda_pin_num > 0 && i2c_pin.sda_pin_num < MAX_GPIO_PIN_NUM)
    {
        vSocGpioOutPutInit(i2c_pin.sda_pin_num, 1);
    }

    if(i2c_pin.scl_pin_num > 0 && i2c_pin.scl_pin_num < MAX_GPIO_PIN_NUM)
    {
        vSocGpioOutPutInit(i2c_pin.scl_pin_num, 1);
    }

    if(i2c_pin.ctrl_pin_num > 0 && i2c_pin.ctrl_pin_num < MAX_GPIO_PIN_NUM)
    {
        vSocGpioOutPutInit(i2c_pin.ctrl_pin_num, 1);
    }
}

VOID I2C_SDA_RESET(VOID)
{
    vSocGpioOutPutWrite(g_i2c_gpio.sda_pin_num, 0);
}

VOID I2C_SDA_SET(VOID)
{
    vSocGpioOutPutWrite(g_i2c_gpio.sda_pin_num, 1);
}

VOID I2C_SCL_RESET(VOID)
{
    vSocGpioOutPutWrite(g_i2c_gpio.scl_pin_num, 0);
}

VOID I2C_SCL_SET(VOID)
{
    vSocGpioOutPutWrite(g_i2c_gpio.scl_pin_num, 1);
}

VOID I2C_CTRL_RESET(VOID)
{
    vSocGpioOutPutWrite(g_i2c_gpio.ctrl_pin_num, 0);
}

VOID I2C_CTRL_SET(VOID)
{
    vSocGpioOutPutWrite(g_i2c_gpio.ctrl_pin_num, 1);
}

UCHAR_T I2C_SDA_OUTPUT_READ(VOID)
{
    return vSocGpioOutPutRead(g_i2c_gpio.sda_pin_num);
}
UCHAR_T I2C_SDA_INPUT_READ(VOID)
{
    UCHAR_T ret;
    vSocGpioInPutInit(g_i2c_gpio.sda_pin_num,1);
    ret = vSocGpioInPutRead(g_i2c_gpio.sda_pin_num);
    vSocGpioOutPutInit(g_i2c_gpio.sda_pin_num,1);
    return ret;
}

















