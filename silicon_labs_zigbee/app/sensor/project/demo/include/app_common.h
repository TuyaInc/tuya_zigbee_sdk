/*************************************************************************************/
/* Automatically-generated file. Do not edit! */
/*************************************************************************************/
#ifndef __APP_COMMON_H__
#define __APP_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif


#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "zigbee_modules.h"
#include "zigbee_sdk.h"

void app_relays_on_op(uint8_t index,DEV_IO_ST_T st);
void app_relays_off_op(uint8_t index,DEV_IO_ST_T st);
void per_dev_init(void);
void zg_dev_init(void);
void ias_zone_sensor_init(void);

#ifdef __cplusplus
}
#endif
#endif