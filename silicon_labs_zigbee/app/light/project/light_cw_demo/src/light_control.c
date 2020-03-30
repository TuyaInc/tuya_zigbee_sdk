/*
 * @file name: light_control.c
 * @Description: light control process
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-04-28 14:35:54
 * @LastEditTime: 2019-12-03 20:22:27
 */

#include "light_control.h"
#include "light_types.h"
#include "user_flash.h"
#include "user_timer.h"
#include "tuya_zigbee_sdk.h"



/// control power gain (100 --> gain = 1 times )
#define CTRL_POEWER_GAIN_MIN        100
/// control power gain (200 --> gain = 2 times )
#define CTRL_POEWER_GAIN_MAX        200

/// control CW&RGB bright limit max
#define CTRL_BRIGHT_LIMIT_MAX       100
/// control CW&RGB bright limit min
#define CTRL_BRIGHT_LIMIT_MIN       0

/// control CW&RGB temper limit max
#define CTRL_TEMPER_LIMIT_MAX       100
/// control CW&RGB temper limit min
#define CTRL_TEMPER_LIMIT_MIN       0


#define SHADE_STEP_GAIN_DEFAULT     (CTRL_CAL_VALUE_RANGE * LIGHT_SHADE_CYCLE / SHADE_CHANG_MAX_TIME)

#define BREATH_TIMER_CYCLE_MS        10 //ms
#define BREATH_STEP_VALUE            CTRL_CAL_VALUE_RANGE/(uiBreathTimeMs/BREATH_TIMER_CYCLE_MS)

/**
 * @brief software timer use id enum
 */
typedef enum {
    CLEAR_RESET_CNT_SW_TIMER = 0,
    BLINK_SW_TIMER = 1,
    SCENE_SW_TIMER = 2 ,
    AUTOSAVE_SW_TIMER = 3,
    BREATH_SW_TIMER = 4,
    COUNTDOWN_SW_TIMER = 5,
    LOWPOWER_SW_TIMER = 6,
}SW_TIMER_ID_E;

/**
 * @brief hardware timer drive actively control structure
 */
typedef struct
{
    UINT_T uiTargetCnt;     /* shade time per one change */
    UINT_T uiCnt;           /* hardware time */
    BOOL_T bEnable;         /* hardware time deal with shade change flag */
}HW_TIMER_PARAM_S;

/// hardware time set param
STATIC HW_TIMER_PARAM_S HWTimerParam = {
    .uiTargetCnt = 0xFFFFFF,
    .uiCnt = 0,
    .bEnable = FALSE,
};


/// light ctrl configuration
light_ctrl_cfg_t LightCfgData;
/// light ctrl data(status)
STATIC LIGHT_CTRL_DATA_T LightCtrlData;

STATIC UINT_T uiBlinkTimeMs = 0;

extern OPERATE_RET opUserFlashWriteResetCnt(IN UCHAR_T data);

UINT_T vLightGetABSValue(IN INT_T value)
{
    return (value > 0 ? value : -value);
}
/**
 * @berief: set light ctrl data to default according lightCfgData
 * @param {none}
 * @retval: none
 */
VOID vLightCtrlDataReset(VOID)
{

    memset(&LightCtrlData, 0, SIZEOF(LIGHT_CTRL_DATA_T));

    LightCtrlData.bSwitch = TRUE;
    LightCtrlData.Mode = WHITE_MODE;

    LightCtrlData.usBright = 1000;
    LightCtrlData.usTemper = 1000;
}


/**
 * @berief: Light system control hardware timer callback
 * @param {none}
 * @attention: this function need to implement by system,
 *              decide how to call vLightCtrlShadeGradually function.
 * @retval: none
 */
WEAK VOID vLightSysHWTimerCB(VOID)
{
    app_print("shade tick 5ms...");
}


WEAK VOID vLightSysResetCntClearCallback(VOID)
{

}

/**
 * @berief: Light control hardware timer callback
 * @param {none}
 * @attention: vLightSysHWTimerCB() func need to implement by system
 * @retval: none
 */
VOID vLightCtrlHWTimerCB(VOID)
{
    if(HWTimerParam.bEnable != TRUE) {
        return;
    }

    HWTimerParam.uiCnt += HW_TIMER_CYCLE_MS;
    if(HWTimerParam.uiCnt >= HWTimerParam.uiTargetCnt) {
        vLightSysHWTimerCB();
        HWTimerParam.uiCnt = 0;
    }
}

