/*
 * @Author: Deven
 * @email: liming@tuya.com
 * @LastEditors: Deven
 * @file name: &file name&
 * @Description: 
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-04-26 17:03:11
 * @LastEditTime: 2019-06-06 13:54:48
 */
#include "stdio.h"
#include "stdlib.h"
#include "zigbee_sdk.h"
#include "system.h"
#include "zigbee_dev_template.h"
#include "app_sensor.h"
#include "gpio_sensor.h"
#include "ias_zone_server.h"
#include "app_common.h"

extern led_mode_eu get_led_mode(void);

#define IAS_ZONE_ENDPOINT               0x01
#define IAS_ZONE_STATUS_BIT_ACTIVE      0x01
#define IAS_ZONE_STATUS_BIT_INACTIVE    0x00

union {
    uint16_t ias_zone_status;
    struct {
        uint16_t alarm1_status_bit0:        1;
        uint16_t alarm2_status_bit1:        1;
        uint16_t tamper_status_bit2:        1;
        uint16_t battery_status_bit3:       1;
        uint16_t supervision_status_bit4:   1;
        uint16_t restore_status_bit5:       1;
        uint16_t trouble_status_bit6:       1;
        uint16_t ac_mains_status_bit7:      1;
    }zone_bits;
}ias_zone_status_bits;

extern uint8_t ias_zone_server_update_zone_status(uint8_t ep,uint16_t newstatus);

/**
 * @description: ias zone sensor alarm1 status change callback
 * @param {type} none
 * @return: none
 */
VIRTUAL_FUNC void ias_zone_alarm1_pre_change_callback(uint16_t status)
{
    tuya_print( "ias_zone_alarm1_pre_change_callback %d\r\n",status );
    if( LED_OFF_MODE==get_led_mode() )
    {
        dev_led_start_blink( 0, 200, 200, 1, DEV_IO_OFF );
    }
    return ;
}
/**
 * @description: ias zone sensor alarm2 status change callback
 * @param {type} none
 * @return: none
 */
VIRTUAL_FUNC void ias_zone_alarm2_pre_change_callback(uint16_t status)
{
    tuya_print( "ias_zone_alarm2_pre_change_callback %d\r\n",status );
    if( LED_OFF_MODE==get_led_mode() )
    {
        dev_led_start_blink( 0, 200, 200, 1, DEV_IO_OFF );
    }
    return ;
}

/**
 * @description: ias zone sensor tample status change callback
 * @param {type} none
 * @return: none
 */
VIRTUAL_FUNC void ias_zone_tamper_pre_change_callback(uint16_t status)
{
    tuya_print( "ias_zone_tamper_pre_change_callback %d\r\n",status );
    if( LED_OFF_MODE==get_led_mode() )
    {
        dev_led_start_blink( 0, 200, 200, 1, DEV_IO_OFF );
    }
    return ;
}
/**
 * @description: ias zone sensor trouble status change callback
 * @param {type} none
 * @return: none
 */
VIRTUAL_FUNC void ias_zone_trouble_pre_change_callback(uint16_t status)
{
    tuya_print( "ias_zone_trouble_pre_change_callback %d\r\n",status );
    if( LED_OFF_MODE==get_led_mode() )
    {
        dev_led_start_blink( 0, 200, 200, 1, DEV_IO_OFF );
    }
    return ;
}

/**
 * @description: ias zone sensor status pre-report callback
 * @param {type} none
 * @return: none
 */
VIRTUAL_FUNC bool_t ias_zone_status_pre_update_callback(uint16_t status)
{
    tuya_print( "ias_zone_status_pre_update_callback %d\r\n",status );
    return TRUE;
}

void ias_zone_status_init(void)
{
    ias_zone_status_bits.ias_zone_status = 0;
}

/**
 * @description: updata ias zone status
 * @param {type} none
 * @return: none
 */
void ias_zone_status_update(void)
{
    // update status only if conditions are met
    if( ias_zone_status_pre_update_callback( ias_zone_status_bits.ias_zone_status ) )
    {
        tuya_print( "ias_zone_status_update %d\r\n",ias_zone_status_bits.ias_zone_status );
        ias_zone_server_update_zone_status( IAS_ZONE_ENDPOINT,
                                            ias_zone_status_bits.ias_zone_status );
    }
}

