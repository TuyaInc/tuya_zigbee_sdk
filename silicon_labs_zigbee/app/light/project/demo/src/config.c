/*************************************************************************************/
/* Automatically-generated file. Do not edit! */
/*************************************************************************************/
#include "zigbee_sdk.h"
#include "config.h"


char *g_firmware_name = FIRMWARE_NAME;
uint8_t g_firmware_ver = FIRMWARE_VER;

gpio_config_t gpio_output_config[LED_NUM] = {
    {LED0_PORT, LED0_PIN, LED0_MODE, LED0_OUT, LED0_DRIVER },
    {LED1_PORT, LED1_PIN, LED1_MODE, LED1_OUT, LED1_DRIVER },
    {LED2_PORT, LED2_PIN, LED2_MODE, LED2_OUT, LED2_DRIVER },
};

gpio_config_t gpio_input_config[KEY_NUM] = {
    {KEY0_PORT, KEY0_PIN, KEY0_MODE, KEY0_OUT, KEY0_DRIVER },
    {KEY1_PORT, KEY1_PIN, KEY1_MODE, KEY1_OUT, KEY1_DRIVER },
};

void firmware_config(void)
{
    dev_register_base_info(MODEL_ID, PRODUCTOR_ID_PFEFIX, PRODUCTOR_ID);
}