/**
 * @berief: CW bright output limit process
 * @param {IN USHORT_T Bright -> bright value}
 * @retval: USHORT_T usResult
 */
STATIC USHORT_T usLightCtrlDataCWLimit(IN USHORT_T Bright)
{
    USHORT_T usMax = 0, usMin = 0, usResult = 0;

    usMax = CTRL_CW_BRIGHT_VALUE_MAX;
    usMin = 10;

    //app_print("max %d min %d",usMax,usMin);

    usResult = ( Bright - CTRL_CW_BRIGHT_VALUE_MIN ) * ( usMax - usMin ) / \
                ( CTRL_CW_BRIGHT_VALUE_MAX - CTRL_CW_BRIGHT_VALUE_MIN ) + usMin;

    return(usResult);
}

/**
 * @berief: calculate the CW Light_Handle.TargetVal according to
 *          the Light_Data value, and cw output limit and amplify process!
 * @param {IN USHORT_T Bright -> Light_Data bright }
 * @param {IN USHORT_T Temper -> Light_Data temperature}
 * @param {IN BRIGHT_DATA_T *Result -> calc result}
 * @retval: none
 */
VOID vLightCtrlDataCalcCW(IN USHORT_T Bright, IN USHORT_T Temper, OUT BRIGHT_DATA_T *Result)
{
    USHORT_T usBrightTemp = 0;

    //app_print("Bright %d",Bright);

    if(Bright < CTRL_CW_BRIGHT_VALUE_MIN) {
        usBrightTemp = 0;
    } else {
        //app_print("cw limit proc");
        usBrightTemp = usLightCtrlDataCWLimit(Bright);     /* limit CW bright output limit */
    }
    usBrightTemp = (USHORT_T) (((FLOAT_T)( 100 / CTRL_POEWER_GAIN_MIN )) * usBrightTemp );     /* power amplification */
    Result ->usWhite = usBrightTemp * Temper / CTRL_CAL_VALUE_RANGE;
    Result ->usWarm = usBrightTemp - Result ->usWhite;

    /* output max limit --  power amplification, convert overflow power to another  */
    if(Result ->usWhite > CTRL_CAL_VALUE_RANGE) {
        Result ->usWarm = (USHORT_T)( Result ->usWarm * ( (FLOAT_T) ( Result ->usWhite / CTRL_CAL_VALUE_RANGE )) );
        Result ->usWhite = CTRL_CAL_VALUE_RANGE;
    }
    if( Result ->usWarm > CTRL_CAL_VALUE_RANGE ) {
        Result ->usWhite = (USHORT_T)( Result ->usWhite * ( (FLOAT_T) ( Result ->usWarm / CTRL_CAL_VALUE_RANGE )) );
        Result ->usWarm = CTRL_CAL_VALUE_RANGE;
    }
}

/**
 * @berief: reponse switch property process,
 *          this need to implement by system.
 * @param {OUT BOOL_T bONOFF -> switch status, TRUE is ON}
 * @retval: none
 */
WEAK VOID vLightCtrlDataSwitchRespone(OUT BOOL_T bONOFF)
{
    ;
}

/**
 * @berief: set light switch data, adapte control data issued by system
 *          to control data format.
 * @param {IN BOOL_T bONOFF -> switch data, TRUE will turn on}
 * @retval: OPERATE_RET -> OPRT_OK meaning need to call opLightCtrlProc() function!
 */
OPERATE_RET opLightCtrlDataSwitchSet(IN BOOL_T bONOFF)
{
    BOOL_T bLastStatus ;

    bLastStatus = LightCtrlData.bSwitch;

    if( TRUE == bONOFF ) {
        LightCtrlData.bSwitch = TRUE;
    } else {
        LightCtrlData.bSwitch = FALSE;
    }

    vLightCtrlDataSwitchRespone(bONOFF);      /* reponse property */

    if(bONOFF == bLastStatus) {
        app_print("the same switch set");
        return OPRT_INVALID_PARM;
    }

    return OPRT_OK;
}

/**
 * @berief: reponse mode property process,
 *          this need to implement by system.
 * @param {OUT LIGHT_MODE_E Mode}
 * @retval: none
 */
