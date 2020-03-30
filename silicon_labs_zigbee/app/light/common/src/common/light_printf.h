#ifndef __LIHGT_PRINT_H__
#define __LIHGT_PRINT_H__

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include "light_types.h"
#include "zigbee_sdk.h"

#define PRINT_LEVEL_ERR    1
#define PRINT_LEVEL_DEBUG  0
#define PRINT_LEVEL_NOTICE 0
#if (PRINT_LEVEL_ERR)
#define PR_ERR(...) uart_printf(UART_ID_UART0, __VA_ARGS__)
#else
#define PR_ERR(...)
#endif

#if (PRINT_LEVEL_DEBUG)
#define PR_DEBUG(...) uart_printf(UART_ID_UART0, __VA_ARGS__)
#else
#define PR_DEBUG(...)
#endif
  
#if (PRINT_LEVEL_NOTICE)
#define PR_NOTICE(...) uart_printf(UART_ID_UART0, __VA_ARGS__)
#else
#define PR_NOTICE(...)
#endif










#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __LIHGT_PRINT_H__*/ 

