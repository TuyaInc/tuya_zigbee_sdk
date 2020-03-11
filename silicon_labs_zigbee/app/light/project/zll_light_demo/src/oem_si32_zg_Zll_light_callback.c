
#include "stdio.h"
#include "stdlib.h"
#include "mf_test_callback.h"
#include "zigbee_sdk.h"
#include "zigbee_dev_template.h"
#include "config.h"
#include "common_def.h"
#include "report_attr.h"
#include "onoff_control.h"
#include "color_control.h"
#include "level_control.h"
#include "Uart_Process.h"

//zigbee协议栈和系统基础组件初始化完毕后调用，可以使用串口，timer，自定义初始化
void dev_system_on_init(void);

//设备上电调用，需要初始化GPIO
void dev_power_on_init(void);

//实现zigbee命令接收处理
ZCL_CMD_RET_T dev_msg_recv_callback(dev_msg_t *dev_msg);

//实现情景相关功能，执行情景时调用
void dev_scene_recall_callback(uint8_t endpoint, const scene_save_data_t *in_data);
//实现情景相关功能，增加情景时调用
void dev_scene_add_callback(uint8_t endpoint, uint8_t *out_data, uint8_t *in_out_len);

//实现上电报文发送
void dev_power_on_report_info(void);

//进入模块或者成品测试时调用该函数
MF_TEST_RET_T dev_mf_test_callback(MF_TEST_CMD_T cmd, uint8_t *args, uint16_t arg_len);

//zigbee发送恢复出厂设置命令时调用，做恢复出厂设置操作
void dev_recovery_factory(void);

//zigbee网络发生状态变更时调用
void nwk_state_changed_callback(NET_EVT_T state);


#define KEY_1_PORT  PORT_F
#define KEY_1_PIN   PIN_2
#define KEY_1_MODE  GPIO_MODE_INPUT_PULL //输入上拉或者下拉，通过KEY_1_DOUT来确定
#define KEY_1_DOUT  GPIO_DOUT_HIGH  //输入上拉
#define KEY_1_DRIVE GPIO_LEVEL_LOW  //低有效

#define LED_1_PORT  PORT_A
#define LED_1_PIN   PIN_2
#define LED_1_MODE  GPIO_MODE_OUTPUT_PP 
#define LED_1_DOUT  GPIO_DOUT_HIGH  //第一次初始化输出默认高
#define LED_1_DRIVE GPIO_DOUT_LOW   //输出低有效

#define LED_2_PORT  PORT_F
#define LED_2_PIN   PIN_4
#define LED_2_MODE  GPIO_MODE_OUTPUT_PP
#define LED_2_DOUT  GPIO_DOUT_HIGH
#define LED_2_DRIVE GPIO_DOUT_LOW


typedef enum {
    DEV_WORK_ST_NORMAL = 0,
    DEV_WORK_ST_TEST
}DEV_WORK_ST_T;

static DEV_WORK_ST_T g_work_st = DEV_WORK_ST_NORMAL;

const gpio_config_t gpio_input_config[] = {
    {KEY_1_PORT, KEY_1_PIN, KEY_1_MODE, KEY_1_DOUT, KEY_1_DRIVE},
};
const gpio_config_t gpio_ouput_config[] = {
    {LED_1_PORT, LED_1_PIN, LED_1_MODE, LED_1_DOUT, LED_1_DRIVE},
    {LED_2_PORT, LED_2_PIN, LED_2_MODE, LED_2_DOUT, LED_2_DRIVE},
};

#define KEY_1_ID    0



//#define RELAY_ST_ID         2

const attr_t g_group_attr_list[] = {
    GROUP_ATTR_LIST
};

const attr_t g_scene_attr_list[] = {
    SCENE_ATTR_LIST
};

const attr_t g_light_attr_list[] = {
    DIMMER_ON_OFF_LIGHT_ATTR_LIST
};


const attr_t g_light_level_attr_list[] = {
    DIMMER_LEVEL_CONTROL_ATTR_LIST
};

const attr_t g_light_color_attr_list[] = {
    DIMMER_COLOR_CONTROL_ATTR_LIST
};


