
#ifndef ZIGBEE_SDK__H
#define ZIGBEE_SDK__H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"

#include "type_def.h"
#include "zigbee_cmd.h"
#include "zigbee_attr.h"
#include "mf_test_callback.h"
#include "zigbee_modules.h"
#include "hal_battery.h"


#if defined (__IAR_SYSTEMS_ICC__) //for IAR
    #define VIRTUAL_FUNC __weak
#else //for gcc
    #define VIRTUAL_FUNC __attribute__((weak))
#endif

#define NEXT_REJOIN_TIME            0 
#define WAKE_UP_TIME_AFTER_JOIN    30
#define POLL_INTERVAL              250
#define WAIT_APP_ACK_TIME          1000
#define POWER_ON_AUTO_REJOIN_FLAG  1
#define POLL_FOREVER_FLAG          1
#define REJOIN_TRY_TIMES           3

//Zigbee dev define
typedef enum {
    ZG_ROUTER = 0,
    ZG_END_DEVICE,
    ZG_SLEEPY_END_DEVICE,
}ZG_DEV_T;

typedef struct {
    uint32_t next_rejoin_time;          //next rejoin time when rejoin failed. (bet:ms)
    uint32_t wake_up_time_after_join;   //wakeup time when join success.(for gateway get cluser data) (bet:ms)
    uint32_t wake_up_time_after_rejoin; //wakeup time when rejoin success.(for app use) (bet:ms)
    uint8_t  rejoin_try_times;          //rejoin times when parent lost (bet:ms)
    bool_t   power_on_auto_rejoin_flag; //auto rejoin when power on
    bool_t   auto_rejoin_send_data;     //auto rejoin when send data if network status is parent-lost.
}zg_rejoin_config_t;

typedef struct {
    uint16_t poll_interval;     //poll period (bet: ms)
    uint16_t wait_app_ack_time; //tiemout waitting app ack (bet: ms)
    bool_t   poll_forever_flag; //TRUE: forever poll 
    uint16_t  poll_failed_times; //enter parent-lost status when poll failed x times.
}zg_poll_config_t;

typedef struct {
    zg_poll_config_t    poll_conifg;
    zg_rejoin_config_t  rejoin_config;
}zg_sleep_end_device_config;

typedef struct {
    zg_poll_config_t    poll_conifg;
    zg_rejoin_config_t  rejoin_config;
}zg_end_device_config;

typedef struct {
    uint8_t reserved;
}zg_router_config;

typedef enum {
    ZG_SCAN_POLICY_CURR_CHANNEL_FOREVER = 0, //scan current channel forever when rejoin-scanning.
    ZG_SCAN_POLICY_ALL_CHANNEL_ONCE         //scan all channel at the last time when rejoin-scanning.
}ZG_SCAN_POLICY_T;

typedef enum {
    ZB_SCAN_DURATION_0 = 0, //  19.2 ms
    ZB_SCAN_DURATION_1,     //  38.4 ms
    ZB_SCAN_DURATION_2,     //  76.8 ms
    ZB_SCAN_DURATION_3,     //  153.6 ms
    ZB_SCAN_DURATION_4,     //  307.2 ms
    ZB_SCAN_DURATION_5,     //  614.4 ms
    ZB_SCAN_DURATION_6,     //  1.23 sec
    ZB_SCAN_DURATION_7,     //  2.46 sec
    ZB_SCAN_DURATION_8,     //  4.92 sec
    ZB_SCAN_DURATION_9,     //  9.83 sec
    ZB_SCAN_DURATION_10,    //  19.66 sec
    ZB_SCAN_DURATION_11,    //  39.32 sec
    ZB_SCAN_DURATION_12,    //  78.64 sec
    ZB_SCAN_DURATION_13,    //  157.28 sec
    ZB_SCAN_DURATION_14,    //  314.57 sec
}ZB_SCAN_DURATION_T;

typedef struct {
    ZG_DEV_T dev_type;
    union{
        zg_sleep_end_device_config  sleep_dev_cfg;
        zg_end_device_config        end_dev_cfg;
        zg_router_config            router_cfg;
    }config;
    uint16_t            beacon_send_interval_for_join;
    uint16_t            beacon_send_interval_for_rejoin;
    ZB_SCAN_DURATION_T  zb_scan_duration;
}zg_dev_config_t;

//Zigbee profile ID
typedef enum {
    ZHA_PROFILE_ID = 0x0104, //home automation
    ZGP_PROFILE_ID = 0xA1E0, //green power
    ZHC_PROFILE_ID = 0x0108, //personal,home and hospital care
    ZSE_PROFILE_ID = 0x0109, //Smart energy
    ZCBA_PROFILE_ID = 0x0105,//Commerical building automation
    ZIPM_PROFILE_ID = 0x0101,//Industial plant monitoring
    ZTA_PROFILE_ID = 0x0107, //Telecom application
    ZAMI_PROFILE_ID = 0x0109,//Advanced metering initiative
    ZLL_PROFILE_ID = 0xC05E, //ZLL profile
}PROFILE_ID_T;

//cluster ID
typedef enum {
    CLUSTER_BASIC_CLUSTER_ID = 0x0000,
    CLUSTER_POWER_CONFIG_CLUSTER_ID = 0x0001,
    CLUSTER_DEVICE_TEMP_CLUSTER_ID = 0x0002,
    CLUSTER_IDENTIFY_CLUSTER_ID = 0x0003,
    CLUSTER_GROUPS_CLUSTER_ID = 0x0004,
    CLUSTER_SCENES_CLUSTER_ID = 0x0005,
    CLUSTER_ON_OFF_CLUSTER_ID = 0x0006,
    CLUSTER_ON_OFF_SWITCH_CONFIG_CLUSTER_ID = 0x0007,
    CLUSTER_LEVEL_CONTROL_CLUSTER_ID = 0x0008,
    CLUSTER_ALARM_CLUSTER_ID = 0x0009,
    CLUSTER_TIME_CLUSTER_ID = 0x000A,
    CLUSTER_RSSI_LOCATION_CLUSTER_ID = 0x000B,
    CLUSTER_BINARY_INPUT_BASIC_CLUSTER_ID = 0x000F,
    CLUSTER_COMMISSIONING_CLUSTER_ID = 0x0015,
    CLUSTER_PARTITION_CLUSTER_ID = 0x0016,
    CLUSTER_OTA_BOOTLOAD_CLUSTER_ID = 0x0019,
    CLUSTER_POWER_PROFILE_CLUSTER_ID = 0x001A,
    CLUSTER_APPLIANCE_CONTROL_CLUSTER_ID = 0x001B,
    CLUSTER_POLL_CONTROL_CLUSTER_ID = 0x0020,
    CLUSTER_GREEN_POWER_CLUSTER_ID = 0x0021,
    CLUSTER_KEEPALIVE_CLUSTER_ID = 0x0025,
    CLUSTER_SHADE_CONFIG_CLUSTER_ID = 0x0100,
    CLUSTER_DOOR_LOCK_CLUSTER_ID = 0x0101,
    CLUSTER_WINDOW_COVERING_CLUSTER_ID = 0x0102,
    CLUSTER_BARRIER_CONTROL_CLUSTER_ID = 0x0103,
    CLUSTER_PUMP_CONFIG_CONTROL_CLUSTER_ID = 0x0200,
    CLUSTER_THERMOSTAT_CLUSTER_ID = 0x0201,
    CLUSTER_FAN_CONTROL_CLUSTER_ID = 0x0202,
    CLUSTER_DEHUMID_CONTROL_CLUSTER_ID = 0x0203,
    CLUSTER_THERMOSTAT_UI_CONFIG_CLUSTER_ID = 0x0204,
    CLUSTER_COLOR_CONTROL_CLUSTER_ID = 0x0300,
    CLUSTER_BALLAST_CONFIGURATION_CLUSTER_ID = 0x0301,
    CLUSTER_ILLUM_MEASUREMENT_CLUSTER_ID = 0x0400,
    CLUSTER_ILLUM_LEVEL_SENSING_CLUSTER_ID = 0x0401,
    CLUSTER_TEMP_MEASUREMENT_CLUSTER_ID = 0x0402,
    CLUSTER_PRESSURE_MEASUREMENT_CLUSTER_ID = 0x0403,
    CLUSTER_FLOW_MEASUREMENT_CLUSTER_ID = 0x0404,
    CLUSTER_RELATIVE_HUMIDITY_MEASUREMENT_CLUSTER_ID = 0x0405,
    CLUSTER_OCCUPANCY_SENSING_CLUSTER_ID = 0x0406,
    CLUSTER_CARBON_MONOXIDE_CONCENTRATION_MEASUREMENT_CLUSTER_ID = 0x040C,
    CLUSTER_CARBON_DIOXIDE_CONCENTRATION_MEASUREMENT_CLUSTER_ID = 0x040D,
    CLUSTER_ETHYLENE_CONCENTRATION_MEASUREMENT_CLUSTER_ID = 0x040E,
    CLUSTER_ETHYLENE_OXIDE_CONCENTRATION_MEASUREMENT_CLUSTER_ID = 0x040F,
    CLUSTER_HYDROGEN_CONCENTRATION_MEASUREMENT_CLUSTER_ID = 0x0410,
    CLUSTER_HYDROGEN_SULPHIDE_CONCENTRATION_MEASUREMENT_CLUSTER_ID = 0x0411,
    CLUSTER_NITRIC_OXIDE_CONCENTRATION_MEASUREMENT_CLUSTER_ID = 0x0412,
    CLUSTER_NITROGEN_DIOXIDE_CONCENTRATION_MEASUREMENT_CLUSTER_ID = 0x0413,
    CLUSTER_OXYGEN_CONCENTRATION_MEASUREMENT_CLUSTER_ID = 0x0414,
    CLUSTER_OZONE_CONCENTRATION_MEASUREMENT_CLUSTER_ID = 0x0415,
    CLUSTER_SULFUR_DIOXIDE_CONCENTRATION_MEASUREMENT_CLUSTER_ID = 0x0416,
    CLUSTER_DISSOLVED_OXYGEN_CONCENTRATION_MEASUREMENT_CLUSTER_ID = 0x0417,
    CLUSTER_BROMATE_CONCENTRATION_MEASUREMENT_CLUSTER_ID = 0x0418,
    CLUSTER_CHLORAMINES_CONCENTRATION_MEASUREMENT_CLUSTER_ID = 0x0419,
    CLUSTER_CHLORINE_CONCENTRATION_MEASUREMENT_CLUSTER_ID = 0x041A,
    CLUSTER_FECAL_COLIFORM_AND_E_COLI_CONCENTRATION_MEASUREMENT_CLUSTER_ID = 0x041B,
    CLUSTER_FLUORIDE_CONCENTRATION_MEASUREMENT_CLUSTER_ID = 0x041C,
    CLUSTER_HALOACETIC_ACIDS_CONCENTRATION_MEASUREMENT_CLUSTER_ID = 0x041D,
    CLUSTER_TOTAL_TRIHALOMETHANES_CONCENTRATION_MEASUREMENT_CLUSTER_ID = 0x041E,
    CLUSTER_TOTAL_COLIFORM_BACTERIA_CONCENTRATION_MEASUREMENT_CLUSTER_ID = 0x041F,
    CLUSTER_TURBIDITY_CONCENTRATION_MEASUREMENT_CLUSTER_ID = 0x0420,
    CLUSTER_COPPER_CONCENTRATION_MEASUREMENT_CLUSTER_ID = 0x0421,
    CLUSTER_LEAD_CONCENTRATION_MEASUREMENT_CLUSTER_ID = 0x0422,
    CLUSTER_MANGANESE_CONCENTRATION_MEASUREMENT_CLUSTER_ID = 0x0423,
    CLUSTER_SULFATE_CONCENTRATION_MEASUREMENT_CLUSTER_ID = 0x0424,
    CLUSTER_BROMODICHLOROMETHANE_CONCENTRATION_MEASUREMENT_CLUSTER_ID = 0x0425,
    CLUSTER_BROMOFORM_CONCENTRATION_MEASUREMENT_CLUSTER_ID = 0x0426,
    CLUSTER_CHLORODIBROMOMETHANE_CONCENTRATION_MEASUREMENT_CLUSTER_ID = 0x0427,
    CLUSTER_CHLOROFORM_CONCENTRATION_MEASUREMENT_CLUSTER_ID = 0x0428,
    CLUSTER_SODIUM_CONCENTRATION_MEASUREMENT_CLUSTER_ID = 0x0429,
    CLUSTER_IAS_ZONE_CLUSTER_ID = 0x0500,
    CLUSTER_IAS_ACE_CLUSTER_ID = 0x0501,
    CLUSTER_IAS_WD_CLUSTER_ID = 0x0502,
    CLUSTER_GENERIC_TUNNEL_CLUSTER_ID = 0x0600,
    CLUSTER_BACNET_PROTOCOL_TUNNEL_CLUSTER_ID = 0x0601,
    CLUSTER_11073_PROTOCOL_TUNNEL_CLUSTER_ID = 0x0614,
    CLUSTER_ISO7816_PROTOCOL_TUNNEL_CLUSTER_ID = 0x0615,
    CLUSTER_PRICE_CLUSTER_ID = 0x0700,
    CLUSTER_DEMAND_RESPONSE_LOAD_CONTROL_CLUSTER_ID = 0x0701,
    CLUSTER_SIMPLE_METERING_CLUSTER_ID = 0x0702,
    CLUSTER_MESSAGING_CLUSTER_ID = 0x0703,
    CLUSTER_TUNNELING_CLUSTER_ID = 0x0704,
    CLUSTER_PREPAYMENT_CLUSTER_ID = 0x0705,
    CLUSTER_ENERGY_MANAGEMENT_CLUSTER_ID = 0x0706,
    CLUSTER_CALENDAR_CLUSTER_ID = 0x0707,
    CLUSTER_DEVICE_MANAGEMENT_CLUSTER_ID = 0x0708,
    CLUSTER_EVENTS_CLUSTER_ID = 0x0709,
    CLUSTER_MDU_PAIRING_CLUSTER_ID = 0x070A,
    CLUSTER_SUB_GHZ_CLUSTER_ID = 0x070B,
    CLUSTER_KEY_ESTABLISHMENT_CLUSTER_ID = 0x0800,
    CLUSTER_INFORMATION_CLUSTER_ID = 0x0900,
    CLUSTER_DATA_SHARING_CLUSTER_ID = 0x0901,
    CLUSTER_GAMING_CLUSTER_ID = 0x0902,
    CLUSTER_DATA_RATE_CONTROL_CLUSTER_ID = 0x0903,
    CLUSTER_VOICE_OVER_ZIGBEE_CLUSTER_ID = 0x0904,
    CLUSTER_CHATTING_CLUSTER_ID = 0x0905,
    CLUSTER_PAYMENT_CLUSTER_ID = 0x0A01,
    CLUSTER_BILLING_CLUSTER_ID = 0x0A02,
    CLUSTER_APPLIANCE_IDENTIFICATION_CLUSTER_ID = 0x0B00,
    CLUSTER_METER_IDENTIFICATION_CLUSTER_ID = 0x0B01,
    CLUSTER_APPLIANCE_EVENTS_AND_ALERT_CLUSTER_ID = 0x0B02,
    CLUSTER_APPLIANCE_STATISTICS_CLUSTER_ID = 0x0B03,
    CLUSTER_ELECTRICAL_MEASUREMENT_CLUSTER_ID = 0x0B04,
    CLUSTER_DIAGNOSTICS_CLUSTER_ID = 0x0B05,
    CLUSTER_ZLL_COMMISSIONING_CLUSTER_ID = 0x1000,
    CLUSTER_SAMPLE_MFG_SPECIFIC_CLUSTER_ID = 0xFC00,
    CLUSTER_OTA_CONFIGURATION_CLUSTER_ID = 0xFC01,
    CLUSTER_MFGLIB_CLUSTER_ID = 0xFC02,
    CLUSTER_SL_WWAH_CLUSTER_ID = 0xFC57,

    CLUSTER_PRIVATE_TUYA_CLUSTER_ID = 0xEF00,
}CLUSTER_ID_T;

