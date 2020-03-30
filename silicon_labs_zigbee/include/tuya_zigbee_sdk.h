/**
 * @author Leon
 * @par email:
 *      zhangpeng@tuya.com
 * @file tuya_zigbee_sdk.h
 * @brief This is the main entry to use the TUYA Zigbee SDK, 
 * including hardware, systems, zigbee protocol stack, and some common tools.\n
 * VIRTUAL_FUNC means user implementation.\n
 * MUST         means MUST be implemented.\n
 * OPTION       means optional.
 * @copyright HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @par company
 *      http://www.tuya.com
 * @date 2020-03-20 17:29:33
 */
 
#ifndef __TUYA_ZIGBEE_SDK_H__
#define __TUYA_ZIGBEE_SDK_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "type_def.h"
#include "hal_gpio.h"
#include "hal_adc.h"
#include "hal_spi.h"
#include "hal_uart.h"
#include "hal_i2c.h"
#include "hal_battery.h"
#include "hal_timer.h"
#include "hal_pwm.h"
#include "hal_systick_timer.h"
#include "hal_flash.h"

#include "zigbee_attr.h"
#include "zigbee_cmd.h"
#include "zigbee_dev_template.h"
#include "zigbee_raw_cmd_api.h"

#include "tuya_zigbee_stack.h"
#include "tuya_oem_kit.h"
#include "tuya_zigbee_modules.h"
#include "tuya_mf_test.h"
#include "tuya_mcu_os.h"
#include "tuya_app_timer.h"
#include "tuya_tools.h"


/**
 * @remarks (OPTION) This is a debug macro define.You need to initialize the serial port before using it.
 */
#ifdef APP_DEBUG
#define app_print(...)     uart_printf(UART_ID_UART0, __VA_ARGS__)
#define APP_PRINT(fmt,...) app_print("%s(%d)-<%s>: "##fmt"\r\n",__FILE__, __LINE__, __FUNCTION__,##__VA_ARGS__)
#else
#define app_print(...)
#define APP_PRINT(...)
#endif

///@name Functions that must be implemented
/**
 * @note (MUST) This is the first function after the hardware starts.
 * The CPU and base clock are initialized before calling this function.
 * You need to implement the creation of Zigbee devices and 
 * determine the parameters of Zigbee device behavior.Include device roles(router, end device), 
 * device networking(join), rejoin parameters, and more. Refer to the TUYA Zigbee SDK demo for details.
 * @param none
 * @return none
 */
VIRTUAL_FUNC void dev_power_on_init(void);


/**
 * @note (MUST) This is the first function after system startup. 
 * Before calling this function, Zigbee stack and some basic components have been started.
 * You can use all API except individual ones. API limits refer to the API limits description table
 * @param none
 * @return none
 */
VIRTUAL_FUNC void dev_system_on_init(void);



/**
 * @note (MUST) This function is invoked when the network state changes.
 * Handling network-related matters at this function is recommended.
 * @param[in] {state} Refer to NET_EVT_T for more detal.
 * @return none
 */
VIRTUAL_FUNC void nwk_state_changed_callback(NET_EVT_T state);

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
VIRTUAL_FUNC ZCL_CMD_RET_T dev_msg_recv_callback(dev_msg_t *dev_msg);


///@name General network command processing
/**
 * @note (OPTION) This function is called when the factory Settings recovery command is received.
 * You can do things like data recovery
 * @param[in] {type} The type of recovery that represents whether the Zigbee SDK needs to 
 * leave the Zigbee network after calling this function
 * @return none
 */
VIRTUAL_FUNC void dev_recovery_factory(DEV_RESET_TYPE_T type);

/**
 * @note (OPTION) This function is called when an OTA event occurs.Includes the beginning and end of the OTA
 * @param[in] {evt} The type of OTA event. Refer to enum ZG_OTA_EVT_T definition for details.
 * @return none
 */
VIRTUAL_FUNC void zg_ota_evt_callback(ZG_OTA_EVT_T evt);

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
VIRTUAL_FUNC void dev_scene_add_callback(uint8_t endpoint, uint8_t *out_data, uint8_t *in_out_len);

