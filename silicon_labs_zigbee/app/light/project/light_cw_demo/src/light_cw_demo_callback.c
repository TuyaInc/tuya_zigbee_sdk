/**************************************************************
 * @file common main file for light
 * @version 1.0.0
 * @author Jin Lu
 * @data 2019.04.26
 *************************************************************/
#include "stdio.h"
#include "stdlib.h"
#include "zigbee_dev_template.h"
#include "config.h"
#include "user_pwm.h"
#include "soc_timer.h"
#include "light_set_color.h"
#include "soc_flash.h"
#include "user_flash.h"
#include "user_timer.h"
#include "light_control.h"
#include "light_init.h"
#include "light_set_color.h"
#include "soc_gpio.h"

#define  ZCL_ID_ONOFF    100
#define  ZCL_ID_MODE     101
#define  ZCL_ID_BRIGHT   102
#define  ZCL_ID_TEMPER   103
#define  ZCL_ID_HSV      104
#define  ZCL_ID_HS       105
#define  ZCL_ID_H        106
#define  ZCL_ID_S        107
#define  ZCL_ID_COUNT_DOWN 108
#define  ZCL_ID_APP_DATA   109
#define  ZCL_ID_SCENE       110
#define  ZCL_ID_SCENE_SYNC    111
//light define
#define LIGHT_WAY   1  //0---4
#if (MODULE_NAME == ZS3L)
  #define C_NUM    8//PB0
  #define W_NUM    5//PA4
#else
  #define C_NUM    20//PD15
  #define W_NUM    26
#endif

#define APP_COLOR_H_MAX                         360
#define APP_COLOR_S_MAX                         1000
#define APP_COLOR_V_MAX                         1000
#define APP_WHITE_B_MAX                         1000
#define APP_WHITE_T_MAX                         1000

#define ZIGBEE_COLOR_H_MAX                      254     //color: Hue
#define ZIGBEE_COLOR_S_MAX                      254     //color: Saturation
#define ZIGBEE_COLOR_V_MAX                      255     //color: value
#define ZIGBEE_WHITE_B_MAX                      255     //color: level
#define ZIGBEE_WHITE_T_MAX                      255     //color: temperature

#define ZIGBEE_JOIN_MAX_TIMEOUT                 3*60*1000
#define ZIGBEE_HEAR_BEAT_DURATION               150*1000   //150s ~180s
#define NETWORK_JOIN_START_DELAY_TIME           1000  //ms
#define APP_CMD_HANDLE_DELAY_TIME               500

#define NETWORK_JOIN_START_DELAY_TIMER_ID       8
#define DELAY_REPORT_TIMER_ID                   9
#define APP_CMD_HANDLE_DELAY_TIMER_ID           10


#define TY_CMD_SET_COLOR_MODE_CMD_ID            0xF0    //change mode cmd: white/color/scene/music
#define TY_CMD_SET_COLOR_SCENE_CMD_ID           0xF1    //scene data cmd
#define TY_CMD_SET_COLOR_REALTIME_DATA_CMD_ID   0xF2    //real time ctrl cmd: mode(1) + H(2) + S(2) + V(2) + B(2) + T(2) = 11 bytes
#define TY_CMD_SCENE_SYNC_CMD_ID                0xF3    //

#define TY_ATTR_LIGHT_MODE_ATTRIBUTE_ID         0xF000  //light mode attribute (1 byte)
#define TY_ATTR_LIGHT_V_VALUE_ATTRIBUTE_ID      0xF001  //color V attribute (1 byte)
#define TY_ATTR_LIGHT_APP_DATA_ATTRIBUTE_ID     0xF002  //all app data attribute: mode(1) + H(1) + S(1) + V(1) + B(1) + T(2) = 7 Bytes
#define TY_ATTR_LIGHT_SCENE_DATA_ATTRIBUTE_ID   0xF003  //scene data attribute (2+8*n, n<=8) Bytes


#define PRIVATE_V_ATTR \
    { TY_ATTR_LIGHT_MODE_ATTRIBUTE_ID, ATTR_INT8U_ATTRIBUTE_TYPE, 1, (0|ATTR_MASK_SINGLETON), { (uint8_t*)0x00 } }, /* current mode*/\
    { TY_ATTR_LIGHT_V_VALUE_ATTRIBUTE_ID, ATTR_INT8U_ATTRIBUTE_TYPE, 1, (0|ATTR_MASK_SINGLETON), { (uint8_t*)0x00 } }, /* current V*/\
    { TY_ATTR_LIGHT_APP_DATA_ATTRIBUTE_ID, ATTR_INT56U_ATTRIBUTE_TYPE, 7, (0|ATTR_MASK_SINGLETON), { (uint8_t*)0x00 } }, /* app  data*/\
    { TY_ATTR_LIGHT_SCENE_DATA_ATTRIBUTE_ID, ATTR_ARRAY_ATTRIBUTE_TYPE, 67, (ATTR_MASK_SINGLETON), { (uint8_t*)0x00 } }, /* scene data*/\

const attr_t g_group_attr_list[] = {
    GROUP_ATTR_LIST
};

const attr_t g_scene_attr_list[] = {
    SCENE_ATTR_LIST
};

const attr_t g_onoff_attr_list[] = {
    ON_OFF_LIGHT_ATTR_LIST

};
const attr_t g_level_attr_list[] = {
    LEVEL_CONTROL_ATTR_LIST
};

const attr_t g_color_attr_list[] = {
    COLOR_CONTROL_ATTR_LIST
    PRIVATE_V_ATTR
};

const cluster_t g_server_cluster_id[] = {
    DEF_CLUSTER_GROUPS_CLUSTER_ID(g_group_attr_list)
    DEF_CLUSTER_SCENES_CLUSTER_ID(g_scene_attr_list)
    DEF_CLUSTER_ON_OFF_CLUSTER_ID(g_onoff_attr_list)
    DEF_CLUSTER_LEVEL_CONTROL_CLUSTER_ID(g_level_attr_list)
    DEF_CLUSTER_COLOR_CONTROL_CLUSTER_ID(g_color_attr_list)
};

#define SERVER_CLUSTER_LEN  get_array_len(g_server_cluster_id)

const dev_description_t g_dev_des[] = {
    { 1, ZHA_PROFILE_ID, ZG_DEVICE_ID_DIMMABLE_LIGHT, SERVER_CLUSTER_LEN, (cluster_t *)&g_server_cluster_id[0], 0, NULL },
};

#define EP_SUMS sizeof(g_dev_des)/sizeof(g_dev_des[0])
uint8_t count_down_report_onoff_flag = 0;
uint32_t count_down_report_delay = 0;
LIGHT_MODE_E pre_recall_mode = MODE_MAX;
/********************************
    USHORT_T usFreq;            ///< PWM Frequency
    USHORT_T usDuty;            ///< PWM Init duty
    UCHAR_T ucList[5];          ///< GPIO List
    UCHAR_T ucChannel_num;      ///< GPIO List length
    BOOL_T bPolarity;
**********************************/
user_pwm_init_t vLight_pwm_init =
{
   1000,
   500,
   {1,1,1,C_NUM,W_NUM},
   2,
   TRUE
};

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

