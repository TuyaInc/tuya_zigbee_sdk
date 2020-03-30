/*************************************************************************************/
/* Automatically-generated file. Do not edit! */
/*************************************************************************************/
#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "tuya_zigbee_sdk.h"
/* firmware information! */
#define FIRMWARE_INFO                   "sample_smart_plug_1_1.0.0"
#define FIRMWARE_NAME                   "sample_smart_plug_1"
#define FIRMWARE_VER                    0x40
#define PRODUCTOR_ID_PFEFIX             "_TZ3000_"
#define IC                              "EFR32MG13P732F512"
#define OTA_IMAGE_TYPE                  0x1602
#define MODULE_NAME                     TYZS3
#define MODEL_ID                        "TS0101"
#define PRODUCTOR_ID                    "8wcwaxky"


/* io config! */
/* led config! */
#define LED0_PORT                       PORT_F
#define LED0_PIN                        PIN_2
#define LED0_MODE                       GPIO_MODE_OUTPUT_PP
#define LED0_OUT                        GPIO_DOUT_HIGH
#define LED0_DRIVER                     GPIO_LEVEL_LOW

#define LED1_PORT                       PORT_F
#define LED1_PIN                        PIN_4
#define LED1_MODE                       GPIO_MODE_OUTPUT_PP
#define LED1_OUT                        GPIO_DOUT_HIGH
#define LED1_DRIVER                     GPIO_LEVEL_LOW

#define LED2_PORT                       PORT_F
#define LED2_PIN                        PIN_5
#define LED2_MODE                       GPIO_MODE_OUTPUT_PP
#define LED2_OUT                        GPIO_DOUT_LOW
#define LED2_DRIVER                     GPIO_LEVEL_HIGH



/* io config! */
/* key config! */
#define KEY0_PORT                       PORT_A
#define KEY0_PIN                        PIN_3
#define KEY0_MODE                       GPIO_MODE_INPUT_PULL
#define KEY0_OUT                        GPIO_DOUT_HIGH
#define KEY0_DRIVER                     GPIO_LEVEL_LOW


#endif
