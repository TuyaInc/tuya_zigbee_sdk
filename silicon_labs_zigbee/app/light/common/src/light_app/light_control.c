/*
 * @Author: wls
 * @email: wuls@tuya.com
 * @LastEditors: wls
 * @file name: light_control.c
 * @Description: light control process
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-04-28 14:35:54
 * @LastEditTime: 2019-05-22 17:51:12
 */

#include "light_control.h"

#include "light_prod_test.h"
#include "light_flash.h"

#include "light_toolkit.h"


#include "tuya_cloud_com_defs.h"    /*  */

/// Control calculate range 0 ~ 1000
#define CTRL_CAL_VALUE_RANGE        1000    
/// Control cw calc max bright value        
#define CTRL_CW_BRIGHT_VALUE_MAX    CTRL_CAL_VALUE_RANGE
/// Control cw calc min bright value (max value * 0.1)
#define CTRL_CW_BRIGHT_VALUE_MIN    (CTRL_CW_BRIGHT_VALUE_MAX * 0.1) 

/// control power gain (100 --> gain = 1 times )
#define CTRL_POEWER_GAIN_MIN        100 
/// control power gain (200 --> gain = 2 times )   
#define CTRL_POEWER_GAIN_MAX        200    

/// control cw bright limit max 
#define CTRL_CW_LIMIT_MAX           100
/// control cw bright limit min 
#define CTRL_CW_LIMIT_MIN           0

STATIC BOOL_T bLightCTRL_InitFlag = FALSE;

STATIC LIGHT_CTRL_CFG_S Light_CFGData = {
    .Drive_mode = DRIVE_CW,
    .Switch_mode = SWITCH_GRADUAL,
    .Scene_mode = SCENE_STATIC;
};

LIGHT_CTRL_DATA_S Light_Data = {
    .bPower = FALSE;
    .Mode = WHITE_MODE;
    .usBright = 0;
    .usTemper = 0;
};

STATIC LIGHT_CTRL_HANDLE_S Light_Handle;

/// RED gamma 0.8 ~ 100%
STATIC CONST UCHAR_T gamma_red[] = {
0,0,1,1,1,2,2,3,3,4,4,5,6,6,7,7,8,9,9,10,11,11,12,13,13,
14,15,15,16,17,18,18,19,20,21,21,22,23,24,24,25,26,27,28,
28,29,30,31,32,32,33,34,35,36,37,37,38,39,40,41,42,43,44,
44,45,46,47,48,49,50,51,52,52,53,54,55,56,57,58,59,60,61,
62,63,64,65,66,66,67,68,69,70,71,72,73,74,75,76,77,78,79,
80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,
99,100,101,103,104,105,106,107,108,109,110,111,112,113,114,
115,116,117,118,119,121,122,123,124,125,126,127,128,129,130,
131,132,134,135,136,137,138,139,140,141,142,144,145,146,147,
148,149,150,151,152,154,155,156,157,158,159,160,162,163,164,
165,166,167,168,170,171,172,173,174,175,177,178,179,180,181,
182,184,185,186,187,188,189,191,192,193,194,195,196,198,199,
200,201,202,204,205,206,207,208,210,211,212,213,214,216,217,
218,219,220,222,223,224,225,227,228,229,230,231,233,234,235,
236,238,239,240,241,243,244,245,246,248,249,250,251,253,254,255								
};
										
/// GREEN 0.6 ~ 70%
STATIC CONST UCHAR_T gamma_green[] = {
0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,2,2,2,2,2,3,3,3,3,3,4,4,4,
4,5,5,5,6,6,6,7,7,7,7,8,8,8,9,9,10,10,10,11,11,11,12,12,
13,13,13,14,14,15,15,16,16,16,17,17,18,18,19,19,20,20,21,
21,22,22,23,23,24,24,25,25,26,26,27,27,28,29,29,30,30,31,
31,32,33,33,34,34,35,36,36,37,38,38,39,39,40,41,41,42,43,
43,44,45,45,46,47,47,48,49,49,50,51,52,52,53,54,54,55,56,
57,57,58,59,60,60,61,62,63,63,64,65,66,66,67,68,69,70,70,
71,72,73,74,75,75,76,77,78,79,80,80,81,82,83,84,85,86,86,
87,88,89,90,91,92,93,94,94,95,96,97,98,99,100,101,102,103,
104,105,106,106,107,108,109,110,111,112,113,114,115,116,117,
118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,
133,134,135,136,137,139,140,141,142,143,144,145,146,147,148,
149,150,151,152,154,155,156,157,158,159,160,161,162,164,165,
166,167,168,169,170,172,173,174,175,176,177,179
};
								