typedef enum{
	ZIGBEE_CMD_SINGLE = 0,
	ZIGBEE_CMD_GROUP
}ZIGBEE_CMD_T;

user_uart_config_t* mf_test_uart_config(void);

extern void hardware_timer_init(void);
/******************************************************************************************************
FUCTIONS
******************************************************************************************************/

STATIC void zig_bt_2_app_bt(IN uint8_t zig_b, IN uint16_t zig_t, OUT uint16_t *app_b, OUT uint16_t *app_t)
{
    if(app_b != NULL){
        *app_b = (uint16_t)((float)zig_b * APP_WHITE_B_MAX / ZIGBEE_WHITE_B_MAX + 0.5);
    }
    if(app_t != NULL){
        *app_t = (uint16_t)((float)zig_t * APP_WHITE_T_MAX / ZIGBEE_WHITE_T_MAX + 0.5 );
    }
}

STATIC void app_bt_2_zig_bt(OUT uint8_t *zig_b,OUT uint16_t *zig_t, IN uint16_t app_b, IN uint16_t app_t)
{
    if(zig_b != NULL){
        *zig_b = (uint8_t)((float)app_b *  ZIGBEE_WHITE_B_MAX / APP_WHITE_B_MAX + 0.5);
    }

    if(zig_t != NULL){
        *zig_t = (uint16_t)((float)app_t *  ZIGBEE_WHITE_T_MAX / APP_WHITE_T_MAX + 0.5) & 0x00FF;
    }
}
/**
 * @description: Determine whether it is a extern reset (power on reset/extern rest pin,et.)
 * @param {type} none
 * @return: TRUE:reset reason is a extern reset, FALSE: not a extern reset
 */
BOOL_T bLightSysHWRebootJude(VOID)
{
    RESET_REASON_T ret = get_reset_reason();
    app_print("reset reason: %x",ret);
    if(ret == RESET_REASON_POWERON ||
       ret == RESET_REASON_EXTERNAL||
       ret == RESET_REASON_BROWNOUT){
        return TRUE;
    } else {
        return FALSE;
    }
}
/**
 * @description: read app data attribute value,app data = mode(1) + H(1) + S(1) + V(1) + B(1) + T(2) = 7 bytes
 * @param {OUT app_data_t* app_data} readout data value
 * @return: success:1 / faild:0
 */
STATIC bool_t read_app_data_attr(OUT app_data_t* app_data)
{
    app_data_t app_data_temp;
    ATTR_CMD_RET_T ret = dev_zigbee_read_attribute(1,CLUSTER_COLOR_CONTROL_CLUSTER_ID, TY_ATTR_LIGHT_APP_DATA_ATTRIBUTE_ID, &app_data_temp, sizeof(app_data_temp));
    if(ret != ATTR_CMD_RET_SUCCESS){
        return FALSE;
    }
    app_data->mode = app_data_temp.mode;
    app_data->H = app_data_temp.H;
    app_data->S = app_data_temp.S;
    app_data->V = app_data_temp.V;
    app_data->B = app_data_temp.B;
    app_data->T = app_data_temp.T;
    return TRUE;
}

/**
 * @description: write app data attribute value,app data = mode(1) + H(1) + S(1) + V(1) + B(1) + T(2) = 7 bytes
 * @param {OUT app_data_t* app_data} data value to be write
 * @return: success:1 / faild:0
 */
STATIC bool_t write_app_data_attr(app_data_t *app_data)
{
    ATTR_CMD_RET_T ret = dev_zigbee_write_attribute(1, CLUSTER_COLOR_CONTROL_CLUSTER_ID, TY_ATTR_LIGHT_APP_DATA_ATTRIBUTE_ID, app_data, ATTR_INT56U_ATTRIBUTE_TYPE);
    if(ret != ATTR_CMD_RET_SUCCESS){
        return FALSE;
    }
    return TRUE;
}
/**
 * @description: join start delay timer hander, join now
 * @param {none}
 * @return: none
 */
void network_join_start_delay_timer_callback(void)
{
    dev_zigbee_join_start(ZIGBEE_JOIN_MAX_TIMEOUT); //\B2\CE\CA\FD\CA\C7\C5\E4\CD\F8\D7\EE\B4\F3\B3\ACʱ
}
/**
 * @description: light frame callback function when reset cnt over
 * @param {none}
 * @return: OPERATE_RET
 */
OPERATE_RET opLightSysResetCntOverCB(void)
{
    OPERATE_RET ret;
    ret = opUserSWTimerStart(NETWORK_JOIN_START_DELAY_TIMER_ID,\
                             NETWORK_JOIN_START_DELAY_TIME,\
                            (VOID*)network_join_start_delay_timer_callback);

    if(ret!= OPRT_OK){
        return OPRT_COM_ERROR;
    }
    return OPRT_OK;
}
STATIC void clear_recall_scene_report(void)
{
    uint8_t zcl_id = ZCL_ID_ONOFF;
    dev_zigbee_clear_send_data(ZG_CLEAR_ALL_ZCL_ID, &zcl_id);
    zcl_id = ZCL_ID_APP_DATA;
    dev_zigbee_clear_send_data(ZG_CLEAR_ALL_ZCL_ID, &zcl_id);
}
/**
 * @description: report onoff attribute value
 * @param {IN SEND_QOS_T Qos} QOS value
 * @param {IN uint16_t delay_ms} delay report
 * @param {IN bool_t onoff} on / off
 * @return: none
 */
STATIC void report_onoff_value(IN SEND_QOS_T Qos, IN uint16_t delay_ms, IN bool_t onoff)
{
    dev_send_data_t send_data;
    memset(&send_data, 0, sizeof(dev_send_data_t));

    send_data.qos = Qos;
    send_data.delay_time = delay_ms;
    send_data.zcl_id = ZCL_ID_ONOFF;
    send_data.direction = ZCL_DATA_DIRECTION_SERVER_TO_CLIENT;
    send_data.command_id = CMD_REPORT_ATTRIBUTES_COMMAND_ID;
    send_data.addr.mode = SEND_MODE_GW;
    send_data.addr.type.gw.cluster_id = CLUSTER_ON_OFF_CLUSTER_ID;
    send_data.addr.type.gw.src_ep = 1;
    send_data.data.zg.attr_sum = 1;
    send_data.data.zg.attr[0].attr_id = ATTR_ON_OFF_ATTRIBUTE_ID;
    send_data.data.zg.attr[0].type = ATTR_BOOLEAN_ATTRIBUTE_TYPE;
    send_data.data.zg.attr[0].value_size = 1;
    send_data.data.zg.attr[0].value[0] = onoff;

    dev_zigbee_clear_send_data(3, &send_data.zcl_id);
    dev_zigbee_send_data(&send_data,NULL,1000);
    app_print("Report on/off QOS:%d VALUE:%d",Qos,onoff);
}

