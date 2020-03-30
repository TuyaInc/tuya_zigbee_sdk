/*
 * @Author: wuls
 * @email: wuls@tuya.com
 * @LastEditors: wls
 * @file name: light_init.c
 * @Description: light init process 
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-04-28 14:35:54
 * @LastEditTime: 2019-05-16 10:57:47
 */

#include "light_types.h"
#include "light_prod_test.h"
#include "light_flash.h"

#include "light_config.h"
#include "light_drive_adapter.h"

#include "user_pwm.h"
#include "sm16726b.h"
#include "sm2135.h"


/// Fast boot active flag, light app init will not active again when fast boot active 
STATIC BOOL_T bFASTBOOT_Flag = FALSE;

STATIC TIMER_ID Wifi_Status_timer = NULL;   /* wifi status display timer */

STATIC VOID vLightWifiStat_time_cb(UINT_T timerID, PVOID pTimerArg);

/**
 * @berief: 
 * @param {type} none
 * @return: none
 * @retval: none
 */
STATIC OPERATE_RET opLightHardware_Init(VOID)
{
    UCHAR_T ucDriveMode = 0;
    CHAR_T cLight_Num = -1;
    
    
    /* get configuration */ /* hard init */
    ucDriveMode = ucGetLightDriveMode();
    cLight_Num = cGetLight_Num();

    if(( ucDriveMode == /*PWM mode */ ) || (( ucDriveMode == /* sm16726B mode */) && ( cLight_Num > 3 ))) {
        
        USER_PWM_INIT_S PWM_Parm;
        UCHAR_T ucPWM_Cnt = 0;
        
        PWM_Parm.usFreq = usGetLight_PWMFreq();           /*  get  */
        PWM_Parm.usDuty = usGetLight_PWMDuty();
        PWM_Parm.bPolarity = bGetLight_PWMPolarity();

        if( ( ucDriveMode == /* sm16726B mode */) && ( /* light num < 3 */ ) ) {
            ucPWM_Cnt = cLight_Num - 3;
        }
        PWM_Parm.ucList = malloc(ucPWM_Cnt); 

        vGetLight_PWMGPIOList( PWM_Parm.ucList )
        
        opRet = USER_PWM_Init(&PWM_Parm);
        if( PWM_Parm.ucList != NULL ) {
            free(PWM_Parm.ucList);
        }
        
        if( OPRT_OK != opRet ) {
            PE_ERR("PWM INIT ERROR!");
        }
    }

    if( ucDriveMode == /* sm16726B mode */ ) {

        SM16726B_GPIO_S SM16726B_Parm;

        SM16726B_Parm.cSCL_IO = cGetLight_I2CSCL();
        SM16726B_Parm.cSDA_IO = cGetLight_I2CSDA();
        
        vGetLight_SM16726BPowerCtrl( &SM16726B_Parm.CTRL_PARM.cCTRL_IO, &SM16726B_Parm.CTRL_PARM.bLEVEL );
        
        vGetLight_SM16726BList( &SM16726B_Parm.ucList );
        
        opRet = SM16726B_Init(SM16726B_Parm);
        if( OPRT_OK != opRet ) {
            PE_ERR("SM16726B INIT ERROR!");
        }

    }

    if( ucDriveMode == /* sm2135 mode */ ) {
        
        SM2135_GPIO_S SM2135_Parm;

        SM2135_Parm.cSCL_IO = cGetLight_I2CSCL();
        SM2135_Parm.cSDA_IO = cGetLight_I2CSDA();

        vGetLight_SM2135List( &SM2135_Parm.ucList );
        
        SM2135_Parm.ucCW_Current = ucGetLight_SM2135CWCurrent();
        SM2135_Parm.ucRGB_Current = ucGetLight_SM2135RGBCurrent();

        opRet = opSM2135_Init(SM2135_Parm);
        if( OPRT_OK != opRet ) {
            PE_ERR("SM2135 INIT ERROR!");
        }
        
    }

    if(  ) {    /* key init？ */

    }

}

