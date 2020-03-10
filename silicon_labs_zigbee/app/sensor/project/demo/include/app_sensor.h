/*************************************************************************************/
/* APPLICATION SENSOR! */
/*************************************************************************************/
#ifndef __APP_SENSOR_H__
#define __APP_SENSOR_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "zigbee_sdk.h"

// Status bit definitions used when generating report to IAS Zone Server
#define IAS_ZONE_SENSOR_STATUS_ALARM1       0x0001
#define IAS_ZONE_SENSOR_STATUS_NO_ALARM1    0x0000
#define IAS_ZONE_SENSOR_STATUS_ALARM2       0x0002
#define IAS_ZONE_SENSOR_STATUS_NO_ALARM2    0x0000
#define IAS_ZONE_SENSOR_STATUS_TAMPER       0x0004
#define IAS_ZONE_SENSOR_STATUS_NO_TAMPER    0x0000
#define IAS_ZONE_SENSOR_STATUS_TROUBLE      0x0040
#define IAS_ZONE_SENSOR_STATUS_NO_TROUBLE   0x0000

void ias_zone_status_init(void);
void ias_zone_status_update(void);
void ias_sensor_status_sync(void);
void ias_zone_gpio_sensor_status_change_callback(uint8_t bits,uint8_t gpstatus);


#ifdef __cplusplus
}
#endif
#endif