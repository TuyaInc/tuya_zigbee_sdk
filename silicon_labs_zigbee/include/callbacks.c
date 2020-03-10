/**
 * @Author: Leon
 * @email: zhangpeng@tuya.com
 * @LastEditors: Zhu Shaoling
 * @file name: callbacks.c
 * @Description: this file is a template of the sdk callbacks function,
 * user need to achieve the function yourself
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-03-12 17:29:33
 * @LastEditTime: 2019-04-17 15:51:35
 */

#include "stdio.h"
#include "stdlib.h"
#include "mf_test_callback.h"
#include "zigbee_sdk.h"
#include "zigbee_dev_template.h"
#include "config.h"

// #if (LED_ENABLE == true)
// extern gpio_config_t gpio_output_config[LED_NUM];
// #endif

// #if (KEY_ENABLE == true)
// extern gpio_config_t gpio_input_config[KEY_NUM];
// #endif

static void dev_key_handle(uint32_t key_id, key_st_t key_st, uint32_t push_time);

/**
 * @description: device first power on, user can register device and init the gpio
 * @param {type} none
 * @return: none
 */
void dev_power_on_init(void)
{
    /*
    // If OEM device
    uint16_t len;
    uint8_t *data;
    oem_config_load(&len, &data);
    */

    //TODO
// #if (LED_ENABLE == true)    
//     gpio_output_init((gpio_config_t *)gpio_output_config, get_array_len(gpio_output_config));
// #endif

// #if (KEY_ENABLE == true)
//     gpio_button_init((gpio_config_t *)gpio_input_config, get_array_len(gpio_input_config), 50, dev_key_handle);
// #endif
//     return;
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
    // TODO

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
            break;
        }

        case NET_JOIN_START: {
            break;
        }
        case NET_JOIN_TIMEOUT: {
            break;
        }
        case NET_POWER_ON_ONLINE:
        case NET_JOIN_OK: {
            break;
        }
        case NET_LOST: {
            break;
        }
        case NET_REMOTE_LEAVE: {
            break;
        }
        case NET_LOCAL_LEAVE: {
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
 * @description: device manufactury beacon test callback, when device is in manufactury test mode,
 * sdk will use this callback to notify application to enter unsupervised production test;
 * @param {type} none
 * @return: none
 */
void dev_beacon_test_callback(void)
{
    // TODO

    return;
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
 * @param {type} just reset to fdactury or reset to factury and leave
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
    //TODO

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
    //TODO

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
 * @description: write sttribute callback function
 * @param {endpoint} the endpoint of the write attribute
 * @param {cluster} the cluster id of the write attribute
 * @param {attr_id} the attribute id  of the write attribute
 * @param {mask} the attribute id mask
 * @param {manufacturerCode} manufacturerCode
 * @param {type} attribute data type
 * @param {size} data length
 * @param {value} attribute value
 * @return: none
 */
void dev_msg_write_attr_callback_ext(
    uint8_t endpoint, 
    CLUSTER_ID_T cluster, 
    uint16_t attr_id,
    uint8_t mask,
    uint16_t manufacturerCode,
    uint8_t type,
    uint8_t size,
    uint8_t* value)
{
    //TODO

    return;
}


/**
 * @description: key event handler 
 * @param {key_id} key  id
 * @param {key_st} key status
 * @param {push_time} key push time
 * @return: none
 */
static void dev_key_handle(uint32_t key_id, key_st_t key_st, uint32_t push_time)
{
    //TODO

    return;
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
    else if(MODULE_NAME == TYZS5)
    {
        user_uart_config_t default_config = TYZS5_USART_CONFIG_DEFAULT;
        memcpy(&config, &default_config, sizeof(user_uart_config_t));
    }
	// else if(MODULE_NAME == TYZS2) //the current lib is efr32mg13p732, not support module TYZS2
	// {
	// 	user_uart_config_t default_config = TYZS2_USART_CONFIG_DEFAULT;
    //     memcpy(&config, &default_config, sizeof(user_uart_config_t));
	// }
    else
    {
        user_uart_config_t default_config = TYZS3_USART_CONFIG_DEFAULT;
        memcpy(&config, &default_config, sizeof(user_uart_config_t));
    }

    return &config;
}
