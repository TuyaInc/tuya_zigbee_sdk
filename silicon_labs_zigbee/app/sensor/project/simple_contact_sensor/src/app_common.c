/*
 * @file: 
 * @brief: 
 * @author: Deven
 * @date: 2019-11-13 20:17:48
 * @email: liming@tuya.com
 * @copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @company: http://www.tuya.com
 */

#include "config.h"
#include "tuya_zigbee_sdk.h"
#include "app_common.h"
#include "zigbee_dev_template.h"


extern void ias_zone_status_init(void);
extern void ias_zone_info_init_callback(uint8_t ep,uint16_t zone_type);
extern void sensor_basic_alarm_init_io_status_updata(void);

extern void dev_led_base_op(uint8_t index, DEV_IO_ST_T st);
extern void keys_evt_handler(uint32_t key_id, key_st_t key_st, uint32_t push_time);

// key configure list
const gpio_config_t gpio_key_config[] ={
    KEYS_IO_LIST
};

// output io configure list
const gpio_config_t gpio_output_config[] ={
    LEDS_IO_LIST
};

const gpio_config_t gpio_sensor_config[] ={
    SENSORS_IO_LIST
};

#define IAS_ZONE_ATTR_LIST1 \
    { 0x0000, ATTR_ENUM8_ATTRIBUTE_TYPE, 1, (0x00), { (uint8_t*)0x00 } }, /* 21 / IAS Zone / zone state*/\
    { 0x0001, ATTR_ENUM16_ATTRIBUTE_TYPE, 2, (ATTR_MASK_TOKENIZE), { (uint8_t*)0x0028 } }, /* 22 / IAS Zone / zone type*/\
    { 0x0002, ATTR_BITMAP16_ATTRIBUTE_TYPE, 2, (ATTR_MASK_TOKENIZE), { (uint8_t*)0x0000 } }, /* 23 / IAS Zone / zone status*/\
    { 0x0010, ATTR_IEEE_ADDRESS_ATTRIBUTE_TYPE, 8, (ATTR_MASK_TOKENIZE|ATTR_MASK_WRITABLE), { NULL } }, /* 24 / IAS Zone / IAS CIE address*/\
    { 0x0011, ATTR_INT8U_ATTRIBUTE_TYPE, 1, (ATTR_MASK_TOKENIZE), { (uint8_t*)0xff } }, /* 25 / IAS Zone / Zone ID*/\
    { 0xFFFD, ATTR_INT16U_ATTRIBUTE_TYPE, 2, (0x00), { (uint8_t*)0x0001 } }, /* 26 / IAS Zone / cluster revision*/\

// application attribute
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
 * @description: Dynamic generation endpoint from zig_dev_desc_model
 * @param {type} none
 * @return: none
 */
