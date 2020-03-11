/****************************************Copyright (c)*************************
**                               版权所有 (C), 2015-2020, 涂鸦科技
**
**                                 http://www.tuya.com
**
**--------------文件信息-------------------------------------------------------*/
#define SYSTEM_GLOBAL

#include "system.h"
#include "mcu_api.h"

/*****************************************************************************
函数名称 : get_queue_total_data
功能描述 : 读取队列内数据
输入参数 : 无
返回参数 : 无
*****************************************************************************/
unsigned short get_queue_total_data(void)
{
    if(queue_total_data == 0)
    {
        zigbee_protocol_init();
    }
    return (queue_total_data);
}

/*****************************************************************************
函数名称 : Queue_Read_Byte
功能描述 : 读取队列1字节数据
输入参数 : 无
返回参数 : 无
*****************************************************************************/
unsigned char Queue_Read_Byte(void)
{
    unsigned char value = 0;

    //ATOMIC(
        if(queue_total_data > 0)
        {
            //  有数据
            if(queue_out >= (unsigned char *)(zigbee_queue_buf + sizeof(zigbee_queue_buf)))
            {
                //  数据已经到末尾
                queue_out = (unsigned char *)(zigbee_queue_buf);
            }
            value = *queue_out++;
            queue_total_data--;
        }
       //)

    return value;
}

#if 0
/*****************************************************************************
函数名称 : data_handle
功能描述 : 数据帧处理
输入参数 : offset:数据起始位
返回参数 : 无
*****************************************************************************/
void data_handle(unsigned short offset)
{
    unsigned char cmd_type = 0;
    unsigned short total_len = 0;

    cmd_type   = zigbee_uart_rx_buf[offset + FRAME_TYPE];
    total_len  = zigbee_uart_rx_buf[offset + LENGTH_HIGH] * 0x100;
    total_len += zigbee_uart_rx_buf[offset + LENGTH_LOW];
    
    switch(cmd_type)
    {
        case PRODUCT_INFO_CMD:
            break;

        case ZIGBEE_STATE_CMD:
            break;

        case PRODUCT_INFO_CMD:
            break;

        case ZIGBEE_CFG_CMD:
            break;

        case ZIGBEE_DATA_REQ_CMD:
            break;

        case DATA_RSP_SYNC_CMD:
            break;

        case DATA_RSP_ASYNC_CMD:
            break;

        case TIME_GET_CMD:
            break;

        case FUNC_TEST_CMD:
            break;

        default:
            break;
    }
}
#endif
