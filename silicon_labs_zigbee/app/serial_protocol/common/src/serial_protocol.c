/*
 * @Author: Leon
 * @LastEditors: Leon
 * @file name: serial_protocol.c
 * @Description: serial protocol used by TuYa ZigBee modules and MCU communicated by UART
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Date: 2019-03-13 17:20:41
 * @LastEditTime: 2019-05-20 15:34:12
 */

#define __MF_TEST_GLOBALS
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "jsmn.h"
#pragma pack()

#include "zigbee_attr.h"
#include "serial_protocol.h"
#include "serial_protocol_queue.h"
#include "serial_protocol_callbacks.h"
#include "zigbee_sdk.h"
#include "config.h"
#include "oem_util.h"

static volatile bool_t scanRssiFlag = FALSE;  ///<to scan the current channel Rssi value
static volatile uint8_t scanChannel; ///<to scan the current channel
static  uint8_t total_send_num = 0; ///<total send number in  RF test
static  uint8_t total_receive_num = 0; ///<total receive number in  RF test
static volatile uint8_t totalSceneSwitchNum = 0; ///<total scene switch number of MCU
static volatile uint16_t serialProtocolSeqNum = 0; ///<protocol sequence number
static volatile uint16_t productInfoSeqNum = 0;
static volatile uint16_t nwkInfoSeqNum = 0;
static volatile uint16_t dataReqSeqNum = 0;
static volatile uint16_t sceneSwitchNumGetSeqNum = 0;
static volatile uint8_t packet_payload[] = {0x55, 0xaa, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};
static volatile serial_protocol_rf_test_t rf_test;

uint8_t mcu_version = 0;
extern local_time_t local_time;
extern oem_dev_t dev_cfg;

void serial_protocol_event_handler(uint8_t evt);

extern void deviceWorkStateSet(WORK_STATE_E state);
extern WORK_STATE_E deviceWorkStateGet(void);

/** 
 * serialProtocolScanRssiFlagSet
 *serial protocol scan rssi test flag set
 * @param[in]     flag  set the scan flag
 * @return        NULL
 */
void serialProtocolScanRssiFlagSet(bool_t flag)
{
    scanRssiFlag = flag;
}

/** 
 * serialProtocolScanRssiFlagGet
 *serial protocol scan rssi test flag get
 * @param[in]     NULL
 * @return        true or false
 */
bool_t serialProtocolScanRssiFlagGet(void)
{
    return scanRssiFlag;
}

/** 
 * serialTotalSceneSwitchNumSet
 *serial protocol set total scene switch number
 * @param[in]     channel scan channel
 * @return        NULL
 */
void serialTotalSceneSwitchNumSet(uint8_t switchNum)
{
    totalSceneSwitchNum = switchNum;
}

/** 
 * serialTotalSceneSwitchNumGet
 *serial protocol get total scene switch number
 * @param[in]     NULL
 * @return        total scene switch number
 */
uint8_t serialTotalSceneSwitchNumGet(void)
{
    return totalSceneSwitchNum;
}

/** 
 * serialProtocolSeqNumGet
 *serial protocol get sequence number
 * @param[in]     NULL
 * @return        total scene switch number
 */
uint16_t serialProtocolSeqNumGet(void)
{
    serialProtocolSeqNum++;
    if(serialProtocolSeqNum >= 0xFFF0 || serialProtocolSeqNum < 1)
    {
        serialProtocolSeqNum = 1;
    }
    return serialProtocolSeqNum;
}

/** 
 * emberSerialProtocolInitCallback
 * @param[in]     NULL
 * @return        NULL
 */
void  emberSerialProtocolInitCallback(void)
{
    sendQueueInit();
}

/** 
 * serialCommandGenerate
 * generate serial command to the send buffer
 * @param[in]   send_cmd  serial command data need to be send
 * @param[out]  send_buf  send data buffer
 * @param[out]  send_buf_len  send data length
 * @return      true or false
 */
static bool_t serialCommandGenerate(serial_protocol_cmd_t* send_cmd, uint8_t* send_buf, uint8_t* send_buf_len)
{
    uint8_t i, index = 0, cmd_crc = 0;

    send_buf[index++] = send_cmd->cmd_hdr>>8 & 0xff;
    send_buf[index++] = send_cmd->cmd_hdr & 0xff;
    send_cmd->version = SERIAL_PROTOCOL_VER;

    send_buf[index++] = send_cmd->version;

    send_buf[index++] = (send_cmd->seq_num >> 8) & 0xff;
    send_buf[index++] = send_cmd->seq_num & 0xff;
    send_buf[index++] = send_cmd->cmd;

    send_buf[index++] = send_cmd->data_len>>8 & 0xff;
    send_buf[index++] = send_cmd->data_len & 0xff;

    if(send_cmd->data_len > MAX_SERIAL_PAYLOAD_SIZE - index)
    {
        return FALSE;
    }
    
    memcpy(&send_buf[index], send_cmd->data, send_cmd->data_len);
    index += send_cmd->data_len;
    
    /*crc*/
    for(i = 0; i < index; i++)
    {
      cmd_crc += send_buf[i];
    }
    
    send_buf[index++] = cmd_crc;
    *send_buf_len = index;

    return TRUE;
}

/** 
 * productInfoReqCmdSend
 * send product information request command
 * @param[in]   NULL
 * @return      NULL
 */
void productInfoReqCmdSend(void)
{
    serial_protocol_cmd_t send_cmd;
    uint8_t send_buf[MAX_SERIAL_PAYLOAD_SIZE];
    uint8_t send_buf_len = 0;
    
    if(NWK_STATE_OK == deviceWorkStateGet())
    {
        return;
    }
    memset(&send_cmd, 0, sizeof(serial_protocol_cmd_t));
    memset(&send_buf, 0, MAX_SERIAL_PAYLOAD_SIZE);

    send_cmd.cmd_hdr = SERIAL_PROTOCOL_HDR;
    send_cmd.version = SERIAL_PROTOCOL_VER;
    send_cmd.seq_num = serialProtocolSeqNumGet();
    send_cmd.cmd = PRODUCT_INFO_CMD;
    send_cmd.data_len = 0;

    productInfoSeqNum = send_cmd.seq_num;
    if(serialCommandGenerate( &send_cmd, send_buf, &send_buf_len))
    {
        sendQueuePush(send_buf, send_buf_len, send_cmd.seq_num, TRUE);
    }
}

