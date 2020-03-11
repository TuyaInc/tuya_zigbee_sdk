/**
 * @Author: Leon
 * @email: zhangpeng@tuya.com
 * @LastEditors: Leon
 * @file name: callbacks.c
 * @Description: this file is a template of the sdk callbacks function, 
 * user need to achieve the function yourself
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-03-12 17:29:33
 * @LastEditTime: 2019-03-25 10:31:25
 */

#include "serial_protocol_callbacks.h"
#include "config.h"
#include "zigbee_sdk.h"
#include "serial_protocol.h"
#include "serial_protocol_callbacks.h"
#include "oem_util.h"

const attr_t g_group_attr_list[] = {
    GROUP_ATTR_LIST
};

const attr_t g_scene_attr_list[] = {
    SCENE_ATTR_LIST
};

const attr_t g_scene_client_attr_list[] = {
    SCENE_CLINET_ATTR_LIST
};

const attr_t g_private_attr_list[] = {
    PRIVATE_ATTR_LIST
};

const gpio_config_t gpio_input_config[] = {
    {KEY0_PORT, KEY0_PIN, KEY0_MODE, KEY0_OUT, KEY0_DRIVER}
};

const gpio_config_t gpio_ouput_config[] = {
    {LED0_PORT, LED0_PIN, LED0_MODE, LED0_OUT, LED0_DRIVER}
};


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


const cluster_t g_server_cluster_id[] = {
    DEF_CLUSTER_GROUPS_CLUSTER_ID(g_group_attr_list)
    DEF_CLUSTER_SCENES_CLUSTER_ID(g_scene_attr_list)
    DEF_CLUSTER_PRIVATE_CLUSTER_ID(g_private_attr_list)
};

const cluster_t g_client_cluster_id[] = {
    DEF_CLUSTER_SCENES_CLUSTER_ID(g_scene_client_attr_list)
};

#define SERVER_CLUSTER_LEN  get_array_len(g_server_cluster_id)
#define CLIENT_CLUSTER_LEN  get_array_len(g_client_cluster_id)

const dev_description_t g_dev_des[] = {
    { 1, ZHA_PROFILE_ID, ZG_DEVICE_ID_SMART_PLUG, SERVER_CLUSTER_LEN, (cluster_t *)&g_server_cluster_id[0], 0, NULL },
};

const dev_description_t g_dev_des_scene[] = {
    { 1, ZHA_PROFILE_ID, ZG_DEVICE_ID_SCENE_SELECTOR, SERVER_CLUSTER_LEN, (cluster_t *)&g_server_cluster_id[0], CLIENT_CLUSTER_LEN, (cluster_t *)&g_client_cluster_id[0] },
    { 2, ZHA_PROFILE_ID, ZG_DEVICE_ID_SCENE_SELECTOR, SERVER_CLUSTER_LEN, (cluster_t *)&g_server_cluster_id[0], CLIENT_CLUSTER_LEN, (cluster_t *)&g_client_cluster_id[0] },
    { 3, ZHA_PROFILE_ID, ZG_DEVICE_ID_SCENE_SELECTOR, SERVER_CLUSTER_LEN, (cluster_t *)&g_server_cluster_id[0], CLIENT_CLUSTER_LEN, (cluster_t *)&g_client_cluster_id[0] },
    { 4, ZHA_PROFILE_ID, ZG_DEVICE_ID_SCENE_SELECTOR, SERVER_CLUSTER_LEN, (cluster_t *)&g_server_cluster_id[0], CLIENT_CLUSTER_LEN, (cluster_t *)&g_client_cluster_id[0] },
    { 5, ZHA_PROFILE_ID, ZG_DEVICE_ID_SCENE_SELECTOR, SERVER_CLUSTER_LEN, (cluster_t *)&g_server_cluster_id[0], CLIENT_CLUSTER_LEN, (cluster_t *)&g_client_cluster_id[0] },
    { 6, ZHA_PROFILE_ID, ZG_DEVICE_ID_SCENE_SELECTOR, SERVER_CLUSTER_LEN, (cluster_t *)&g_server_cluster_id[0], CLIENT_CLUSTER_LEN, (cluster_t *)&g_client_cluster_id[0] },
    { 7, ZHA_PROFILE_ID, ZG_DEVICE_ID_SCENE_SELECTOR, SERVER_CLUSTER_LEN, (cluster_t *)&g_server_cluster_id[0], CLIENT_CLUSTER_LEN, (cluster_t *)&g_client_cluster_id[0] },
    { 8, ZHA_PROFILE_ID, ZG_DEVICE_ID_SCENE_SELECTOR, SERVER_CLUSTER_LEN, (cluster_t *)&g_server_cluster_id[0], CLIENT_CLUSTER_LEN, (cluster_t *)&g_client_cluster_id[0] },
};

