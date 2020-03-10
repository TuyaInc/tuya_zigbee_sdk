/*
 * @Author: Deven
 * @email: liming@tuya.com
 * @LastEditors: Deven
 * @file name: &file name&
 * @Description: 
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-04-26 17:03:11
 * @LastEditTime: 2019-06-06 11:45:50
 */
#include "stdio.h"
#include "stdlib.h"
#include "zigbee_sdk.h"
#include "system.h"
#include "zigbee_dev_template.h"
#include "gpio_sensor.h"
#include "app_common.h"

#define GPIO_ALARM1_DEBOUNCE_EVT_ID     DEV_EVT_16
#define GPIO_TAMPER_DEBOUNCE_EVT_ID     DEV_EVT_15
#define GPIO_TROUBLE_DEBOUNCE_EVT_ID    DEV_EVT_14

uint8_t new_sensor_status[SENSOR_NUM];
uint8_t last_sensor_status[SENSOR_NUM];

extern void ias_zone_gpio_sensor_status_change_callback(uint8_t bits,uint8_t gpstatus);
extern const gpio_config_t gpio_sensor_config[];

/**
 * @description: sensor pin sample debounce handler
 * @param {type} none
 * @return: none
 */
void gpio_sensor_debounce_handler(uint8_t evt)
{
    #if IAS_ALARM1_ENABLE
        if( evt==GPIO_ALARM1_DEBOUNCE_EVT_ID )
        {
            dev_timer_stop( GPIO_ALARM1_DEBOUNCE_EVT_ID );
            last_sensor_status[0] =  new_sensor_status[0];
            ias_zone_gpio_sensor_status_change_callback( IAD_ZONE_ALARM1, new_sensor_status[0] );
        }
    #endif
    #if IAS_TAMPER_ENABLE
        else if ( evt==GPIO_TAMPER_DEBOUNCE_EVT_ID )
        {
            dev_timer_stop( GPIO_TAMPER_DEBOUNCE_EVT_ID );
            last_sensor_status[1] =  new_sensor_status[1];
            ias_zone_gpio_sensor_status_change_callback( IAD_ZONE_TAMPER, new_sensor_status[1] );
        }
    #endif
    #if IAS_TROUBLE_ENABLE == TRUE
        else
        {
            dev_timer_stop( IAS_TROUBLE_ENABLE );
            last_sensor_status[2] =  new_sensor_status[2];
            ias_zone_gpio_sensor_status_change_callback( IAD_ZONE_TROUBLE, new_sensor_status[2] );
        }
    #endif
}
/**
 * @description: ias zone alarm1 gpio debounce func
 * @param {type} none
 * @return: none
 */
void gpio_sensor_alarm1_debounce(gpiostate newstatus)
{
    tuya_print("alarm1 new status %d,last status %d\r\n",newstatus,last_sensor_status[0]);
    if ( newstatus==last_sensor_status[0] )
    {
        dev_timer_stop( GPIO_ALARM1_DEBOUNCE_EVT_ID );
        return ;
    }
    if( newstatus==GPIO_SENSOR_INACTIVE )
    {
        new_sensor_status[0] = GPIO_SENSOR_INACTIVE;
        dev_timer_start_with_callback( GPIO_ALARM1_DEBOUNCE_EVT_ID, ALARM1_VIBRATE_TIME, gpio_sensor_debounce_handler );
        return ;
    }
    if( newstatus==GPIO_SENSOR_ACTIVE )
    {
        new_sensor_status[0] = GPIO_SENSOR_ACTIVE;
        dev_timer_start_with_callback( GPIO_ALARM1_DEBOUNCE_EVT_ID, ALARM1_VIBRATE_TIME, gpio_sensor_debounce_handler );
        return ;
    }
}

/**
 * @description: ias zone tamper gpio debounce func
 * @param {type} none
 * @return: none
 */
void gpio_sensor_tamper_debounce(gpiostate newstatus)
{
    #if IAS_TAMPER_ENABLE == TRUE
        tuya_print( "tamper new status %d,last status %d\r\n",newstatus,last_sensor_status[1] );
        if ( newstatus==last_sensor_status[1] )
        {
            dev_timer_stop( GPIO_TAMPER_DEBOUNCE_EVT_ID );
            return ;
        }
        if( newstatus==GPIO_SENSOR_INACTIVE )
        {
            new_sensor_status[1] = GPIO_SENSOR_INACTIVE;
            dev_timer_start_with_callback( GPIO_TAMPER_DEBOUNCE_EVT_ID, TAMPER_VIBRATE_TIME, gpio_sensor_debounce_handler );
            return ;
        }
        if( newstatus==GPIO_SENSOR_ACTIVE )
        {
            new_sensor_status[1] = GPIO_SENSOR_ACTIVE;
            dev_timer_start_with_callback( GPIO_TAMPER_DEBOUNCE_EVT_ID, TAMPER_VIBRATE_TIME, gpio_sensor_debounce_handler );
            return ;
        }
    #endif
}

