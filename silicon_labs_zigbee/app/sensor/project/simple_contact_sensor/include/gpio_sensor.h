/*************************************************************************************/
/* SENSOR GPIO CONFIG! */
/*************************************************************************************/
#ifndef __GPIO_SENSOR_H__
#define __GPIO_SENSOR_H__

#include "tuya_zigbee_sdk.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef enum {
    GPIO_SENSOR_INACTIVE = 0x00,
    GPIO_SENSOR_ACTIVE   = 0x01,
} gpiostate;

void sensor_basic_alarm_init_io_status_updata(void);
void sensor_tamper_alarm_init_io_status_updata(void);

#ifdef __cplusplus
}
#endif
#endif