/*
 * @Author: jin lu
 * @email: jinlu@tuya.com
 * @Date: 2019-05-23 11:24:33
 * @file name:
 * @Description:
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 */

#ifndef __LIHGT_CONTROL_H__
#define __LIHGT_CONTROL_H__


#include "light_types.h"
#include "light_set_color.h"


#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

  /// hardware timer cycle (unit:ms)
#define HW_TIMER_CYCLE_MS           HW_TIMER_CYCLE_US/1000

/// Control calculate range 0 ~ 360
#define CTRL_HSV_H_MAX              360
/// Control calculate range 0 ~ 1000
#define CTRL_CAL_VALUE_RANGE        1000
/// Control cw calc max bright value
#define CTRL_CW_BRIGHT_VALUE_MAX    CTRL_CAL_VALUE_RANGE
/// Control cw calc min bright value (max value * 0.01)
#define CTRL_CW_BRIGHT_VALUE_MIN    (CTRL_CW_BRIGHT_VALUE_MAX * 0.01)
/// Control RGB calc max bright value
#define CTRL_RGB_BRIGHT_VALUE_MAX   CTRL_CAL_VALUE_RANGE

/// Control RGB calc max bright value
#define CTRL_RGB_BRIGHT_VALUE_MAX   CTRL_CAL_VALUE_RANGE
/// Control RGB calc min bright value
#define CTRL_RGB_BRIGHT_VALUE_MIN   (CTRL_RGB_BRIGHT_VALUE_MAX * 0.01)

/// hardware timer cycle (unit:us)
#define HW_TIMER_CYCLE_US           5000

/// light shade change cycle (unit:ms)
#define LIGHT_SHADE_CYCLE           5

/// shade change max time(the worst situation),uint:ms
#define SHADE_CHANG_MAX_TIME        1000

/// scene head(scene Num) length
#define SCNE_HRAD_LENGTH            2
/// scene unit length
#define SCENE_UNIT_LENGTH           26
/// scene max unit number
#define SCENE_MAX_UNIT              8
/// scene data min length, formu -> 2+ 26 = 28
#define SCENE_MIN_LENGTH            (SCNE_HRAD_LENGTH + SCENE_UNIT_LENGTH)
/// scene data max length, formu -> 2 + 26*8  = 210
#define SCENE_MAX_LENGTH            (SCNE_HRAD_LENGTH + SCENE_UNIT_LENGTH * SCENE_MAX_UNIT)

/// scene C default scene ctrl data
#define SCENE_DATA_DEFAULT_C        "000e0d00002e03e8000000c803e8"
/// scene CW default scene ctrl data
#define SCENE_DATA_DEFAULT_CW       "000d0d0000000000000000c80000"
/// scene RGB/RGBC/RGBCW default scene ctrl data
#define SCENE_DATA_DEFAULT_RGB      "000e0d00002e03e802cc00000000"


#define LOWPOWER_TIMER_CYCLE_MS     3000 //ms

#pragma pack(1)
/**
 * @brief Light way type enum
 *          1~5 ways
 */
typedef enum {
    LIGHT_C = 0,
    LIGHT_CW,
    LIGHT_RGB,
    LIGHT_RGBC,
    LIGHT_RGBCW,
    LIGHT_MAX,
}CTRL_LIGHT_WAY_E;

typedef enum {
    DEF_COLOR_C = 0,
    DEF_COLOR_W,
    DEF_COLOR_R,
    DEF_COLOR_G,
    DEF_COLOR_B,
    DEF_COLOR_RGB,
    DEF_COLOR_MAX,
}CTRL_DEF_COLOR_E;

/**
 * @brief Light control switch change mode enum
 *          SWITCH_GRADUAL -> turn on/off gradually
 *          SWITCH_DIRECT  -> turn on/off directly
 */
typedef enum {
    SWITCH_GRADUAL = 0,
    SWITCH_DIRECT,
    SWITCH_MAX,
}CTRL_SWITCH_MODE_E;

/**
 * @brief Light control drive mode enum
 *          BRIGHT_MODE_CW  -> CW drive by pwm totally
 *          BRIGHT_MODE_CCT -> C value is bright setting essentially , w is the scale of C&W .
 *                          the light will send warm, when w is set by zero.
 */
typedef enum {
    BRIGHT_MODE_CW = 0,
    BRIGHT_MODE_CCT,
    BRIGHT_MODE_MAX,
}CTRL_BRIGHT_MODE_E;

