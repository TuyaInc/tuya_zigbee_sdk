/**
 * @author Leon
 * @par email:
 *      zhangpeng@tuya.com
 * @file hal_flash.h
 * @brief This file abstracts all the operations of FLASH
 * @copyright HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @par company
 *      http://www.tuya.com
 * @date 2020-03-20 17:29:33
 */

#ifndef __HAL_FLASH_H__
#define __HAL_FLASH_H__

#include "type_def.h"

#ifdef __cplusplus
extern "C" {
#endif


#define FLASH_ADDR_START    (0x0000+250) ///< FLASH virtual address
#define FLASH_TOTAL_SIZE    4000         ///< FLASH total size
#define FLASH_BLOCK_SIZE    250          ///< FLASH virtual page size


/**
 * @enum FLASH block id
 */
typedef enum {
    FLASH_BLOCK_1 = 1,  ///< block id-1
    FLASH_BLOCK_2,      ///< block id-2
    FLASH_BLOCK_3,
    FLASH_BLOCK_4,
    FLASH_BLOCK_5,
    FLASH_BLOCK_6,
    FLASH_BLOCK_7,
    FLASH_BLOCK_8,
    FLASH_BLOCK_9,
    FLASH_BLOCK_10,
    FLASH_BLOCK_11,
    FLASH_BLOCK_12,
    FLASH_BLOCK_13,
    FLASH_BLOCK_14,
    FLASH_BLOCK_15,
    FLASH_BLOCK_16,
}FLASH_BLOCK_T;

/**
 * @note write data to flash
 * @param[in] {block_id} FLASH block id
 * @param[in] {data} data 
 * @param[in] {len} data length
 * @return Length of write data
 */
extern uint16_t flash_block_raw_write(FLASH_BLOCK_T block_id, uint8_t *data, uint16_t len);

/**
 * @note read data from flash
 * @param[in] {block_id} FLASH block id
 * @param[out] {data} a buffer  
 * @param[in] {len} data length
 * @return none
 */
extern uint16_t flash_block_raw_read(FLASH_BLOCK_T block_id, uint8_t *data, uint16_t len);


/**
 * @note write data to flash
 * @param[in] {addr} FLASH virtual address+offset
 * @param[out] {data} data 
 * @param[in] {len} data length
 * @return Length of write data
 */
extern uint16_t flash_addr_raw_write(uint16_t addr, uint8_t *data, uint16_t len);

/**
 * @note write data to flash
 * @param[in] {addr} FLASH virtual address+offset
 * @param[out] {data} data 
 * @param[in] {len} data length
 * @return Length of write data
 */
extern uint16_t flash_addr_raw_read(uint16_t addr, uint8_t *data, uint16_t len);


typedef struct {
    uint16_t flash_start_addr; ///< It's between "FLASH_ADDR_START" to "FLASH_TOTAL_SIZE-1".
    uint16_t items;
    uint16_t item_size;
}dev_record_cfg_t;

/**
 * @enum The result of record functions
 */
typedef enum {
    RECORD_RET_SUCCESS = 0, ///< success
    RECORD_RET_ERR,         ///< fail
    RECORD_RET_SPACE_ERR,   ///< space error
}RECORD_RET_T;

typedef int record_fd_t;

extern record_fd_t dev_record_init(dev_record_cfg_t *cfg);
extern RECORD_RET_T dev_record_push(record_fd_t fd, void *record, uint16_t size);
extern RECORD_RET_T dev_record_pop(record_fd_t fd, void *record, uint16_t size);
extern RECORD_RET_T dev_record_get(record_fd_t fd, void *record, uint16_t size);
extern uint16_t dev_record_get_used(record_fd_t fd);
extern uint16_t dev_record_get_capacity    (record_fd_t fd);
extern void dev_record_clear_all(record_fd_t fd); //clear all record

/**
 * @note write data to flash
 * @param[in] {data} data
 * @param[in] {len} data length
 * @return none
 */
extern uint8_t user_flash_data_write(uint8_t *data, uint8_t len);

/**
 * @note read data from flash 
 * @param[in] {data} data
 * @param[in] {len} data length
 * @return none
 */
extern uint8_t user_flash_data_read(uint8_t *data, uint8_t len);


#ifdef __cplusplus
}
#endif

#endif




