/*************************************************************************************/
/* Automatically-generated file. Do not edit! */
/*************************************************************************************/
#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "tuya_zigbee_sdk.h"
/* firmware information! */
#define FIRMWARE_NAME                   "simple_contact_sensor"
#define FIRMWARE_VER                    0x40
#define PRODUCTOR_ID_PFEFIX             "_TZ3000_"
#define IC                              "EFR32MG13P732F512"
#define MANUFACTURER_CODE               0x1002
#define OTA_IMAGE_TYPE                  0x1602
#define MODEL_ID                        "TS0203"
#define PRODUCTOR_ID                    "alo8ncai"
#define MODULE_NAME                     TYZS3


/* uart config! */
#define UART_ENABLE                     true
#define UART_NUM                        0x1

#define UART0_RX_PORT                   PORT_A
#define UART0_RX_PIN                    PIN_1
#define UART0_RX_LOC                    LOC_0
#define UART0_TX_PORT                   PORT_A
#define UART0_TX_PIN                    PIN_0
#define UART0_TX_LOC                    LOC_0
#define UART0_BANDRATE                  115200



/* io config! */
/* led config! */
#define LED_NUM                         2
#define LED0_PORT                       PORT_A
#define LED0_PIN                        PIN_2
#define LED0_MODE                       GPIO_MODE_OUTPUT_PP
#define LED0_OUT                        GPIO_DOUT_HIGH
#define LED0_DRIVER                     GPIO_DOUT_LOW

#define LED1_PORT                       PORT_A
#define LED1_PIN                        PIN_3
#define LED1_MODE                       GPIO_MODE_OUTPUT_PP
#define LED1_OUT                        GPIO_DOUT_HIGH
#define LED1_DRIVER                     GPIO_DOUT_LOW



/* io config! */
/* key config! */
#define KEY_NUM                         1
#define KEY0_PORT                       PORT_D
#define KEY0_PIN                        PIN_15
#define KEY0_MODE                       GPIO_MODE_INPUT_PULL
#define KEY0_OUT                        GPIO_DOUT_HIGH
#define KEY0_DRIVER                     GPIO_DOUT_LOW



/* io config! */
/* sensor config! */
#define SENSOR_NUM                      1
#define ALARM1_PORT                     PORT_F
#define ALARM1_PIN                      PIN_4
#define ALARM1_MODE                     GPIO_MODE_INPUT_HIGH_IMPEDANCE
#define ALARM1_OUT                      GPIO_DOUT_LOW
#define ALARM1_DRIVER                   GPIO_LEVEL_ALL


/* io list config! */
#define LEDS_IO_LIST \
    {LED0_PORT, LED0_PIN, LED0_MODE, LED0_OUT, LED0_DRIVER, }, \
    {LED1_PORT, LED1_PIN, LED1_MODE, LED1_OUT, LED1_DRIVER, }, \

/* io list config! */
#define KEYS_IO_LIST \
    {KEY0_PORT, KEY0_PIN, KEY0_MODE, KEY0_OUT, KEY0_DRIVER, }, \

/* io list config! */
#define SENSORS_IO_LIST \
    {ALARM1_PORT, ALARM1_PIN, ALARM1_MODE, ALARM1_OUT, ALARM1_DRIVER, }, \

/* epinfo parameter config! */
#define DEV_EP_SUM                      1
#define IAS_ZONE_EP                     1

/* ledinfo parameter config! */
#define POWER_UP_LED_ON_TIME            3000
#define NWK_JOIN_LED_BLINK_FREQ         250

/* join parameter config! */
#define RESET_TIME_MS                   3000
#define CHANNEL_SW_PER_MS               300
#define JOINED_CONTINUE_POLL_TIME_MS    25000
#define POWER_ON_JOIN                   0
#define RM_LEAVE_JOIN                   1
#define JOIN_TIME_OUT_MS                30000

/* rejoin parameter config! */
#define POWER_ON_REJOIN                 1
#define AUTO_REJOIN_POLL                1
#define REJOINED_CONTINUE_POLL_TIME_MS  15000
#define BEACON_PER_MS                   1000
#define BEACON_TIME                     4
#define NEXT_REJOIN_PER_HOUR            4

/* poll parameter config! */
#define POLL_FOREVER                    0
#define POLL_INTERVAL_MS                800
#define WAIT_APP_ACK_MS                 2000
#define POLL_MISS_MAX                   6

/* zoneinfo parameter config! */
#define IAS_ZONE_TYPE                   21

/* alarm1info parameter config! */
#define ALARM1_LED_FLASH_TIME           200
#define ALARM1_GPIO_POLARITY            0
#define ALARM1_VIBRATE_TIME             200
#define ALARM1_REPORT_TIME              0

/* battery_param parameter config! */
#define BATTERY_REPORT_MINUTE           240
#define BATTERY_MAX_MILLIV              3000
#define BATTERY_MIN_MILLIV              2500

#endif