const attr_t g_light_zll_commissioning_attr_list[] = {
    DIMMER_ZLL_COMMISSIONING_ATTR_LIST
};



const cluster_t g_server_cluster_id[] = {
    DEF_CLUSTER_GROUPS_CLUSTER_ID(g_group_attr_list)
    DEF_CLUSTER_SCENES_CLUSTER_ID(g_scene_attr_list)
    DEF_CLUSTER_ON_OFF_CLUSTER_ID(g_light_attr_list)
    DEF_CLUSTER_LEVEL_CONTROL_CLUSTER_ID(g_light_level_attr_list)
    DEF_CLUSTER_COLOR_CONTROL_CLUSTER_ID(g_light_color_attr_list)
    DEF_CLUSTER_ZLL_COMMISSIONING_CLUSTER_ID(g_light_zll_commissioning_attr_list)
};

#define SERVER_CLUSTER_LEN  get_array_len(g_server_cluster_id)

const cluster_t g_client_cluster_id[] = {
    DEF_CLUSTER_ZLL_COMMISSIONING_CLUSTER_ID(g_light_zll_commissioning_attr_list)
};

#define CLIENT_CLUSTER_LEN  get_array_len(g_client_cluster_id)

const dev_description_t g_dev_des[] = {
    { 1, ZHA_PROFILE_ID, ZG_DEVICE_ID_COLOR_DIMMABLE_LIGHT, SERVER_CLUSTER_LEN, (cluster_t *)&g_server_cluster_id[0], CLIENT_CLUSTER_LEN, (cluster_t *)&g_client_cluster_id[0] },
};

#define EP_SUMS sizeof(g_dev_des)/sizeof(g_dev_des[0])


#define KEY_PUSH_TIME_FOR_ZIGBEE_JOIN_NEW   5000 //5s 
#define ZIGBEE_JOIN_MAX_TIMEOUT             180000//3min

uint8_t zcl_seq = 0;
uint8_t Dev_onoff_status[ENDPOINT_COUTER] = {1};
uint8_t Dev_level_status[ENDPOINT_COUTER] = {1};
uint16_t Dev_colortemp_status[ENDPOINT_COUTER] = {1};
zg_dev_config_t g_zg_dev_config;

static void dev_evt_callback(uint8_t evt);

#define DEMO_EVT_USE1 DEV_EVT_1
#define DEMO_EVT_USE2 DEV_EVT_2
#define DEMO_EVT_USE3 DEV_EVT_3


bool_t OnOffClusterServerCommandParse(uint8_t endpoint,attr_value_t ControlCmd);
bool_t LevelControlClusterServerCommandParse(uint8_t endpoint,attr_value_t ControlCmd);
bool_t ColorControlClusterServerCommandParse(uint8_t endpoint,attr_value_t ControlCmd);

uint8_t SceneOnOff_Flag = SCENE_FALSE;
uint8_t SceneLevel_Flag = SCENE_FALSE;
uint8_t SceneColorTemp_Flag = SCENE_FALSE;
void SceneControlOnOffEventCallback(uint8_t evnent_id);
void SceneControlLevelEventCallback(uint8_t evnent_id);
void SceneControlColorTempEventCallback(uint8_t evnent_id);


static void __dev_status_load(void)
{
    uint8_t i;
    uint8_t st;
    uint16_t temp;
    for(i=0; i<EP_SUMS; i++) {
        st = 0;
        dev_zigbee_read_attribute(
            g_dev_des[i].ep,
            CLUSTER_ON_OFF_CLUSTER_ID,
            ATTR_ON_OFF_ATTRIBUTE_ID,
            &st,
            1
        );
        Dev_onoff_status[i] = st;
        dev_zigbee_read_attribute(
            g_dev_des[i].ep,
            CLUSTER_LEVEL_CONTROL_CLUSTER_ID,
            ATTR_CURRENT_LEVEL_ATTRIBUTE_ID,
            &st,
            1
        );
        Dev_level_status[i] = st;
        dev_zigbee_read_attribute(
            g_dev_des[i].ep,
            CLUSTER_COLOR_CONTROL_CLUSTER_ID,
            ATTR_COLOR_CONTROL_COLOR_TEMPERATURE_ATTRIBUTE_ID,
            &temp,
            2
        ); 
        Dev_colortemp_status[i] = temp;
        dev_io_op(LED_LIGHT_ST_ID, (DEV_IO_ST_T)!Dev_onoff_status[i]);
        //Dev_OnOff_op(g_dev_des[i].ep, Dev_onoff_status[i]);
        //Dev_Level_op(g_dev_des[i].ep, Dev_level_status[i]);
        //Dev_ColorTemp_op(g_dev_des[i].ep,Dev_colortemp_status[i]);
    }
}

