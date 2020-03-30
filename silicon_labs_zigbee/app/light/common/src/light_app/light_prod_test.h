/*
 * @Author: wls
 * @email: wuls@tuya.com
 * @LastEditors: wls
 * @file name: light_prod_test.h
 * @Description: light production test include file
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-04-26 13:55:40
 * @LastEditTime: 2019-05-08 16:17:44
 */

#ifndef __LIHGT_PROD_TEST_H__
#define __LIHGT_PROD_TEST_H__


#include ".h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/// Weak signal breath display change time(unit ms)
#define WEAK_SINGAL_BREATHR_TIME_INTERVAL           1500
/// Weak signal on/off display change time(unit ms)
#define WEAK_SINGAL_ONOFF_TIME_INTERVAL             250
/// Weak signal on/off blink cnt
#define WEAK_SINGAL_ONOFF_CNT                       2


/// Production detected signal weak judge value
#define AUZ_TEST_WEAK_SIGNAL                        -60 
/// Production unauthorized display on/off time(unit ms)
#define AUZ_TEST_FAIL_TIME_INTERVAL                 3000    


/// Productoin oeverall function single led blink time(unit ms)
#define PORD_CHECK_ONOF_TIMER_INTERVAL              1000
/// Production oberall fuction test time(unit ms)
#define PROD_CHECK_TIMER_INTERVAL                   (2*60*1000)


/// Production aging restart signle led blink timer(unit ms)
#define PROD_AGING_START_ONOFF_TIMER_INTERVAL       500
/// Production aging restart loop cnt()
#define PROD_AGING_START_LOOP_CNT                   5


/// Production repeat test signle led up/down timer(unit ms)
#define PROD_REPEAT_ONOFF_TIMER_INTERVAL        1000


/**
 * @berief: prodution test process init
 * @param {
 *         CHAR_T cSignal_strength -> singal strength } 
 * @return: none
 * @retval: none
 */
VOID vLightProd_Init(VOID);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif  /* __LIHGT_PROD_TEST_H__ */