/**
 * @berief: light software resource init
 * @param {none} 
 * @return: none
 * @retval: none
 */
STATIC OPERATE_RET opLightSoftware_Init(VOID)
{
    OPERATE_RET opRet = -1;

    opRet = sys_add_timer(vLightWifiStat_time_cb, NULL, &Wifi_Status_timer);
    if( OPRT_OK != opRet ) {
        PR_ERR("Wifi status display timer init error %d!", opRet);
        return opRet;
    }

}


/**
 * @berief: light app init
 * @param {type} none
 * @return: none
 * @retval: none
 */
STATIC OPERATE_RET opLightInitSend()
{

}





STATIC VOID vLightDevice_Cb()
{

}

/**
 * @berief: light initlization status send out
 * @param {type} none
 * @return: none
 * @retval: none
 */
STATIC VOID vLightInitStat_Set(VOID)
{

}

/**
 * @berief: light wifi status display timer callback
 * @param {type} none
 * @return: none
 * @retval: none
 */
STATIC VOID vLightWifiStat_time_cb(UINT_T timerID, PVOID pTimerArg)
{
    STATIC BOOL_T bStatus = TRUE;
    USHORT_T usBright = 0;
    UCHAR_T usNetConfigLED = 0;
    BOOL_T bLight_CCT = FALSE;

    usBright = usGetLight_NetConfigBright();
    usNetConfigLED = ucGetLight_NetConfigLED();
    bLight_CCT = bGetLight_CCT();
      
    if( TRUE == bStatus ) {  /* blink on */
        bStatus = FALSE;
        
        switch (usNetConfigLED)
        {
            case Send_R:    /* Send R LED ON */
                vLight_Drive_Send(usBright, 0, 0, 0, 0);    
                break;

            case Send_G:
                vLight_Drive_Send(0, usBright, 0, 0, 0); 
                break;
            
            case Send_B:
                vLight_Drive_Send(0, 0, usBright, 0, 0);
                break;
                
            case Send_C:
                if( TRUE == bLight_CCT ) {
                    vLight_Drive_Send(0, 0, 0, usBright, LIGHT_MAX_CTL_VAL);
                } else {
                    vLight_Drive_Send(0, 0, 0, usBright, 0); 
                }
                break;

            case Send_W:
                if( TRUE == bLight_CCT ) {
                    vLight_Drive_Send(0, 0, 0, usBright, 0);
                } else {
                    vLight_Drive_Send(0, 0, 0, 0, usBright); 
                }
                break;

            case Send_RGB:  /* RGB mixed */
                break;

            default:
                break;
        }
    } else {    /* blink off */
        bStatus = TRUE;

        if(( TRUE == bLight_CCT ) && ( Send_C == usNetConfigLED )) {
            vLight_Drive_Send(0, 0, 0, 0, LIGHT_MAX_CTL_VAL);       /* make sure cct drive mode C LED blink don't has warm */
        } else {
            vLight_Drive_Send(0, 0, 0, 0, 0);
        }
    }


}

/**
 * @berief: wifi status callback
 * @param {type} none
 * @return: none
 * @retval: none
 */
STATIC VOID vLightWifiStat_Cb(IN CONST GW_WIFI_NW_STAT_E stat)
{
    OPERATE_RET opRet = -1;
    STATIC GW_WIFI_NW_STAT_E LastWifi_Stat = 0xFF;

    if( LastWifi_Stat != stat) {
        PR_DEBUG("Last wifi statu:%d", LastWifi_Stat);
        PR_DEBUG("Current wifi status:%d", stat);

        switch (stat)
        {
            case STAT_LOW_POWER:    
                /* code */
                break;
                
            case STAT_UNPROVISION:      /* smart configuration status */
                vLightCtrlData_Reset();
                sys_start_timer(Wifi_Status_timer, NETCONFIG_SMARTBLINK_TIME, TIMER_CYCLE);
                break;

            case STAT_AP_STA_UNCFG:     /* AP configuration status */
                vLightCtrlData_Reset();
                sys_start_timer(Wifi_Status_timer, NETCONFIG_APBLINK_TIME, TIMER_CYCLE);
                break;
            
            case STAT_AP_STA_CONN:     /* connect to router status */ 
            case STAT_STA_CONN:
                if( IsThisSysTimerRun(Wifi_Status_timer)) {
                     sys_stop_timer(Wifi_Status_timer);   
                }

                break;
            
            case STAT_CLOUD_CONN:
            case STAT_AP_CLOUD_CONN:
                break;
            
            case STAT_AP_STA_DISC:
            case STAT_STA_DISC:
                break;
            
            default:
                break;
        }


    }
}

