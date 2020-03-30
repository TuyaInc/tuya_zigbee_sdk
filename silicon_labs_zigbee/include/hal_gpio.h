/**
 * @author Leon
 * @par email:
 *      zhangpeng@tuya.com
 * @file hal_gpio.h
 * @brief This file abstracts all the operations of GPIO
 * @copyright HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @par company
 *      http://www.tuya.com
 * @date 2020-03-20 17:29:33
 */

#ifndef __HAL_GPIO_H__
#define __HAL_GPIO_H__

#include "type_def.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @enum Represents the output state of GPIO, OFF represents invalid, and ON represents valid
 */
typedef enum {
    DEV_IO_OFF = 0, ///< The output level is not equal to gpio_config_t->drive_flag
    DEV_IO_ON,      ///< The output level is equal to gpio_config_t->drive_flag
} DEV_IO_ST_T;

/**
 * @enum Unused interface
 */
typedef enum {
    RELAY_ST_OFF = 0,   ///< RELAY_ST_OFF
    RELAY_ST_ON,        ///< RELAY_ST_ON
}RELAY_ST_T;

/**
 * @enum Represents the state of the button. UP means release and PUSH means press down
 */
typedef enum {
    KEY_ST_UP = 0, ///< button release
    KEY_ST_PUSH,   ///< button push
}key_st_t;

/**
 * @enum GPIO port number
 */
typedef enum {
    PORT_A = 0x00, ///< portA
    PORT_B,        ///< portB
    PORT_C,
    PORT_D,
    PORT_E,
    PORT_F,
    PORT_H,
    PORT_I,
    PORT_J,
    PORT_K,
}GPIO_PORT_T;

/**
 * @enum GPIO pin number
 */
typedef enum {
    PIN_0 = 0x00, ///< PIN_0
    PIN_1,        ///< PIN_1
    PIN_2,
    PIN_3,
    PIN_4,
    PIN_5,
    PIN_6,
    PIN_7,
    PIN_8,
    PIN_9,
    PIN_10,
    PIN_11,
    PIN_12,
    PIN_13,
    PIN_14,
    PIN_15,
}GPIO_PIN_T;

/**
 * @enum GPIO function redirection
 */
typedef enum {
    LOC_0 = 0, ///< LOC_0
    LOC_1,     ///< LOC_1
    LOC_2,
    LOC_3,
    LOC_4,
    LOC_5,
    LOC_6,
    LOC_7,
    LOC_8,
    LOC_9,
    LOC_10,
    LOC_11,
    LOC_12,
    LOC_13,
    LOC_14,
    LOC_15,
    LOC_16,
    LOC_17,
    LOC_18,
    LOC_19,
    LOC_20,
    LOC_21,
    LOC_22,
    LOC_23,
    LOC_24,
    LOC_25,
    LOC_26,
    LOC_27,
    LOC_28,
    LOC_29,
    LOC_30,
    LOC_31,
}GPIO_LOC_T;


/**
 * @enum GPIO mode definition
 */
typedef enum {
    GPIO_MODE_INPUT_HIGH_IMPEDANCE = 0, ///< input mode: high impedance
    GPIO_MODE_INPUT_PULL,               ///< input mode: pull(up or down)
    GPIO_MODE_OUTPUT_PP,                ///< output mode: Push-pull 
    GPIO_MODE_OUTPUT_OD,                ///< output mode: Open drain 
    GPIO_MODE_OUTPUT_OD_PULL_UP,
    GPIO_MODE_OUTPUT_OD_PULL_DOWN,
}GPIO_MODE_T;

/**
 * @enum GPIO output/input configuration 
 */
typedef enum {
    GPIO_DOUT_LOW = 0,  ///< ouput low or input pull down
    GPIO_DOUT_HIGH = 1, ///< ouput high or input pull up
}GPIO_DOUT_T;

/**
 * @enum GPIO interrupt mode 
 */
typedef enum {
    GPIO_LEVEL_LOW = 0, ///< Drop edge triggers interrupt
    GPIO_LEVEL_HIGH,    ///< Rising edge triggers interrupt
    GPIO_LEVEL_ALL,     ///< Rising and Drop edge triggers interrupt
}GPIO_LEVEL_T;

/**
 * @note GPIO configuration description
 */
typedef struct {
    GPIO_PORT_T port;
    GPIO_PIN_T pin;
    GPIO_MODE_T mode;
    GPIO_DOUT_T out;
    GPIO_LEVEL_T drive_flag; ///< the effectively level (0 or 1)
} gpio_config_t;


typedef struct {
    GPIO_PORT_T port;
    GPIO_PIN_T pin;
}GPIO_PORT_PIN_T;


#define DEV_LED_BLINK_FOREVER 0xFFFF