//BLUE 0.6-75%
STATIC CONST UCHAR_T gamma_blue[] = {
0,0,0,0,0,0,0,0,1,1,1,1,1,1,2,2,2,2,2,3,3,3,3,3,4,4,4,5,
5,5,5,6,6,6,7,7,7,8,8,8,9,9,9,10,10,11,11,11,12,12,13,13,
14,14,14,15,15,16,16,17,17,18,18,19,19,20,20,21,21,22,22,
23,23,24,24,25,25,26,27,27,28,28,29,29,30,31,31,32,32,33,
34,34,35,36,36,37,38,38,39,40,40,41,42,42,43,44,44,45,46,
46,47,48,49,49,50,51,51,52,53,54,54,55,56,57,58,58,59,60,
61,61,62,63,64,65,65,66,67,68,69,70,70,71,72,73,74,75,76,
76,77,78,79,80,81,82,83,83,84,85,86,87,88,89,90,91,92,93,
94,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,
109,110,111,112,113,114,115,116,117,118,119,120,121,122,
123,124,125,127,128,129,130,131,132,133,134,135,136,137,
138,139,141,142,143,144,145,146,147,148,150,151,152,153,
154,155,156,158,159,160,161,162,163,165,166,167,168,169,
170,172,173,174,175,176,178,179,180,181,183,184,185,186,
188,189,190,191
};

/**
 * @berief: light control init 
 * @param {LIGHT_CTRL_CFG_S Config_Data} 
 * @return: none
 * @retval: none
 */
OPERATE_RET opLightCtrl_Init(LIGHT_CTRL_CFG_S Config_Data)
{
    if( TRUE == bLightCTRL_InitFlag ) {   
        PR_ERR("light control already init! Don't initlize again!!");
        return OPRT_COM_ERROR;
    }
    bLightCTRL_InitFlag = TRUE;
    
    /* CW / CCT Drive mode setting */
    if( Config_Data.Drive_mode >= DRIVE_MAX ) {         
        PR_ERR("light drive mode set error!!");
        return OPRT_INVALID_PARM;
    }
    Light_CFGData.Drive_mode = Config_Data.Drive_mode;

    /* turn on/off change directly or gradually setting */
    if( Config_Data.Switch_mode >= SWITCH_MAX ) {       
        PR_ERR("light turn on/off change mode set error!!");
        return OPRT_INVALID_PARM;
    }
    Light_CFGData.Switch_mode = Config_Data.Switch_mode;

    /* lowpower process*/

    /* scene change setting */
    if( Config_Data.Scene_mode >= SCENE_MAX ) {
        PR_ERR("light scene change mode set error!!");
        return OPRT_INVALID_PARM;
    }
    Light_CFGData.Scene_mode = Config_Data.Scene_mode;

    /* power gain setting */
    if( Config_Data.ucPowerGain < CTRL_POEWER_GAIN_MIN ) || \
        (Config_Data.ucPowerGain > CTRL_POEWER_GAIN_MAX ) ) {
        PR_ERR("light power gain set error!!");
        return OPRT_INVALID_PARM;
    }
    Light_CFGData.ucPowerGain = Config_Data.ucPowerGain;

    /* light bright limit max&min setting */
    if( Config_Data.ucLimitCWMax > CTRL_CW_LIMIT_MAX ) || \
        (Config_Data.ucLimitCWMin > Config_Data.ucLimitCWMax ) ) {
        PR_ERR("light cw bright max&min limit set error!!");
        return OPRT_INVALID_PARM;
    }
    Light_CFGData.ucLimitCWMax = Config_Data.ucLimitCWMax;
    Light_CFGData.ucLimitCWMin = Config_Data.ucLimitCWMin;


}