/**
 * @description: report mode attribute value
 * @param {IN SEND_QOS_T Qos} QOS value
 * @param {IN uint16_t delay_ms} delay report
 * @param {IN uint8_t mode} mode value
 * @return: none
 */
STATIC void report_mode_value(IN SEND_QOS_T Qos, IN uint16_t delay_ms, IN uint8_t mode)
{
    dev_send_data_t send_data;
    memset(&send_data, 0, sizeof(dev_send_data_t));

    send_data.qos = Qos;
    send_data.delay_time = delay_ms;
    send_data.zcl_id = ZCL_ID_MODE;
    send_data.direction = ZCL_DATA_DIRECTION_SERVER_TO_CLIENT;
    send_data.command_id = CMD_REPORT_ATTRIBUTES_COMMAND_ID;
    send_data.addr.mode = SEND_MODE_GW;
    send_data.addr.type.gw.cluster_id = CLUSTER_COLOR_CONTROL_CLUSTER_ID;
    send_data.addr.type.gw.src_ep = 1;
    send_data.data.zg.attr_sum = 1;
    send_data.data.zg.attr[0].attr_id = TY_ATTR_LIGHT_MODE_ATTRIBUTE_ID;
    send_data.data.zg.attr[0].type = ATTR_INT8U_ATTRIBUTE_TYPE;
    send_data.data.zg.attr[0].value_size = 1;
    send_data.data.zg.attr[0].value[0] = mode;

    dev_zigbee_clear_send_data(3, &send_data.zcl_id);
    dev_zigbee_send_data(&send_data,NULL,1000);
    app_print("Report mode QOS:%d VALUE %d",Qos,mode);
}

/**
 * @description: report level  attribute value
 * @param {IN SEND_QOS_T Qos} QOS value
 * @param {IN uint16_t delay_ms} delay report
 * @param {IN uint8_t level} LEVEL  value
 * @return: none
 */
STATIC void report_level_value(IN SEND_QOS_T Qos, IN uint16_t delay_ms, IN uint8_t level)
{
    dev_send_data_t send_data;
    memset(&send_data, 0, sizeof(dev_send_data_t));

    send_data.qos = Qos;
    send_data.delay_time = delay_ms;
    send_data.zcl_id = ZCL_ID_BRIGHT;
    send_data.direction = ZCL_DATA_DIRECTION_SERVER_TO_CLIENT;
    send_data.command_id = CMD_REPORT_ATTRIBUTES_COMMAND_ID;
    send_data.addr.mode = SEND_MODE_GW;
    send_data.addr.type.gw.cluster_id = CLUSTER_LEVEL_CONTROL_CLUSTER_ID;
    send_data.addr.type.gw.src_ep = 1;
    send_data.data.zg.attr_sum = 1;
    send_data.data.zg.attr[0].attr_id = ATTR_CURRENT_LEVEL_ATTRIBUTE_ID;
    send_data.data.zg.attr[0].type = ATTR_INT8U_ATTRIBUTE_TYPE;
    send_data.data.zg.attr[0].value_size = 1;
    send_data.data.zg.attr[0].value[0] = level;

    dev_zigbee_clear_send_data(3, &send_data.zcl_id);
    dev_zigbee_send_data(&send_data,NULL,1000);
    app_print("Report level QOS:%d VALUE %d",Qos,level);
}
/**
 * @description: report temperature attribute value
 * @param {IN SEND_QOS_T Qos} QOS value
 * @param {IN uint16_t delay_ms} delay report
 * @param {IN uint16_t temperature} temperature  value
 * @return: none
 */
STATIC void report_temperature_value(IN SEND_QOS_T Qos, IN uint16_t delay_ms, IN uint16_t temperature)
{
    dev_send_data_t send_data;
    memset(&send_data, 0, sizeof(dev_send_data_t));

    send_data.qos = Qos;
    send_data.delay_time = delay_ms;
    send_data.zcl_id = ZCL_ID_TEMPER;
    send_data.direction = ZCL_DATA_DIRECTION_SERVER_TO_CLIENT;
    send_data.command_id = CMD_REPORT_ATTRIBUTES_COMMAND_ID;
    send_data.addr.mode = SEND_MODE_GW;
    send_data.addr.type.gw.cluster_id = CLUSTER_COLOR_CONTROL_CLUSTER_ID;
    send_data.addr.type.gw.src_ep = 1;
    send_data.data.zg.attr_sum = 1;
    send_data.data.zg.attr[0].attr_id = ATTR_COLOR_CONTROL_COLOR_TEMPERATURE_ATTRIBUTE_ID;
    send_data.data.zg.attr[0].type = ATTR_INT16U_ATTRIBUTE_TYPE;
    send_data.data.zg.attr[0].value_size = 2;
    send_data.data.zg.attr[0].value[0] = temperature;
    send_data.data.zg.attr[0].value[1] = temperature >>8 ;

    dev_zigbee_clear_send_data(3, &send_data.zcl_id);
    dev_zigbee_send_data(&send_data,NULL,1000);
    app_print("Report temperature QOS:%d VALUE %d",Qos,temperature);
}

STATIC void report_count_down_data(IN SEND_QOS_T Qos,IN uint16_t delay_ms, IN uint16_t countdown)
{
    dev_send_data_t send_data;
    memset(&send_data, 0, sizeof(dev_send_data_t));

    send_data.delay_time = delay_ms;
    send_data.qos = Qos;
    send_data.zcl_id = ZCL_ID_COUNT_DOWN;
    send_data.direction = ZCL_DATA_DIRECTION_SERVER_TO_CLIENT;
    send_data.command_id = CMD_REPORT_ATTRIBUTES_COMMAND_ID;
    send_data.addr.mode = SEND_MODE_GW;
    send_data.addr.type.gw.cluster_id = CLUSTER_ON_OFF_CLUSTER_ID;
    send_data.addr.type.gw.src_ep = 1;
    send_data.data.zg.attr_sum = 1;
    send_data.data.zg.attr[0].attr_id = ATTR_OFF_WAIT_TIME_ATTRIBUTE_ID;
    send_data.data.zg.attr[0].type = ATTR_INT16U_ATTRIBUTE_TYPE;
    send_data.data.zg.attr[0].value_size = 2;
    send_data.data.zg.attr[0].value[0] = countdown;
    send_data.data.zg.attr[0].value[1] = countdown >>8 ;

    dev_zigbee_clear_send_data(3, &send_data.zcl_id);
    dev_zigbee_send_data(&send_data,NULL,1000);
    app_print("Report count domn: %x", countdown);
}
/**
 * @description: report app data attribute value
 * @param {IN SEND_QOS_T Qos} QOS value
 * @param {IN uint16_t delay_ms} delay report
 * @param {IN app_data_t *app_data} app data value
 * @return: none
 */
