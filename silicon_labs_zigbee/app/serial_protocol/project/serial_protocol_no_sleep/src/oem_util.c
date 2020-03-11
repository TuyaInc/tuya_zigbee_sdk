/*
 * @Author: Leon
 * @email: zhangpeng@tuya.com
 * @Date: 2019-09-16 10:56:20
 * @LastEditors: Leon
 * @LastEditTime: 2019-10-22 22:03:00
 * @file name: 
 * @Description: 
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 */
#include "oem_util.h"
#include "serial_protocol_callbacks.h"
#include "zigbee_sdk.h"

static oem_key_value_table_t value;
static oem_key_value_t oem_serial_protocol_cfg[] = 
{
    {"module", &value.zigbee_model_value, 7, VALUE_TYPE_STRING, FALSE},
    {"jv", &value.json_version_value, 6, VALUE_TYPE_STRING, FALSE},
    {"br", &value.bandrate, 4, VALUE_TYPE_NUMBER_DECIMAL, FALSE},
    {"urx", &value.uart_rx_index, 1, VALUE_TYPE_NUMBER_DECIMAL, FALSE},
    {"utx", &value.uart_tx_index, 1, VALUE_TYPE_NUMBER_DECIMAL, FALSE},
    {"conf_type", &value.config_type, 9, VALUE_TYPE_STRING, FALSE},
    {"wake_per", &value.wakeup_period, 4, VALUE_TYPE_NUMBER_DECIMAL, FALSE},
    {"wake_gpio_mcu_pin", &value.mcu_wakeup_gpio_index, 1, VALUE_TYPE_NUMBER_DECIMAL, FALSE},
    {"wake_gpio_soc_pin", &value.soc_wakeup_gpio_index, 1, VALUE_TYPE_NUMBER_DECIMAL, FALSE},
    {"wake_gpio_mcu_lv", &value.mcu_wakeup_gpio_level, 1, VALUE_TYPE_NUMBER_DECIMAL, FALSE},
    {"wake_gpio_soc_lv", &value.soc_wakeup_gpio_level, 1, VALUE_TYPE_NUMBER_DECIMAL, FALSE},
};

/**
 * @description: zigbee model get function
 * @param {modle_name} zigbee model string
 * @return: none
 */
uint8_t zigbee_model_get(uint8_t* modle_name)
{
    if(!memcmp(modle_name, "TYZS3", 5))
    {
        return TYZS3;
    }
    else if(!memcmp(modle_name, "TYZS5", 5))
    {
        return TYZS5;
    }
    else if(!memcmp(modle_name, "TYZS7", 5))
    {
        return TYZS7;
    }
    else if(!memcmp(modle_name, "ZS3L", 5))
    {
        return ZS3L;
    }
    else
    {
        return 0;
    }
}

uint8_t app_type_get(uint8_t* modle_name)
{
    if(!memcmp(modle_name, "dc_power", 8))
    {
        return HA_COMMON_DEV_DC;
    }
    else if(!memcmp(modle_name, "lo_power", 8))
    {
        return HA_COMMON_DEV_SLEEP;
    }
    else if(!memcmp(modle_name, "scene", 5))
    {
        return HA_SCENE_SELECTOR;
    }
    else
    {
        return 0;
    }
}
/**
 * @description: zigbee wakeup type get function
 * @param {wakeup_mode} wakeup model string
 * @return: none
 */
uint8_t zigbee_wkeup_type_get(uint8_t* wakeup_mode)
{
    if(!memcmp(wakeup_mode, "GPIO", 4))
    {
        return 1;//SOC_MCU_WAKEUP_BY_GPIO;
    }
    else if(!memcmp(wakeup_mode, "UART", 4))
    {
        return 0;//SOC_MCU_WAKEUP_BY_UART;
    }

    return 0;
}

/**@brief Load device config.
 *
 * @param[cfg] oem device config info
 * 
 return : true or false
 */  