/**
 * @brief Light control scene change mode enum
 *          SCENE_STATIC    -> scene hold on no change
 *          SCENE_JUMP      -> scene change by directly
 *          SCENE_SHADOW    -> scene change by gradually
 */
typedef enum {
    SCENE_STATIC = 0,
    SCENE_JUMP,
    SCENE_SHADE,
    SCENE_MAX,
}CTRL_SCENE_MODE_E;

typedef enum {
    RESET_MOD_CNT = 0,
}CTRL_RESET_MODE_E;             /* this mode can't choose in bulb! */

/**
 * @brief Light control need configuration sturct
 * the configuration need to set firstly ,when use the control proccess
 */
typedef struct
{
    CTRL_LIGHT_WAY_E    LightWay;
    CTRL_DEF_COLOR_E    NetColor;
    USHORT_T            usNetBright;    /* blink bright in connect proc */
    USHORT_T            usNetTemper;    /* blink temper in connect proc */
    CTRL_DEF_COLOR_E    DefColor;
    USHORT_T            usDefBright;    /* default bright */
    USHORT_T            usDefTemper;    /* default temper */
    CTRL_SWITCH_MODE_E  SwitchMode;     /* turn on/off mode */
    CTRL_BRIGHT_MODE_E  BrightMode;     /* CCT&CW drive mode */
    UCHAR_T             ucPowerGain;    /* power gain(amplification) */
    UCHAR_T             ucLimitCWMax;   /* CW limit Max value */
    UCHAR_T             ucLimitCWMin;   /* CW limit Min value */
    UCHAR_T             ucLimitRGBMax;
    UCHAR_T             ucLimitRGBMin;
    UCHAR_T             ucResetCnt;     /* re distibute cnt */
    UCHAR_T             ucConnectMode;  /* connect cfg mode */
    BOOL_T              bmemory;        /* ifnot save flag  */
}light_ctrl_cfg_t;

/**
 * @brief Light control data structure
 * storage the light control data(normal issued by app)
 */
typedef enum
{
    WHITE_MODE = 0,
    COLOR_MODE,
    SCENE_MODE,
    MUSIC_MODE,
    MODE_MAX,
}LIGHT_MODE_E;

/**
 * @brief Light control color(RGB) data structure
 * storage the light control color(RGB) data(normal issued by app)
 */
typedef struct
{
    USHORT_T usRed;         /* color R setting */
    USHORT_T usGreen;
    USHORT_T usBlue;
}COLOR_RGB_T;



/**
 * @brief Light control color(HSV) data structure
 * storage the light control color(HSV) data(normal issued by app)
 */
typedef struct
{
    USHORT_T usHue;         /* color H setting */
    USHORT_T usSaturation;  /* color S setting */
    USHORT_T usValue;       /* color V setting */
}COLOR_HSV_T;

/**
 * @brief Light control color control original data structure
 */
typedef struct {
    USHORT_T usColorData1;
    USHORT_T usColorData2;
    USHORT_T usColorData3;
    //UCHAR_T ucColorStr[13];     /* HSV format hex */
}COLOR_ORIGIN_T;

/**
 * @brief Light control real time control change mode enum
 */
typedef enum {
    REALTIME_CHANGE_JUMP = 0,
    REALTIME_CHANGE_SHADE,
}REALTIME_CHANGE_E;

/**
 * @brief Light control data structure
 * storage the light control data(normal issued by app)
 */
typedef struct
{
    BOOL_T bSwitch;             /* on off setting */
    LIGHT_MODE_E Mode;
    USHORT_T usBright;
    USHORT_T usTemper;
    COLOR_RGB_T Color;
    COLOR_ORIGIN_T ColorOrigin;
    UCHAR_T ucScene[SCENE_MAX_LENGTH + 1];
    BOOL_T bSceneFirstSet;
    UINT_T ulCountDown;
    UCHAR_T ucRealTimeData[22];
    UCHAR_T ucRealTimeFlag;
}LIGHT_CTRL_DATA_T;


typedef struct
{
    CTRL_SCENE_MODE_E ChangeMode;
    UINT_T uiSpeed;
    UINT_T uiTimes;    /* unit:ms */
}LIGHT_SCENE_CTRL_T;


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
}BRIGHT_DATA_T;

/**
 * @brief Light gradually change structure
 * Used in light gradually change calculation process
 */