WEAK VOID vLightCtrlDataModeResponse(OUT LIGHT_MODE_E Mode)
{
    ;
}

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
OPERATE_RET opLightCtrlDataModeSet(IN LIGHT_MODE_E Mode)
{
    LIGHT_MODE_E LastMode ;

    LastMode = LightCtrlData.Mode;

    if( Mode > MODE_MAX) {
        app_print("mode is illegal,set error");
        return OPRT_INVALID_PARM;
    }

    LightCtrlData.Mode = Mode;
    vLightCtrlDataModeResponse(Mode);

    if((Mode == LastMode)&&(Mode != SCENE_MODE)) {      /* only scene mode can be set many time */
        app_print("the same mode set");
        return OPRT_INVALID_PARM;
    }
    if((Mode == SCENE_MODE) && (LightCtrlData.ucRealTimeFlag == TRUE)){
        app_print("this scene mode don't need proc!");
        LightCtrlData.ucRealTimeFlag = FALSE;
        return OPRT_INVALID_PARM;
    }

    if(SCENE_MODE == LightCtrlData.Mode) {              /* restart scene mode */
        LightCtrlData.bSceneFirstSet = TRUE;
    }

    return OPRT_OK;
}

/**
 * @berief: reponse bright property process,
 *          this need to implement by system.
 * @param {OUT LIGHT_MODE_E Mode}
 * @param {OUT USHORT_T usBright}
 * @attention: need reponse mode property,as set bright value, will auto set the Mode to WHITE_MODE!
 * @retval: none
 */
WEAK VOID vLightCtrlDataBrightResponse(OUT LIGHT_MODE_E Mode, OUT USHORT_T usBright)
{
    ;
}

/**
 * @berief: set light bright data, adapte control data issued by system
 *          to control data format.
 * @param {IN USHORT_T usBright}
 * @attention: acceptable range:10~1000
 * @attention: set bright value, will auto set the Mode to WHITE_MODE !
 * @retval: OPERATE_RET -> OPRT_OK meaning need to call opLightCtrlProc() function!
 */
OPERATE_RET opLightCtrlDataBrightSet(IN USHORT_T usBright)
{
    USHORT_T usLastBright;

    usLastBright = LightCtrlData.usBright;

    if(usBright < CTRL_CW_BRIGHT_VALUE_MIN) {
        app_print("bright value is exceed range,set error");
        return OPRT_INVALID_PARM;
    }

    if(usBright > CTRL_CW_BRIGHT_VALUE_MAX) {
        app_print("bright value is exceed range,set error");
        return OPRT_INVALID_PARM;
    }

    LightCtrlData.usBright = usBright;
    LightCtrlData.Mode = WHITE_MODE;    /* change mode to white mode forcibly */

    vLightCtrlDataBrightResponse(LightCtrlData.Mode, usBright);

    if(TRUE == LightCtrlData.ucRealTimeFlag) {
        app_print("this commad need to save and proc!");
        LightCtrlData.ucRealTimeFlag = FALSE;
    } else {
        if(usBright == usLastBright) {
            app_print("the same bright set");
            return OPRT_INVALID_PARM;
        }
    }

    return OPRT_OK;
}

/**
 * @berief: reponse temperature property process,
 *          this need to implement by system.
 * @param {OUT LIGHT_MODE_E Mode}
 * @param {OUT USHORT_T usTemperature}
 * @attention: need reponse mode property,as set temperature value, will auto set the Mode to WHITE_MODE!
 * @retval: none
 */
WEAK VOID vLightCtrlDataTemperatureResponse(OUT LIGHT_MODE_E Mode, OUT USHORT_T usTemperature)
{
    ;
}

/**
 * @berief: set light temrperature data, adapte control data issued by system
 *          to control data format.
 * @param {IN USHORT_T usTemperature}
 * @attention: acceptable range:0~1000
 * @retval: OPERATE_RET -> OPRT_OK meaning need to call opLightCtrlProc() function!
 */
