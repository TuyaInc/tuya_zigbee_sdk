/*
 * @Author: Leon
 * @email: zhangpeng@tuya.com
 * @LastEditors: Leon
 * @file name: mf_test_callback.h
 * @Description: manufactury test callback files
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-03-25 21:41:17
 * @LastEditTime: 2019-04-15 21:32:52
 */

#ifndef MF_TEST_CALLBACK__H
#define MF_TEST_CALLBACK__H

#ifdef __cplusplus
extern "C"
{
#endif

#include "type_def.h"

//device manufactury test resut status
typedef enum
{
    MF_TEST_SUCCESS,
    MF_TEST_FAILED,
    MF_TEST_DOING
} MF_TEST_RET_T;

typedef enum {
    MF_TEST_SRC_UART = 0,
    MF_TEST_SRC_ZIGBEE,
    MF_TEST_SRC_BEACON,
}MF_TEST_SRC_T;

/**
* @description: device manufactury test result notify function
* @param {result} none
* @return: MF_TEST_RET_T manufactury test resut status
*/
extern void dev_mf_result_notify(MF_TEST_RET_T result);

/**
* @description: device manufactury test(button test) notify 
* @param {key_id} manufactury test key id 
* @return: none
*/
extern void dev_mf_button_test_notify(uint32_t key_id);


/**
* @description: device manufactury test(bool sensor test) notify 
* @param {sensor_type} sensor type
* @param {index} sensor index 
* @param {result} OK:TRUE  ERR:FALSE 
* @return: none
*/
extern void dev_mf_bool_sensor_notify(uint8_t sensor_type, uint8_t index, bool_t result);

/**
* @description: general send function
* @param {cmd} test commander
* @param {len} data's length
* @param {data} data 
* @return: none
*/
extern void dev_mf_general_send(uint8_t cmd, uint8_t len, uint8_t *data);


extern bool_t dev_mf_test_is_doing(MF_TEST_SRC_T *src);

#ifdef __cplusplus
}
#endif

#endif