static void __dev_key_handle(uint32_t key_id, key_st_t key_st, uint32_t push_time)
{
    if(EP_SUMS <= key_id) {
        return;
    }
    if(g_work_st == DEV_WORK_ST_TEST) {
        if(key_st != KEY_ST_PUSH) {
            dev_mf_button_test_notify(key_id);
        }
        return;
    }
    switch(key_id) {
        case KEY_1_ID: {
            uint8_t ep = 1;
            if(key_st == KEY_ST_PUSH) {
                if(push_time == KEY_PUSH_TIME_FOR_ZIGBEE_JOIN_NEW) {
                    //长按了10s,开始闪烁提醒用户即将进入配网
                    dev_led_start_blink(LED_ZIGBEE_ST_ID, 500, 500, DEV_LED_BLINK_FOREVER, DEV_IO_OFF);
                }
            } 
            else {
                /*松开按钮后，发现长按的时间在10s内，进行短按操作*/
                if(push_time < KEY_PUSH_TIME_FOR_ZIGBEE_JOIN_NEW) {
                    //__dev_switch_op(ep, (DEV_IO_ST_T)!Dev_onoff_status[ep-1]);
                    //__dev_report_onoff_msg(ep);

                } 
                else { //松开后，发现长按超过了10s，进行真正配网
                    dev_zigbee_join_start(ZIGBEE_JOIN_MAX_TIMEOUT); //参数是配网最大超时
                }
            }
            break;
        }
        default: {
            break;
        }
    }
}



bool_t Dev_OnOff_op(uint8_t ep, bool_t Onoff)
{
    Dev_onoff_status[ep-1] = Onoff;
    switch(ep) {
        case 1:
//            dev_io_op(RELAY_ST_ID, (DEV_IO_ST_T)g_relay_onoff_status[ep-1]);
            Send_Control_Light(0);
            
            break;
        default:
            break;
    }  
    return TRUE;
}


bool_t Dev_Level_op(uint8_t ep, uint8_t  Levelval)
{
    Dev_level_status[ep-1] = Levelval;
    switch(ep) {
        case 1:
//            dev_io_op(RELAY_ST_ID, (DEV_IO_ST_T)g_relay_onoff_status[ep-1]);
//            dev_io_op(LED_RELAY_ST_ID, (DEV_IO_ST_T)Dev_onoff_status[ep-1]);
            Send_Control_Light(0);
            break;
        default:
            break;
    }  
    return TRUE;  
}

bool_t Dev_ColorTemp_op(uint8_t ep, uint16_t  TempVal)
{
    Dev_colortemp_status[ep-1] = TempVal;
    switch(ep) {
        case 1:
//            dev_io_op(RELAY_ST_ID, (DEV_IO_ST_T)g_relay_onoff_status[ep-1]);
//            dev_io_op(LED_RELAY_ST_ID, (DEV_IO_ST_T)Dev_onoff_status[ep-1]);
            Send_Control_Light(0);
            break;
        default:
            break;
    }    
    return TRUE;
}


/***********************************************************
*  Function: dev_recovery_factory
*  description: app发送恢复出厂设置时调用该函数，
                用户可以    清除相关历史数据
*  Input: none
*  Output: none
*  Return:none
***********************************************************/
void dev_recovery_factory(void)
{

}

