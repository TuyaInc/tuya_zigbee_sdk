/*
 * @Author: jin lu
 * @email: jinlu@tuya.com
 * @Date: 2019-05-23 16:56:24
 * @LastEditors:
 * @LastEditTime: 2019-08-23 09:58:16
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
    {"ZS3L", ZS3L},
};
num2pin_t MODULE_TYZS[]=
{
    {1,PORT_A,PIN_0},
    {2,PORT_A,PIN_1},
    {3,PORT_A,PIN_2},
    {4,PORT_A,PIN_3},
    {5,PORT_A,PIN_4},
    {6,PORT_A,PIN_5},
    {7,PORT_B,PIN_11},
    {8,PORT_B,PIN_12},
    {9,PORT_B,PIN_13},
    {10,PORT_B,PIN_14},
    {11,PORT_B,PIN_15},
    {12,PORT_C,PIN_6},
    {13,PORT_C,PIN_7},
    {14,PORT_C,PIN_8},
    {15,PORT_C,PIN_9},
    {16,PORT_C,PIN_10},
    {17,PORT_C,PIN_11},
    {18,PORT_D,PIN_13},
    {19,PORT_D,PIN_14},
    {20,PORT_D,PIN_15},
    {21,PORT_F,PIN_0},
    {22,PORT_F,PIN_1},
    {23,PORT_F,PIN_2},
    {24,PORT_F,PIN_3},
    {25,PORT_F,PIN_4},
    {26,PORT_F,PIN_5},
    {27,PORT_F,PIN_6},
    {28,PORT_F,PIN_7},
    {29,PORT_D,PIN_10},
    {30,PORT_D,PIN_11},
    {31,PORT_D,PIN_12},
};
num2pin_t MODULE_MG21[]=
{
    {1,PORT_A,PIN_0},
    {2,PORT_A,PIN_1},
    {3,PORT_A,PIN_2},
    {4,PORT_A,PIN_3},
    {5,PORT_A,PIN_4},
    {6,PORT_A,PIN_5},
    {7,PORT_A,PIN_6},
    {8,PORT_B,PIN_0},
    {9,PORT_B,PIN_1},
    {10,PORT_C,PIN_0},
    {11,PORT_C,PIN_1},
    {12,PORT_C,PIN_2},
    {13,PORT_C,PIN_3},
    {14,PORT_C,PIN_4},
    {15,PORT_C,PIN_5},
    {16,PORT_D,PIN_0},
    {17,PORT_D,PIN_1},
    {18,PORT_D,PIN_2},
    {19,PORT_D,PIN_3},
    {20,PORT_D,PIN_4},
};

#define MODULE_TABLE_LEN   get_array_len(module_tabel)
#define TYZS_PIN_TABLE_LEN   get_array_len(MODULE_TYZS)
#define MG21_PIN_TABLE_LEN   get_array_len(MODULE_MG21)

UCHAR_T g_module = TYZS3;
/**
 * @description: compare two string
 * @param {IN CHAR_T* str1 -> string 1}
 * @param {IN CHAR_T* str2 -> string 2}
 * @return: 0: if these two string is not same,
 *          1: these two string is all same
 */
BOOL_T bStringCompare(IN CHAR_T* str1, IN CHAR_T* str2)
{

    while(*str1 !='\0'&& *str2 != '\0')
    {
        if(*str1 != *str2){
            return 0;
        }
        str1++;
        str2++;
    }

    if(*str1 =='\0'&& *str2 == '\0') {
        return 1;
    } else {
      return 0;
    }
}

/********************* platform specific interface **********************/

/********************* platform specific interface **********************/
UCHAR_T get_module(void)
{
    return g_module;
}
/********************* platform specific interface end********************/


/********************    common interface              *******************/
/**
 * @description: module init, config the global variable : g_module
 * @param {name} module name string
 * @param {len}  module name string length
 * @return: none
 */
OPERATE_RET opSocGpioModuleInit(PCHAR_T name, UCHAR_T len)
{
    UCHAR_T i;
    for(i=0;i<MODULE_TABLE_LEN;i++){
       if(bStringCompare(name, module_tabel[i].string)){
          g_module =  module_tabel[i].module;
          return OPRT_OK;
       }
    }
    app_print("GPIO module init ERR");
    return OPRT_INVALID_PARM;
}
/**
 * @description: get port number from pin index
 * @param {pin_num} pin index of the module
 * @return: port number of pin index
 */
