/**
 * @Author: Leon
 * @email: zhangpeng@tuya.com
 * @LastEditors: Leon
 * @file name: no_sleep_process.c
 * @Description: serial protocol no sleep device application file
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-03-12 17:29:33
 * @LastEditTime: 2019-03-25 10:31:25
 */

#include "serial_protocol_callbacks.h"
#include "config.h"
#include "oem_util.h"

data_confirm_t data_conf[DATA_CONF_MAX] = {0};
static volatile uint8_t data_items = 0;
static volatile uint8_t rspSequenceNember = 0; ///<current response sequence number
static volatile uint8_t defaultRspSuquenceNumber = 0; ///<default response sequence number
uint8_t total_send_failure = 0;
volatile bool_t mcu_wake_flag = FALSE;

extern volatile WORK_STATE_E deviceWorkState;
extern SERIAL_COMMAND_SEND_STATE_E sendState; ///<serial send state
extern volatile bool_t emberRestartFlag; ///<device restart flag
extern void serialNetworkStateReportCallback(uint8_t nwk_state);
extern serial_protocol_config_t protocol_config;
extern uint8_t mcu_version ;
extern oem_dev_t dev_cfg;
extern void serialNetworkStateReportCallback(uint8_t nwk_state);

void serialDataReceiveHandler(unsigned char* payload, unsigned short payload_len);
WORK_STATE_E deviceWorkStateGet(void);

/**
 * @description: serial protocol send data confirm queue init
 * @param {type} none
 * @return: none
 */
void dataConfQueueInit(void)
{
    memset(&data_conf, 0, sizeof(data_confirm_t)*DATA_CONF_MAX);
    rspSequenceNember = 0;
    defaultRspSuquenceNumber = 0;
}

/**
 * @description: serial protocol send data confirm queue add 
 * @param {confirm_seq_num} the confirm sequence number form gateway
 * @param {zcl_seq_num} the zcl sequence number of send data
 * @return: true or false
 */
bool_t dataConfQueueAdd(uint16_t confirm_seq_num, uint16_t zcl_seq_num)
{
    uint8_t i = 0;

    if(data_items >= DATA_CONF_MAX)
    {
        return FALSE;
    }

    for(i=0; i<DATA_CONF_MAX; i++)
    {
        if(data_conf[i].flag == FALSE)
        {
            data_conf[i].flag = TRUE;
            data_items++;
            data_conf[i].zcl_seq_num = zcl_seq_num;
            data_conf[i].confirm_seq_num = confirm_seq_num;
            data_conf[i].currentTime = dev_current_millisecond_ticks_get();

             return  TRUE;
        }
    }
    return FALSE;
}

/**
 * @description: serial protocol data confirm queue check by ticks 
 * @param {type} none
 * @return: the sequence number of timeout item in queue
 */
uint16_t dataConfQueueCheck(void)
{
    uint8_t i = 0;
    uint16_t confirm_seq_num = 0xFFFF;
    uint32_t  time = dev_current_millisecond_ticks_get();

    for(i=0; i<DATA_CONF_MAX; i++)
    {
        if(data_conf[i].flag == TRUE)
        {
            if(time - data_conf[i].currentTime >= 3000)
            {
                confirm_seq_num = data_conf[i].confirm_seq_num;
                data_conf[i].flag = FALSE;
                data_items--;
                break;
            }
        }
    }
    return confirm_seq_num;
}

/**
 * @description: serial protocol data queue confirm find function
 * @param {zcl_seq_num} sequence number of the item need to find
 * @return: the sequence number of find result item in queue
 */
