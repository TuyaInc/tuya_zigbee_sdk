/*
 * @file: 
 * @brief: 
 * @author: Deven
 * @date: 2019-11-13 20:17:48
 * @email: liming@tuya.com
 * @copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @company: http://www.tuya.com
 */

#include "stdio.h"
#include "stdlib.h"
#include "tuya_zigbee_sdk.h"
#include "config.h"
#include "gpio_sensor.h"
#include "app_common.h"

#define ALARM1_VIBRATE_TIME             200
sensor_t alarm_info;

extern void ias_zone_alarm1_gpio_status_change_callback(uint8_t gpstatus);

/**
 * @description: sensor pin sample debounce handler
 * @param {type} none
 * @return: none
 */
void gpio_sensor_debounce_handler(uint8_t evt)
{
    if( evt == GPIO_ALARM1_DEBOUNCE_EVT_ID ) {

        dev_timer_stop( GPIO_ALARM1_DEBOUNCE_EVT_ID );
        alarm_info.last_status = alarm_info.new_status;
        ias_zone_alarm1_gpio_status_change_callback( alarm_info.new_status );
    }
}
/**
 * @description: ias zone alarm1 gpio debounce func
 * @param {type} none
 * @return: none
 */
void gpio_sensor_alarm1_debounce(gpiostate newstatus)
{
    tuya_print("alarm1 new status %d,last status %d\r\n",newstatus,alarm_info.last_status);
    if( newstatus == (gpiostate)alarm_info.last_status ) {
        dev_timer_stop( GPIO_ALARM1_DEBOUNCE_EVT_ID );
        return ;
    }
    if( newstatus == GPIO_SENSOR_INACTIVE ) {
        alarm_info.new_status = GPIO_SENSOR_INACTIVE;
    }
    else {
        alarm_info.new_status = GPIO_SENSOR_ACTIVE;
    }
    dev_timer_start_with_callback( GPIO_ALARM1_DEBOUNCE_EVT_ID, ALARM1_VIBRATE_TIME, gpio_sensor_debounce_handler );
}

/**
 * @description: ias zone alarm1 gpio interrupt
 * @param {type} none
 * @return: none
 */
void gpio_sensor_alarm1_interrupt_handler(GPIO_PORT_T port, GPIO_PIN_T pin)
{
    uint8_t alarm1_value;

    alarm1_value = gpio_raw_input_read_status( alarm_info.sensor_io_st.port,
                                               alarm_info.sensor_io_st.pin );
    tuya_print("alarm st %d\r\n",alarm1_value);
    if ( alarm_info.gpio_polarity ) {
        if( alarm1_value == DEV_IO_OFF ) {
            gpio_sensor_alarm1_debounce( GPIO_SENSOR_INACTIVE );
        }
        else {
            gpio_sensor_alarm1_debounce( GPIO_SENSOR_ACTIVE );
        }
    }
    else {
        if( alarm1_value==DEV_IO_OFF ) {
            gpio_sensor_alarm1_debounce( GPIO_SENSOR_ACTIVE );
        }
        else {
            gpio_sensor_alarm1_debounce( GPIO_SENSOR_INACTIVE );
        }
    }
}

/**
 * @description: sensor init gpio set
 * @param {type} none
 * @return: none
 */
uint8_t sensor_init_io_status_set(sensor_t *senor_info)
{
    if ( senor_info->gpio_polarity == DEV_IO_ON ) {
        if ( senor_info->init_status == DEV_IO_ON ) {
            senor_info->new_status = GPIO_SENSOR_ACTIVE;
        } else {
            senor_info->new_status = GPIO_SENSOR_INACTIVE;
        }
    } else {
        if ( senor_info->init_status == DEV_IO_OFF ) {
            senor_info->new_status = GPIO_SENSOR_ACTIVE;
        } else {
            senor_info->new_status = GPIO_SENSOR_INACTIVE;
        }
    }
    senor_info->last_status = senor_info->new_status;
    return 1;
}

/**
 * @description: ias zone sensor basic alarm gpio init
 * @param {type} none
 * @return: none
 */
void sensor_basic_alarm_init_io_status_updata(void)
{
    if( !sensor_init_io_status_set( &alarm_info ) ) {
        return;
    }
    
    ias_zone_alarm1_gpio_status_change_callback( alarm_info.new_status );
}


