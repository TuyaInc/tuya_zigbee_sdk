/*
 * @Author: wls
 * @email: wuls@tuya.com
 * @Date: 2019-05-22 10:17:39
 * @LastEditors: wls
 * @LastEditTime: 2019-05-22 16:03:11
 * @file name: light_system.c
 * @Description: system adapter process
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 */

#include "light_system.h"
#include "light_control.h"

/**
 * @berief: set light switch data, adapte control data issued by system
 *          to control data format
 * @param { BOOL_T onoff 
 *              -> TRUE     turn on
 *              -> FALSE    turn off } 
 * @return: none
 * @retval: none
 */
STATIC VOID vLightSysData_Switch(BOOL_T onoff)
{
   if( TRUE == onoff ) {
       Light_Data.bSwitch = TRUE;
   } else {
       Light_Data.bSwitch = FALSE;
   }
}

/**
 * @berief: set light mode data, adapte control data issued by system
 *          to control data format
 * @param { BOOL_T onoff 
 *              -> TRUE     turn on
 *              -> FALSE    turn off } 
 * @return: none
 * @retval: none
 */
STATIC VOID vLightSysData_Mode(UCHAR_T Mode)
{
    Light_Data.Mode = Mode;
}

/**
 * @berief: set light CW bright data, adapte control data issued by system
 *          to control data format
 * @param { BOOL_T onoff 
 *              -> TRUE     turn on
 *              -> FALSE    turn off } 
 * @return: none
 * @retval: none
 */
STATIC VOID vLightSysData_CWBright(USHORT_T Bright)
{
    Light_Data.usBright = Bright;
}

/**
 * @berief: set light CW Temperature data, adapte control data issued by system
 *          to control data format
 * @param { BOOL_T onoff 
 *              -> TRUE     turn on
 *              -> FALSE    turn off } 
 * @return: none
 * @retval: none
 */
STATIC VOID vLightSysData_CWTemper(USHORT_T Temper)
{
    Light_Data.usTemper = Temper;
}

/**
 * @berief: set light color RGB data, adapte control data issued by system
 *          to control data format(RGB format)
 * @param { CHAR_T *ColorString -> color HSV string set } 
 * @attention: e.g 000011112222
 *              0000：H (hue:           0-360,      0X0000-0X0168)
 *              1111：S	(saturation：   0-1000,	    0X0000-0X03E8) 
 *              2222：V (Value:         0-1000，    0X0000-0X03E8)
 * @return: none
 * @retval: none
 */
STATIC VOID vLightSysData_RGB(CHAR_T *ColorString)
{
    USHORT_T usVal_H, usVal_S, usVal_V;
    FLOAT_T fVal_H, fVal_S, fVal_V;
    USHORT_T usVal_R, usVal_G, usVal_B;

    usVal_H = usLightTool_Str2Ushort( ucLightTool_Asc2Hex(ColorString[0]), ucLightTool_Asc2Hex(ColorString[1]),\
                                      ucLightTool_Asc2Hex(ColorString[2], ucLightTool_Asc2Hex(ColorString[3]) );
    
    usVal_S = usLightTool_Str2Ushort( ucLightTool_Asc2Hex(ColorString[4]), ucLightTool_Asc2Hex(ColorString[5]),\
                                      ucLightTool_Asc2Hex(ColorString[6], ucLightTool_Asc2Hex(ColorString[7]) );
    
    usVal_V = usLightTool_Str2Ushort( ucLightTool_Asc2Hex(ColorString[8]), ucLightTool_Asc2Hex(ColorString[9]),\
                                      ucLightTool_Asc2Hex(ColorString[10], ucLightTool_Asc2Hex(ColorString[11]) );

    fVal_H = (FLOAT_T)usVal_H;
    fVal_S = (FLOAT_T)(usVal_S / 1000.0);
    fVal_V = (FLOAT_T)(usVal_V / 1000.0);

    vLightTool_HSV2RGB( fVal_H, fVal_S, fVal_V, &usVal_R, &usVal_G, &usVal_B, 1000 );
    
    Light_Data.Color.usR_Value = usVal_R;
    Light_Data.Color.usG_Value = usVal_G;
    Light_Data.Color.usB_Value = usVal_B;
}

/**
 * @berief: get current light power ctrl data 
 * @param {none} 
 * @return: BOOL_T 
 * @retval: bPower
 */
BOOL_T bGetLightCtrlData_Power(VOID)
{
    return (Light_Data.bPower);
}


/**
 * @berief: !!!!this function move to new file!
 * @param {type} none
 * @return: none
 * @retval: none
 */
bLightConttrol_Proc(TY_OBJ_DP_S *root)
{
    UCHAR_T ucDPID = 0;


    ucDPID = root -> dpid;

    switch (ucDPID)
    {
        case DPID_SWITCH:   /* turn on/off */
            if( bGetLightCtrlData_Switch() != root ->value.dp_bool ) {      /* need to active */
                vLightSysData_Switch(root ->value.dp_bool);
                
            }
            break;
        
        case DPID_MODE：
            break;

        case DPID_BRIGHT:
            break;
        
        default:
            break;
    }
}