#define EP_SUMS sizeof(g_dev_des)/sizeof(g_dev_des[0])
#define EP_SUMS_SCENE  sizeof(g_dev_des_scene)/sizeof(g_dev_des_scene[0])

static volatile uint8_t rspSequenceNember = 0; ///<current response sequence number
static volatile uint8_t defaultRspSuquenceNumber = 0; ///<default response sequence number
SERIAL_COMMAND_SEND_STATE_E sendState = STATE_IDEL; ///<serial send state
static volatile uint8_t sceneSwitchNum = 0; ///<scene switch numbers
volatile WORK_STATE_E deviceWorkState;
volatile bool_t emberRestartFlag = FALSE; ///<device restart flag
serial_protocol_config_t protocol_config;
static uint16_t app_seq_num = 0;
local_time_t local_time;
oem_dev_t dev_cfg;

void deviceWorkStateSet(WORK_STATE_E state);
void serialNetworkStateReportCallback(uint8_t nwk_state);
static void serialWaitConfirmCallback(uint8_t sequenceNumber);
static void serial_rx_callback(uint8_t* data, uint16_t data_len );

extern uint8_t mcu_version;
extern uint8_t total_send_failure;
extern data_confirm_t data_conf[DATA_CONF_MAX];

extern void dataConfQueueInit(void);
extern bool_t dataConfQueueAdd(uint16_t confirm_seq_num, uint16_t zcl_seq_num);
extern uint16_t dataConfQueueFind( uint16_t zcl_seq_num );
extern uint16_t dataConfQueueCheck(void);

void serial_protocol_load_config(serial_protocol_config_t* config)
{
    memset(&protocol_config, 0, sizeof(serial_protocol_config_t));

    protocol_config.serial_bandrate = config->serial_bandrate;
    protocol_config.join_timeout = config->join_timeout;
    protocol_config.device_type = config->device_type;
}

/**
 * @description: get user application data send sequence numbers 
 * @param {type} none
 * @return: sequence number
 */
uint16_t app_seq_num_get(void)
{
    if(app_seq_num >= 0xFFFE) //0xFFFF is invaild
    {
        app_seq_num = 1;
    }
    else
    {
        app_seq_num++;
    }
    
    return app_seq_num;
}

/**
 * @description: application data send ack receive callback 
 * @param {st} send state
 * @param {data} send data
 * @return: none
 */
void app_ack_receive_callback(SEND_ST_T st, dev_send_data_t *data)
{
    uint16_t confSeqNum = 0xFFFF;

    app_print("ack received, seq = 0x%x \n\r;", data->zcl_id);
    
    if(st == SEND_ST_OK)
    {
        confSeqNum = dataConfQueueFind(data->zcl_id);
        
        if( confSeqNum != 0xFFFF )
        {
            serialDataConfirmSend(FALSE, TRUE, confSeqNum);
            total_send_failure = 0;
        }
    }
}

static void dev_key_handle(uint32_t key_id, key_st_t key_st, uint32_t push_time)
{
    // user_uart_send(SERIAL_PROTOCOL_UART_ID, "1111", strlen("1111"));
    if(key_st == KEY_ST_PUSH) 
    {
        gpio_raw_output_write_status(LED0_PORT, LED0_PIN, 0); //clean the wakeup pin
        // dev_io_op(0, DEV_IO_OFF);
        if(push_time >= 10*1000) 
        {
            //长按了10s,开始闪烁提醒用户即将进入配网
            //user_uart_send(SERIAL_PROTOCOL_UART_ID, "2222", strlen("2222"));
            // dev_led_start_blink(0, 125, 125, DEV_LED_BLINK_FOREVER, DEV_IO_OFF);
        }
        else
        {
            // dev_led_base_op(0, DEV_IO_ON);
        }
    } 
    else
    {
        // dev_io_op(0, DEV_IO_ON);
        gpio_raw_output_write_status(LED0_PORT, LED0_PIN, 1); //clean the wakeup pin

        if(push_time >= 10*1000)
        {
            // dev_led_start_blink(0, 125, 125, DEV_LED_BLINK_FOREVER, DEV_IO_OFF);
            //user_uart_send(SERIAL_PROTOCOL_UART_ID, "3333", strlen("3333"));
            dev_zigbee_join_start(1000*60);
        }
    }
}