/** 
 * serialDeviceConfigRspSend
 * send device configure response command
 * @param[in]   seq_num sequence number
 * @return      NULL
 */
static void serialDeviceConfigRspSend(uint16_t seq_num)
{
    serial_protocol_cmd_t send_cmd;
    uint8_t send_buf[MAX_SERIAL_PAYLOAD_SIZE];
    uint8_t send_buf_len = 0;
    uint16_t tick = 0xCFFF;

    memset(&send_cmd, 0, sizeof(serial_protocol_cmd_t));
    memset(&send_buf, 0, MAX_SERIAL_PAYLOAD_SIZE);

    send_cmd.cmd_hdr = SERIAL_PROTOCOL_HDR;
    send_cmd.version = SERIAL_PROTOCOL_VER;
    send_cmd.seq_num = seq_num;
    send_cmd.cmd = ZIGBEE_CFG_CMD;
    send_cmd.data_len = 0;

    if(serialCommandGenerate( &send_cmd, send_buf, &send_buf_len))
    {
        user_uart_send(SERIAL_PROTOCOL_UART_ID, send_buf, send_buf_len);
    }

    do
    {
        tick--;
    }while (tick > 0); //delay for a while to wait uart send ok
    
}

/** 
 * nwkInfoReqCmdSend
 * send network information request command
 * @param[in]   nwk_status: network status enum type, 0x00: not join, 0x01: joined;
 * @return      NULL
 */
void nwkInfoReqCmdSend(uint8_t nwk_status)
{
    serial_protocol_cmd_t send_cmd;
    uint8_t send_buf[MAX_SERIAL_PAYLOAD_SIZE];
    uint8_t send_buf_len = 0;

    memset(&send_cmd, 0, sizeof(serial_protocol_cmd_t));
    memset(&send_buf, 0, MAX_SERIAL_PAYLOAD_SIZE);

    send_cmd.cmd_hdr = SERIAL_PROTOCOL_HDR;
    send_cmd.version = SERIAL_PROTOCOL_VER;
    send_cmd.seq_num = serialProtocolSeqNumGet();
    send_cmd.cmd = ZIGBEE_STATE_CMD;
    
    send_cmd.data[0] = nwk_status;
    send_cmd.data_len++;

    nwkInfoSeqNum = send_cmd.seq_num;
    
    if(serialCommandGenerate( &send_cmd, send_buf, &send_buf_len))
    {
        sendQueuePush(send_buf, send_buf_len, send_cmd.seq_num, FALSE);
    }
}

/**
 * serialDataReqCmdSend
 * serial data request command send to MCU
 * @param[in]   data: data request command payload
 * @param[in]   data_len: data request command payload len
 * @return      true or false
 */