/**
 * @description: ias zone trouble gpio debounce func
 * @param {type} none
 * @return: none
 */
void gpio_sensor_trouble_debounce(gpiostate newstatus)
{
    #if IAS_TROUBLE_ENABLE == TRUE
        tuya_print( "trouble status %d,last status %d\r\n",newstatus,last_sensor_status[2] );
        if ( newstatus==last_sensor_status[2] )
        {
            dev_timer_stop( GPIO_TROUBLE_DEBOUNCE_EVT_ID );
            return ;
        }
        if( newstatus==GPIO_SENSOR_INACTIVE )
        {
            new_sensor_status[2] = GPIO_SENSOR_INACTIVE;
            dev_timer_start_with_callback( GPIO_TROUBLE_DEBOUNCE_EVT_ID, TROUBLE_VIBRATE_TIME, gpio_sensor_debounce_handler );
            return ;
        }
        if( newstatus==GPIO_SENSOR_ACTIVE )
        {
            new_sensor_status[2] = GPIO_SENSOR_ACTIVE;
            dev_timer_start_with_callback( GPIO_TROUBLE_DEBOUNCE_EVT_ID, TROUBLE_VIBRATE_TIME, gpio_sensor_debounce_handler );
            return ;
        }
    #endif
}

/**
 * @description: ias zone alarm1 gpio interrupt
 * @param {type} none
 * @return: none
 */
void gpio_sensor_alarm1_interrupt_handler(GPIO_PORT_T port, GPIO_PIN_T pin)
{
    #if IAS_ALARM1_ENABLE == TRUE
        if( ( ALARM1_PORT==port ) && 
            ( ALARM1_PIN==pin ) )
        {
            uint8_t alarm1_value;
            alarm1_value = gpio_raw_input_read_status( ALARM1_PORT, ALARM1_PIN );
            if ( ALARM1_GPIO_POLARITY ){
                if( alarm1_value==0 ){
                    gpio_sensor_alarm1_debounce( GPIO_SENSOR_INACTIVE );
                }
                else{
                    gpio_sensor_alarm1_debounce( GPIO_SENSOR_ACTIVE );
                }
            }
            else{
                if( alarm1_value==0 ){
                    gpio_sensor_alarm1_debounce( GPIO_SENSOR_ACTIVE );
                }
                else{
                    gpio_sensor_alarm1_debounce( GPIO_SENSOR_INACTIVE );
                }
            }
        }
    #endif
}

/**
 * @description: ias zone tamper gpio interrupt
 * @param {type} none
 * @return: none
 */
void gpio_sensor_tamper_interrupt_handler(GPIO_PORT_T port, GPIO_PIN_T pin)
{
    #if IAS_TAMPER_ENABLE == TRUE
        if( ( TAMPER_PORT==port ) && 
            ( TAMPER_PIN==pin ) )
        {
            uint8_t tamper_value;
            tamper_value = gpio_raw_input_read_status( TAMPER_PORT, TAMPER_PIN );
            if ( TAMPER_GPIO_POLARITY ){
                if( tamper_value==0 ){
                    gpio_sensor_tamper_debounce( GPIO_SENSOR_INACTIVE );
                }
                else{
                    gpio_sensor_tamper_debounce( GPIO_SENSOR_ACTIVE );
                }
            }
            else{
                if( tamper_value==0 ){
                    gpio_sensor_tamper_debounce( GPIO_SENSOR_ACTIVE );
                }
                else{
                    gpio_sensor_tamper_debounce( GPIO_SENSOR_INACTIVE );
                }
            }
        }
    #endif
}

/**
 * @description: ias zone trouble gpio interrupt
 * @param {type} none
 * @return: none
 */