STATIC void report_app_data_value(IN SEND_QOS_T Qos,IN uint16_t delay_ms, IN app_data_t *app_data)
{
    dev_send_data_t send_data;
    memset(&send_data, 0, sizeof(dev_send_data_t));

    send_data.qos = Qos;
    send_data.delay_time = delay_ms;
    send_data.zcl_id = ZCL_ID_APP_DATA;
    send_data.direction = ZCL_DATA_DIRECTION_SERVER_TO_CLIENT;
    send_data.command_id = CMD_REPORT_ATTRIBUTES_COMMAND_ID;
    send_data.addr.mode = SEND_MODE_GW;
    send_data.addr.type.gw.cluster_id = CLUSTER_COLOR_CONTROL_CLUSTER_ID;
    send_data.addr.type.gw.src_ep = 1;
    send_data.data.zg.attr_sum = 1;
    send_data.data.zg.attr[0].attr_id = TY_ATTR_LIGHT_APP_DATA_ATTRIBUTE_ID;
    send_data.data.zg.attr[0].type = ATTR_INT56U_ATTRIBUTE_TYPE;
    send_data.data.zg.attr[0].value_size = sizeof(app_data_t);
    memcpy(&send_data.data.zg.attr[0].value[0], app_data, sizeof(app_data_t));

    dev_zigbee_clear_send_data(ZG_CLEAR_ALL_ZCL_ID, &send_data.zcl_id);
    dev_zigbee_send_data(&send_data,NULL,1000);
    app_print("Report APP DATA  : %d %d %d %d %d %d %d %d", send_data.data.zg.attr[0].value[0],\
                                                            send_data.data.zg.attr[0].value[1],\
                                                            send_data.data.zg.attr[0].value[2],\
                                                            send_data.data.zg.attr[0].value[3],\
                                                            send_data.data.zg.attr[0].value[4],\
                                                            send_data.data.zg.attr[0].value[5],\
                                                            send_data.data.zg.attr[0].value[6],\
                                                            send_data.data.zg.attr[0].value[7]);

}
VOID vLightDataCountDownResponse(OUT UINT_T RemainTimeSec)
{
    STATIC uint32_t last_remain_sec = 0;
    uint8_t onoff = 0;

    if(last_remain_sec == RemainTimeSec){
        //return;
    } else {
        report_count_down_data(QOS_1, count_down_report_delay ,(uint16_t)RemainTimeSec);
	app_print("report count down %d",RemainTimeSec);
    }

    if(RemainTimeSec == 0){
        opLightCtrlDataSwitchGet(&onoff);
        if(count_down_report_onoff_flag == 1){
            report_onoff_value(QOS_0, count_down_report_delay , onoff);
            app_print("report onoff %d",onoff);
        }
        dev_zigbee_write_attribute(1,\
                                CLUSTER_ON_OFF_CLUSTER_ID,\
                                ATTR_ON_OFF_ATTRIBUTE_ID,\
                                &onoff,\
                                ATTR_INT8U_ATTRIBUTE_TYPE);
    }
    last_remain_sec = RemainTimeSec;
}
/**
 * @description: app command received and process now
 * @param {type} none
 * @return: none
 */
void app_cmd_handle_delay_timer_cb(void)
{
    opLightCtrlProc();
}

/**
 * @description: app on/off command handler
 * @param {IN uint8_t onoff} onoff value
 * @return: none
 */
void app_cmd_set_onoff_handler(ZIGBEE_CMD_T cmd_type,  uint8_t onoff)
{
    SEND_QOS_T QOS = QOS_VIP_0;
    uint32_t delay_ms = 0;
    OPERATE_RET ret;
    BRIGHT_DATA_T rgbcw;
    USHORT_T bright;
    USHORT_T temper;
    app_print("Receive on/off: %d",onoff);

    if(cmd_type == ZIGBEE_CMD_GROUP){
        delay_ms = 10000 + random_ms(10000);
        QOS = QOS_0;
    }
    report_onoff_value(QOS, delay_ms, onoff);
    count_down_report_onoff_flag = 0; //dont report onoff again in the count down handler
    count_down_report_delay = delay_ms;
    opLightCtrlDataCountDownSet(0);

    ret = opLightCtrlDataSwitchSet(onoff);
    if(ret != OPRT_OK){
        return;
    }
    if(onoff){
        opLightCtrlDataBrightGet(&bright);
        opLightCtrlDataTemperatureGet(&temper);
        vLightCtrlDataCalcCW(bright,temper,&rgbcw);
        app_print("\r\nrgbcw:B->%d,C->%d,W->%d",bright,rgbcw.usWhite,rgbcw.usWarm);
        opLightSetRGBCW(0,0,0,rgbcw.usWhite,rgbcw.usWarm);//off

    }else
    {
        opLightSetRGBCW(0,0,0,0,0);//off
    }
    //update on/off attribute
    dev_zigbee_write_attribute(1,\
                               CLUSTER_ON_OFF_CLUSTER_ID,\
                               ATTR_ON_OFF_ATTRIBUTE_ID,\
                               &onoff,\
                               ATTR_INT8U_ATTRIBUTE_TYPE);
}


void handleTheCountDown(uint32_t delay_ms)
{
    uint8_t pre_onoff;
    opLightCtrlDataSwitchGet(&pre_onoff);
    opLightCtrlDataSwitchSet(1);
    if(pre_onoff != 1){
        report_onoff_value(QOS_0, delay_ms, 1);
        count_down_report_onoff_flag = 0; //ONOFF changed,need report
        count_down_report_delay = delay_ms;
	    opLightCtrlDataCountDownSet(0);
    }
}

/**
 * @description: app set mode command handler
 * @param {IN uint8_t mode} mode value
 * @return: none
 */
void app_cmd_set_mode_handler(ZIGBEE_CMD_T cmd_type, uint8_t mode)
{
    SEND_QOS_T QOS = QOS_VIP_0;
    uint32_t delay_ms = 0;

    app_print("Receive  mode : %d",mode);
    if(cmd_type == ZIGBEE_CMD_GROUP){
        delay_ms = 10000 + random_ms(10000);
        QOS = QOS_0;
    }
    pre_recall_mode = mode;

    report_mode_value(QOS, delay_ms, mode);
    handleTheCountDown(delay_ms);

    OPERATE_RET ret = opLightCtrlDataModeSet(mode);
    if(ret!=OPRT_OK){
        return;
    }
    //updata mode attribute
    dev_zigbee_write_attribute(1,\
                               CLUSTER_COLOR_CONTROL_CLUSTER_ID,\
                               TY_ATTR_LIGHT_MODE_ATTRIBUTE_ID,\
                               &mode,\
                               ATTR_INT8U_ATTRIBUTE_TYPE);
    //updata app data attribute
    app_data_t app_data;
    read_app_data_attr(&app_data);
    app_data.mode = mode;
    write_app_data_attr(&app_data);
}

