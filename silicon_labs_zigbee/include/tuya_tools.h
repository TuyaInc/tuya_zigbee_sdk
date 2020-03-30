/**
 * @author Leon
 * @par email:
 *      zhangpeng@tuya.com
 * @file tuya_tools.h
 * @brief This file defines some general functional API
 * @copyright HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @par company
 *      http://www.tuya.com
 * @date 2020-03-20 17:29:33
 */

#ifndef __TUYA_TOOLS_H__
#define __TUYA_TOOLS_H__

#include "type_def.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @enum hex string format definition
 */
typedef enum {
    LETTER_TYPE_UPPER = 0, ///< hex string is upper letter
    LETTER_TYPE_LOWER,     ///< hex string is lower letter
}LETTER_TYPE_T;

/**
 * @enum The result of JSON string parsing
 */
typedef enum {
    JSON_RET_PARSE_OK = 0, ///< JSON parse ok
    JSON_RET_ERR,          ///< JSON parse error
}JSON_RET_T;

#define get_array_len(x) (sizeof(x)/sizeof(x[0])) ///< get array number
#define GET_ARRAY_LEN(x) get_array_len(x)         ///< get array number


/**
 * @note hex to hexstr convert function
 * @param[in] {type} upper or lower letter
 * @param[in] {hex} input hex data
 * @param[in] {hex_len} input hex data length
 * @param[out] {hex_str} output covert hexstr result
 * @return none
 */
extern void hex_to_hexstr(LETTER_TYPE_T type, uint8_t *hex, uint16_t hex_len, char *hex_str);

/**
 * @note hexstr to hex convert function
 * @param[in] {hex_str} input hexstr data
 * @param[out] {hex} output covert hex result
 * @return hex data length
 */
extern uint16_t hexstr_to_hex(char *hex_str, uint8_t *hex);

/**
 * @note device firmware version get
 * @param[in] {ver} the int version 
 * @return convert result of string version
 */
extern char *get_dev_firmware_ver(uint8_t ver);

/**
 * @note data reversal function
 * @param[in] {in_data} reversal source data
 * @param[out] {out_data} reversal dest data
 * @param[in] {len} reversal data buffer length
 * @return none
 */
extern void data_reversal(uint8_t *in_data, uint8_t *out_data, uint16_t len);

/**
 * @note random a time with 0-t ms
 * @param[in] {t} random maxinum ms
 * @return none
 */
extern uint32_t random_ms(uint32_t t);

/**
 * @note caculate the crc of the date
 * @param[in] {data} data
 * @param[in] {data_len} data length
 * @return crc result
 */
extern uint16_t make_crc16(uint8_t *msg, uint16_t len);


/**
 * @note Get the string type data in the JSON string by key
 * @param[in] {json_str} bare JSON string
 * @param[in] {key} JSON key
 * @param[out] {out_buffer} The parsed value
 * @param[in] {out_buffer_len} out_buffer's length
 * @return {JSON_RET_T} JSON_RET_PARSE_OK: Parsing the success, JSON_RET_ERR: Parse failure
 * @code Usage
 *  char *json_str  = "{\"key1\":\"hello\", \"key2\":1000}";
 *  char value_str[128] = {0};
 *  int value_int;
 *  JSON_RET_T ret;

 *  ret = json_get_str_value(json_str, "key1", value_str, sizeof(value_str));
 *  if(ret == JSON_RET_PARSE_OK) {
 *      app_print("key1's value: %s\n", value_str); // value_str equ hello
 *  }
 *
 *  ret = json_get_int_value(json_str, "key2", &value_int); //value_int equ 1000
 *  if(ret == JSON_RET_PARSE_OK) {
 *      app_print("key2's value: %d\n", value_int);
 *  }
 * @endcode
 */
extern JSON_RET_T json_get_str_value(const char *json_str, char *key, char *out_buffer, uint16_t out_buffer_len);

/**
 * @note Get the integer type data in the JSON string by key
 * @param[in] {json_str} bare JSON string
 * @param[in] {key} JSON key
 * @param[out] {out_int_value} The parsed value
 * @return {JSON_RET_T} JSON_RET_PARSE_OK: Parsing the success, JSON_RET_ERR: Parse failure
 */
extern JSON_RET_T json_get_int_value(const char *in_json_str, char *key, int *out_int_value);


#ifdef __cplusplus
}
#endif

#endif