void gpio_sensor_trouble_interrupt_handler(GPIO_PORT_T port, GPIO_PIN_T pin)
{
    #if IAS_TROUBLE_ENABLE == TRUE
        if( ( TROUBLE_PORT==port ) && 
            ( TROUBLE_PIN==pin ) )
        {
            uint8_t trouble_value;
            trouble_value = gpio_raw_input_read_status( TROUBLE_PORT, TROUBLE_PIN );
            if ( TROUBLE_GPIO_POLARITY ){
                if( trouble_value==0 ){
                    gpio_sensor_trouble_debounce( GPIO_SENSOR_INACTIVE );
                }
                else{
                    gpio_sensor_trouble_debounce( GPIO_SENSOR_ACTIVE );
                }
            }
            else{
                if( trouble_value==0 ){
                    gpio_sensor_trouble_debounce( GPIO_SENSOR_ACTIVE );
                }
                else{
                    gpio_sensor_trouble_debounce( GPIO_SENSOR_INACTIVE );
                }
            }
        }
    #endif
}

/**
 * @description: ias zone sensor gpio init
 * @param {type} none
 * @return: none
 */
void gpio_sensor_interrupt_init(void)
{
    uint8_t read_value[SENSOR_NUM];

    #if IAS_ALARM1_ENABLE == TRUE
    gpio_int_register( (gpio_config_t *)&gpio_sensor_config[0], gpio_sensor_alarm1_interrupt_handler );
        read_value[0] = gpio_raw_input_read_status( gpio_sensor_config[0].port, gpio_sensor_config[0].pin );
        if ( ALARM1_GPIO_POLARITY ) {
            if ( read_value[0] ) {
                new_sensor_status[0] = GPIO_SENSOR_ACTIVE;
                last_sensor_status[0] = new_sensor_status[0];
            } else {
                new_sensor_status[0] = GPIO_SENSOR_INACTIVE;
                last_sensor_status[0] = new_sensor_status[0];
            }
        } else {
            if ( read_value[0]==0 ) {
                new_sensor_status[0] = GPIO_SENSOR_ACTIVE;
                last_sensor_status[0] = new_sensor_status[0];
            } else {
                new_sensor_status[0] = GPIO_SENSOR_INACTIVE;
                last_sensor_status[0] = new_sensor_status[0];
            }
        }
        ias_zone_gpio_sensor_status_change_callback( IAD_ZONE_ALARM1, new_sensor_status[0] );
    #endif
    #if IAS_TAMPER_ENABLE == TRUE
        gpio_int_register( (gpio_config_t *)&gpio_sensor_config[1], gpio_sensor_tamper_interrupt_handler );
        read_value[1] = gpio_raw_input_read_status( gpio_sensor_config[1].port, gpio_sensor_config[1].pin );
        tuya_print( "tamper gpio status %d\r\n",read_value[1] );
        if ( TAMPER_GPIO_POLARITY ) {
            if ( read_value[1] ) {
                new_sensor_status[1] = GPIO_SENSOR_ACTIVE;
                last_sensor_status[1] = new_sensor_status[1];
            } else {
                new_sensor_status[1] = GPIO_SENSOR_INACTIVE;
                last_sensor_status[1] = new_sensor_status[1];
            }
        } else {
            if ( read_value[1]==0 ) {
                new_sensor_status[1] = GPIO_SENSOR_ACTIVE;
                last_sensor_status[1] = new_sensor_status[1];
            } else {
                new_sensor_status[1] = GPIO_SENSOR_INACTIVE;
                last_sensor_status[1] = new_sensor_status[1];
            }
        }
        ias_zone_gpio_sensor_status_change_callback( IAD_ZONE_TAMPER, new_sensor_status[1] );
    #endif
    #if IAS_TROUBLE_ENABLE == TRUE
        read_value[2] = gpio_raw_input_read_status( gpio_sensor_config[2].port, gpio_sensor_config[2].pin );
        tuya_print( "trouble gpio status %d\r\n",read_value[2] );
        if ( TROUBLE_GPIO_POLARITY ) {
            if ( read_value[2] ) {
                new_sensor_status[2] = GPIO_SENSOR_ACTIVE;
                last_sensor_status[2] = new_sensor_status[2];
            } else {
                new_sensor_status[2] = GPIO_SENSOR_INACTIVE;
                last_sensor_status[2] = new_sensor_status[2];
            }
        } else {
            if ( read_value[2]==0 ) {
                new_sensor_status[2] = GPIO_SENSOR_ACTIVE;
                last_sensor_status[2] = new_sensor_status[2];
            } else {
                new_sensor_status[2] = GPIO_SENSOR_INACTIVE;
                last_sensor_status[2] = new_sensor_status[2];
            }
        }
        ias_zone_gpio_sensor_status_change_callback( IAD_ZONE_TROUBLE, new_sensor_status[2] );
    #endif
}


