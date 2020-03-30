/**
 * @author Leon
 * @par email:
 *      zhangpeng@tuya.com
 * @file hal_systick_timer.h
 * @brief This file abstracts all the operations of SYSTICK TIMER
 * @copyright HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @par company
 *      http://www.tuya.com
 * @date 2020-03-20 17:29:33
 */

#ifndef __HAL_SYSTICK_TIMER_H__
#define __HAL_SYSTICK_TIMER_H__

#ifdef __cplusplus
extern "C" {
#endif



typedef enum {
    HARDWARE_TIMER_AUTO_RELOAD_ENABLE = 0,  ///< auto reload time configuration
    HARDWARE_TIMER_AUTO_RELOAD_DISABLE      ///< execute once
}TIMER_RELOAD_FLAG_T;

typedef enum {
    V_TIMER0 = 0,   ///<Virtual timer id-0
    V_TIMER1,       ///<Virtual timer id-1
    V_TIMER2,
    V_TIMER3,
    V_TIMER4,
    V_TIMER5,
    V_TIMER6,
    V_TIMER7,
    V_TIMER8,
    V_TIMER9,
    V_TIMER10,
    V_TIMER11,
    V_TIMER12,
    V_TIMER13,
    V_TIMER14,
    V_TIMER15,
    V_TIMER16,
    V_TIMER17,
    V_TIMER18,
    V_TIMER19,
    V_TIMER_ERR = 0xFF
}TIMER_ID_T;

typedef void (*hardware_timer_func_t)(TIMER_ID_T);

typedef struct {
    uint32_t temp_time;
    uint32_t delay_time;
    hardware_timer_func_t func;
    TIMER_RELOAD_FLAG_T auto_reload_flag;
    uint8_t valid_flag;
    uint8_t exe_flag;
}hardware_timer_t;



//******************************************************************************
//                                                                              
//                              hardware timer api                                      
//                                                                              
//******************************************************************************
/**
 * @description: hardware timer enable
 * @param {type} none
 * @return: none
 */
extern void hardware_timer_enable(void);

/**
 * @description: hardware timer disable
 * @param {type} none
 * @return: none
 */
extern void hardware_timer_disable(void);

/**
 * @description: hardware timer start with a us timedelay
 * @param {t} times with us
 * @param {flag} reload timer or not
 * @param {func} callback function
 * @return: TIMER_ID_T: time id
 */
extern TIMER_ID_T timer_hardware_start_100us(uint32_t t, TIMER_RELOAD_FLAG_T flag, hardware_timer_func_t func);

/**
 * @description: hardware timer start with a us timedelay
 * @param {i} user define TIMER_ID_T, the used method is same with dev_timer_start_with_callback();
 * @param {t} times with us
 * @param {flag} reload timer or not
 * @param {func} callback function
 * @return: none
 */
extern void timer_hardware_start_with_id(TIMER_ID_T i, uint32_t t, TIMER_RELOAD_FLAG_T flag, hardware_timer_func_t func);

/**
 * @description: hardware timer stop
 * @param {id} time id
 * @return: none
 */
extern void timer_hardware_stop_100us(TIMER_ID_T id);


/**
 * @description: get timer active flag
 * @param {id} time id
 * @return: TRUE:active, FALSE:inactive 
 */
extern bool_t timer_hardware_is_active(TIMER_ID_T id);

#ifdef __cplusplus
}
#endif

#endif