static uint8_t zig_dev_desc_ep_init(void)
{
    uint8_t i = 0;
    
    zig_dev_desc_list = (dev_description_t *)malloc(sizeof(zig_dev_desc_model)*DEV_EP_SUM);
    if( zig_dev_desc_list==NULL ) {
        return FALSE;
    }
    for( i=0;i<DEV_EP_SUM;i++ ) {
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
    memset(&st_zg_dev_config, 0, sizeof(st_zg_dev_config));

    // Dynamic generation endpoint 
    zig_dev_desc_ep_init();

    // zigbee device info register
    dev_register_zg_ep_infor( (dev_description_t *)zig_dev_desc_list, DEV_EP_SUM );
    
    // device type choice
    st_zg_dev_config.dev_type = ZG_SLEEPY_END_DEVICE;
    st_zg_dev_config.beacon_send_interval_for_join = CHANNEL_SW_PER_MS;                     // Channel switching period 
    st_zg_dev_config.zb_scan_duration = ZB_SCAN_DURATION_3;                                 // RF rx open interval
    st_zg_dev_config.beacon_send_interval_for_rejoin = BEACON_PER_MS;                       // rejoin beacon send interval 
    
    // poll parameter registe
    st_zg_dev_config.config.sleep_dev_cfg.poll_conifg.poll_interval = POLL_INTERVAL_MS;
    st_zg_dev_config.config.sleep_dev_cfg.poll_conifg.wait_app_ack_time = WAIT_APP_ACK_MS;
    st_zg_dev_config.config.sleep_dev_cfg.poll_conifg.poll_forever_flag = POLL_FOREVER;
    st_zg_dev_config.config.sleep_dev_cfg.poll_conifg.poll_failed_times = POLL_MISS_MAX;

    // rejoin parameter registe
    st_zg_dev_config.config.sleep_dev_cfg.rejoin_config.next_rejoin_time = (NEXT_REJOIN_PER_HOUR*60*60*1000);       // rejoin group interval
    st_zg_dev_config.config.sleep_dev_cfg.rejoin_config.wake_up_time_after_join = JOINED_CONTINUE_POLL_TIME_MS;     // continuous poll interval after joined
    st_zg_dev_config.config.sleep_dev_cfg.rejoin_config.wake_up_time_after_rejoin = REJOINED_CONTINUE_POLL_TIME_MS; // continuous poll interval after rejoined
    st_zg_dev_config.config.sleep_dev_cfg.rejoin_config.rejoin_try_times = BEACON_TIME;                             // try rejoin times
    st_zg_dev_config.config.sleep_dev_cfg.rejoin_config.power_on_auto_rejoin_flag = POWER_ON_REJOIN;
    st_zg_dev_config.config.sleep_dev_cfg.rejoin_config.auto_rejoin_send_data = AUTO_REJOIN_POLL;
    dev_register_zg_dev_config(&st_zg_dev_config);
   
    // join parameter registe
    join_config_t st_join_config;
    memset(&st_join_config, 0, sizeof(st_join_config));
    st_join_config.auto_join_power_on_flag = POWER_ON_JOIN;
    st_join_config.auto_join_remote_leave_flag = RM_LEAVE_JOIN;
    st_join_config.join_timeout = JOIN_TIME_OUT_MS;
    dev_zg_join_config(&st_join_config);
    tuya_print( "zig dev init ok....\r\n");
}


/**@brief Set sensor gpio output config
 */
extern void gpio_sensor_alarm1_interrupt_handler(GPIO_PORT_T port, GPIO_PIN_T pin);
void oem_sensor_alarm_gpio_init(sensor_t *info_t)
{
    info_t->gpio_polarity = ALARM1_GPIO_POLARITY;
    info_t->sensor_func = gpio_sensor_alarm1_interrupt_handler;
    gpio_int_register( &(info_t->sensor_io_st), info_t->sensor_func );

    info_t->init_status = gpio_raw_input_read_status( info_t->sensor_io_st.port, 
                                                      info_t->sensor_io_st.pin );
    tuya_print("init st %d\r\n",info_t->init_status);
}

/**
 * @description: ias zone sensor init
 * @param {type} none
 * @return: none
 */
void ias_sensor_init(void)
{
    ias_zone_status_init();
    ias_zone_info_init_callback( IAS_ZONE_EP, IAS_ZONE_TYPE );

    memcpy( &alarm_info.sensor_io_st, &gpio_sensor_config, sizeof(gpio_config_t) );

    oem_sensor_alarm_gpio_init( &alarm_info );
    sensor_basic_alarm_init_io_status_updata();
}

/**
 * @description: device peripherals init
 * @param {type} none
 * @return: none
 */
void per_dev_init(void)
{
    ias_sensor_init();
    gpio_output_init( (gpio_config_t *)gpio_output_config, get_array_len(gpio_output_config) );
    gpio_button_init( (gpio_config_t *)gpio_key_config, get_array_len(gpio_key_config), 50, keys_evt_handler ); 
    tuya_print( "per init ok....\r\n");
}

/**
 * @description: power sampler init
 * @param {type} none
 * @return: none
 */
void power_sampler_init(void)
{
    adc_cfg_t adc0={
        ADC_TYPE_VDD,
        NULL,
        NULL,
    };
    
    battery_cfg_t battery_cfg = {
        5*60*1000UL,
        5000,
        BATTERY_REPORT_MINUTE*60*1000UL,
        BATTERY_MAX_MILLIV,
        BATTERY_MIN_MILLIV,
        adc0,
    };

    battery_table_t battery_table[] = {
        BATTERY_DEFAULT_TABLE
    };

    hal_battery_config( &battery_cfg, battery_table, get_array_len(battery_table) );

    battery_report_policy_t bat_policy;
    bat_policy.type = BATTERY_TYPE_DRY_BATTERY;
    bat_policy.level = DEV_BUSY_LEVEL_IDLE;
    bat_policy.report_no_limits_first = TRUE;
    bat_policy.limits = BATTERY_REPORT_DECREASE_LIMITS_20;
    hal_battery_report_policy_config( &bat_policy );
}
