/**
 * @author Leon
 * @par email:
 *      zhangpeng@tuya.com
 * @file tuya_mf_test.h
 * @brief This file defines production-related functions
 * @copyright HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @par company
 *      http://www.tuya.com
 * @date 2020-03-20 17:29:33
 */


#ifndef __TUYA_MF_TEST_H__
#define __TUYA_MF_TEST_H__

#include "type_def.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @enum test command
 */
typedef enum {
    MF_TEST_LED_ON_ALL,         ///< Switch on all LED
    MF_TEST_LED_OFF_ALL,        ///< Switch off all LED
    MF_TEST_LED_BLINK_ALL,      ///< Blink all LED
    MF_TEST_RELAY_ON_ALL,       ///< Switch on all relay
    MF_TEST_RELAY_OFF_ALL,      ///< Switch off all relay
    MF_TEST_RELAY_BLINK_ALL,    ///< Blink all relay
    MF_TEST_BUTTON,             ///< Button testing
    MF_TEST_GET_ELEC_INFO,      ///< Get electricity information
    MF_TEST_ADJUST_ELEC_INFO,   ///< Calibration quantity information
    MF_TRANSFER,                ///< transfer rare test data
}MF_TEST_CMD_T;

/**
 * @enum testing result, dev_mf_test_callback using
 */
typedef enum {
    MF_TEST_SUCCESS,        ///< Test success
    MF_TEST_FAILED,         ///< Test failure
    MF_TEST_DOING           ///< Testing is underway
} MF_TEST_RET_T;

/**
 * @enum Test data source
 */
typedef enum {
    MF_TEST_SRC_UART = 0, ///< from uart
    MF_TEST_SRC_ZIGBEE,   ///< from zigbee
    MF_TEST_SRC_BEACON,   ///< from beacon
}MF_TEST_SRC_T;


typedef void (*rf_recv_fun_t)(uint8_t *packet, uint8_t lqi, int8_t rssi); ///< rf test callback

typedef struct {
    uint8_t channel;
    uint8_t data_len;
    uint8_t data[50];
    rf_recv_fun_t received_callback; ///< rf test callback
}serial_protocol_rf_test_t;

/**
* @note device manufactury test result notify function
* @param[in] {result} none
* @return none
*/
extern void dev_mf_result_notify(MF_TEST_RET_T result);

/**
* @note device manufactury test(button test) notify 
* @param[in] {key_id} manufactury test key id 
* @return none
*/
extern void dev_mf_button_test_notify(uint32_t key_id);


/**
* @note device manufactury test(bool sensor test) notify 
* @param[in] {sensor_type} sensor type
* @param[in] {index} sensor index 
* @param[in] {result} OK:TRUE  ERR:FALSE 
* @return none
*/
extern void dev_mf_bool_sensor_notify(uint8_t sensor_type, uint8_t index, bool_t result);

/**
* @note general send function
* @param[in] {cmd} test commander
* @param[in] {len} data's length
* @param[in] {data} data 
* @return none
*/
extern void dev_mf_general_send(uint8_t cmd, uint8_t len, uint8_t *data);

/**
 * @note disable beacon test
 * @param none
 * @return none
 */
extern void mf_test_disable_beacon_test(void);

/**
 * @note send data from mac layer immediately
 * @param[in] {rf_test} RF test data
 * @return none
 */
extern void dev_sys_data_send_mac(serial_protocol_rf_test_t* rf_test);

/**
 * @note get system authkey for security.
 * @param[out] {buf} a buffer store authkey
 * @param[in] {buf_len} buffer length
 * @return authkey length. 0 means error.
 */
extern uint8_t dev_get_auth_key(uint8_t *buf, uint8_t buf_len);

#ifdef __cplusplus
}
#endif

#endif

