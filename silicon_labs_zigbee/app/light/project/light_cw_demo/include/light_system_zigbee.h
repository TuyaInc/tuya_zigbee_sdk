/*
 * @Author: jinlu
 * @email: jinlu@tuya.com
 * @LastEditors:   
 * @file name: light_init.c
 * @Description: light init process 
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-04-28 14:35:54
 * @LastEditTime: 2019-10-10 20:41:31
 */
#ifndef __LIGHT_SYSTEM_H__
#define __LIGHT_SYSTEM_H__

#include "light_types.h"
#include "zigbee_sdk.h"


//应用层的ZCL ID,用于每次上报数据之前清空待发送的相同ID的数据. 空出0~99留作他用
#define  ZCL_ID_ONOFF    100
#define  ZCL_ID_MODE     101
#define  ZCL_ID_BRIGHT   102
#define  ZCL_ID_TEMPER   103
#define  ZCL_ID_HSV      104
#define  ZCL_ID_HS       105
#define  ZCL_ID_H        106
#define  ZCL_ID_S        107
#define  ZCL_ID_COUNT_DOWN 108  
#define  ZCL_ID_APP_DATA 109
#define  ZCL_ID_SCENE    110
#define  ZCL_ID_SCENE_SYNC    111



#pragma pack(1)

typedef enum{
	ZIGBEE_CMD_SINGLE = 0,
	ZIGBEE_CMD_GROUP
}ZIGBEE_CMD_T;


typedef struct{
    uint8_t module_name_str[15];
    uint8_t module_name_len;
}module_name_t;

typedef struct{
    uint16_t H;
    uint16_t S;
    uint16_t V;
}app_hsv_t;

typedef struct{
    uint8_t h;
    uint8_t s;
    uint8_t v;
}zigbee_hsv_t;


typedef struct{
    uint8_t mode;
    uint8_t H;
    uint8_t S;
    uint8_t V;
    uint8_t B;
    uint16_t T;
}app_data_t;

typedef struct{
    SEND_QOS_T QOS;
    uint16_t delay_ms;
    CLUSTER_ID_T cluster;
    uint16_t attr_id;
}report_data_t;

#pragma 


BOOL_T bLightSysHWRebootJude(VOID);

user_uart_config_t* mf_test_uart_config(void);


#endif