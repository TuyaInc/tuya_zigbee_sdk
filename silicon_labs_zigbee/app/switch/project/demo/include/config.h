/*************************************************************************************/
/* Automatically-generated file. Do not edit! */
/*************************************************************************************/
#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "zigbee_sdk.h"
/* firmware information! */
#define FIRMWARE_NAME                   "demo"
#define FIRMWARE_VER                    0x40
#define PRODUCTOR_ID_PFEFIX             "_TYZB01_"
#define IC                              "efr32mg13p732gm48"
#define OTA_IMAGE_TYPE                  0x1602
#define MODULE_NAME                     TYZS3
#define MODEL_ID                        "TS0003"
#define PRODUCTOR_ID                    "vindpw7g"


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
#define LED_ENABLE                      true
#define LED_NUM                         3
#define LED0_PORT                       PORT_F
#define LED0_PIN                        PIN_1
#define LED0_MODE                       GPIO_MODE_OUTPUT_PP
#define LED0_OUT                        GPIO_DOUT_LOW
#define LED0_DRIVER                     GPIO_LEVEL_HIGH

#define LED1_PORT                       PORT_F
#define LED1_PIN                        PIN_0
#define LED1_MODE                       GPIO_MODE_OUTPUT_PP
#define LED1_OUT                        GPIO_DOUT_LOW
#define LED1_DRIVER                     GPIO_LEVEL_HIGH

#define LED2_PORT                       PORT_F
#define LED2_PIN                        PIN_5
#define LED2_MODE                       GPIO_MODE_OUTPUT_PP
#define LED2_OUT                        GPIO_DOUT_LOW
#define LED2_DRIVER                     GPIO_LEVEL_HIGH



/* io config! */
/* key config! */
#define KEY_ENABLE                      true
#define KEY_NUM                         1
#define KEY0_PORT                       PORT_A
#define KEY0_PIN                        PIN_3
#define KEY0_MODE                       GPIO_MODE_INPUT_PULL
#define KEY0_OUT                        GPIO_DOUT_LOW
#define KEY0_DRIVER                     GPIO_LEVEL_LOW


#endif
