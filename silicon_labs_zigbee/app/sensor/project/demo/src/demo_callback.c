/**
 * @Author: Deven
 * @email: liming@tuya.com
 * @LastEditors: li
 * @file name: callbacks.c
 * @Description: this file is a template of the sdk callbacks function,
 * user need to achieve the function yourself
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-03-12 17:29:33
 * @LastEditTime: 2019-04-17 15:51:35
 */

#include "stdio.h"
#include "stdlib.h"
#include "mf_test_callback.h"
#include "zigbee_sdk.h"
#include "zigbee_dev_template.h"
#include "system.h"
#include "app_sensor.h"

#define LED_POWER_UP_SYN_EVT_ID         DEV_EVT_1
#define NWK_UP_EVT_ID                   DEV_EVT_2
#define NWK_REJON_UP_EVT_ID             DEV_EVT_4
#define NWK_UP_LED_SYNC_EVT_ID          DEV_EVT_5
#define NWK_START_ASSOCIATE_EVT_ID      DEV_EVT_6

extern const gpio_config_t gpio_sensor_config[];

static bool_t button_lock = TRUE;
static uint32_t system_tick_ms = 0;
static led_mode_eu led_mode = LED_HOLD_ON_MODE;

extern void ias_zone_sensor_state_sync_callback(uint8_t newstatus);
extern void ias_zone_stack_status_callback(uint8_t nwkstatus);
extern void ias_zone_enroll_response_callback(uint8_t ep,uint8_t enrollcode,uint8_t zoneid);
extern void ias_zone_cie_address_write_callback(void);
extern void zg_dev_init(void);
extern void per_dev_init(void);
extern void power_sampler_init(void);
extern void ias_zone_sensor_init(void);
extern void ias_zone_unenroll(uint8_t ep);
extern void ias_zone_status_report(uint8_t ep,uint16_t newstatus,uint16_t dtime);
extern void dev_led_base_op(uint8_t index, DEV_IO_ST_T st);
extern void ias_sensor_status_sync(void);

static void dev_evt_callback(uint8_t evt);
void dev_key_handle(uint32_t key_id, key_st_t key_st, uint32_t push_time);


/**
 * @description: get led mode
 * @param {type} none
 * @return: none
 */
led_mode_eu get_led_mode(void)
{
    return led_mode;
}

/**
 * @description: ias zone sensor tample status change callback
 * @param {type} none
 * @return: none
 */
