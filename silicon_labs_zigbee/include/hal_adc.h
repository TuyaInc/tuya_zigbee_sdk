/**
 * @author Leon
 * @par email:
 *      zhangpeng@tuya.com
 * @file hal_adc.h
 * @brief This file abstracts all the operations of ADC
 * @copyright HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @par company
 *      http://www.tuya.com
 * @date 2020-03-20 17:29:33
 */

#ifndef __HAL_ADC_H__
#define __HAL_ADC_H__

#include "type_def.h"
#include "hal_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @enum ADC IO select
 */
typedef enum {
    ADC_TYPE_VDD = 0,   ///< capture vdd 
    ADC_TYPE_PORT_PIN,  ///< capture GPIO
}ADC_TYPE_T;

typedef struct {
    ADC_TYPE_T adc_type;      ///< ADC IO select
    GPIO_PORT_PIN_T *pos_pin; ///< Positive voltage IO configuration
    GPIO_PORT_PIN_T *neg_pin; ///< Negative voltage IO configuration
}adc_cfg_t;



/**
 * @note config ADC pin
 * @param[in] {ADC_TYPE_T} ADC capture type
 * @param[in] {pos_pin} ADC pin
 * @param[in] {neg_pin} negative ADC pin
 * @return none
 */
extern void hal_adc_init(ADC_TYPE_T adc_type, GPIO_PORT_PIN_T *pos_pin, GPIO_PORT_PIN_T *neg_pin);

/**
 * @note ADC start capture, return MAX value is 4095(12bits), basic voltage 5V
 * @param[in] {void} 
 * @return ADC value
 */
extern uint16_t hal_adc_get_value(void);

/**
 * @note translate adc to voltage.
 * @param[in] {adv_value} value of hal_adc_get_value.
 * @return real voltage*1000
 */
extern uint16_t hal_adc_to_voltage(uint16_t adv_value);


#ifdef __cplusplus
}
#endif

#endif