/**
 * @berief: get current light power ctrl data 
 * @param {none} 
 * @return: BOOL_T 
 * @retval: bPower
 */
BOOL_T bGetLightCtrlData_Switch(VOID)
{
    return (Light_Data.bSwitch);
}

/**
 * @berief: Light control turn on/off
 * @param {none} 
 * @return: none
 * @retval: none
 */
OPERATE_RET opLightCtrl_Switch(VOID)
{
    if( bLightCTRL_InitFlag != TRUE ) {
        PR_ERR("light control switch error, light control don't initlize!")
        return OPRT_COM_ERROR;
    }

    /* 低功耗处理？？ */

    if( FALSE == onoff ) {  /* onoff ctrl state -- turn off */

        /* 需要关闭场景 & 调光的线程》》 */

        
        memset(&Light_Handle.TargetVal, 0, sizeof(BRIGHT_DATA_S));  /* set target contol data!!! */
        if( DRIVE_CCT == Light_CFGData.Drive_mode ) {   /* CCT drive mode make sure don't change warm */
            Light_Handle.TargetVal.usWarm = Light_Handle.CurrVal.usWarm;
        }

        if( SWITCH_GRADUAL == Light_CFGData.Switch_mode ) {     /* shut down gradually */
            switch(Light_Data.Mode)
            {
                case WHITE_MODE:
                case COLOR_MODE:
                    vLightCtrl_ChangeGraduallyStart();
                    /* start change thread */
                    break;
                    
                case SECEN_MODE:
                    if( SCENE_JUMP == Light_CFGData.Scene_mode ) {
                        vLight_Drive_Send(Light_Handle.TargetVal.usRed, Light_Handle.TargetVal.usGreen, Light_Handle.TargetVal.usBlue, \
                                            Light_Handle.TargetVal.usWhite, Light_Handle.TargetVal.usWarm);
                    } else {
                        vLightCtrl_ChangeGraduallyStart();
                    }
                    break;

                case MUSIC_MODE:
                    vLight_Drive_Send(Light_Handle.TargetVal.usRed, Light_Handle.TargetVal.usGreen, Light_Handle.TargetVal.usBlue, \
                                        Light_Handle.TargetVal.usWhite, Light_Handle.TargetVal.usWarm);
                    break;
                    
                default:
                    break;
            }
        } else if( SWITCH_DIRECT == Light_CFGData.Switch_mode ) {   /* shut down directly */
            vLight_Drive_Send(Light_Handle.TargetVal.usRed, Light_Handle.TargetVal.usGreen, Light_Handle.TargetVal.usBlue, \
                                Light_Handle.TargetVal.usWhite, Light_Handle.TargetVal.usWarm);
            
        } else {    /* Light_CFGData.Switch_mode */
            ;       /* no possible  */
        }
        

    } else {    /* onoff ctrl state -- turn on */ 

        if( SWITCH_GRADUAL == Light_CFGData.Switch_mode ) {

            vLightCtrl_ChangeGraduallyStart();

        } else if( SWITCH_DIRECT == Light_CFGData.Switch_mode ){
            switch(Light_Data.Mode)
            {
                case WHITE_MODE:
                    vLightCtrlData_CalcCW(Light_Data.usBright, Light_Data.usTemper, Light_Handle.TargetVal);
                    vLight_Drive_Send(Light_Handle.TargetVal.usRed, Light_Handle.TargetVal.usGreen, Light_Handle.TargetVal.usBlue, \
                                        Light_Handle.TargetVal.usWhite, Light_Handle.TargetVal.usWarm);
                    break;
                
                case COLOR_MODE:
                    vLightCtrlData_CalcRGB();     

                    
                case SECEN_MODE:
                   
                    break;

                case MUSIC_MODE:
                   
                    break;
                    
                default:
                    break;
            }
        } else {    /* Light_CFGData.Switch_mode */    
            ;       /* no possible  */
        }

    }
    return OPRT_OK;
}

/**
 * @berief: Light control turn on/off
 * @param { BOOL_T onoff 
 *          TRUE  -> turn on
 *          FALSE -> turn off} 
 * @return: none
 * @retval: none
 */
OPERATE_RET opLightCtrl_Mode(BOOL_T onoff)
{
    vLightCtrlData_CalcCW();

}

/**
 * @berief: get change gradually process the max error of 5ways
 * @param {type} none
 * @return: none
 * @retval: none
 */
STATIC VOID vLightCtrl_ChangeGraduallyStart()
{
    /* 定时间， 定步进 两种方法  */

    //定时间就需要把时间传进来；

    //启动一个硬件的timer？ 对于wifi其实timer一直是启动的，wifi就是设定一个timer的定时；
    //定时满足之后，释放一个信号量；等待信号的线程轮转；

    //ble 是启动一个硬件的timer

}

/**
 * @berief: get change gradually process the max error of 5ways
 * @param {type} none
 * @return: none
 * @retval: none
 */
STATIC USHORT_T usLight_GetChange_MAX(BRIGHT_DATA_S *TargetVal, BRIGHT_DATA_S *CurrVal)
{
    USHORT_T usMAX_Value = 0;

    usMAX_Value = (USHORT_T) uiLightTool_GetMaxValue(\
                uiLightTool_GetABSValue(TargetVal->usRed - CurrVal->usRed), \
                uiLightTool_GetABSValue(TargetVal->usGreen - CurrVal->usGreen),\
                uiLightTool_GetABSValue(TargetVal->usBlue - CurrVal->usBlue),\
                uiLightTool_GetABSValue(TargetVal->usWhite - CurrVal->usWhite),\
                uiLightTool_GetABSValue(TargetVal->usWarm - CurrVal->usWarm)\
                );
    
    return usMAX_Value;
}