/**
 * @description: device first power on, user can register device and init the gpio
 * @param {type} none
 * @return: none
 */
void dev_power_on_init(void)
{
    zg_dev_config_t g_zg_dev_config;
    join_config_t cfg;

    serial_protocol_config_t config;

    config.serial_bandrate = UART0_BANDRATE;
    config.join_timeout = ZIGBEE_JOIN_MAX_TIMEOUT;
    config.device_type = HA_COMMON_DEV_DC;
    //config.device_type = HA_SCENE_SELECTOR;
    serial_protocol_load_config(&config);//just for current test

    if(protocol_config.device_type == HA_COMMON_DEV_DC)
    {
        dev_register_zg_ep_infor((dev_description_t *)g_dev_des, EP_SUMS);
    }
    else
    {
        dev_register_base_info(MODEL_ID_SCENE, PRODUCTOR_ID_PFEFIX_SCENE, PRODUCTOR_ID);
        dev_register_zg_ep_infor((dev_description_t *)g_dev_des_scene, EP_SUMS_SCENE);
    }

    memset(&g_zg_dev_config, 0, sizeof(zg_dev_config_t));
    memset(&cfg, 0, sizeof(join_config_t));

    g_zg_dev_config.dev_type = ZG_ROUTER;
    g_zg_dev_config.config.router_cfg.reserved = 0;
    dev_register_zg_dev_config(&g_zg_dev_config);

    cfg.auto_join_power_on_flag = 0;
    cfg.auto_join_remote_leave_flag = 0;
    cfg.join_timeout = protocol_config.join_timeout;

    dev_zg_join_config(&cfg);
    dev_heartbeat_set(APP_VERSION, 1000*60*2); //2min

    gpio_button_init((gpio_config_t *)gpio_input_config, get_array_len(gpio_input_config), 50, dev_key_handle);
    gpio_output_init((gpio_config_t *)gpio_ouput_config, get_array_len(gpio_ouput_config));

    return;
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

    user_uart_config_t uart_config = APP_UART_CONFIG_DEFAULT;

    uart_config.func = serial_rx_callback;
    uart_config.baud_rate = protocol_config.serial_bandrate;
    user_uart_init(&uart_config);
    
    sendState = STATE_IDEL;
    deviceWorkStateSet(NWK_STATE_ERROR);
    emberRestartFlag = TRUE;

    emberSerialProtocolInitCallback();
    deviceWorkStateSet(NWK_STATE_ERROR);
    dev_timer_start_with_callback(serialCommadDelaySendEventControl, 1000*5, serial_protocol_event_Hanlder_callback);
    dev_timer_start_with_callback(emberProductIfoPidCheckEventControl, 1000*10, serial_protocol_event_Hanlder_callback);
    zigbee_protocol_init();
    app_print("%s \n\r", __FUNCTION__);
    user_uart_send(SERIAL_PROTOCOL_UART_ID, (uint8_t*)FIRMWARE_INFO, sizeof(FIRMWARE_INFO));
    memset(&local_time, 0, sizeof(local_time_t));
    
    gpio_button_init((gpio_config_t *)gpio_input_config, get_array_len(gpio_input_config), 50, dev_key_handle);
    gpio_output_init((gpio_config_t *)gpio_ouput_config, get_array_len(gpio_ouput_config));
    dev_io_op(0, DEV_IO_OFF);

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
        case NET_POWER_ON_LEAVE: {
            app_print("nwk state is NET_IDLE");
            serialNetworkStateReportCallback(NWK_NOT_JOIN);
            break;
        }

        case NET_JOIN_START: {
                dev_led_start_blink(0, 125, 125, DEV_LED_BLINK_FOREVER, DEV_IO_OFF);
            break;
        }
        case NET_JOIN_TIMEOUT: {
            serialNetworkStateReportCallback(NWK_NOT_JOIN);
            break;
        }
        case NET_POWER_ON_ONLINE:
        case NET_JOIN_OK: {
            dataConfQueueInit();
            serialNetworkStateReportCallback(NWK_JOINED);
            memset(&data_conf, 0, sizeof(data_confirm_t)*DATA_CONF_MAX);
            dev_timer_start_with_callback(emSerialMcuVerReportEventControl, MCU_VERSION_REPORT_DELAY_MS, serial_protocol_event_Hanlder_callback);
            dev_timer_start_with_callback(serialProtocolTimerSyncEventControl, 1000*10, serial_protocol_event_Hanlder_callback);
            dev_led_start_blink(0, 500, 500, DEV_LED_BLINK_FOREVER, DEV_IO_OFF);

            break;
        }
        case NET_LOST: {
            break;
        }
        case NET_REMOTE_LEAVE: {
            serialNetworkStateReportCallback(NWK_NOT_JOIN);
            dev_zigbee_join_start(1000*60);
            break;
        }
        case NET_LOCAL_LEAVE: {
            // serialNetworkStateReportCallback(NWK_NOT_JOIN);
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
 * @description: device report inforamtion when first power on(if need),;
 * @param {type} none
 * @return: none
 */
void dev_power_on_report_info(void)
{
    //TODO

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
    app_print("data receive, cluster = %d \n\r", dev_msg->cluster);

    if(protocol_config.device_type != HA_COMMON_DEV_DC)
    {
        return ZCL_CMD_RET_FAILED; //scene selector device need do nothing with this command
    }

    if(dev_msg->cluster == CLUSTER_PRIVATE_TUYA_CLUSTER_ID)
    {
        emberAfSerialProtocolCommandParseCallback(dev_msg);
    }
    else
    {
        //other cluster
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
    app_print("recall scene, data: %s, data_len: %d \n\r", in_data->data, in_data->data_len);

    uint8_t payload[64] = {0};
    uint8_t payloadLen = 0;
    uint16_t seqNum = serialProtocolSeqNumGet();
    
    if(protocol_config.device_type == HA_SCENE_SELECTOR)
    {
        return; //scene selector do nothing with recall scene command
    }

    payloadLen = in_data->data_len; /*insert two bytes sequence num*/
    payload[0] = seqNum >> 8;
    payloadLen++;
    payload[1] = seqNum & 0x00FF;
    payloadLen++;
    
    memcpy(&payload[2], in_data->data, in_data->data_len);
    serialDataReqCmdSend( payload, payloadLen);

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
    //TODO
    app_print("add scene, data: %s \n\r", out_data);

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
    //TODO

    return;
}

/**
 * @description: get current device work state
 * @param {type} none
 * @return: WORK_STATE_E current work state
 */
WORK_STATE_E deviceWorkStateGet(void)
{
    return deviceWorkState;
}

 /**
  * @description: set device work state
  * @param {state} WORK_STATE_E state
  * @return: none
  */
 void deviceWorkStateSet(WORK_STATE_E state)
 {
    deviceWorkState = state;
 }

/**
 * @description: random a millisecond time, the total time is from base to base+randOffSet
 * @param {base} time base
 * @param {randOffSet} random time offset
 * @return: random time
 */
uint16_t randomBackOffMillisecond(uint16_t base, uint16_t randOffSet)
{
    uint8_t randTime = 0;
    srand( (unsigned)time( NULL ) );   /* random seek*/  
    randTime = rand()%randOffSet + base; /*a rand value from 500 - 1000*/

    return randTime;
}

/**
 * @description: random a second time, the total time is from base to base+randOffSet
 * @param {base} time base
 * @param {randOffSet} random time offset
 * @return: random time
 */
uint16_t randomBackOffSecond(uint16_t base, uint16_t randOffSet)
{
    uint8_t randTime = 0;
    srand( (unsigned)time( NULL ) );   /* random seek*/  
    randTime = rand()%randOffSet + base; /*a rand value from randOffSet to base+randOffSet*/

    return randTime;
}

/**
 * @description: send network state to the MCU when the zigbee state is changed;
 * @param {nwk_status} network status enum type, 0x00: not join, 0x01: joined;
 * @return: none
 */
void serialNetworkStateReportCallback(uint8_t nwk_state)
{
    nwkInfoReqCmdSend( nwk_state);
}

/**
 * @description: send network state to the MCU when the zigbee state is changed;
 * @param {config} network config flag, 0x00: reset the zigbee device, 0x01: start join;
 * @return: none
 */
void  serialDeviceConfigCallback(uint8_t config)
{
    if( 0x00 == config)
    {
        dev_sys_reset(); //software reset
    }
    else if( 0x01 == config )
    {
        /*network steering*/
        app_print("network steering start");

        dev_zigbee_leave_for_user();
        dev_timer_start_with_callback(customNetworkStateIndEventControl, 1000, serial_protocol_event_Hanlder_callback);
    }
}

/**
 * @description: serial receive data handle function 
 * @param {*data} received data
 * @param {data_len} received data length
 * @return: none
 */
extern void zigbee_uart_service();
static void serial_rx_callback(uint8_t* data, uint16_t data_len )
{
    uint16_t  index = 0;
    uint8_t* ptr = NULL;
    
    app_print("data receive \n\r");
    
    ptr = data;
    
    do
    {
        uart_receive_input(*ptr++);
        index++;
    }while(index < data_len);

    while(get_queue_total_data() > 0)
    {
        zigbee_uart_service();
    }
}

/**
 * @description: serial data response send callback, receive data from mcu and send to gateway 
 * @param {sync} true is synchronized, false is  asynchronized
 * @param {* data} receive data ptr
 * @param {data_len} receive data length
 * @return: none
 */
void serialDataRspSendCallback(bool_t sync, uint8_t* data, uint8_t data_len)
{
    dev_send_data_t send_data;
    static uint8_t sourceEndpoint = 1;
    uint8_t zclSequenceNumber = 0;
    uint16_t confirmSeq = 0;

    if(NULL == data || data_len < 2)
    {
        return ;
    }

    if(data_len > MAX_ZIGBEE_PAYLOAD_SIZE)
    {
        return;
    }

    confirmSeq = (data[0] << 8) | data[1];

    if(protocol_config.device_type != HA_COMMON_DEV_DC)
    {
        serialDataConfirmSend(sync, 0x00, confirmSeq);
        return; //scene selector device need do nothing with this command
    }

    if((NET_JOIN_OK != nwk_state_get()) && (NET_POWER_ON_ONLINE != nwk_state_get()))
    {
        serialDataConfirmSend(sync, 0x00, confirmSeq);
        total_send_failure++;

        return;
    }

    memset(&send_data, 0, sizeof(dev_send_data_t));

    send_data.zcl_id = app_seq_num_get();
    send_data.qos = QOS_1;
    send_data.addr.mode = SEND_MODE_GW;
    send_data.addr.type.gw.cluster_id = CLUSTER_PRIVATE_TUYA_CLUSTER_ID;
    send_data.addr.type.gw.src_ep = sourceEndpoint;
    send_data.delay_time = 0;
    send_data.random_time = 0;
    send_data.commmand_type = ZCL_COMMAND_CLUSTER_SPEC_CMD;
    send_data.direction = ZCL_DATA_DIRECTION_SERVER_TO_CLIENT;
    
    if(TRUE == sync)
    {
        send_data.command_id = SERIAL_PROTOCOL_DATA_REPORT ;
    }
    else
    {
        send_data.command_id = SERIAL_PROTOCOL_DATA_RSP ;
    }

    if(data_len < 64)
    {
        memcpy(send_data.data.private.data, data, data_len);
        send_data.data.private.len = data_len;
    }

    if(sync == TRUE)
    {
        serialDataConfirmSend(sync, 0x01, confirmSeq);
    }
    else
    {
        serialWaitConfirmCallback(zclSequenceNumber);
        dataConfQueueAdd(confirmSeq, send_data.zcl_id);
    }
    
    app_print("data send, len: %d \n\r", send_data.data.private.len);
    dev_zigbee_send_data(&send_data, app_ack_receive_callback, 1000);
}

/**
 * @description: serial data response send callback, receive data from mcu and send to gateway 
 * @param {cmd_id} send data command id
 * @param {* data} receive data ptr
 * @param {data_len} receive data length
 * @return: none
 */
void serialMcuOtaDataSendCallback(uint8_t cmd_id, uint8_t* data, uint8_t data_len)
{
    dev_send_data_t send_data;
    static uint8_t sourceEndpoint = 1;
    
    if(NULL == data)
    {
        return; //scene selector device need do nothing with this command
    }

    memset(&send_data, 0, sizeof(dev_send_data_t));

    send_data.zcl_id = app_seq_num_get();
    send_data.qos = QOS_1;
    send_data.addr.mode = SEND_MODE_GW;
    send_data.addr.type.gw.cluster_id = CLUSTER_PRIVATE_TUYA_CLUSTER_ID;
    send_data.addr.type.gw.src_ep = sourceEndpoint;
    send_data.delay_time = 0;
    send_data.random_time = 0;
    send_data.commmand_type = ZCL_COMMAND_CLUSTER_SPEC_CMD;
    send_data.direction = ZCL_DATA_DIRECTION_SERVER_TO_CLIENT;
    
    send_data.command_id = cmd_id ;
   
    if(data_len <= 64)
    {
        memcpy(send_data.data.private.data, data, data_len);
        send_data.data.private.len = data_len;
    }
    
    app_print("data send, len: %d \n\r", send_data.data.private.len);
    dev_zigbee_send_data(&send_data, NULL, 1000);
}

/**
 * @description: asynchronization data response callback
 * @param {sequenceNumber} asyc data response sequence nember
 * @return: none
 */
static void serialWaitConfirmCallback(uint8_t sequenceNumber)
{
    rspSequenceNember = sequenceNumber;
    dev_timer_start_with_callback( rspConfirmTimeoutEventControl,RSP_CONFIRM_TIMEOUT, serial_protocol_event_Hanlder_callback);
}

/**
 * @description: get last sequence number of default response callback
 * @param {resp_seq_num} asyc data response sequence nember
 * @return: none
 */
void customeGetLastRespondSeqNumberCallback(uint8_t resp_seq_num)
{
    defaultRspSuquenceNumber = resp_seq_num;
    
    if(rspSequenceNember == resp_seq_num)
    {
        uint16_t confSeqNum = 0xFFFF;

        dev_timer_stop(rspConfirmTimeoutEventControl);
        confSeqNum = dataConfQueueFind(resp_seq_num);
        
        if(confSeqNum != 0xFFFF)
        {
            serialDataConfirmSend(FALSE, 0x01, confSeqNum);
            total_send_failure = 0;
        }
    }   
}

/**
 * @description: serial scene response callback form muc to zigbee for recall scene
 * @param {data} data from MCU to ZigBee module
 * @param {data_len} data length
 * @return: none
 */
void  serialSceneRspSendCallback(uint8_t* data, uint8_t data_len)
{
    dev_send_data_t send_data;
    static uint8_t sourceEndpoint = 1;
    
    memset(&send_data, 0, sizeof(dev_send_data_t));

    send_data.zcl_id = 0;
    send_data.qos = QOS_0;
    send_data.addr.mode = SEND_MODE_GW;
    send_data.addr.type.gw.cluster_id = CLUSTER_PRIVATE_TUYA_CLUSTER_ID;
    send_data.addr.type.gw.src_ep = sourceEndpoint;
    send_data.delay_time = 0;
    send_data.random_time = 0;
    send_data.commmand_type = ZCL_COMMAND_CLUSTER_SPEC_CMD;
    send_data.direction = ZCL_DATA_DIRECTION_SERVER_TO_CLIENT;
    
    send_data.command_id = SERIAL_PROTOCOL_DATA_RSP ;

    if(data_len < 64)
    {
        memcpy(send_data.data.private.data, data, data_len);
        send_data.data.private.len = data_len;
    }
    
    dev_zigbee_send_data(&send_data, app_ack_receive_callback, 1000);
}

/**
 * @description: serial protocol time request callback
 * @param {*data} data request command payload
 * @param {data_len} data request command payload len
 * @return: none
 */
void serialProtocolTimeReqCallback(uint8_t* data, uint8_t data_len)
{
    dev_send_data_t send_data;
    static uint8_t sourceEndpoint = 1;
 
    memset(&send_data, 0, sizeof(dev_send_data_t));
    
    send_data.zcl_id = app_seq_num_get();
    send_data.qos = QOS_0;
    send_data.addr.mode = SEND_MODE_GW;
    send_data.addr.type.gw.cluster_id = CLUSTER_PRIVATE_TUYA_CLUSTER_ID;
    send_data.addr.type.gw.src_ep = sourceEndpoint;
    send_data.delay_time = 0;
    send_data.random_time = 0;
    send_data.commmand_type = ZCL_COMMAND_CLUSTER_SPEC_CMD;
    send_data.direction = ZCL_DATA_DIRECTION_SERVER_TO_CLIENT;
    
    send_data.command_id = SERIAL_PROTOCOL_TIME_REQ ;

    if(data_len < 64)
    {
        memcpy(send_data.data.private.data, data, data_len);
        send_data.data.private.len = data_len;
    }
    
    dev_zigbee_send_data(&send_data, app_ack_receive_callback, 1000);
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
    else if (MODULE_NAME == TYZS3)
    {
        user_uart_config_t default_config = TYZS3_USART_CONFIG_DEFAULT;
        memcpy(&config, &default_config, sizeof(user_uart_config_t));
    }
    else if(MODULE_NAME == TYZS5)
    {
        user_uart_config_t default_config = TYZS5_USART_CONFIG_DEFAULT;
        memcpy(&config, &default_config, sizeof(user_uart_config_t));
    }
    else
    {
        return NULL;
    }

    return &config;
}

/**
 * @description: get total scene switch number callback
 * @param {switchNum} total switch number
 * @return: none
 */
void serialTotalSceneSwitchNumCallback(uint8_t switchNum)
{
    app_print("scene switch number is %d", switchNum);
    
    if(protocol_config.device_type != HA_SCENE_SELECTOR)
    {
        return;
    }

    if( switchNum > 0 && switchNum <= MAX_SCENE_SWITCH_ID)
    {
        sceneSwitchNum = switchNum;
    }
    else
    {
        app_print("scene switch number is error");
    }
}

/**
 * @description: scene switch recall callback
 * @param {switchId} switch id
 * @return: none
 */
void serialSceneSwitchRecallCallback(uint8_t switchId)
{
    app_print("%s , switchId = 0x%x \n\r", __FUNCTION__, switchId);

    if(protocol_config.device_type != HA_SCENE_SELECTOR)
    {
        return; //scene server device need do nothing with recall command
    }

    /*scene recall with current switch ID*/
    if((switchId > 0) && (switchId <= MAX_SCENE_SWITCH_ID))
    {
        uint8_t  sceneId = 0;
        bool_t status;
        uint16_t groupId = 0;

         if(FALSE == devGetSceneCallback(switchId, (uint16_t*)&groupId, (uint8_t*)&sceneId))
        {
            app_print("scene attribute error");
            return;
        }

        app_print("scene id:0x%x, group id:0x%x;", sceneId, groupId);
        if(( 0 == sceneId ) || ( 0 == groupId ))
        {
            app_print("scene id or group id is error");
            return;
        }

        status = dev_scene_recall_send_command(switchId, groupId, sceneId);

        if (status != TRUE) 
        {
            /*Consider it a failure of the timeout. We can't island ourself.*/
            app_print("sendCommand not success");
        }
    }
    else
    {
        app_print("switchID is error: %d", switchId);
    }
}
 
 /**
 * @description: remove all scenes in scene table of current endpoint; just used by scene control device; default is not remove
 * @param {endpoint} endpoint
 * @return: true: remove all, false: not remove;
 */
 bool_t zigbee_sdk_scene_remove_before_add(uint8_t endpoint)
 {
     if(protocol_config.device_type != HA_SCENE_SELECTOR)
     {
         return FALSE;
     }
     else
     {
         return TRUE; //scene selector device need to remove all scene of current endpoint befor add scene
     }
 }