typedef struct
{
    BRIGHT_DATA_T TargetVal;
    BRIGHT_DATA_T CurrVal;
    BOOL_T bFirstChange;    /* first change flag, need to calculate change step */
    UCHAR_T ucSceneUnit;
    UCHAR_T ucUnitCnt;
    BOOL_T bSceneSetFirstFlag;
    BOOL_T bSceneStopFlag;
}LIGHT_CTRL_HANDLE_T;

#pragma pack()


VOID vLightCtrlDataCalcCW(IN USHORT_T Bright, IN USHORT_T Temper, OUT BRIGHT_DATA_T *Result);
VOID vLightSetResetHandleData(IN BRIGHT_DATA_T data);
/**
 * @berief: set light ctrl data to default according lightCfgData
 * @param {none}
 * @retval: none
 */
VOID vLightCtrlDataReset(VOID);

/**
 * @berief: Light system control hardware timer callback
 * @param {none}
 * @attention: this function need to implement by system,
 *              decide how to call vLightCtrlShadeGradually function.
 * @retval: none
 */
VOID vLightSysHWTimerCB(VOID);

/**
 * @description: light system callback of reset count clear
 * @param {type}
 * @return:
 */
VOID vLightSysResetCntClearCallback(VOID);
/**
 * @berief: Light control hardware timer callback
 * @param {none}
 * @attention: vLightSysHWTimerCB() func need to implement by system
 * @retval: none
 */
VOID vLightCtrlHWTimerCB(VOID);


/**
 * @berief: reponse switch property process,
 *          this need to implement by system.
 * @param {OUT BOOL_T bONOFF -> switch status, TRUE is ON}
 * @retval: none
 */
VOID vLightCtrlDataSwitchRespone(OUT BOOL_T bONOFF);

/**
 * @berief: set light switch data, adapte control data issued by system
 *          to control data format.
 * @param {IN BOOL_T bONOFF -> switch data, TRUE will turn on}
 * @retval: OPERATE_RET -> OPRT_OK meaning need to call opLightCtrlProc() function!
 */
OPERATE_RET opLightCtrlDataSwitchSet(IN BOOL_T bONOFF);

/**
 * @berief: reponse mode property process,
 *          this need to implement by system.
 * @param {OUT LIGHT_MODE_E Mode}
 * @retval: none
 */
VOID vLightCtrlDataModeResponse(OUT LIGHT_MODE_E Mode);

/**
 * @berief: set light mode data
 * @param {IN LIGHT_MODE_E Mode}
 * @attention:Mode value is below:
 *                                  WHITE_MODE = 0,
 *                                  COLOR_MODE = 1,
 *                                  SCENE_MODE = 2,
 *                                  MUSIC_MODE = 3,
 * @retval: OPERATE_RET -> OPRT_OK meaning need to call opLightCtrlProc() function!
 */
OPERATE_RET opLightCtrlDataModeSet(IN LIGHT_MODE_E Mode);

/**
 * @berief: reponse bright property process,
 *          this need to implement by system.
 * @param {OUT LIGHT_MODE_E Mode}
 * @param {OUT USHORT_T usBright}
 * @attention: need reponse mode property,as set bright value, will auto set the Mode to WHITE_MODE!
 * @retval: none
 */
VOID vLightCtrlDataBrightResponse(OUT LIGHT_MODE_E Mode, OUT USHORT_T usBright);

/**
 * @berief: set light bright data, adapte control data issued by system
 *          to control data format.
 * @param {IN USHORT_T usBright}
 * @attention: acceptable range:10~1000
 * @attention: set bright value, will auto set the Mode to WHITE_MODE !
 * @retval: OPERATE_RET -> OPRT_OK meaning need to call opLightCtrlProc() function!
 */
OPERATE_RET opLightCtrlDataBrightSet(IN USHORT_T usBright);

/**
 * @berief: reponse temperature property process,
 *          this need to implement by system.
 * @param {OUT LIGHT_MODE_E Mode}
 * @param {OUT USHORT_T usTemperature}
 * @attention: need reponse mode property,as set temperature value, will auto set the Mode to WHITE_MODE!
 * @retval: none
 */
VOID vLightCtrlDataTemperatureResponse(OUT LIGHT_MODE_E Mode, OUT USHORT_T usTemperature);

/**
 * @berief: set light temrperature data, adapte control data issued by system
 *          to control data format.
 * @param {IN USHORT_T usTemperature}
 * @attention: acceptable range:0~1000
 * @retval: OPERATE_RET -> OPRT_OK meaning need to call opLightCtrlProc() function!
 */
OPERATE_RET opLightCtrlDataTemperatureSet(IN USHORT_T Temperature);


