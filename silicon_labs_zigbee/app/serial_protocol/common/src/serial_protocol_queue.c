/*
 * @Author: Leon
 * @email: zhangpeng@tuya.com
 * @LastEditors: Leon
 * @file name: serial_protocol.c
 * @Description: serial protocol used by TuYa ZigBee modules and MCU communicated by UART
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-03-13 17:20:41
 * @LastEditTime: 2019-03-31 13:42:40
 */

#define __MF_TEST_GLOBALS
#include <stdio.h>
#include <string.h>
#include <time.h>
#pragma pack()
#include "serial_protocol_queue.h"
#include "serial_protocol.h"
#include "serial_protocol_callbacks.h"
#include "zigbee_sdk.h"
#include "oem_util.h"

extern void emberSerialProtocolMcuSleepCallback();
extern void emberSerialProtocolMcuWakeUpCallback(void);

extern oem_dev_t dev_cfg;
static send_queue_t sendQueue;

/** 
 * sendQueueInit
 * generate serial command to the send buffer
 * @param[in]   NULL
 * @param[out]  NULL
 * @return      NULL
 */
void sendQueueInit(void)
{
    memset((void*)&sendQueue, 0, sizeof(send_queue_t));
    //emberSerialProtocolMcuSleepCallback();
}

/** 
 * sendQueueInit
 * generate serial command to the send buffer
 * @param[in]  data  add to queue data buffer
 * @param[in]  dataLen  add to queue  length
 * @param[in]  seqNum  queuence number of current data
 * @param[in]  retransmitFlag  retransmit flag of current data
 * @return     TRUE or FALSE
 */
//#define NO_SLEEPED_DEVICE
bool_t sendQueuePush(uint8_t* data, uint8_t dataLen, uint16_t seqNum, bool_t retransmitFlag)
{
    uint8_t index = sendQueue.tail;

    if(dev_cfg.app_type != HA_COMMON_DEV_SLEEP)
    {
        user_uart_send(SERIAL_PROTOCOL_UART_ID, data, dataLen);
        return 0;
    }

    if(sendQueue.tail >= MAX_QUEUE_ITEM)
    {
        sendQueueCheckCallback(); /*check send queue periods*/
        return FALSE; /*queue table full*/
    }
    if(sendQueue.currentPushOffset + dataLen >= MAX_QUEUE_PAYLOAD_SIZE)
    {
        sendQueueCheckCallback(); /*check send queue periods*/
        return FALSE; /*queue table full*/
    }
    
    if(TRUE == retransmitFlag)
    {
        sendQueue.retransmitTime[index] = MAX_RETRY_TIME;
    }
    else
    {
        sendQueue.retransmitTime[index] = 1;
    }

    sendQueue.index_len[index] = dataLen;
    sendQueue.seqNum[index] = seqNum;
    memcpy((void*)&sendQueue.payalod[sendQueue.currentPushOffset], data, dataLen);
    sendQueue.currentPushOffset += dataLen;
    sendQueue.tail++;

    sendQueueCheckCallback(); /*check send queue periods*/

    return TRUE;
}

/** 
 * sendQueuePop
 * send queue pop
 * @param[out]  data  add to queue data buffer
 * @param[out]  dataLen  add to queue  length
 * @return     TRUE or FALSE
 */
bool_t sendQueuePop(uint8_t* data, uint8_t* dataLen)
{
    
    uint8_t currentIndex = 0;

    if(0 == sendQueue.tail)
    {
        return FALSE; /*queue table empty*/
    }
    if(sendQueue.head >= sendQueue.tail)
    {
        return FALSE; /*queue table no items*/
    }

    while(sendQueue.retransmitTime[sendQueue.head] <= 0)
    {
        if(sendQueue.head < sendQueue.tail)
        {
            sendQueue.currentPopOffset += sendQueue.index_len[sendQueue.head];
            sendQueue.head++;
        }
        else
        {
            sendQueueInit(); /*queue is empty*/
            return FALSE;
        }
    }
    currentIndex = sendQueue.head; 
    
    *dataLen = sendQueue.index_len[currentIndex];

    memcpy(data, (void*)&sendQueue.payalod[sendQueue.currentPopOffset], *dataLen);

    sendQueue.retransmitTime[currentIndex]--;
    if(sendQueue.retransmitTime[currentIndex] <= 0)
    {
        sendQueue.currentPopOffset += *dataLen;
        sendQueue.head++;
    }

    if(sendQueue.head >= sendQueue.tail)
    {
        sendQueueInit(); /*queue is empty*/
    }

    return TRUE;
}

/** 
 * sendQueueUpdate
 * send queue data update
 * @param[in]  seqNum  queuence number of current data
 * @return     NULL
 */
void sendQueueUpdate(uint16_t seqNum)
{
    uint8_t index = 0;

    for(index = sendQueue.head; index < sendQueue.tail; index++)
    {
        if(sendQueue.seqNum[index] == seqNum)
        {
            sendQueue.retransmitTime[index] = 0;
            if(index == sendQueue.head )
            {
                sendQueue.currentPopOffset += sendQueue.index_len[sendQueue.head];
                sendQueue.head++;
            }
            break;
        }
    }
}

/** 
 * sendQueueCheckCallback
 * send queue check callback
 * @param[in]  NULL
 * @return     NULL
 */
void sendQueueCheckCallback(void)
{
    if(0 == sendQueue.tail)
    {
        //emberSerialProtocolMcuSleepCallback();
        return ; /*queue table empty*/
    }
    
    if(sendQueue.head >= sendQueue.tail)
    {
        sendQueueInit();
        return ; /*queue table no items*/
    }

    else
    {
        if(!dev_timer_get_valid_flag(emSerialProtocolSendQueueTickEventControl))
        {
            emberSerialProtocolMcuWakeUpCallback();
            dev_timer_start_with_callback(emSerialProtocolSendQueueTickEventControl, 50, serial_protocol_event_Hanlder_callback);
        }
    }
}

/** 
 * emSerialProtocolSendQueueTickEventHandler
 * send queue tick event handler
 * @param[in]  NULL
 * @return     NULL
 */
void emSerialProtocolSendQueueTickEventHandler(void)
{
    uint8_t data[MAX_POP_DATA_SIZE] = {0};
    uint8_t dataLen = 0;

    dev_timer_stop(emSerialProtocolSendQueueTickEventControl);
    
    if(FALSE == sendQueuePop(data, &dataLen))
    {
        sendQueueInit();
        emberSerialProtocolMcuSleepCallback();
        return;
    }
    else
    {
        if( FALSE == mcu_wake_flag)
        {
            emberSerialProtocolMcuWakeUpCallback();
        }
        else
        {
            user_uart_send(SERIAL_PROTOCOL_UART_ID, data, dataLen);
        }
        dev_timer_start_with_callback(emSerialProtocolSendQueueTickEventControl, 100, serial_protocol_event_Hanlder_callback);
    }
}