/**
 * @description: app move to level (set bright)command handler
 * @param {IN uint8_t bright} bright value
 * @return: none
 */
void app_cmd_set_bright_handler(ZIGBEE_CMD_T cmd_type, uint8_t bright)
{
    uint16_t app_level;
    LIGHT_MODE_E pre_mode;
    LIGHT_MODE_E after_mode;
    SEND_QOS_T QOS = QOS_VIP_0;
    uint32_t delay_ms = 0;
    OPERATE_RET ret = 0;
    BRIGHT_DATA_T rgbcw;
    USHORT_T temper;

    if(cmd_type == ZIGBEE_CMD_GROUP){
        delay_ms = 10000 + random_ms(10000);
        QOS = QOS_0;
    }

    app_print("Receive bright: %d",bright);

    zig_bt_2_app_bt(bright,0,&app_level,NULL);
    opLightCtrlDataModeGet(&pre_mode);
    opLightCtrlDataBrightSet(app_level); //handler the cmd
    opLightCtrlDataModeGet(&after_mode);
    if(pre_mode != after_mode){
        report_mode_value(QOS, delay_ms, after_mode);
    }
    pre_recall_mode = after_mode;
    report_level_value(QOS, delay_ms, bright);

    handleTheCountDown(delay_ms);
    opLightCtrlDataTemperatureGet(&temper);
    vLightCtrlDataCalcCW(app_level,temper,&rgbcw);
    app_print("\r\nrgbcw:B->%d,C->%d,W->%d",app_level,rgbcw.usWhite,rgbcw.usWarm);
    opLightSetRGBCW(0,0,0,rgbcw.usWhite,rgbcw.usWarm);//off
    //updata bright attribute
    dev_zigbee_write_attribute(1,\
                               CLUSTER_LEVEL_CONTROL_CLUSTER_ID,\
                               ATTR_CURRENT_LEVEL_ATTRIBUTE_ID,\
                               &bright,\
                               ATTR_INT8U_ATTRIBUTE_TYPE);
    //updata app data  attribute
    app_data_t app_data;
    read_app_data_attr(&app_data);
    app_data.B = bright;
    write_app_data_attr(&app_data);
}

/**
 * @description: app move to temperature command handler
 * @param {IN uint16_t temper} temper value
 * @return: none
 */
void app_cmd_set_temperature_handler(ZIGBEE_CMD_T cmd_type, uint16_t temper)
{
    uint16_t app_temper = 0;
    LIGHT_MODE_E pre_mode;
    LIGHT_MODE_E after_mode;
    SEND_QOS_T QOS = QOS_VIP_0;
    uint32_t delay_ms = 0;
    OPERATE_RET ret = 0;
    USHORT_T bright;
    BRIGHT_DATA_T rgbcw;

    app_print("Receive temper: %d",temper);
    if(cmd_type == ZIGBEE_CMD_GROUP){
        delay_ms = 10000 + random_ms(10000);
        QOS = QOS_0;
    }
    zig_bt_2_app_bt(0,temper,NULL,&app_temper);
    handleTheCountDown(delay_ms);
    opLightCtrlDataSwitchSet(1);
    opLightCtrlDataModeGet(&pre_mode);
    opLightCtrlDataTemperatureSet(app_temper);
    opLightCtrlDataModeGet(&after_mode);
    if(pre_mode != after_mode){
        report_mode_value(QOS, delay_ms, after_mode);
    }
    pre_recall_mode = after_mode;
    report_temperature_value(QOS, delay_ms, temper);
    //handle the cmd
   opLightCtrlDataBrightGet(&bright);
   vLightCtrlDataCalcCW(bright,app_temper,&rgbcw);
   app_print("\r\nrgbcw:B->%d,C->%d,W->%d",bright,rgbcw.usWhite,rgbcw.usWarm);
   opLightSetRGBCW(0,0,0,rgbcw.usWhite,rgbcw.usWarm);//off
    //updata temperature attribute
    dev_zigbee_write_attribute(1,\
                                CLUSTER_COLOR_CONTROL_CLUSTER_ID,\
                                ATTR_COLOR_CONTROL_COLOR_TEMPERATURE_ATTRIBUTE_ID,\
                                &temper,\
                                ATTR_INT16U_ATTRIBUTE_TYPE);
    //updata app data attribute
    app_data_t app_data;
    read_app_data_attr(&app_data);
    app_data.T = temper;
    write_app_data_attr(&app_data);
}
/**
 * @description: app command set count down handler
 * @param {count_down} seconds
 * @return: none
 */
void app_cmd_set_count_domn_handler(uint16_t count_down)
{
    app_print("Receive count domw: %x",count_down);
    uint32_t count = count_down;
    opLightCtrlDataCountDownSet(count);
    report_count_down_data(QOS_0, 5, count_down);
    count_down_report_onoff_flag = 1;
    count_down_report_delay = 0;
}
/**
 * @description: load app data from Flash,and init the attributes
 * @param {type} none
 * @return: none
 */
static void load_attr_data(void)
{
    uint8_t level = 0;
    uint16_t temper = 0;

    // Get curren ctrl data
    LIGHT_CTRL_DATA_T app_data = opLightCtrlDataGet();

    // updata on/off attribute
    dev_zigbee_write_attribute(1,
                                CLUSTER_ON_OFF_CLUSTER_ID,
                                ATTR_ON_OFF_ATTRIBUTE_ID,
                                &app_data.bSwitch,
                                ATTR_BOOLEAN_ATTRIBUTE_TYPE);
    // updata mode attribute
    dev_zigbee_write_attribute(1,
                                CLUSTER_COLOR_CONTROL_CLUSTER_ID,
                                TY_ATTR_LIGHT_MODE_ATTRIBUTE_ID,
                                &app_data.Mode,
                                ATTR_INT8U_ATTRIBUTE_TYPE);
    // updata level attribute
    dev_zigbee_write_attribute(1,
                                CLUSTER_LEVEL_CONTROL_CLUSTER_ID,
                                ATTR_CURRENT_LEVEL_ATTRIBUTE_ID,
                                &level,
                                ATTR_INT8U_ATTRIBUTE_TYPE);
    // updata temper attribute
    dev_zigbee_write_attribute(1,
                                CLUSTER_COLOR_CONTROL_CLUSTER_ID,
                                ATTR_COLOR_CONTROL_COLOR_TEMPERATURE_ATTRIBUTE_ID,
                                &temper,
                                ATTR_INT16U_ATTRIBUTE_TYPE);

}
/**
 * @description: network join ok handler
 * @param {none}
 * @return: none
 */