OPERATE_RET opLightCtrlDataHSVSet(IN COLOR_HSV_T *Color);

/**
 * @berief: get light switch data
 * @param {OUT BOOL_T *onpONOFFoff -> switch data return}
 * @retval: OPERATE_RET
 */
OPERATE_RET opLightCtrlDataSwitchGet(OUT BOOL_T *onoff);

/**
 * @berief: geta light bright data
 * @param {OUT USHORT_T *Bright -> bright data return}
 * @retval: OPERATE_RET
 */
OPERATE_RET opLightCtrlDataBrightGet(OUT USHORT_T *Bright);

/**
 * @berief: get light temrperature data
 * @param {OUT USHORT_T *Temperature -> temperature data return}
 * @retval: OPERATE_RET
 */
OPERATE_RET opLightCtrlDataTemperatureGet(OUT USHORT_T *Temperature);

/**
 * @berief: get light mode data
 * @param {OUT LIGHT_MODE_E *Mode -> mode data return}
 * @retval: OPERATE_RET
 */
OPERATE_RET opLightCtrlDataModeGet(OUT LIGHT_MODE_E *Mode);

/**
 * @berief: get light RGB data
 * @param {OUT COLOR_ORIGIN_T *Color -> color original data return}
 * @retval: OPERATE_RET
 */
OPERATE_RET opLightCtrlDataRGBGet(OUT COLOR_RGB_T *rgb_data, OUT COLOR_ORIGIN_T *origin_data);
/**
 * @berief: get control ctrl data
 * @param {none}
 * @retval: LIGHT_CTRL_DATA_T
 */
LIGHT_CTRL_DATA_T opLightCtrlDataGet(VOID);

/**
 * @berief: Light control proc
 * @param {none}
 * @retval: OPERATE_RET
 */
OPERATE_RET opLightCtrlProc(VOID);


/**
 * @berief: countdown lave time return proc
 * @param {OUT UINT_T RemainTimeSec -> left time,unit:sec}
 * @attention: this function need to implement in system
 * @retval: none
 */
VOID vLightDataCountDownResponse(OUT UINT_T RemainTimeSec);

/**
 * @berief: set light countdown value
 * @param {IN INT_T CountDownSec -> unit:second}
 * @attention: countdown lave time will return with
 *              calling vLightDataCountDownResponse function every minutes.
 *              switch status will return with calling
 *              vLightCtrlDataSwitchRespone function when countdown active.
 * @retval: OPERATE_RET -> OPRT_OK set countdown OK.
 */
OPERATE_RET opLightCtrlDataCountDownSet(IN INT_T CountDownSec);

/**
 * @berief: system reboot as hardware mode jude proc
 * @param {none}
 * @attention: this function need to implement by each plantform.
 * @retval: BOOL_T TRUE -> system reboot
 */
BOOL_T bLightSysHWRebootJude(VOID);

/**
 * @berief: Light hardware reboot judge & proc
 *          process detail:
 *                  1. hardware reset judge;
 *                  2. load reboot cnt data;
 *                  3. reboot cnt data increase;
 *                  4. start software time to clear reboot cnt;
 * @param {none}
 * @retval: none
 */
VOID vLightCtrlHWRebootProc(VOID);

/**
 * @berief: system reset proc
 * @param {none}
 * @attention: this function need implememt by system,
 *              need to deal with different thing in each plantform.
 * @retval: none
 */
OPERATE_RET opLightSysResetCntOverCB(VOID);

/**
 * @berief: Light reset to re-distribute proc
 * @param {none}
 * @attention: this func will call opLightSysResetCntOverCB()
 *              opLightSysResetCntOverCB() need to implement by system
 * @retval: OPERATE_RET
 */
OPERATE_RET opLightCtrlResetCntProcess(VOID);

/**
 * @description: start blink
 * @param {IN UINT_T BlinkTimeMs -> blink phase}
 * @attention: blink display will as the parm
 *             -- NetColor, usNetBright in configuration.
 * @retval: none
 */
OPERATE_RET opLightCtrlBlinkStart(IN UINT_T BlinkTimeMs);

/**
 * @description: stop blink
 * @param {type} none
 * @attention: blink stop will directly go to normal status display
 *              normal status will bright as default bright parm
 *              -- usDefBright,usDefTemper,DefColor in configuration.
 * @retval: none
 */
OPERATE_RET opLightCtrlBlinkStop(VOID);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif  /* __LIHGT_CONTROL_H__ */