OPERATE_RET opLightCtrlDataTemperatureSet(IN USHORT_T usTemperature)
{
    USHORT_T usLastTemperature;

    usLastTemperature = LightCtrlData.usTemper;

    if(usTemperature > CTRL_CW_BRIGHT_VALUE_MAX) {
        app_print("temperature value is exceed range,set error");
        return OPRT_INVALID_PARM;
    }

    LightCtrlData.usTemper = usTemperature;
    LightCtrlData.Mode = WHITE_MODE;    /* change mode to white mode forcibly */

    vLightCtrlDataTemperatureResponse(LightCtrlData.Mode, usTemperature);

    if(TRUE == LightCtrlData.ucRealTimeFlag) {
        app_print("this commad need to save and proc!");
        LightCtrlData.ucRealTimeFlag = FALSE;
    } else {
    if(usTemperature == usLastTemperature) {
        app_print("the same temperature set");
        return OPRT_INVALID_PARM;
        }
    }

    return OPRT_OK;
}
/**
 * @berief: get light switch data
 * @param {OUT BOOL_T *onpONOFFoff -> switch data return}
 * @retval: OPERATE_RET
 */
OPERATE_RET opLightCtrlDataSwitchGet(OUT BOOL_T *pONOFF)
{
    *pONOFF = LightCtrlData.bSwitch;

    return OPRT_OK;
}

/**
 * @berief: geta light bright data
 * @param {OUT USHORT_T *pBright -> bright data return}
 * @retval: OOPERATE_RET
 */
OPERATE_RET opLightCtrlDataBrightGet(OUT USHORT_T *pBright)
{

    *pBright = LightCtrlData.usBright;

    return OPRT_OK;
}

/**
 * @berief: get light temrperature data
 * @param {OUT USHORT_T *pTemperature -> temperature data return}
 * @retval: OPERATE_RET
 */
OPERATE_RET opLightCtrlDataTemperatureGet(OUT USHORT_T *pTemperature)
{
    *pTemperature = LightCtrlData.usTemper;

    return OPRT_OK;
}
/**
 * @berief: get light RGB data
 * @param {OUT COLOR_ORIGIN_T *Color -> color original data return}
 * @retval: OPERATE_RET
 */
OPERATE_RET opLightCtrlDataRGBGet(OUT COLOR_RGB_T *rgb_data, OUT COLOR_ORIGIN_T *origin_data)
{


    return OPRT_OK;
}
/**
 * @berief: get light mode data
 * @param {OUT LIGHT_MODE_E *Mode -> mode data return}
 * @retval: OPERATE_RET
 */
OPERATE_RET opLightCtrlDataModeGet(OUT LIGHT_MODE_E *Mode)
{
    *Mode = LightCtrlData.Mode;

    return OPRT_OK;
}
/**
 * @berief: get control ctrl data
 * @param {none}
 * @retval: LIGHT_CTRL_DATA_T
 */
LIGHT_CTRL_DATA_T opLightCtrlDataGet(VOID)
{

    return LightCtrlData;
}

/**
 * @description: scene change has been recycle
 * @param {type} none
 * @return: none
 */
WEAK VOID vLightCtrlSceneRecycleCallback(VOID)
{

}
/**
 * @berief: Light control proc
 * @param {none}
 * @retval: OPERATE_RET
 */
OPERATE_RET opLightCtrlProc(VOID)
{
  return OPRT_OK;
}


/**
 * @berief: countdown lave time return proc
 * @param {OUT UINT_T RemainTimeSec -> left time,unit:sec}
 * @attention: this function need to implement in system
 * @retval: none
 */
WEAK VOID vLightDataCountDownResponse(OUT UINT_T RemainTimeSec)
{
    ;
}

/**
 * @berief: light ctrl breath proc
 * @param {none}
 * @return: OPERATE_RET
 * @retval: none
 */
STATIC VOID vLightCtrlCountDownTimerCB(VOID)
{
    OPERATE_RET opRet = -1;

    if(LightCtrlData.ulCountDown > 1) {     /* to avoid ulCountDown = 0 %60 also equal to 0 */
        LightCtrlData.ulCountDown --;

        if((LightCtrlData.ulCountDown % 60) == 0) {     /* upload countdown value per min */
            vLightDataCountDownResponse( LightCtrlData.ulCountDown);
        }

    } else {
        LightCtrlData.ulCountDown = 0;

        LightCtrlData.bSwitch = (LightCtrlData.bSwitch != FALSE) ? FALSE : TRUE;
        opRet = opLightCtrlProc();
        if(opRet != OPRT_OK) {
            app_print("CountDown process error!");
        }
        vLightDataCountDownResponse(0);      /* opload after ctrl deal with */
        return;     /* stop timer */
    }

    opRet = opUserSWTimerStart(COUNTDOWN_SW_TIMER, 1000, (VOID*)vLightCtrlCountDownTimerCB);
    if(opRet != OPRT_OK) {
        app_print("CountDown timer restart error!");
    }
}

