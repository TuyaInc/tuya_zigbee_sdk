/*
 * @Author: wls
 * @email: wuls@tuya.com
 * @LastEditors:   
 * @file name:  soc_i2c.c
 * @Description: Realtek 8710bn IIC gpio proc
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-04-16 16:46:43
 * @LastEditTime: 2019-05-28 19:57:10
 */
#ifndef __SOC_I2C_H__
#define __SOC_I2C_H__

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include "light_types.h"
#include "soc_gpio.h"
#include "zigbee_sdk.h"

typedef struct 
{
    UCHAR_T sda_pin_num;
    UCHAR_T scl_pin_num;
    UCHAR_T ctrl_pin_num;
}i2c_pin_t;


VOID vSocI2CInit(IN i2c_pin_t i2c_pin);
VOID I2C_SDA_RESET(VOID);
VOID I2C_SDA_SET(VOID);
VOID I2C_SCL_RESET(VOID);
VOID I2C_SCL_SET(VOID);
VOID I2C_CTRL_RESET(VOID);
VOID I2C_CTRL_SET(VOID);
UCHAR_T I2C_SDA_OUTPUT_READ(VOID);
UCHAR_T I2C_SDA_INPUT_READ(VOID);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif  /* __SOC_I2C_H__ */