// ZigBee device IDs
typedef enum {
    ZG_DEVICE_ID_ON_OFF_SWITCH = 0x0000,
    ZG_DEVICE_ID_LEVEL_CONTROL_SWITCH = 0x0001,
    ZG_DEVICE_ID_ON_OFF_OUTPUT = 0x0002,
    ZG_DEVICE_ID_LEVEL_CONTROL_OUTPUT = 0x0003,
    ZG_DEVICE_ID_SCENE_SELECTOR = 0x0004,
    ZG_DEVICE_ID_CONFIG_TOOL = 0x0005,
    ZG_DEVICE_ID_REMOTE_CONTROL = 0x0006,
    ZG_DEVICE_ID_COMBINED_INTERFACE = 0x0007,
    ZG_DEVICE_ID_RANGE_EXTENDER = 0x0008,
    ZG_DEVICE_ID_MAINS_POWER_OUTLET = 0x0009,
    ZG_DEVICE_ID_DOOR_LOCK = 0x000a,
    ZG_DEVICE_ID_DOOR_LOCK_CONTROLLER = 0x000b,
    ZG_DEVICE_ID_SIMPLE_SENSOR = 0x000c,
    ZG_DEVICE_ID_CONSUMPTION_AWARENESS_DEVICE = 0x000d,

    ZG_DEVICE_ID_HOME_GATEWAY = 0x0050,
    ZG_DEVICE_ID_SMART_PLUG = 0x0051,
    ZG_DEVICE_ID_WHITE_GOODS = 0x0052,
    ZG_DEVICE_ID_METER_INTERFACE = 0x0053,

    ZG_DEVICE_ID_ON_OFF_LIGHT = 0x0100,
    ZG_DEVICE_ID_DIMMABLE_LIGHT = 0x0101,
    ZG_DEVICE_ID_COLOR_DIMMABLE_LIGHT = 0x0102,
    ZG_DEVICE_ID_ON_OFF_LIGHT_SWITCH = 0x0103,
    ZG_DEVICE_ID_DIMMER_SWITCH = 0x0104,
    ZG_DEVICE_ID_COLOR_DIMMER_SWITCH = 0x0105,
    ZG_DEVICE_ID_LIGHT_SENSOR = 0x0106,
    ZG_DEVICE_ID_OCCUPANCY_SENSOR = 0x0107,

    ZG_DEVICE_ID_SHADE = 0x0200,
    ZG_DEVICE_ID_SHADE_CONTROLLER = 0x0201,
    ZG_DEVICE_ID_WINDOW_COVERING_DEVICE = 0x0202,
    ZG_DEVICE_ID_WINDOW_COVERING_CONTROLLER = 0x0203,

    ZG_DEVICE_ID_HEATING_COOLING_UNIT = 0x0300,
    ZG_DEVICE_ID_THERMOSTAT = 0x0301,
    ZG_DEVICE_ID_TEMPERATURE_SENSOR = 0x0302,
    ZG_DEVICE_ID_PUMP = 0x0303,
    ZG_DEVICE_ID_PUMP_CONTROLLER = 0x0304,
    ZG_DEVICE_ID_PRESSURE_SENSOR = 0x0305,
    ZG_DEVICE_ID_FLOW_SENSOR = 0x0306,
    ZG_DEVICE_ID_MINI_SPLIT_AC = 0x0307,

    ZG_DEVICE_ID_IAS_CIE = 0x0400,
    ZG_DEVICE_ID_IAS_ANCILLARY_CONTROL = 0x0401,
    ZG_DEVICE_ID_IAS_ZONE = 0x0402,
    ZG_DEVICE_ID_IAS_WARNING = 0x0403,
}DEV_ID_T;
    
// ZCL attribute types
typedef enum {
    ATTR_NO_DATA_ATTRIBUTE_TYPE                        = 0x00, // No data
    ATTR_DATA8_ATTRIBUTE_TYPE                          = 0x08, // 8-bit data
    ATTR_DATA16_ATTRIBUTE_TYPE                         = 0x09, // 16-bit data
    ATTR_DATA24_ATTRIBUTE_TYPE                         = 0x0A, // 24-bit data
    ATTR_DATA32_ATTRIBUTE_TYPE                         = 0x0B, // 32-bit data
    ATTR_DATA40_ATTRIBUTE_TYPE                         = 0x0C, // 40-bit data
    ATTR_DATA48_ATTRIBUTE_TYPE                         = 0x0D, // 48-bit data
    ATTR_DATA56_ATTRIBUTE_TYPE                         = 0x0E, // 56-bit data
    ATTR_DATA64_ATTRIBUTE_TYPE                         = 0x0F, // 64-bit data
    ATTR_BOOLEAN_ATTRIBUTE_TYPE                        = 0x10, // Boolean
    ATTR_BITMAP8_ATTRIBUTE_TYPE                        = 0x18, // 8-bit bitmap
    ATTR_BITMAP16_ATTRIBUTE_TYPE                       = 0x19, // 16-bit bitmap
    ATTR_BITMAP24_ATTRIBUTE_TYPE                       = 0x1A, // 24-bit bitmap
    ATTR_BITMAP32_ATTRIBUTE_TYPE                       = 0x1B, // 32-bit bitmap
    ATTR_BITMAP40_ATTRIBUTE_TYPE                       = 0x1C, // 40-bit bitmap
    ATTR_BITMAP48_ATTRIBUTE_TYPE                       = 0x1D, // 48-bit bitmap
    ATTR_BITMAP56_ATTRIBUTE_TYPE                       = 0x1E, // 56-bit bitmap
    ATTR_BITMAP64_ATTRIBUTE_TYPE                       = 0x1F, // 64-bit bitmap
    ATTR_INT8U_ATTRIBUTE_TYPE                          = 0x20, // Unsigned 8-bit integer
    ATTR_INT16U_ATTRIBUTE_TYPE                         = 0x21, // Unsigned 16-bit integer
    ATTR_INT24U_ATTRIBUTE_TYPE                         = 0x22, // Unsigned 24-bit integer
    ATTR_INT32U_ATTRIBUTE_TYPE                         = 0x23, // Unsigned 32-bit integer
    ATTR_INT40U_ATTRIBUTE_TYPE                         = 0x24, // Unsigned 40-bit integer
    ATTR_INT48U_ATTRIBUTE_TYPE                         = 0x25, // Unsigned 48-bit integer
    ATTR_INT56U_ATTRIBUTE_TYPE                         = 0x26, // Unsigned 56-bit integer
    ATTR_INT64U_ATTRIBUTE_TYPE                         = 0x27, // Unsigned 64-bit integer
    ATTR_INT8S_ATTRIBUTE_TYPE                          = 0x28, // Signed 8-bit integer
    ATTR_INT16S_ATTRIBUTE_TYPE                         = 0x29, // Signed 16-bit integer
    ATTR_INT24S_ATTRIBUTE_TYPE                         = 0x2A, // Signed 24-bit integer
    ATTR_INT32S_ATTRIBUTE_TYPE                         = 0x2B, // Signed 32-bit integer
    ATTR_INT40S_ATTRIBUTE_TYPE                         = 0x2C, // Signed 40-bit integer
    ATTR_INT48S_ATTRIBUTE_TYPE                         = 0x2D, // Signed 48-bit integer
    ATTR_INT56S_ATTRIBUTE_TYPE                         = 0x2E, // Signed 56-bit integer
    ATTR_INT64S_ATTRIBUTE_TYPE                         = 0x2F, // Signed 64-bit integer
    ATTR_ENUM8_ATTRIBUTE_TYPE                          = 0x30, // 8-bit enumeration
    ATTR_ENUM16_ATTRIBUTE_TYPE                         = 0x31, // 16-bit enumeration
    ATTR_FLOAT_SEMI_ATTRIBUTE_TYPE                     = 0x38, // Semi-precision
    ATTR_FLOAT_SINGLE_ATTRIBUTE_TYPE                   = 0x39, // Single precision
    ATTR_FLOAT_DOUBLE_ATTRIBUTE_TYPE                   = 0x3A, // Double precision
    ATTR_OCTET_STRING_ATTRIBUTE_TYPE                   = 0x41, // Octet string
    ATTR_CHAR_STRING_ATTRIBUTE_TYPE                    = 0x42, // Character string
    ATTR_LONG_OCTET_STRING_ATTRIBUTE_TYPE              = 0x43, // Long octet string
    ATTR_LONG_CHAR_STRING_ATTRIBUTE_TYPE               = 0x44, // Long character string
    ATTR_ARRAY_ATTRIBUTE_TYPE                          = 0x48, // Array
    ATTR_STRUCT_ATTRIBUTE_TYPE                         = 0x4C, // Structure
    ATTR_SET_ATTRIBUTE_TYPE                            = 0x50, // Set
    ATTR_BAG_ATTRIBUTE_TYPE                            = 0x51, // Bag
    ATTR_TIME_OF_DAY_ATTRIBUTE_TYPE                    = 0xE0, // Time of day
    ATTR_DATE_ATTRIBUTE_TYPE                           = 0xE1, // Date
    ATTR_UTC_TIME_ATTRIBUTE_TYPE                       = 0xE2, // UTC Time
    ATTR_CLUSTER_ID_ATTRIBUTE_TYPE                     = 0xE8, // Cluster ID
    ATTR_ATTRIBUTE_ID_ATTRIBUTE_TYPE                   = 0xE9, // Attribute ID
    ATTR_BACNET_OID_ATTRIBUTE_TYPE                     = 0xEA, // BACnet OID
    ATTR_IEEE_ADDRESS_ATTRIBUTE_TYPE                   = 0xF0, // IEEE address
    ATTR_SECURITY_KEY_ATTRIBUTE_TYPE                   = 0xF1, // 128-bit security key
    ATTR_UNKNOWN_ATTRIBUTE_TYPE                        = 0xFF // Unknown
} ATTR_TYPE_T;

//attr_t->attr_mask
// Attribute masks modify how attributes are used by the framework
// Attribute that has this mask is NOT read-only
#define ATTR_MASK_WRITABLE (0x01)
// Attribute that has this mask is saved to a token
#define ATTR_MASK_TOKENIZE (0x02)
// Attribute that has this mask has a min/max values
#define ATTR_MASK_MIN_MAX (0x04)
// Manufacturer specific attribute
#define ATTR_MASK_MANUFACTURER_SPECIFIC (0x08)
// Attribute deferred to external storage
#define ATTR_MASK_EXTERNAL_STORAGE (0x10)
// Attribute is singleton
#define ATTR_MASK_SINGLETON (0x20)
// Attribute is a client attribute
#define ATTR_MASK_CLIENT (0x40)
// Attribute that has this mask is saved to a token, size limited: 1,2,4,6, fastest token
#define ATTR_MASK_TOKEN_FAST (0x80)