typedef void(*key_func_t)(uint32_t,key_st_t,uint32_t);
typedef void (*gpio_int_func_t)(GPIO_PORT_T, GPIO_PIN_T);


/**
 * @note device output io event handler function
 * @param[in] {evt} event id
 * @param[in] {tick} event period ticks
 * @return none
 */
extern void dev_led_output_handle(uint8_t evt, uint16_t tick);

/**
 * @note device output(led) blink start funciton
 * @param[in] {led_index} led id
 * @param[in] {on_time} led on time
 * @param[in] {off_time} led off time
 * @param[in] {st} end state, end with on or off
 * @return none 
 */
extern void dev_led_start_blink(uint8_t led_index, uint16_t on_time, uint16_t off_time, uint16_t blink_times, DEV_IO_ST_T st);

/**
 * @note device output(led) blink stop function
 * @param[in] {led_index} led id
 * @param[in] {st} end state, end with on or off
 * @return none
 */
extern void dev_led_stop_blink(uint8_t led_index, DEV_IO_ST_T st);

/**
 * @note device led is blink or not
 * @param[in] {led_index} led id
 * @return 0 is not blink, 1 is blink
 */
extern uint8_t dev_led_is_blink(uint8_t led_index);
#define dev_io_op dev_led_stop_blink


//******************************************************************************
//                                                                              
//                              gpio op api                                 
//                                                                              
//******************************************************************************
/**
 * @note read input gpio status 
 * @param[in] {port} port
 * @param[in] {pin} pin
 * @return current status value
 */
extern uint8_t gpio_raw_input_read_status( GPIO_PORT_T port, GPIO_PIN_T pin);


/**
 * @note config a pin as output
 * @param[in] {port} port
 * @param[in] {pin} pin
 * @return none
 */
extern void gpio_raw_init(gpio_config_t pin);
/**
 * @note read output gpio status
 * @param[in] {port} port
 * @param[in] {pin} pin
 * @return current status value
 */
extern uint8_t gpio_raw_output_read_status(  GPIO_PORT_T port, GPIO_PIN_T pin);

/**
 * @note write output gpio status
 * @param[in] {port} port
 * @param[in] {pin} pin
 * @return none
 */
extern void gpio_raw_output_write_status(  GPIO_PORT_T port, GPIO_PIN_T pin, uint8_t value);

/**
 * @note switch output gpio to toggle's status
 * @param[in] {port} port
 * @param[in] {pin} pin
 * @return none
 */
extern void gpio_raw_output_toggle(GPIO_PORT_T port, GPIO_PIN_T pin);

/**
 * @note read input gpio status with index 
 * @param[in] {i} index
 * @return current status value
 */
extern uint8_t gpio_index_input_read_status(  uint8_t i);

/**
 * @note write output gpio status with index 
 * @param[in] {index} index
 * @param[in] {value} value
 * @return none
 */
extern void gpio_index_output_write_status(uint8_t index, uint8_t value);
//gpio input api


/**
 * @note input gpio ISR handle function register
 * @param[in] {key_func} ISR handle fuction
 * @return none
 */
extern void dev_gpio_input_set_handle_func(key_func_t key_func);

/**
 * @note input gpio ISR handle function get
 * @param[in] {type} none
 * @return ISR handle fuction
 */
extern key_func_t dev_gpio_input_get_handle_func(void);

/**
 * @note gpio button initialize
 * @param[in] {config} config information
 * @param[in] {sum} numbers of button
 * @param[in] {jitter_time} key jitter time
 * @param[in] {key_func} key ISR handle callback
 * @return none
 */
extern void gpio_button_init(gpio_config_t *config, uint8_t sum, uint32_t jitter_time, key_func_t key_func);

/**
 * @note gpio output initialize
 * @param[in] {config} config information
 * @param[in] {sum} no=umbers of output gpio information
 * @return none
 */
extern void gpio_output_init(gpio_config_t *config, uint8_t sum);

/**
 * @note gpio interrupt functoin set
 * @param[in] {config} config information
 * @param[in] {func} interrupt callback
 * @return none
 */
extern void gpio_int_register(gpio_config_t *config, gpio_int_func_t func);

/**
 * @note gpio interrupt enable
 * @param[in] {port} GPIO port
 * @param[in] {pin} GPIO pin
 * @return none
 */
extern void gpio_int_enable(GPIO_PORT_T port, GPIO_PIN_T pin);

/**
 * @note gpio interrupt disable
 * @param[in] {port} GPIO port
 * @param[in] {pin} GPIO pin
 * @return none
 */
extern void gpio_int_disable(GPIO_PORT_T port, GPIO_PIN_T pin);

#ifdef __cplusplus
}
#endif

#endif