static void network_join_ok_handler(void)
{
    //report mode attribute
    LIGHT_MODE_E mode = 0;
    uint8_t onoff = 1;
    uint8_t level = 254;
    uint16_t temperature = 0;
    uint32_t random = random_ms(5000);
    opLightCtrlDataModeSet(0);
    opLightCtrlDataBrightSet(1000);
    opLightCtrlDataTemperatureSet(0);
    load_attr_data();
    report_onoff_value(QOS_1,2000, onoff);
    report_mode_value(QOS_1,0, mode);
    report_count_down_data(QOS_1, 0, 0);

    report_level_value(QOS_0,0,level);
    report_temperature_value(QOS_0,0,temperature);

}
/**
 * @description: device first power on, user can register device and init the gpio
 * @param {type} none
 * @return: none
 */

void dev_power_on_init(void)
{
    gpio_config_t c_pin;
    gpio_config_t w_pin;
#if (MODULE_NAME == ZS3L)
    c_pin.port = PORT_A;
    c_pin.pin = PIN_4;
    c_pin.mode = GPIO_MODE_OUTPUT_PP;
    c_pin.out = GPIO_DOUT_LOW;
    c_pin.drive_flag = GPIO_LEVEL_HIGH;

    w_pin.port = PORT_B;
    w_pin.pin = PIN_0;
    w_pin.mode = GPIO_MODE_OUTPUT_PP;
    w_pin.out = GPIO_DOUT_LOW;
    w_pin.drive_flag = GPIO_LEVEL_HIGH;

#else
    c_pin.port = PORT_D;
    c_pin.pin = PIN_15;
    c_pin.mode = GPIO_MODE_OUTPUT_PP;
    c_pin.out = GPIO_DOUT_LOW;
    c_pin.drive_flag = GPIO_LEVEL_HIGH;

    w_pin.port = PORT_F;
    w_pin.pin = PIN_5;
    w_pin.mode = GPIO_MODE_OUTPUT_PP;
    w_pin.out = GPIO_DOUT_LOW;
    w_pin.drive_flag = GPIO_LEVEL_HIGH;
#endif

    gpio_raw_init(c_pin);
    gpio_raw_init(w_pin);
    dev_register_zg_ep_infor((dev_description_t *)g_dev_des, EP_SUMS);

    zg_dev_config_t g_zg_dev_config;
    g_zg_dev_config.dev_type = ZG_ROUTER;
    g_zg_dev_config.config.router_cfg.reserved = 0;
    dev_register_zg_dev_config(&g_zg_dev_config);

    join_config_t cfg;
    cfg.auto_join_power_on_flag = 0;
    cfg.auto_join_remote_leave_flag = 0;
    cfg.join_timeout = ZIGBEE_JOIN_MAX_TIMEOUT;
    dev_zg_join_config(&cfg);

}

/**
 * @description: dev_init_before_mf_test,
 * @param {type} none
 * @return: none
 */