bool_t oem_cfg_load(oem_dev_t *cfg)
{
    memset(&value, 0, sizeof(oem_key_value_table_t));

    if(cfg == NULL)
    {
        return FALSE;
    }

    if(TRUE == get_oem_key_values(oem_serial_protocol_cfg, sizeof(oem_serial_protocol_cfg) / sizeof(oem_key_value_t)))
    {
        uint8_t index = 0;

        if(oem_serial_protocol_cfg[index++].handle_flag) //zigbee model
        {
            uint8_t model_value = 0;

            model_value = zigbee_model_get(value.zigbee_model_value);

            if(model_value)
            {
                cfg->zigbee_model = model_value;
            }
        }

        if(oem_serial_protocol_cfg[index++].handle_flag) //json version
        {
            strncpy(cfg->json_version, value.json_version_value, JSON_VER_LENTH );
        }

        if(oem_serial_protocol_cfg[index++].handle_flag) //serial bandrate
        {
            cfg->bandrate = value.bandrate;
        }
        
        if(oem_serial_protocol_cfg[index++].handle_flag) //uart rx
        {
            GPIO_PORT_PIN_T *gpio = NULL;

            if((gpio = oem_gpio_get(value.uart_rx_index)) != NULL)
            {
                cfg->uart_rx_gpio.port = gpio->port;
                cfg->uart_rx_gpio.pin = gpio->pin;
            }
        }

        if(oem_serial_protocol_cfg[index++].handle_flag) //uart tx
        {
            GPIO_PORT_PIN_T *gpio = NULL;

            if((gpio = oem_gpio_get(value.uart_tx_index)) != NULL)
            {
                cfg->uart_tx_gpio.port = gpio->port;
                cfg->uart_tx_gpio.pin = gpio->pin;
            }
        }

        if(oem_serial_protocol_cfg[index++].handle_flag) //config app type
        {
            cfg->app_type = app_type_get(value.config_type);
        }
        else
        {
            return FALSE;
        }

        if(cfg->app_type == HA_COMMON_DEV_SLEEP)
        {
            if(oem_serial_protocol_cfg[index++].handle_flag) //sleep device wakeup period
            {
                cfg->wakeup_period = value.wakeup_period;

                if(cfg->wakeup_period < 200)
                {
                    cfg->wakeup_period = 200;
                }
            }
            else
            {
                return FALSE;
            }

            if(oem_serial_protocol_cfg[index++].handle_flag) //mcu wakeup gpio
            {
                GPIO_PORT_PIN_T *gpio = NULL;
                
                if((gpio = oem_gpio_get(value.mcu_wakeup_gpio_index)) != NULL)
                {
                    cfg->mcu_wakeup_gpio.port = gpio->port;
                    cfg->mcu_wakeup_gpio.pin = gpio->pin;
                }
            }
            else
            {
                return FALSE;
            }

            if(oem_serial_protocol_cfg[index++].handle_flag) //soc wakeup gpio
            {
                GPIO_PORT_PIN_T *gpio = NULL;
                
                if((gpio = oem_gpio_get(value.soc_wakeup_gpio_index)) != NULL)
                {
                    cfg->soc_wakeup_gpio.port = gpio->port;
                    cfg->soc_wakeup_gpio.pin = gpio->pin;
                }
            }
            else
            {
                return FALSE;
            }

            if(oem_serial_protocol_cfg[index++].handle_flag) //mcu wakeup gpio level
            {
                cfg->mcu_wakeup_gpio.drive_flag = value.mcu_wakeup_gpio_level;
                cfg->mcu_wakeup_gpio.out = (value.mcu_wakeup_gpio_level == TRUE) ? FALSE : TRUE;
                cfg->mcu_wakeup_gpio.mode = GPIO_MODE_OUTPUT_PP;
            }
            else
            {
                return FALSE;
            }

            if(oem_serial_protocol_cfg[index++].handle_flag) //soc wakeup gpio level
            {
                cfg->soc_wakeup_gpio.drive_flag = value.soc_wakeup_gpio_level;
                cfg->soc_wakeup_gpio.out = (value.soc_wakeup_gpio_level == TRUE) ? FALSE : TRUE;
                cfg->soc_wakeup_gpio.mode = GPIO_MODE_INPUT_PULL;
            }
            else
            {
                return FALSE;
            }
        }

        return TRUE;
    }
}
