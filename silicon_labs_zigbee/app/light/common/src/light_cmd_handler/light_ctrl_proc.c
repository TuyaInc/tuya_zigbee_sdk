/*
 * @Author: wls
 * @email: wuls@tuya.com
 * @Date: 2019-05-16 17:18:21
 * @LastEditors: wls
 * @LastEditTime: 2019-05-16 17:33:40
 * @file name: light_ctrl_proc.c
 * @Description: light control loop (this file different form each platform)
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 */

#include "light_control.h"

#include "light_prod_test.h"
#include "light_flash.h"

#include "light_toolkit.h"


#include "tuya_cloud_com_defs.h"    /*  */


#define DPID_SWITCH         20
#define DPID_MODE           21
#define DPID_BRIGHT         22
#define DPID_TEMP           23
#define DPID_COLOR          24
#define DPID_SCENE          25
#define DPID_COUNTDOWN      26
#define DPID_MUSIC          27
#define DPID_CONTROL        28


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