GPIO_PORT_T xSocGpioGetPort(UCHAR_T pin_num)
{
    GPIO_PORT_T port;
    UCHAR_T i;
    switch(g_module)
    {
        case ZS3L:
        {
            for(i = 0; i < MG21_PIN_TABLE_LEN; i++){
                if(pin_num == MODULE_MG21[i].num){
                    return MODULE_MG21[i].port;
                }
            }
            if(i == MG21_PIN_TABLE_LEN){ //can't find port in the table
                return PIN_NUM_ERROR;
            }
          break;
        }
        default:
        {
            for(i = 0; i < TYZS_PIN_TABLE_LEN; i++){
                if(pin_num == MODULE_TYZS[i].num){
                    return MODULE_TYZS[i].port;
                }
            }
            if(i == TYZS_PIN_TABLE_LEN){ //can't find port in the table
                return PIN_NUM_ERROR;
            }
            break;
        }
    }
    return PIN_NUM_ERROR;
}

/**
 * @description: get port number from pin index
 * @param {pin_num} pin index of the module
 * @return: actual pin number of pin index
 */
GPIO_PIN_T xSocGpioGetPin(UCHAR_T pin_num)
{
 UCHAR_T i;
    switch(g_module)
    {
        case ZS3L:
        {
            for(i = 0; i < MG21_PIN_TABLE_LEN; i++){
                if(pin_num == MODULE_MG21[i].num){
                    return MODULE_MG21[i].pin;
                }
            }
            if(i == MG21_PIN_TABLE_LEN){ //can't find port in the table
                return PIN_NUM_ERROR;
            }
            break;
        }
        default:
        {
            for(i = 0; i < TYZS_PIN_TABLE_LEN; i++){
                if(pin_num == MODULE_TYZS[i].num){
                    return MODULE_TYZS[i].pin;
                }
            }
            if(i == TYZS_PIN_TABLE_LEN){ //can't find port in the table
                return  PIN_NUM_ERROR;
            }
            break;
        }
    }
    return PIN_NUM_ERROR;
}

/**
 * @description: GPIO output init
 * @param {type} none
 * @return: none
 */
OPERATE_RET opSocGpioOutPutInit(UCHAR_T pin_num, UCHAR_T init_value)
{
    gpio_config_t hal_gpio_config;
    hal_gpio_config.port = xSocGpioGetPort(pin_num);
    hal_gpio_config.pin = xSocGpioGetPin(pin_num);
    hal_gpio_config.mode = GPIO_MODE_OUTPUT_PP;
    hal_gpio_config.out = init_value;
    hal_gpio_config.drive_flag = 0;

    if((PIN_NUM_ERROR == hal_gpio_config.port) ||(PIN_NUM_ERROR == hal_gpio_config.pin)){
        app_print("GPIO pin num ERR");
        return OPRT_INVALID_PARM;
    }

    gpio_raw_init(hal_gpio_config);
    return OPRT_OK;
}

/**
 * @description: read gpio level
 * @param {type} none
 * @return: none
 */
UCHAR_T ucSocGpioOutPutRead(UCHAR_T pin_num)
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

OPERATE_RET opSocGpioInPutInit(UCHAR_T pin_num, UCHAR_T pull_value)
{
    gpio_config_t hal_gpio_config;
    hal_gpio_config.port = xSocGpioGetPort(pin_num);
    hal_gpio_config.pin = xSocGpioGetPin(pin_num);
    hal_gpio_config.mode = GPIO_MODE_INPUT_PULL;
    hal_gpio_config.out = pull_value;
    hal_gpio_config.drive_flag = 0;

    if((PIN_NUM_ERROR == hal_gpio_config.port) ||(PIN_NUM_ERROR == hal_gpio_config.pin)){
        app_print("GPIO pin num ERR");
        return OPRT_INVALID_PARM;
    }

    gpio_raw_init(hal_gpio_config);
    return OPRT_OK;
}


OPERATE_RET opSocGpioInPutInitNoPull(UCHAR_T pin_num)
{
    gpio_config_t hal_gpio_config;
    hal_gpio_config.port = xSocGpioGetPort(pin_num);
    hal_gpio_config.pin = xSocGpioGetPin(pin_num);
    hal_gpio_config.mode = GPIO_MODE_INPUT_HIGH_IMPEDANCE;
    hal_gpio_config.out = 0;
    hal_gpio_config.drive_flag = 0;

    if((PIN_NUM_ERROR == hal_gpio_config.port) ||(PIN_NUM_ERROR == hal_gpio_config.pin)){
        app_print("GPIO pin num ERR");
        return OPRT_INVALID_PARM;
    }

    gpio_raw_init(hal_gpio_config);
    return OPRT_OK;
}




UCHAR_T ucSocGpioInPutRead(UCHAR_T pin_num)
{
    GPIO_PORT_T port = xSocGpioGetPort(pin_num);
    GPIO_PIN_T pin = xSocGpioGetPin(pin_num);
    return gpio_raw_input_read_status(port, pin);
}