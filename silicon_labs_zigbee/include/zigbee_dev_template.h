/*
 * @Author: Leon
 * @email: zhangpeng@tuya.com
 * @LastEditors: Leon
 * @file name: zigbee_dev_template.h
 * @Description: zigbee device cluster and attribute template files
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-03-25 21:41:18
 * @LastEditTime: 2019-04-15 20:24:42
 */

#ifndef ZIGBEE_DEV_TEMPLATE__H
#define ZIGBEE_DEV_TEMPLATE__H

#ifdef __cplusplus
extern "C" {
#endif

#include "zigbee_sdk.h"

extern const uint8_t basic_default_value[];

//attributes define
#define BASE_ATTR_LIST \
    { 0x0000, ATTR_INT8U_ATTRIBUTE_TYPE, 1, (ATTR_MASK_SINGLETON),  (uint8_t*)0x03 }, /* 0 / Basic / ZCL version*/\
    { 0x0001, ATTR_INT8U_ATTRIBUTE_TYPE, 1, (ATTR_MASK_SINGLETON),  (uint8_t*)0x00  }, /* 1 / Basic / application version*/\
    { 0x0002, ATTR_INT8U_ATTRIBUTE_TYPE, 1, (ATTR_MASK_SINGLETON),  (uint8_t*)0x00  }, /* 2 / Basic / stack version*/\
    { 0x0003, ATTR_INT8U_ATTRIBUTE_TYPE, 1, (ATTR_MASK_SINGLETON),  (uint8_t*)0x00  }, /* 3 / Basic / hardware version*/\
    { 0x0004, ATTR_CHAR_STRING_ATTRIBUTE_TYPE, 33, (ATTR_MASK_TOKENIZE|ATTR_MASK_SINGLETON),  NULL  }, /* 4 / Basic / manufacturer name*/\
    { 0x0005, ATTR_CHAR_STRING_ATTRIBUTE_TYPE, 33, (ATTR_MASK_SINGLETON),  NULL  }, /* 5 / Basic / model identifier*/\
    { 0x0006, ATTR_CHAR_STRING_ATTRIBUTE_TYPE, 17, (ATTR_MASK_SINGLETON),  NULL  }, /* 6 / Basic / date code*/\
    { 0x0007, ATTR_ENUM8_ATTRIBUTE_TYPE, 1, (ATTR_MASK_SINGLETON),  (uint8_t*)0x01  }, /* 7 / Basic / power source*/\
    { 0xFFDE, ATTR_INT8U_ATTRIBUTE_TYPE, 1, (ATTR_MASK_SINGLETON | ATTR_MASK_WRITABLE),  (uint8_t*)13  }, /* 8 / basic / tx power cfg*/\
    { 0xFFFD, ATTR_INT16U_ATTRIBUTE_TYPE, 2, (ATTR_MASK_SINGLETON),  (uint8_t*)0x0001  }, /* 8 / Basic / cluster revision*/\
    { 0xFFFE, ATTR_ENUM8_ATTRIBUTE_TYPE, 1, (ATTR_MASK_SINGLETON),  (uint8_t*)0x00  },/*9*/\

#define POWER_ATTR_LIST \
    { 0x0000, ATTR_INT16U_ATTRIBUTE_TYPE, 2, (ATTR_MASK_SINGLETON),  (uint8_t*)0x0000UL }, /* 10 / Power Configuration / mains voltage*/\
    { 0x0020, ATTR_INT8U_ATTRIBUTE_TYPE, 1, (ATTR_MASK_SINGLETON),  (uint8_t*)0x00UL  }, /* 11 / Power Configuration / battery voltage*/\
    { 0x0021, ATTR_INT8U_ATTRIBUTE_TYPE, 1, (ATTR_MASK_SINGLETON),  (uint8_t*)0x00 } , /* 12 / Power Configuration / battery percentage remaining*/\
    { 0xFFFD, ATTR_INT16U_ATTRIBUTE_TYPE, 2, (ATTR_MASK_SINGLETON),  (uint8_t*)0x0001  }, /* 13 / Power Configuration / cluster revision*/\


#define OTA_ATTR_LIST \
    { 0x0000, ATTR_IEEE_ADDRESS_ATTRIBUTE_TYPE, 8, (ATTR_MASK_CLIENT),  (uint8_t*)&(basic_default_value[0]) }, /* 24 / Over the Air Bootloading / OTA Upgrade Server ID*/\
    { 0x0001, ATTR_INT32U_ATTRIBUTE_TYPE, 4, (ATTR_MASK_CLIENT), (uint8_t*)&(basic_default_value[8]) }, /* 25 / Over the Air Bootloading / Offset (address) into the file*/\
    { 0x0006, ATTR_ENUM8_ATTRIBUTE_TYPE, 1, (ATTR_MASK_CLIENT), (uint8_t*)0x00 }, /* 26 / Over the Air Bootloading / OTA Upgrade Status*/\
    { 0xFFFD, ATTR_INT16U_ATTRIBUTE_TYPE, 2, (ATTR_MASK_CLIENT), (uint8_t*)0x0001 }, /* 27 / Over the Air Bootloading / cluster revision*/\

#define GROUP_ATTR_LIST \
    { 0x0000, ATTR_BITMAP8_ATTRIBUTE_TYPE, 1, (0x00), (uint8_t*)0x00 }, /* 12 / Groups / name support*/\
    { 0xFFFD, ATTR_INT16U_ATTRIBUTE_TYPE, 2, (0x00), (uint8_t*)0x0001 }, /* 13 / Groups / cluster revision*/\

#define SCENE_ATTR_LIST \
    { 0x0000, ATTR_INT8U_ATTRIBUTE_TYPE, 1, (ATTR_MASK_TOKENIZE), (uint8_t*)0x00 }, /* 15 / Scenes / scene count*/\
    { 0x0001, ATTR_INT8U_ATTRIBUTE_TYPE, 1, (ATTR_MASK_TOKENIZE), (uint8_t*)0x00 }, /* 16 / Scenes / current scene*/\
    { 0x0002, ATTR_INT16U_ATTRIBUTE_TYPE, 2, (ATTR_MASK_TOKENIZE), (uint8_t*)0x0000 }, /* 16 / Scenes / current group*/\
    { 0x0003, ATTR_BOOLEAN_ATTRIBUTE_TYPE, 1, (ATTR_MASK_TOKENIZE), (uint8_t*)0x00 }, /* 17 / Scenes / scene valid*/\
    { 0x0004, ATTR_BITMAP8_ATTRIBUTE_TYPE, 1, (ATTR_MASK_TOKENIZE), (uint8_t*)0x00 }, /* 18 / Scenes / name support*/\
    { 0xFFFD, ATTR_INT16U_ATTRIBUTE_TYPE, 2, (0x00), (uint8_t*)0x0001 }, /* 19 / Scenes / cluster revision*/\

#define SCENE_CLINET_ATTR_LIST \
    { 0xFFFD, ATTR_INT16U_ATTRIBUTE_TYPE, 2, (ATTR_MASK_CLIENT),  (uint8_t*)0x0001 }, /* 13 / Scenes client / cluster revision*/\

#define ON_OFF_LIGHT_ATTR_LIST \
    { 0x0000, ATTR_BOOLEAN_ATTRIBUTE_TYPE, 1, (ATTR_MASK_TOKEN_FAST),  (uint8_t*)0x00 }, /* 20 / On/off / on/off*/\

#define LEVEL_CONTROL_ATTR_LIST \
    { 0x0000, ATTR_INT8U_ATTRIBUTE_TYPE, 1, (ATTR_MASK_WRITABLE),  (uint8_t*)0x00 }, /* 20 / Level Control / Current Level*/\

#define TIME_ATTR_LIST \
    { 0x0007, ATTR_INT32U_ATTRIBUTE_TYPE, 4, (ATTR_MASK_SINGLETON), (uint8_t*)0x00000000 }, /* 1 / Time / Local time*/\

        
#define COLOR_CONTROL_ATTR_LIST \
    { 0x0000, ATTR_INT8U_ATTRIBUTE_TYPE, 1, (ATTR_MASK_WRITABLE), (uint8_t*)0x10 }, /* 41 / Color Control / current hue*/\
    { 0x0001, ATTR_INT8U_ATTRIBUTE_TYPE, 1, (ATTR_MASK_WRITABLE), (uint8_t*)0x10 }, /* 42 / Color Control / current saturation*/\
    { 0x0002, ATTR_INT16U_ATTRIBUTE_TYPE, 2, (0x00), (uint8_t*)0x0000 }, /* 43 / Color Control / remaining time*/\
    { 0x0003, ATTR_INT16U_ATTRIBUTE_TYPE, 2, (ATTR_MASK_WRITABLE), (uint8_t*)0x0000 }, /* 44 / Color Control / current x*/\
    { 0x0004, ATTR_INT16U_ATTRIBUTE_TYPE, 2, (ATTR_MASK_WRITABLE), (uint8_t*)0x0000 }, /* 45 / Color Control / current y*/\
    { 0x0007, ATTR_INT16U_ATTRIBUTE_TYPE, 2, (ATTR_MASK_WRITABLE), (uint8_t*)0x0000 }, /* 46 / Color Control / color temperature*/\
    { 0x0008, ATTR_ENUM8_ATTRIBUTE_TYPE, 1, (ATTR_MASK_WRITABLE), (uint8_t*)0x02 }, /* 47 / Color Control / color mode*/\


#define IAS_ZONE_ATTR_LIST \
    { 0x0000, ATTR_ENUM8_ATTRIBUTE_TYPE, 1, (0x00), (uint8_t*)0x00 }, /* 21 / IAS Zone / zone state*/\
    { 0x0001, ATTR_ENUM16_ATTRIBUTE_TYPE, 2, (ATTR_MASK_TOKENIZE), (uint8_t*)0x0028 }, /* 22 / IAS Zone / zone type*/\
    { 0x0002, ATTR_BITMAP16_ATTRIBUTE_TYPE, 2, (0x00), (uint8_t*)0x0000 }, /* 23 / IAS Zone / zone status*/\
    { 0x0010, ATTR_IEEE_ADDRESS_ATTRIBUTE_TYPE, 8, (ATTR_MASK_WRITABLE), NULL }, /* 24 / IAS Zone / IAS CIE address*/\
    { 0x0011, ATTR_INT8U_ATTRIBUTE_TYPE, 1, (0x00), (uint8_t*)0xff }, /* 25 / IAS Zone / Zone ID*/\
    { 0xFFFD, ATTR_INT16U_ATTRIBUTE_TYPE, 2, (0x00), (uint8_t*)0x0001 }, /* 26 / IAS Zone / cluster revision*/\

#define WINDOW_COVERING_ATTR_LIST \
    { 0x0008, ATTR_INT8U_ATTRIBUTE_TYPE, 1, (ATTR_MASK_TOKEN_FAST|ATTR_MASK_SINGLETON), (uint8_t*)0x00 }, /* current positiong lift percentage*/\
    { 0x0009, ATTR_INT8U_ATTRIBUTE_TYPE, 1, (ATTR_MASK_TOKEN_FAST|ATTR_MASK_SINGLETON), (uint8_t*)0x00 }, /* current positiong tilt percentage*/\
    { 0xFFFD, ATTR_INT16U_ATTRIBUTE_TYPE, 2, (0x00), (uint8_t*)0x0001 }, /* 26 / IAS Zone / cluster revision*/\

#define DOORLOCK_ATTR_LIST \
    { 0x0000, ATTR_ENUM8_ATTRIBUTE_TYPE, 1, (ATTR_MASK_SINGLETON), (uint8_t*)0x00 }, /* 23 / Door Lock / lock state*/\
    { 0x0001, ATTR_ENUM8_ATTRIBUTE_TYPE, 1, (ATTR_MASK_SINGLETON), (uint8_t*)0x00 }, /* 24 / Door Lock / lock type*/\
    { 0x0002, ATTR_BOOLEAN_ATTRIBUTE_TYPE, 1, (ATTR_MASK_SINGLETON), (uint8_t*)0x00 }, /* 25 / Door Lock / actuator enabled*/\
    { 0x0003, ATTR_ENUM8_ATTRIBUTE_TYPE, 1, (ATTR_MASK_SINGLETON), (uint8_t*)0x00 }, /* 26 / Door Lock / door state*/\
    { 0xFFFD, ATTR_INT16U_ATTRIBUTE_TYPE, 2, (0x00), (uint8_t*)0x0001 }, /* 27 / Door Lock / cluster revision*/\

#define SIMPLE_METERING_ATTR_LIST \
    { 0x0000, ATTR_INT48U_ATTRIBUTE_TYPE, 6, (ATTR_MASK_TOKEN_FAST), NULL }, /* 27 / Simple Metering / current summation delivered*/\
    { 0x0200, ATTR_BITMAP8_ATTRIBUTE_TYPE, 1, (0x00), (uint8_t*)0x00 }, /* 28 / Simple Metering / status*/\
    { 0x0300, ATTR_ENUM8_ATTRIBUTE_TYPE, 1, (0x00), (uint8_t*)0x00 }, /* 29/ Simple Metering / unit of measure*/\
    { 0x0301, ATTR_INT24U_ATTRIBUTE_TYPE, 3, (0x00), (uint8_t*)0x000001 }, /* 30 / Simple Metering / multiplier*/\
    { 0x0302, ATTR_INT24U_ATTRIBUTE_TYPE, 3, (0x00), (uint8_t*)0x000064 }, /* 31 / Simple Metering / divisor*/\
    { 0x0303, ATTR_BITMAP8_ATTRIBUTE_TYPE, 1, (0x00), (uint8_t*)0xC2 }, /* 32 / Simple Metering / summation formatting*/\
    { 0x0306, ATTR_BITMAP8_ATTRIBUTE_TYPE, 1, (0x00), (uint8_t*)0x00 }, /* 33 / Simple Metering / metering device type*/\
    { 0xFFFD, ATTR_INT16U_ATTRIBUTE_TYPE, 2, (0x00), (uint8_t*)0x0001 }, /* 34 / Simple Metering / cluster revision*/\

#define ELECTRICAL_MEASUREMENT_ATTR_LIST \
    { 0x0000, ATTR_BITMAP32_ATTRIBUTE_TYPE, 4, (0x00), (uint8_t*)0x00000105 }, /* 35 / Electrical Measurement / measurement type*/\
    { 0x0505, ATTR_INT16U_ATTRIBUTE_TYPE, 2, (0x00), (uint8_t*)0 }, /* 36 / Electrical Measurement / rms voltage*/\
    { 0x0508, ATTR_INT16U_ATTRIBUTE_TYPE, 2, (0x00), (uint8_t*)0 }, /* 37 / Electrical Measurement / rms current*/\
    { 0x050B, ATTR_INT16S_ATTRIBUTE_TYPE, 2, (0x00), (uint8_t*)0 }, /* 38 / Electrical Measurement / active power*/\
    { 0x0600, ATTR_INT16U_ATTRIBUTE_TYPE, 2, (0x00), (uint8_t*)0x01 }, /* 39 / Electrical Measurement / ac voltage multiplier*/\
    { 0x0601, ATTR_INT16U_ATTRIBUTE_TYPE, 2, (0x00), (uint8_t*)0x01 }, /* 40 / Electrical Measurement / ac voltage divisor*/\
    { 0x0602, ATTR_INT16U_ATTRIBUTE_TYPE, 2, (0x00), (uint8_t*)0x01 }, /* 41 / Electrical Measurement / ac current multiplier*/\
    { 0x0603, ATTR_INT16U_ATTRIBUTE_TYPE, 2, (0x00), (uint8_t*)1000 }, /* 42 / Electrical Measurement / ac current divisor*/\
    { 0x0604, ATTR_INT16U_ATTRIBUTE_TYPE, 2, (0x00), (uint8_t*)0x01 }, /* 43 / Electrical Measurement / ac power multiplier*/\
    { 0x0605, ATTR_INT16U_ATTRIBUTE_TYPE, 2, (0x00), (uint8_t*)0x01 }, /* 44 / Electrical Measurement / ac power divisor*/\
    { 0x0801, ATTR_INT16S_ATTRIBUTE_TYPE, 2, (0x00), (uint8_t*)250 }, /* 45 / Electrical Measurement / ac voltage overload*/\
    { 0x0802, ATTR_INT16S_ATTRIBUTE_TYPE, 2, (0x00), (uint8_t*)16 }, /* 46 / Electrical Measurement / ac current overload*/\
    { 0x0803, ATTR_INT16S_ATTRIBUTE_TYPE, 2, (0x00), (uint8_t*)4000 }, /* 47 / Electrical Measurement / ac active power overload*/\
    { 0xFFFD, ATTR_INT16U_ATTRIBUTE_TYPE, 2, (0x00), (uint8_t*)0x0001 }, /* 48 / Electrical Measurement / cluster revision*/\


#define PRIVATE_ATTR_LIST \
    { 0x0000, ATTR_INT8U_ATTRIBUTE_TYPE, 1, (ATTR_MASK_TOKEN_FAST|ATTR_MASK_SINGLETON), (uint8_t*)0x00 }, /* current positiong lift percentage*/\

   
#define DIMMER_ZLL_COMMISSIONING_ATTR_LIST \
    { 0xFFFD, ATTR_INT16U_ATTRIBUTE_TYPE, 2, (0x00), (uint8_t*)0x0001}, /* 1 / ZLL Commissioning / cluster revision*/\


//ZCL define
#define DEF_CLUSTER_BASIC_CLUSTER_ID(a) \
    { CLUSTER_BASIC_CLUSTER_ID, (attr_t *)&((a)[0]), get_array_len((a)) },
    
#define DEF_CLUSTER_POWER_CLUSTER_ID(a) \
    { CLUSTER_POWER_CONFIG_CLUSTER_ID, (attr_t *)&((a)[0]), get_array_len((a)) },

#define DEF_CLUSTER_OTA_BOOTLOAD_CLUSTER_ID(a) \
    { CLUSTER_OTA_BOOTLOAD_CLUSTER_ID, (attr_t *)&((a)[0]), get_array_len((a)) },

#define DEF_CLUSTER_GROUPS_CLUSTER_ID(a) \
    { CLUSTER_GROUPS_CLUSTER_ID, (attr_t *)&((a)[0]), get_array_len((a)) },

#define DEF_CLUSTER_SCENES_CLUSTER_ID(a) \
    { CLUSTER_SCENES_CLUSTER_ID, (attr_t *)&((a)[0]), get_array_len((a)) },

#define DEF_CLUSTER_ON_OFF_CLUSTER_ID(a) \
    { CLUSTER_ON_OFF_CLUSTER_ID, (attr_t *)&((a)[0]), get_array_len((a)) },

#define DEF_CLUSTER_LEVEL_CONTROL_CLUSTER_ID(a) \
    { CLUSTER_LEVEL_CONTROL_CLUSTER_ID, (attr_t *)&((a)[0]), get_array_len((a)) },

#define DEF_CLUSTER_TIME_CLUSTER_ID(a) \
    { CLUSTER_TIME_CLUSTER_ID, (attr_t *)&((a)[0]), get_array_len((a)) },
    
#define DEF_CLUSTER_COLOR_CONTROL_CLUSTER_ID(a) \
    { CLUSTER_COLOR_CONTROL_CLUSTER_ID, (attr_t *)&((a)[0]), get_array_len((a)) },
    
#define DEF_CLUSTER_IAS_ZONE_CLUSTER_ID(a) \
    { CLUSTER_IAS_ZONE_CLUSTER_ID, (attr_t *)&((a)[0]), get_array_len((a)) },

#define DEF_CLUSTER_WINDOW_COVERING_CLUSTER_ID(a) \
    { CLUSTER_WINDOW_COVERING_CLUSTER_ID, (attr_t *)&((a)[0]), get_array_len((a)) },

#define DEF_CLUSTER_DOOR_LOCK_CLUSTER_ID(a) \
    { CLUSTER_DOOR_LOCK_CLUSTER_ID, (attr_t *)&((a)[0]), get_array_len((a)) },

#define DEF_CLUSTER_SIMPLE_METERING_CLUSTER_ID(a) \
    { CLUSTER_SIMPLE_METERING_CLUSTER_ID, (attr_t *)&((a)[0]), get_array_len((a)) },

#define DEF_CLUSTER_ELECTRICAL_MEASUREMENT_CLUSTER_ID(a) \
    { CLUSTER_ELECTRICAL_MEASUREMENT_CLUSTER_ID, (attr_t *)&((a)[0]), get_array_len((a)) },

#define DEF_CLUSTER_PRIVATE_CLUSTER_ID(a) \
    { CLUSTER_PRIVATE_TUYA_CLUSTER_ID, (attr_t *)&((a)[0]), get_array_len((a)) },

#define DEF_CLUSTER_ZLL_COMMISSIONING_CLUSTER_ID(a) \
    { CLUSTER_ZLL_COMMISSIONING_CLUSTER_ID, (attr_t *)&((a)[0]), get_array_len((a)) },

    
#ifdef __cplusplus
}
#endif

#endif