uint16_t dataConfQueueFind( uint16_t zcl_seq_num )
{
    uint8_t i = 0, valid_item = 0;
    uint16_t confirm_seq_num = 0xFFFF;
    uint32_t  time = 0;

    /*delete the timeout items*/
    for(i=0; i<DATA_CONF_MAX; i++)
    {
        if(data_conf[i].flag == TRUE)
        {
            valid_item++;
            if(valid_item > data_items)
            {
                break;
            }

            time = dev_current_millisecond_ticks_get();
            if(time - data_conf[i].currentTime >= 6000)
            {
                data_conf[i].flag = FALSE;
                data_items--;
                continue;
            }
        }
    }

    valid_item = 0;
    for(i=0; i<DATA_CONF_MAX; i++)
    {
        if(data_conf[i].flag == TRUE)
        {
            valid_item++;
            if(valid_item > data_items)
            {
                break;
            }

            if(data_conf[i].zcl_seq_num == zcl_seq_num)
            {
                confirm_seq_num = data_conf[i].confirm_seq_num;
                data_conf[i].flag = FALSE;
                data_items--;
                break;
            }
        }
    }
    return confirm_seq_num;
}

/**
 * @description: network Rejoin state event handler
 * @param {type} none
 * @return: none
 */
void emberNetworkRejoinEventHandler(void)
{
    dev_timer_stop(emberNetworkRejoinEventControl);
    serialNetworkStateReportCallback(NWK_NOT_JOIN);
}

/**
 * @description: default response timeout event handler
 * @param {type} none
 * @return: none
 */
void rspConfirmTimeoutEventHandler(void)
{
    uint16_t confSeqNum = 0xFFFF;
    
    dev_timer_stop(rspConfirmTimeoutEventControl);
    confSeqNum = dataConfQueueCheck();

    if(0xFFFF != confSeqNum)
    {
        serialDataConfirmSend(FALSE, 0x00, confSeqNum);
        total_send_failure++ ;
        dev_timer_start_with_callback(rspConfirmTimeoutEventControl, 500, serial_protocol_event_Hanlder_callback);
    }

    if(total_send_failure > MAX_FAILURE_TIMES)
    {
        dev_sys_reset();
    }

    return;
}

/**
 * @description: serial protocol time synchronization event handler
 * @param {type} none
 * @return: none
 */
void serialProtocolTimerSyncEventHandler(void)
    {
        uint16_t seq_num = serialProtocolSeqNumGet();
        uint8_t data[2] = {0};
    
        data[0] = seq_num >> 8 & 0xff;
        data[1] = seq_num &0xff;
        serialProtocolTimeReqCallback( data, 2);
        dev_timer_start_with_callback(serialProtocolTimerSyncEventControl, TIME_SYNC_PERIOD, serial_protocol_event_Hanlder_callback);
    }

/**
 * @description: serial command delay send event handler
 * @param {type} none
 * @return: none
 */
void serialCommadDelaySendEventHandler(void)
{

    if(STATE_IDEL == sendState) /*send product info get command to serial*/
    {
        sendState = SCENE_NUM;
        dev_timer_start_with_callback(serialCommadDelaySendEventControl, SERIAL_CMD_DELAY_SEND_MS, serial_protocol_event_Hanlder_callback);
    }
    else if(SCENE_NUM == sendState) /*send scene numnber get command to serial*/
    {
        if(protocol_config.device_type == HA_SCENE_SELECTOR)
        {
            serialSceneSwitchNumGetSend();
        }
      
        sendState = STATE_IDEL;
        dev_timer_stop(serialCommadDelaySendEventControl);
    }
}

void malloc_test(void)
{
    void* addr = NULL;

    if((addr = malloc(1024 * 8)))
    {
        user_uart_send(SERIAL_PROTOCOL_UART_ID, "1111 \n\r", strlen("1111 \n\r"));
    }
    else
    {
        user_uart_send(SERIAL_PROTOCOL_UART_ID, "2222 \n\r", strlen("2222 \n\r"));
    }
    
}

/**
 * @description: ember product info device pid check event handler
 * @param {type} none
 * @return: none
 */
