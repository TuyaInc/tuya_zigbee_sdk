/*
 * @Author: jin lu
 * @email: jinlu@tuya.com
 * @Date: 2019-05-23 11:24:33
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

#include "tuya_zigbee_sdk.h"
#include "light_types.h"
#include "light_printf.h"

typedef struct{
    uint8_t num;
    GPIO_PORT_T port;
    GPIO_PIN_T pin;
}num2pin_t;

typedef struct {
    char*   string;
    uint8_t module;
}module_table_t;

#define PIN_NUM_ERROR       0xFF
#define MAX_GPIO_PIN_NUM    40


/************************* platform specific interface ****************/
GPIO_PORT_T xSocGpioGetPort(UCHAR_T pin_num);
GPIO_PIN_T xSocGpioGetPin(UCHAR_T pin_num);
UCHAR_T get_module(void);
/************************* platform specific interface*****************/


/************************* common interface **************************/
BOOL_T IsOldJsonVersion(VOID);//add by jin
OPERATE_RET  xSocGpioArrayVersionSet(UCHAR_T version);//add by jin
OPERATE_RET opSocGpioModuleInit(PCHAR_T name, UCHAR_T len);
OPERATE_RET opSocGpioOutPutInit(UCHAR_T pin_num, UCHAR_T init_value);
OPERATE_RET opSocGpioInPutInit(UCHAR_T pin_num, UCHAR_T pull_value);
OPERATE_RET opSocGpioInPutInitNoPull(UCHAR_T pin_num);

UCHAR_T ucSocGpioOutPutRead(UCHAR_T pin_num);
VOID vSocGpioOutPutWrite(UCHAR_T pin_num, UCHAR_T value);

UCHAR_T ucSocGpioInPutRead(UCHAR_T pin_num);
/************************* common interface end **********************/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif


