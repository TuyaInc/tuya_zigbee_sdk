/**
 * @Author: Leon
 * @email: zhangpeng@tuya.com
 * @LastEditors: Leon
 * @file name: serial_protocol.h
 * @Description: serial protocol used by TuYa ZigBee modules and MCU communicated by UART
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-03-25 21:41:05
 * @LastEditTime: 2019-03-25 23:04:11
 */

#ifndef __SERIAL_PROTOCOL_H
#define __SERIAL_PROTOCOL_H
#include "stdint.h"
#include "system.h"
#include "mcu_api.h"
#include "zigbee_sdk.h"
#include "config.h"
//#include "oem_util.h"

#ifdef __cplusplus
	extern "C" {
#endif

typedef signed char int8;

//#define SERIAL_PROTOCOL_SCENE_ENABLE   ///<serial protocol scene switch enable

#ifdef SERIAL_PROTOCOL_PRINT_ENABLE
#define emberSerialProtocolPrintln(...)  emberAfPrintln(EMBER_AF_PRINT_CORE,  __VA_ARGS__)
#else
#define emberSerialProtocolPrintln(...)
#endif

//* 
//* @brief serial protocol event define. 
//*
#define customNetworkStateIndEventControl DEV_EVT_1 ///<device network state indication event
#define rspConfirmTimeoutEventControl DEV_EVT_2  ///<response confirm timeout event
#define serialProtocolTimerSyncEventControl DEV_EVT_3 ///<time sync event
//#define emberHeartBeatDurationEventControl DEV_EVT_4  ///<heart beat duration event
#define serialCommadDelaySendEventControl DEV_EVT_5  ///<serial command delay send event
#define emberNetworkRejoinEventControl DEV_EVT_6  ///<network rejion event
#define emberProductIfoPidCheckEventControl DEV_EVT_7  ///<pid check event
//#define emberResetFacturyEventControl DEV_EVT_8  ///<reset factury event
#define emSerialMcuVerReportEventControl DEV_EVT_9 
#define emSerialProtocolSendQueueTickEventControl DEV_EVT_10
#define emSerialProtocolScanRssiEventControl DEV_EVT_11   ///< serial protocoal scan Rssi event
#define emSerialProtocolScanRssiTimeOutEventControl DEV_EVT_12  ///< serial protocol scan time out event
#define emberSocWakeUpEventControl DEV_EVT_13  ///< serial protocol soc weakup event
#define emberMcuWakeUpEventControl DEV_EVT_14  ///<MCU ISR wakeup event
#define emberPollRateDelayToSetEventControl DEV_EVT_15 
#define serialMcuOtaPollrateTimeoutEventControl  DEV_EVT_16

//* 
//* @brief serial protocol data define. 
//*
#define MAX_SERIAL_PAYLOAD_SIZE            128     ///<serial protocol max payload size
#define MAX_ZIGBEE_PAYLOAD_SIZE            64      ///<zigbee frame max payload size
#define MAX_MANU_NAME_SIZE                 34      ///<manufacture name max payload size
#define DEFAULT_PID_LEN                    8       ///<product id default length
#define DEFAULT_VER_LEN                    5       ///<MCU version default length
#define SERIAL_PROTOCOL_SCAN_RSSI_PERIOD   20      ///<scan rssi period 20ms
#define SERIAL_RF_SEND_NUM                 100      ///<RF test send number
#define SERIAL_PROTOCOL_SCAN_RSSI_TIMEOUT  \
    SERIAL_PROTOCOL_SCAN_RSSI_PERIOD*SERIAL_RF_SEND_NUM*2 ///<scan rssi timeout 2000ms
#define SERIAL_PROTOCOL_RETRANSMITE_TIME   4       ///<max retransmission times
#define SERIAL_PROTOCOL_RETRANSMITE_PERIOD 500     ///<retransmission period is 500ms
#define SERIAL_PROTOCOL_UART_ID            UART_ID_UART0
#define SERIAL_POLL_TIMEOUT                1000*90   //90 sec

#define KEY_0_ID    0

#if (MODULE_NAME == TYZS3)
#define KEY0_PORT                       PORT_A
#define KEY0_PIN                        PIN_2
#define KEY0_MODE                       GPIO_MODE_INPUT_PULL
#define KEY0_DOUT                       GPIO_DOUT_HIGH
#define KEY0_DRIVE                      GPIO_LEVEL_LOW

#define LED0_PORT                       PORT_F
#define LED0_PIN                        PIN_5
#define LED0_MODE                       GPIO_MODE_OUTPUT_PP
#define LED0_DOUT                       GPIO_DOUT_HIGH
#define LED0_DRIVE                      GPIO_DOUT_LOW

#elif (MODULE_NAME == TYZS5)
#define KEY0_PORT                       PORT_A
#define KEY0_PIN                        PIN_3
#define KEY0_MODE                       GPIO_MODE_INPUT_PULL
#define KEY0_DOUT                       GPIO_DOUT_HIGH
#define KEY0_DRIVE                      GPIO_LEVEL_LOW

#define LED0_PORT                       PORT_A
#define LED0_PIN                        PIN_4
#define LED0_MODE                       GPIO_MODE_OUTPUT_PP
#define LED0_DOUT                       GPIO_DOUT_HIGH
#define LED0_DRIVE                      GPIO_DOUT_LOW
#endif


typedef enum
{
    MF_TEST_MODULE,
    NWK_STATE_OK,
    NWK_STATE_ERROR
}WORK_STATE_E;

typedef enum
{
    NWK_NOT_JOIN = 0,
    NWK_JOINED,
    NWK_INFO_ERROR,
    NWK_JOINING
}NWK_STATE_E;

typedef enum
{
    DEVICE_CFG_RESET = 0,  ///<configure the device to reset module
    DEVICE_CFG_JOIN,       ///<configure the device to join nodule
}DEVICE_CFG;

//* 
//* @brief serial protocol command data struct. 
//*
typedef struct {
    uint16_t cmd_hdr;                        ///<serial protocol command header
    uint8_t  version;                        ///<serial protocol version
    uint16_t seq_num;                        ///<frame sequence number
    uint8_t cmd;                              ///<serial protocol command type
    uint16_t  data_len;                       ///<serial protocol data length
    uint8_t  data[MAX_SERIAL_PAYLOAD_SIZE];  ///<serial protocol data
    uint8_t  crc;                            ///<serial protocol data frame crc
}serial_protocol_cmd_t;

typedef struct
{
    uint32_t last_tick;
    uint32_t utc_time;
    uint32_t current_time;
}local_time_t;

typedef enum
{
    SERIAL_PROTOCOL_DATA_REQ = 0,
    SERIAL_PROTOCOL_DATA_RSP,
    SERIAL_PROTOCOL_DATA_REPORT,
    SERIAL_PROTOCOL_DATA_QUERY,
    SERIAL_PROTOCOL_MCU_OTA_VER_REQ = 0x10,
    SERIAL_PROTOCOL_MCU_OTA_VER_RSP = 0x11,
    SERIAL_PROTOCOL_MCU_OTA_NOTIFY = 0x12,
    SERIAL_PROTOCOL_MCU_OTA_DATA_REQ = 0x13,
    SERIAL_PROTOCOL_MCU_OTA_DATA_RSP = 0x14,
    SERIAL_PROTOCOL_MCU_OTA_END = 0x15,
    SERIAL_PROTOCOL_TIME_REQ = 0x24,
}SPECIFIC_CLUSTER_COMAMND_ID_E;

extern void emSerialProtocolScanRssiEventHandler(void);
extern void emSerialProtocolScanRssiTimeOutEventHandler(void);
extern uint8_t serialTotalSceneSwitchNumGet(void);
extern void productInfoReqCmdSend(void);
extern void nwkInfoReqCmdSend(uint8_t nwk_status);
extern bool_t serialDataReqCmdSend(uint8_t* data, uint8_t data_len);
extern void serialFuncTestRssiRspSend(uint8_t percentageRssi);
extern void serialDataConfirmSend(bool_t sync, uint8_t status ,uint16_t seq_num);
extern void serialSceneSwitchNumGetSend(void);
extern void serialCommandRxHandler(serial_protocol_cmd_t* receive_cmd);
extern void serialProtocolScanRssiResultCallback(int8 rssi);
extern void serialProtocolRfReceiveProcess(uint8_t* packet);
extern uint8_t emberAfSerialProtocolCommandParseCallback(dev_msg_t *cmd);
extern void  emberSerialProtocolInitCallback(void);
extern uint16_t serialProtocolSeqNumGet(void);
extern void sendQueueInit(void);
extern void sendQueueUpdate(uint16_t seqNum);
extern void sendQueueCheckCallback(void);
extern void emSerialProtocolSendQueueTickEventHandler(void);
extern bool_t sendQueuePush(uint8_t* data, uint8_t dataLen, uint16_t seqNum, bool_t retransmitFlag);
extern void serialDataReceiveHandler(unsigned char* payload, unsigned short payload_len);
extern void serialTotalSceneSwitchNumCallback(uint8_t switchNum);
extern void serialSceneSwitchRecallCallback(uint8_t switchId);


#ifdef __cplusplus
}
#endif

#endif /*__SERIAL_PROTOCOL_H*/
