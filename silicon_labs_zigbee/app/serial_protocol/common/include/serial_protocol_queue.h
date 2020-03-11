/**
 * @Author: Leon
 * @email: zhangpeng@tuya.com
 * @LastEditors: Leon
 * @file name: serial_protocol_queue.h 
 * @Description: serial protocol used by TuYa ZigBee modules and MCU communicated by UART
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-03-25 21:41:05
 * @LastEditTime: 2019-03-25 23:03:25
 */

#ifndef __SERIAL_PROTOCOL_QUEUE_H
#define __SERIAL_PROTOCOL_QUEUE_H
#include "stdint.h"
#include "zigbee_sdk.h"

#ifdef __cplusplus
	extern "C" {
#endif

#define MAX_QUEUE_PAYLOAD_SIZE   1000
#define MAX_POP_DATA_SIZE  128
#define MAX_QUEUE_ITEM     50
#define MAX_RETRY_TIME     1


typedef struct
{
    uint8_t tail; ///< tail of the send queue
    uint8_t head; ///< head of the send queue
    uint8_t index; ///< total num of index in the queue
    uint8_t currentPopOffset;///< current pop payload offset position
    uint8_t currentPushOffset;///< current push payload offset position
    uint8_t retransmitTime[MAX_QUEUE_ITEM]; ///< retransmit times of every index
    uint8_t index_len[MAX_QUEUE_ITEM]; ///< index len of every index
    uint16_t seqNum[MAX_QUEUE_ITEM]; ///< sequence number of every index
    uint8_t payalod[MAX_QUEUE_PAYLOAD_SIZE]; ///< total payload
}send_queue_t;

// extern EmberEventControl emSerialProtocolSendQueueTickEventControl;

#ifdef __cplusplus
}
#endif

#endif /*__SERIAL_PROTOCOL_QUEUE_H*/
