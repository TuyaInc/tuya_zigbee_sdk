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
#include "config.h"
#include "app_common.h"
#include "app_sensor.h"
#include "tuya_zigbee_sdk.h"
#include "ias_zone_server.h"
#include "zigbee_dev_template.h"

void uart_init(void);
static void dev_evt_callback(uint8_t evt);


bool_t g_key_lock_flg = TRUE;

// power type enum
typedef enum {
    battery_source = 3,
    DC_source = 4
}power_type_t;

union key_st {
  uint8_t key_bits_flg;
   struct{
    uint8_t key_short_bit   :1;
    uint8_t key_long_bit    :1;
  }key_bits;
};
union key_st key_flg;


/**
 * @description: dev mode id write
 * @param {type} none
 * @return: none
 */
static void sensor_mode_id_select(void)
{
    uint8_t model_id[8] = {0};
    memcpy(model_id, "\6TS0203", 8);
           
    dev_zigbee_write_attribute(
        IAS_ZONE_EP,
        CLUSTER_BASIC_CLUSTER_ID,
        ATTR_MODEL_IDENTIFIER_ATTRIBUTE_ID,
        model_id,
        ATTR_CHAR_STRING_ATTRIBUTE_TYPE);
}


/**
 * @description: write battery type property function
 * @param {in} type: power source type
 * @return: none
 */
void dev_config_power_source_type(power_type_t type)
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
 * @description: report battery voltage
 * @param {in} ep: endpoind
 * @return: none
 */
void zig_report_battery_voltage(uint8_t ep)
{
    uint8_t battery_percent;
    dev_zigbee_read_attribute(
                IAS_ZONE_EP,
                CLUSTER_POWER_CONFIG_CLUSTER_ID,
                ATTR_BATTERY_PERCENTAGE_REMAINING_ATTRIBUTE_ID,
                &battery_percent,
                sizeof(battery_percent)
            );

    dev_send_data_t ret_data;
    attr_key_value_t *ret_attr = ret_data.data.zg.attr;

    memset(&ret_data, 0, sizeof(ret_data));
    /* Don't care
        ret_data.delay_time
        ret_data.random_time
        ret_data.zcl_id
        ret_data.command_type
        ret_data.addr.mode
    */
    ret_data.command_id = CMD_REPORT_ATTRIBUTES_COMMAND_ID;
    ret_data.direction = ZCL_DATA_DIRECTION_SERVER_TO_CLIENT;
    ret_data.qos = QOS_0;
    
    ret_data.addr.type.gw.src_ep = ep;
    ret_data.addr.type.gw.cluster_id = CLUSTER_POWER_CONFIG_CLUSTER_ID;
    ret_attr[0].attr_id = ATTR_BATTERY_PERCENTAGE_REMAINING_ATTRIBUTE_ID;
    ret_attr[0].type = ATTR_INT8U_ATTRIBUTE_TYPE;
    ret_attr[0].value_size = 1;
    ret_attr[0].value[0] = battery_percent;
    ret_data.data.zg.attr_sum++;
    dev_zigbee_send_data(&ret_data, NULL, 2000);
}

/**
 * @description: basic alarm pre callback function
 * @param {in} status: alarm status
 * @return: none
 */
void ias_zone_alarm1_pre_change_callback(uint16_t status)
{
    dev_led_base_op( 0, DEV_IO_ON );
    dev_timer_start_with_callback( LED_ALARM_FLASH_EVT_ID, 250, dev_evt_callback );
}

/**
 * @description: OTA callback
 * @param {in} evt: event id
 * @return: none
 */
void zg_ota_evt_callback(ZG_OTA_EVT_T evt)
{
    switch(evt) {
        case ZG_OTA_EVT_START: {
            zg_poll_interval_change( 250 );
            zg_poll_start();
            break;
        }
        default: {
            zg_poll_interval_change( 1000 );
            zg_poll_end();
            break;
        }
    }
}

/**
 * @description: event handler
 * @param: {in} evt: event id
 * @return: none
 */
void dev_evt_callback(uint8_t evt)
{
    switch( evt )
    {
        case LED_POWER_UP_SYN_EVT_ID: {
            dev_led_base_op( 0, DEV_IO_OFF );
            g_key_lock_flg = FALSE;
            break;
        }
        case LED_ALARM_FLASH_EVT_ID: {
            dev_led_base_op( 0, DEV_IO_OFF );
            break;
        }
        case NWK_START_ASSOCIATE_EVT_ID: {
            dev_led_start_blink( 0, 250, 250, DEV_LED_BLINK_FOREVER, DEV_IO_OFF );
            break;
        }
        case NWK_UP_EVT_ID: {
            ias_zone_status_update();
            break;
        }
        default:
        break;
    }
}

