/*
 * @Author: Deven
 * @email: liming@tuya.com
 * @LastEditors: Deven
 * @file name: &file name&
 * @Description: 
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-04-26 17:03:11
 * @LastEditTime: 2019-06-07 14:48:21
 */
#include "stdio.h"
#include "stdlib.h"
#include "zigbee_sdk.h"
#include "app_common.h"
#include "system.h"
#include "zigbee_dev_template.h"
#include "ias_zone_server.h"
#include "gpio_sensor.h"

extern void gpio_sensor_interrupt_init(void);
extern void ias_zone_info_init_callback(uint8_t ep);
extern void dev_key_handle(uint32_t key_id, key_st_t key_st, uint32_t push_time);
extern void gpio_sensor_interrupt_handler(GPIO_PORT_T port, GPIO_PIN_T pin);

// key configure list
const gpio_config_t gpio_key_config[] ={
    KEYS_IO_LIST
};

const gpio_config_t gpio_sensor_config[] ={
    SENSORS_IO_LIST
};

// output io configure list
const gpio_config_t gpio_output_config[] ={
    LEDS_IO_LIST
};

const gpio_config_t gpio_led_config[] ={
    LEDS_IO_LIST
};

// ias zone attributes list
#define IAS_ZONE_ATTR_LIST1 \
    { 0x0000, ATTR_ENUM8_ATTRIBUTE_TYPE, 1, (0x00), { (uint8_t*)0x00 } }, /* 21 / IAS Zone / zone state*/\
    { 0x0001, ATTR_ENUM16_ATTRIBUTE_TYPE, 2, (ATTR_MASK_TOKENIZE), { (uint8_t*)0x0028 } }, /* 22 / IAS Zone / zone type*/\
    { 0x0002, ATTR_BITMAP16_ATTRIBUTE_TYPE, 2, (ATTR_MASK_TOKENIZE), { (uint8_t*)0x0000 } }, /* 23 / IAS Zone / zone status*/\
    { 0x0010, ATTR_IEEE_ADDRESS_ATTRIBUTE_TYPE, 8, (ATTR_MASK_TOKENIZE|ATTR_MASK_WRITABLE), { NULL } }, /* 24 / IAS Zone / IAS CIE address*/\
    { 0x0011, ATTR_INT8U_ATTRIBUTE_TYPE, 1, (ATTR_MASK_TOKENIZE), { (uint8_t*)0xff } }, /* 25 / IAS Zone / Zone ID*/\
    { 0xFFFD, ATTR_INT16U_ATTRIBUTE_TYPE, 2, (0x00), { (uint8_t*)0x0001 } }, /* 26 / IAS Zone / cluster revision*/\

// ias zone attribute
const attr_t ias_zone_attr_list[] = {
    IAS_ZONE_ATTR_LIST1
};

// power attribute
const attr_t power_attr_list[] = {
    POWER_ATTR_LIST
};

// application cluster
const cluster_t app_server_cluster_list[] ={
    DEF_CLUSTER_POWER_CLUSTER_ID( power_attr_list )
    DEF_CLUSTER_IAS_ZONE_CLUSTER_ID( ias_zone_attr_list )
};

#define SERVER_CLUSTER_NUM  get_array_len(app_server_cluster_list)
// application device description
const dev_description_t zig_dev_desc_model[] ={
    { 1, ZHA_PROFILE_ID, ZG_DEVICE_ID_IAS_ZONE, SERVER_CLUSTER_NUM, (cluster_t *)&app_server_cluster_list[0], 0, NULL },
};

dev_description_t *zig_dev_desc_list = NULL;


/**
 * @description: gipo control
 * @param {type} none
 * @return: none
 */
void dev_gpio_op(const gpio_config_t gpio_config[],uint8_t index, DEV_IO_ST_T st)
{
    if(st == DEV_IO_ON) {
        if (gpio_config[index].drive_flag == GPIO_LEVEL_HIGH) {
            gpio_raw_output_write_status( gpio_config[index].port, gpio_config[index].pin, GPIO_LEVEL_HIGH );
        } else {
            gpio_raw_output_write_status( gpio_config[index].port, gpio_config[index].pin, GPIO_LEVEL_LOW );
        }
    } else {
        if (gpio_config[index].drive_flag == GPIO_LEVEL_LOW) {
            gpio_raw_output_write_status( gpio_config[index].port, gpio_config[index].pin, GPIO_LEVEL_HIGH );
        } else {
            gpio_raw_output_write_status( gpio_config[index].port, gpio_config[index].pin, GPIO_LEVEL_LOW );
        }
    }
}