typedef enum {
    NET_IDLE,
    NET_POWER_ON_LEAVE,     //power on and device is not joined network
    NET_POWER_ON_ONLINE,    //power on and device is already joined network
    NET_JOIN_START,         //start joining network
    NET_JOIN_TIMEOUT,       //network joining timeout
    NET_JOIN_OK,            //network joined success
    NET_LOST,               //network lost
    NET_REJOIN_OK,          //network rejoin ok
    NET_REMOTE_LEAVE,       //remove device by remote device
    NET_LOCAL_LEAVE,        //remove device by local
    NET_MF_TEST_LEAVE,      //remove device by PC test tools
    NET_ZLL_JOINED,         //network joined zll network
    NET_ZLL_LEAVE,          //remove device Zll Reset To Factory New
}NET_EVT_T;

typedef struct {
    bool_t auto_join_power_on_flag;  //上电是否配网
    bool_t auto_join_remote_leave_flag; //从app删除设备后是否自动配网
    uint32_t join_timeout; //join max time. bet:ms
}join_config_t;

typedef struct {
    uint16_t attr_id;
    ATTR_TYPE_T type;
    uint8_t attr_value_size;
    uint8_t attr_mask;
    void *default_value;
}attr_t;

typedef struct {
    CLUSTER_ID_T cluster_id;
    attr_t *attr_list;
    uint8_t  attr_sums;
}cluster_t;

// Simple Description Format Structure
typedef struct {
  uint8_t  ep;
  PROFILE_ID_T profile_id;
  DEV_ID_T     dev_id;
  uint8_t         server_cluster_sums;
  cluster_t *server_cluster_list; //input cluster
  uint8_t         client_cluster_sums;
  cluster_t *client_cluster_list; //output cluster
} dev_description_t;

typedef struct {
    uint8_t original_ep;
    uint8_t alias_ep1;
    uint8_t alias_ep2;
    uint8_t alias_ep3;
}dev_ep_alias_t;

typedef enum {
    SEND_ST_OK,
    SEND_ST_ERR,
    SEND_ST_MEMORY_ERR,
    SEND_ST_QUEUE_FULL_ERR,
    SEND_ST_NO_NETWORK,
    SEND_ST_ADDR_MODE_ERR,
}SEND_ST_T;

typedef enum {
    MSG_SRC_GW_UNICAST,
    MSG_SRC_GW_BROADCAST,
    MSG_SRC_DEV_UNICAST,
    MSG_SRC_DEV_BROADCAST,
    MSG_SRC_MULTICAST
}MSG_SRC_T;

typedef struct {
    uint16_t attr;
    uint8_t  cmd;
    uint8_t value_len;
    uint8_t *value;
}attr_value_t;

typedef struct {
    uint8_t attr_value_sums;
    attr_value_t *attr_value;
}attr_data_t;

typedef struct {
    uint8_t commandId;
    uint8_t len;
    void *data;
}bare_data_t;

typedef struct {
    MSG_SRC_T msg_src;
    CLUSTER_ID_T cluster;
    uint8_t zcl_id;
    uint8_t endpoint;
    uint16_t src_addr;
    union {
        attr_data_t attr_data;
        bare_data_t bare_data;
    }data;
}dev_msg_t;

typedef enum {
    DEV_IO_OFF = 0,
    DEV_IO_ON,
} DEV_IO_ST_T;

typedef enum {
    KEY_ST_UP = 0,
    KEY_ST_PUSH,
} key_st_t;
 
typedef enum {
    RELAY_ST_OFF = 0,
    RELAY_ST_ON
}RELAY_ST_T;

typedef enum {
    MF_TEST_LED_ON_ALL,
    MF_TEST_LED_OFF_ALL,
    MF_TEST_LED_BLINK_ALL,
    MF_TEST_RELAY_ON_ALL,
    MF_TEST_RELAY_OFF_ALL,
    MF_TEST_RELAY_BLINK_ALL,
    MF_TEST_BUTTON,
    MF_TEST_GET_ELEC_INFO,
    MF_TEST_ADJUST_ELEC_INFO,
    MF_TRANSFER, //transfer rare test data
}MF_TEST_CMD_T;

typedef enum {
    DEV_EVT_1  = 35,
    DEV_EVT_2,
    DEV_EVT_3,
    DEV_EVT_4,
    DEV_EVT_5,
    DEV_EVT_6,
    DEV_EVT_7,
    DEV_EVT_8,
    DEV_EVT_9,
    DEV_EVT_10,
    DEV_EVT_11,
    DEV_EVT_12,
    DEV_EVT_13,
    DEV_EVT_14,
    DEV_EVT_15,
    DEV_EVT_16,
}DEV_EVT_T;
    
typedef enum {
    PORT_A = 0x00,
    PORT_B,
    PORT_C,
    PORT_D,
    PORT_E,
    PORT_F,
    PORT_H,
    PORT_I,
    PORT_J,
    PORT_K
}GPIO_PORT_T;

typedef enum {
    PIN_0 = 0x00,
    PIN_1,
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

typedef enum {
    LOC_0 = 0,
    LOC_1,
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

//GPIO_Mode_TypeDef
typedef enum {
    GPIO_MODE_INPUT_HIGH_IMPEDANCE = 0,
    GPIO_MODE_INPUT_PULL,
    GPIO_MODE_OUTPUT_PP,
    GPIO_MODE_OUTPUT_OD,
    GPIO_MODE_OUTPUT_OD_PULL_UP,
    GPIO_MODE_OUTPUT_OD_PULL_DOWN,
}GPIO_MODE_T;

typedef enum {
    GPIO_DOUT_LOW = 0,
    GPIO_DOUT_HIGH = 1,
}GPIO_DOUT_T;

typedef enum {
    GPIO_LEVEL_LOW = 0,
    GPIO_LEVEL_HIGH,
    GPIO_LEVEL_ALL
}GPIO_LEVEL_T;

typedef struct {
    GPIO_PORT_T port;
    GPIO_PIN_T pin;
    GPIO_MODE_T mode;
    GPIO_DOUT_T out;
    GPIO_LEVEL_T drive_flag; //the effectively level (0 or 1)
} gpio_config_t;

typedef enum {
    UART_ID_UART0 = 0,
    UART_ID_UART1
}UART_ID_T;

//USART_Parity_TypeDef
typedef enum {
    USART_PARITY_NONE = 0,    /**< No parity. */
    USART_PARITY_EVEN,      /**< Even parity. */
    USART_PARITY_ODD,      /**< Odd parity. */
} USART_PARITY_T;

//USART_Stopbits_TypeDef
typedef enum {
    USART_STOPBITS_HALF = 0,        /**< 0.5 stop bits. */
    USART_STOPBITS_ONE,         /**< 1 stop bits. */
    USART_STOPBITS_ONEANDAHALF, /**< 1.5 stop bits. */
    USART_STOPBITS_TWO          /**< 2 stop bits. */
} USART_STOPBITS_T;

//USART_DATABITS_T
typedef enum {
    USART_DATABITS_8BIT = 0,        /**< 8 bits databits. */
    USART_DATABITS_9BIT,         /**< 9 bits databits. */
} USART_DATABITS_T;

typedef enum {
    UART_PIN_TYPE_DEFAULT = 0,//default uart is PA0,PA1
    UART_PIN_TYPE_CONFIG //user config uart port and pin 
}UART_PIN_TYPE_T;

typedef struct {
    GPIO_PORT_T port;
    GPIO_PIN_T pin;
}GPIO_PORT_PIN_T;

typedef void(*uart_callback)(uint8_t *data, uint16_t len);

typedef struct {
    UART_ID_T uart_id; 
    UART_PIN_TYPE_T pin_type; 
    GPIO_PORT_PIN_T tx;
    GPIO_PORT_PIN_T rx;
    GPIO_LOC_T tx_loc;
    GPIO_LOC_T rx_loc;
    uint32_t baud_rate;
    USART_PARITY_T parity;
    USART_STOPBITS_T stop_bits;
    USART_DATABITS_T data_bits;
    uart_callback func;
}user_uart_config_t;

#ifdef EFR32MG13P
#define USART_CONFIG_DEFAULT {\
    UART_ID_UART0,\
    UART_PIN_TYPE_CONFIG,\
    {PORT_A, PIN_0},\
    {PORT_A, PIN_1},\
    LOC_0,\
    LOC_0,\
    115200,\
    USART_PARITY_NONE,\
    USART_STOPBITS_ONE,\
    USART_DATABITS_8BIT,\
    NULL\
}
#else
#define USART_CONFIG_DEFAULT {\
    UART_ID_UART0,\
    UART_PIN_TYPE_CONFIG,\
    {PORT_A, PIN_5},\
    {PORT_A, PIN_6},\
    LOC_0,\
    LOC_0,\
    115200,\
    USART_PARITY_NONE,\
    USART_STOPBITS_ONE,\
    USART_DATABITS_8BIT,\
    NULL\
}
#endif
typedef enum
{
    APP_VERSION = 0,
    BATTERY_VOLTAGE,
}HEARTBEAT_TYPE_E;

typedef enum {
    ZCL_CMD_RET_SUCCESS,
    ZCL_CMD_RET_FAILED
}ZCL_CMD_RET_T;

typedef enum {
    ATTR_CMD_RET_SUCCESS,
    ATTR_CMD_RET_FAILED
}ATTR_CMD_RET_T;

typedef enum {
    ZG_OTA_EVT_DOWNLOAD_AND_VERIFY_SUCCESS = 0,
    ZG_OTA_EVT_DOWNLOAD_TIME_OUT           = 1,
    ZG_OTA_EVT_VERIFY_FAILED               = 2,
    ZG_OTA_EVT_SERVER_ABORTED              = 3,
    ZG_OTA_EVT_CLIENT_ABORTED              = 4,
    ZG_OTA_EVT_ERASE_FAILED                = 5,
    ZG_OTA_EVT_START                       = 6
}ZG_OTA_EVT_T;

typedef enum {
    QOS_0,     //permit loss packages
    QOS_1,     //not permit loss packages
    QOS_VIP_0, //send first, permit loss packages
    QOS_VIP_1, //send first, not permit loss packages
}SEND_QOS_T;

typedef enum {
    SEND_MODE_GW,
    SEND_MODE_GROUP,
    SEND_MODE_BINDING,
    SEND_MODE_DEV,
}SEND_MODE_T;

typedef struct {
    uint16_t group_id;
    uint8_t src_ep;
    uint8_t dest_ep;
    CLUSTER_ID_T cluster_id;
}group_addr_t;

typedef struct {
    uint8_t src_ep;
    CLUSTER_ID_T cluster_id;
}bind_addr_t;

typedef struct {
    uint8_t src_ep;
    CLUSTER_ID_T cluster_id;
}gw_addr_t;

typedef struct {
    uint8_t src_ep;
    uint8_t dest_ep;
    CLUSTER_ID_T cluster_id;
    uint16_t dest_addr;
}dev_addr_t;


typedef struct {
    SEND_MODE_T mode;
    union {
        group_addr_t group;
        bind_addr_t  bind;
        gw_addr_t    gw;
        dev_addr_t   dev;
    }type;
} af_addr_t;

typedef struct {
    uint16_t attr_id;
    ATTR_TYPE_T type;
    uint8_t value_size;
    uint8_t value[8];
}attr_key_value_t;

typedef struct {
    uint8_t attr_sum;
    attr_key_value_t attr[8];
}send_zg_t;

typedef struct {
    uint8_t len;
    uint8_t data[64];
}send_private_t;


typedef enum {
    /*ZCL profile wide command such as read, write, 
    report or global cluster such as basic, identify*/
    ZCL_COMMAND_GLOBAL_CMD = 0,

    /*zcl cluster specific command*/ 
    ZCL_COMMAND_CLUSTER_SPEC_CMD
}ZCL_COMMAND_TYPE_T;

typedef enum {
    ZCL_DATA_DIRECTION_CLIENT_TO_SERVER = 0,
    ZCL_DATA_DIRECTION_SERVER_TO_CLIENT
}ZCL_DATA_DIRECTION_T;

typedef struct {
    /*real send time delay is equal to delay_time+rand()%random_time*/
    uint16_t delay_time; //send delay time with ms
    uint16_t random_time;//send random times with ms
    
    uint8_t zcl_id; //applicaiton sequence number
    uint8_t command_id; //zcl command id, the detail in commmand-id.h
    ZCL_COMMAND_TYPE_T commmand_type;
    ZCL_DATA_DIRECTION_T direction;
    SEND_QOS_T qos;
    af_addr_t addr;
    union {
        send_zg_t zg;
        send_private_t private;
    }data;
}dev_send_data_t;

typedef struct {
    SEND_ST_T st;
    dev_send_data_t send_data;
}zck_ack_t;

typedef enum {
    ZG_CLEAR_ALL_DATA = 0,
    ZG_CLEAR_LATEST_DATA,
    ZG_CLEAR_ALL_SAME_DATA,
    ZG_CLEAR_ALL_ZCL_ID,
}ZG_CLEAR_TYPE_T;

typedef void (*send_result_func_t)(SEND_ST_T, dev_send_data_t *);

#define SCENES_MAX_DATA_LEN   128