/**
 * @note (OPTION) This function is similar to dev_scene_add_callback.
 * Two input parameters have been added. These two parameters are derived from 
 * the extension field of the "enhance add scene" command
 * @param[in] {endpoint} Zigbee endpoint
 * @param[in] {ext_data} The extension field of the "enhance add scene" command
 * @param[in] {ext_data_len} Indicates the length of ext_data
 * @param[out] {out_data} Where the endpoint state data is stored
 * @param[in] {in_out_len} \n
 * @param[out] {in_out_len} \n
    Indicates the length of the buffer for out_data when used as input.\n
    Indicates the length of data that needs to be saved when used as output
 * @return none
 */
VIRTUAL_FUNC void dev_scene_add_callback_with_ext_data(uint8_t endpoint, uint8_t *ext_data, uint8_t ext_data_len, uint8_t *out_data, uint8_t *in_out_len);

/**
 * @note (OPTION) This function is used with dev_scene_add_callback[_with_ext_data].
 * This function is called when it receives the "recall scene" command.
 * This scene_save_data_t structure stores the data stored as "add scene.
 * @param[in] {endpoint} Zigbee endpoint
 * @param[in] {in_data} Refer to scene_save_data_t for details.
 * @return none
 */
VIRTUAL_FUNC void dev_scene_recall_callback(uint8_t endpoint, const scene_save_data_t *in_data);

/**
 * @note (OPTION) This function is called when it receives the "write attribute" command.
 * @param[in] {endpoint} Zigbee endpoint
 * @param[in] {cluster} Zigbee cluster
 * @param[in] {attr_id} Zigbee attribute
 * @return none
 */
VIRTUAL_FUNC void dev_msg_write_attr_callback(uint8_t endpoint, CLUSTER_ID_T cluster, uint16_t attr_id);

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
VIRTUAL_FUNC void dev_msg_write_attr_callback_ext(
    uint8_t endpoint,
    CLUSTER_ID_T cluster,
    uint16_t attr_id,
    uint8_t mask,
    uint16_t manufacturer_code,
    uint8_t type,
    uint8_t size,
    uint8_t* value);

/**
 * @note (OPTION) This function is called when the global command is received.
 * Most of you do not need to care about the global command, 
 * the main purpose is for you to do special processing
 * @param[in] {args} Raw data for the global command, Refer to ext_plugin_args_t for more details.
 * @return none
 */
VIRTUAL_FUNC void dev_global_response_cmd_handle(ext_plugin_args_t *args);


///@name Special network scenarios are used
/**
 * @note (OPTION) When the Zigbee network state becomes NET_LOST, 
 * the Zigbee SDK tries to send a beacon request in order to rejoin. 
 * The default is to send a beacon request immediately after NET_LOST.
 * You can change the interval by the return value of this function.
 * This interval is in milliseconds.
 * @param none 
 * @return This interval.
 */
VIRTUAL_FUNC uint32_t dev_get_rejoin_delay_time_after_poll_failed(void);

/**
 * @note (OPTION) This function is called when the data request fails 
 * due to wireless interference.You can do some statistical work in this function for signal quality
 * @param none 
 * @return none
 */
VIRTUAL_FUNC void poll_busy_callback(void);

/**
 * @note (OPTION) This function is called when join to TUYA gateway.
 * You can do some special adaptation work.
 * @param[in] {args} It is currently an NULL parameter
 * @return none
 */
VIRTUAL_FUNC void zg_join_tuya_gw_callback(void *args);

/**
 * @note (OPTION) device report inforamtion when first power on(if need),;
 * @param none
 * @return none
 */
VIRTUAL_FUNC void dev_power_on_report_info(void);


///@name Functions used in a particular startup order
/**
 * @note (OPTION) This function is called before a production test.
 * You can do some special work.
 * @param none
 * @return none
 */
VIRTUAL_FUNC void dev_init_before_mf_test(void);

/**
 * @note (OPTION) This function is called before the hardware is initialized.
 * You can do some special work.
 * @param none
 * @return none
 */
VIRTUAL_FUNC void before_hal_init(void);

/**
 * @note (OPTION) This function is called after the hardware is initialized.
 * You can do some special work.
 * @param none
 * @return none
 */
VIRTUAL_FUNC void after_hal_init(void);


///@name For production testing
/**
 * @note (OPTION) This function is called when a beacon test is received.
 * You can do test in this function.
 * @param none
 * @return none
 */
VIRTUAL_FUNC void dev_beacon_test_callback(void);

