/*************************************************************************************/
/* GLOBAL HEADER FILE! */
/*************************************************************************************/
#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "zigbee_sdk.h"
#include "config.h"


// #define TUYA_DEBUG
#ifdef TUYA_DEBUG
#define tuya_print(...) uart_printf(UART_ID_UART0, __VA_ARGS__)
#else
#define tuya_print(...)
#endif

#define DEV_EP_SUM		                    0x01
#define IAS_ZONE_EP                         0x01
#define MANUFACTURER_CODE                   0x1002


#define IAS_ZONE_TYPE                       0x0015
#define SENSOR_NUM                          0x02
#define IAS_ALARM1_ENABLE                   TRUE
#define ALARM1_VIBRATE_TIME                 200
#define ALARM1_GPIO_POLARITY                1
#define IAS_TAMPER_ENABLE                   TRUE
#define TAMPER_VIBRATE_TIME                 200
#define TAMPER_GPIO_POLARITY                1


#define LED0_ID                             0
#define KEY0_ID                             0
#define NWK_INDP_LED                        LED0_ID
#define IAS_ZONE_LED                        LED0_ID

#define NWK_HEART_BEAT_TIME_MS              (4*60*60*1000)
#define KEY_LONG_PRESS_TIME_MS              (5*1000)
#define NWK_JOIN_LED_BLINK_FREQ             200
#define NWK_UP_LED_ON_TIME_MS               (3*1000)
#define PWR_UP_DELAY_SYNC_STATUS_TIME_MS    (3*1000)
#define NWK_JOIN_TIME_OUT_MS                (30*1000)
#define POWER_UP_LED_ON_TIME                (5*1000)


// led mode enum
typedef enum {
    LED_OFF_MODE =0,
    LED_BLINK_MODE,
    LED_HOLD_ON_MODE
}led_mode_eu;

// power source type enum
typedef enum {
    battery_source = 3,
    DC_source = 4
}power_type;


/* io list config! */
#define LEDS_IO_LIST \
    {LED0_PORT, LED0_PIN, LED0_MODE, LED0_OUT, LED0_DRIVER, }, \

/* io list config! */
#define KEYS_IO_LIST \
    {KEY0_PORT, KEY0_PIN, KEY0_MODE, KEY0_OUT, KEY0_DRIVER, }, \

/* io list config! */
#define SENSORS_IO_LIST \
    {ALARM1_PORT, ALARM1_PIN, ALARM1_MODE, ALARM1_OUT, ALARM1_DRIVER, }, \
    {TAMPER_PORT, TAMPER_PIN, TAMPER_MODE, TAMPER_OUT, TAMPER_DRIVER, }, \

#ifdef __cplusplus
}
#endif
#endif
