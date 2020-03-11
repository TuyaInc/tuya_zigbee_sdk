#ifndef __OEM_UTIL_H__
#define __OEM_UTIL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "zigbee_sdk.h"

#define JSON_VER_LENTH    6

typedef enum
{
    HA_COMMON_DEV_DC = 1,
    HA_COMMON_DEV_SLEEP,
    HA_SCENE_SELECTOR
}APP_TYPE_E;

typedef enum
{
    ZIGBEE_ROUTE = 1,
    ZIGBEE_SLEEP_ED
}DEV_TYPE_E;

typedef struct 
{
    uint8_t zigbee_model_value[7];
    uint8_t config_type[9];
    uint8_t json_version_value[6];
    uint32_t wakeup_period;
    uint8_t mcu_wakeup_gpio_index;
    uint8_t soc_wakeup_gpio_index;
    bool_t mcu_wakeup_gpio_level;
    bool_t soc_wakeup_gpio_level;
    uint8_t uart_rx_index;
    uint8_t uart_tx_index;
    uint32_t bandrate;
}oem_key_value_table_t;

typedef struct
{
    uint8_t zigbee_model;
    APP_TYPE_E app_type;
    DEV_TYPE_E dev_type;
    gpio_config_t mcu_wakeup_gpio;
    gpio_config_t soc_wakeup_gpio;
    gpio_config_t uart_rx_gpio;
    gpio_config_t uart_tx_gpio;
    uint32_t wakeup_period;
    uint32_t bandrate;
    uint8_t json_version[JSON_VER_LENTH + 1];
}oem_dev_t;

extern bool_t oem_cfg_load(oem_dev_t *cfg);

#ifdef __cplusplus
}
#endif

#endif
