/**
 * @author Leon
 * @par email:
 *      zhangpeng@tuya.com
 * @file tuya_mcu_os.h
 * @brief This file defines the functions of the MCU or system
 * @copyright HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @par company
 *      http://www.tuya.com
 * @date 2020-03-20 17:29:33
 */


#ifndef __TUYA_MCU_OS_H__
#define __TUYA_MCU_OS_H__

#include "type_def.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DEV_DEFAULT_WAKEUP_MAX_TIMEOUT   (1000*60*2)  ///< default max wakeup interval is 2 min

/**
 * @enum Reasons for restart
 */
typedef enum{
    RESET_REASON_UNKNOWN    = 0, ///< Underterminable cause
    RESET_REASON_FIB        = 1, ///< Reset originated from the FIB bootloader
    RESET_REASON_BOOTLOADER = 2, ///< Reset relates to an Ember bootloader
    RESET_REASON_EXTERNAL   = 3, ///< External reset trigger
    RESET_REASON_POWERON    = 4, ///< Poweron reset type, supply voltage < power-on threshold
    RESET_REASON_WATCHDOG   = 5, ///< Watchdog reset occurred
    RESET_REASON_SOFTWARE   = 6, ///< Software triggered reset
    RESET_REASON_CRASH      = 7, ///< Software crash
    RESET_REASON_FLASH      = 8, ///< Flash failure cause reset
    RESET_REASON_FATAL      = 9, ///< A non-recoverable fatal error occurred
    RESET_REASON_FAULT      = 10,///< A access fault occurred
    RESET_REASON_BROWNOUT   = 11,///< Brown out
}RESET_REASON_T;

/**
 * @enum Keep track of important things, 1-15 is used by sdk
 */
typedef enum {
    ERROR_CODE_IDLE             = 0, ///< idle state
    ERROR_CODE_ABNORMAL_RESET   = 16, ///< device reset
    ERROR_CODE_PARENT_LOST      = 17, ///< device parent lost
    ERROR_CODE_NWK_LEAVE_SELF   = 18, ///< device leave network by itself
    ERROR_CODE_NWK_LEAVE_GW     = 19, ///< device leave network by remote command, such as gateway or it's parent
    ERROR_CODE_IO_ERROR         = 20,  ///< GPIO input or output error
    ERROR_CODE_MEM_ASSERT       = 21,  ///< memory assert error
    ERROR_CODE_ADDR_CHANGE      = 22,  ///< addr change
    ERROR_CODE_RESET_LEAVE      = 23,  ///< recv ZCL_RESET_TO_FACTORY_DEFAULTS_COMMAND_ID
    ERROR_CODE_RESET_NO_LEAVE   = 24,  ///< recv basic cluster cmd 0xF0(private cmd)
    ERROR_CODE_WAKUP_TIMEOUT    = 25,  ///< a sleep device is always wakeup.
    ERROR_CODE_RESET_LEAVE_INVALID = 26, ///< recv ZCL_RESET_TO_FACTORY_DEFAULTS_COMMAND_ID with invalid arguments.
}DEV_ERROR_CODE_E;

/**
 * @enum All the software timers available to the user
 */
typedef enum {
    DEV_EVT_1  = 35, ///< event id 1
    DEV_EVT_2,       ///< event id 2
    DEV_EVT_3,
    DEV_EVT_4,
    DEV_EVT_5,
    DEV_EVT_6,
    DEV_EVT_7,
    DEV_EVT_8,
    DEV_EVT_9,
    DEV_EVT_10,
    DEV_EVT_11,
    DEV_EVT_12,
    DEV_EVT_13,
    DEV_EVT_14,
    DEV_EVT_15,
    DEV_EVT_16,
}DEV_EVT_T;

typedef void (*timer_func_t)(uint8_t); ///< software timer callback


/**
 * @enum Type parameter of the callback function
 */
typedef enum {
    CALLBACK_TYPE_REPORT_BATTERY = 0, ///< after report
    CALLBACK_TYPE_CAPTURE_BATTERY,    ///< after capture
    CALLBACK_TYPE_POLL,               ///< after poll
    CALLBACK_TYPE_HEARTBEAT,          ///< after heartbeat
}CALLBACK_TYPE_T;

typedef struct {
    uint16_t report_voltage; ///< Display voltage reported to the gateway
    uint16_t real_voltage;   ///< The actual voltage reported to the gateway
    uint8_t  report_percent; ///< Display percentage reported to the gateway
    uint8_t  real_percent;   ///< The actual percentage reported to the gateway
}cb_battery_args_t;