typedef enum {
    SCENE_DATA_TYPE_SERVER_DEFINE = 0,
    SCENE_DATA_TYPE_USER_DEFINE = 1,
    SCENE_DATA_TYPE_ZIGBEE_DEFINE
}SCENE_DATA_TYPE_T;

typedef struct {
    SCENE_DATA_TYPE_T type;
    uint8_t data_len;
    uint8_t data[SCENES_MAX_DATA_LEN];
}scene_save_data_t;

typedef enum {
    LETTER_TYPE_UPPER = 0,
    LETTER_TYPE_LOWER
}LETTER_TYPE_T;

typedef enum {
    HARDWARE_TIMER_AUTO_RELOAD_ENABLE = 0,
    HARDWARE_TIMER_AUTO_RELOAD_DISABLE
}TIMER_RELOAD_FLAG_T;

typedef enum {
    V_TIMER0 = 0,
    V_TIMER1,
    V_TIMER2,
    V_TIMER3,
    V_TIMER4,
    V_TIMER5,
    V_TIMER6,
    V_TIMER7,
    V_TIMER8,
    V_TIMER9,
    V_TIMER10,
    V_TIMER11,
    V_TIMER12,
    V_TIMER13,
    V_TIMER14,
    V_TIMER15,
    V_TIMER16,
    V_TIMER17,
    V_TIMER18,
    V_TIMER19,
    V_TIMER_ERR = 0xFF
}TIMER_ID_T;

typedef void (*hardware_timer_func_t)(TIMER_ID_T);

typedef struct {
    uint32_t temp_time;
    uint32_t delay_time;
    hardware_timer_func_t func;
    TIMER_RELOAD_FLAG_T auto_reload_flag;
    uint8_t valid_flag;
    uint8_t exe_flag;
}hardware_timer_t;


typedef enum {
    ADC_TYPE_VDD = 0, //capture vdd 
    ADC_TYPE_PORT_PIN //capture GPIO
}ADC_TYPE_T;

typedef struct {
    ADC_TYPE_T adc_type;
    GPIO_PORT_PIN_T *pos_pin;
    GPIO_PORT_PIN_T *neg_pin;
}adc_cfg_t;

typedef struct {
    uint32_t cap_first_delay_time;          //delay time of first battery percentage capture
    uint32_t cap_waitting_silence_time;     //time to wait for system silence
    uint32_t cap_max_period_time;           //maximum capture period
    uint16_t cap_max_voltage;               //maximum voltage of battery.
    uint16_t cap_min_voltage;               //minimum voltage of battery.
    adc_cfg_t adc;
}battery_cfg_t;

typedef struct {
    uint16_t voltage;       //battery voltage
    uint8_t  percent;       //percentage of current battery voltage
}battery_table_t;


typedef enum {
    QUEUE_RET_OK = 0,
    QUEUE_RET_ERR,
}QUEUE_RET_T;

typedef void * dev_queue_t;

typedef void (*serialMfgRxCallback)(uint8_t *packet, uint8_t lqi, int8_t rssi);
typedef struct
{
    uint8_t channel;
    uint8_t data_len;
    uint8_t data[50];
    serialMfgRxCallback received_callback;
}serial_protocol_rf_test_t;



typedef enum{
    RESET_REASON_UNKNOWN    = 0, // Underterminable cause
    RESET_REASON_FIB        = 1, // Reset originated from the FIB bootloader
    RESET_REASON_BOOTLOADER = 2, // Reset relates to an Ember bootloader
    RESET_REASON_EXTERNAL   = 3, // External reset trigger
    RESET_REASON_POWERON    = 4, // Poweron reset type, supply voltage < power-on threshold
    RESET_REASON_WATCHDOG   = 5, // Watchdog reset occurred
    RESET_REASON_SOFTWARE   = 6, // Software triggered reset
    RESET_REASON_CRASH      = 7, // Software crash
    RESET_REASON_FLASH      = 8, // Flash failure cause reset
    RESET_REASON_FATAL      = 9, // A non-recoverable fatal error occurred
    RESET_REASON_FAULT      = 10,// A access fault occurred
    RESET_REASON_BROWNOUT   = 11,// Brown out
}RESET_REASON_T;

typedef enum
{
    ERROR_CODE_IDLE             = 0, // idle state
    /*1-15 is used by sdk*/
    ERROR_CODE_ABNORMAL_RESET   = 16, // device reset
    ERROR_CODE_PARENT_LOST      = 17, // device parent lost
    ERROR_CODE_NWK_LEAVE_SELF   = 18, // device leave network by itself
    ERROR_CODE_NWK_LEAVE_GW     = 19, // device leave network by remote command, such as gateway or it's parent
    ERROR_CODE_IO_ERROR         = 20,  // GPIO input or output error
    ERROR_CODE_MEM_ASSERT       = 21,  // memory assert error
    ERROR_CODE_ADDR_CHANGE      = 22,  // add change
    ERROR_CODE_RESET_LEAVE      = 23,  // recv ZCL_RESET_TO_FACTORY_DEFAULTS_COMMAND_ID
    ERROR_CODE_RESET_NO_LEAVE   = 24,  // recv basic cluster cmd 0xF0(private cmd)
    ERROR_CODE_WAKUP_TIMEOUT    = 25,  // a sleep device is always wakeup.
    ERROR_CODE_RESET_LEAVE_INVALID = 26, //recv ZCL_RESET_TO_FACTORY_DEFAULTS_COMMAND_ID with invalid arguments.
}DEV_ERROR_CODE_E;

typedef enum {
    DEV_RESET_TYPE_TO_FACTORY_ONLY = 0,
    DEV_RESET_TYPE_TO_FACTORY_AND_LEAVE
}DEV_RESET_TYPE_T;

#ifdef APP_DEBUG
#define app_print(...) uart_printf(UART_ID_UART0, __VA_ARGS__)
#define APP_PRINT(fmt,...) app_print("%s(%d)-<%s>: "##fmt"\r\n",__FILE__, __LINE__, __FUNCTION__,##__VA_ARGS__)
#else
#define app_print(...)
#endif

#define get_array_len(x) (sizeof(x)/sizeof(x[0]))

/**
 * @description: get reset info
 * @param {type} none
 * @return: RESET_REASON_T
 */
extern RESET_REASON_T get_reset_reason(void);

/**
 * @description: watch dog reload.
 * @param {type} none
 * @return: none
 */
extern void watchdog_reload(void);
//******************************************************************************
//                                                                              
//                              UART API                                     
//                                                                              
//******************************************************************************
/**
 * @description: user uart init function
 * @param {config} user uart configuration information
 * @return: none
 */
extern void user_uart_init(user_uart_config_t *config);

/**
 * @description: user uart data send funciton
 * @param {uart_id} UART_ID_UART0 or UART_ID_UART1
 * @param {data} send data
 * @param {data_len} send data length
 * @return: none
 */
extern void user_uart_send(UART_ID_T uart_id, uint8_t* data, uint16_t data_len);

/**
 * @description: user uart function disable function
 * @param {uart_id} UART_ID_UART0 or UART_ID_UART1
 * @return: none
 */
extern void user_uart_disable(UART_ID_T uart_id);

/**
 * @description: uart log print function
 * @param {uart_id} UART_ID_UART0 or UART_ID_UART1
 * @param {formatString} print data list
  * @return: none
 */
extern void uart_printf(UART_ID_T uart_id, const char *formatString, ...);



//******************************************************************************
//                                                                              
//                              Soft timer API                                       
//                                                                              
//******************************************************************************
typedef void (*timer_func_t)(uint8_t);

/**
 * @description: system event stop function
 * @param {evt} event id
 * @return: none
 */
extern void dev_timer_stop(uint8_t evt);

/**
 * @description: system event start with callback function
 * @param {evt} event id
 * @param {t} event run delay time(ms)
 * @param {func} event handler function
 * @return: none
 */
extern void dev_timer_start_with_callback(uint8_t evt, uint32_t t, timer_func_t func);

/**
 * @description: system event start with no callback function
 * @param {evt} event id
 * @param {t} event run delay time(ms)
 * @return: none
 */
extern void dev_timer_start(uint8_t evt, uint32_t t);

/**
 * @description: system event active flag get function
 * @param {evt} event id
 * @return: bool_t: true is active, false is inctive
 */
extern bool_t dev_timer_get_valid_flag(uint8_t evt);

/**
 * @description: The amount of milliseconds remaining before the event is
 *               scheduled to run. If the event is inactive, 0xFFFFFFFF is returned.
 * @param {evt} event id
 * @return: The amount of milliseconds remaining
 */
extern uint32_t dev_timer_get_remaining_time(uint8_t evt);

//******************************************************************************
//                                                                              
//                              base tools api                                  
//                                                                              
//******************************************************************************
/**
 * @description: hex to hexstr convert function
 * @param {type} upper or lower letter
 * @param {hex} input hex data
 * @param {hex_len} input hex data length
 * @param {hex_str} output covert hexstr result
 * @return: none
 */
extern void hex_to_hexstr(LETTER_TYPE_T type, uint8_t *hex, uint16_t hex_len, char *hex_str);

/**
 * @description: hexstr to hex convert function
 * @param {hex_str} input hexstr data
 * @param {hex} output covert hex result
 * @return: hex data length
 */
extern uint16_t hexstr_to_hex(char *hex_str, uint8_t *hex);

/**
 * @description: device firmware version get
 * @param {ver} the int version 
 * @return: convert result of string version
 */
extern char *get_dev_firmware_ver(uint8_t ver);

/**
 * @description: data reversal function
 * @param {in_data} reversal source data
 * @param {out_data} reversal dest data
 * @param {len} reversal data length
 * @return: none
 */
extern void data_reversal(uint8_t *in_data, uint8_t *out_data, uint16_t len);

//******************************************************************************
//                                                                              
//                              base info register                                 
//                                                                              
//******************************************************************************
/**
 * @description: device basic information register function
 * @param {model_id} modle id attribute of basic cluster
 * @param {pid_prefix} manufacture name attribute(0-8bytes) of basic cluster
 * @param {pid} manufacture name attribute(9-16bytes) of basic cluster
 * @return: none
 */
extern void dev_register_base_info(char *model_id, char *pid_prefix, char *pid);

/**
 * @description: zigbee endpoint information register function
 * @param {ep_desc} endpoint information description, include profileid, 
 * deviceid, in and out clusters
 * @param {ep_sums} totals endpoints
  * @return: none
 */
extern void dev_register_zg_ep_infor(dev_description_t *ep_desc, uint8_t ep_sums);


/**
 * @description: setting alias endpoint for a endpoint.
 * @param {dev_ep_alias_t} 
  * @return: TRUE for successful, FALSE for failed.
 */
extern bool_t dev_set_endpoint_alias(dev_ep_alias_t *alias_item);

/**
 * @description: zigbee endpoint enable disable
 * @param {endpint} endpoint number
 * @param {enable} TRUE: enable,  FALSE: disable
  * @return: none
 */
extern void dev_endpint_enable_disable(uint8_t endpoint, bool_t enable);

/**
 * @description: zigbee device information register funciton
 * @param {config} device configuratin, inculde device type, join  mechanism ETC
 * @return: none
 */
extern void dev_register_zg_dev_config(zg_dev_config_t *config);

/**
 * @description: network join information configuration
 * @param {*cfg} network join struct
 * @return: none
 */
extern void dev_zg_join_config(join_config_t *cfg);

/**
 * @description: zigbee device start to join network
 * @param {join_timeout} join timeout ticks, uint is ms
 * @return: true or false
 */
extern bool_t dev_zigbee_join_start(uint32_t join_timeout);

/**
 * @description: stop join, throw JOIN_START_ITMEOUT to nwk_state_changed_callback
 * @param {void}
 * @return: void
 */
extern void dev_zigbee_join_stop(void);

/**
 * @description: enable permit join after steering finish. 
 * @param {type} none
 * @return: none
 */
extern void dev_zg_enable_steering_join_permit(void);

/**
 * @description: set all attributes value to default.
 * @param {typet} none
 * @return: none
 */
extern void dev_attr_recovery(void);



//report table config

typedef enum {
    ZCL_REPORTING_DIRECTION_REPORTED = 0x00,
    ZCL_REPORTING_DIRECTION_RECEIVED = 0x01,
} ZCL_REPORTING_DIRECTION_T;


typedef enum {
    ZCL_CLUSTER_MASK_SERVER = 0x40,
    ZCL_CLUSTER_MASK_CLIENT = 0x80,
} ZCL_CLUSTER_MASK_T;

typedef struct {
    ZCL_REPORTING_DIRECTION_T direction;
    uint8_t endpoint;
    CLUSTER_ID_T cluster;
    uint16_t     attribute;

    /** CLUSTER_MASK_SERVER for server-side attributes or CLUSTER_MASK_CLIENT for
    *  client-side attributes.
    */
    ZCL_CLUSTER_MASK_T mask;
    /** Manufacturer code associated with the cluster and/or attribute.  If the
    *  cluster id is inside the manufacturer-specific range, this value
    *  indicates the manufacturer code for the cluster.  Otherwise, if this
    *  value is non-zero and the cluster id is a standard ZCL cluster, it
    *  indicates the manufacturer code for attribute.
    */
    uint16_t manufacturer_code;
    union {
        struct {
            /** The minimum reporting interval, measured in seconds. */
            uint16_t min_interval;
            /** The maximum reporting interval, measured in seconds. */
            uint16_t max_interval;
            /** The minimum change to the attribute that will result in a report
            *  being sent.
            */
            uint32_t reportable_change;
        } reported;
        struct {
            /** The node id of the source of the received reports. */
            uint16_t source_addr;
            /** The remote endpoint from which the attribute is reported. */
            uint8_t endpoint;
            /** The maximum expected time between reports, measured in seconds. */
            uint16_t timeout;
        } received;
    } data;
} zg_report_table_t;

