/*************************************************************************************/
/* Automatically-generated file. Do not edit! */
/*************************************************************************************/
#include "config.h"


char *g_firmware_name = FIRMWARE_NAME;
uint8_t g_firmware_ver = FIRMWARE_VER;

void firmware_config(void)
{
    dev_register_base_info(MODEL_ID, PRODUCTOR_ID_PFEFIX, PRODUCTOR_ID);
}