bool_t serialDataReqCmdSend(uint8_t* data, uint8_t data_len)
{
    serial_protocol_cmd_t send_cmd;
    uint8_t send_buf[MAX_SERIAL_PAYLOAD_SIZE];
    uint8_t send_buf_len = 0;

    memset(&send_cmd, 0, sizeof(serial_protocol_cmd_t));
    memset(&send_buf, 0, MAX_SERIAL_PAYLOAD_SIZE);

    send_cmd.cmd_hdr = SERIAL_PROTOCOL_HDR;
    send_cmd.version = SERIAL_PROTOCOL_VER;
    send_cmd.seq_num = serialProtocolSeqNumGet();
    send_cmd.cmd = ZIGBEE_DATA_REQ_CMD;

    /*payload first and second byte is sequence number in version 2*/
    uint16_t seqNum = 0;
    uint8_t* ptr= NULL;

    ptr = data;/*the start three byte is frame control, sequence and command id*/
    seqNum = *ptr << 8 ;
    ptr++;
    seqNum |= *ptr;
    ptr++;

    send_cmd.seq_num = seqNum;
    send_cmd.data_len = data_len - 2;
    memcpy(send_cmd.data, ptr, send_cmd.data_len);
    dataReqSeqNum = send_cmd.seq_num;

    if(serialCommandGenerate( &send_cmd, send_buf, &send_buf_len))
    {
        sendQueuePush(send_buf, send_buf_len, send_cmd.seq_num, TRUE);
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

/**
 * serialDataConfirmSend
 * send data request confirm status to MCU
 * @param[in]   sync:true sync, false async 
 * @param[in]   status: data response status, 0x00:failure, 0x01:success;
 * @return      NULL
 */
void serialDataConfirmSend(bool_t sync, uint8_t status ,uint16_t seq_num)
{
    serial_protocol_cmd_t send_cmd;
    uint8_t send_buf[MAX_SERIAL_PAYLOAD_SIZE];
    uint8_t send_buf_len = 0;

    memset(&send_cmd, 0, sizeof(serial_protocol_cmd_t));
    memset(&send_buf, 0, MAX_SERIAL_PAYLOAD_SIZE);

    send_cmd.cmd_hdr = SERIAL_PROTOCOL_HDR;
    send_cmd.version = SERIAL_PROTOCOL_VER;
    send_cmd.seq_num = seq_num;

    if(TRUE == sync)
    {
        send_cmd.cmd = DATA_RSP_SYNC_CMD;
    }
    else
    {
        send_cmd.cmd = DATA_RSP_ASYNC_CMD;
    }

    send_cmd.data_len = 1;
    send_cmd.data[0] = status;

    if(serialCommandGenerate( &send_cmd, send_buf, &send_buf_len))
    {
        sendQueuePush(send_buf, send_buf_len, send_cmd.seq_num, FALSE);
    }
}

/**
 * serialFuncTestRssiRspSend
 * send rssi scan result to MCU
 * @param[in]   status: data response status, 0x00:failure, 0x01:success;
 * @return      NULL
 */
void serialFuncTestRssiRspSend(uint8_t percentageRssi)
{
    serial_protocol_cmd_t send_cmd;
    uint8_t send_buf[MAX_SERIAL_PAYLOAD_SIZE];
    uint8_t send_buf_len = 0;

    memset(&send_cmd, 0, sizeof(serial_protocol_cmd_t));
    memset(&send_buf, 0, MAX_SERIAL_PAYLOAD_SIZE);

    send_cmd.cmd_hdr = SERIAL_PROTOCOL_HDR;
    send_cmd.version = SERIAL_PROTOCOL_VER;
    send_cmd.seq_num = serialProtocolSeqNumGet();
    send_cmd.cmd = FUNC_TEST_CMD;

    send_cmd.data_len = 2;

    if( 0 >= percentageRssi)
    {
        send_cmd.data[0] = FALSE;
        send_cmd.data[1] = 0;
    }
    else
    {
        send_cmd.data[0] = TRUE;
        send_cmd.data[1] = percentageRssi;
    }
    
    if(serialCommandGenerate( &send_cmd, send_buf, &send_buf_len))
    {
        sendQueuePush(send_buf, send_buf_len, send_cmd.seq_num, FALSE);
    }
}

/**
 * serialTimerReqCmdSend
 * serial timer request command send to MCU
 * @param[in]   data: data request command payload
 * @param[in]   data_len: data request command payload len
 * @return      true or false
 */
static bool_t serialTimerReqCmdSend(uint8_t* data, uint8_t data_len)
{
    serial_protocol_cmd_t send_cmd;
    uint8_t send_buf[MAX_SERIAL_PAYLOAD_SIZE];
    uint8_t send_buf_len = 0;
    uint16_t seqNum = 0;
    uint8_t* ptr= NULL;

    if(NULL == data)
    {
        return FALSE;
    }

    memset(&send_cmd, 0, sizeof(serial_protocol_cmd_t));
    memset(&send_buf, 0, MAX_SERIAL_PAYLOAD_SIZE);

    send_cmd.cmd_hdr = SERIAL_PROTOCOL_HDR;
    send_cmd.version = SERIAL_PROTOCOL_VER;
    send_cmd.seq_num = serialProtocolSeqNumGet();
    send_cmd.cmd = TIME_GET_CMD;

    ptr = data;/*the start three byte is frame control, sequence and command id*/
    seqNum = *ptr << 8 ;
    ptr++;
    seqNum |= *ptr;
    ptr++;

    send_cmd.seq_num = seqNum;
    send_cmd.data_len = data_len - 2;

    memcpy(send_cmd.data, ptr, send_cmd.data_len);

    if(serialCommandGenerate( &send_cmd, send_buf, &send_buf_len))
    {
        sendQueuePush(send_buf, send_buf_len, send_cmd.seq_num, FALSE);
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

/**
 * serialTimerReqCmdSend
 * serial timer request command send to MCU
 * @param[in]   data: data request command payload
 * @param[in]   data_len: data request command payload len
 * @return      true or false
 */
static bool_t serialTimerReqCmdReceive(uint8_t* data, uint8_t data_len)
{
    if((NULL == data) || (data_len < 10))
    {
        return FALSE;
    }

    if(local_time.current_time == 0 || local_time.utc_time == 0)
    {
        serialTimerReqCmdSend(data, data_len);
    }

    local_time.utc_time = data[2] << 24 \
                  | data[3] << 16 \
                  | data[4] << 8 \
                  | data[5] ;
    local_time.current_time = data[6] << 24 \
                  | data[7] << 16 \
                  | data[8] << 8 \
                  | data[9] ;
    local_time.last_tick = dev_current_millisecond_ticks_get();

    return TRUE;
}

/**
 * serialSceneSwitchNumGetSend
 * send scene switch num get command to MCU
 * @param[in]   NULL
 * @return      NULL
 */
void serialSceneSwitchNumGetSend(void)
{
    serial_protocol_cmd_t send_cmd;
    uint8_t send_buf[MAX_SERIAL_PAYLOAD_SIZE];
    uint8_t send_buf_len = 0;

    memset(&send_cmd, 0, sizeof(serial_protocol_cmd_t));
    memset(&send_buf, 0, MAX_SERIAL_PAYLOAD_SIZE);

    send_cmd.cmd_hdr = SERIAL_PROTOCOL_HDR;
    send_cmd.version = SERIAL_PROTOCOL_VER;
    send_cmd.seq_num = serialProtocolSeqNumGet();
    send_cmd.cmd = SCENE_SWITCH_NUM_GET_CMD;
    send_cmd.data_len = 0;

    if(serialCommandGenerate( &send_cmd, send_buf, &send_buf_len))
    {
        sendQueuePush(send_buf, send_buf_len, send_cmd.seq_num, TRUE);
    }
}

/**
 * serialSceneSwitchIdReportRspSend
 * send scene switch Id report response to MCU
 * @param[in]   result 0:recall scene fail, 1:recall scene success
 * @return      NULL
 */
static void serialSceneSwitchIdReportRspSend(bool_t result)
{
    serial_protocol_cmd_t send_cmd;
    uint8_t send_buf[MAX_SERIAL_PAYLOAD_SIZE];
    uint8_t send_buf_len = 0;

    memset(&send_cmd, 0, sizeof(serial_protocol_cmd_t));
    memset(&send_buf, 0, MAX_SERIAL_PAYLOAD_SIZE);

    send_cmd.cmd_hdr = SERIAL_PROTOCOL_HDR;
    send_cmd.version = SERIAL_PROTOCOL_VER;
    send_cmd.seq_num = serialProtocolSeqNumGet();
    send_cmd.cmd = SCENE_SWITCH_ID_REPORT_CMD;
    send_cmd.data_len = 1;
    send_cmd.data[0] = result;

    if(serialCommandGenerate( &send_cmd, send_buf, &send_buf_len))
    {
        sendQueuePush(send_buf, send_buf_len, send_cmd.seq_num, FALSE);
    }
}

/**
 * serialMcuOtaVerReqCmdSend
 * serial mcu ota version request command send to MCU
 * @param[in]   NULL
 * @param[in]   NULL
 * @return      true or false
 */
static bool_t serialMcuOtaVerReqCmdSend(void)
{
    serial_protocol_cmd_t send_cmd;
    uint8_t send_buf[MAX_SERIAL_PAYLOAD_SIZE];
    uint8_t send_buf_len = 0;

    memset(&send_cmd, 0, sizeof(serial_protocol_cmd_t));
    memset(&send_buf, 0, MAX_SERIAL_PAYLOAD_SIZE);

    send_cmd.cmd_hdr = SERIAL_PROTOCOL_HDR;
    send_cmd.version = SERIAL_PROTOCOL_VER;
    send_cmd.seq_num = serialProtocolSeqNumGet();
    send_cmd.cmd = MCU_OTA_VER_REQ_CMD;
    send_cmd.data_len = 0;

    if(mcu_version > 0)
    {
        uint8_t data[4] = {0};
        uint8_t data_len = 0;

        data[data_len++] = 0x00;
        data[data_len++] = 0x01;
        data[data_len++] = mcu_version;
        serialMcuOtaDataSendCallback(MCU_OTA_VER_REQ_CMD, data, data_len);
    }

    if(serialCommandGenerate( &send_cmd, send_buf, &send_buf_len))
    {
        sendQueuePush(send_buf, send_buf_len, send_cmd.seq_num, TRUE);
    }

    return TRUE;
}

/**
 * serialMcuOtaNotifyCmdSend
 * serial ota notify command send to MCU
 * @param[in]   data: data request command payload
 * @param[in]   data_len: data request command payload len
 * @return      true or false
 */
static bool_t serialMcuOtaNotifyCmdSend(uint8_t* data, uint8_t data_len)
{
    serial_protocol_cmd_t send_cmd;
    uint8_t send_buf[MAX_SERIAL_PAYLOAD_SIZE];
    uint8_t send_buf_len = 0;

    if((NULL == data) ||(0 == data_len))
    {
        return FALSE;
    }

    memset(&send_cmd, 0, sizeof(serial_protocol_cmd_t));
    memset(&send_buf, 0, MAX_SERIAL_PAYLOAD_SIZE);

    send_cmd.cmd_hdr = SERIAL_PROTOCOL_HDR;
    send_cmd.version = SERIAL_PROTOCOL_VER;
    send_cmd.seq_num = serialProtocolSeqNumGet();
    send_cmd.cmd = MCU_OTA_NOTIFY_CMD;

    /*payload first and second byte is sequence number in version 2*/
    uint16_t seqNum = 0;
    uint8_t* ptr= NULL;

    ptr = data;/*the start three byte is frame control, sequence and command id*/
    seqNum = *ptr << 8 ;
    ptr++;
    seqNum |= *ptr;
    ptr++;

    send_cmd.seq_num = seqNum;
    send_cmd.data_len = data_len - 2;
    memcpy(send_cmd.data, ptr, send_cmd.data_len);

    if(serialCommandGenerate( &send_cmd, send_buf, &send_buf_len))
    {
        sendQueuePush(send_buf, send_buf_len, send_cmd.seq_num, FALSE);
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

/**
 * serialMcuOtaDataRspCmdSend
 * serial ota data response command send to MCU
 * @param[in]   data: data request command payload
 * @param[in]   data_len: data request command payload len
 * @return      true or false
 */
static bool_t serialMcuOtaDataRspCmdSend(uint8_t* data, uint8_t data_len)
{
    serial_protocol_cmd_t send_cmd;
    uint8_t send_buf[MAX_SERIAL_PAYLOAD_SIZE];
    uint8_t send_buf_len = 0;

    if((NULL == data) ||(0 == data_len))
    {
        return FALSE;
    }

    memset(&send_cmd, 0, sizeof(serial_protocol_cmd_t));
    memset(&send_buf, 0, MAX_SERIAL_PAYLOAD_SIZE);

    send_cmd.cmd_hdr = SERIAL_PROTOCOL_HDR;
    send_cmd.version = SERIAL_PROTOCOL_VER;
    send_cmd.seq_num = serialProtocolSeqNumGet();
    send_cmd.cmd = MCU_OTA_DATA_REQ_CMD;

    /*payload first and second byte is sequence number in version 2*/
    uint16_t seqNum = 0;
    uint8_t* ptr= NULL;

    ptr = data;/*the start three byte is frame control, sequence and command id*/
    seqNum = *ptr << 8 ;
    ptr++;
    seqNum |= *ptr;
    ptr++;

    send_cmd.seq_num = seqNum;
    send_cmd.data_len = data_len - 2;
    memcpy(send_cmd.data, ptr, send_cmd.data_len);

    if(serialCommandGenerate( &send_cmd, send_buf, &send_buf_len))
    {
        sendQueuePush(send_buf, send_buf_len, send_cmd.seq_num, FALSE);
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

/** 
 * productInfoReqRspCmdHandler
 * handle the product information request or response command
 * @param[in]   receive_cmd  received data command from the uart
 * @return      NULL
 */
static void productInfoReqRspCmdHandler(serial_protocol_cmd_t* receive_cmd)
{
    uint8_t len = 0;
    char productPid[DEFAULT_PID_LEN + 1] = {0};
    char productVer[DEFAULT_VER_LEN + 1] = {0};
    char tokType_manufacturer_name[MAX_MANU_NAME_SIZE] = {0};

    if(receive_cmd->data[receive_cmd->data_len - 1] != 0x7D) //0x7D is '}'
    {
        receive_cmd->data[receive_cmd->data_len - 1] = 0; // last byte is mcu ota flag
    }

    jsmntok_t *pid_tok = json_get_value((char *)(receive_cmd->data), "p");
    jsmntok_t *ver_tok = json_get_value((char *)(receive_cmd->data), "v");
    if((pid_tok == NULL) || (ver_tok == NULL))
    {
        emberSerialProtocolPrintln("cjson get p object error");
        return;
    }

    /*get the pid info*/
    len = pid_tok->end - pid_tok->start;
    if(len > DEFAULT_PID_LEN || len == 0)
    {
        return;
    }
    
    if(pid_tok->type == JSMN_STRING)
    {
        memcpy(productPid, receive_cmd->data + pid_tok->start, len);
    }

    /*get the version info*/
    len = ver_tok->end - ver_tok->start;
    if(len > DEFAULT_PID_LEN || len == 0)
    {
        return;
    }
    
    if(ver_tok->type == JSMN_STRING) 
    {
        memcpy(productVer, receive_cmd->data + ver_tok->start, len);
    }

    //write the pid to basic manufacture name attribute 9-16bit
    app_print("productPid is: %s \r\n", productPid);
    app_print("productVer is: %s \r\n", productVer);

    dev_zigbee_read_attribute(0x01, CLUSTER_BASIC_CLUSTER_ID, \
                ATTR_MANUFACTURER_NAME_ATTRIBUTE_ID, \
                tokType_manufacturer_name,\
                MAX_MANU_NAME_SIZE - 1);
    memcpy(&tokType_manufacturer_name[9], productPid, DEFAULT_PID_LEN);

    dev_zigbee_write_attribute(0x01, CLUSTER_BASIC_CLUSTER_ID, \
                ATTR_MANUFACTURER_NAME_ATTRIBUTE_ID, \
                tokType_manufacturer_name,\
                ATTR_CHAR_STRING_ATTRIBUTE_TYPE);

    /*read for confirm*/
    memset(tokType_manufacturer_name, 0, MAX_MANU_NAME_SIZE);
    dev_zigbee_read_attribute(0x01, CLUSTER_BASIC_CLUSTER_ID, \
                ATTR_MANUFACTURER_NAME_ATTRIBUTE_ID, \
                tokType_manufacturer_name,\
                MAX_MANU_NAME_SIZE - 1);
    app_print("tokType_manufacturer_name is: %s \r\n", tokType_manufacturer_name);

    mcu_version = (my_atoi(&productVer[0]) << 6) & 0xC0;
    mcu_version += (my_atoi(&productVer[2]) << 4) & 0x30;
    mcu_version += my_atoi(&productVer[4]) & 0x0F;
    
    deviceWorkStateSet(NWK_STATE_OK);
    /*stop the retransmission event and clean the retransmisson time*/
    sendQueueUpdate(productInfoSeqNum);
}

/** 
 * nwkInfoReqRspCmdHandler
 * handle the network information request or response command
 * @param[in]   receive_cmd  received data command from the uart
 * @return      NULL
 */
static void nwkInfoReqRspCmdHandler(serial_protocol_cmd_t* receive_cmd)
{
    /*stop the retransmission event and clean the retransmisson time*/
    sendQueueUpdate(nwkInfoSeqNum);
    
    return;
}

/** 
 * serialDevResetCmdHandler
 * handle the serial device reset command
 * @param[in]   receive_cmd  received data command from the uart
 * @return      NULL
 */
static void serialDevCfgCmdHandler(serial_protocol_cmd_t* receive_cmd)
{
    /*reset the zigbee device or start the zigbee device to join network*/
    if(1 != receive_cmd->data_len)
    {
        return;
    }

    serialDeviceConfigRspSend(receive_cmd->seq_num);
    serialDeviceConfigCallback(receive_cmd->data[0]);
}

/** 
 * serialDataRspSyncCmdHandler
 * handle the serial data response command
 * @param[in]  sync true: synchronize response; false:asynchronize response;
 *             receive_cmd  received data command from the uart
 * @return      NULL
 */
static void serialDataRspCmdHandler(bool_t sync, serial_protocol_cmd_t* receive_cmd)
{
    //uint16_t reqSeq = 0;

    if((0 >= receive_cmd->data_len) || (MAX_ZIGBEE_PAYLOAD_SIZE + 2 < receive_cmd->data_len))
    {
        return;
    }

    if(SERIAL_PROTOCOL_VER >= 2)
    {
        if(MAX_ZIGBEE_PAYLOAD_SIZE > receive_cmd->data_len - 2)
        {
            uint8_t data[MAX_SERIAL_PAYLOAD_SIZE] = {0};

            data[0] = (receive_cmd->seq_num >> 8) & 0xff;
            data[1] = receive_cmd->seq_num & 0xff;

            memcpy(&data[2], receive_cmd->data, receive_cmd->data_len);
            receive_cmd->data_len += 2;
            memcpy(receive_cmd->data, data, receive_cmd->data_len);
        }
        else
        {
            app_print("payload is too long");
            return;
        }
    }

    /*update the retransmission data from the queuse*/
    sendQueueUpdate(dataReqSeqNum);

    serialDataRspSendCallback(sync, receive_cmd->data, receive_cmd->data_len);
}

/** 
 * serialTimeReqCmdHanlder
 * handle the serial time request command
 * @param[in]   receive_cmd  received data command from the uart
 * @return      NULL
 */
static void serialTimeReqCmdHanlder(serial_protocol_cmd_t* receive_cmd)
{
    if(( NULL == receive_cmd )||(receive_cmd->data_len != 0))
    {
        return; //time request command has no payload
    }

    if(SERIAL_PROTOCOL_VER >= 2)
    {
        if(MAX_SERIAL_PAYLOAD_SIZE >= receive_cmd->data_len - 2)
        {
            uint8_t data[MAX_SERIAL_PAYLOAD_SIZE] = {0};

            data[0] = (receive_cmd->seq_num >> 8) & 0xff;
            data[1] = receive_cmd->seq_num & 0xff;

            memcpy(&data[2], receive_cmd->data, receive_cmd->data_len);
            receive_cmd->data_len += 2;
        }
        else
        {
            app_print("payload is too long");
            return;
        }

         uint32_t tick = 0, utc_time = 0, current_time = 0;
        uint8_t data[10] = {0};
        uint8_t data_len = 0;

        if(dev_cfg.app_type != HA_SCENE_SELECTOR)
        {
            if(local_time.utc_time != 0 && local_time.current_time != 0)
            {
                uint32_t tick = 0, utc_time = 0, current_time = 0;
                uint32_t sys_tick = dev_current_millisecond_ticks_get();
                uint8_t data[10] = {0};
                uint8_t data_len = 0;

                if(sys_tick > local_time.last_tick)
                {
                    tick = (sys_tick - local_time.last_tick) / 1000;
                }
                else
                {
                    tick = (0xFFFFFFFF - local_time.last_tick + sys_tick) / 1000;
                }

                utc_time = local_time.utc_time + tick;
                current_time = local_time.current_time + tick;

            }
            else
            {
                serialProtocolTimeReqCallback(receive_cmd->data, receive_cmd->data_len);
            }
        }
        else
        {
            local_time.utc_time = 0;
            local_time.current_time = get_current_time() + 0x386CD300; //uinx time is start with 1970.1.1, zigbee time is start with 2000.1.1, the dif is 946,656,000
        }

        utc_time = local_time.utc_time + tick;
        current_time = local_time.current_time + tick;
        data[data_len++] = (receive_cmd->seq_num >> 8) & 0xff;
        data[data_len++] = receive_cmd->seq_num & 0xff;
        data[data_len++] = utc_time >> 24;
        data[data_len++] = utc_time >> 16 & 0xFF;
        data[data_len++] = utc_time >> 8 & 0xFF;
        data[data_len++] = utc_time  & 0xFF;
        data[data_len++] = current_time >> 24;
        data[data_len++] = current_time >> 16 & 0xFF;
        data[data_len++] = current_time >> 8 & 0xFF;
        data[data_len++] = current_time  & 0xFF;

        serialTimerReqCmdSend( data, data_len);
    }
}

static void dev_serial_rf_recv_callback(uint8_t *packet, uint8_t lqi, int8_t rssi)
{
    uint8_t payload_len = packet[0];

    if((payload_len < 3) ||(packet == NULL) )
    {
        app_print("invalid data");
        return;
    }

    if(!memcmp((void*)packet_payload, (void*)&packet[1], payload_len - 3))
    {
        total_receive_num++;
    }

    if(total_receive_num >= total_send_num)
    {
        dev_timer_stop(emSerialProtocolScanRssiEventControl);
        dev_timer_start_with_callback(emSerialProtocolScanRssiTimeOutEventControl, 1000, serial_protocol_event_handler);
    }
}

/** 
 * serialFuncTestCmdHanlder
 * handle the serial function test request command
 * @param[in]   receive_cmd  received data command from the uart
 * @return      NULL
 */
static void serialFuncTestReqCmdHanlder(serial_protocol_cmd_t* receive_cmd)
{
    /*network scan*/
    uint8_t scan_channel = receive_cmd->data[0];
    
    if((NET_JOIN_OK == nwk_state_get()) || (NET_REJOIN_OK == nwk_state_get()))
    {
        app_print("nwk state error \n\r");
        return;
    }

    memset((void*)&rf_test, 0, sizeof(serial_protocol_rf_test_t));
    serialProtocolScanRssiFlagSet(TRUE);
    rf_test.channel = scan_channel;
    total_send_num = SERIAL_RF_SEND_NUM;
    
    if(total_send_num > 0)
    {
        dev_timer_start_with_callback(emSerialProtocolScanRssiEventControl, SERIAL_PROTOCOL_SCAN_RSSI_PERIOD, serial_protocol_event_handler);
    }
    else
    {
      serialFuncTestRssiRspSend(FALSE);
      return;
    }

    /*start the timeout event when send first package*/
    if(total_receive_num == 0)
    {
        dev_timer_start_with_callback(emSerialProtocolScanRssiTimeOutEventControl, SERIAL_PROTOCOL_SCAN_RSSI_TIMEOUT, serial_protocol_event_handler);
    }
}

/** 
 * serialSceneSwitchNumGetCmdHanlder
 * handle the serial scene switch number get command
 * @param[in]   receive_cmd  received data command from the uart
 * @return      NULL
 */
static void serialSceneSwitchNumGetCmdHanlder(serial_protocol_cmd_t* receive_cmd)
{
    /*total scene switch number*/
    uint8_t sceneSwitchNum ;

    if(NULL == receive_cmd)
    {
        return;
    }

    if(receive_cmd->data_len <=0 || receive_cmd->data_len > 3)
    {
        return;
    }

    sceneSwitchNum = receive_cmd->data[0];
    
    if(sceneSwitchNum > 0)
    {
        serialTotalSceneSwitchNumSet( sceneSwitchNum );
        serialTotalSceneSwitchNumCallback( sceneSwitchNum );
    }
    else
    {
        emberSerialProtocolPrintln("scene switch number is error");
    }
    sendQueueUpdate(sceneSwitchNumGetSeqNum);
}

/**
 * serialSceneSwitchIdReportCmdHanlder
 * handle the serial scene switch id report command
 * @param[in]   receive_cmd  received data command from the uart
 * @return      NULL
 */
static void serialSceneSwitchIdReportCmdHanlder(serial_protocol_cmd_t* receive_cmd)
{
    /*network scan*/
    uint8_t switchId = 0;

    if(NULL == receive_cmd)
    {
        return;
    }

    if(receive_cmd->data_len <=0 || receive_cmd->data_len > 3)
    {
        return;
    }

    switchId = receive_cmd->data[0];

    if(switchId > 0)
    {
        serialSceneSwitchRecallCallback(switchId);
    }

    serialSceneSwitchIdReportRspSend(TRUE);
}

/**
 * serialMcuOtaCmdHanlder
 * handle the mcu ota version request command
 * @param[in]   receive_cmd  received data command from the uart
 * @return      NULL
 */
static void serialMcuOtaCmdHanlder(uint8_t receive_cmd_id, serial_protocol_cmd_t* receive_cmd)
{
    //uint16_t reqSeq = 0;
    uint8_t send_cmd_id = 0;
    
    if((0 >= receive_cmd->data_len) || (MAX_SERIAL_PAYLOAD_SIZE < receive_cmd->data_len))
    {
        return;
    }

    if(MAX_SERIAL_PAYLOAD_SIZE > receive_cmd->data_len - 2)
    {
        uint8_t data[MAX_SERIAL_PAYLOAD_SIZE] = {0};

        data[0] = (receive_cmd->seq_num >> 8) & 0xff;
        data[1] = receive_cmd->seq_num & 0xff;

        memcpy(&data[2], receive_cmd->data, receive_cmd->data_len);
        receive_cmd->data_len += 2;
        memcpy(receive_cmd->data, data, receive_cmd->data_len);
    }
    else
    {
        app_print("payload is too long");
        return;
    }

    /*update the retransmission data from the queuse*/
    sendQueueUpdate(dataReqSeqNum);

    if(MCU_OTA_VER_REQ_CMD == receive_cmd_id)
    {
        send_cmd_id = SERIAL_PROTOCOL_MCU_OTA_VER_RSP;
        if((mcu_version != receive_cmd->data[2]) || (mcu_version == 0))
        {
            mcu_version = receive_cmd->data[2];
        }
        else
        {
            return;
        }
    }
    else if(MCU_OTA_DATA_REQ_CMD == receive_cmd_id)
    {
        send_cmd_id = SERIAL_PROTOCOL_MCU_OTA_DATA_REQ;
    }
    else if(MCU_OTA_END_CMD == receive_cmd_id) //end command need to send an ack to mcu
    {
        serial_protocol_cmd_t send_cmd;
        uint8_t send_buf[MAX_SERIAL_PAYLOAD_SIZE];
        uint8_t send_buf_len = 0;

        send_cmd_id = SERIAL_PROTOCOL_MCU_OTA_END;
        memset(&send_cmd, 0, sizeof(serial_protocol_cmd_t));
        memset(&send_buf, 0, MAX_SERIAL_PAYLOAD_SIZE);

        send_cmd.cmd_hdr = SERIAL_PROTOCOL_HDR;
        send_cmd.version = SERIAL_PROTOCOL_VER;
        send_cmd.seq_num = receive_cmd->seq_num;
        send_cmd.cmd = MCU_OTA_END_CMD;
        
        send_cmd.data[0] = 0x00;
        send_cmd.data_len++;

        if(serialCommandGenerate( &send_cmd, send_buf, &send_buf_len))
        {
            sendQueuePush(send_buf, send_buf_len, send_cmd.seq_num, FALSE);
        }
    }
    else
    {
        /* code */
        app_print("receive cmd err \n\r");
        return;
    }
    
    serialMcuOtaDataSendCallback(send_cmd_id, receive_cmd->data, receive_cmd->data_len);
}

// /**
//  * serialMcuOtaNotifyCmdHanlder
//  * handle the mcu ota version request command
//  * @param[in]   receive_cmd  received data command from the uart
//  * @return      NULL
//  */
// static void serialMcuOtaNotifyCmdHanlder(serial_protocol_cmd_t* receive_cmd)
// {
//     /*network scan*/
//     uint8_t status = receive_cmd->data[0];
    
//     if(NULL == receive_cmd)
//     {
//         return;
//     }
    
//     if(status == 0)
//     {
//         app_print("status is success \n\r");
//     }
//     else
//     {
//         /* code */
//         app_print("status is fail \n\r");
//     } 
// }

// /**
//  * serialMcuOtaDataReqCmdHanlder
//  * handle the mcu ota data request command
//  * @param[in]   receive_cmd  received data command from the uart
//  * @return      NULL
//  */
// static void serialMcuOtaDataReqCmdHanlder(serial_protocol_cmd_t* receive_cmd)
// {
//     /*network scan*/
//     uint8_t mcuVersion = receive_cmd->data[0];
//     serial_protocol_cmd_t send_cmd;
//     uint8_t send_buf[MAX_SERIAL_PAYLOAD_SIZE];
//     uint8_t send_buf_len = 0;

//     if(NULL == receive_cmd)
//     {
//         return;
//     }

//     memset(&send_cmd, 0, sizeof(serial_protocol_cmd_t));
//     memset(&send_buf, 0, MAX_SERIAL_PAYLOAD_SIZE);

//     send_cmd.cmd_hdr = SERIAL_PROTOCOL_HDR;
//     send_cmd.version = SERIAL_PROTOCOL_VER;
//     send_cmd.seq_num = serialProtocolSeqNumGet();
//     send_cmd.cmd = MCU_OTA_DATA_REQ_CMD;
//     send_cmd.data_len = 1;
//     send_cmd.data[0] = receive_cmd->data[0];

//     if(serialCommandGenerate( &send_cmd, send_buf, &send_buf_len))
//     {
//         sendQueuePush(send_buf, send_buf_len, send_cmd.seq_num, FALSE);
//     }
// }

/** 
 * emSerialProtocolScanRssiEventHandler
 * emSerialProtocolScanRssiEventControl handle function
 * @param[in]   NULL
 * @return      NULL
 */
void emSerialProtocolScanRssiEventHandler(void)
{
    static uint8_t num = 0;

    dev_timer_stop(emSerialProtocolScanRssiEventControl);
    serialProtocolScanRssiFlagSet(FALSE);

    num++;
    
    if(total_send_num <= num)
    {
        num = 0;
        return;
    }
    else
    {
        uint8_t packet_len = 0 ;

        packet_len = sizeof(packet_payload);

        memcpy((void*)rf_test.data, (void*)packet_payload, packet_len);
        rf_test.data_len = packet_len;
        rf_test.received_callback = dev_serial_rf_recv_callback;

        dev_sys_data_send_mac((serial_protocol_rf_test_t*)&rf_test);

        dev_timer_start_with_callback(emSerialProtocolScanRssiEventControl, SERIAL_PROTOCOL_SCAN_RSSI_PERIOD, serial_protocol_event_handler);
        
        if(1 == num)
        {
            /*start a timer delay event when the packet first send */
            dev_timer_start_with_callback(emSerialProtocolScanRssiTimeOutEventControl, SERIAL_PROTOCOL_SCAN_RSSI_TIMEOUT, serial_protocol_event_handler);
        }
    }
}

/** 
 * emSerialProtocolScanRssiTimeOutEventHandler
 * Serial Protocol Scan Rssi Timeout Event handle function
 * @param[in]   NULL
 * @return      NULL
 */
void emSerialProtocolScanRssiTimeOutEventHandler(void)
{
    app_print("total receive: %d \n\r", total_receive_num);

    dev_timer_stop(emSerialProtocolScanRssiTimeOutEventControl);
    
    if(total_receive_num > total_send_num)
    {
        total_receive_num = total_send_num;
    }

    serialFuncTestRssiRspSend(total_receive_num * 100 / total_send_num);
    // emberAfMfglibStart(0);
    total_receive_num = 0;
}

/****************************************************************************
* name      :   serialProtocolRfReceiveProcess
* funtion   :   serial protocol RF received message process
* parameters:   packet: received message data, the first bytes is message len
* return    :   NULL
* call      :   mfglibRxHandler()
****************************************************************************/ 
void serialProtocolRfReceiveProcess(uint8_t* packet)
{
    unsigned short payload_header = 0x0000;
    uint8_t* ptr = NULL;
    uint8_t payload_len = *packet;
    
    if((payload_len < 2) || (NULL == packet))
    {
    	emberSerialProtocolPrintln("received err");
        return;
    }
    ptr = packet;
    ptr++; /*the first byte is the data length*/
    payload_header = (((unsigned short)(*ptr))<<8) | (*(ptr+1));

    if(payload_header != SERIAL_PROTOCOL_HDR)
    {
        return;
    }

    total_receive_num++;

    if(total_receive_num >= total_send_num)
    {
        /*Receive completed, stop the timer delay send event and timeout event*/
        dev_timer_stop(emSerialProtocolScanRssiEventControl);
        dev_timer_stop(emSerialProtocolScanRssiTimeOutEventControl);
        serialFuncTestRssiRspSend(100);
        // emberAfMfglibStart(0);
        total_receive_num = 0;
    }
}

/** 
 * serialCommandRxHandler
 * generate serial command to the send buffer
 * @param[in]   payload  received data payload from the uart
 * @param[in]   payload_len  received data payload length from the uart
 * @return      NULL
 */
void serialCommandRxHandler(serial_protocol_cmd_t* receive_cmd)
{
    app_print("rx handler, cmd=%d \r\n", receive_cmd->cmd);
    switch(receive_cmd->cmd) {
        case PRODUCT_INFO_CMD:
        {
            productInfoReqRspCmdHandler( receive_cmd );
        }
        break;

        case ZIGBEE_STATE_CMD:
        {
            nwkInfoReqRspCmdHandler( receive_cmd );
        }
        break;

        case ZIGBEE_CFG_CMD:
        {
            serialDevCfgCmdHandler( receive_cmd );
        }
        break;

        case ZIGBEE_DATA_REQ_CMD:
        {
            sendQueueUpdate(receive_cmd->seq_num);
        }
        break;

        case DATA_RSP_SYNC_CMD:
        {
            serialDataRspCmdHandler( TRUE, receive_cmd );
        }
        break;
        
        case DATA_RSP_ASYNC_CMD:
        {
            serialDataRspCmdHandler( FALSE, receive_cmd );
        }
        break;


        case FUNC_TEST_CMD:
        {
            serialFuncTestReqCmdHanlder( receive_cmd );
        }
        break;
        
        case SCENE_SWITCH_NUM_GET_CMD:
        {
            serialSceneSwitchNumGetCmdHanlder( receive_cmd );
        }
        break;

        case SCENE_SWITCH_ID_REPORT_CMD:
        {
            serialSceneSwitchIdReportCmdHanlder( receive_cmd );
        }
        break;

        case MCU_OTA_VER_REQ_CMD:
        {
            serialMcuOtaCmdHanlder( MCU_OTA_VER_REQ_CMD, receive_cmd );
        }
        break;

        case MCU_OTA_NOTIFY_CMD:
        {
            serialMcuOtaCmdHanlder( MCU_OTA_NOTIFY_CMD, receive_cmd );
        }
        break;

        case MCU_OTA_DATA_REQ_CMD:
        {
            zg_ota_evt_callback(ZG_OTA_EVT_START);
            dev_timer_start_with_callback(serialMcuOtaPollrateTimeoutEventControl, \
                    SERIAL_POLL_TIMEOUT, \
                    serial_protocol_event_handler);
            serialMcuOtaCmdHanlder( MCU_OTA_DATA_REQ_CMD, receive_cmd );
        }
        break;

        case MCU_OTA_END_CMD:
        {
            zg_ota_evt_callback(ZG_OTA_EVT_DOWNLOAD_AND_VERIFY_SUCCESS);
            serialMcuOtaCmdHanlder( MCU_OTA_END_CMD, receive_cmd );
        }
        break;
      
        case TIME_GET_CMD:
        {
            serialTimeReqCmdHanlder( receive_cmd );
        }
        break;
        
        default:
        break;
    }
}

/** 
 * serialProtocolScanRssiResultCallback
 * send the scan rssi result to the mcu
 * @param[in]   NULL
 * @return      NULL
 */
void serialProtocolScanRssiResultCallback(int8 rssi)
{
    emberSerialProtocolPrintln("rssi is: %d", rssi);

    serial_protocol_cmd_t send_cmd;
    uint8_t send_buf[MAX_SERIAL_PAYLOAD_SIZE];
    uint8_t send_buf_len = 0;

    memset(&send_cmd, 0, sizeof(serial_protocol_cmd_t));
    memset(&send_buf, 0, MAX_SERIAL_PAYLOAD_SIZE);

    send_cmd.cmd_hdr = SERIAL_PROTOCOL_HDR;
    send_cmd.version = SERIAL_PROTOCOL_VER;
    send_cmd.cmd = FUNC_TEST_CMD;

    send_cmd.data[0] = rssi;
    send_cmd.data_len++;

    if(serialCommandGenerate( &send_cmd, send_buf, &send_buf_len))
    {
        sendQueuePush(send_buf, send_buf_len, send_cmd.seq_num, FALSE);
    }
}

/** 
 * emberAfSerialProtocolCommandParseCallback
 * Cluster: serial protocol private cluster command parse callback
 * @param[in]   cmd): received command
 * @return      NULL
 */
uint8_t emberAfSerialProtocolCommandParseCallback(dev_msg_t *cmd)
{
    uint8_t *buffer = NULL;
    uint8_t bufferLen = 0;
    bool_t status = FALSE;

    buffer = cmd->data.bare_data.data;/*the start three byte is frame control, sequence and command id*/
    bufferLen = cmd->data.bare_data.len;

    switch (cmd->data.bare_data.commandId)
    {
        case SERIAL_PROTOCOL_DATA_REQ:
            status = serialDataReqCmdSend(buffer, bufferLen);
        break;

        case SERIAL_PROTOCOL_DATA_RSP:
            status = TRUE;
        break;

        case SERIAL_PROTOCOL_DATA_REPORT:
            status = TRUE;
        break;

        case SERIAL_PROTOCOL_DATA_QUERY:
            nwkInfoReqCmdSend(NWK_JOINED);
        break;

        case SERIAL_PROTOCOL_TIME_REQ:
            status = serialTimerReqCmdReceive(buffer, bufferLen);
        break;

        case SERIAL_PROTOCOL_MCU_OTA_VER_REQ:
            status = serialMcuOtaVerReqCmdSend();
        break;

        case SERIAL_PROTOCOL_MCU_OTA_NOTIFY:
            status = serialMcuOtaNotifyCmdSend(buffer, bufferLen);
        break;

        case SERIAL_PROTOCOL_MCU_OTA_DATA_RSP:
            status = serialMcuOtaDataRspCmdSend(buffer, bufferLen);
        break;

        default:
        break;
    }
    if(TRUE == status)
    {
       return ZCL_CMD_RET_SUCCESS;
    }
    else
    {
        return ZCL_CMD_RET_FAILED;
    }
}

/**
 * @description: serial protocol event handler function 
 * @param in: evt: event id
 * @param out: none
 * @return: none
 */
void serial_protocol_event_handler(uint8_t evt)
{
    app_print("evt id = %d \r\n", evt);
    switch(evt)
    {
        case emSerialProtocolScanRssiEventControl:
            emSerialProtocolScanRssiEventHandler();
        break;

        case emSerialProtocolScanRssiTimeOutEventControl:
            emSerialProtocolScanRssiTimeOutEventHandler();
        break;

        case serialMcuOtaPollrateTimeoutEventControl:
            serial_mcu_ota_pollrate_timeout_handler();
        break;

        default:
        break;
    }
}
