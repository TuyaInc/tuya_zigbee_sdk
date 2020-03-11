/*************************************************************************************/
/* Automatically-generated file. Do not edit! */
/*************************************************************************************/
#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "zigbee_sdk.h"
/* firmware information! */
#define FIRMWARE_NAME                   "oem_si32_zg_Zll_light"
#define FIRMWARE_VER                    0x40
#define PRODUCTOR_ID_PFEFIX             "_TYZB01_"
#define IC                              "efr32mg13p732gm48"
#define OTA_IMAGE_TYPE                  0x1201
#define MODEL_ID                        "TS0502"
#define PRODUCTOR_ID                    "jgf2orvu"
#define MODULE_NAME                     TYZS13


/* uart config! */
#define UART_ENABLE                     true
#define UART_NUM                        0x2

#define UART0_RX_PORT                   PORT_A
#define UART0_RX_PIN                    PIN_1
#define UART0_RX_LOC                    LOC_0
#define UART0_TX_PORT                   PORT_A
#define UART0_TX_PIN                    PIN_0
#define UART0_TX_LOC                    LOC_0
#define UART0_BANDRATE                  115200

#define UART1_RX_PORT                   PORT_A
#define UART1_RX_PIN                    PIN_2
#define UART1_RX_LOC                    LOC_0
#define UART1_TX_PORT                   PORT_A
#define UART1_TX_PIN                    PIN_3
#define UART1_TX_LOC                    LOC_0
#define UART1_BANDRATE                  115200



/* io config! */
/* led config! */
#define LED0_PORT                       PORT_A
#define LED0_MODE                       GPIO_MODE_OUTPUT_PP
#define LED0_OUT                        GPIO_DOUT_LOW
#define LED0_PIN                        PIN_2
#define LED0_DRIVER                     GPIO_DOUT_HIGH

#define LED1_PIN                        PIN_4
#define LED1_OUT                        GPIO_DOUT_HIGH
#define LED1_DRIVER                     GPIO_DOUT_LOW
#define LED1_MODE                       GPIO_MODE_OUTPUT_PP
#define LED1_PORT                       PORT_F



/* io config! */
/* key config! */
#define KEY0_PORT                       PORT_F
#define KEY0_DRIVER                     GPIO_LEVEL_LOW
#define KEY0_OUT                        GPIO_DOUT_LOW
#define KEY0_MODE                       GPIO_MODE_INPUT_PULL
#define KEY0_PIN                        PIN_2


#endif
