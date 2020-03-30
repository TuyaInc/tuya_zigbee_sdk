/**
 * @author Leon
 * @par email:
 *      zhangpeng@tuya.com
 * @file hal_uart.h
 * @brief This file abstracts all the operations of UART
 * @copyright HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @par company
 *      http://www.tuya.com
 * @date 2020-03-20 17:29:33
 */

#ifndef __HAL_UART_H__
#define __HAL_UART_H__

#include "type_def.h"
#include "hal_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @enum UART id
 */
typedef enum {
    UART_ID_UART0 = 0,  /**< UART0 */
    UART_ID_UART1,      /**< UART1 */
}UART_ID_T;

/**
 * @enum UART parity type
 */
typedef enum {
    USART_PARITY_NONE = 0,    /**< No parity. */
    USART_PARITY_EVEN,      /**< Even parity. */
    USART_PARITY_ODD,      /**< Odd parity. */
} USART_PARITY_T;

/**
 * @enum UART stop-bits 
 */
typedef enum {
    USART_STOPBITS_HALF = 0,        /**< 0.5 stop bits. */
    USART_STOPBITS_ONE,         /**< 1 stop bits. */
    USART_STOPBITS_ONEANDAHALF, /**< 1.5 stop bits. */
    USART_STOPBITS_TWO          /**< 2 stop bits. */
} USART_STOPBITS_T;

/**
 * @enum UART data-bits 
 */
typedef enum {
    USART_DATABITS_8BIT = 0,        /**< 8 bits databits. */
    USART_DATABITS_9BIT,         /**< 9 bits databits. */
} USART_DATABITS_T;

/**
 * @enum UART GPIO select
 */
typedef enum {
    UART_PIN_TYPE_DEFAULT = 0, ///<default uart is PA0,PA1
    UART_PIN_TYPE_CONFIG       ///<user config uart port and pin 
}UART_PIN_TYPE_T;


/**
 * @note This is a callback function style for uart rx.
 */
typedef void(*uart_callback)(uint8_t *data, uint16_t len);

/**
 * @enum UART base configuration
 */
typedef struct {
    UART_ID_T uart_id; 
    UART_PIN_TYPE_T pin_type; 
    GPIO_PORT_PIN_T tx;
    GPIO_PORT_PIN_T rx;
    GPIO_LOC_T tx_loc;
    GPIO_LOC_T rx_loc;
    uint32_t baud_rate;
    USART_PARITY_T parity;
    USART_STOPBITS_T stop_bits;
    USART_DATABITS_T data_bits;
    uart_callback func;
}user_uart_config_t;

/**
 * @remarks Defines the default configuration for the serial port
 */
#ifdef EFR32MG13P
#define USART_CONFIG_DEFAULT {\
    UART_ID_UART0,\
    UART_PIN_TYPE_CONFIG,\
    {PORT_A, PIN_0},\
    {PORT_A, PIN_1},\
    LOC_0,\
    LOC_0,\
    115200,\
    USART_PARITY_NONE,\
    USART_STOPBITS_ONE,\
    USART_DATABITS_8BIT,\
    NULL\
}
#else
#define USART_CONFIG_DEFAULT {\
    UART_ID_UART0,\
    UART_PIN_TYPE_CONFIG,\
    {PORT_A, PIN_5},\
    {PORT_A, PIN_6},\
    LOC_0,\
    LOC_0,\
    115200,\
    USART_PARITY_NONE,\
    USART_STOPBITS_ONE,\
    USART_DATABITS_8BIT,\
    NULL\
}
#endif


/**
 * @note user uart init function
 * @param[in] {config} user uart configuration information
 * @return none
 */
extern void user_uart_init(user_uart_config_t *config);

/**
 * @note user uart data send funciton
 * @param[in] {uart_id} UART_ID_UART0 or UART_ID_UART1
 * @param[in] {data} send data
 * @param[in] {data_len} send data length
 * @return none
 */
extern void user_uart_send(UART_ID_T uart_id, uint8_t* data, uint16_t data_len);

/**
 * @note user uart function disable function
 * @param[in] {uart_id} UART_ID_UART0 or UART_ID_UART1
 * @return none
 */
extern void user_uart_disable(UART_ID_T uart_id);

/**
 * @note uart log print function
 * @param[in] {uart_id} UART_ID_UART0 or UART_ID_UART1
 * @param[in] {formatString} print data list
 * @return none
 */
extern void uart_printf(UART_ID_T uart_id, const char *formatString, ...);

/**
 * @note Use this function if you need to wake up the MCU with RX pin in the serial port.
 * Used before serial port initialization
 * @param[in] {flag} \n
 * TRUE: use RX pin to wake up MCU. \n
 * FALSE: no need
 * @return none
 */
extern void set_uart_rx_wakeup_flag(bool_t flag);



#ifdef __cplusplus
}
#endif

#endif