/**
 * @description: ias zone sensor alarm1 status active
 * @param {type} none
 * @return: none
 */
void alarm_switch_alarm1_active(void)
{
    ias_zone_status_bits.zone_bits.alarm1_status_bit0 = IAS_ZONE_STATUS_BIT_ACTIVE;
    ias_zone_alarm1_pre_change_callback( ias_zone_status_bits.zone_bits.alarm1_status_bit0 );
    ias_zone_status_update();
}
void alarm_switch_alarm1_inactive(void)
{
    ias_zone_status_bits.zone_bits.alarm1_status_bit0 = IAS_ZONE_STATUS_BIT_INACTIVE;
    ias_zone_alarm1_pre_change_callback( ias_zone_status_bits.zone_bits.alarm1_status_bit0 );
    ias_zone_status_update();
}

/**
 * @description: ias zone sensor alarm2 status active
 * @param {type} none
 * @return: none
 */
void alarm_switch_alarm2_active(void)
{
    ias_zone_status_bits.zone_bits.alarm2_status_bit1 = IAS_ZONE_STATUS_BIT_ACTIVE;
    ias_zone_alarm2_pre_change_callback( ias_zone_status_bits.zone_bits.alarm2_status_bit1 );
    ias_zone_status_update();
}
void alarm_switch_alarm2_inactive(void)
{
    ias_zone_status_bits.zone_bits.alarm2_status_bit1 = IAS_ZONE_STATUS_BIT_INACTIVE;
    ias_zone_alarm2_pre_change_callback( ias_zone_status_bits.zone_bits.alarm2_status_bit1 );
    ias_zone_status_update();
}

/**
 * @description: ias zone sensor tamper status active
 * @param {type} none
 * @return: none
 */
void temper_switch_tamper_active_callback(void)
{
    ias_zone_status_bits.zone_bits.tamper_status_bit2 = IAS_ZONE_STATUS_BIT_ACTIVE;
    ias_zone_tamper_pre_change_callback( ias_zone_status_bits.zone_bits.tamper_status_bit2 );
    ias_zone_status_update();
}

void temper_switch_tamper_inactive(void)
{
    ias_zone_status_bits.zone_bits.tamper_status_bit2 = IAS_ZONE_STATUS_BIT_INACTIVE;
    ias_zone_tamper_pre_change_callback( ias_zone_status_bits.zone_bits.tamper_status_bit2 );
    ias_zone_status_update();
}

/**
 * @description: ias zone sensor trouble status active
 * @param {type} none
 * @return: none
 */
void trouble_switch_trouble_active(void)
{
    ias_zone_status_bits.zone_bits.trouble_status_bit6 = IAS_ZONE_STATUS_BIT_ACTIVE;
    ias_zone_trouble_pre_change_callback( ias_zone_status_bits.zone_bits.trouble_status_bit6 );
    ias_zone_status_update();
}
void trouble_switch_trouble_inactive(void)
{
    ias_zone_status_bits.zone_bits.trouble_status_bit6 = IAS_ZONE_STATUS_BIT_INACTIVE;
    ias_zone_trouble_pre_change_callback( ias_zone_status_bits.zone_bits.trouble_status_bit6 );
    ias_zone_status_update();
}

/**
 * @description: ias zone sensor status sync
 * @param {type} none
 * @return: none
 */