/**
 * @berief: 
 * @param {type} none
 * @return: none
 * @retval: none
 */
STATIC OPERATE_RET opLightSmartFrame_Init(VOID)
{
    OPERATE_RET opRet = -1;
    UCHAR_T ucWIFI_CFG = 0;

    TY_IOT_CBS wf_cbs = {
        NULL,\
        NULL,\
        NULL,\
        vLightDevice_Cb,
        NULL,\
        NULL,\
        NULL,
    };

    ucWIFI_CFG = ucGetLight_WifiCFG();
    opRet = tuya_iot_wf_soc_dev_init_parm(ucWIFI_CFG, WF_START_SMART_FIRST, &wf_cbs, FIRMWARE_KEY, PRODECT_KEY, USER_SW_VER);
    if( OPRT_OK != opRet ) {
        PR_ERR("tuya iot wifi soc dev init err %d", opRet);
        return opRet;
    }

    opRet = tuya_iot_reg_get_wf_nw_stat_cb(vLightWifiStat_Cb);
    if( OPRT_OK != opRet ) {
        PR_ERR("tuya iot register get wifi new status callback init err %d", opRet);
        return opRet;
    }



}





/**
 * @berief: light fast boot process
 * @param {none} 
 * @return: none
 * @retval: none
 */
VOID vLightFastBoot(VOID)
{
    OPERATE_RET opRet = -1;

    opRet = opLightConfigLoad();     /* configuration load */ 
    if( OPRT_OK != opRet ) {
        PR_NOTICE("Cann't get configuration,will try in app_init!");
        bFASTBOOT_Flag = FALSE;
        return;
    } else {
        bFASTBOOT_Flag = TRUE;
    }
    
    opRet = opLightHardware_Init();
    if( OPRT_OK != opRet ) {
        PR_ERR("Fast boot hardware init error!");
        return;
    }

    opRet = opLightSoftware_Init();
    if( OPRT_OK != opRet ) {
        PR_ERR("Fast boot software init error!");
        return;
    }

    opRet = opGetLightControlData();
    if( OPRT_OK != opRet ) {
        vLightControlData_Reset();
    } 

    if( ) { /* light power on forcibly when hardware energized! */

    }

    opLightInitSend()
    
}


/**
 * @berief: light app init
 * @param {type} none
 * @return: none
 * @retval: none
 */
OPERATE_RET opLightAPPInit(VOID)
{
    OPERATE_RET opRet = -1;

    if( bFASTBOOT_Flag != FALSE) {
        PR_NOTICE("light app init will not active,because fast boot already actived!");
        return OPRT_OK;
    }

    opRet = opLightConfigLoad();     /* configuration load */ 
    if( opRet != opRet) {
        PR_ERR("Cann't get configuration!");
        return OPRT_COM_ERROR;
    }
    
    opRet = opLightHardware_Init();
    if( opRet != opRet) {
        PR_ERR("Light hardware init error!");
        return;
    }

    opRet = opLightSoftware_Init();
    if( opRet != opRet) {
        PR_ERR("Light software init error!");
        return;
    }


    
}

/**
 * @berief: light device init
 *          register wifi control data process callback
 * @param {type} none
 * @return: none
 * @retval: none
 */
OPERATE_RET opLightDevice_Init(VOID)
{
    opLightSmartFrame_Init();      /* wf smart frame init */
    
}