/*
 * @Author: jin lu
 * @email: jinlu@tuya.com
 * @Date: 2019-05-23 16:56:09
 * @file name: 
 * @Description: 
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 */

#ifndef SOC_GPIO__H
#define SOC_GPIO__H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include "zigbee_sdk.h"
#include "light_types.h"

typedef struct{
    uint8_t num;
    GPIO_PORT_T port;
    GPIO_PIN_T pin;
}num2pin_t;

typedef struct {
    char*   string;
    uint8_t module;
}module_table_t;

typedef enum {
    BUTTON_STATE_UP = 0,
    BUTTON_STATE_PUSH,
} button_state_t;

#define MAX_GPIO_PIN_NUM    40


/************************* platform specific interface ****************/
GPIO_PORT_T xSocGpioGetPort(UCHAR_T pin_num);
GPIO_PIN_T xSocGpioGetPin(UCHAR_T pin_num);
/************************* platform specific interface*****************/


/************************* common interface **************************/
BOOL_T bSocGpioModuleInit(PCHAR_T name, UCHAR_T len);

VOID vSocGpioOutPutInit(UCHAR_T pin_num, UCHAR_T init_value);
UCHAR_T vSocGpioOutPutRead(UCHAR_T pin_num);
VOID vSocGpioOutPutWrite(UCHAR_T pin_num, UCHAR_T value);

VOID vSocGpioInPutInit(UCHAR_T pin_num, UCHAR_T pull_value);
UCHAR_T vSocGpioInPutRead(UCHAR_T pin_num);
/************************* common interface end **********************/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif


