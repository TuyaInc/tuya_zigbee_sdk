

#ifndef HAL_BATTERY__H
#define HAL_BATTERY__H

#ifdef __cplusplus
extern "C"
{
#endif

#include "type_def.h"

//CR2032 Battery characteristic
#define BATTERY_DEFAULT_TABLE \
    {2400, 0}, /*minimum voltage, percentage*/ \
    {2600, 10},\
    {2800, 20},\
    {2825, 30},\
    {2850, 40},\
    {2875, 50},\
    {2900, 60},\
    {2925, 70},\
    {2950, 80},\
    {2975, 90},\
    {3000, 100},/*maximum voltage, percentage*/ \




/***********************************************************
    usage:

    only use hal_battery_config:
        hal_battery_config(NULL, NULL, 0);
        or configure use others parameters
        battery_cfg_t cfg = {
            120000UL,
            10000UL,
            4*3600*1000UL,
            3000,
            2400,
        };

        battery_table_t table[] = {
            BATTERY_DEFAULT_TABLE
        };

        uint8_t sums=sizeof(table)/sizeof(table[0]);
        hal_battery_config(&cfg, table, sums);

*******************************************************
*/


#ifdef __cplusplus
}
#endif

#endif

