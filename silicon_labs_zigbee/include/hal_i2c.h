/**
 * @author Leon
 * @par email:
 *      zhangpeng@tuya.com
 * @file hal_i2c.h
 * @brief This file abstracts all the operations of I2C
 * @copyright HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @par company
 *      http://www.tuya.com
 * @date 2020-03-20 17:29:33
 */

#ifndef __HAL_I2C_H__
#define __HAL_I2C_H__

#include "type_def.h"
#include "hal_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
//                                                                              
//                              soft i2c                               
//                                                                              
//******************************************************************************
typedef struct{
    GPIO_PORT_PIN_T scl; //scl pin config
    GPIO_PORT_PIN_T sda; //sda pin config
    GPIO_PORT_PIN_T power; //iic slave device power control pin config
    bool_t power_pin_enable; //enable/disable iic slave device power control
}i2c_gpio_t;

bool_t hal_i2c_init(i2c_gpio_t *config, uint8_t iic_id);
bool_t hal_i2c_start(uint8_t iic_id);
bool_t hal_i2c_stop(uint8_t iic_id);
bool_t hal_i2c_check_ack(uint8_t iic_id);
void hal_i2c_send_byte(uint8_t iic_id, uint8_t data);
void hal_i2c_power_enable(uint8_t iic_id);
void hal_i2c_power_disable(uint8_t iic_id);

void hal_i2c_set_sda_high(uint8_t iic_id);
void hal_i2c_set_sda_low(uint8_t iic_id);
uint8_t hal_i2c_get_sda_value(uint8_t iic_id);
void hal_i2c_set_scl_high(uint8_t iic_id);
void hal_i2c_set_scl_low(uint8_t iic_id);
uint8_t hal_i2c_get_scl_value(uint8_t iic_id);
void hal_i2c_set_power_high(uint8_t iic_id);
void hal_i2c_set_power_low(uint8_t iic_id);
uint8_t hal_i2c_get_power_value(uint8_t iic_id);





//******************************************************************************
//                                                                              
//                               hardware i2c                                   
//                                                                              
//******************************************************************************
typedef enum {
    I2C0_M = 0, ///< I2C0_M
}HARDWARE_I2C_TYPE_M;

typedef enum {
    I2C0_FREQ_10_KHz_M = 0, ///< I2C0_FREQ_10_KHz_M
    I2C0_FREQ_40_KHz_M,     ///< I2C0_FREQ_40_KHz_M
    I2C0_FREQ_100_KHz_M,
}HARDWARE_I2C_FREQ_M;

typedef struct{
    HARDWARE_I2C_TYPE_M i2c; ///< scl type config

    GPIO_PORT_T scl_port; ///< scl pin config
    GPIO_PIN_T scl_pin;
    uint8_t scl_location;

    GPIO_PORT_T sda_port; ///< scl pin config
    GPIO_PIN_T sda_pin;
    uint8_t sda_location;

    HARDWARE_I2C_FREQ_M i2c_freq; ///< i2c freq
}hardware_i2c_config_t;

/*********************************************/
// APIs for custom hardware iic protocol
void hal_hardware_i2c_init(hardware_i2c_config_t *set_i2cConfig);
int8_t hal_hardware_i2c_write_bytes(uint8_t address, uint8_t *buffer, uint8_t count);
int8_t hal_hardware_i2c_write_bytes_delay(uint8_t address, const uint8_t *buffer, uint8_t count, uint8_t delay);
int8_t hal_hardware_i2c_read_bytes(uint8_t address, uint8_t *buffer, uint8_t count);


#ifdef __cplusplus
}
#endif

#endif