void ias_zone_sensor_state_sync_callback(uint8_t newstatus)
{
    tuya_print("Security new state:  %x\r\n", newstatus);
    if ((newstatus&IAS_ZONE_SENSOR_STATUS_ALARM1) == IAS_ZONE_SENSOR_STATUS_ALARM1) {
        ias_zone_status_bits.ias_zone_status = ias_zone_status_bits.ias_zone_status | IAS_ZONE_SENSOR_STATUS_ALARM1;
    } else {
        ias_zone_status_bits.ias_zone_status = ias_zone_status_bits.ias_zone_status & (~IAS_ZONE_SENSOR_STATUS_ALARM1);
    }
    if ((newstatus&IAS_ZONE_SENSOR_STATUS_ALARM2) == IAS_ZONE_SENSOR_STATUS_ALARM2) {
        ias_zone_status_bits.ias_zone_status = ias_zone_status_bits.ias_zone_status | IAS_ZONE_SENSOR_STATUS_ALARM2;
    } else {
        ias_zone_status_bits.ias_zone_status = ias_zone_status_bits.ias_zone_status & (~IAS_ZONE_SENSOR_STATUS_ALARM2);
    }
    if ((newstatus&IAS_ZONE_SENSOR_STATUS_TAMPER) == IAS_ZONE_SENSOR_STATUS_TAMPER) {
        ias_zone_status_bits.ias_zone_status = ias_zone_status_bits.ias_zone_status | IAS_ZONE_SENSOR_STATUS_TAMPER;
    } else {
        ias_zone_status_bits.ias_zone_status = ias_zone_status_bits.ias_zone_status & (~IAS_ZONE_SENSOR_STATUS_TAMPER);
    }
    if ((newstatus&IAS_ZONE_SENSOR_STATUS_TROUBLE) == IAS_ZONE_SENSOR_STATUS_TROUBLE) {
        ias_zone_status_bits.ias_zone_status = ias_zone_status_bits.ias_zone_status | IAS_ZONE_SENSOR_STATUS_TROUBLE;
    } else {
        ias_zone_status_bits.ias_zone_status = ias_zone_status_bits.ias_zone_status & (~IAS_ZONE_SENSOR_STATUS_TROUBLE);
    }
    
  ias_zone_status_update();
}

/**
 * @description: when joned nwk or rejoined nwk will be sync ias zone status 
 * @param {type} none
 * @return: none
 */
void ias_sensor_status_sync(void)
{
	static uint16_t ias_alarm_status;
    if( !gpio_raw_input_read_status( ALARM1_PORT, ALARM1_PIN ))
    {
        if ( !ALARM1_GPIO_POLARITY )
            ias_alarm_status |= IAS_ZONE_SENSOR_STATUS_ALARM1;
        else
            ias_alarm_status &= (~IAS_ZONE_SENSOR_STATUS_ALARM1);
    }
    else
    {
        if ( !ALARM1_GPIO_POLARITY )
            ias_alarm_status &= (~IAS_ZONE_SENSOR_STATUS_ALARM1);
        else
            ias_alarm_status |= IAS_ZONE_SENSOR_STATUS_ALARM1;
    }

    if( !gpio_raw_input_read_status( TAMPER_PORT, TAMPER_PIN ))
    {
        if ( !TAMPER_GPIO_POLARITY )
            ias_alarm_status |= IAS_ZONE_SENSOR_STATUS_TAMPER;
        else
            ias_alarm_status &= (~IAS_ZONE_SENSOR_STATUS_TAMPER);
    }
    else
    {
        if ( !TAMPER_GPIO_POLARITY )
            ias_alarm_status &= (~IAS_ZONE_SENSOR_STATUS_TAMPER);
        else
            ias_alarm_status |= IAS_ZONE_SENSOR_STATUS_TAMPER;
    }
    ias_zone_sensor_state_sync_callback( ias_alarm_status );
}

/**
 * @description: ias zone gpio status change callback
 * @param {type} none
 * @return: none
 */
void ias_zone_gpio_sensor_status_change_callback(uint8_t bits, uint8_t gpstatus)
{
    switch( bits )
    {
        #if IAS_ALARM1_ENABLE == TRUE
            case IAD_ZONE_ALARM1:
                if( gpstatus==GPIO_SENSOR_INACTIVE ){
                    alarm_switch_alarm1_inactive();
                }
                else{
                    alarm_switch_alarm1_active();
                }
            break;
        #endif
        #if IAS_TAMPER_ENABLE == TRUE
            case IAD_ZONE_TAMPER:
                if( gpstatus==GPIO_SENSOR_INACTIVE ){
                    temper_switch_tamper_inactive();
                }
                else{
                    temper_switch_tamper_active_callback();
                }
            break;
        #endif
        #if IAS_TROUBLE_ENABLE == TRUE
            case IAD_ZONE_TROUBLE:
                if( gpstatus==GPIO_SENSOR_INACTIVE ){
                    trouble_switch_trouble_inactive();
                }
                else{
                    trouble_switch_trouble_active();
                }
            break;
        #endif
        default:
        break;
    }
}

