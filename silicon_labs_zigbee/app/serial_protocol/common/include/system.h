/****************************************Copyright (c)*************************
**                               ��Ȩ���� (C), 2018-0704, Ϳѻ�Ƽ�
**
**                                 http://www.tuya.com
**
**--------------�ļ���Ϣ-------------------------------------------------------
**��   ��   ��: system.c
**��        ��: zigbee���ݴ�������
**ʹ �� ˵ �� : �û�������ĸ��ļ�ʵ������
**
**
**--------------��ǰ�汾�޶�---------------------------------------------------
** ��  ��: v1.0.0
** �ա���: 2018��7��4��
** �衡��: 1:Э�����
**
**-----------------------------------------------------------------------------
******************************************************************************/
#ifndef __SYSTEM_H_
#define __SYSTEM_H_

#ifdef SYSTEM_GLOBAL
  #define SYSTEM_EXTERN
#else
  #define SYSTEM_EXTERN   extern
#endif

//=============================================================================
//֡���ֽ�˳��
//=============================================================================
#define         HEAD_FIRST                      0
#define         HEAD_SECOND                     1
#define         PROTOCOL_VERSION                2
#define         FRAME_TYPE                      3
#define         LENGTH_HIGH                     4
#define         LENGTH_LOW                      5
#define         DATA_START                      6

//=============================================================================
//����֡����
//=============================================================================
#define         PRODUCT_INFO_CMD                0x01                            //��Ʒ��Ϣ
#define         ZIGBEE_STATE_CMD                0x02                            //zigbee����״̬	
#define         ZIGBEE_CFG_CMD                  0x03                            //����zigbeeģ��
#define         ZIGBEE_DATA_REQ_CMD             0x04                            //zigbee�����·�����
#define         DATA_RSP_SYNC_CMD               0x05                            //MCU״̬�����ϱ�
#define         DATA_RSP_ASYNC_CMD              0x06                            //MCU״̬�����ϱ�
#define         FUNC_TEST_CMD                   0x08                            //zigbeeģ�鹦�ܲ���
#define         SCENE_SWITCH_NUM_GET_CMD        0x09                            //��ȡ�������ظ����������ڳ������أ�
#define         SCENE_SWITCH_ID_REPORT_CMD      0x0A                            //��ȡ�����ϱ�ID�������ڳ������أ�
#define         MCU_OTA_VER_REQ_CMD             0x0B                            //MCU OTA版本号请求
#define         MCU_OTA_NOTIFY_CMD              0x0C                            //MCU OTA固件升级通知
#define         MCU_OTA_DATA_REQ_CMD            0x0D                            //MCU OTA数据请求
#define         MCU_OTA_END_CMD                 0x0E                            //MCU OTA升级结束
#define         TIME_GET_CMD                    0x24                            //zigbeeģ��ʱ��ͬ��

//=============================================================================
#define SERIAL_PROTOCOL_VER          0x02                                            //Э��汾��
#define PROTOCOL_HEAD                0x07                                            //�̶�Э��ͷ����
#define FIRST_FRAME_HEAD             0x55
#define SECOND_FRAME_HEAD            0xaa
#define SERIAL_PROTOCOL_HDR          (FIRST_FRAME_HEAD<<8 | SECOND_FRAME_HEAD)   ///<Э����ͷ����

/******************************************************************************
                         �����շ�����:
                    �統ǰʹ��MCU��RAM����,���޸�Ϊ24
******************************************************************************/
#define ZIGBEE_UART_QUEUE_LMT             512//16         //数据接收队列大小,如MCU的RAM不够,可缩小
#define ZIGBEE_UART_RECV_BUF_LMT          128              //根据用户DP数据大小量定,必须大于24
#define ZIGBEE_UART_SEND_BUF_LMT          128              //根据用户DP数据大小量定,必须大于24

//============================================================================= 
SYSTEM_EXTERN volatile unsigned char zigbee_queue_buf[PROTOCOL_HEAD + ZIGBEE_UART_QUEUE_LMT];        //���ڶ��л���
SYSTEM_EXTERN volatile unsigned char zigbee_uart_rx_buf[PROTOCOL_HEAD + ZIGBEE_UART_RECV_BUF_LMT];   //���ڽ��ջ���
SYSTEM_EXTERN volatile unsigned char zigbee_uart_tx_buf[PROTOCOL_HEAD + ZIGBEE_UART_SEND_BUF_LMT];   //串口发送缓存
//
SYSTEM_EXTERN volatile unsigned char *queue_in;
SYSTEM_EXTERN volatile unsigned char *queue_out;
SYSTEM_EXTERN volatile unsigned short queue_total_data;


/*****************************************************************************
�������� : get_queue_total_data
�������� : ��ȡ�������ַ�
������� : NULL
���ز��� : char
*****************************************************************************/
unsigned short get_queue_total_data(void);
unsigned char Queue_Read_Byte(void);

#endif