extern bool_t zg_report_table_init(zg_report_table_t *report_config, uint8_t report_config_sums);


typedef void (*ext_plugin_cmd_callback_func_t)(uint8_t endpoint, uint16_t cluster, ZCL_DATA_DIRECTION_T direction, uint8_t cmd, uint8_t args_len, void *args);
typedef struct {
    uint16_t cluster_id;
    ext_plugin_cmd_callback_func_t func;
}ext_plugin_cmd_callback_struct_t;

extern void ext_plugin_identify_client_enable(void);
extern void ext_plugin_identify_server_enable(void);
extern void ext_plugin_green_power_client_enable(void);
extern void ext_plugin_reporting_enable(void);
extern bool_t ext_plugin_register_cmd_handle(ext_plugin_cmd_callback_struct_t *list, uint8_t list_sums);


//******************************************************************************
//                                                                              
//                              hardware timer api                                      
//                                                                              
//******************************************************************************
/**
 * @description: hardware timer enable
 * @param {type} none
 * @return: none
 */
extern void hardware_timer_enable(void);

/**
 * @description: hardware timer disable
 * @param {type} none
 * @return: none
 */
extern void hardware_timer_disable(void);

/**
 * @description: hardware timer start with a us timedelay
 * @param {t} times with us
 * @param {flag} reload timer or not
 * @param {func} callback function
 * @return: TIMER_ID_T: time id
 */
extern TIMER_ID_T timer_hardware_start_100us(uint32_t t, TIMER_RELOAD_FLAG_T flag, hardware_timer_func_t func);

/**
 * @description: hardware timer start with a us timedelay
 * @param {i} user define TIMER_ID_T, the used method is same with dev_timer_start_with_callback();
 * @param {t} times with us
 * @param {flag} reload timer or not
 * @param {func} callback function
 * @return: none
 */
extern void timer_hardware_start_with_id(TIMER_ID_T i, uint32_t t, TIMER_RELOAD_FLAG_T flag, hardware_timer_func_t func);

/**
 * @description: hardware timer stop
 * @param {id} time id
 * @return: none
 */
extern void timer_hardware_stop_100us(TIMER_ID_T id);


/**
 * @description: get timer active flag
 * @param {id} time id
 * @return: TRUE:active, FALSE:inactive 
 */
extern bool_t timer_hardware_is_active(TIMER_ID_T id);


/**
 * @description: get the current systerm millisecond ticks
 * @param {type} none
 * @return: current millisecond ticks
 */
extern uint32_t dev_current_millisecond_ticks_get(void);

/**
 * @description: systerm reset function
 * @param in: none
 * @param out: none
 * @return: none
 */
extern void dev_sys_reset(void) ;

/**
 * @description: send data from mac layer immediately
 * @param in: data: data to be send
 * @param in: data_len: data_len to be send
 * @return: none
 */
extern void dev_sys_data_send_mac(serial_protocol_rf_test_t* rf_test);


//******************************************************************************
//                                                                              
//                              led api                                 
//                                                                              
//******************************************************************************
#define DEV_LED_BLINK_FOREVER 0xFFFF

/**
 * @description: device output io event handler function
 * @param {evt} event id
 * @param {tick} event period ticks
 * @return: none
 */
extern void dev_led_output_handle(uint8_t evt, uint16_t tick);

/**
 * @description: device output(led) blink start funciton
 * @param {led_index} led id
 * @param {on_time} led on time
 * @param {off_time} led off time
 * @param {st} end state, end with on or off
 * @return: none 
 */
extern void dev_led_start_blink(uint8_t led_index, uint16_t on_time, uint16_t off_time, uint16_t blink_times, DEV_IO_ST_T st);

/**
 * @description: device output(led) blink stop function
 * @param {led_index} led id
 * @param {st} end state, end with on or off
 * @return: none
 */
extern void dev_led_stop_blink(uint8_t led_index, DEV_IO_ST_T st);

/**
 * @description: device led is blink or not
 * @param {led_index} led id
 * @return: 0 is not blink, 1 is blink
 */
extern uint8_t dev_led_is_blink(uint8_t led_index);
#define dev_io_op dev_led_stop_blink


//******************************************************************************
//                                                                              
//                              gpio op api                                 
//                                                                              
//******************************************************************************
/**
 * @description: read input gpio status 
 * @param {port} port
 * @param {pin} pin
 * @return: current status value
 */
extern uint8_t gpio_raw_input_read_status( GPIO_PORT_T port, GPIO_PIN_T pin);


/**
 * @description: config a pin as output
 * @param {port} port
 * @param {pin} pin
 * @return: none
 */
extern void gpio_raw_init(gpio_config_t pin);
/**
 * @description: read output gpio status
  * @param {port} port
 * @param {pin} pin
 * @return: current status value
 */
extern uint8_t gpio_raw_output_read_status(  GPIO_PORT_T port, GPIO_PIN_T pin);

/**
 * @description: write output gpio status
  * @param {port} port
 * @param {pin} pin
 * @return: none
 */
extern void gpio_raw_output_write_status(  GPIO_PORT_T port, GPIO_PIN_T pin, uint8_t value);

/**
 * @description: switch output gpio to toggle's status
  * @param {port} port
 * @param {pin} pin
 * @return: none
 */
extern void gpio_raw_output_toggle(GPIO_PORT_T port, GPIO_PIN_T pin);

/**
 * @description: read input gpio status with index 
 * @param {i} index
 * @return: current status value
 */
extern uint8_t gpio_index_input_read_status(  uint8_t i);

/**
 * @description: write output gpio status with index 
 * @param {index} index
 * @param {value} value
 * @return: none
 */
extern void gpio_index_output_write_status(uint8_t index, uint8_t value);
//gpio input api
typedef void(*key_func_t)(uint32_t,key_st_t,uint32_t);
typedef void (*gpio_int_func_t)(GPIO_PORT_T, GPIO_PIN_T);

/**
 * @description: input gpio ISR handle function register
 * @param {key_func} ISR handle fuction
 * @return: none
 */
extern void dev_gpio_input_set_handle_func(key_func_t key_func);

/**
 * @description: input gpio ISR handle function get
 * @param {type} none
 * @return: ISR handle fuction
 */
extern key_func_t dev_gpio_input_get_handle_func(void);

/**
 * @description: gpio button initialize
 * @param {config} config information
 * @param {sum} numbers of button
 * @param {jitter_time} key jitter time
 * @param {key_func} key ISR handle callback
 * @return: none
 */
extern void gpio_button_init(gpio_config_t *config, uint8_t sum, uint32_t jitter_time, key_func_t key_func);

/**
 * @description: gpio output initialize
 * @param {config} config information
 * @param {sum} no=umbers of output gpio information
 * @return: none
 */
extern void gpio_output_init(gpio_config_t *config, uint8_t sum);

/**
 * @description: gpio interrupt functoin set
 * @param {config} config information
 * @param {func} interrupt callback
 * @return: none
 */
extern void gpio_int_register(gpio_config_t *config, gpio_int_func_t func);

/**
 * @description: gpio interrupt enable
 * @param {port} GPIO port
 * @param {pin} GPIO pin
 * @return: none
 */
extern void gpio_int_enable(GPIO_PORT_T port, GPIO_PIN_T pin);

/**
 * @description: gpio interrupt disable
 * @param {port} GPIO port
 * @param {pin} GPIO pin
 * @return: none
 */
extern void gpio_int_disable(GPIO_PORT_T port, GPIO_PIN_T pin);

/**
 * @description: config ADC pin
 * @param {ADC_TYPE_T} ADC capture type
 * @param {pos_pin} ADC pin
 * @param {neg_pin} negative ADC pin
 * @return: none
 */
extern void hal_adc_init(ADC_TYPE_T adc_type, GPIO_PORT_PIN_T *pos_pin, GPIO_PORT_PIN_T *neg_pin);

/**
 * @description: ADC start capture, return MAX value is 4095(12bits), basic voltage 5V
 * @param {void} 
 * @return: ADC value
 */
extern uint16_t hal_adc_get_value(void);

/**
 * @description: translate adc to voltage.
 * @param {adv_value} value of hal_adc_get_value.
 * @return: real voltage*1000
 */
extern uint16_t hal_adc_to_voltage(uint16_t adv_value);


/**
* @description: input battery characteristic to system. 
* @param {cfg} battery capture parameters 
* @param {table} battery voltage characteristic table. 
* @param {table_sums} number of table records.
* @return: none
*/
extern void hal_battery_config(battery_cfg_t *cfg, battery_table_t *table, uint8_t table_sums);

/**
* @description: delay battery capture and report battery info to gateway.
* @param {delay_time} delay time. bet:ms
* @return: none
*/
extern void hal_battery_capture_manual(uint32_t delay_time);

/**
* @description: change battery capture period.
* @param {cap_period_time} battery capture period
* @return: none
*/
extern void hal_battery_capture_period_change(uint32_t cap_period_time);

typedef enum {
    BATTERY_TYPE_DRY_BATTERY = 0,  //Battery percentage always decrease.
    BATTERY_TYPE_CHARGE_BATTERY,   //Permissible battery percentage increase.
}BATTERY_TYPE_T;

typedef enum {
    DEV_BUSY_LEVEL_IDLE = 0, //always sleep
    DEV_BUSY_LEVEL_ALWAYS,   //always wakeup, example: poll forever.
}DEV_BUSY_LEVEL_T;

typedef enum {
    BATTERY_REPORT_DECREASE_LIMITS_10 = 1,
    BATTERY_REPORT_DECREASE_LIMITS_20,
    BATTERY_REPORT_DECREASE_LIMITS_30,
    BATTERY_REPORT_DECREASE_LIMITS_40,
    BATTERY_REPORT_DECREASE_LIMITS_50,
    BATTERY_REPORT_DECREASE_LIMITS_60,
    BATTERY_REPORT_DECREASE_LIMITS_70,
    BATTERY_REPORT_DECREASE_LIMITS_80,
    BATTERY_REPORT_DECREASE_LIMITS_90,
    BATTERY_REPORT_NO_LIMITS,
    BATTERY_REPORT_EXT_LIMITS,
}BATTERY_REPORT_DECREASE_LIMITS_T;

typedef struct {
    BATTERY_TYPE_T   type;
    DEV_BUSY_LEVEL_T level;
    bool_t report_no_limits_first;
    BATTERY_REPORT_DECREASE_LIMITS_T limits;
    uint8_t ext_limits;
}battery_report_policy_t;

/**
* @description: Set battery type
* BATTERY_TYPE_DRY_BATTERY:    Battery percentage always decrease.
* BATTERY_TYPE_CHARGE_BATTERY: Permissible battery percentage increase.
* DEV_BUSY_LEVEL_IDLE:   Battery capture in idle time.
* DEV_BUSY_LEVEL_ALWAYS: battery capture when time out.
* @param {type} battery type
* @param {level} battery type
* @return: none
*/
extern void hal_battery_set_battery_type(BATTERY_TYPE_T type, DEV_BUSY_LEVEL_T level);

/**
* @description: config report policy.
* type:
*   BATTERY_TYPE_DRY_BATTERY:    Battery percentage always decrease.
*   BATTERY_TYPE_CHARGE_BATTERY: Permissible battery percentage increase.
* level:
*   DEV_BUSY_LEVEL_IDLE:   Battery capture in idle time.
*   DEV_BUSY_LEVEL_ALWAYS: battery capture when time out.
* report_no_limits_first:
*   TRUE: report the actual battery percentage value first.
*   FALSE: report the handled battery percentage value first. 
* limits:
*   the limit level of report power percentage.
* ext_limits:
*   use othes value if limits = BATTERY_REPORT_EXT_LIMITS.  limits[2, 100]
* @param {type} battery type
* @param {level} battery type
* @return: none
*/
extern void hal_battery_report_policy_config(battery_report_policy_t *policy);

//******************************************************************************
//                                                                              
//                              flash api                              
//                                                                              
//******************************************************************************
#define FLASH_ADDR_START    (0x0000+250)
#define FLASH_TOTAL_SIZE    4000
#define FLASH_BLOCK_SIZE    250

typedef enum {
    FLASH_BLOCK_1 = 1,
    FLASH_BLOCK_2,
    FLASH_BLOCK_3,
    FLASH_BLOCK_4,
    FLASH_BLOCK_5,
    FLASH_BLOCK_6,
    FLASH_BLOCK_7,
    FLASH_BLOCK_8,
    FLASH_BLOCK_9,
    FLASH_BLOCK_10,
    FLASH_BLOCK_11,
    FLASH_BLOCK_12,
    FLASH_BLOCK_13,
    FLASH_BLOCK_14,
    FLASH_BLOCK_15,
    FLASH_BLOCK_16,
}FLASH_BLOCK_T;

extern uint16_t flash_block_raw_write(FLASH_BLOCK_T block_id, uint8_t *data, uint16_t len);
extern uint16_t flash_block_raw_read(FLASH_BLOCK_T block_id, uint8_t *data, uint16_t len);
extern uint16_t flash_addr_raw_write(uint16_t addr, uint8_t *data, uint16_t len);
extern uint16_t flash_addr_raw_read(uint16_t addr, uint8_t *data, uint16_t len);


