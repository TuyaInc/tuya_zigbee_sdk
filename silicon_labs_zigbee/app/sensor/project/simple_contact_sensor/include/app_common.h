/*************************************************************************************/
/* Automatically-generated file. Do not edit! */
/*************************************************************************************/
#ifndef __APP_COMMON_H__
#define __APP_COMMON_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "tuya_zigbee_modules.h"
#include "tuya_zigbee_sdk.h"

#ifdef __cplusplus
extern "C" {
#endif

// #define TUYA_DEBUG
#ifdef TUYA_DEBUG
#define tuya_print(...) uart_printf(UART_ID_UART0, __VA_ARGS__)
#else
#define tuya_print(...)
#endif

#define LED_POWER_UP_SYN_EVT_ID             DEV_EVT_1
#define LED_ALARM_FLASH_EVT_ID              DEV_EVT_2
#define NWK_UP_EVT_ID                       DEV_EVT_3
#define NWK_START_ASSOCIATE_EVT_ID          DEV_EVT_4

#define GPIO_ALARM1_DEBOUNCE_EVT_ID         DEV_EVT_16
#define GPIO_TAMPER_DEBOUNCE_EVT_ID         DEV_EVT_15
#define QUICKEN_ARRIVE_LOSE_STATUS_EVT_ID   DEV_EVT_13

typedef void (*func_type)(GPIO_PORT_T, GPIO_PIN_T);
typedef struct {
    DEV_IO_ST_T init_status;
    DEV_IO_ST_T new_status;
    DEV_IO_ST_T last_status;
    DEV_IO_ST_T gpio_polarity;
    gpio_config_t sensor_io_st;
    func_type sensor_func;
} sensor_t;

extern sensor_t alarm_info;

void per_dev_init(void);
void zg_dev_init(void);
void power_sampler_init(void);

#ifdef __cplusplus
}
#endif

#endif