STATIC VOID vLightCtrl_ChangeGradually(PVOID_T pArg)
{
    INT_T iError_Red = 0;
    INT_T iError_Green = 0;
    INT_T iError_Blue = 0;
    INT_T iError_White = 0;
    INT_T iError_Warm = 0;

    USHORT_T usMAX_Error;

    STATIC FLOAT_T fScale_Red = 0;
    STATIC FLOAT_T fScale_Green = 0;
    STATIC FLOAT_T fScale_Blue = 0;
    STATIC FLOAT_T fScale_White = 0;
    STATIC FLOAT_T fScale_Warm = 0;

    UINT_T uiStep_Red = 0;
    UINT_T uiStep_Green = 0;
    UINT_T uiStep_Blue = 0;
    UINT_T uiStep_White = 0;
    UINT_T uiStep_Warm = 0;
    

    while(1) {
        WaitSemephore();

        if( memcmp()) {

            /* calulate the error between current value and target value */
            iError_Red = Light_Handle.TargetVal.usRed - Light_Handle.CurrVal.usRed;
            iError_Green = Light_Handle.TargetVal.usGreen - Light_Handle.CurrVal.usGreen;
            iError_Blue = Light_Handle.TargetVal.usBlue - Light_Handle.CurrVal.usBlue;
            iError_White = Light_Handle.TargetVal.usWhite - Light_Handle.CurrVal.usWhite;
            iError_Warm = Light_Handle.TargetVal.usWarm - Light_Handle.CurrVal.usWarm;

            usMAX_Error = usLight_GetChange_MAX(&Light_Handle.TargetVal, &Light_Handle.CurrVal);

            if( TRUE == bFirstChange ) {    /* calculate change scale */
                fScale_Red = uiLightTool_GetABSValue(iError_Red) / 1.0 / usMAX_Error;
                fScale_Green = uiLightTool_GetABSValue(iError_Green) / 1.0 / usMAX_Error;
                fScale_Blue = uiLightTool_GetABSValue(iError_Blue) / 1.0 / usMAX_Error;
                fScale_White = uiLightTool_GetABSValue(iError_Blue) / 1.0 / usMAX_Error;
                fScale_Warm = uiLightTool_GetABSValue(iError_Blue) / 1.0 / usMAX_Error;
                bFirstChange = FALSE;
            }

            if( usMAX_Error == uiLightTool_GetABSValue(iError_Red) ) {
                uiStep_Red = 1;
            } else {
                uiStep_Red = uiLightTool_GetABSValue(iError_Red) - usMAX_Error * fScale_Red;
            }

            if( usMAX_Error == uiLightTool_GetABSValue(iError_Green) ) {
                uiStep_Green = 1;
            } else {
                uiStep_Green = uiLightTool_GetABSValue(iError_Green) - usMAX_Error * fScale_Green;
            }

            if( usMAX_Error == uiLightTool_GetABSValue(iError_Blue) ) {
                uiStep_Blue = 1;
            } else{
                uiStep_Blue = uiLightTool_GetABSValue(iError_Blue) - usMAX_Error *fScale_Blue;
            }
            
            if( usMAX_Error == uiLightTool_GetABSValue(iError_White) ) {
                uiStep_White = 1;
            } else{
                uiStep_White = uiLightTool_GetABSValue(iError_White) - usMAX_Error *fScale_White;
            }

            if( usMAX_Error == uiLightTool_GetABSValue(iError_Warm) ) {
                uiStep_Warm = 1;
            } else{
                uiStep_Warm = uiLightTool_GetABSValue(iError_Warm - usMAX_Error *fScale_Warm;
            }

            if( iError_Red != 0) {
                if( uiLightTool_GetABSValue(iError_Red) < uiStep_Red ) {
                    Light_Handle.CurrVal.usRed += iError_Red;
                } else {
                    if( iError_Red < 0 ) {
                        Light_Handle.CurrVal.usRed -= uiStep_Red;
                    } else {
                        Light_Handle.CurrVal.usRed += uiStep_Red;
                    }
                }
            }

            if( iError_Green != 0) {
                if( uiLightTool_GetABSValue(iError_Green) < uiStep_Green ) {
                    Light_Handle.CurrVal.usGreen += iError_Green;
                } else {
                    if( iError_Green < 0 ) {
                        Light_Handle.CurrVal.usGreen -= uiStep_Green;
                    } else {
                        Light_Handle.CurrVal.usGreen += uiStep_Green;
                    }
                }
            }

            if( iError_Blue != 0) {
                if( uiLightTool_GetABSValue(iError_Blue) < uiStep_Blue ) {
                    Light_Handle.CurrVal.usBlue += iError_Blue;
                } else {
                    if( iError_Blue < 0 ) {
                        Light_Handle.CurrVal.usBlue -= uiStep_Blue;
                    } else {
                        Light_Handle.CurrVal.usBlue += uiStep_Blue;
                    }
                }
            }

            if( iError_White != 0) {
                if( uiLightTool_GetABSValue(iError_White) < uiStep_White ) {
                    Light_Handle.CurrVal.usWhite += iError_White;
                } else {
                    if( iError_White < 0 ) {
                        Light_Handle.CurrVal.usWhite -= uiStep_White;
                    } else {
                        Light_Handle.CurrVal.usWhite += uiStep_White;
                    }
                }
            }

            if( iError_Warm!= 0) {
                if( uiLightTool_GetABSValue(iError_Warm) < uiStep_Warm ) {
                    Light_Handle.CurrVal.usWarm += iError_Warm;
                } else {
                    if( iError_Warm < 0 ) {
                        Light_Handle.CurrVal.usWarm -= uiStep_Warm;
                    } else {
                        Light_Handle.CurrVal.usWarm += uiStep_Warm;
                    }
                }
            }

            /* send out! */
            vLight_Drive_Send(Light_Handle.CurrVal.usRed, Light_Handle.CurrVal.usGreen, Light_Handle.CurrVal.usBlue, Light_Handle.CurrVal.usWhite, Light_Handle.CurrVal.usWarm);

        } else{

        }
    }
}




/**
 * @berief: light control data reset
 * @param：none 
 * @return: none
 * @retval: none
 */
VOID vLightCtrlData_Reset(VOID)
{
    UCHAR_T ucDefLightLED = 0;


    memset(&Light_Data, 0, SIZEOF(LIGHT_CTRL_DATA_S));

    Light_Data.bPower = TRUE;

    ucDefLightLED = ucGetDefLightLED();
    if( ucDefLightLED < COLOR_C) {
        Light_Data.Mode = COLOR_MODE;
    } else {
        Light_Data.Mode = WHITE_MODE;
    }

    Light_Data.usBright = usGetDefLightBright();        /* 这两个接口应该放在oem json的获取 */

    Light_Data.usTemper = usGetDefLightTemper();

    vGetDefLightColor(Light_Data.ucColor);

    vGetDefLightScene(Light_Data.ucScene);

}

/**
 * @berief: CW bright output limit process
 * @param {type} none
 * @return: none
 * @retval: none
 */
STATIC USHORT_T usLightCtrlData_CWLimit(USHORT_T Bright)
{
    USHORT_T usMax = 0, usMin = 0, usResult = 0;

    usMax = CTRL_CW_BRIGHT_VALUE_MAX * ( Light_CFGData.ucLimitCWMax / 100 );
    usMin = CTRL_CW_BRIGHT_VALUE_MAX * ( Light_CFGData.ucLimitCWMin / 100 );

    usResult = ( Bright - CTRL_CW_BRIGHT_VALUE_MIN ) * ( usMax - usMin ) / \
                ( CTRL_CW_BRIGHT_VALUE_MAX - CTRL_CW_BRIGHT_VALUE_MIN ) + usMin;

    return(usResult);
}

/**
 * @berief: calculate the CW Light_Handle.TargetVal according to
 *          the Light_Data value, and cw output limit and amplify process!
 * @param { USHORT_T Bright -> Light_Data bright 
 *          USHORT_T Temper -> Light_Data temperature 
 *          BRIGHT_DATA_S *Result -> calc result } 
 * @return: none
 * @retval: none
 */
STATIC VOID vLightCtrlData_CalcCW(USHORT_T Bright, USHORT_T Temper, BRIGHT_DATA_S *Result)
{
    USHORT_T usBrightTmep = 0;

    if( Bright < CW_BRIGHT_VALUE_MIN ) {    
        usBrightTmep = 0;
    } else {
        usBrightTmep = usLightCtrlData_CWLimit(Bright);     /* limit CW bright output limit */
    }

    if( DRIVE_CW == Light_CFGData.Drive_mode ) {
        usBrightTmep = (USHORT_T) (((FLOAT_T)( Light_CFGData.ucPowerGain / CTRL_POEWER_GAIN_MIN )) * usBrightTmep );     /* power amplification */

        Result ->usWhite = usBrightTmep * Temper / CTRL_CAL_VALUE_RANGE;
        Result ->usWarm = usBrightTmep - Result ->usWhite;

        /* output max limit -- convert overflow power to another  */
        if( Result ->usWhite > CTRL_CAL_VALUE_RANGE ) {     
            Result ->usWarm = (USHORT_T)( Result ->usWarm * ( (FLOAT_T) ( Result ->usWhite / CTRL_CAL_VALUE_RANGE )) );
            Result ->usWhite = CTRL_CAL_VALUE_RANGE;
        }
        if( Result ->usWarm > CTRL_CAL_VALUE_RANGE ) {     
            Result ->usWhite = (USHORT_T)( Result ->usWhite * ( (FLOAT_T) ( Result ->usWarm / CTRL_CAL_VALUE_RANGE )) );
            Result ->usWarm = CTRL_CAL_VALUE_RANGE;
        }
    } else if ( DRIVE_CCT == Light_CFGData.Drive_mode ) {
        Result ->usWhite = Bright
        Result ->usWarm = Temper;
    }

#if 0
/// attention 不知道为啥！！
    if( Result ->usWhite < 5 ) {
        Result ->usWhite  = 0;
        Result ->usWarm = Bright;
    } else if( Result ->usWarm < 5) {
        Result ->usWarm  = 0;
        Result ->usWhite = Bright;
    }
#endif

}

STAITC VOID vLightCtrlData_GammaAdjust(COLOR_RGB_S Color, COLOR_RGB_S *Result)
{

}

/**
 * @berief: calculate the RGB Light_Handle.TargetVal according to
 *          the Light_Data value
 * @param {type} none
 * @return: none
 * @retval: none
 */
STATIC VOID vLightCtrlData_CalcRGB(COLOR_RGB_S Color, BRIGHT_DATA_S *Result)
{
    
}



VOID vLightHandleData_BrightCW( )
{
    
}