/**
 * @berief: set light countdown value
 * @param {IN INT_T CountDownSec -> unit:second}
 * @attention: countdown lave time will return with
 *              calling vLightDataCountDownResponse function every minutes.
 *              switch status will return with calling
 *              vLightCtrlDataSwitchRespone function when countdown active.
 * @retval: OPERATE_RET -> OPRT_OK set countdown OK.
 */
OPERATE_RET opLightCtrlDataCountDownSet(IN INT_T CountDownSec)
{
    OPERATE_RET opRet = -1;

    if((CountDownSec < 0)|| (CountDownSec > 86400)){
        app_print("Set countdwon value error!");
        return OPRT_INVALID_PARM;
    }

    LightCtrlData.ulCountDown = CountDownSec;
    if(CountDownSec <= 0) {
        opRet = opUserSWTimerStop(COUNTDOWN_SW_TIMER);      /* cancel coutdown proc */
        if(opRet != OPRT_OK) {
            app_print("Stop countdown timer error!");
        }
        vLightDataCountDownResponse(0);
        return OPRT_OK;

    } else {
        opRet = opUserSWTimerStart(COUNTDOWN_SW_TIMER, 1000, (VOID*)vLightCtrlCountDownTimerCB);
        if(opRet != OPRT_OK) {
            app_print("Start countdown timer error!");
        }
    }
    vLightDataCountDownResponse(CountDownSec);       /* upload countdown value */

    return OPRT_OK;
}

/**
 * @berief: Light reset cnt clear timercallback
 * @param {none}
 * @return: none
 * @retval: none
 */
STATIC VOID vLightCtrlResetCntClearTimrCB(VOID)
{
    OPERATE_RET opRet = -1;

    //app_print("reset cnt clear!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    opRet = opUserFlashWriteResetCnt(0);     /* Reset cnt ++ &save to flash */
    if(opRet != OPRT_OK) {
        app_print("Reset cnt clear error!");
        return ;
    }
    app_print("cnt clear now!");
    vLightSysResetCntClearCallback();
}

/**
 * @berief: system reboot as hardware mode jude proc
 * @param {none}
 * @attention: this function need to implement by each plantform.
 * @retval: BOOL_T TRUE -> system reboot
 */
WEAK BOOL_T bLightSysHWRebootJude(VOID)
{
    return TRUE;
}

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
VOID vLightCtrlHWRebootProc(VOID)
{
    OPERATE_RET opRet = -1;
    BOOL_T bHWRebootFlag = FALSE;
    UCHAR_T ucCnt = 0;

    bHWRebootFlag = bLightSysHWRebootJude();
    if(TRUE != bHWRebootFlag) {
        return;
    }

    app_print("Light hardware reboot, turn on light!");

    opRet = opUserFlashReadResetCnt(&ucCnt);     /* read cnt from flash */
    if(opRet != OPRT_OK) {
        app_print("Read reset cnt error!");
        ucCnt = 0;
    }
    app_print("read reset cnt %d",ucCnt);

    ucCnt++;
    opRet = opUserFlashWriteResetCnt(ucCnt);     /* Reset cnt ++ &save to flash */
    if(opRet != OPRT_OK) {
        app_print("Reset cnt add write error!");
        return ;
    }

    app_print("start reset cnt clear timer!!!!!");

    /* start clear reset cnt timer */
    opRet = opUserSWTimerStart(CLEAR_RESET_CNT_SW_TIMER, 5000, (VOID*)vLightCtrlResetCntClearTimrCB);
    if(opRet != OPRT_OK) {
        app_print("start reset clear timer error!");
    }

}
/**
 * @berief: system reset proc
 * @param {none}
 * @attention: this function need implememt by system,
 *              need to deal with different thing in each plantform.
 * @retval: none
 */
WEAK OPERATE_RET opLightSysResetCntOverCB(VOID)
{
    return OPRT_OK;
}

/**
 * @berief: Light reset to re-distribute proc
 * @param {none}
 * @attention: this func will call opLightSysResetCntOverCB()
 *              opLightSysResetCntOverCB() need to implement by system
 * @retval: OPERATE_RET
 */
