/*
 * @Author: wls
 * @email: wuls@tuya.com
 * @LastEditors: wls
 * @file name: light_control.h
 * @Description: light control include file
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-04-26 13:55:40
 * @LastEditTime: 2019-05-21 10:13:39
 */

#ifndef __LIHGT_CONTROL_H__
#define __LIHGT_CONTROL_H__


#include "light_types.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * @brief Light control drive mode enum 
 * Light
 *      DRIVE_CW  -> CW drive by pwm totally
 *      DRIVE_CCT -> C value is bright setting essentially , w is the scale of C&W .
 *                   the light will send warm, when w is set by zero.
 */
typedef enum {
    DRIVE_CW = 0,
    DRIVE_CCT,
    DRIVE_MAX,
}CTRL_DRIVE_E;

/**
 * @brief Light control switch change mode enum 
 * Light
 *      SWITCH_GRADUAL -> turn on/off gradually
 *      SWITCH_DIRECT  -> turn on/off directly
 */
typedef enum {
    SWITCH_GRADUAL = 0;
    SWITCH_DIRECT,
    SWITCH_MAX,
}CTRL_SWITCH_MODE_E;

/**
 * @brief Light control scene change mode enum 
 * Light
 *      SCENE_STATIC    -> scene hold on no change 
 *      SCENE_JUMP      -> scene change by directly
 *      SCENE_SHADOW    -> scene change by gradually 
 */
typedef enum {
    SCENE_STATIC = 0;
    SCENE_JUMP,
    SCENE_SHADOW,
    SCENE_MAX,
}CTRL_SCENE_MODE_E;

/// control power gain (100 --> gain )
#define CTRL_POEWER_GAIN_MIN        100 
/// control power gain (100 --> gain )   
#define CTRL_POEWER_GAIN_MIN        200    

/**
 * @brief Light control need configuration sturct
 * the configuration need to set firstly ,when use the control proccess 
 */
typedef struct 
{
    CTRL_DRIVE_E        Drive_mode;     /* CCT&CW drive mode */
    CTRL_SWITCH_MODE_E  Switch_mode;    /* turn on/off mode */
    CTRL_SCENE_MODE_E   Scene_mode;     /* scene change mode */
    UCHAR_T             ucPowerGain;    /* power gain(amplification) */
    UCHAR_T             ucLimitCWMax;   /* CW limit Max value */
    UCHAR_T             ucLimitCWMin;   /* CW limit Min value */
    
    
}LIGHT_CTRL_CFG_S;


/**
 * @brief Light control data structure
 * storage the light control data(normal issued by app)
 */
typedef enum
{
    WHITE_MODE = 0,
    COLOR_MODE ,
    SECEN_MODE ,
    MUSIC_MODE,
}LIGHT_MODE_E;

/**
 * @brief Light control data structure
 * storage the light control data(normal issued by app)
 */
typedef struct 
{
    BOOL_T bPower;      /* data */
    LIGHT_MODE_E Mode;
    USHORT_T usBright;
    USHORT_T usTemper;
    UCHAR_T ucColor[];
    UCHAR_T ucScene[];
    UINT_T ulCountDown;                     
}LIGHT_CTRL_DATA_S;

/**
 * @brief Bright 5ways value structure
 * Used in light gradually change calculation process
 */
typedef struct 
{
    USHORT_T usRed;
    USHORT_T usGreen;
    USHORT_T usBlue;
    USHORT_T usWhite;
    USHORT_T usWarm;
}BRIGHT_DATA_S;

/**
 * @brief Light gradually change structure
 * Used in light gradually change calculation process
 */
typedef struct 
{
    BRIGHT_DATA_S TargetVal;
    BRIGHT_DATA_S CurrVal;

    BOOL_T bFirstChange;    /* first change flag, need to calculate change step */
    /* data */
}LIGHT_CTRL_HANDLE_S;

/**
 * @berief: light control init 
 * @param {LIGHT_CTRL_CFG_S Config_Data} 
 * @return: none
 * @retval: none
 */
OPERATE_RET opLightCtrl_Init(LIGHT_CTRL_CFG_S Config_Data);


/**
 * @berief: get current light power ctrl data 
 * @param {none} 
 * @return: BOOL_T 
 * @retval: bPower
 */
BOOL_T bGetLightCtrlData_Power(VOID);



#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif  /* __LIHGT_CONTROL_H__ */