typedef struct {
    uint16_t flash_start_addr; //[FLASH_ADDR_START, FLASH_TOTAL_SIZE-1]
    uint16_t items;
    uint16_t item_size;
}dev_record_cfg_t;

typedef enum {
    RECORD_RET_SUCCESS = 0,
    RECORD_RET_ERR,
    RECORD_RET_SPACE_ERR,
}RECORD_RET_T;

typedef int record_fd_t;

extern record_fd_t dev_record_init(dev_record_cfg_t *cfg);
extern RECORD_RET_T dev_record_push(record_fd_t fd, void *record, uint16_t size);
extern RECORD_RET_T dev_record_pop(record_fd_t fd, void *record, uint16_t size);
extern RECORD_RET_T dev_record_get(record_fd_t fd, void *record, uint16_t size);
extern uint16_t dev_record_get_used(record_fd_t fd);
extern uint16_t dev_record_get_capacity    (record_fd_t fd);
extern void dev_record_clear_all(record_fd_t fd); //clear all record

/**
 * @description: write data to flash
 * @param {data} data
 * @param {len} data length
 * @return: none
 */
extern uint8_t user_flash_data_write(uint8_t *data, uint8_t len);

/**
 * @description: read data from flash 
 * @param {data} data
 * @param {len} data length
 * @return: none
 */
extern uint8_t user_flash_data_read(uint8_t *data, uint8_t len);

/**
 * @description: network state get 
 * @param {type} none
 * @return: current network state
 */


//******************************************************************************
//                                                                              
//                              zigbee api                              
//                                                                              
//******************************************************************************

typedef struct {
    uint16_t pan_id;
    uint16_t short_addr;
    uint8_t  channel;
    int8_t   tx_power_db;
    uint8_t  ext_pan_id[8];
}zg_info_t;


/**
 * @description: disable self recovery. NOTE: call this function after dev_zigbee_join_start()
 * @param {type} none
 * @return: none
 */
extern void nwk_disable_self_recovery_once(void);


/**
 * @description: enable self recovery. NOTE: call this function after join timeout.
 * @param {type} none
 * @return: none
 */
extern void nwk_enable_self_recovery_once(void);

/**
 * @description: self recovery. NOTE: call this function after join timeout
 * @param {type} none
 * @return: none
 */
extern void nwk_self_recovery_manual(void);

/**
 * @description: get zigbee dev network status information
 * @param {out_zg_info} output information
 * @return: bool_t: return TRUE if ok. return FLASE else.
 */
extern bool_t zigbee_get_net_info(zg_info_t *out_zg_info);
extern NET_EVT_T nwk_state_get(void);

/**
 * @description: zigbee data send function
 * @param {send_data} data information need to be send
 * @param {fun} data send callback
 * @param {send_timeout} data send timeout
 * @return: none
 */
extern void dev_zigbee_send_data(dev_send_data_t *send_data, send_result_func_t fun, uint32_t send_timeout);

/**
 * @description: clear zigbee data waitting send.
 * @param {type} enum
 * @param {args} reserved
 * @return: none
 */
extern void dev_zigbee_clear_send_data(ZG_CLEAR_TYPE_T type, void *args);
/**
 * @description: clear sepcific cmd send queue,
 * @param {zcl_id} zcl serial number.
 * @param {dest_ep} destination endpoint
 * @param {cluster_id} specific cluster id
 * @return: none
 */
extern void dev_zigbee_specific_response_handle(uint8_t zcl_id, uint8_t dest_ep, uint16_t cluster_id);

/**
 * @description: device heartbeat type and duration set
 * @param {type} heart type, app version attribute or power percentage attribute 
 * @param {duration} heartbeat send duration
 * @return: none
 */
extern bool_t dev_heartbeat_set(HEARTBEAT_TYPE_E type, uint32_t duration);

/**
 * @description: delay bearbeat by user call
 * @param {delay_time} delay time(ms)
 * @return: none
 */
extern void dev_heartbeat_delay_ms(uint32_t delay_time);

/**
 * @description: device leave by user
 * @param {type} none
 * @return: none
 */
extern void dev_zigbee_leave_for_user(void);

/**
 * @description: change dev tx power
 * @param {normal_db} tx power when link is normal 
 * @param {max_db} tx power when link is not normal
 * @return: none
 */
extern void dev_change_power(int8_t normal_db, int8_t max_db);

/**
 * @description: disable gateway change tx power.
 * @param {none} 
 * @return: none
 */
extern void disable_gw_change_power(void);

/**
 * @description: rejoin scan channel config
 * @param {type} poll interval time
 * @return: none
 */
extern void zg_rejoin_scan_policy(ZG_SCAN_POLICY_T type);

/**
 * @description: change poll interval time(ms)
 * @param {poll_interval} poll interval time
 * @return: none
 */
extern void zg_poll_interval_change(uint16_t poll_interval);

/**
 * @description: start poll manual. POLL will be send forever.
 * @param {type} none
 * @return: none
 */
extern void zg_poll_start(void);

/**
 * @description: stop poll manual. the rest of POLL will be send.
 * @param {t} none
 * @return: none
 */
extern void zg_poll_end(void);

/**
 * @description: stop poll manual. the rest of POLL will be clean.
 * @param {t} none
 * @return: none
 */
extern void zg_poll_clear(void);

/**
 * @description: device wakeup with a time; when timeout, it will be sleep
 * @param {t} wakeup time
 * @return: none
 */
extern void zg_wake_up(uint32_t t); //sleep after t ms

/**
 * @description: device sleep now
 * @param {void} none
 * @return: none
 */
extern void zg_sleep(void); //sleep

/**
 * @description: device rejoin now
 * @param {type} none
 * @return: none
 */
extern void zg_rejoin_manual(void);


/**
 * @description: zigbee data will be send soon
 * @param {type} none
 * @return: none
 */
extern bool_t zg_is_busy(void);

/**
 * @description: zigbee is joined or not?
 * @param {type} none
 * @return: boolt_t  TRUE: joined. FALSE no network.
 */
extern bool_t zg_is_join(void);


/**
 * @description: zigbee attribute write function
 * @param {endpoint} endpoint of the attribute
 * @param {cluster} cluster of the attribute
 * @param {attr_id} attribute id
 * @param {data} attribute data to be write
 * @param {data_type} attribute datatype to be write
 * @return: write status
 */
extern ATTR_CMD_RET_T dev_zigbee_write_attribute(
    uint8_t endpoint,
    CLUSTER_ID_T cluster,
    uint16_t attr_id, //attribute-id.h
    void* data,
    ATTR_TYPE_T dataType
);

/**
 * @description: zigbee attribute read function
 * @param {endpoint} endpoint of the attribute
 * @param {cluster} cluster of the attribute
 * @param {attr_id} attribute id
 * @param {data} attribute data read result
 * @param {read_length} attribute datalength to be read
 * @return: write status
 */
extern ATTR_CMD_RET_T dev_zigbee_read_attribute(
    uint8_t endpoint,
    CLUSTER_ID_T cluster,
    uint16_t attr_id, //attribute-id.h
    void *data,
    uint8_t read_length
);

//base tools
/**
 * @description: random a time with 0-t ms
 * @param {t} random maxinum ms
 * @return: none
 */
extern uint32_t random_ms(uint32_t t);

/**
 * @description: caculate the crc of the date
 * @param {data} data
 * @param {data_len} data length
 * @return: crc result
 */
extern uint16_t make_crc16(uint8_t *msg, uint16_t len);

/**
 * @description: firmware security config
 * @param {flag} TRUE: switch on verify,  FALSE: swicth off verify.
 * @return: none
 */
extern void dev_security_mode_set(bool_t flag);

/**
 * @description:  scene cluster view valid callback, this function is used to check if the secne and group of the endpoint is exist 
 * @param {endpoint} endpoint
 * @param {groupId} group id
 * @param {sceneId} scene id
 * @return: true: exist, false: not exist;
 */
extern bool_t devViewSceneValidCallback(uint16_t endpoint, uint16_t groupId, uint8_t sceneId);

/**
 * @description: this function is used to remove the scene info from scene table of the endpoint
 * @param {endpoint} endpoint
 * @param {groupId} group id
 * @param {sceneId} scene id
 * @return: true or false
 */
extern bool_t devScenesClusterRemoveSceneCallback(uint16_t endpoint, uint16_t groupId, uint8_t sceneId);

/**
 * @description: this function is used to get the scene info from scene table of the endpoint
 * @param {endpoint} in   endpoint
 * @param {*groupId} out  group id
 * @param {*sceneId} out  scene id
 * @return: true or false
 */
extern bool_t devGetSceneCallback(uint16_t endpoint, uint16_t* groupId, uint8_t* sceneId);

/**
 * @description: remove all scenes in scene table of current endpoint; just used by scene control device; default is not remove
 * @param {endpoint} endpoint
 * @return: true: remove all, false: not remove;
 */
extern bool_t zigbee_sdk_scene_remove_before_add(uint8_t endpoint);

/**
 * @description: scene recall command send
 * @param {endpoint} src endpoint of recall scene
 * @param {groupId} group id
 * @param {sceneId} scene id
 * @return: none
 */
extern bool_t dev_scene_recall_send_command(uint8_t endpoint, uint16_t groupId, uint8_t sceneId);


//******************************************************************************                                                                      
//                              pwm api    
// user should call hal_pwm_init() first to initialize the pwm module.
// if you want to set duty_cycle with specific resolution, follow steps bellow:
// 1.uint16_t max_value = get_max_pwm_value();
// 2.uint16_t value = x * max_value/256;
// 3.set_pwm_value(value);
// thus, resolution = 256, and duty_cycle is x/256
//         
// if you want to set duty_cycle with resolution 1%, you just neet to call the 
// API:set_pwm_duty().
//                                                       
//******************************************************************************
typedef struct{
    GPIO_PORT_T port;
    GPIO_PIN_T pin;
    GPIO_LEVEL_T lv;
    bool_t invert;
}pwm_gpio_t;
/**
 * @description: pwm init 
 * @param {config} pwm pins config
 * @param {sum} pwm channel want to be init
 * @param {pwm_freq} pwm frequency
 * @return: 1:success, 0: failed
 */
uint8_t hal_pwm_init(pwm_gpio_t *config, uint8_t sum, uint16_t pwm_freq);

/**
 * @description: set pwm value, duty_cycle = value/max_value
 * @param {index} choose pwn channel
 * @param {value} value to be set
 * @return: 1:success, 0: failed
 */
uint8_t set_pwm_value(uint8_t index, uint16_t value);

/**
 * @description: get max pwm value 
 * @param {none} 
 * @return: max pwm value 
 */
uint16_t get_max_pwm_value(void);

/**
 * @description: get max pwm value 
 * @param {none} 
 * @return: max pwm value 
 */
uint8_t set_pwm_duty(uint8_t index, uint16_t duty, uint16_t precision);


//******************************************************************************
//                                                                              
//                              soft i2c                               
//                                                                              
//******************************************************************************
typedef struct{
    GPIO_PORT_PIN_T scl; //scl pin config
    GPIO_PORT_PIN_T sda; //sda pin config
    GPIO_PORT_PIN_T power; //iic slave device power control pin config
    bool_t power_pin_enable; //enable/disable iic slave device power control
}i2c_gpio_t;

bool_t hal_i2c_init(i2c_gpio_t *config, uint8_t iic_id);
bool_t hal_i2c_start(uint8_t iic_id);
bool_t hal_i2c_stop(uint8_t iic_id);
bool_t hal_i2c_check_ack(uint8_t iic_id);
void hal_i2c_send_byte(uint8_t iic_id, uint8_t data);
void hal_i2c_power_enable(uint8_t iic_id);
void hal_i2c_power_disable(uint8_t iic_id);

/*********************************************/
// APIs for custom iic protocol
void hal_i2c_set_sda_high(uint8_t iic_id);
void hal_i2c_set_sda_low(uint8_t iic_id);
uint8_t hal_i2c_get_sda_value(uint8_t iic_id);
void hal_i2c_set_scl_high(uint8_t iic_id);
void hal_i2c_set_scl_low(uint8_t iic_id);
uint8_t hal_i2c_get_scl_value(uint8_t iic_id);
void hal_i2c_set_power_high(uint8_t iic_id);
void hal_i2c_set_power_low(uint8_t iic_id);
uint8_t hal_i2c_get_power_value(uint8_t iic_id);
// APIs for custom iic protocol end
/*********************************************/

/*********************************************/
// APIs for spi driver
/// SPI driver instance type.
typedef enum {
  SPI_MASTER = 0,               ///< Act as an SPI master.
  SPI_SLAVE  = 1                ///< Act as an SPI slave.
} SPI_TYPE_T;

/// SPI bus bit order.
typedef enum {
  SPI_LSB_FIRST = 0,     ///< LSB bit is transmitted first.
  SPI_MSB_FIRST = 1      ///< MSB bit is transmitted first.
} SPI_BIT_ORDER_T;

/// SPI clock mode (clock polarity and phase).
typedef enum {
  SPI_CLOCK_MODE0  = 0,           ///< SPI mode 0: CLKPOL=0, CLKPHA=0.
  SPI_CLOCK_MODE1  = 1,           ///< SPI mode 1: CLKPOL=0, CLKPHA=1.
  SPI_CLOCK_MODE2  = 2,           ///< SPI mode 2: CLKPOL=1, CLKPHA=0.
  SPI_CLOCK_MODE3  = 3            ///< SPI mode 3: CLKPOL=1, CLKPHA=1.
} SPI_CLOCK_MODE_T;