/**
 * @description: Dynamic generation endpoint from zig_dev_desc_model
 * @param {type} none
 * @return: none
 */
static uint8_t zig_dev_desc_ep_init(void)
{
    uint8_t i = 0;
    zig_dev_desc_list = (dev_description_t *)malloc(sizeof(zig_dev_desc_model)*DEV_EP_SUM);
    if( zig_dev_desc_list==NULL )
    {
        tuya_print("comm----malloc fail\r\n");
       return FALSE;
    }
    for( i=0;i<DEV_EP_SUM;i++ )
    {
        memcpy( &zig_dev_desc_list[i],zig_dev_desc_model,sizeof(zig_dev_desc_model) );
        zig_dev_desc_list[i].ep = i+1;
    }
    return TRUE;
}

/**
 * @description: zigbee device init
 * @param {type} none
 * @return: none
 */

void zg_dev_init(void)
{
    // device info registe
    zg_dev_config_t st_zg_dev_config;
    
    // Dynamic generation endpoint 
    zig_dev_desc_ep_init();

    // zigbee device info register
    dev_register_zg_ep_infor( (dev_description_t *)zig_dev_desc_list, DEV_EP_SUM );
    
    // device type choice
    st_zg_dev_config.dev_type = ZG_SLEEPY_END_DEVICE;                                           // sleep end device
    st_zg_dev_config.beacon_send_interval_for_join = 300;                                       // Channel switching period 300ms
    st_zg_dev_config.zb_scan_duration = ZB_SCAN_DURATION_3;                                     // RF rx open interval
    st_zg_dev_config.beacon_send_interval_for_rejoin = 1000;                                    // rejoin beacon send interval 
    
    // poll parameter registe
    st_zg_dev_config.config.sleep_dev_cfg.poll_conifg.poll_interval = 1000;                     // set polling interval 1s
    st_zg_dev_config.config.sleep_dev_cfg.poll_conifg.wait_app_ack_time = 1200;                 // wait app ack,wait_app_ack_time >= poll_interval
    st_zg_dev_config.config.sleep_dev_cfg.poll_conifg.poll_forever_flag = FALSE;                // no permanent polling
    st_zg_dev_config.config.sleep_dev_cfg.poll_conifg.poll_failed_times = 3;                    // poll miss max

    // rejoin parameter registe
    st_zg_dev_config.config.sleep_dev_cfg.rejoin_config.next_rejoin_time = (4*60*60*1000);      // rejoin group interval 4h
    st_zg_dev_config.config.sleep_dev_cfg.rejoin_config.wake_up_time_after_join = (30*1000);    // continuous poll interval after joined
    st_zg_dev_config.config.sleep_dev_cfg.rejoin_config.wake_up_time_after_rejoin = (10*1000);  // continuous poll interval after rejoined
    st_zg_dev_config.config.sleep_dev_cfg.rejoin_config.rejoin_try_times = 6;                   // try rejoin times
    st_zg_dev_config.config.sleep_dev_cfg.rejoin_config.power_on_auto_rejoin_flag = TRUE;       // power on auto rejoin
    dev_register_zg_dev_config( &st_zg_dev_config );
   
    // join parameter registe
    join_config_t st_join_config;
    st_join_config.auto_join_power_on_flag = FALSE;                                             // power up not auto join
    st_join_config.auto_join_remote_leave_flag = TRUE;                                          // remote leave auto rejoin
    st_join_config.join_timeout = (30*1000);                                                    // associate timeout
    dev_zg_join_config( &st_join_config );
}

/**
 * @description: device peripherals init
 * @param {type} none
 * @return: none
 */
void per_dev_init(void)
{
    gpio_output_init((gpio_config_t *)gpio_output_config, get_array_len(gpio_output_config));
    gpio_button_init((gpio_config_t *)gpio_key_config, get_array_len(gpio_key_config), 5, dev_key_handle); 
}

/**
 * @description: power sampler init
 * @param {type} none
 * @return: none
 */
#define BATTERY_REPORT_PERIOD               4*3600*1000UL
void power_sampler_init(void)
{
    battery_cfg_t cfg = {
        5*60*1000UL,
        5000,
        BATTERY_REPORT_PERIOD,
        3000,
        2400,
    };

    battery_table_t table[] = {
        BATTERY_DEFAULT_TABLE
    };

    hal_battery_config( &cfg, table, get_array_len(table) );
}

/**
 * @description: ias zone sensor init
 * @param {type} none
 * @return: none
 */
extern void ias_zone_status_init(void);
void ias_zone_sensor_init(void)
{
    ias_zone_status_init();
    gpio_sensor_interrupt_init();
    ias_zone_info_init_callback( IAS_ZONE_EP );
}
