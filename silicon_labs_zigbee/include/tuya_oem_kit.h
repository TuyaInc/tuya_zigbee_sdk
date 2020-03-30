/**
 * @author Leon
 * @par email:
 *      zhangpeng@tuya.com
 * @file tuya_oem_kit.h
 * @brief This file defines the functions associated with the TUYA automation framework
 * @copyright HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @par company
 *      http://www.tuya.com
 * @date 2020-03-20 17:29:33
 */

#ifndef __TUYA_OEM_KIT_H__
#define __TUYA_OEM_KIT_H__

#include "type_def.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @enum TUYA value type
 */
typedef enum {
    VALUE_TYPE_STRING,         ///< ex:char a[3] = {'n','b','\0'};
    VALUE_TYPE_CHAR,           ///< ex:char a = 'b';
    VALUE_TYPE_NUMBER_DECIMAL, ///< ex:u32 a = 100000;
    VALUE_TYPE_NUMBER_HEX,     ///< ex:u8 a[3] = {2,0xDE,0xED}; 2=>hex sums
}OEM_VALUE_TYPE;

typedef struct {
    char* key;              ///< user "json" key
    void* value;            ///< user value buffer
    uint8_t value_size;     ///< user value buffer length
    OEM_VALUE_TYPE type;    ///< user value type
    bool_t handle_flag;     ///< TRUE: got a valid value.
}oem_key_value_t;


/**
 * @note Load device information
 * @param[out] {len} len Data length
 * @param[out] {data} data Data to be retrieved.
 * @return {bool_t} \n
 * TRUE: success \n
 * FALSE: fail
 */
extern bool_t oem_config_load(uint16_t *len, uint8_t **data);

/**
 * @note setting oem-config-data, 
 * @param[in] {data} oem config data
 * @param[in] {data_len} oem config data length
 * @return {bool_t} \n
 * TRUE: success \n
 * FALSE: fail
 */
extern bool_t oem_config_set(uint8_t *data, uint16_t data_len);

/**
 * @note Parse key-value data, ex: {key1:value1,key2:value2,} 
 * @param[in] {kv_str} key-value strings.
 * @param[in] {int_out_table} The arguments before parsing
 * @param[out] {int_out_table} The parsed data is stored in "int_out_table->value"
 * @param[in] {table_sums} user table sums
 * @return {bool_t} \n
 * TRUE: success \n
 * FALSE: fail
 */
extern bool_t get_key_values(char *kv_str, oem_key_value_t *int_out_table, uint16_t table_sums);

/**
 * @note load oem config and parse key-value data, ex: {key1:value1,key2:value2,} 
 * @param[in] {int_out_table} The arguments before parsing
 * @param[out] {int_out_table} The parsed data is stored in "int_out_table->value"
 * @param[in] {table_sums} user table sums
 * @return {bool_t} \n
 * TRUE: success \n
 * FALSE: fail
 */
extern bool_t get_oem_key_values(oem_key_value_t *int_out_table, uint16_t table_sums);

/**
 * @note get the gpio port and pin according the index
 * @param[in] {index} gpio index
 * @return gpio port and pin 
 */
extern GPIO_PORT_PIN_T* oem_gpio_get(uint8_t index);


#ifdef __cplusplus
}
#endif

#endif