/***********************************************************
*  Function: dev_power_on_report_info
*  description: 设备上电时到了可以发送报文的时机调用，
                用户可以上报除了版本号意外的额外信息，
                如果不做处理，SDK内部上报版本号
*  Input: none
*  Output: none
*  Return:none
***********************************************************/
void dev_power_on_report_info(void)
{

}


/***********************************************************
*  Function: dev_mf_test_callback
*  description: 进入产测后，会调用该函数
*  Input: MF_TEST_INNER_CMD_T:参数命令
          args 命令参数
          arg_len 命令参数长度
*  Output: none
*  Return:
    MF_TEST_SUCCESS, 测试成功,
    MF_TEST_FAILED, 测试失败
    MF_TEST_DOING 正在测试，异步测试用该返回值
***********************************************************/
MF_TEST_RET_T dev_mf_test_callback(MF_TEST_CMD_T cmd, uint8_t *args, uint16_t arg_len)
{
    switch(cmd) {
        case MF_TEST_LED_ON_ALL: {
            dev_led_stop_blink(LED_ZIGBEE_ST_ID, DEV_IO_ON);
            dev_led_stop_blink(LED_LIGHT_ST_ID, DEV_IO_ON);
            break;
        }
        case MF_TEST_LED_OFF_ALL: {
            dev_led_stop_blink(LED_ZIGBEE_ST_ID, DEV_IO_OFF);
            dev_led_stop_blink(LED_LIGHT_ST_ID, DEV_IO_OFF);
            break;
        }
        case MF_TEST_LED_BLINK_ALL: {
            dev_led_start_blink(LED_ZIGBEE_ST_ID, 500, 500, 4, DEV_IO_OFF);
            dev_led_start_blink(LED_LIGHT_ST_ID, 500, 500, 4, DEV_IO_OFF);
            break;
        }
        case MF_TEST_RELAY_ON_ALL: {
//            dev_led_stop_blink(RELAY_ST_ID, DEV_IO_ON);
            break;
        }
        case MF_TEST_RELAY_OFF_ALL: {
//            dev_led_stop_blink(RELAY_ST_ID, DEV_IO_OFF);
            break;
        }
        case MF_TEST_RELAY_BLINK_ALL: {
//            dev_led_start_blink(RELAY_ST_ID, 500, 500, 4, DEV_IO_OFF);
            break;
        }
        case MF_TEST_BUTTON: {
            g_work_st = DEV_WORK_ST_TEST;
            return MF_TEST_DOING;
        }
        
        default : {
            break;
        }
    }
    return MF_TEST_SUCCESS;
}


/***********************************************************
*  Function: dev_scene_add_callback
*  description: 收到增加情景模式调用，用户填充自定义数据
*  Input: 
    endpoint 需要增加情景的endpoint
*  Output: 
    out_data: 用户填充的当前设备的状态数据
    in_out_len: out_data长度
*  Return: none
***********************************************************/
void dev_scene_add_callback(uint8_t endpoint, uint8_t *out_data, uint8_t *in_out_len)
{
    *in_out_len = 0;
    out_data[(*in_out_len)++] = SceneOnOff_Flag;
    out_data[(*in_out_len)++] = Dev_onoff_status[endpoint-1];
    out_data[(*in_out_len)++] = SceneLevel_Flag;
    out_data[(*in_out_len)++] = Dev_level_status[endpoint-1];
    out_data[(*in_out_len)++] = SceneColorTemp_Flag;
    out_data[(*in_out_len)++] =(uint8_t)(Dev_colortemp_status[endpoint-1]>>8) & 0xff;
    out_data[(*in_out_len)++] =(uint8_t)(Dev_colortemp_status[endpoint-1]>>0) & 0xff;
}

