/**
 * @author Leon
 * @par email:
 *      zhangpeng@tuya.com
 * @file hal_timer.h
 * @brief This file abstracts all the operations of TIMER
 * @copyright HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @par company
 *      http://www.tuya.com
 * @date 2020-03-20 17:29:33
 */

#ifndef __HAL_TIMER_H__
#define __HAL_TIMER_H__

#include "type_def.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @remarks TIMER interupt flag
 */
#define INT_FLAG_ICBOF3  (0x80UL)
#define INT_FLAG_ICBOF2  (0x400UL)
#define INT_FLAG_ICBOF1  (0x200UL)
#define INT_FLAG_ICBOF0  (0x100UL)
#define INT_FLAG_CC3     (0x80UL)
#define INT_FLAG_CC2     (0x40UL)
#define INT_FLAG_CC1     (0x20UL)
#define INT_FLAG_CC0     (0x10UL)
#define INT_FLAG_UF      (0x02UL)
#define INT_FLAG_OF      (0x01UL)

/**
 * @note Call this function when the time is up
 */
typedef void (*timer1_int_callback_t)(uint32_t int_flag);

/**
 * @enum Hardware Timer Compare Channel
 */
typedef enum {
    TIMER1_CC0 = 0, ///< channel 0
    TIMER1_CC1 = 1, ///< channel 1
    TIMER1_CC2 = 2, ///< channel 2
    TIMER1_CC3 = 3, ///< channel 3
}TIMER1_CC_T;

/**
 * @enum Hardware Timer Frequency division
*/
typedef enum {
    TIMER_DIV1 = 0,     /**< Divide by 1. */
    TIMER_DIV2,         /**< Divide by 2. */
    TIMER_DIV4,         /**< Divide by 4. */
    TIMER_DIV8,         /**< Divide by 8. */
    TIMER_DIV16,        /**< Divide by 16. */
    TIMER_DIV32,        /**< Divide by 32. */
    TIMER_DIV64,        /**< Divide by 64. */
    TIMER_DIV128,       /**< Divide by 128. */
    TIMER_DIV256,       /**< Divide by 256. */
    TIMER_DIV512,       /**< Divide by 512. */
    TIMER_DIV1024,      /**< Divide by 1024. */
}TIMER_DIV_T;

/**
 * @note timer1 init
 * @param[in] {enable} init completed enable
 * @param[in] {prescale} timer prescale
 * @param[in] {func} Call this function when the time is up
 * @return none
 */
extern void dev_timer1_init(bool_t enable, TIMER_DIV_T prescale, timer1_int_callback_t func);

/**
 * @note get timer1 count value
 * @param none
 * @return none
 */
extern uint32_t dev_timer1_counter_get(void);


/**
 * @note clear timer1 count value
 * @param none
 * @return none
 */
extern void dev_timer1_counter_reset(void);

/**
 * @note timer1 overflow interrupt enable/disable
 * @param[in] {int_enable} TRUE:enable, FALSE:disable
 * @return none
 */
extern void dev_timer1_overflow_int_set(bool_t int_enable);


/**
 * @note set timer top value
 * @param[in] {us} microseconds
 * @return none
 */
extern void dev_timer1_top_set(uint32_t us);

/**
 * @note set timer top buff value,will ato updated in next wrap around
 * @param[in] {us} microseconds
 * @return none
 */
extern void dev_timer1_top_next_set(uint32_t us);

/**
 * @note init timer1 compare mode and enable compare interrupt
 * @param[in] {ch} compare channel
 * @return none
 */
extern void dev_timer1_cc_init(TIMER1_CC_T ch);


/**
 * @note default init timer capture/compare and disable interrupt
 * @param[in] {ch} TIMER CC
 * @return none
 */
extern void dev_timer1_cc_deinit(TIMER1_CC_T ch);

/**
 * @note compare interrupt enable/disable
 * @param[in] {ch} TIMER CC
 * @param[in] {int_enable} TRUE:enable, FALSE:disable
 * @return none
 */
extern void dev_timer1_cc_int_set(TIMER1_CC_T ch, bool_t int_enable);

/**
 * @note set compare value
 * @param[in] {ch} channel
 * @param[in] {us} microseconds
 * @return none
 */
extern void dev_timer1_compare_counter_set(TIMER1_CC_T ch, uint32_t us);

/**
 * @note set timer compare buff value,will ato updated in next wrap around
 * @param[in] {ch} channel
 * @param[in] {us} microseconds
 * @return none
 */
extern void dev_timer1_compare_counter_buf_set(TIMER1_CC_T ch, uint32_t us);

/**
 * @note start timer1
 * @param none
 * @return none
 */
extern void dev_timer1_start(void);

/**
 * @note stop timer1
 * @param none
 * @return none
 */
extern void dev_timer1_stop(void);

/**
 * @note reset timer1
 * @param none
 * @return none
 */
extern void dev_timer1_reset(void);

/**
 * @note get timer1 start flag
 * @param none
 * @return none
 */
extern bool_t dev_timer1_started(void);




#ifdef __cplusplus
}
#endif

#endif




