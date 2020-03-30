/**
 * @author Leon
 * @par email:
 *      zhangpeng@tuya.com
 * @file tuya_app_timer.h
 * @brief This file describes an advanced API for timers
 * @copyright HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @par company
 *      http://www.tuya.com
 * @date 2020-03-20 17:29:33
 */

#ifndef __TUYA_APP_TIMER_H__
#define __TUYA_APP_TIMER_H__

#include "type_def.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
  uint16_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hours;
  uint8_t minutes;
  uint8_t seconds;
  uint8_t week; //0=MON, 1=TUES, etc.
} device_time_struct_t;


typedef struct {
    uint16_t year; ///< normal range:> 2019, 0xFFFF means: Wildcard character, It's valid for any year
    uint8_t  mon;  ///< normal range:1~12, 0xFF means: Wildcard character, It's valid for any month
    uint8_t  day;  ///< normal range:1~31, 0xFF means: Wildcard character, It's valid for any day
}app_day_t;

typedef struct {
    uint8_t  hour; ///< normal range:0~23, 0xFF means: Wildcard character, It's valid for any hour
    uint8_t  min;  ///< normal range:0~59, 0xFF means: Wildcard character, It's valid for any minute
    uint8_t  sec;  ///< range: 0~59
}app_time_t;

/**
 * @enum timer type
 */
typedef enum {
    APP_TIMER_TYPE_CALENDAR = 0, ///< Time according to calendar format
    APP_TIMER_TYPE_WEEK,         ///< Time according to week format
}APP_TIMER_TYPE_T;


#define WEEK_BITS_MAP_MONDAY    0x01 ///< bit map for monday, WEEK_BITS_MAP_T use
#define WEEK_BITS_MAP_TUESDAY   0x02 ///< bit map for tuesday, WEEK_BITS_MAP_T use
#define WEEK_BITS_MAP_WEDNESDAY 0x04 ///< bit map for wednesday, WEEK_BITS_MAP_T use
#define WEEK_BITS_MAP_THURSDAY  0x08 ///< bit map for thursday, WEEK_BITS_MAP_T use
#define WEEK_BITS_MAP_FRIDAY    0x10 ///< bit map for friday, WEEK_BITS_MAP_T use
#define WEEK_BITS_MAP_SATURDAY  0x20 ///< bit map for saturday, WEEK_BITS_MAP_T use
#define WEEK_BITS_MAP_SUNDAY    0x40 ///< bit map for sunday, WEEK_BITS_MAP_T use

typedef uint8_t WEEK_BITS_MAP_T; ///< bit map for week

typedef void (*app_timer_callback_t)(uint8_t index); ///< This function is called when the time is up

typedef struct {
    APP_TIMER_TYPE_T type;      ///< Currently only APP_TIMER_TYPE_WEEK is supported
    bool_t period_action_flag;  ///< Whether to execute periodically
    bool_t valid;               ///< Whether the configuration is valid or not
    app_time_t time;            ///< The specific time
    union {
        app_day_t calendar_day;
        WEEK_BITS_MAP_T week_map; ///< It's a bit map. WEEK_BITS_MAP_MONDAY|WEEK_BITS_MAP_TUESDAY....
    } day_struct;
    app_timer_callback_t func;  ///< This function is called when the time is up
}app_timer_struct_t;

/**
 * @note config the app timer
 * @param[in] {app_timer_struct_list} timer task list.
 * @param[in] {sums} timer task sums
 * @return TRUE for successful, FALSE for failed
 */
extern bool_t app_timer_config(app_timer_struct_t *app_timer_struct_list, uint8_t sums);

/**
 * @note get the remain seconds of next app timer task
 * @param none
 * @return time: seconds
 */
extern uint32_t app_timer_get_next_task_seconds(void);

/**
 * @note set time period for device get time form gateway
 * @param[in] {time_period} time period  (unit: ms) ,default time (30 * 60 *1000 ms) 
 * @return none
 */
extern void set_read_time_period(uint32_t time_period);

/**
 * @note get device current time (unit:S)
 * @param[in] none
 * @return device current time (unit:S)
 */
extern uint32_t  get_current_time(void);

/**
 * @note set time update interval. default value: 1 (unit: second)
 * @param[in] {interval_sec} update ervery interval_sec.
 * @return none
 */
extern void dev_time_tick_update_interval_set(uint16_t interval_sec);

/**
 * @note get the parameter of update time (unit:second)
 * @param none
 * @return interval of update time (unit:S)
 */
extern uint16_t dev_time_tick_update_interval_get(void);


/**
 * @note get device current time with convert time struct
 * @param[out] {device_time} time struct with yeah ,month,day,hour,minute,second
 * @return none
 */
extern void get_current_time_struct(device_time_struct_t *device_time);


#ifdef __cplusplus
}
#endif

#endif