/***********************************************************
*  Function: dev_scene_recall_callback
*  description: 执行情景模式调用，用户根据之前填充的数据改变设备状态
*  Input: 
    endpoint 需要执行情景的endpoint
    data     用户在dev_scene_add_callback填充的数据
        data->type 
            SCENE_DATA_TYPE_INNER_YOUR_SELF代表用户自定义数据
            SCENE_DATA_TYPE_EXT_SERVER代表服务器配置的数据
*  Output: none
*  Return: none
***********************************************************/
void dev_scene_recall_callback(uint8_t endpoint, const scene_save_data_t *in_data)
{
    uint8_t dataOffset = 0;
    switch(in_data->type) {
        case SCENE_DATA_TYPE_USER_DEFINE: {
          
            if(in_data->data[dataOffset] == SCENE_TRUE)
            {
                Dev_onoff_status[endpoint-1] = in_data->data[dataOffset + 1];
                dev_report_onoff_msg(endpoint,Dev_onoff_status[endpoint-1]);
            }
            dataOffset += 2;
            if(in_data->data[dataOffset] == SCENE_TRUE)
            {
                Dev_level_status[endpoint-1] = in_data->data[dataOffset + 1];
                dev_report_level_msg(endpoint,Dev_level_status[endpoint-1]);
            }
            dataOffset += 2;
            if(in_data->data[dataOffset] == SCENE_TRUE)
            {
                Dev_colortemp_status[endpoint-1] = in_data->data[dataOffset + 1];
                Dev_colortemp_status[endpoint-1] <<=8;
                Dev_colortemp_status[endpoint-1] |= in_data->data[dataOffset + 2]; 
                dev_report_ColorTemp_msg(endpoint,Dev_colortemp_status[endpoint-1]);
            }
            Dev_OnOff_op(endpoint,Dev_onoff_status[endpoint-1]);
            break;
        }
        case SCENE_DATA_TYPE_SERVER_DEFINE: //根据服务器的配置格式解析
            break;
        case SCENE_DATA_TYPE_ZIGBEE_DEFINE: //根据zigbee标准协议格式解析
            break;
        default:
            break;
    }

}

void SceneControlOnOffEventCallback(uint8_t evnent_id)
{
    SceneOnOff_Flag = SCENE_FALSE;    
}

void SceneControlLevelEventCallback(uint8_t evnent_id)
{
    SceneLevel_Flag = SCENE_FALSE;    
}

void SceneControlColorTempEventCallback(uint8_t evnent_id)
{
    SceneColorTemp_Flag = SCENE_FALSE;    
}

/***********************************************************
*  Function: dev_msg_write_attr_callback
*  description: 写属性后回调
*  Input: 
*  Output: none
*  Return: ZCL_CMD_RET_T 命令执行结果反馈
***********************************************************/
void dev_msg_write_attr_callback(uint8_t endpoint, CLUSTER_ID_T cluster, uint16_t attr_id)
{


}




/***********************************************************
*  Function: dev_msg_recv_callback
*  description: 收到zigbee数据，调用该函数
*  Input: 
    dev_msg_t *cmd 消息结构体
*  Output: none
*  Return: ZCL_CMD_RET_T 命令执行结果反馈
***********************************************************/
ZCL_CMD_RET_T dev_msg_recv_callback(dev_msg_t *dev_msg)
{
    ZCL_CMD_RET_T result = ZCL_CMD_RET_SUCCESS;

    switch (dev_msg->cluster) {
        case CLUSTER_PRIVATE_TUYA_CLUSTER_ID: { //私有数据处理
            uint8_t len = dev_msg->data.bare_data.len;
            uint8_t *data = dev_msg->data.bare_data.data;
            //todo:
            break;
        }

        //标准数据处理
        case CLUSTER_ON_OFF_CLUSTER_ID: {
            attr_value_t *attr_list = dev_msg->data.attr_data.attr_value;
//            uint8_t attr_sums = dev_msg->data.attr_data.attr_value_sums;
//            uint8_t i;
            //for(i=0; i<attr_sums; i++)
            {
                OnOffClusterServerCommandParse(dev_msg->endpoint,*attr_list);
            }
            break;
        }
        //标准数据处理
        case CLUSTER_LEVEL_CONTROL_CLUSTER_ID: {
            attr_value_t *attr_list = dev_msg->data.attr_data.attr_value;
//            uint8_t attr_sums = dev_msg->data.attr_data.attr_value_sums;
//            uint8_t i;

            //for(i=0; i<attr_sums; i++)
            {
                LevelControlClusterServerCommandParse(dev_msg->endpoint,*attr_list);
            }
            break;
        }
        //标准数据处理
        case CLUSTER_COLOR_CONTROL_CLUSTER_ID: {
            attr_value_t *attr_list = dev_msg->data.attr_data.attr_value;
//            uint8_t attr_sums = dev_msg->data.attr_data.attr_value_sums;
//            uint8_t i;

            //for(i=0; i<attr_sums; i++) 
            {
                ColorControlClusterServerCommandParse(dev_msg->endpoint,*attr_list);
            }
            
        }
        default:
            // Unrecognized cluster ID, error status will apply.
            break;
    }

    return result;
}

