/**
 * @author Leon
 * @par email:
 *      zhangpeng@tuya.com
 * @file hal_pwm.h
 * @brief This file abstracts all the operations of PWM
 * @copyright HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @par company
 *      http://www.tuya.com
 * @date 2020-03-20 17:29:33
 * @par Usage
 * @code
 *   user should call hal_pwm_init() first to initialize the pwm module.
 *   if you want to set duty_cycle with specific resolution, follow steps bellow:
 *   1.uint16_t max_value = get_max_pwm_value();
 *   2.uint16_t value = x * max_value/256;
 *   3.set_pwm_value(value);
 
 *   thus, resolution = 256, and duty_cycle is x/256
 *   if you want to set duty_cycle with resolution 1%, you just neet to call set_pwm_duty().
 * @endcode
 */

#ifndef __HAL_PWM_H__
#define __HAL_PWM_H__

#include "type_def.h"
#include "hal_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif



typedef struct{
    GPIO_PORT_T port;
    GPIO_PIN_T pin;
    GPIO_LEVEL_T lv;
    bool_t invert;
}pwm_gpio_t;

/**
* @note pwm init 
* @param[in] {config} pwm pins config
* @param[in] {sum} pwm channel want to be init
* @param[in] {pwm_freq} pwm frequency
* @return 1:success, 0: failed
*/
uint8_t hal_pwm_init(pwm_gpio_t *config, uint8_t sum, uint16_t pwm_freq);

/**
 * @note set pwm value, duty_cycle = value/max_value
 * @param[in] {index} choose pwn channel
 * @param[in] {value} value to be set
 * @return 1:success, 0: failed
 */
uint8_t set_pwm_value(uint8_t index, uint16_t value);

/**
 * @note get max pwm value 
 * @param[in] {none} 
 * @return max pwm value 
 */
uint16_t get_max_pwm_value(void);

/**
 * @note get max pwm value 
 * @param[in] {none} 
 * @return max pwm value 
 */
uint8_t set_pwm_duty(uint8_t index, uint16_t duty, uint16_t precision);


#ifdef __cplusplus
}
#endif

#endif






