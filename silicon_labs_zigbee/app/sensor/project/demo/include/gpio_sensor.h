/*************************************************************************************/
/* SENSOR GPIO CONFIG! */
/*************************************************************************************/
#ifndef __GPIO_SENSOR_H__
#define __GPIO_SENSOR_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "zigbee_sdk.h"


#define IAD_ZONE_ALARM1                 0x00
#define IAD_ZONE_ALARM2                 0x01
#define IAD_ZONE_TAMPER                 0x02
#define IAD_ZONE_TROUBLE                0x03

typedef enum {
    GPIO_SENSOR_INACTIVE = 0x00,
    GPIO_SENSOR_ACTIVE = 0x01,
} gpiostate;

void gpio_sensor_interrupt_init(void);

#ifdef __cplusplus
}
#endif
#endif