void gpio_test(GPIO_PORT_T port, GPIO_PIN_T pin)
{
    Dev_OnOff_op(1, (DEV_IO_ST_T)!Dev_onoff_status[1-1]);
    dev_report_onoff_msg(1,Dev_onoff_status[0]);
}


/***********************************************************
*  Function: dev_power_on_init
*  description: 上电初始化函数,CPU刚启动就会调用该函数
*  Input: none
*  Output: none
*  Return: none
***********************************************************/
void dev_power_on_init(void)
{
    dev_register_zg_ep_infor((dev_description_t *)g_dev_des, EP_SUMS);

    zg_dev_config_t g_zg_dev_config;
    g_zg_dev_config.dev_type = ZG_ROUTER;
    g_zg_dev_config.config.router_cfg.reserved = 0;
    dev_register_zg_dev_config(&g_zg_dev_config);

    join_config_t cfg;
    cfg.auto_join_power_on_flag = 0;
    cfg.auto_join_remote_leave_flag = 1;
    cfg.join_timeout = ZIGBEE_JOIN_MAX_TIMEOUT;
    dev_zg_join_config(&cfg);

    //gpio_int_register(gpio_input_config, gpio_test);
    gpio_button_init((gpio_config_t *)gpio_input_config, get_array_len(gpio_input_config), 50, __dev_key_handle);
    gpio_output_init((gpio_config_t *)gpio_ouput_config, get_array_len(gpio_ouput_config));


}



/***********************************************************
*  Function: dev_system_on_init
*  description: 系统启动后初始化函数，系统启动好后会调用该
                函数,可以使用启动事件
*  Input: none
*  Output: none
*  Return: none
***********************************************************/
void dev_system_on_init(void)
{
    emberSetRadioPower(11);
    /*uart_config_t config = {
        UART_ID,
        UART_BAUD_RATE,
        UART_PARITY,
        UART_STOP_BITS,
        NULL
    };

    hardware_uart_init(&config);*/
    //__dev_status_save();
    //dev_led_stop_blink(LED_ZIGBEE_ST_ID, DEV_IO_ON);
    //dev_led_stop_blink(LED_LIGHT_ST_ID, DEV_IO_ON);
    __dev_status_load();
    
//    dev_timer_start_with_callback(DEMO_EVT_USE1, 3000, dev_evt_callback);
    //dev_timer_start_with_callback(DEMO_EVT_USE2, 2000, dev_evt_callback);
    //dev_timer_start_with_callback(DEMO_EVT_USE3, 60000, dev_evt_callback);
    
    user_uart_config_t uart0_config={\
        UART_ID_UART0,\
        UART_PIN_TYPE_CONFIG,\
        {PORT_A, PIN_0},\
        {PORT_A, PIN_1},\
        LOC_0,\
        LOC_0,\
        9600,\
        USART_PARITY_NONE,\
        USART_STOPBITS_ONE,\
        USART_DATABITS_8BIT,\
        NULL\
    };
    uart0_config.func = Uart_Parse_callback;
    user_uart_init(&uart0_config);
    //todo:user init code

}

