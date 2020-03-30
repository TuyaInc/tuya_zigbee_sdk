/**
 * @author Leon
 * @par email:
 *      zhangpeng@tuya.com
 * @file tuya_zigbee_stack.h
 * @brief This file defines the interface associated with the Zigbee protocol stack.
 * @copyright HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @par company
 *      http://www.tuya.com
 * @date 2020-03-20 17:29:33
 */

#ifndef __TUYA_ZIGBEE_STACK_H__
#define __TUYA_ZIGBEE_STACK_H__

#include "type_def.h"
#include "zigbee_cmd.h"
#include "zigbee_attr.h"



#ifdef __cplusplus
extern "C" {
#endif

/**
 * @enum Zigbee profile ID
 */
typedef enum {
    ZHA_PROFILE_ID = 0x0104, ///< home automation
    ZGP_PROFILE_ID = 0xA1E0, ///< green power
    ZHC_PROFILE_ID = 0x0108, ///< personal,home and hospital care
    ZSE_PROFILE_ID = 0x0109, ///< Smart energy
    ZCBA_PROFILE_ID = 0x0105,///< Commerical building automation
    ZIPM_PROFILE_ID = 0x0101,///< Industial plant monitoring
    ZTA_PROFILE_ID = 0x0107, ///< Telecom application
    ZAMI_PROFILE_ID = 0x0109,///< Advanced metering initiative
    ZLL_PROFILE_ID = 0xC05E, ///< ZLL profile
}PROFILE_ID_T;

/**
 * @enum Zigbee cluster ID
 */
typedef enum {
    CLUSTER_BASIC_CLUSTER_ID = 0x0000,          ///< basic cluster id
    CLUSTER_POWER_CONFIG_CLUSTER_ID = 0x0001,
    CLUSTER_DEVICE_TEMP_CLUSTER_ID = 0x0002,
    CLUSTER_IDENTIFY_CLUSTER_ID = 0x0003,
    CLUSTER_GROUPS_CLUSTER_ID = 0x0004,
    CLUSTER_SCENES_CLUSTER_ID = 0x0005,
    CLUSTER_ON_OFF_CLUSTER_ID = 0x0006,         ///< on off cluster id
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

/**
 * @enum Zigbee device ID
 */
typedef enum {
    ZG_DEVICE_ID_ON_OFF_SWITCH = 0x0000,        ///< on off switch
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

    ZG_DEVICE_ID_ON_OFF_LIGHT = 0x0100,             ///< on off light
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

/**
 * @enum Zigbee scan policy
 */
typedef enum {
    ZG_SCAN_POLICY_CURR_CHANNEL_FOREVER = 0, ///< scan current channel forever when rejoin-scanning.
    ZG_SCAN_POLICY_ALL_CHANNEL_ONCE,         ///< scan all channel at the last time when rejoin-scanning.
}ZG_SCAN_POLICY_T;

/**
 * @enum Zigbee scan duration(The duration of channel reception)
 */
typedef enum {
    ZB_SCAN_DURATION_0 = 0, ///<   19.2 ms
    ZB_SCAN_DURATION_1,     ///<   38.4 ms
    ZB_SCAN_DURATION_2,     ///<   76.8 ms
    ZB_SCAN_DURATION_3,     ///<   153.6 ms
    ZB_SCAN_DURATION_4,     ///<   307.2 ms
    ZB_SCAN_DURATION_5,     ///<   614.4 ms
    ZB_SCAN_DURATION_6,     ///<   1.23 sec
    ZB_SCAN_DURATION_7,     ///<   2.46 sec
    ZB_SCAN_DURATION_8,     ///<   4.92 sec
    ZB_SCAN_DURATION_9,     ///<   9.83 sec
    ZB_SCAN_DURATION_10,    ///<   19.66 sec
    ZB_SCAN_DURATION_11,    ///<   39.32 sec
    ZB_SCAN_DURATION_12,    ///<   78.64 sec
    ZB_SCAN_DURATION_13,    ///<   157.28 sec
    ZB_SCAN_DURATION_14,    ///<   314.57 sec
}ZB_SCAN_DURATION_T;

/**
 * @enum Contains all of Zigbee's network states
 */
typedef enum {
    NET_IDLE = 0,           ///< inner using
    NET_POWER_ON_LEAVE,     ///< power on and device is not joined network
    NET_POWER_ON_ONLINE,    ///< power on and device is already joined network
    NET_JOIN_START,         ///< start joining network
    NET_JOIN_TIMEOUT,       ///< network joining timeout
    NET_JOIN_OK,            ///< network joined success
    NET_LOST,               ///< network lost
    NET_REJOIN_OK,          ///< network rejoin ok
    NET_REMOTE_LEAVE,       ///< remove device by remote device
    NET_LOCAL_LEAVE,        ///< remove device by local
    NET_MF_TEST_LEAVE,      ///< remove device by PC test tools
    NET_ZLL_JOINED,         ///< network joined zll network
    NET_ZLL_LEAVE,          ///< remove device Zll Reset To Factory New
}NET_EVT_T;

/**
 * @enum Contains the sent state
 */
typedef enum {
    SEND_ST_OK = 0,         ///< Successful delivery
    SEND_ST_ERR,            ///< Transmission failed due to network failure
    SEND_ST_MEMORY_ERR,     ///< Send failed due to memory error
    SEND_ST_QUEUE_FULL_ERR, ///< Send failed because the queue was full
    SEND_ST_NO_NETWORK,     ///< Sending failed due to no network
    SEND_ST_ADDR_MODE_ERR,  ///< Delivery failed due to address error
}SEND_ST_T;

/**
 * @enum Contains the message source address type
 */
typedef enum {
    MSG_SRC_GW_UNICAST = 0, ///< Gateway/coordinator unicast address
    MSG_SRC_GW_BROADCAST,   ///< Gateway/coordinator broadcast address
    MSG_SRC_DEV_UNICAST,    ///< device unicast address
    MSG_SRC_DEV_BROADCAST,  ///< device broadcast address
    MSG_SRC_MULTICAST,      ///< device/coordinator multicast address
}MSG_SRC_T;

/**
 * @enum Contains the result of the receive processing. dev_msg_recv_callback using
 */
typedef enum {
    ZCL_CMD_RET_SUCCESS = 0, ///< This means that the command has been processed correctly and no default ACK is required
    ZCL_CMD_RET_FAILED,      ///< This means that the command is not processed and requires a default ACK
}ZCL_CMD_RET_T;

/**
 * @enum The result of the read and write attribute
 */
typedef enum {
    ATTR_CMD_RET_SUCCESS = 0, ///< Read or write attribute successful
    ATTR_CMD_RET_FAILED,      ///< Read or write attribute failure
}ATTR_CMD_RET_T;


/**
 * @enum Contains all the states of the OTA
 */
typedef enum {
    ZG_OTA_EVT_DOWNLOAD_AND_VERIFY_SUCCESS = 0, ///< OTA successful
    ZG_OTA_EVT_DOWNLOAD_TIME_OUT           = 1, ///< OTA timeout
    ZG_OTA_EVT_VERIFY_FAILED               = 2, ///< OTA image validation failed
    ZG_OTA_EVT_SERVER_ABORTED              = 3, ///< An OTA server side exception
    ZG_OTA_EVT_CLIENT_ABORTED              = 4, ///< An OTA client side exception
    ZG_OTA_EVT_ERASE_FAILED                = 5, ///< Erase flash exceptions
    ZG_OTA_EVT_START                       = 6, ///< OTA start
}ZG_OTA_EVT_T;

/**
 * @enum The reliable behavior of sending data
 */
typedef enum {
    QOS_0 = 0, ///< There is no retransmission mechanism
    QOS_1,     ///< There is retransmission mechanism
    QOS_VIP_0, ///< Data is sent first, there is no retransmission mechanism
    QOS_VIP_1, ///< Data is sent first, there is retransmission mechanism
}SEND_QOS_T;

/**
 * @enum Contains the Zigbee address pattern
 */
typedef enum {
    SEND_MODE_GW = 0,   ///< Send to gateway/coordinator, No short address is required
    SEND_MODE_GROUP,    ///< Send a multicast message, group address is required
    SEND_MODE_BINDING,  ///< Sent according to the binding table, No short address is required
    SEND_MODE_DEV,      ///< Send to device, short address is required
}SEND_MODE_T;

/**
 * @enum Type of ZCL Zigbee command
 */
typedef enum {
    ZCL_COMMAND_GLOBAL_CMD = 0,   ///< A ZCL global command
    ZCL_COMMAND_CLUSTER_SPEC_CMD, ///< A ZCL cluster specific command
}ZCL_COMMAND_TYPE_T;

/**
 * @enum Direction of the ZCL command
 */
typedef enum {
    ZCL_DATA_DIRECTION_CLIENT_TO_SERVER = 0, ///< Server to client
    ZCL_DATA_DIRECTION_SERVER_TO_CLIENT,     ///< Client to server
}ZCL_DATA_DIRECTION_T;

/**
 * @enum Way to clear the send queue
 */
typedef enum {
    ZG_CLEAR_ALL_DATA = 0,  ///< Clear all queue data
    ZG_CLEAR_LATEST_DATA,   ///< Clear the lastest data
    ZG_CLEAR_ALL_SAME_DATA, ///< Clear all the same data
    ZG_CLEAR_ALL_ZCL_ID,    ///< Clear all the data with the same ZCL ID
}ZG_CLEAR_TYPE_T;

/**
 * @enum The type of scenario data
 */
typedef enum {
    SCENE_DATA_TYPE_SERVER_DEFINE = 0, ///< The format defined by TUYA server
    SCENE_DATA_TYPE_USER_DEFINE,       ///< The format defined by user
    SCENE_DATA_TYPE_ZIGBEE_DEFINE,     ///< The format defined by Zigbee standard 
}SCENE_DATA_TYPE_T;
    
/**
 * @enum The direction of the report table
 */
typedef enum {
    ZCL_REPORTING_DIRECTION_REPORTED = 0x00, ///< The direction of active reporting
    ZCL_REPORTING_DIRECTION_RECEIVED = 0x01, ///< The direction of receiving data
} ZCL_REPORTING_DIRECTION_T;

/**
 * @enum Define whether cluster is server or client
 */
typedef enum {
    ZCL_CLUSTER_MASK_SERVER = 0x40, ///< A server cluster
    ZCL_CLUSTER_MASK_CLIENT = 0x80, ///< A client cluster
} ZCL_CLUSTER_MASK_T;

/**
 * @enum Results of finding and binding
 */
typedef enum {
    FIND_BIND_ST_OK = 0, ///< Finding and binding was successful
    FIND_BIND_ST_ERR,    ///< Finding and binding failed
}FIND_BIND_ST_T;

/**
 * @enum Topology types of Zigbee networks
 */
typedef enum {
    ZG_JOIN_TYPE_CENTRALIZED = 0, ///< centralized join to gateway
    ZG_JOIN_TYPE_DISTRIBUTE,      ///< distribute join to router
    ZG_JOIN_TYPE_NO_NETWORK,       ///< no network
}ZG_JOIN_TYPE_T;

/**
 * @enum gateway types of Zigbee networks
 */
typedef enum {
    ZG_JOIN_GW_TYPE_TUYA = 0,    ///< tuya gateway
    ZG_JOIN_GW_TYPE_OTHERS,      ///< others gateway
    ZG_JOIN_GW_TYPE_UNKNOWN,     ///< no network or distribute network
}ZG_JOIN_GW_TYPE_T;

/**
 * @enum Standard definition for Zigbee version
 */
typedef enum {
    DEV_VER_ZHA = 0x00, ///< Zigbee HA
    DEV_VER_ZG30,       ///< Zigbee3.0
    DEV_VER_ZLL,        ///< Zigbee Light link
}DEV_VER_T;

/**
 * @enum Zigbee binding types
 */
typedef enum {
    DEV_BIND_TYPE_UNUSED         = 0, ///< A binding that is currently not in use.
    DEV_BIND_TYPE_UNICAST        = 1, ///< A unicast binding whose 64-bit identifier is the destination EUI64.
    DEV_BIND_TYPE_MANY_TO_ONE    = 2, ///< A unicast binding whose 64-bit identifier is the many-to-one destination EUI64.  Route discovery should be disabled when sending unicasts via many-to-one bindings.
    DEV_BIND_TYPE_MULTICAST      = 3, ///< A multicast binding whose 64-bit identifier is the group address. This binding can be used to send messages to the group and to receive messages sent to the group.
}DEV_BIND_TYPE_T;

/**
 * @enum the type of factory data reset
 */
typedef enum {
    DEV_RESET_TYPE_TO_FACTORY_ONLY = 0,  ///< Restore factory Settings, not LEAVE
    DEV_RESET_TYPE_TO_FACTORY_AND_LEAVE, ///< Restore factory Settings, and LEAVE
}DEV_RESET_TYPE_T;

/**
 * @enum The message type of the heartbeat
 */
typedef enum {
    APP_VERSION = 0, ///< Reported version number
    BATTERY_VOLTAGE, ///< Report battery information
}HEARTBEAT_TYPE_E;

/**
 * @enum Zigbee device role ID
 */
typedef enum {
    ZG_ROUTER = 0,          ///< Zigbee router device
    ZG_END_DEVICE,          ///< Zigbee end device
    ZG_SLEEPY_END_DEVICE,   ///< Zigbee sleepy end device 
}ZG_DEV_T;



typedef struct {
    uint32_t next_rejoin_time;          ///< next rejoin time when rejoin failed. (bet:ms)
    uint32_t wake_up_time_after_join;   ///< wakeup time when join success.(for gateway get cluser data) (bet:ms)
    uint32_t wake_up_time_after_rejoin; ///< wakeup time when rejoin success.(for app use) (bet:ms)
    uint8_t  rejoin_try_times;          ///< rejoin times when parent lost (bet:ms)
    bool_t   power_on_auto_rejoin_flag; ///< auto rejoin when power on
    bool_t   auto_rejoin_send_data;     ///< auto rejoin when send data if network status is parent-lost.
}zg_rejoin_config_t;

typedef struct {
    uint16_t poll_interval;     ///< poll period (bet: ms)
    uint16_t wait_app_ack_time; ///< tiemout waitting app ack (bet: ms)
    bool_t   poll_forever_flag; ///< TRUE: forever poll 
    uint16_t  poll_failed_times; ///< enter parent-lost status when poll failed x times.
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



typedef struct {
    ZG_DEV_T dev_type;
    union{
        zg_sleep_end_device_config  sleep_dev_cfg;
        zg_end_device_config        end_dev_cfg;
        zg_router_config            router_cfg;
    }config;
    uint16_t            beacon_send_interval_for_join;   ///< The interval of the beacon request for join. unit: milliseconds
    uint16_t            beacon_send_interval_for_rejoin; ///< The interval of the beacon request for rejoin. unit: milliseconds
    ZB_SCAN_DURATION_T  zb_scan_duration;
}zg_dev_config_t;


    

typedef struct {
    bool_t auto_join_power_on_flag;     ///< Whether the system automatically tries to network after starting
    bool_t auto_join_remote_leave_flag; ///< After being deleted by the APP, whether the network automatically
    uint32_t join_timeout; ///< Maximum timeout for networking. unit:ms
}join_config_t;

typedef struct {
    uint16_t attr_id;        ///< Attributes ID
    ATTR_TYPE_T type;        ///< Attributes data type
    uint8_t attr_value_size; ///< Length of attributes value
    uint8_t attr_mask;       ///< Attributes MASK(Refer to ATTR_MASK_CLIENT)
    void *default_value;     ///< The default value of Attribute
}attr_t;

typedef struct {
    CLUSTER_ID_T cluster_id; ///< Zigbee cluster ID
    attr_t *attr_list;       ///< A list of attributes definitions
    uint8_t  attr_sums;      ///< How many attributes does a cluster have
}cluster_t;

typedef struct {
  uint8_t  ep;
  PROFILE_ID_T profile_id;
  DEV_ID_T     dev_id;
  uint8_t         server_cluster_sums; ///< The number of clusters on the server side
  cluster_t *server_cluster_list;      ///< server cluster
  uint8_t         client_cluster_sums; ///< The number of clusters on the client side
  cluster_t *client_cluster_list;      ///< client cluster
} dev_description_t;


typedef struct {
    uint16_t profile_id;
    uint16_t zg_device_id;
    uint8_t  endpoint;
    uint8_t  version;
}dev_ext_zg_info_t;

typedef struct {
    uint8_t original_ep;
    uint8_t alias_ep1;
    uint8_t alias_ep2;
    uint8_t alias_ep3;
}dev_ep_alias_t;



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



typedef struct {
    /*real send time delay is equal to delay_time+rand()%random_time*/
    uint16_t delay_time; ///<send delay time with ms
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



typedef void (*send_result_func_t)(SEND_ST_T, dev_send_data_t *);

#define SCENES_MAX_DATA_LEN   128


typedef struct {
    SCENE_DATA_TYPE_T type;
    uint8_t data_len;
    uint8_t data[SCENES_MAX_DATA_LEN];
}scene_save_data_t;

typedef struct {
    ZCL_REPORTING_DIRECTION_T direction;
    uint8_t endpoint;
    CLUSTER_ID_T cluster;
    uint16_t     attribute;
    ZCL_CLUSTER_MASK_T mask;
    uint16_t manufacturer_code;
    union {
        struct {
            uint16_t min_interval;
            uint16_t max_interval;
            uint32_t reportable_change;
        } reported;
        struct {
            uint16_t source_addr;
            uint8_t endpoint;
            uint16_t timeout;
        } received;
    } data;
} zg_report_table_t;

typedef struct {
    uint8_t dest_endpoint;
    uint16_t cluster;
    ZCL_DATA_DIRECTION_T direction;
    uint8_t zcl_id;
    uint8_t cmd;
    uint8_t args_len;
    void *args;
}ext_plugin_args_t;

typedef void (*ext_plugin_cmd_callback_func_t)(ext_plugin_args_t *args);
typedef struct {
    uint16_t cluster_id;
    ext_plugin_cmd_callback_func_t func;
}ext_plugin_cmd_callback_struct_t;


typedef struct {
    uint16_t pan_id;
    uint16_t short_addr;
    uint8_t  channel;
    int8_t   tx_power_db;
    uint8_t  ext_pan_id[8];
}zg_info_t;

typedef struct{
    uint8_t long_addr[8];
    uint16_t short_addr;
    ZG_DEV_T dev_type;
    uint8_t ep_num;
    uint8_t ep_val[16];
}zll_device_info_t;


typedef void (*find_bind_callback_t)(FIND_BIND_ST_T st);
typedef void (*find_nwk_addr_callback_t)(uint16_t nwk_addr);
typedef void (*find_match_dev_callback_t)(uint16_t nwk_addr, uint8_t endpoint);


typedef struct {
    DEV_BIND_TYPE_T type;
    uint8_t local;
    uint16_t cluster_id;
    uint8_t remote;
    uint8_t identifier[8];
}dev_bind_table_entry;



///@name Zigbee basic information configuration
/**
 * @note Register tuya-related information to the SDK, the SDK will be automatically called, you do not need to use
 * @param[in] {model_id} modle id attribute of basic cluster
 * @param[in] {pid_prefix} manufacture name attribute(0-8bytes) of basic cluster
 * @param[in] {pid} manufacture name attribute(9-16bytes) of basic cluster
 * @return none
 */
extern void dev_register_base_info(char *model_id, char *pid_prefix, char *pid);

/**
 * @note Zigbee endpoint information register function
 * @param[in] {ep_desc} endpoint information description, include profileid, 
 * deviceid, in and out clusters
 * @param[in] {ep_sums} totals endpoints
 * @return none
 */
extern void dev_register_zg_ep_infor(dev_description_t *ep_desc, uint8_t ep_sums);

/**
 * @note: setting alias endpoint for a endpoint.
 * @param[in] {dev_ep_alias_t} Add the list of endpoint aliases
 * @return: {bool_t} \n 
 * TRUE for successful\n
 * FALSE for failed
 */
extern bool_t dev_set_endpoint_alias(dev_ep_alias_t *alias_item);

/**
 * @note: Added multi-group matching information.The ZLL network is currently in use.
 * @param[in] {p_zg_info} Refer to dev_ext_zg_info_t for detail.
 * @param[in] {sums} Number of p_zg_info structures
 * @return: {bool_t} \n 
 * TRUE for successful\n
 * FALSE for failed
 */
extern bool_t dev_set_ext_zg_info(dev_ext_zg_info_t *p_zg_info, uint8_t sums);

/**
 * @note: Zigbee endpoint enable disable
 * @param[in] {endpint} endpoint number
 * @param[in] {enable} TRUE: enable,  FALSE: disable
  * @return none
 */
extern void dev_endpint_enable_disable(uint8_t endpoint, bool_t enable);

/**
 * @note Zigbee device information register funciton
 * @param[in] {config} Device configuratin, inculde device type, join  mechanism ETC
 * @return none
 */
extern void dev_register_zg_dev_config(zg_dev_config_t *config);

/**
 * @note Network join information configuration
 * @param[in] {cfg} network join struct
 * @return none
 */
extern void dev_zg_join_config(join_config_t *cfg);



///@name Zigbee network control
/**
 * @note Zigbee device start to join network
 * @param[in] {join_timeout} join timeout ticks, uint is ms
 * @return {bool_t} \n 
 * TRUE for successful\n
 * FALSE for failed
 */
extern bool_t dev_zigbee_join_start(uint32_t join_timeout);

/**
 * @note Stop join, throw JOIN_START_ITMEOUT to nwk_state_changed_callback
 * @param none
 * @return none
 */
extern void dev_zigbee_join_stop(void);

/**
 * @note Enable permit join after steering finish. 
 * @param none
 * @return none
 */
extern void dev_zg_enable_steering_join_permit(void);


/**
 * @note disable self recovery. NOTE: call this function after dev_zigbee_join_start()
 * @param none
 * @return none
 */
extern void nwk_disable_self_recovery_once(void);


/**
 * @note enable self recovery. NOTE: call this function after join timeout.
 * @param[in] {type} none
 * @return none
 */
extern void nwk_enable_self_recovery_once(void);

/**
 * @note self recovery. NOTE: call this function after join timeout
 * @param[in]  none
 * @return none
 */
extern void nwk_self_recovery_manual(void);

/**
 * @note device leave by user
 * @param none
 * @return none
 */
extern void dev_zigbee_leave_for_user(void);

/**
 * @note rejoin scan channel config
 * @param[in] {type} poll interval time
 * @return none
 */
extern void zg_rejoin_scan_policy(ZG_SCAN_POLICY_T type);

/**
 * @note change poll interval time(ms)
 * @param[in] {poll_interval} poll interval time
 * @return none
 */
extern void zg_poll_interval_change(uint16_t poll_interval);

/**
 * @note start poll manual. POLL will be send forever.
 * @param none
 * @return none
 */
extern void zg_poll_start(void);

/**
 * @note stop poll manual. the rest of POLL will be send.
 * @param none
 * @return none
 */
extern void zg_poll_end(void);

/**
 * @note stop poll manual. the rest of POLL will be clean.
 * @param none
 * @return none
 */
extern void zg_poll_clear(void);

/**
 * @note device rejoin now
 * @param none
 * @return none
 */
extern void zg_rejoin_manual(void);


///@name Zigbee sends data-related functions
/**
 * @note zigbee data send function
 * @param[in] {send_data} data information need to be send
 * @param[in] {fun} data send callback
 * @param[in] {send_timeout} data send timeout
 * @return none
 */
extern void dev_zigbee_send_data(dev_send_data_t *send_data, send_result_func_t fun, uint32_t send_timeout);

/**
 * @note clear zigbee data waitting send.
 * @param[in] {type} enum
 * @param[in] {args} reserved
 * @return none
 */
extern void dev_zigbee_clear_send_data(ZG_CLEAR_TYPE_T type, void *args);
/**
 * @note clear sepcific cmd send queue,
 * @param[in] {zcl_id} zcl serial number.
 * @param[in] {dest_ep} destination endpoint
 * @param[in] {cluster_id} specific cluster id
 * @return none
 */
extern void dev_zigbee_specific_response_handle(uint8_t zcl_id, uint8_t dest_ep, uint16_t cluster_id);

/**
 * @note device heartbeat type and duration set
 * @param[in] {type} heart type, app version attribute or power percentage attribute 
 * @param[in] {duration} heartbeat send duration
 * @return none
 */
extern bool_t dev_heartbeat_set(HEARTBEAT_TYPE_E type, uint32_t duration);

/**
 * @note delay bearbeat by user call
 * @param[in] {delay_time} delay time(ms)
 * @return none
 */
extern void dev_heartbeat_delay_ms(uint32_t delay_time);

/**
 * @note Enable/Disable OTA function
 * @param[in] {en_flag} \n
 * TRUE: enable
 * FALSE: disable
 * @return none
 */
extern void zg_ota_enable_disable(bool_t en_flag);


///@name Zigbee TX power control
/**
 * @note change dev tx power
 * @param[in] {normal_db} tx power when link is normal 
 * @param[in] {max_db} tx power when link is not normal
 * @return none
 */
extern void dev_change_power(int8_t normal_db, int8_t max_db);

/**
 * @note disable gateway change tx power.
 * @param none
 * @return none
 */
extern void disable_gw_change_power(void);


///@name Zigbee attributes access
/**
 * @note zigbee attribute write function
 * @param[in] {endpoint} endpoint of the attribute
 * @param[in] {cluster} cluster of the attribute
 * @param[in] {attr_id} attribute id
 * @param[in] {data} attribute data to be write
 * @param[in] {data_type} attribute datatype to be write
 * @return {ATTR_CMD_RET_T} return write status
 */
extern ATTR_CMD_RET_T dev_zigbee_write_attribute(
    uint8_t endpoint,
    CLUSTER_ID_T cluster,
    uint16_t attr_id, //attribute-id.h
    void* data,
    ATTR_TYPE_T dataType
);

/**
 * @note zigbee attribute read function
 * @param[in] {endpoint} endpoint of the attribute
 * @param[in] {cluster} cluster of the attribute
 * @param[in] {attr_id} attribute id
 * @param[out] {data} attribute data read result
 * @param[in] {read_length} attribute datalength to be read
 * @return {ATTR_CMD_RET_T} return read status
 */
extern ATTR_CMD_RET_T dev_zigbee_read_attribute(
    uint8_t endpoint,
    CLUSTER_ID_T cluster,
    uint16_t attr_id, //attribute-id.h
    void *data,
    uint8_t read_length
);


///@name Zigbee scenarios
/**
 * @note  scene cluster view valid callback, this function is used to check if the secne and group of the endpoint is exist 
 * @param[in] {endpoint} endpoint
 * @param[in] {groupId} group id
 * @param[in] {sceneId} scene id
 * @return {bool_t} \n
 * TRUE: exist \n
 * FALSE: not exist
 */
extern bool_t devViewSceneValidCallback(uint16_t endpoint, uint16_t groupId, uint8_t sceneId);

/**
 * @note this function is used to remove the scene info from scene table of the endpoint
 * @param[in] {endpoint} endpoint
 * @param[in] {groupId} group id
 * @param[in] {sceneId} scene id
 * @return {bool_t} \n
 * TRUE: exist \n
 * FALSE: not exist
 */
extern bool_t devScenesClusterRemoveSceneCallback(uint16_t endpoint, uint16_t groupId, uint8_t sceneId);

/**
 * @note this function is used to get the scene info from scene table of the endpoint
 * @param[in] {endpoint}   endpoint
 * @param[out] {groupId} group id
 * @param[out] {sceneId} scene id
 * @return {bool_t} \n
 * TRUE: exist \n
 * FALSE: not exist
 */
extern bool_t devGetSceneCallback(uint16_t endpoint, uint16_t* groupId, uint8_t* sceneId);

/**
 * @note remove all scenes in scene table of current endpoint; just used by scene control device; default is not remove
 * @param[in] {endpoint} endpoint
 * @return {bool_t}\n
 * TRUE: remove all \n
 * FALSE:not remove
 */
extern bool_t zigbee_sdk_scene_remove_before_add(uint8_t endpoint);

/**
 * @note scene recall command send
 * @param[in] {endpoint} src endpoint of recall scene
 * @param[in] {groupId} group id
 * @param[in] {sceneId} scene id
 * @return {bool_t} \n
 * TRUE: send ok
 * FALSE: send fail
 */
extern bool_t dev_scene_recall_send_command(uint8_t endpoint, uint16_t groupId, uint8_t sceneId);


///@name Function to get Zigbee information
/**
 * @note device mac address get function
 * @param[out] {returnEui64} 8 bytes mac address
 * @return none
 */
extern void device_mac_get(Device_Mac_64 returnEui64);

/**
 * @note Get join type types of Zigbee networks
 * @param[in]  none
 * @return {ZG_JOIN_TYPE_T}
 */
extern ZG_JOIN_TYPE_T zg_get_join_type(void);

/**
 * @note Get gateway type of Zigbee networks
 * @param[in]  none
 * @return {ZG_JOIN_GW_TYPE_T}
 */
extern ZG_JOIN_GW_TYPE_T zg_get_join_gw_type(void);

/**
 * @note Determine if the device is joined
 * @param none
 * @return {bool_t}\n
 * TRUE: joined.\n
 * FALSE no network.
 */
extern bool_t zg_is_join(void);

/**
 * @note get zigbee dev network status information
 * @param[out] {out_zg_info} output information
 * @return {bool_t} \n
 * TRUE if ok. \n
 * FLASE else.
 */
extern bool_t zigbee_get_net_info(zg_info_t *out_zg_info);

/**
 * @note network state get 
 * @param[in] {type} none
 * @return: {NET_EVT_T} \n
 * current network state
 */
extern NET_EVT_T nwk_state_get(void);


///@name Zigbee3.0 related features
/**
 * @note The report table is initialized
 * @param[in] {report_config} The basic elements of the report table
 * @param[in] {report_config_sums} Number of report tables
 * @return {bool_t}\n
 * TRUE for successful\n
 * FALSE for failed
 */
extern bool_t zg_report_table_init(zg_report_table_t *report_config, uint8_t report_config_sums);

/**
 * @note Enable the identify function for client
 * @param none
 * @return none
 */
extern void ext_plugin_identify_client_enable(void);

/**
 * @note Enable the identify function for server
 * @param none
 * @return none
 */
extern void ext_plugin_identify_server_enable(void);

/**
 * @note Enable the Green Power function for client
 * @param none
 * @return none
 */
extern void ext_plugin_green_power_client_enable(void);

/**
 * @note Enable reporting. Report the data according to the report table.
 * @param none
 * @return none
 */
extern void ext_plugin_reporting_enable(void);

/**
 * @note Enable the identify function for client
 * @param[in] {list} Register cluster-related callback functions
 * @param[in] {list_sums} The number of list structures
 * @return none
 */
extern bool_t ext_plugin_register_cmd_handle(ext_plugin_cmd_callback_struct_t *list, uint8_t list_sums);

/**
 * @note Enable/Disable automatic OTA function
 * @param[in] {enable} \n
 * TRUE: enable
 * FALSE: disable
 * @return none
 */
extern void zg_ota_auto_start_config(bool_t enable);

/**
 * @note Add a record
 * @param[in] {bind_entry} binding table record
 * @return {bool_t} \n
 * TRUE for successful\n
 * FALSE for failed
 */
extern bool_t dev_bind_table_add(dev_bind_table_entry *bind_entry);

/**
 * @note Delete a record
 * @param[in] {bind_entry} binding table record
 * @return {bool_t} \n
 * TRUE for successful\n
 * FALSE for failed
 */
extern bool_t dev_bind_table_del(dev_bind_table_entry *bind_entry);

/**
 * @note Query short addresses with MAC address.
 * @param[in] {mac} Zigbee MAC address
 * @param[in] {func} Call this function when the lookup succeeds
 * @return none
 */
extern void zg_find_nwk_addr(uint8_t *mac, find_nwk_addr_callback_t func);

/**
 * @note Start the Zigbee cluster match process
 * @param[in] {dest_addr} Destination address
 * @param[in] {profile_id} Zigbee profile
 * @param[in] {cluster_id} Zigbee cluster
 * @param[in] {is_server_cluster} Zigbee cluster type(server or client)
 * @param[in] {func} This function is called when the match is successful
 * @return none
 */
extern void zg_match_dev_by_profile_cluster(uint16_t dest_addr, 
                                                    PROFILE_ID_T profile_id,
                                                    CLUSTER_ID_T cluster_id,
                                                    bool_t is_server_cluster,
                                                    find_match_dev_callback_t func);

/**
 * @note Start the process of finding and binding
 * @param[in] {endpoint} Zigbee endpoint
 * @return none
 */
extern void zg_find_bind_start(uint8_t endpoint);

/**
 * @note Start the process of finding and binding
 * @param[in] {endpoint} Zigbee endpoint
 * @param[in] {func} This function is called when the finding and binding ends
 * @return none
 */
extern void zg_find_bind_start_with_callback(uint8_t endpoint, find_bind_callback_t func);


///@name Zigbee light link related features
/**
 * @note device create distributed network
 * @param none
 * @return {bool_t} 
 * TRUE:success 
 * FALSE: failed
 */
extern bool_t zll_create_distributed_network(void);

/**
 * @note ZLL touch link scan 
 * @param none
 * @return none
 */
extern void zll_touch_link_scan(void);

/**
 * @note ZLL device information request
 * @param none 
 * @return none
 */
extern void zll_device_information_request(void);

/**
 * @note ZLL identify request
 * @param none 
 * @return none
 */
extern void zll_identify_request(void);

/**
 * @note ZLL reset to factory new request
 * @param none 
 * @return none
 */
extern void zll_reset_to_factory_new_request(void);

/**
 * @note Determine whether it is a ZLL network
 * @param none 
 * @return {bool_t} \n
 * TRUE: ZLL network
 * FALSE: no ZLL network
 */
extern bool_t zg_is_zll_net(void); //for zll lib





///@name Zigbee SDK patch function
/**
 * @note Fixed lost PID problem
 * @param[in] {pid_header} PID prefix
 * @param[in] {pid_header_len} PID prefix length
 * @param[in] {pid_len} PID length
 * @return {bool_t}\n
 * TRUE for successful\n
 * FALSE for failed
 */
extern bool_t tuya_sdk_patch_for_pid(char *pid_header, uint8_t pid_header_len, uint8_t pid_len);

#ifdef __cplusplus
}
#endif

#endif