/**
 * @description: key short press option
 * @param: {in} key_id: key id
 * @return: none
 */
void keys_short_press_op(uint8_t key_id )
{
    switch( key_id )
    {
        case 0:{
           NET_EVT_T net_status = nwk_state_get();            
            if( (NET_JOIN_OK==net_status) || (net_status==NET_REJOIN_OK) 
                || (net_status==NET_LOST) ) {
                ias_zone_status_update();
                zig_report_battery_voltage( IAS_ZONE_EP );
            }
        }
        break;
        default:
        break;
    }
}

/**
 * @description: Key event handler
 * @param: {in} key_id: key id
 * @param: {in} key_st: key status
 * @param: {in} push_time: key push time
 * @return: none
 */
void keys_evt_handler(uint32_t key_id, key_st_t key_st, uint32_t push_time)
{
    if( g_key_lock_flg ) {
        return;
    }

    if( key_st == KEY_ST_UP ) {
        key_flg.key_bits_flg = 0;
        dev_led_base_op( 0, DEV_IO_OFF );

        if( push_time < RESET_TIME_MS ) {
            keys_short_press_op( key_id );
        }
    }
    else {
        if( push_time < RESET_TIME_MS ) {
            if( !key_flg.key_bits.key_short_bit ) {
                key_flg.key_bits.key_short_bit = 1;
                dev_led_base_op( 0, DEV_IO_ON );
            }
        }
        else {
            if( !key_flg.key_bits.key_long_bit ) {
                key_flg.key_bits.key_long_bit = 1;
                //start associate NWK
                dev_zigbee_join_start( JOIN_TIME_OUT_MS );
            }
        }
    }
}

/**
 * @description: battery sampling event callback
 * @param {in} data_st: args point
 * @return: none
 */
void battery_sampling_evt_callback(cb_args_t *data_st)
{
    if( data_st->type==CALLBACK_TYPE_CAPTURE_BATTERY ) 
    {
        if( data_st->args.battery.real_percent <= 20 ) {
            hal_battery_capture_period_change( 60*60*1000UL );
            dev_heartbeat_set( BATTERY_VOLTAGE, 60*60*1000 );
        }
        else {
            hal_battery_capture_period_change( BATTERY_REPORT_MINUTE*60*1000UL );
            dev_heartbeat_set( BATTERY_VOLTAGE, BATTERY_REPORT_MINUTE*60*1000UL );
        }
    }
}

/**
 * @description: power on init
 * @param {type} none
 * @return: none
 */
void dev_power_on_init(void)
{
    g_key_lock_flg = TRUE;
    key_flg.key_bits_flg = 0;

    // zigbee device network parameter config
    zg_dev_init();
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
    #ifdef TUYA_DEBUG
    uart_init();
    tuya_print( "/---------------system up--------------/\r\n");
    #endif

    per_dev_init();                                         // peripheral init
    power_sampler_init();

    sensor_mode_id_select();                                // mode id write
    dev_config_power_source_type( battery_source );         // write power source type
    dev_heartbeat_set( BATTERY_VOLTAGE, 4*60*60*1000UL );     // set heart beat interval 4h

    // register raw poll callback
    sdk_cb_register( CALLBACK_TYPE_POLL, zig_poll_status_evt_callback );
    sdk_cb_register( CALLBACK_TYPE_CAPTURE_BATTERY, battery_sampling_evt_callback );

    dev_led_base_op( 0, DEV_IO_ON );
    dev_timer_start_with_callback( LED_POWER_UP_SYN_EVT_ID, POWER_UP_LED_ON_TIME, dev_evt_callback );

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
            tuya_print("NET_POWER_ON_LEAVE\r\n");
            break;
        }
        case NET_JOIN_START: {
            tuya_print("NET_JOIN_START\r\n");
            dev_timer_start_with_callback( NWK_START_ASSOCIATE_EVT_ID, 0, dev_evt_callback );
            break;
        }
        case NET_JOIN_TIMEOUT: {
            tuya_print("NET_JOIN_TIMEOUT\r\n");
            dev_led_stop_blink( 0, DEV_IO_OFF);
            break;
        }
        case NET_POWER_ON_ONLINE:
        case NET_JOIN_OK: {
            tuya_print("NET_JOIN_OK\r\n");
            ias_sensor_status_sync();
            dev_led_stop_blink( 0, DEV_IO_OFF );
            dev_timer_start_with_callback( NWK_UP_EVT_ID, 3000, dev_evt_callback );
            break;
        }
        case NET_LOST: {
            tuya_print("NET_LOST\r\n");
            dev_timer_stop( NWK_UP_EVT_ID );
            dev_led_base_op( 0, DEV_IO_OFF );
            break;
        }
        case NET_REJOIN_OK:{
            tuya_print("NET_REJOIN_OK\r\n");
            ias_sensor_status_sync();
            dev_timer_start_with_callback( NWK_UP_EVT_ID, 2000, dev_evt_callback );
            break;
        }
        case NET_REMOTE_LEAVE: {
            tuya_print("NET_REMOTE_LEAVE\r\n");
            dev_timer_start_with_callback( NWK_START_ASSOCIATE_EVT_ID, 0, dev_evt_callback ) ;
            break;
        }
        case NET_LOCAL_LEAVE: {
            tuya_print("NET_LOCAL_LEAVE\r\n");
            break;
        }
        case NET_MF_TEST_LEAVE: {
            tuya_print("NET_LOCAL_LEAVE\r\n");
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
    switch(cmd)
    {
        case MF_TEST_LED_ON_ALL: {       // all leds turn on
            break;
        }
        case MF_TEST_LED_OFF_ALL: {      // all leds turn off
            break;
        }
        case MF_TEST_LED_BLINK_ALL: {    // all leds start blink
            break;
        }
        case MF_TEST_RELAY_ON_ALL: {     // relays turn on
            break;
        }
        case MF_TEST_RELAY_OFF_ALL: {    // relays turn off
            break;
        }
        case MF_TEST_RELAY_BLINK_ALL: { // relays start blink
            break;
        }
        case MF_TEST_BUTTON: {          // button mf test
            return MF_TEST_DOING;
        }
        default: {
            break;
        }
    }
    return MF_TEST_SUCCESS;
}