extern void emberAfAddToCurrentAppTasksCallback(uint32_t tasks);
extern void emberAfPluginEndDeviceSupportPollingNetworkEventHandler(void);
/***********************************************************
*  Function: dev_evt_callback
*  description: 事件有效后，调用该函数
*  Input: DEV_EVT_T: 用户事件id
*  Output: none
*  Return: none
***********************************************************/
static void dev_evt_callback(uint8_t evt)
{
    switch(evt) {
        case DEMO_EVT_USE1:
            uint8_t i;
            for(i=0; i<EP_SUMS; i++) {
                Dev_OnOff_op(g_dev_des[i].ep, Dev_onoff_status[i]);
//                Dev_Level_op(g_dev_des[i].ep, Dev_level_status[i]);
//                Dev_ColorTemp_op(g_dev_des[i].ep,Dev_colortemp_status[i]);
                dev_report_onoff_msg(g_dev_des[i].ep,Dev_onoff_status[i]);
                dev_report_level_msg(g_dev_des[i].ep,Dev_level_status[i]);
                dev_report_ColorTemp_msg(g_dev_des[i].ep,Dev_colortemp_status[i]);
            }
            break;
        case DEMO_EVT_USE2:
            //dev_led_stop_blink(LED_ZIGBEE_ST_ID, DEV_IO_OFF);
            //dev_led_stop_blink(LED_RELAY_ST_ID, DEV_IO_OFF);
            break;
        case DEMO_EVT_USE3:
           //COM_WriteData(HAL_SERIAL_PORT_USART0, "DEMO_EVT_USE3\n", sizeof("DEMO_EVT_USE3\n"));
            
           // dev_report_onoff_msg(1,Dev_onoff_status[0]); // for test
            break;
        default:
            break;
    }
}


void nwk_state_changed_callback(NET_EVT_T state)
{
    switch(state) {
        case NET_POWER_ON_LEAVE: {
            app_print("nwk state is NET_IDLE");
            dev_led_stop_blink(LED_ZIGBEE_ST_ID, DEV_IO_ON);
            //ZllCreateDistributedNetwork();
            break;
        }

        case NET_JOIN_START: {
            dev_led_start_blink(LED_ZIGBEE_ST_ID, 500, 500, DEV_LED_BLINK_FOREVER, DEV_IO_ON);
            
            break;
        }
        case NET_JOIN_TIMEOUT: {
            dev_led_stop_blink(LED_ZIGBEE_ST_ID, DEV_IO_ON);
            //ZllCreateDistributedNetwork();
            break;
        }
        case NET_POWER_ON_ONLINE:
        case NET_JOIN_OK: {
            dev_led_stop_blink(LED_ZIGBEE_ST_ID, DEV_IO_OFF);
            dev_timer_start_with_callback(DEMO_EVT_USE1, 3000, dev_evt_callback);
            break;
        }
        case NET_LOST: {
            break;
        }
        case NET_REMOTE_LEAVE: {
            dev_led_start_blink(LED_ZIGBEE_ST_ID, 500, 500, DEV_LED_BLINK_FOREVER, DEV_IO_ON);
            break;
        }
        case NET_LOCAL_LEAVE: {
            break;
        }
        case NET_ZLL_JOINED: {
            break;
        }
        case NET_ZLL_LEAVE: {
            break;
        }
        default: {
            break;
        }
    }
}


/**
 * @description: The identify request command callback is used to request that
 *      the recipient identifies itself in some application specific way to aid with touchlinking
 * @param {durationS} (unit:S)
 *          0x0000 Exit identify mode
 *          0x0001 - 0xfffe Number of seconds to remain in identify mode
 *          0xffff Remain in identify mode for a default time known by the receiver
 * @return: none
 */
void zll_server_identify_callback(uint16_t durationS)
{

}

/**
 * @description: zll Server joined a new network with touchlink 
 * @param {panId} the new network panid
 * @return: none 
 */
void zll_Server_touchlink_joined(uint16_t panId )
{
    
}
/**
 * @description: zll Server leave network with touchlink
 * @param {none} 
 * @return: none
 */
void zll_Server_touchlink_leave(void)
{
    
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
    else if ((MODULE_NAME == TYZS3) || (MODULE_NAME == TYZS13))
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