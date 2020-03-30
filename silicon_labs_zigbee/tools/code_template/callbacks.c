/**
 * @author Leon
 * @par email:
 *      zhangpeng@tuya.com
 * @file tuya_zigbee_sdk.h
 * @brief This is a template file.
 * MUST         means MUST be implemented.\n
 * OPTION       means optional.
 * @copyright HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @par company
 *      http://www.tuya.com
 * @date 2020-03-20 17:29:33
 */

#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "tuya_zigbee_sdk.h"


/**
 * @note (MUST) This is the first function after the hardware starts.
 * The CPU and base clock are initialized before calling this function.
 * You need to implement the creation of Zigbee devices and 
 * determine the parameters of Zigbee device behavior.Include device roles(router, end device), 
 * device networking(join), rejoin parameters, and more. Refer to the TUYA Zigbee SDK demo for details.
 * @param none
 * @return none
 */
void dev_power_on_init(void)
{
    // TODO
}

/**
 * @note (MUST) This is the first function after system startup. 
 * Before calling this function, Zigbee stack and some basic components have been started.
 * You can use all API except individual ones. API limits refer to the API limits description table
 * @param none
 * @return none
 */
void dev_system_on_init(void)
{
    // TODO

    return;
}

/**
 * @note (MUST) This function is invoked when the network state changes.
 * Handling network-related matters at this function is recommended.
 * @param[in] {state} Refer to NET_EVT_T for more detal.
 * @return none
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
 * @note (OPTION) device manufactury test callback, when device is in manufactury test model,
 * sdk will use this callback to notify application the test item and test command;
 * @param[in] {cmd} manufactury test type
 * @param[in] {args} manufactury test data
 * @param[in] {arg_len} manufactury test data length
 * @return none
 */
MF_TEST_RET_T dev_mf_test_callback(MF_TEST_CMD_T cmd, uint8_t *args, uint16_t arg_len)
{
    //TODO

    return MF_TEST_SUCCESS;
}

/**
 * @note (OPTION) This function is called when a beacon test is received.
 * You can do test in this function.
 * @param none
 * @return none
 */
void dev_beacon_test_callback(void)
{
    // TODO

    return;
}

/**
 * @note (OPTION) device report inforamtion when first power on(if need),;
 * @param none
 * @return none
 */
void dev_power_on_report_info(void)
{
    //TODO

    return;
}

/**
 * @note (OPTION) This function is called when the factory Settings recovery command is received.
 * You can do things like data recovery
 * @param[in] {type} The type of recovery that represents whether the Zigbee SDK needs to 
 * leave the Zigbee network after calling this function
 * @return none
 */
void dev_recovery_factory(DEV_RESET_TYPE_T type)
{
    //TODO

    return;
}

/**
 * @note (MUST) This function is invoked when a special command related to Zigbee Cluster is received.\n
 * Global write-related operations are handled by dev_msg_write_attr_callback.\n
 * Global read related operations have no entry for the time being.
 * @param[in] {dev_msg} contains the received data. Refer to dev_msg_t structure definition for details
 * @return {ZCL_CMD_RET_T}\n
 * ZCL_CMD_RET_SUCCESS means that it has been processed and no default ack is required\n
 * ZCL_CMD_RET_FAILED  means that no corresponding processing was found. 
 *                         The TUYA Zigbee SDK will generate a default ack to the source address
 */
ZCL_CMD_RET_T dev_msg_recv_callback(dev_msg_t *dev_msg)
{
    //TODO

    return ZCL_CMD_RET_SUCCESS;
}

/**
 * @note (OPTION) This function is used with dev_scene_add_callback[_with_ext_data].
 * This function is called when it receives the "recall scene" command.
 * This scene_save_data_t structure stores the data stored as "add scene.
 * @param[in] {endpoint} Zigbee endpoint
 * @param[in] {in_data} Refer to scene_save_data_t for details.
 * @return none
 */
void dev_scene_recall_callback(uint8_t endpoint, const scene_save_data_t *in_data)
{
    //TODO

    return;
}

/**
 * @note (OPTION) This function is called when the "add scene" command is received.
 * All you need to do is put the endpoint status data memcpy to out_data in this function.
 * This saves the scenario data at this time.When the "recall scene" command is received, 
 * dev_scene_recall_callback is called, and the saved data is passed to the function as 
 * a parameter to restore the scene.
 * @param[in] {endpoint} Zigbee endpoint
 * @param[out] {out_data} Where the endpoint state data is stored
 * @param[in]  {in_out_len} \n
 * @param[out] {in_out_len} \n
    Indicates the length of the buffer for out_data when used as input.\n
    Indicates the length of data that needs to be saved when used as output
 * @return none
 */
void dev_scene_add_callback(uint8_t endpoint, uint8_t *out_data, uint8_t *in_out_len)
{
    //TODO

    return;
}

/**
 * @note (OPTION) This function is called when it receives the "write attribute" command.
 * @param[in] {endpoint} Zigbee endpoint
 * @param[in] {cluster} Zigbee cluster
 * @param[in] {attr_id} Zigbee attribute
 * @return none
 */
void dev_msg_write_attr_callback(uint8_t endpoint, CLUSTER_ID_T cluster, uint16_t attr_id)
{
    //TODO

    return;
}

/**
 * @note (OPTION) This function is similar to dev_msg_write_attr_callback.
 * More details are passed in as function arguments.
 * @param[in] {endpoint} Zigbee endpoint
 * @param[in] {cluster} Zigbee cluster
 * @param[in] {attr_id} Zigbee attribute
 * @param[in] {mask} \n
        is a server attribute (0x40)\n
        is a client attribute (0x80)
 * @param[in] {manufacturer_code} Zigbee manufacturer code
 * @param[in] {type} Zigbee attribute data type(eg: u8 u16 u32 ...)
 * @param[in] {size} The length of value
 * @param[in] {value} The original argument of the "write attribute" command
 * @return none
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
 * @note mf test uart register, aotomatic generated, not edit
 * @param none
 * @return user_uart_config_t mf uart config information
 */
user_uart_config_t* mf_test_uart_config(void)
{
    static user_uart_config_t config;
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
    else if(MODULE_NAME == ZS3L)
    {
        user_uart_config_t default_config = ZS3L_USART_CONFIG_DEFAULT;
        memcpy(&config, &default_config, sizeof(user_uart_config_t));
    }
    else
    {
        user_uart_config_t default_config = TYZS3_USART_CONFIG_DEFAULT;
        memcpy(&config, &default_config, sizeof(user_uart_config_t));
    }

    return &config;
}