void emberProductIfoPidCheckEventHandler(void)
{
    if(NWK_STATE_OK == deviceWorkStateGet())
    {
        if(( NET_JOIN_OK == nwk_state_get()) || ( NET_POWER_ON_ONLINE == nwk_state_get()))
        {
            serialNetworkStateReportCallback(NWK_JOINED);
        }
        else
        {
            serialNetworkStateReportCallback(NWK_NOT_JOIN);
        }
        dev_timer_stop(emberProductIfoPidCheckEventControl);
    }
    else
    {
        productInfoReqCmdSend();
        dev_timer_start_with_callback(emberProductIfoPidCheckEventControl, 5000, serial_protocol_event_Hanlder_callback);
    }

    //malloc_test();
}

/**
 * @description: ember reset device to factury event handler
 * @param {type} none
 * @return: none
 */
/*void emberResetFacturyEventHandler(void)
{
    dev_timer_stop(emberResetFacturyEventControl);
    dev_sys_reset();
}*/

/**
 * @description: mcu version report event handler
 * @param {type} none
 * @return: none
 */
void emSerialMcuVerReportEventhandler(void)
{
    dev_timer_stop(emSerialMcuVerReportEventControl);

    /*report the MCU version*/
    if(mcu_version > 0)
    {
        uint8_t data[4] = {0};
        uint8_t data_len = 0;
        uint16_t seq_num = serialProtocolSeqNumGet();

        data[data_len++] = seq_num >> 8 & 0xff;
        data[data_len++] = seq_num &0xff;
        data[data_len++] = mcu_version;
        serialMcuOtaDataSendCallback(SERIAL_PROTOCOL_MCU_OTA_VER_RSP, data, data_len);
    }
}

/**
 * @description: serial protocol event handler 
 * @param {event_id} event need to processed
 * @return: none
 */
void serial_protocol_event_Hanlder_callback(uint8_t event_id)
{
    app_print("event id = %d", event_id);
    
    switch(event_id) 
    {
        case customNetworkStateIndEventControl:
            dev_zigbee_join_start(ZIGBEE_JOIN_MAX_TIMEOUT);
            break;

        case rspConfirmTimeoutEventControl:
            rspConfirmTimeoutEventHandler();
            break;

        case serialProtocolTimerSyncEventControl:
            serialProtocolTimerSyncEventHandler();
            break;

        case serialCommadDelaySendEventControl:
            serialCommadDelaySendEventHandler();
            break;

        case emberNetworkRejoinEventControl:
            emberNetworkRejoinEventHandler();
            break;

        case emberProductIfoPidCheckEventControl:
            emberProductIfoPidCheckEventHandler();
            break;

        /*case emberResetFacturyEventControl:
            emberResetFacturyEventHandler();
            break;*/

        case emSerialMcuVerReportEventControl:
            emSerialMcuVerReportEventhandler();
            break;

        case emSerialProtocolSendQueueTickEventControl:
            emSerialProtocolSendQueueTickEventHandler();
            break;
        
        default:
            break;
    }
}

/**
 * @description: serial protocol wakeup by mcu callback
 * @param {type} none
 * @return: none
 */
void emberSerialProtocolMcuWakeUpCallback(void)
{
    return;
}

/**
 * @description: serial protocol sleeped by mcu callback
 * @param {type} none
 * @return: none
 */
void emberSerialProtocolMcuSleepCallback(void)
{
  return;
}

/**
 * @description: This callback is used to init the user gpio after mfg test complete in sametimes
 * the UART gpio and the user GPIO need to be reused.
 * @param {type} none
 * @return: none
 */
void mfgTestCompleteWithGpioInitCallback(void)
{
    
}

void serial_mcu_ota_pollrate_timeout_handler(void)
{
    dev_timer_stop(serialMcuOtaPollrateTimeoutEventControl);
    zg_ota_evt_callback(ZG_OTA_EVT_DOWNLOAD_TIME_OUT);
}