/**
 * @enum The result of poll
 */
typedef enum {
    POLL_OK = 0, ///< ok
    POLL_NO_ACK, ///< no mac ack
    POLL_BUSY,   ///< interference
}POLL_RESULT_T;

typedef struct {
    POLL_RESULT_T result;
}cb_poll_args_t;

typedef struct {
    SEND_ST_T result;
}cb_heartbeat_args_t;

typedef struct {
    CALLBACK_TYPE_T type;
    union {
        cb_battery_args_t battery;
        cb_poll_args_t poll;
        cb_heartbeat_args_t heartbeat;
    }args;
}cb_args_t;


typedef void (*sdk_evt_callback_fun_t)(cb_args_t *args); ///< important events handler
typedef void (*soft_sv_callback_t)(void); ///< CPU pendsv interrupt handler

///@name pendsv callback
/**
 * @note start a PendSV interrupt. PendSV_Handler will call this funciton(soft_sv_callback_t).
 * @param[in] {func} user function
 * @return none 
 */
extern void start_soft_int(soft_sv_callback_t func);


///@name Registers handlers for important events
/**
 * @note Registers handlers for important events
 * @param[in] {type} event type 
 * @param[in] {func} callback
 * @return RESET_REASON_T
 */
extern void sdk_cb_register(CALLBACK_TYPE_T type, sdk_evt_callback_fun_t func);


///@name System restart and error logging
/**
 * @note get reset info
 * @param[in] {type} none
 * @return RESET_REASON_T
 */
extern RESET_REASON_T get_reset_reason(void);

/**
 * @note device error code set function, when device is run with something, 
 * application should use this function to story the error code to flash, when reconnect
 * to the gateway, the error code will be report to gateway.
 * @param[in] {err_code} DEV_ERROR_CODE_E: error code
 * @return true or false
 */
bool_t dev_error_code_set(DEV_ERROR_CODE_E err_code);

/**
 * @note watch dog reload.
 * @param[in] {type} none
 * @return none
 */
extern void watchdog_reload(void);

/**
 * @note systerm reset function
 * @param[in] in: none
 * @param[in] out: none
 * @return none
 */
extern void dev_sys_reset(void) ;


///@name Soft timer API
/**
 * @note system event stop function
 * @param[in] {evt} event id
 * @return none
 */
extern void dev_timer_stop(uint8_t evt);

/**
 * @note system event start with callback function
 * @param[in] {evt} event id
 * @param[in] {t} event run delay time(ms)
 * @param[in] {func} event handler function
 * @return none
 */
extern void dev_timer_start_with_callback(uint8_t evt, uint32_t t, timer_func_t func);

/**
 * @note system event start with no callback function
 * @param[in] {evt} event id
 * @param[in] {t} event run delay time(ms)
 * @return none
 */
extern void dev_timer_start(uint8_t evt, uint32_t t);

/**
 * @note system event active flag get function
 * @param[in] {evt} event id
 * @return bool_t: true is active, false is inctive
 */
extern bool_t dev_timer_get_valid_flag(uint8_t evt);

/**
 * @note The amount of milliseconds remaining before the event is
 *               scheduled to run. If the event is inactive, 0xFFFFFFFF is returned.
 * @param[in] {evt} event id
 * @return The amount of milliseconds remaining
 */
extern uint32_t dev_timer_get_remaining_time(uint8_t evt);

/**
 * @note get the current systerm millisecond ticks
 * @param[in] {type} none
 * @return current millisecond ticks
 */
extern uint32_t dev_current_millisecond_ticks_get(void);


///@name Function of system sleep wake
/**
 * @note set the unusual wakeup timeout times for sleep end device. if the wakeup time is more than the wakeupTime,
    then will call the dev_unusual_wakeup_timeout_callback function to app;
 * @param[in] {wakeupTime} wakeup time
 * @return none 
 */
extern void dev_unusual_wakeup_timeout_set(uint32_t wakeupTime);


/**
 * @note device wakeup with a time; when timeout, it will be sleep
 * @param[in] {t} wakeup time
 * @return none
 */
extern void zg_wake_up(uint32_t t); //sleep after t ms

/**
 * @note device sleep now
 * @param[in] {void} none
 * @return none
 */
extern void zg_sleep(void); //sleep

/**
 * @note system is busy
 * @param[in] {type} none
 * @return none
 */
extern bool_t zg_is_busy(void);

#ifdef __cplusplus
}
#endif

#endif