void dev_init_before_mf_test(void)
{
    OPERATE_RET ret;
#if (MODULE_NAME == ZS3L)
    opSocGpioModuleInit("ZS3L",18);
#else
    opSocGpioModuleInit("TYZS3",18);
#endif

#ifdef APP_DEBUG
    user_uart_config_t *default_config = mf_test_uart_config();
    user_uart_init(default_config);
#endif

    app_print("light start----------------");
    hardware_timer_init();
    opLightDriveInit(&vLight_pwm_init);
    app_print("light pwn init----------------");
    opLightSetRGBCW(0,0,0,1000,0);
    ret = opLightInit();
    if(ret!=OPRT_OK){
        app_print("light init ERROR");
    }

    load_attr_data();

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
    uint8_t ret;
    uint8_t attr_read[67] = {0};
    uint8_t reset_cnt = 0;

#ifdef APP_DEBUG
    user_uart_config_t *default_config = mf_test_uart_config();
    user_uart_init(default_config);
#endif

    // update model id
    uint8_t light_way = 1;
    switch (light_way){
        case 0:{
            memcpy(attr_read, "\7TS0501A", 8);
        }
        break;
        case 1:{
            memcpy(attr_read, "\7TS0502A", 8);
        }
        break;
        case 2:{
            memcpy(attr_read, "\7TS0503A", 8);
        }
        break;
        case 3:{
            memcpy(attr_read, "\7TS0504A", 8);
        }
        break;
        case 4:{
            memcpy(attr_read, "\7TS0505A", 8);
        }
        break;
        default:
            memcpy(attr_read, "\7TS0505A", 8);
        break;
    }
    dev_zigbee_write_attribute(1,
                                CLUSTER_BASIC_CLUSTER_ID,
                                ATTR_MODEL_IDENTIFIER_ATTRIBUTE_ID,
                                attr_read,
                                ATTR_CHAR_STRING_ATTRIBUTE_TYPE);


    // read model id
    ret = dev_zigbee_read_attribute(1,
                                    CLUSTER_BASIC_CLUSTER_ID,
                                    ATTR_MODEL_IDENTIFIER_ATTRIBUTE_ID,
                                    attr_read,
                                    8);
    if(ret == ATTR_CMD_RET_SUCCESS){
        app_print("attr mode id : %s",attr_read+1);
    }else{
        app_print("Read ERROR");
    }


    ret = opUserFlashReadResetCnt(&reset_cnt);
    app_print("Light reset cnt : %d",reset_cnt);
    // read pid
    ret = dev_zigbee_read_attribute(1,
                                    CLUSTER_BASIC_CLUSTER_ID,
                                    ATTR_MANUFACTURER_NAME_ATTRIBUTE_ID,
                                    attr_read,
                                    17);
    if(ret == ATTR_CMD_RET_SUCCESS){
        app_print("attr pid : %s",attr_read+1);
    }else{
        app_print("Read ERROR");
    }

    // read on/off
    ret = dev_zigbee_read_attribute(1,
                                    CLUSTER_ON_OFF_CLUSTER_ID,
                                    ATTR_ON_OFF_ATTRIBUTE_ID,
                                    attr_read,
                                    1);

    if(ret == ATTR_CMD_RET_SUCCESS){
        app_print("attr on/off : %d",attr_read[0]);
    }else{
        app_print("Read ERROR");
    }

    // read light mode
    ret = dev_zigbee_read_attribute(1,
                                    CLUSTER_COLOR_CONTROL_CLUSTER_ID,
                                    TY_ATTR_LIGHT_MODE_ATTRIBUTE_ID,
                                    attr_read,
                                    1);
    if(ret == ATTR_CMD_RET_SUCCESS){
        app_print("attr mode : %d",attr_read[0]);
    }else{
        app_print("Read ERROR");
    }

    // read H
    ret = dev_zigbee_read_attribute(1,
                                    CLUSTER_COLOR_CONTROL_CLUSTER_ID,
                                    ATTR_COLOR_CONTROL_CURRENT_HUE_ATTRIBUTE_ID,
                                    attr_read,
                                    1);
    if(ret == ATTR_CMD_RET_SUCCESS){
        app_print("attr H : %x",attr_read[0]);
    }else{
        app_print("Read ERROR");
    }

    // read S
    ret = dev_zigbee_read_attribute(1,
                                    CLUSTER_COLOR_CONTROL_CLUSTER_ID,
                                    ATTR_COLOR_CONTROL_CURRENT_SATURATION_ATTRIBUTE_ID,
                                    attr_read,
                                    1);
    if(ret == ATTR_CMD_RET_SUCCESS){
        app_print("attr S : %x",attr_read[0]);
    }else{
        app_print("Read ERROR");
    }

    // read V
    ret = dev_zigbee_read_attribute(1,
                                    CLUSTER_COLOR_CONTROL_CLUSTER_ID,
                                    TY_ATTR_LIGHT_V_VALUE_ATTRIBUTE_ID,
                                    attr_read,
                                    1);
    if(ret == ATTR_CMD_RET_SUCCESS){
        app_print("attr V : %x",attr_read[0]);
    }else{
        app_print("Read ERROR");
    }
   // read level
    ret = dev_zigbee_read_attribute(1,
                                    CLUSTER_LEVEL_CONTROL_CLUSTER_ID,
                                    ATTR_CURRENT_LEVEL_ATTRIBUTE_ID,
                                    attr_read,
                                    1);
    if(ret == ATTR_CMD_RET_SUCCESS){
        app_print("attr B : %x",attr_read[0]);
    }else{
        app_print("Read ERROR");
    }

    // read temper
    ret = dev_zigbee_read_attribute(1,
                                    CLUSTER_COLOR_CONTROL_CLUSTER_ID,
                                    ATTR_COLOR_CONTROL_COLOR_TEMPERATURE_ATTRIBUTE_ID,
                                    &attr_read,
                                    2);
    if(ret == ATTR_CMD_RET_SUCCESS){
        app_print("attr T : %x",(attr_read[0]+(attr_read[1]<<8)));
    }else{
        app_print("Read ERROR");
    }
    // read APP data
    read_app_data_attr((app_data_t*)attr_read);

    if(ret == ATTR_CMD_RET_SUCCESS){
        app_print("attr app data M H S V B T: ");
        for(uint8_t i=0;i<sizeof(app_data_t);i++){
            app_print("%d ",attr_read[i]);
        }
        app_print("\r\n ");
    }else{
        app_print("Read ERROR");
    }

    // read scene data
    ret = dev_zigbee_read_attribute(1,
                                    CLUSTER_COLOR_CONTROL_CLUSTER_ID,
                                    TY_ATTR_LIGHT_SCENE_DATA_ATTRIBUTE_ID,
                                    attr_read,
                                    67);
    if(ret == ATTR_CMD_RET_SUCCESS){
        app_print("attr scene : ");
        for(uint8_t i=0;i<attr_read[0]+1;i++){
            app_print("%X ",attr_read[i]);
        }
        app_print("\r\n ");
    }else{
        app_print("Read ERROR");
    }


    dev_heartbeat_set(APP_VERSION, ZIGBEE_HEAR_BEAT_DURATION);

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
    switch(state) {
        case NET_POWER_ON_LEAVE: { //\C9豸\C9ϵ\E7\C0\EB\CD\F8
            app_print("Network state: NET_POWER_ON_LEAVE");
            break;
        }
        case NET_POWER_ON_ONLINE:{ //\C9豸\C9ϵ\E7\D4\DA\CD\F8
            app_print("Network state: NET_POWER_ON_ONLINE");
            network_join_ok_handler();
            opLightCtrlProc();
            break;
        }
        case NET_JOIN_START: {     //\C9豸\BF\AAʼ\C5\E4\CD\F8
            app_print("Network state: NET_JOIN_START");
            opLightCtrlBlinkStart(1000);
            break;
        }
        case NET_JOIN_TIMEOUT: {   //\C9豸\C5\E4\CD\F8\B3\ACʱ
            app_print("Network state: NET_JOIN_TIMEOUT");
            opLightCtrlBlinkStop();
            break;
        }
        case NET_JOIN_OK: {        //\C9豸\C5\E4\CD\F8\B3ɹ\A6
            app_print("Network state: NET_JOIN_OK");
            opLightCtrlBlinkStop();
            network_join_ok_handler();
            opLightCtrlProc();
            break;
        }
        case NET_LOST: {           //˯\C3߽ڵ\E3:\B8\B8\BDڵ㶪ʧ
            app_print("Network state: NET_LOST");
            break;
        }
        case NET_REJOIN_OK: {      //˯\C3߽ڵ㣺rejoin \B3ɹ\A6
            app_print("Network state: NET_REJOIN_OK");
            break;
        }
        case NET_REMOTE_LEAVE: {   //APP leave
            app_print("Network state: NET_REMOTE_LEAVE");

            break;
        }
        case NET_LOCAL_LEAVE: {    //\B1\BE\B5\D8 leave
            app_print("Network state: NET_LOCAL_LEAVE");
            break;
        }
        case NET_MF_TEST_LEAVE:{   //\B2\FA\B2\E2 leave
            app_print("Network state: NET_MF_TEST_LEAVE");
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
    user_uart_config_t *default_config = mf_test_uart_config();
    user_uart_init(default_config);

    opUserSWTimerStop(NETWORK_JOIN_START_DELAY_TIMER_ID);

    app_print("mf test !!!");
    return;
}

/**
 * @description: device report inforamtion when first power on(if need),;
 * @param {type} none
 * @return: none
 */
void dev_power_on_report_info(void)
{
    uint8_t onoff;
    report_count_down_data(QOS_1, 0, 0);
    opLightCtrlDataSwitchGet(&onoff);
    report_onoff_value(QOS_1, 0, onoff);
    return;
}

/**
 * @description: device recovery to factury command handler
 * @param {type} none
 * @return: none
 */
void dev_recovery_factory(DEV_RESET_TYPE_T type)
{
    //TODO
    return;
}

/**
 * @description: device receive message callback
 * @param {*dev_msg} received message information
 * @return: ZCL_CMD_RET_T
 */
ZCL_CMD_RET_T dev_msg_recv_callback(dev_msg_t *dev_msg)
{
    uint8_t i;
    uint16_t count_down_data = 0;
    attr_value_t *attr_list = dev_msg->data.attr_data.attr_value;
    uint8_t attr_sum = dev_msg->data.attr_data.attr_value_sums;
    ZIGBEE_CMD_T app_cmd_type = ZIGBEE_CMD_SINGLE;

    if(dev_msg->msg_src == MSG_SRC_GW_UNICAST || dev_msg->msg_src == MSG_SRC_DEV_UNICAST){
		app_cmd_type = ZIGBEE_CMD_SINGLE;
        //app_print("receive single message");
    }else{
		app_cmd_type = ZIGBEE_CMD_GROUP;

        //app_print("receive group message");
    }
    switch(dev_msg->cluster){
        case CLUSTER_ON_OFF_CLUSTER_ID:{
            for(i=0;i<attr_sum;i++){
                switch (attr_list[0].cmd){
                    case CMD_OFF_COMMAND_ID:{
                        clear_recall_scene_report();
                        app_cmd_set_onoff_handler(app_cmd_type,0);
                        break;
                    }
                    case CMD_ON_COMMAND_ID:{
                        clear_recall_scene_report();
                        app_cmd_set_onoff_handler(app_cmd_type,1);
                        break;
                    }
                    case CMD_ON_WITH_TIMED_OFF_COMMAND_ID:
                        count_down_data = attr_list[0].value[1] + (attr_list[0].value[2] << 8);
                        app_cmd_set_count_domn_handler(count_down_data);

                        break;
                    default:
                        break;
                }
            }
            break;
        }
        case CLUSTER_LEVEL_CONTROL_CLUSTER_ID:{
            clear_recall_scene_report();
            for(i=0;i<attr_sum;i++){
                if(attr_list[i].cmd == CMD_MOVE_TO_LEVEL_COMMAND_ID){
                    uint8_t level = attr_list[i].value[0];
                    app_cmd_set_bright_handler(app_cmd_type,level);
                }
            }
            break;
        }
        case CLUSTER_COLOR_CONTROL_CLUSTER_ID:{
            clear_recall_scene_report();
            for(i=0;i<attr_sum;i++){
                switch (attr_list[0].cmd){
                    case CMD_MOVE_TO_COLOR_TEMPERATURE_COMMAND_ID:{
                        uint16_t temperature = attr_list[0].value[0] +((uint16_t)attr_list[0].value[1]<<8);

                        app_cmd_set_temperature_handler(app_cmd_type,temperature);
                        break;
                    }
                    case TY_CMD_SET_COLOR_MODE_CMD_ID:{
                        uint8_t mode = attr_list[0].value[0];

                        app_cmd_set_mode_handler(app_cmd_type, mode);
                        break;
                    }
                    default:
                        break;
                }
            }
            break;
        }
        default :
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
    uint8_t onoff;
    uint8_t mode;
	uint8_t zig_level;
	bool_t proc_flag = FALSE;

    uint16_t app_bright;
    uint16_t app_temper;

	uint32_t delay_ms = 0;
	OPERATE_RET ret = 0;
    uint16_t zig_temper;

    COLOR_RGB_T app_rgb;
    zigbee_hsv_t zig_hsv = {0,0,0};
    app_hsv_t  app_hsv = {0,0,0};
    app_data_t app_attr_data;

    uint8_t pre_onoff;

    app_print("RECALL SCENE DATA LEN = %d", in_data->data_len);
    app_print("RECALL SCENE DATA: ");

    for(uint8_t i=0;i<in_data->data_len;i++){
        app_print("%d ",in_data->data[i]);
    }
    app_print("\r\n");

    onoff = in_data->data[0];
    mode = in_data->data[1];
    app_rgb.usRed = (in_data->data[2]<<8) | in_data->data[3];
    app_rgb.usGreen = (in_data->data[4]<<8) | in_data->data[5];
    app_rgb.usBlue = (in_data->data[6]<<8) | in_data->data[7];
    app_bright = (in_data->data[8]<<8) | in_data->data[9];
    app_temper = (in_data->data[10]<<8) | in_data->data[11];

    delay_ms = 10000 + random_ms(10000);

    clear_recall_scene_report();

    opLightCtrlDataModeGet(&pre_recall_mode); //save mode, before change
    opLightCtrlDataSwitchGet(&pre_onoff);
    ret = opLightCtrlDataSwitchSet(onoff);

    if(pre_onoff != onoff){
        //onoff state has been changed, stop the count down timer, and report onoff
        count_down_report_onoff_flag = 1;
        count_down_report_delay = delay_ms + 1000;
		opLightCtrlDataCountDownSet(0);
    }
    if(ret == OPRT_OK){
        proc_flag = TRUE;
    }
    ret = opLightCtrlDataModeSet(mode);
    if(ret == OPRT_OK){
        proc_flag = TRUE;
    }
    if(mode == WHITE_MODE){
        ret =opLightCtrlDataBrightSet(app_bright);
        if(ret == OPRT_OK){
            proc_flag = TRUE;
        }
        ret =opLightCtrlDataTemperatureSet(app_temper);
        if(ret == OPRT_OK){
            proc_flag = TRUE;
        }
    }else{
        // do nothing
    }
    if(proc_flag){
        opLightCtrlProc();
    }

    read_app_data_attr(&app_attr_data);
    app_attr_data.mode = mode;

    if(mode == WHITE_MODE){
        app_bt_2_zig_bt(&zig_level, &zig_temper, app_bright, app_temper);
        app_attr_data.B    = zig_level;
        app_attr_data.T    = zig_temper;
    }
    write_app_data_attr(&app_attr_data);
    report_app_data_value(QOS_1, delay_ms, &app_attr_data);

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
    uint8_t onoff;
    uint8_t mode;
    COLOR_RGB_T rgb;
    uint16_t bright;
    uint16_t temper;

    opLightCtrlDataSwitchGet(&onoff);
    opLightCtrlDataModeGet(&mode);
    opLightCtrlDataRGBGet(&rgb, NULL);
    opLightCtrlDataBrightGet(&bright);
    opLightCtrlDataTemperatureGet(&temper);

    out_data[0] = onoff;
    out_data[1] = mode;

    out_data[2] = rgb.usRed>>8;
    out_data[3] = rgb.usRed;

    out_data[4] = rgb.usGreen>>8;
    out_data[5] = rgb.usGreen;

    out_data[6] = rgb.usBlue >>8;
    out_data[7] = rgb.usBlue;

    out_data[8] = bright>>8;
    out_data[9] = bright;

    out_data[10] = temper>>8;
    out_data[11] = temper;

    *in_out_len = 12;
/*
    memcpy(out_data+1, &app_data, sizeof(app_data_t));
    *in_out_len = 1 + sizeof(app_data_t);
*/
    app_print("ADD SCENE DATA LEN = 0x%x", *in_out_len);
    app_print("ADD SCENE DATA : ");
    for(uint8_t i=0;i<(*in_out_len);i++)
    {
        app_print("0x%x ",out_data[i]);
    }
    app_print("\r\n");
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
    else if(MODULE_NAME == ZS3L)
    {
        user_uart_config_t default_config = ZS3L_USART_CONFIG_DEFAULT;
        memcpy(&config, &default_config, sizeof(user_uart_config_t));
    }
    else
    {
        user_uart_config_t default_config = TYZS3_USART_CONFIG_DEFAULT;
        memcpy(&config, &default_config, sizeof(user_uart_config_t));
    }
    return &config;
}
