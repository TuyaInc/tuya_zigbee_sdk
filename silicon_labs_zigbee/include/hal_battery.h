/**
 * @author Leon
 * @par email:
 *      zhangpeng@tuya.com
 * @file hal_battery.h
 * @brief This file defines the functions related to power collection
 * @copyright HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @par company
 *      http://www.tuya.com
 * @date 2020-03-20 17:29:33
 * @par Usage
 * @code
 *      only use hal_battery_config:
 *      hal_battery_config(NULL, NULL, 0);
 *      or configure use others parameters
 *      battery_cfg_t cfg = {
 *          120000UL,
 *          10000UL,
 *          4*3600*1000UL,
 *          3000,
 *          2400,
 *      };
 *      battery_table_t table[] = {
 *          BATTERY_DEFAULT_TABLE
 *      };
 *
 *      uint8_t sums=sizeof(table)/sizeof(table[0]);
 *      hal_battery_config(&cfg, table, sums);
 * @endcode
 */


#ifndef __HAL_BATTERY_H__
#define __HAL_BATTERY_H__

#include "type_def.h"
#include "hal_adc.h"

#ifdef __cplusplus
extern "C"
{
#endif




/**
 * @note CR2032 Battery characteristic
 */
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


/**
 * @enum battery type
 */
typedef enum {
    BATTERY_TYPE_DRY_BATTERY = 0,  ///< Battery percentage always decrease.
    BATTERY_TYPE_CHARGE_BATTERY,   ///< Permissible battery percentage increase.
}BATTERY_TYPE_T;

typedef struct {
    uint32_t cap_first_delay_time;          ///< delay time of first battery percentage capture
    uint32_t cap_waitting_silence_time;     ///< time to wait for system silence
    uint32_t cap_max_period_time;           ///< maximum capture period
    uint16_t cap_max_voltage;               ///< maximum voltage of battery.
    uint16_t cap_min_voltage;               ///< minimum voltage of battery.
    adc_cfg_t adc;
}battery_cfg_t;

typedef struct {
    uint16_t voltage;       ///< battery voltage
    uint8_t  percent;       ///< percentage of current battery voltage
}battery_table_t;

/**
 * @enum The way the device works
 */
typedef enum {
    DEV_BUSY_LEVEL_IDLE = 0, ///< always sleep
    DEV_BUSY_LEVEL_ALWAYS,   ///< always wakeup, example: poll forever.
}DEV_BUSY_LEVEL_T;

/**
 * @enum Gradient descent of the battery
 */
typedef enum {
    BATTERY_REPORT_DECREASE_LIMITS_10 = 1, ///< The difference between two reported battery percentages should not exceed 10
    BATTERY_REPORT_DECREASE_LIMITS_20,
    BATTERY_REPORT_DECREASE_LIMITS_30,
    BATTERY_REPORT_DECREASE_LIMITS_40,
    BATTERY_REPORT_DECREASE_LIMITS_50,
    BATTERY_REPORT_DECREASE_LIMITS_60,
    BATTERY_REPORT_DECREASE_LIMITS_70,
    BATTERY_REPORT_DECREASE_LIMITS_80,
    BATTERY_REPORT_DECREASE_LIMITS_90,
    BATTERY_REPORT_NO_LIMITS,   ///< no limits
    BATTERY_REPORT_EXT_LIMITS,  ///< The difference between the two reported battery percentages cannot exceed the custom data
}BATTERY_REPORT_DECREASE_LIMITS_T;

typedef struct {
    BATTERY_TYPE_T   type;
    DEV_BUSY_LEVEL_T level;
    bool_t report_no_limits_first;
    BATTERY_REPORT_DECREASE_LIMITS_T limits;
    uint8_t ext_limits;
}battery_report_policy_t;

/**
* @note input battery characteristic to system. 
* @param[in] {cfg} battery capture parameters 
* @param[in] {table} battery voltage characteristic table. 
* @param[in] {table_sums} number of table records.
* @return none
*/
extern void hal_battery_config(battery_cfg_t *cfg, battery_table_t *table, uint8_t table_sums);

/**
* @note delay battery capture and report battery info to gateway.
* @param[in] {delay_time} delay time. bet:ms
* @return none
*/
extern void hal_battery_capture_manual(uint32_t delay_time);

/**
* @note change battery capture period.
* @param[in] {cap_period_time} battery capture period
* @return none
*/
extern void hal_battery_capture_period_change(uint32_t cap_period_time);



/**
* @note Set battery type\n
* BATTERY_TYPE_DRY_BATTERY:    Battery percentage always decrease.\n
* BATTERY_TYPE_CHARGE_BATTERY: Permissible battery percentage increase.\n
* DEV_BUSY_LEVEL_IDLE:   Battery capture in idle time.\n
* DEV_BUSY_LEVEL_ALWAYS: battery capture when time out.\n
* @param[in] {type} battery type
* @param[in] {level} battery type
* @return none
*/
extern void hal_battery_set_battery_type(BATTERY_TYPE_T type, DEV_BUSY_LEVEL_T level);

/**
* @note config report policy.\n
* type:\n
*   BATTERY_TYPE_DRY_BATTERY:    Battery percentage always decrease.\n
*   BATTERY_TYPE_CHARGE_BATTERY: Permissible battery percentage increase.\n
* level:\n
*   DEV_BUSY_LEVEL_IDLE:   Battery capture in idle time.\n
*   DEV_BUSY_LEVEL_ALWAYS: battery capture when time out.\n
* report_no_limits_first:\n
*   TRUE: report the actual battery percentage value first.\n
*   FALSE: report the handled battery percentage value first.\n 
* limits:\n
*   the limit level of report power percentage.\n
* ext_limits:\n
*   use othes value if limits = BATTERY_REPORT_EXT_LIMITS.  limits[2, 100]
* @param[in] {type} battery type
* @param[in] {level} battery type
* @return none
*/
extern void hal_battery_report_policy_config(battery_report_policy_t *policy);




#ifdef __cplusplus
}
#endif

#endif

