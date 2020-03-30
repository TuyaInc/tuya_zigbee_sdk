/**
 * @author Leon
 * @par email:
 *      zhangpeng@tuya.com
 * @file tuya_zigbee_modules.h
 * @brief This file defines TUYA module-related functions
 * @copyright HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @par company
 *      http://www.tuya.com
 * @date 2020-03-20 17:29:33
 */


#ifndef __TUYA_ZIGBEE_MODULES_H__
#define __TUYA_ZIGBEE_MODULES_H__

#include "hal_uart.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TYZS1  0    ///< chip: EFR32MG13P732F512
#define TYZS1L 1    ///< chip: EFR32MG13P732F512
#define TYZS2  2    ///< chip: EFR32MG13P732F512
#define TYZS2R 3    ///< chip: EFR32MG13P732F512
#define TYZS3  4    ///< chip: EFR32MG13P732F512
#define TYZS4  5    ///< chip: EFR32MG13P732F512
#define TYZS5  6    ///< chip: EFR32MG13P732F512
#define TYZS5L 7    ///< chip: EFR32MG13P732F512
#define TYZS6  8    ///< chip: EFR32MG13P732F512
#define TYZS7  9    ///< chip: EFR32MG13P732F512
#define TYZS8  10   ///< chip: EFR32MG13P732F512
#define TYZS9V 11   ///< chip: EFR32MG13P732F512
#define TYZS10 12   ///< chip: EFR32MG13P732F512
#define TYZS11 13   ///< chip: EFR32MG13P732F512
#define TYZS12 14   ///< chip: EFR32MG13P732F512
#define TYZS13 15   ///< chip: EFR32MG13P732F512
#define TYZS15 16   ///< chip: EFR32MG13P732F512
#define ZS3L   17   ///< chip: EFR32MG21A020F768


/**
 * @remarks TYZS3 uart define
 */
#define TYZS3_USART_CONFIG_DEFAULT {\
    UART_ID_UART0,\
    UART_PIN_TYPE_CONFIG,\
    {PORT_A, PIN_0},\
    {PORT_A, PIN_1},\
    LOC_0, \
    LOC_0, \
    115200,\
    USART_PARITY_NONE,\
    USART_STOPBITS_ONE,\
    USART_DATABITS_8BIT,\
    NULL\
}

/**
 * @remarks TYZS5 uart define
 */
#define TYZS5_USART_CONFIG_DEFAULT {\
    UART_ID_UART0,\
    UART_PIN_TYPE_CONFIG,\
    {PORT_F, PIN_5},\
    {PORT_F, PIN_2},\
    LOC_29, \
    LOC_25, \
    115200,\
    USART_PARITY_NONE,\
    USART_STOPBITS_ONE,\
    USART_DATABITS_8BIT,\
    NULL\
}

/**
 * @remarks TYZS2R uart define
 */
#define TYZS2R_USART_CONFIG_DEFAULT {\
    UART_ID_UART0,\
    UART_PIN_TYPE_CONFIG,\
    {PORT_A, PIN_3},\
    {PORT_A, PIN_4},\
    LOC_3, \
    LOC_3, \
    115200,\
    USART_PARITY_NONE,\
    USART_STOPBITS_ONE,\
    USART_DATABITS_8BIT,\
    NULL\
}

/**
 * @remarks TYZS2 uart define
 */
#define TYZS2_USART_CONFIG_DEFAULT {\
    UART_ID_UART0,\
    UART_PIN_TYPE_CONFIG,\
    {PORT_F, PIN_5},\
    {PORT_F, PIN_2},\
    LOC_29, \
    LOC_25, \
    115200,\
    USART_PARITY_NONE,\
    USART_STOPBITS_ONE,\
    USART_DATABITS_8BIT,\
    NULL\
}

/**
 * @remarks ZS3L uart define
 */
#define ZS3L_USART_CONFIG_DEFAULT {\
    UART_ID_UART0,\
    UART_PIN_TYPE_CONFIG,\
    {PORT_A, PIN_5},\
    {PORT_A, PIN_6},\
    LOC_29, \
    LOC_25, \
    115200,\
    USART_PARITY_NONE,\
    USART_STOPBITS_ONE,\
    USART_DATABITS_8BIT,\
    NULL\
}

#ifdef __cplusplus
}
#endif

#endif //ZIGBEE_MODULES_H