/**
 * @description: device report inforamtion when first power on(if need),;
 * @param {type} none
 * @return: none
 */
void dev_power_on_report_info(void)
{
    return;
}
/**
 * @description: device recovery to factury command handler
 * @param {type} none
 * @return: none
 */
void dev_recovery_factory(DEV_RESET_TYPE_T type)
{
    if( DEV_RESET_TYPE_TO_FACTORY_AND_LEAVE==type ) {
        ias_zone_unenroll( IAS_ZONE_EP );
    }
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
            break;
        }
        case CLUSTER_IAS_ZONE_CLUSTER_ID:{
            attr_value_t *attr_list = dev_msg->data.attr_data.attr_value;
            uint8_t attr_sums = dev_msg->data.attr_data.attr_value_sums;
            uint8_t ep = dev_msg->endpoint;
            if( attr_list[0].cmd==CMD_ZONE_ENROLL_RESPONSE_COMMAND_ID ) {
                dev_zigbee_specific_response_handle( dev_msg->zcl_id, dev_msg->endpoint, dev_msg->cluster );
                // get enroll reponse status
                ias_zone_enroll_response_callback( ep, attr_list[0].value[0], attr_list[0].value[1] );
            }
            else if( attr_list[0].cmd==CMD_ZONE_STATUS_CHANGE_NOTIFICATION_COMMAND_ID ) {
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
    if( cluster==CLUSTER_IAS_ZONE_CLUSTER_ID ) {

        if( attr_id==ATTR_IAS_CIE_ADDRESS_ATTRIBUTE_ID ) {

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
    else if (MODULE_NAME == TYZS3)
    {
        user_uart_config_t default_config = TYZS3_USART_CONFIG_DEFAULT;
        memcpy(&config, &default_config, sizeof(user_uart_config_t));
    }
    else if (MODULE_NAME == TYZS5)
    {
        user_uart_config_t default_config = TYZS5_USART_CONFIG_DEFAULT;
        memcpy(&config, &default_config, sizeof(user_uart_config_t));
    }
    else
    {
        return NULL;
    }
    return &config;
}

#ifdef TUYA_DEBUG
#define USER_USART_CONFIG_DEFAULT {\
    UART_ID_UART0,\
    UART_PIN_TYPE_CONFIG,\
    {UART0_TX_PORT, UART0_TX_PIN},\
    {UART0_RX_PORT, UART0_RX_PIN},\
    UART0_TX_LOC, \
    UART0_RX_LOC, \
    115200,\
    USART_PARITY_NONE,\
    USART_STOPBITS_ONE,\
    USART_DATABITS_8BIT,\
    NULL\
}

/**
 * @description: uart init func
 * @param {type} none
 * @return: none
 */
void uart_init(void)
{
    user_uart_config_t config = USER_USART_CONFIG_DEFAULT;
    user_uart_init( &config );
}
#endif
