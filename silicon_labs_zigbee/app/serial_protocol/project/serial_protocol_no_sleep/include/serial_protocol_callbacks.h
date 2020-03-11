/**
 * @Author: Leon
 * @email: zhangpeng@tuya.com
 * @LastEditors: Leon
 * @file name: serial_protocol_callbacks.h
 * @Description: serial protocol application function handler file
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-03-25 21:41:05
 * @LastEditTime: 2019-03-25 22:58:56
 */

#ifndef __SERIAL_PROTOCOL_H__
#define __SERIAL_PROTOCOL_H__

#include "zigbee_sdk.h"
#include "zigbee_dev_template.h"
#include "serial_protocol.h"
#include "oem_util.h"

// event macros
/**/
#define DATA_CONF_MAX    20

// Custom macros
/**/
typedef enum
{
    STATE_IDEL = 0,
    SCENE_NUM,
    NWK_STATE,
}SERIAL_COMMAND_SEND_STATE_E;

typedef enum {
    DEV_WORK_ST_NORMAL = 0,
    DEV_WORK_ST_TEST
}DEV_WORK_ST_T;

typedef struct
{
    bool_t flag;
    uint16_t zcl_seq_num;
    uint16_t confirm_seq_num;
    uint32_t currentTime;
}data_confirm_t;


//UART_PIN_TYPE_CONFIG
#define APP_UART_CONFIG_DEFAULT {\
    UART_ID_UART0,\
    UART_PIN_TYPE_CONFIG,\
    {UART0_TX_PORT, UART0_TX_PIN},\
    {UART0_RX_PORT, UART0_RX_PIN},\
    UART0_TX_LOC, \
    UART0_RX_LOC, \
    UART0_BANDRATE,\
    USART_PARITY_NONE,\
    USART_STOPBITS_ONE,\
    USART_DATABITS_8BIT,\
    NULL\
}

typedef struct 
{
    /* data */
    DEV_TYPE_E device_type;
    uint32_t serial_bandrate;
    uint32_t join_timeout;

}serial_protocol_config_t;

#define APP_BAUD_RATE                  9600  ///< user baudrate is 9600
#define BUTTON_HOLD_DURATION_MS        10000
#define HEART_BEAT_DURATION_QS         2*60*4  //2 min
#define MCU_VERSION_REPORT_DELAY_MS    1000*30    //3 min
#define NEIGHBOR_TABLE_CHECK_PERIOD_QS 60*4 // 1min
#define SERIAL_CMD_DELAY_SEND_MS       2000 // 2000 ms
#define RSP_CONFIRM_TIMEOUT            3000
#define SHORT_REJOIN_INTERVAL           60
#define MIDDLE_REJOIN_INTERVAL          SHORT_REJOIN_INTERVAL*30
#define LONG_REJOIN_INTERVAL            MIDDLE_REJOIN_INTERVAL*16
#define WAKE_UP_STAY_TIME               100
#define MAX_FAILURE_TIMES               100
#define MAX_SERIAL_PAYLOAD_SIZE         128     ///<serial protocol max payload size
#define PRODUCTOR_ID_PFEFIX_SCENE       "_TZB000_"
#define MODEL_ID_SCENE                  "TS0602"
#define DEFAULT_POLL_RATE               1000*5 //5s
#define TIME_SYNC_PERIOD                    1000*60*60*1 //2 hour

#ifdef SERIAL_PROTOCOL_PRINT_ENABLE
#define serial_print(...) uart_printf(UART_ID_UART0, __VA_ARGS__)
#else
#define serial_print(...)
#endif

#define MAX_SCENE_SWITCH_ID                 10
#define ZIGBEE_JOIN_MAX_TIMEOUT             1000*60*3 //3min

extern volatile bool_t mcu_wake_flag;

extern void emberSerialProtocolMcuWakeUpCallback(void);
extern void serialProtocolTimeReqCallback(uint8_t* data, uint8_t data_len);
extern void serialDeviceConfigCallback(uint8_t config);
extern void serialDataRspSendCallback(bool_t sync, uint8_t* data, uint8_t data_len);
extern void customeGetLastRespondSeqNumberCallback(uint8_t resp_seq_num);
extern void serial_protocol_event_Hanlder_callback(uint8_t event_id);
extern void serialMcuOtaDataSendCallback(uint8_t cmd_id, uint8_t* data, uint8_t data_len);

#endif //__SERIAL_PROTOCOL_H__