OPERATE_RET opLightCtrlResetCntProcess(VOID)
{
    OPERATE_RET opRet = -1;
    UCHAR_T ucCnt = 0;

    opRet = opUserFlashReadResetCnt(&ucCnt);
    if(opRet != OPRT_OK) {
        app_print("Read reset cnt error!");
        return OPRT_COM_ERROR;
    }

    if(ucCnt < 3) {
        app_print("Don't reset ctrl data!");
        return OPRT_OK;
    }


    app_print("Reset ctrl data!");
    opRet = opLightSysResetCntOverCB();
    if(opRet != OPRT_OK){
        app_print("Light reset proc error!");
    }

    return opRet;
}

/**
 * @berief: get connect mode cfg
 * @param {none}
 * @retval: UCHAR_T
 */
UCHAR_T ucLightCtrlGetConnectMode(VOID)
{
    return (LightCfgData.ucConnectMode);
}
/**
 * @berief: light ctrl normal status(constantly bright) display proc
 * @param {none}
 * @retval: none
 */
STATIC VOID vLightCtrlNormalDisplay(VOID)
{
    OPERATE_RET opRet = -1;
    BRIGHT_DATA_T usCtrlData;

    //PR_DEBUG("lowpower display....");
    memset(&usCtrlData, 0, SIZEOF(BRIGHT_DATA_T));

    opRet = opLightSetRGBCW(0, 0, 0, 0, 1000);
}
/**
 * @berief: light ctrl blink proc
 * @param {none}
 * @return: UCHAR_T
 * @retval: UCHAR_T
 */
STATIC VOID vLightCtrlBlinkDisplay(VOID)
{
    OPERATE_RET opRet = -1;
    STATIC UCHAR_T ucCnt = 0;       /* first blink off */
    BRIGHT_DATA_T usCtrlData;
    memset(&usCtrlData, 0, SIZEOF(BRIGHT_DATA_T));
    usCtrlData.usWhite = 0;
    usCtrlData.usWarm = (ucCnt % 2) ? 1000 : 0;
    opRet = opLightSetRGBCW(0, 0, 0, usCtrlData.usWhite, usCtrlData.usWarm);
    app_print("blink...C->%d,W->%d",usCtrlData.usWhite,usCtrlData.usWarm);
    if(opRet != OPRT_OK){
        app_print("blink set RGBCW error!");
    }
    ucCnt++;

}

/**
 * @berief: light ctrl blink timer callback
 * @param {none}
 * @return: none
 * @retval: none
 */
STATIC VOID vLightCtrlBlinkTimerCB(VOID)
{
    OPERATE_RET opRet = -1;

    vLightCtrlBlinkDisplay();
    opRet = opUserSWTimerStart(BLINK_SW_TIMER, uiBlinkTimeMs, (VOID*)vLightCtrlBlinkTimerCB);
    if(opRet != OPRT_OK) {
        app_print("Scene timer start error!");
    }

}

/**
 * @description: start blink
 * @param {IN UINT_T BlinkTimeMs -> blink phase}
 * @attention: blink display will as the parm
 *             -- NetColor, usNetBright in configuration.
 * @retval: none
 */
OPERATE_RET opLightCtrlBlinkStart(IN UINT_T BlinkTimeMs)
{
    OPERATE_RET opRet = -1;

    uiBlinkTimeMs = BlinkTimeMs;
    opRet = opUserSWTimerStart(BLINK_SW_TIMER, BlinkTimeMs, (VOID*)vLightCtrlBlinkTimerCB);
    if(opRet != OPRT_OK) {
        app_print("Scene timer start error!");
    }

    return OPRT_OK;
}

/**
 * @description: stop blink
 * @param {type} none
 * @attention: blink stop will directly go to normal status display
 *              normal status will bright as default bright parm
 *              -- usDefBright,usDefTemper,DefColor in configuration.
 * @retval: none
 */
OPERATE_RET opLightCtrlBlinkStop(VOID)
{
    OPERATE_RET opRet = -1;

    opRet = opUserSWTimerStop(BLINK_SW_TIMER);
    if(opRet != OPRT_OK) {
        app_print("Scene timer stop error!");
    }
    vLightCtrlNormalDisplay();
    return OPRT_OK;
}