/**
 * @note (OPTION) device manufactury test callback, when device is in manufactury test model,
 * sdk will use this callback to notify application the test item and test command;
 * @param[in] {cmd} manufactury test type
 * @param[in] {args} manufactury test data
 * @param[in] {arg_len} manufactury test data length
 * @return none
 */
VIRTUAL_FUNC MF_TEST_RET_T dev_mf_test_callback(MF_TEST_CMD_T cmd, uint8_t *args, uint16_t arg_len);


///@name Speical endpoint setting
/**
 * @note (OPTION) Adapt to non-zigbee3.0 products.
 * Get the standard version of zigbee corresponding to the endpoint
 * @param[in] {ep} Zigbee endpoint
 * @return {DEV_VER_T} \n
 * Refer to DEV_VER_T for detail.
 */
VIRTUAL_FUNC DEV_VER_T dev_ep_to_dev_ver(uint8_t ep);


///@name Wakeup and sleep callback
/**
 * @note (OPTION) This function is called before the system goes to sleep
 * @param none
 * @return none
 */
VIRTUAL_FUNC void dev_before_sleep_callback(void);

/**
 * @note (OPTION) This function is called when the system wakes up
 * @param none
 * @return none
 */
VIRTUAL_FUNC void dev_after_wakeup_callback(void);

/**
 * @note (OPTION) This function is used when the system wakes up for too long
 * @param none
 * @return {bool_t}\n
 * TRUE: The system will restart as an exception.
 * FALSE The system will not restart.The application layer knows what's going on.
 */
VIRTUAL_FUNC bool_t dev_unusual_wakeup_timeout_callback(void);

/**
 * @note (OPTION) When this function is called, the system goes to sleep.
 * @param[in] {t} How many milliseconds of sleep.
 * @return none
 */
VIRTUAL_FUNC void dev_sleep_before_callback(uint32_t t);

/**
 * @note (OPTION) The system calls this function from sleep to wake
 * @param[in] {t} How many milliseconds of sleep.
 * @return none
 */
VIRTUAL_FUNC void dev_wake_up_callback(uint32_t t);

/**
 * @note (OPTION) When the serial wake function is used, the function is called after the serial wake.
 * @param[in] {pin} GPIO pin.
 * @return none
 */
VIRTUAL_FUNC void dev_uart_gpio_wake_up_callback(GPIO_PIN_T pin);


///@name The ZLL server side implements these functions
/**
 * @note (OPTION) This function is called when the ZLL server side receives the ZLL client's "identify" command
 * @param[in] {duration} Duration of the identify command \n
 *          0x0000 Exit identify mode \n
 *          0x0001 - 0xfffe Number of seconds to remain in identify mode \n
 *          0xffff Remain in identify mode for a default time known by the receiver
 * @return none
 */
VIRTUAL_FUNC void zll_server_identify_callback(uint16_t duration);

/**
 * @note (OPTION) This function is called when the ZLL server side receives 
 * the ZLL client's "reset to factory new" command
 * @param none
 * @return none
 */
VIRTUAL_FUNC void zll_reset_to_factory_new_callback(void);

/**
 * @note (OPTION) This function is called when the ZLL server side "touchlink" succeeds
 * @param[in] {pan_id} Zigbee PAN ID
 * @return none
 */
VIRTUAL_FUNC void zll_Server_touchlink_joined(uint16_t pan_id );

/**
 * @note (OPTION) This function is called when the ZLL server "leave" succeeds
 * @param none
 * @return none
 */
VIRTUAL_FUNC void zll_Server_touchlink_leave(void);


///@name The ZLL client side implements these functions
/**
 * @note (OPTION) This function is called when the ZLL client "touchlink" succeeds
 * @param[in] {zll_dev_info} When the touchlink succeeds, the device information is found
 * Refer to zll_device_info_t for detail.
 * @return none
 */
VIRTUAL_FUNC void zll_joined_device_info(zll_device_info_t zll_dev_info);

/**
 * @note (OPTION) This function is similar to zll_joined_device_info. Debugging using
 * @param[in] {zll_dev_info} When the touchlink succeeds, the device information is found
 * Refer to zll_device_info_t for detail.
 * @return none
 */
VIRTUAL_FUNC void zll_joined_device_info_for_debug(zll_device_info_t zll_dev_info);

/**
 * @note (OPTION) This function is called when the "touchlink" fails
 * @param none
 * @return none
 */
VIRTUAL_FUNC void zll_touch_link_failed(void);


#ifdef __cplusplus
}
#endif

#endif

