/*
 * @Author: jin lu
 * @email: jinlu@tuya.com
 * @Date: 2019-05-23 16:56:24
 * @LastEditors:   
 * @LastEditTime: 2019-05-28 19:57:56
 * @file name: 
 * @Description: 
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 */
/************************************************************************/
#include "soc_gpio.h"


/************************************************************************/
module_table_t module_tabel[] = 
{
    {"TYZS1", TYZS1},
    {"TYZS1L", TYZS1L},
    {"TYZS2", TYZS2},
    {"TYZS2R", TYZS2R},
    {"TYZS3", TYZS3},
    {"TYZS4", TYZS4},
    {"TYZS5", TYZS5},
    {"TYZS5L", TYZS5L},
    {"TYZS6", TYZS6},
    {"TYZS7", TYZS7},
    {"TYZS8", TYZS8},
    {"TYZS9V", TYZS9V},
    {"TYZS10", TYZS10},
    {"TYZS11", TYZS11},
    {"TYZS12", TYZS12},
    {"TYZS13", TYZS13},
    {"TYZS15", TYZS15},
};
num2pin_t MODULE_TYZS[]=
{
    {0,PORT_A,PIN_3},
    {1,NULL,  NULL},
    {2,PORT_A,PIN_5},
    {3,PORT_D,PIN_15},
    {4,NULL,  NULL},
    {5,PORT_B,PIN_11},
    {6,PORT_F,PIN_1},
    {7,PORT_F,PIN_0},
    {8,PORT_F,PIN_2},
    {9,PORT_A,PIN_0},
    {10,PORT_A,PIN_1},
    {11,PORT_F,PIN_5},
    {12,PORT_A,PIN_2},
    {13,PORT_F,PIN_4},
    {14,NULL,  NULL},
    {15,NULL,  NULL},
    {16,NULL,  NULL},
    {17,NULL,  NULL},
    {18,NULL,  NULL},
    {19,NULL,  NULL},
    {20,PORT_B,PIN_12},
    {21,PORT_B,PIN_13},
    {22,PORT_B,PIN_15},
    {23,PORT_C,PIN_7},
    {24,PORT_C,PIN_8},
    {25,PORT_C,PIN_9},
    {26,PORT_C,PIN_10},
    {27,PORT_C,PIN_11},
    {28,PORT_D,PIN_11},
    {29,PORT_D,PIN_12},
    {30,PORT_D,PIN_13},
    {31,PORT_D,PIN_14},
    {32,PORT_F,PIN_3},
    {33,PORT_F,PIN_6},
};

num2pin_t MODULE_TYZS5[]=
{
    {0,NULL,NULL},
    {1,NULL,NULL},
    {2,NULL,NULL},
    {3,NULL,NULL},
    {4,NULL,NULL},
    {5,PORT_D,PIN_15},
    {6,PORT_F,PIN_1},
    {7,PORT_F,PIN_0},
    {8,NULL,NULL},
    {9,PORT_F,PIN_5},
    {10,PORT_F,PIN_2},
    {11,PORT_A,PIN_4},
    {12,PORT_A,PIN_3},
    {13,NULL,NULL},
    {14,NULL,NULL},
    {15,NULL,NULL},
    {16,PORT_A,PIN_0},
    {17,PORT_A,PIN_1},
};

#define MODULE_TABLE_LEN   get_array_len(module_tabel)
UCHAR_T g_module = TYZS3;


/********************* platform specific interface **********************/
/**
 * @description: get port number from pin index
 * @param {pin_num} pin index of the module
 * @return: port number of pin index
 */
GPIO_PORT_T xSocGpioGetPort(UCHAR_T pin_num)
{
    GPIO_PORT_T port;
    switch(g_module)
    {
        case TYZS2R:
        case TYZS5:
            port = MODULE_TYZS5[pin_num].port;
        break;

        default:
            port = MODULE_TYZS[pin_num].port;
        break;
    }

    return port;
}

/**
 * @description: get port number from pin index
 * @param {pin_num} pin index of the module
 * @return: actual pin number of pin index
 */
GPIO_PIN_T xSocGpioGetPin(UCHAR_T pin_num)
{
    GPIO_PIN_T pin;
    switch(g_module)
    {
        case TYZS2R:
        case TYZS5:
            pin = MODULE_TYZS5[pin_num].pin;
        break;

        default:
            pin = MODULE_TYZS[pin_num].pin;
        break;
    }
    return pin;
}
/********************* platform specific interface end********************/


/********************    common interface              *******************/
/**
 * @description: module init, config the global variable : g_module
 * @param {name} module name string 
 * @param {len}  module name string length
 * @return: none
 */
BOOL_T bSocGpioModuleInit(PCHAR_T name, UCHAR_T len)
{
    UCHAR_T i;
    for(i=0;i<MODULE_TABLE_LEN;i++){ 
       if(string_compare(name, module_tabel[i].string)){
          g_module =  module_tabel[i].module;
          return true;
       }
    }
    return false;
}
/**
 * @description: GPIO output init
 * @param {type} none
 * @return: none
 */
VOID vSocGpioOutPutInit(UCHAR_T pin_num, UCHAR_T init_value)
{
    gpio_config_t hal_gpio_config;
    hal_gpio_config.port = xSocGpioGetPort(pin_num);
    hal_gpio_config.pin = xSocGpioGetPin(pin_num);
    hal_gpio_config.mode = GPIO_MODE_OUTPUT_PP;
    hal_gpio_config.out = init_value;
    hal_gpio_config.drive_flag = 0;
    gpio_raw_init(hal_gpio_config);
}

/**
 * @description: read gpio level
 * @param {type} none
 * @return: none
 */
UCHAR_T vSocGpioOutPutRead(UCHAR_T pin_num)
{
    GPIO_PORT_T port = xSocGpioGetPort(pin_num);
    GPIO_PIN_T pin = xSocGpioGetPin(pin_num);
    return gpio_raw_output_read_status(port,pin);
}
/**
 * @description: 
 * @param {type} none
 * @return: none
 */
VOID vSocGpioOutPutWrite(UCHAR_T pin_num, UCHAR_T value)
{
    GPIO_PORT_T port = xSocGpioGetPort(pin_num);
    GPIO_PIN_T pin = xSocGpioGetPin(pin_num);
    gpio_raw_output_write_status(port, pin , value);
}

VOID vSocGpioInPutInit(UCHAR_T pin_num, UCHAR_T pull_value)
{
    gpio_config_t hal_gpio_config;
    hal_gpio_config.port = xSocGpioGetPort(pin_num);
    hal_gpio_config.pin = xSocGpioGetPin(pin_num);
    hal_gpio_config.mode = GPIO_MODE_INPUT_PULL;
    hal_gpio_config.out = pull_value;
    hal_gpio_config.drive_flag = 0;
    gpio_raw_init(hal_gpio_config);
}

UCHAR_T vSocGpioInPutRead(UCHAR_T pin_num)
{
    GPIO_PORT_T port = xSocGpioGetPort(pin_num);
    GPIO_PIN_T pin = xSocGpioGetPin(pin_num);
    return gpio_raw_input_read_status(port, pin);
}