/// SPI master chip select (CS) control scheme.
typedef enum {
  SPI_CS_CONTROL_AUTO  = 0,        ///< CS controlled by the SPI driver.
  SPI_CS_CONTROL_APP  = 1  ///< CS controlled by the application.
} SPI_CSCONTROL_T ;

/// SPI slave transfer start scheme.
typedef enum {
  SPI_SLAVE_START_IMMEDIATE  = 0,  ///< Transfer starts immediately.
  SPI_SLAVE_START_DELAYED  = 1     ///< Transfer starts when the bus is idle (CS deasserted).
} SPI_SLAVE_START_T ;

/// SPI frame length .
typedef enum {
  SPI_FRAME4  = 4,  
  SPI_FRAME5  = 5,   
  SPI_FRAME6  = 6,
  SPI_FRAME7  = 7,
  SPI_FRAME8  = 8,
  SPI_FRAME9  = 9,
  SPI_FRAME10  = 10,
  SPI_FRAME11  = 11,
  SPI_FRAME12  = 12,
  SPI_FRAME13  = 13,
  SPI_FRAME14  = 14,
  SPI_FRAME15  = 15,
  SPI_FRAME16  = 16
} SPI_FRAME_LENGTH_T;


/// An SPI driver instance initialization structure.
typedef struct {
  UART_ID_T           port_id;            ///< The USART used for SPI.
  GPIO_LOC_T          port_mosi_loc;   ///< A location number for the SPI Tx pin.
  GPIO_LOC_T          port_miso_loc;   ///< A location number for the SPI Rx pin.
  GPIO_LOC_T          port_clk_loc;  ///< A location number for the SPI Clk pin.
  GPIO_LOC_T          port_cs_loc;   ///< A location number for the SPI Cs pin.
  GPIO_PORT_PIN_T     cs;
  uint32_t            bitRate;          ///< An SPI bitrate.
  SPI_FRAME_LENGTH_T  frame_length;      ///< An SPI framelength, valid numbers are 4..16
  SPI_TYPE_T          type;             ///< An SPI type, master or slave.
  SPI_BIT_ORDER_T     bit_order;         ///< A bit order on the SPI bus, MSB or LSB first.
  SPI_CLOCK_MODE_T    clock_mode;        ///< SPI mode, CLKPOL/CLKPHASE setting.
  SPI_CSCONTROL_T     cs_control;        ///< A select master mode chip select (CS) control scheme.
  SPI_SLAVE_START_T   slave_start_mode;   ///< A slave mode transfer start scheme.
} spi_init_config_t ;

// Configuration data for SPI Master Init
#define DEFAULT_SPI_CONFIG                                                     \
  {                                                                    \
    UART_ID_UART1,          /* USART port                       */ \
    LOC_23,                 /* USART Tx/MOSI pin location number default PD15 */ \
    LOC_4,                  /* USART Rx/MISO pin location number default PA5 */ \
    LOC_0,                  /* USART Clk pin location number default PA2*/ \
    LOC_23,                 /* USART Cs pin location number  default PF2*/ \
    {PORT_F, PIN_2},\
    50000,                  /* Bitrate                          */ \
    SPI_FRAME8,             /* Frame length                     */ \
    SPI_MASTER,             /* SPI mode                         */ \
    SPI_MSB_FIRST,          /* Bit order on bus                 */ \
    SPI_CLOCK_MODE0,        /* SPI clock/phase mode             */ \
    SPI_CS_CONTROL_APP,     /* CS controlled by aplication      */ \
    SPI_SLAVE_START_IMMEDIATE   /* Slave start transfers immediately*/ \
  }
/***************************************************************************//**
 * @brief Initializes the SPI communication peripheral.
 * @param spi_config - spi initializes parameters
 * @return success 1  failed 0.

*******************************************************************************/
bool_t spi_init_config(spi_init_config_t *spi_config);
/*****************************************************************************
 * @brief SPI deinit 
 *
 * @param none.
 * @return none.
*******************************************************************************/
void spi_deinit(void);

/*****************************************************************************
 * @brief SPI Writes and read data 
 *
 * @param data_len - write data len.
 * @param data_buffer - spi write data 
 * @return none.
*******************************************************************************/
void spi_write_data(uint8_t data_len,uint8_t data_buffer[]);

/*****************************************************************************
 * @brief SPI read data 
 *
 * @param data_len - read data len.
 * @param data_buffer - spi read data 
 * @return none.
*******************************************************************************/
void spi_read_data(uint8_t data_len,uint8_t data_buffer[]);

/*****************************************************************************
 * @brief SPI Writes and read data 
 *
 * @param write_data_len - write data len.
 * @param write_data_buffer - spi write data 
 * @param read_data_len - read data len.
 * @param read_data_buffer - spi read data 
 * @return none.
*******************************************************************************/

void spi_write_and_read_data(uint8_t write_data_len,uint8_t write_data_buffer[],\
                                        uint8_t read_data_len,uint8_t read_data_buffer[]);


// APIs  for spi driver
/*********************************************/

void set_uart_rx_wakeup_flag(bool_t flag);

/**
 * @description: device mac address get function
 * @param {returnEui64} 8 bytes mac address
 * @return: none
 */
void device_mac_get(Device_Mac_64 returnEui64);

/*********************************************/
// APIs for Time Cluster Sync

/**
 * @description: zigbee time struct 
 */

typedef struct {
  uint16_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hours;
  uint8_t minutes;
  uint8_t seconds;
  uint8_t week; //0=MON, 1=TUES, etc.
} device_time_struct_t;


typedef struct {
    uint16_t year; //normal range:> 2019, 0xFFFF means: Wildcard character, It's valid for any year
    uint8_t  mon;  //normal range:1~12, 0xFF means: Wildcard character, It's valid for any month
    uint8_t  day;  //normal range:1~31, 0xFF means: Wildcard character, It's valid for any day
}app_day_t;

typedef struct {
    uint8_t  hour; //normal range:0~23, 0xFF means: Wildcard character, It's valid for any hour
    uint8_t  min;  //normal range:0~59, 0xFF means: Wildcard character, It's valid for any minute
    uint8_t  sec;  //range: 0~59
}app_time_t;

typedef enum {
    APP_TIMER_TYPE_CALENDAR = 0,
    APP_TIMER_TYPE_WEEK,
}APP_TIMER_TYPE_T;


#define WEEK_BITS_MAP_MONDAY    0x01
#define WEEK_BITS_MAP_TUESDAY   0x02
#define WEEK_BITS_MAP_WEDNESDAY 0x04
#define WEEK_BITS_MAP_THURSDAY  0x08
#define WEEK_BITS_MAP_FRIDAY    0x10
#define WEEK_BITS_MAP_SATURDAY  0x20
#define WEEK_BITS_MAP_SUNDAY    0x40

typedef uint8_t WEEK_BITS_MAP_T;

//index: app_timer_struct_t index
typedef void (*app_timer_callback_t)(uint8_t index);

typedef struct {
    APP_TIMER_TYPE_T type;
    bool_t period_action_flag;
    bool_t valid;
    app_time_t time;
    union {
        app_day_t calendar_day;
        WEEK_BITS_MAP_T week_map;
    } day_struct;
    app_timer_callback_t func;
}app_timer_struct_t;

/**
 * @description: config the app timer
 * @param {app_timer_struct_list} timer task list.
 * @param {sums} timer task sums
 * @return: TRUE for successful, FALSE for failed
 */
extern bool_t app_timer_config(app_timer_struct_t *app_timer_struct_list, uint8_t sums);

/**
 * @description: get the remain seconds of next app timer task
 * @param {none}
 * @return: time: seconds
 */
extern uint32_t app_timer_get_next_task_seconds(void);

/**
 * @description: set time period for device get time form gateway
 * @param {time_period} time period  (unit: ms) ,default time (30 * 60 *1000 ms) 
 * @return: none
 */

void set_read_time_period(uint32_t time_period);

/**
 * @description: get device current time (unit:S)
 * @param none
 * @return: device current time (unit:S)
 */
uint32_t  get_current_time(void);

/**
 * @description: set time update interval. default value: 1 (unit: second)
 * @param {interval_sec} update ervery interval_sec.
 * @return: none
 */
void dev_time_tick_update_interval_set(uint16_t interval_sec);

/**
 * @description: get the parameter of update time (unit:second)
 * @param none
 * @return: interval of update time (unit:S)
 */
uint16_t dev_time_tick_update_interval_get(void);


/**
 * @description: get device current time with convert time struct
 * @param {device_time} time struct with yeah ,month,day,hour,minute,second
 * @return: none
 */
void get_current_time_struct(device_time_struct_t *device_time);
// APIs for Time Cluster Sync end
/*********************************************/

/*********************************************/
// APIs for ZLL
/**
 * @description: device create distributed network
 * @param {none} 
 * @return: 1:success, 0: failed
 */
bool_t zll_create_distributed_network(void);
/**
 * @description: The identify request command callback is used to request that
 *      the recipient identifies itself in some application specific way to aid with touchlinking
 * @param {durationS} (unit:S)
 *          0x0000 Exit identify mode
 *          0x0001 - 0xfffe Number of seconds to remain in identify mode
 *          0xffff Remain in identify mode for a default time known by the receiver
 * @return: none
 */
void zll_server_identify_callback(uint16_t durationS);
/**
 * @description: zll Server joined a new network with touchlink 
 * @param {panId} the new network panid
 * @return: none 
 */
void zll_Server_touchlink_joined(uint16_t panId );
/**
 * @description: zll Server leave network with touchlink
 * @param {none} 
 * @return: none
 */
void zll_Server_touchlink_leave(void);

// zll client api

typedef struct{
    uint8_t long_addr[8];
    uint16_t short_addr;
    uint8_t ep_num;
    uint8_t ep_val[16];
}zll_device_info_t;


/**
 * @description: zll  joined device callback 
 * @param {zll  target device information} 
 * @return: none
 */
void zll_joined_device_info(zll_device_info_t zll_dev_info);

/**
 * @description: zll  touch link scan failed callback
 * @param {none} 
 * @return: none
 */
void zll_touch_link_failed(void);

/**
 * @description: zll  touch link scan 
 * @param {none} 
 * @return: none
 */
void zll_touch_link_scan(void);

/**
 * @description: zll  device information request
 * @param {none} 
 * @return: none
 */
void zll_device_information_request(void);

/**
 * @description: zll  identify request
 * @param {none} 
 * @return: none
 */
void zll_identify_request(void);

/**
 * @description: zll reset to factory new request
 * @param {none} 
 * @return: none
 */
void zll_reset_to_factory_new_request(void);
// APIs for ZLL end
/*********************************************/

//******************************************************************************
//                                                                              
//                               hardware i2c                                   
//                                                                              
//******************************************************************************
typedef enum {
    I2C0_M = 0,
}HARDWARE_I2C_TYPE_M;

typedef enum {
    I2C0_FREQ_10_KHz_M = 0,
    I2C0_FREQ_40_KHz_M,
    I2C0_FREQ_100_KHz_M,
}HARDWARE_I2C_FREQ_M;

typedef struct{
    HARDWARE_I2C_TYPE_M i2c; //scl type config

    GPIO_PORT_T scl_port; //scl pin config
    GPIO_PIN_T scl_pin;
    uint8_t scl_location;

    GPIO_PORT_T sda_port; //scl pin config
    GPIO_PIN_T sda_pin;
    uint8_t sda_location;

    HARDWARE_I2C_FREQ_M i2c_freq; //i2c freq
}hardware_i2c_config_t;

/*********************************************/
// APIs for custom hardware iic protocol
void hal_hardware_i2c_init(hardware_i2c_config_t *set_i2cConfig);
int8_t hal_hardware_i2c_write_bytes(uint8_t address, uint8_t *buffer, uint8_t count);
int8_t hal_hardware_i2c_write_bytes_delay(uint8_t address, const uint8_t *buffer, uint8_t count, uint8_t delay);
int8_t hal_hardware_i2c_read_bytes(uint8_t address, uint8_t *buffer, uint8_t count);

// APIs for custom hardware iic protocol end
/*********************************************/

/**
 * @description: device error code set function, when device is run with something, 
 * application should use this function to story the error code to flash, when reconnect
 * to the gateway, the error code will be report to gateway.
 * @param {err_code} DEV_ERROR_CODE_E: error code
 * @return: true or false
 */
bool_t dev_error_code_set(DEV_ERROR_CODE_E err_code);


typedef enum {
    CALLBACK_TYPE_REPORT_BATTERY = 0,
    CALLBACK_TYPE_CAPTURE_BATTERY,
    CALLBACK_TYPE_POLL,
}CALLBACK_TYPE_T;

typedef struct {
    uint16_t report_voltage;
    uint16_t real_voltage;
    uint8_t  report_percent;
    uint8_t  real_percent;
}cb_battery_args_t;

typedef enum {
    POLL_OK = 0,
    POLL_NO_ACK,
    POLL_BUSY,
}POLL_RESULT_T;

typedef struct {
    POLL_RESULT_T result;
}cb_poll_args_t;


typedef struct {
    CALLBACK_TYPE_T type;
    union {
        cb_battery_args_t battery;
        cb_poll_args_t poll;
    }args;
}cb_args_t;

typedef void (*sdk_evt_callback_fun_t)(cb_args_t *args);

extern void sdk_cb_register(CALLBACK_TYPE_T type, sdk_evt_callback_fun_t func);


typedef void (*soft_sv_callback_t)(void);

/**
 * @description: start a PendSV interrupt. PendSV_Handler will call this funciton(soft_sv_callback_t).
 * @param {func} user function
 * @return: none 
 */
extern void start_soft_int(soft_sv_callback_t func);

/**
 * @description: device recovery to factury command handler
 * @param {type} just reset to fdactury or reset to factury and leave
 * @return: none
 */
extern void dev_recovery_factory(DEV_RESET_TYPE_T type);


typedef enum {
    VALUE_TYPE_STRING,         //ex:char a[3] = {'n','b','\0'};
    VALUE_TYPE_CHAR,           //ex:char a = 'b';
    VALUE_TYPE_NUMBER_DECIMAL, //ex:u32 a = 100000;
    VALUE_TYPE_NUMBER_HEX,     //ex:u8 a[3] = {2,0xDE,0xED}; 2=>hex sums
}OEM_VALUE_TYPE;

typedef struct {
    char* key;          //user "json" key
    void* value;        //user value buffer
    uint8_t value_size; //user value buffer length
    OEM_VALUE_TYPE type;    //user value type
    bool_t handle_flag; //TRUE: got a valid value.
}oem_key_value_t;


/**@brief Load device information
 *
 * @param[len] OUT len Data length
 * @param[data] OUT data Data to be retrieved.
 *
 * @return Return success or
 */
extern bool_t oem_config_load(uint16_t *len, uint8_t **data);

/**
 * @description: setting oem-config-data, 
 * @param {data} IN oem config data
 * @param {data_len} IN oem config data length
 * @return: TRUE or FALSE
 */
extern bool_t oem_config_set(uint8_t *data, uint16_t data_len);

/**
 * @description: Parse key-value data, ex: {key1:value1,key2:value2,} 
 * @param {kv_str} IN key-value strings.
 * @param {int_out_table} IN OUT user table
 * @param {table_sums} IN OUT user table sums
 * @return: TRUE or FALSE
 */
extern bool_t get_key_values(char *kv_str, oem_key_value_t *int_out_table, uint16_t table_sums);

/**
 * @description: load oem config and parse key-value data, ex: {key1:value1,key2:value2,} 
 * @param {int_out_table} IN OUT user table
 * @param {table_sums} IN OUT user table sums
 * @return: TRUE or FALSE
 */
extern bool_t get_oem_key_values(oem_key_value_t *int_out_table, uint16_t table_sums);


/**
 * @description: get system authkey for security.
 * @param {buf} a buffer store authkey
 * @param {buf_len} buffer length
 * @return: authkey length. 0 means error.
 */
extern uint8_t dev_get_auth_key(uint8_t *buf, uint8_t buf_len);
         
///////////////////////////////////////////////////////////////
//timer1 hardware tools.
//int flag
#define INT_FLAG_ICBOF3  (0x80UL)
#define INT_FLAG_ICBOF2  (0x400UL)
#define INT_FLAG_ICBOF1  (0x200UL)
#define INT_FLAG_ICBOF0  (0x100UL)
#define INT_FLAG_CC3     (0x80UL)
#define INT_FLAG_CC2     (0x40UL)
#define INT_FLAG_CC1     (0x20UL)
#define INT_FLAG_CC0     (0x10UL)
#define INT_FLAG_UF      (0x02UL)
#define INT_FLAG_OF      (0x01UL)

typedef void (*timer1_int_callback_t)(uint32_t int_flag);

typedef enum {
    TIMER1_CC0 = 0,
    TIMER1_CC1 = 1,
    TIMER1_CC2 = 2,
    TIMER1_CC3 = 3,
}TIMER1_CC_T;

typedef enum {
    TIMER_DIV1 = 0,     /**< Divide by 1. */
    TIMER_DIV2,         /**< Divide by 2. */
    TIMER_DIV4,         /**< Divide by 4. */
    TIMER_DIV8,         /**< Divide by 8. */
    TIMER_DIV16,        /**< Divide by 16. */
    TIMER_DIV32,        /**< Divide by 32. */
    TIMER_DIV64,        /**< Divide by 64. */
    TIMER_DIV128,       /**< Divide by 128. */
    TIMER_DIV256,       /**< Divide by 256. */
    TIMER_DIV512,       /**< Divide by 512. */
    TIMER_DIV1024,      /**< Divide by 1024. */
}TIMER_DIV_T;

/**
 * @description: timer1 init
 * @param {enable} init completed enable
 * @param {prescale} timer prescale
 * @param {func} timer callback
 * @return: none
 */
extern void dev_timer1_init(bool_t enable, TIMER_DIV_T prescale, timer1_int_callback_t func);

/**
 * @description: get timer1 count value
 * @param {type} none
 * @return: none
 */
extern uint32_t dev_timer1_counter_get(void);


/**
 * @description: clear timer1 count value
 * @param {type} none
 * @return: none
 */
extern void dev_timer1_counter_reset(void);

/**
 * @description: timer1 overflow interrupt enable/disable
 * @param {type} none
 * @return: none
 */
extern void dev_timer1_overflow_int_set(bool_t int_enable);


/**
 * @description: set timer top value
 * @param {us} us
 * @return: none
 */
extern void dev_timer1_top_set(uint32_t us);

/**
 * @description: set timer top buff value,will ato updated in next wrap around
 * @param {us} us
 * @return: none
 */
extern void dev_timer1_top_next_set(uint32_t us);

/**
 * @description: init timer1 compare mode and enable compare interrupt
 * @param {ch} compare channel
 * @return: none
 */
extern void dev_timer1_cc_init(TIMER1_CC_T ch);


/**
 * @description: default init timer capture/compare and disable interrupt
 * @param {type} none
 * @return: none
 */
extern void dev_timer1_cc_deinit(TIMER1_CC_T ch);

/**
 * @description: compare interrupt enable/disable
 * @param {type} none
 * @return: none
 */
extern void dev_timer1_cc_int_set(TIMER1_CC_T ch, bool_t int_enable);

/**
 * @description: set compare value
 * @param {ch} channel
 * @param {us} us
 * @return: none
 */
extern void dev_timer1_compare_counter_set(TIMER1_CC_T ch, uint32_t us);

/**
 * @description: set timer compare buff value,will ato updated in next wrap around
 * @param {ch} channel
 * @param {us} us
 * @return: none
 */
extern void dev_timer1_compare_counter_buf_set(TIMER1_CC_T ch, uint32_t us);

/**
 * @description: start timer1
 * @param {type} none
 * @return: none
 */
extern void dev_timer1_start(void);

/**
 * @description: stop timer1
 * @param {type} none
 * @return: none
 */
extern void dev_timer1_stop(void);

/**
 * @description: reset timer1
 * @param {type} none
 * @return: none
 */
extern void dev_timer1_reset(void);

/**
 * @description: get timer1 start flag
 * @param {type} none
 * @return: none
 */
extern bool_t dev_timer1_started(void);



typedef enum {
    FIND_BIND_ST_OK = 0,
    FIND_BIND_ST_ERR,
}FIND_BIND_ST_T;

typedef void (*find_bind_callback_t)(FIND_BIND_ST_T st);
typedef void (*find_nwk_addr_callback_t)(uint16_t nwk_addr);
typedef void (*find_match_dev_callback_t)(uint16_t nwk_addr, uint8_t endpoint);

extern void zg_find_nwk_addr(uint8_t *mac, find_nwk_addr_callback_t func);
extern void zg_match_dev_by_profile_cluster(uint16_t dest_addr, 
                                                    PROFILE_ID_T profile_id,
                                                    CLUSTER_ID_T cluster_id,
                                                    bool_t is_server_cluster,
                                                    find_match_dev_callback_t func);
extern void zg_find_bind_start(uint8_t endpoint);
extern void zg_find_bind_start_with_callback(uint8_t endpoint, find_bind_callback_t func);


/**
 * @description: get the gpio port and pin according the index
 * @param {index} gpio index
 * @return: gpio port and pin 
 */
extern GPIO_PORT_PIN_T* oem_gpio_get(uint8_t index);


typedef enum {
    ZG_JOIN_TYPE_CENTRALIZED = 0, //centralized join to gateway
    ZG_JOIN_TYPE_DISTRIBUTE,      //distribute join to router
    ZG_JOIN_TYPE_NO_NETWORK       //no network
}ZG_JOIN_TYPE_T;

typedef enum {
    ZG_JOIN_GW_TYPE_TUYA = 0,    //tuya gateway
    ZG_JOIN_GW_TYPE_OTHERS,      //others gateway
    ZG_JOIN_GW_TYPE_UNKNOWN      //no network or distribute network
}ZG_JOIN_GW_TYPE_T;


extern ZG_JOIN_TYPE_T zg_get_join_type(void);
extern ZG_JOIN_GW_TYPE_T zg_get_join_gw_type(void);
extern bool_t zg_is_zll_net(void); //for zll lib


//ota enable disable
extern void zg_ota_enable_disable(bool_t en_flag);

#define DEV_DEFAULT_WAKEUP_MAX_TIMEOUT   (1000*60*2)  //default max wakeup interval is 2 min
/**
 * @description: set the unusual wakeup timeout times for sleep end device. if the wakeup time is more than the wakeupTime,
    then will call the dev_unusual_wakeup_timeout_callback function to app;
 * @param {wakeupTime} wakeup time
 * @return: none 
 */
extern void dev_unusual_wakeup_timeout_set(uint32_t wakeupTime);

/**
 * @description: a weak function. default return TRUE. user can  redefine this function.
                 unusal wakeup timeout time callback function
 * @param {} user function
 * @return: bool_t TRUE: system will reboot, FLASE: system do nothing.
 */
VIRTUAL_FUNC bool_t dev_unusual_wakeup_timeout_callback(void);


typedef enum {
    DEV_VER_ZHA = 0x00,
    DEV_VER_ZG30,
    DEV_VER_ZLL,
}DEV_VER_T;

VIRTUAL_FUNC DEV_VER_T dev_ep_to_dev_ver(uint8_t ep);

VIRTUAL_FUNC void zll_reset_to_factory_new_callback(void);

VIRTUAL_FUNC void dev_sleep_before_callback(uint32_t t);
VIRTUAL_FUNC void dev_wake_up_callback(uint32_t t);
VIRTUAL_FUNC void dev_uart_gpio_wake_up_callback(GPIO_PIN_T pin);

//simple json parse function
typedef enum {
    JSON_RET_PARSE_OK = 0,
    JSON_RET_ERR
}JSON_RET_T;

/*usage:
    char *json_str  = "{\"key1\":\"hello\", \"key2\":1000}";
    char value_str[128] = {0};
    int value_int;
    JSON_RET_T ret;

    ret = json_get_str_value(json_str, "key1", value_str, sizeof(value_str));
    if(ret == JSON_RET_PARSE_OK) {
        app_print("key1's value: %s\n", value_str); // value_str equ hello
    }

    ret = json_get_int_value(json_str, "key2", &value_int); //value_int equ 1000
    if(ret == JSON_RET_PARSE_OK) {
        app_print("key2's value: %d\n", value_int);
    }
*/
extern JSON_RET_T json_get_str_value(const char *json_str, char *key, char *out_buffer, uint16_t out_buffer_len);
extern JSON_RET_T json_get_int_value(const char *in_json_str, char *key, int *out_int_value);



typedef enum {
    /** A binding that is currently not in use. */
    DEV_BIND_TYPE_UNUSED         = 0,
    /** A unicast binding whose 64-bit identifier is the destination EUI64. */
    DEV_BIND_TYPE_UNICAST        = 1,
    /** A unicast binding whose 64-bit identifier is the many-to-one
    * destination EUI64.  Route discovery should be disabled when sending
    * unicasts via many-to-one bindings. */
    DEV_BIND_TYPE_MANY_TO_ONE    = 2,
    /** A multicast binding whose 64-bit identifier is the group address. This
    * binding can be used to send messages to the group and to receive
    * messages sent to the group. */
    DEV_BIND_TYPE_MULTICAST      = 3,
}DEV_BIND_TYPE_T;

typedef struct {
    /** The type of binding. */
    DEV_BIND_TYPE_T type;
    /** The endpoint on the local node. */
    uint8_t local;
    /** A cluster ID that matches one from the local endpoint's simple descriptor.
    * This cluster ID is set by the provisioning application to indicate which
    * part an endpoint's functionality is bound to this particular remote node
    * and is used to distinguish between unicast and multicast bindings. Note
    * that a binding can be used to to send messages with any cluster ID, not
    * just that listed in the binding.
    */
    uint16_t cluster_id;
    /** The endpoint on the remote node (specified by \c identifier). */
    uint8_t remote;
    /** A 64-bit identifier.  This is either:
    * - The destination EUI64, for unicasts.
    * - A 16-bit multicast group address, for multicasts.
    */
    uint8_t identifier[8];
}dev_bind_table_entry;

extern bool_t dev_bind_table_add(dev_bind_table_entry *bind_entry); //add a entry
extern bool_t dev_bind_table_del(dev_bind_table_entry *bind_entry); //delete a entry

#ifdef __cplusplus
}
#endif

#endif