void ias_zone_alarm1_pre_change_callback(uint16_t status)
{
    tuya_print( "ias_zone_alarm1_pre_change_callback %d\r\n", status );
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
void ias_zone_tamper_pre_change_callback(uint16_t status)
{
    tuya_print( "ias_zone_tamper_pre_change_callback %d\r\n",status );
    if( LED_OFF_MODE==get_led_mode() )
    {
        dev_led_start_blink( 0, 200, 200, 1, DEV_IO_OFF );
    }
    return ;
}

/**
 * @description: sorft time event callback handler function 
 * @param {type} soft time event id
 * @return: none
 */
static void dev_evt_callback(uint8_t evt)
{
    switch(evt)
    {
        case LED_POWER_UP_SYN_EVT_ID:{                  // power-on 5s later sync led status
            dev_timer_stop( LED_POWER_UP_SYN_EVT_ID );
            dev_led_base_op( LED0_ID, DEV_IO_OFF );
            led_mode = LED_OFF_MODE;
            button_lock = FALSE;
            break;
        }
        case NWK_START_ASSOCIATE_EVT_ID:{               // start join nwk to turn on all lights 
            dev_timer_stop( NWK_START_ASSOCIATE_EVT_ID );
            led_mode = LED_BLINK_MODE;
            dev_led_start_blink( NWK_INDP_LED, NWK_JOIN_LED_BLINK_FREQ, NWK_JOIN_LED_BLINK_FREQ, DEV_LED_BLINK_FOREVER, DEV_IO_OFF );
            break;
        }
        case NWK_UP_LED_SYNC_EVT_ID:{                   // when nwk up led opt
            dev_timer_stop( NWK_UP_LED_SYNC_EVT_ID );
            led_mode = LED_OFF_MODE;
            dev_led_stop_blink( NWK_INDP_LED, DEV_IO_OFF );
            break;
        }
        case NWK_UP_EVT_ID:{                            // after network up opt
            dev_timer_stop( NWK_UP_EVT_ID );
            dev_led_stop_blink( NWK_INDP_LED, DEV_IO_ON );
            led_mode = LED_HOLD_ON_MODE;
            ias_sensor_status_sync();                   // after network up to sync sensor status
            dev_timer_start_with_callback( NWK_UP_LED_SYNC_EVT_ID, NWK_UP_LED_ON_TIME_MS, dev_evt_callback );
            break;
        }
        case NWK_REJON_UP_EVT_ID:{                      // after rejoin delay to sync ep status
            dev_timer_stop( NWK_REJON_UP_EVT_ID );
            uint32_t current_tick_ms = dev_current_millisecond_ticks_get();
            if( current_tick_ms-system_tick_ms < PWR_UP_DELAY_SYNC_STATUS_TIME_MS )   // when power up rejoin will be wait 3s to sync ep status
            {
                dev_timer_start_with_callback( NWK_REJON_UP_EVT_ID, 200, dev_evt_callback );
            }
            else
            {
               ias_sensor_status_sync();
            }
            break;
        }
        default:
        break;
    }
}

/**
 * @description: keys short press event handler
 * @param: {key_id} key id
 * @return: none
 */
void keys_short_press_op(uint8_t key_id )
{
    switch( key_id )
    {
        case 0:{
            ias_zone_status_update();                   // send sensor current status
            tuya_print( "key %d press\r\n",key_id );
        }
        break;      
        default:
        break;
    }
}

/**
 * @description: key event handler 
 * @param {key_id} key  id
 * @param {key_st} key status
 * @param {push_time} key push time
 * @return: none
 */
void dev_key_handle(uint32_t key_id, key_st_t key_st, uint32_t push_time)
{
    static bool_t kes_status = FALSE; 
    if( key_st == KEY_ST_UP )                           // key release
    {
        if( !button_lock )
        {
            if( push_time < KEY_LONG_PRESS_TIME_MS )    //short press release
            {
                kes_status = FALSE;
                keys_short_press_op( key_id );
            }
            else                                        //long press release
            {
                kes_status = FALSE;
                dev_zigbee_leave_for_user();
                dev_zigbee_join_start( NWK_JOIN_TIME_OUT_MS ); //start associate NWK
            }
        }
    }
    else                                                // key release
    {
        if( !button_lock )
        {
            if( push_time == KEY_LONG_PRESS_TIME_MS )
            {
                dev_led_start_blink( NWK_INDP_LED, NWK_JOIN_LED_BLINK_FREQ, NWK_JOIN_LED_BLINK_FREQ, DEV_LED_BLINK_FOREVER, DEV_IO_OFF );
            }
            if( !kes_status )
            {
                kes_status = TRUE;
                if( LED_OFF_MODE==get_led_mode() )
                {
                    dev_led_start_blink( NWK_INDP_LED, NWK_JOIN_LED_BLINK_FREQ, NWK_JOIN_LED_BLINK_FREQ, 1, DEV_IO_OFF );
                }
            }
        }
    }
}

/**
 * @description: battery source type chose
 * @param {type} none
 * @return: none
 */
void dev_config_power_source_type(power_type type)
{
    dev_zigbee_write_attribute(
            IAS_ZONE_EP,
            CLUSTER_BASIC_CLUSTER_ID,
            ATTR_POWER_SOURCE_ATTRIBUTE_ID,
            &type,
            ATTR_ENUM8_ATTRIBUTE_TYPE
        );
}

/**
 * @description: device first power on, user can register device and init the gpio
 * @param {type} none
 * @return: none
 */
void dev_power_on_init(void)
{    
    zg_dev_init();          // zigbee device network parameter config

    per_dev_init();         // peripheral init
    return;
}

/**
 * @description: system start on, zigbee stack is inited completely and
 * manufactury test is ended, the uart and the timer is ok, user can
 * init the application event and any other application variables;
 * @param {type} none
 * @return: none
 */
void dev_system_on_init(void)
{
    led_mode = LED_HOLD_ON_MODE;                                // set led mode

    dev_config_power_source_type( battery_source );             // write power type

    power_sampler_init();                                       // power sample init

    ias_zone_sensor_init();                                     // ias gpio init and register interrupt

    // dev_heartbeat_set( APP_VERSION, NWK_HEART_BEAT_TIME_MS );   // set heart beat interval 4h
    dev_heartbeat_set( BATTERY_VOLTAGE, 30*1000 );

    dev_led_base_op( LED0_ID, DEV_IO_ON );

    dev_timer_start_with_callback( LED_POWER_UP_SYN_EVT_ID, POWER_UP_LED_ON_TIME, dev_evt_callback );
    system_tick_ms = dev_current_millisecond_ticks_get();
    return;
}

/**
 * @description: network state changed callback, when network state is changed,
 * the stack will call this function to notify the user;
 * @param {state} the new network state
 * @return: none
 */
void nwk_state_changed_callback(NET_EVT_T state)
{
    ias_zone_stack_status_callback( state );
    switch(state) {
        case NET_POWER_ON_LEAVE: {
            tuya_print("NET_POWER_ON_LEAVE---\r\n");
            break;
        }
        case NET_JOIN_START: {
            tuya_print("NET_JOIN_START-------\r\n");
            dev_timer_start_with_callback( NWK_START_ASSOCIATE_EVT_ID, 0, dev_evt_callback );
            break;
        }
        case NET_JOIN_TIMEOUT: {
            tuya_print("NET_JOIN_TIMEOUT------\r\n");
            dev_led_stop_blink( NWK_INDP_LED, DEV_IO_OFF );
            led_mode = LED_OFF_MODE;
            break;
        }
        case NET_POWER_ON_ONLINE:
        case NET_JOIN_OK: {
            tuya_print("NET_JOIN_OK-----------\r\n");
            dev_timer_start_with_callback( NWK_UP_EVT_ID, 2000, dev_evt_callback );
            break;
        }
        case NET_LOST: {            // no parent,will be ato rejoin
            tuya_print("NET_LOST---------------\r\n");
            dev_timer_stop( NWK_UP_EVT_ID );
            break;
        }
        case NET_REJOIN_OK:{
            tuya_print("NET_REJOIN_OK-----------\r\n");
            dev_timer_start_with_callback( NWK_UP_EVT_ID, 2000, dev_evt_callback );
            break;
        }
        case NET_REMOTE_LEAVE: {
            tuya_print("NET_REMOTE_LEAVE-------\r\n");
            dev_timer_start_with_callback( NWK_START_ASSOCIATE_EVT_ID, 0, dev_evt_callback ) ;
            break;
        }
        case NET_LOCAL_LEAVE: {
            tuya_print("NET_LOCAL_LEAVE--------\r\n");
            break;
        }
        default: {
            break;
        }
    }
}

/**
 * @description: device manufactury test callback, when device is in manufactury test model,
 * sdk will use this callback to notify application the test item and test command;
 * @param {cmd} manufactury test type
 * @param {*args} manufactury test data
 * @param {arg_len} manufactury test data length
 * @return: none
 */
MF_TEST_RET_T dev_mf_test_callback(MF_TEST_CMD_T cmd, uint8_t *args, uint16_t arg_len)
{
    //TODO

    return MF_TEST_SUCCESS;
}

/**
 * @description: device manufactury beacon test callback, when device is in manufactury test mode,
 * sdk will use this callback to notify application to enter unsupervised production test;
 * @param {type} none
 * @return: none
 */
void dev_beacon_test_callback(void)
{
    // TODO

    return;
}

/**
 * @description: device report inforamtion when first power on(if need),;
 * @param {type} none
 * @return: none
 */
void dev_power_on_report_info(void)
{
    //TODO

    return;
}

/**
 * @description: device recovery to factury command handler
 * @param {type} none
 * @return: none
 */
void dev_recovery_factory(void)
{
    ias_zone_unenroll( IAS_ZONE_EP );    // unenroll
    return;
}

/**
 * @description: device receive message callback
 * @param {*dev_msg} received message information
 * @return: ZCL_CMD_RET_T
 */
ZCL_CMD_RET_T dev_msg_recv_callback(dev_msg_t *dev_msg)
{
    switch ( dev_msg->cluster )
    {
        case CLUSTER_PRIVATE_TUYA_CLUSTER_ID:{
            uint8_t len = dev_msg->data.bare_data.len;
            uint8_t *data = dev_msg->data.bare_data.data;
            //TODO:

            break;
        }
        case CLUSTER_IAS_ZONE_CLUSTER_ID:{
            attr_value_t *attr_list = dev_msg->data.attr_data.attr_value;
            uint8_t attr_sums = dev_msg->data.attr_data.attr_value_sums;
            uint8_t ep = dev_msg->endpoint;

            if( attr_list[0].cmd==CMD_ZONE_ENROLL_RESPONSE_COMMAND_ID )
            {
                tuya_print("enroll status %d, zone id %d\r\n",attr_list[0].value[0],attr_list[0].value[1]);
                ias_zone_enroll_response_callback( ep, attr_list[0].value[0], attr_list[0].value[1] );
                dev_zigbee_specific_response_handle( dev_msg->zcl_id, dev_msg->endpoint, dev_msg->cluster );
            }
            else if( attr_list[0].cmd==CMD_ZONE_STATUS_CHANGE_NOTIFICATION_COMMAND_ID )
            {
                dev_zigbee_specific_response_handle( dev_msg->zcl_id, dev_msg->endpoint, dev_msg->cluster );
            }
            break;
        }
        default:
        break;
    }
    
    return ZCL_CMD_RET_SUCCESS;
}

/**
 * @description: device recall scene command, recovery the scene add status
 * @param {endpoint} the endpoint of scene recall command
 * @param {*in_data} the data need to be handled of the device status
 * @return: none
 */
void dev_scene_recall_callback(uint8_t endpoint, const scene_save_data_t *in_data)
{
    //TODO

    return;
}

/**
 * @description: device scene add command, store the current status to the scene table
 * @param {endpoint} the endpoint of scene add command
 * param {*out_data} the data need to be store to scene table
 * param {*in_out_len} the data length of the store data
 * @return: none
 */
void dev_scene_add_callback(uint8_t endpoint, uint8_t *out_data, uint8_t *in_out_len)
{
    //TODO

    return;
}

/**
 * @description: write sttribute callback function
 * @param {endpoint} the endpoint of the write attribute
 * @param {cluster} the cluster id of the write attribute
 * @param {attr_id} the attribute id  of the write attribute
 * @return: none
 */
void dev_msg_write_attr_callback(uint8_t endpoint, CLUSTER_ID_T cluster, uint16_t attr_id)
{ 
    if( cluster==CLUSTER_IAS_ZONE_CLUSTER_ID )
    {
        if( ATTR_IAS_CIE_ADDRESS_ATTRIBUTE_ID )
        {
            tuya_print("rcv ice ieee ,start enroll requset\r\n");
            ias_zone_cie_address_write_callback();
        }
    }
    return;
}


/**
 * @description: mf test uart register, aotomatic generated, not edit
 * @param {void} none
 * @return: user_uart_config_t mf uart config information
 */
user_uart_config_t config;
user_uart_config_t* mf_test_uart_config(void)
{

    memset(&config, 0, sizeof(user_uart_config_t));

    if (MODULE_NAME == TYZS2R)
    {
        user_uart_config_t default_config = TYZS2R_USART_CONFIG_DEFAULT;
        memcpy(&config, &default_config, sizeof(user_uart_config_t));
    }
    else if(MODULE_NAME == TYZS5)
    {
        user_uart_config_t default_config = TYZS5_USART_CONFIG_DEFAULT;
        memcpy(&config, &default_config, sizeof(user_uart_config_t));
    }
    else
    {
        user_uart_config_t default_config = TYZS3_USART_CONFIG_DEFAULT;
        memcpy(&config, &default_config, sizeof(user_uart_config_t));
    }

    return &config;
}
