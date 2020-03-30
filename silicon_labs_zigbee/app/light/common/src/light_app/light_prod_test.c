
/*
 * @Author: wls
 * @email: wuls@tuya.com
 * @LastEditors:   
 * @file name: light_prod_test.c
 * @Description: light production test proc
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-04-26 13:55:40
 * @LastEditTime: 2019-05-06 15:15:10
 */

#include "light_types.h"
#include "light_prod_test.h"
#include "light_flash.h"


/// Weak signal display change base time(unit ms)
#define WEAK_SINGAL_BASE_INTERVAL                   10   

/// Unauthorized display change base time(unit ms)
#define UNAUTHOR_BASE_TIME_INTERVAL                 10  

/// Production overall function check base timr(unit ms)
#define PROD_CHECK_BASE_INTERVAL            10

/// Production aging restart check base time(unit ms)
#define PROD_AGING_START_BASE_INTERVAL    10

/// Production aging check base time(unit ms)
#define PROD_AGING_BASE_INTERVAL            500

/// Production repeat test base time(unit ms)
#define PROD_REPEAT_BASE_INERVAL            10




/// need to move to adapter.h
typedef WORD TIMER_ID;  


/**
 * @brief Light LED enum
 */
typedef enum{
    LED_R = 0,
    LED_G,
    LED_B,
    LED_C,
    LED_W,
    LED_MAX,
    LED_CW,     /* CW AGING END */
    LED_RGB     /* RGB AGING */
}LIGHT_LED_E;

/**
 * @brief Light Aging time enum
 */
typedef enum{
    AGING_C,
    AGING_W,
    AGING_RGB,
    AGING_MAX
}LIGHT_AGING_E;

/**
 * @berief: Production factory step
 */
typedef enum{
    WEAK_SINGAL = 0,
    UNAUTHORIZED,
    PROD_CHECK,
    PROD_AGINGSTART,
    PROD_AGING,
    PROD_REPEAT
}PROD_STEP_E;


/**
 * @berief: Production weak singal display status
 */
typedef enum{
    WS_BreathUp = 0,
    WS_BreathDown,
    WS_BlinkON,
    WS_BlinkOFF
}WEAKSINGAL_STATUS_E;



/**
 * @brief 
 * IIC IC enable ctrl gpio pin control status enum
 */
typedef struct 
{
    TIMER_ID weaksingal_timer;      /*  */
    TIMER_ID unauthor_timer;        /* data */
    TIMER_ID prodcheck_timer; 
    TIMER_ID agingstart_timer;
    TIMER_ID prodaging_timer; 
    TIMER_ID repeat_timer;       
}Light_Prod_Test_S;



STATIC Light_Prod_Test_S PT_Handle = {

    .weaksingal_timer = NULL;
    .unauthor_timer = NULL;
    .prodcheck_timer = NULL;
    .agingstart_timer = NULL;
    .prodaging_timer = NULL;
    
    
};

/**
 * @berief: get production factory test mode
 * @param {none} 
 * @return: cMode
 * @retval: none
 */
STATIC CHAR_T cGetProdTest_Mode(VOID)
{
    CHAR_T cMode = -1;
    SHORT_T sRead_Buf[2] = {-1, -1};

    vUserFlash_Read(DTYPED_PORD, sRead_Buf);

    cMode = (CHAR_T) sRead_Buf[0];

    return(cMode);
}

/**
 * @berief: get production factory test aging time
 * @param {none} 
 * @return: usAgingTime -> aging time(already consumed)unit：min
 * @retval: none
 */
STATIC USHORT_T cGetProdTest_AgingTime(VOID)
{
    USHORT_T usAgingTime = 0;
    SHORT_T sRead_Buf[2] = {-1, -1};

    vUserFlash_Read(DTYPED_PORD, sRead_Buf);

    usAgingTime = (CHAR_T) sRead_Buf[1];

    return(usAgingTime);
}

/**
 * @berief: set production factory test mode & aging time
 * @param {none} 
 * @return: cMode
 * @retval: none
 */
STATIC OPERATE_RET opSetProdTest_Mode(UCHAR_T ucMode, USHORT_T usAgingTime)
{
    OPERATE_RET opRet = -1;
    SHORT_T sRead_Buf[2] = {-1, -1};

    sRead_Buf[0] = ucMode;
    sRead_Buf[1] = usAgingTime;
    
    opRet = opUserFlash_Write(DTYPED_PORD, sRead_Buf);
    if( OPRT_OK != opRet ) {
        PR_ERR("Set production test mode error!");
        return opRet;
    }

    return OPRT_OK;
}

/**
 * @berief: get correct display light led according to  
 *          the Production test document
 * @param {PROD_STEP_E Step -> Production step
 *          CHAR_T *LED_LIST -> Production display led list} 
 * @return: Light_Num -> light led num
 * @retval: none
 */
STATIC CHAR_T cGetLight_Led(PROD_STEP_E Step, UCHAR_T *ucLED_LIST)
{
    CHAR_T cLight_Num = -1;
    CONST UCHAR_T ucBase_list[6] = {LED_R, LED_G, LED_B, LED_C, LED_W, LED_RGB};
    UCHAR_T i = 0;
    
    cLight_Num = cGetLight_Num();    /* get light num */

    if( cLight_Num < 1 ) {
        PR_ERR("get light led num error!");
        return(cLight_Num);
    }

    switch (Step) 
    {
        case WEAK_SINGAL:       /* weak singal display led */
        case UNAUTHORIZED:      /* unauthorized display led */
            if( cLight_Num != 3 ) {               
                ucLED_LIST[0] = LED_C;    /* led_c blink except 3way light */
            } else {
                ucLED_LIST[0] = LED_R;
            }                       
            break;
        
        case PROD_CHECK:
        case PROD_AGINGRSTART:      /* overall check & aging restart display led */
        case PROD_REPEAT:
            if( cLight_Num < 3 ) {
                for(i = 0; i < cLight_Num; i++) {
                    ucLED_LIST[i] = ucBase_list[ 3 + i ];          /* C / CW */   
                }
            } else {
                for(i = 0; i < cLight_Num; i++) {
                    ucLED_LIST[i] = ucBase_list[i];                /* RGB/RGBC/RGBCW */
                }
            }
            break;
        
        case PROD_AGING:
            if( cLight_Num < 3 ) {
                for(i = 0; i < cLight_Num; i++) {
                    ucLED_LIST[i] = ucBase_list[ 3 + i ];         
                }
            } else if( cLight_Num == 3) {   
                ucLED_LIST[0] = LED_RGB;   
                cLight_Num = 1;                 /* cLight_Num to save aging led num */
            } else if( cLight_Num == 4) {
                ucLED_LIST[0] = LED_C;
                ucLED_LIST[1] = LED_RGB;
                cLight_Num = 2;
            } else if() {
                ucLED_LIST[0] = LED_C;
                ucLED_LIST[1] = LED_W;
                ucLED_LIST[3] = LED_RGB;
                cLight_Num = 3;
            }
            break;
            
        default:
            break;
    }
    
    return cLight_Num;
}

/**
 * @berief: get correct display light led according to  
 *          the Production test document
 * @param {PROD_STEP_E Step -> Production step
 *          CHAR_T *LED_LIST -> Production display led list} 
 * @return: Light_Num -> light led num
 * @retval: none
 */
STATIC OPERATE_RET cGetAgingTarget_Time( USHORT_T *usTarget )
{
    CHAR_T cLight_Num = -1;
    CONST USHORT_T usTarget_time[5][3] = {
            /* C  W   RGB aging time*/
            { 40, 0 , 0 },          /* 1 way light  */
            { 20, 20, 0 },          /* 2 way light  */
            { 0 , 0 , 40},          /* 3 way light  */
            { 30, 0 , 10},          /* 4 way light  */
            { 20, 20, 10}           /* 5 way light  */
    }
    
    cLight_Num = cGetLight_Num();    /* get light num */

    if( cLight_Num < 1 ) {
        PR_ERR("get light led num error!");
        return OPRT_INVALID_PARM ;
    }

    memcpy(usTarget, usTarget_time[cLight_Num], AGING_MAX);

    return OPRT_OK;
}


/**
 * @berief: production send drive light
 * @param {LIGHT_LED_E Bright_LED -> need to light led 
 *         USHORT_T usBright_Value -> bright value} 
 * @return: none
 * @retval: none
 */
STATIC VOID vProd_Send(LIGHT_LED_E Bright_LED, USHORT_T usBright_Value)
{
    UCHART_T ucCWDrive_Mode = 0;
    USHORT_T usCCTTemp_Scale = 0;

    switch (Bright_LED)
    {
        case LED_R:  /* constant-expression */
            vLight_Drive_Send(usBright_Value, 0, 0, 0, 0); 
            break;

        case LED_G:
            vLight_Drive_Send(0, usBright_Value, 0, 0, 0); 
            break;

        case LED_G:
            vLight_Drive_Send(0, 0, usBright_Value, 0, 0); 
            break;

        case LED_C:
            ucCWDrive_Mode = ucGetCWDrive_Mode(&usCCTTemp_Scale);
            if(ucCWDrive_Mode == CCT) {
                vLight_Drive_Send(0, 0, 0, usBright_Value, usCCTTemp_Scale);    /* CCT Drive mode bright W LED! */
            } else {
                vLight_Drive_Send(0, 0, 0, usBright_Value, 0);          
            }
            break;
        
        case LED_W:
            ucCWDrive_Mode = ucGetCWDrive_Mode(&usCCTTemp_Scale);
            if(ucCWDrive_Mode == CCT) {
                vLight_Drive_Send(0, 0, 0, usBright_Value, 0);                  /* CCT Drive mode bright W LED! */
            } else {
                vLight_Drive_Send(0, 0, 0, 0, usBright_Value);  
            }
            
            break;

        case LED_CW:
            ucCWDrive_Mode = ucGetCWDrive_Mode(&usCCTTemp_Scale);
            if(ucCWDrive_Mode == CCT) {
                vLight_Drive_Send(0, 0, 0, usBright_Value/2, usCCTTemp_Scale/2);  /* CCT Drive mode C&W LED bright mix */
            } else {
                vLight_Drive_Send(0, 0, 0, usBright_Value, usBright_Value); 
            }
            break;
        
        case LED_RGB:
            vLight_Drive_Send(usBright_Value, usBright_Value, usBright_Value, 0, 0); 
            break;
            
        case LED_MAX:
        default:
            PR_ERR("Production send led error!");
            vLight_Drive_Send(0, 0, 0, 0, 0);    /* shut down! */
            break;
    }
}

/**
 * @berief: Production factory test detect weak signal display timer callback
 *          display styple
 *              C     ->  C breath up -> breath down -> blink 
 *              CW    ->  C breath up -> breath down -> blink
 *              RGB   ->  R breath up -> breath down -> blink
 *              RGBC  ->  C breath up -> breath down -> blink
 *              RGBCW ->  C breath up -> breath down -> blink
 *          above display base on WEAK_SINGAL_BASE_INTERVAL frequency
 * @param {UINT_T timerID -> timerID
 *         PVOID pTimerArg -> timer param}
 * @return: none
 * @retval: none
 */
STATIC VOID vWeakSingal_time_cb(UINT_T timerID, PVOID pTimerArg)
{
    OPERATE_RET opRet = -1;
    STATIC UCHAR_T ucStatus = 0;
    STATIC UCHAR_T *ucLED_LIST = NULL;
    STATIC USHORT_T usBright_Max = 0;
    STAITC USHORT_T usChange_Step = 0;
    STATIC SHORT_T sSend_Value = 0;
    CHAR_T cLED_Num = -1;
    STATIC USHORT_T usStand_Cnt = 0;
    STATIC UCHAR_T ucONOFF_Cnt = 0;

    if( NULL == ucLED_LIST ) {
        ucLED_LIST = (UCHAR_T *) Malloc(LED_MAX);
        if( NULL == ucLED_LIST ) {
            PR_ERR("start weak singal display malloc error,will try again!");
        }
        memset(ucLED_LIST, 0, LED_MAX);
        cLED_Num = cGet_Light_Led(WEAK_SINGAL, ucLED_LIST);
        if( 1 != cLED_Num) {
            PR_ERR("programm internal error!");
        } 

        usBright_Max = usGetLight_MaxBright();          /* bright need to get from json */
        usChange_Step = usBright_Max / (WEAK_SINGAL_BREATHR_TIME_INTERVAL / WEAK_SINGAL_BASE_INTERVAL);
    } 

    switch(ucStatus) 
    {
        case WS_BreathUp:
            vProd_Send(ucLED_LIST[0], sSend_Value);     /* breath up */
            sSend_Value += usChange_Step;
            if( sSend_Value >= usBright_Max ) {
                ucStatus = WS_BreathDown;
            }
            break;
        
        case WS_BreathDown:                             /* breath down */
            vProd_Send(ucLED_LIST[0], sSend_Value);
            sSend_Value -= usChange_Step;
            if( usChange_Step <= 0 ) {
                ucStatus = WS_BlinkON;
            }
            break;
            
        case WS_BlinkON:                                /* blink on off */                                 
            usStand_Cnt++;
            vProd_Send(ucLED_LIST[0], usBright_Max);
            if( (usStand_Cnt * WEAK_SINGAL_BASE_INTERVAL) >= WEAK_SINGAL_ONOFF_TIME_INTERVAL) ) {
                usStand_Cnt = 0;
                ucStatus = WS_BlinkOFF;
            }
            break;
            
        case WS_BlinkOFF:
            vProd_Send(ucLED_LIST[0], 0);
            if( (usStand_Cnt * WEAK_SINGAL_BASE_INTERVAL) >= WEAK_SINGAL_ONOFF_TIME_INTERVAL) ) {
                if( ucONOFF_Cnt < (WEAK_SINGAL_ONOFF_CNT - 1)) {
                    usStand_Cnt = 0;
                    ucONOFF_Cnt++;
                    ucStatus = WS_BlinkON;
                } else { 
                    usStand_Cnt = 0;
                    ucONOFF_Cnt = 0;
                    ucStatus = WS_BreathUp;
                }    
            }
            break;

        default:
            break;
    }
    
}

/**
 * @berief: weak signal process init
 *           create timer cb handle
 * @param {none}
 * @return: none
 * @retval: none
 */
STATIC VOID vWeakSignal_Proc(VOID)
{
    OPERATE_RET opRet = -1;

    opRet = sys_add_timer(vWeakSingal_time_cb, NULL, &PT_Handle.weaksingal_timer);
    if( OPRT_OK != opRet ) {
        Light_Drive_Send(0,0,0,0,0);        /* light shut down */
        PR_ERR("weak signal display init failure,light shut down!");
        return;
    }

    opRet = sys_start_timer(PT_Handle.weaksingal_timer, WEAK_SINGAL_BASE_INTERVAL, TIMER_CYCLE);
    if( OPRT_OK != opRet ) {
        Light_Drive_Send(0,0,0,0,0);        /* light shut down */
        PR_ERR("weak singal display init failure,light shut down!");
        return;
    }     
}

/**
 * @berief: Production unauthorizeed display timer callback
 *          display styple
 *              C     ->  C on/off mimimus brightness
 *              CW    ->  C on/off mimimus brightness
 *              RGB   ->  R on/off mimimus brightness
 *              RGBC  ->  C on/off mimimus brightness
 *              RGBCW ->  C on/off mimimus brightness 
 *          above display on/off as AUZ_TEST_FAIL_TIME_INTERVAL frequency
 * @param {UINT_T timerID -> timerID
 *         PVOID pTimerArg -> timer param}
 * @return: none
 * @retval: none
 */
STATIC VOID vUnauthor_time_cb(UINT_T timerID, PVOID pTimerArg)
{
    OPERATE_RET opRet = -1;
    STATIC BOOL_T bStatus = FALSE;
    STATIC UCHAR_T *ucLED_LIST = NULL;
    CHAR_T cLED_Num = -1;
    STATIC USHORT_T usBright_Min = 0xFFFF;
    STATIC USHORT_T usStand_Cnt = 0;

    if( NULL == ucLED_LIST ) {
        ucLED_LIST = (UCHAR_T *) Malloc(LED_MAX);
        if( NULL == ucLED_LIST ) {
            PR_ERR("start unauthorized display malloc error,will try again!!");
        }
        memset(ucLED_LIST, 0, LED_MAX);
        cLED_Num = cGetLight_Led(UNAUTHORIZED, ucLED_LIST);
        if( 1 != cLED_Num) {
            PR_ERR("programm internal error ！");
        }
        usBright_Min = usGetLight_MinBright();
    } 
   
    if( TRUE == bStatus ) {
        usStand_Cnt++;
        vProd_Send(LED_LIST[0], usBright_Min);
        if( (usStand_Cnt * UNAUTHOR_BASE_TIME_INTERVAL) >= AUZ_TEST_FAIL_TIME_INTERVAL) ) {
            usStand_Cnt = 0;
            bStatus = FALSE;
        }
    } else {
        usStand_Cnt++;
        vProd_Send(LED_LIST[0], 0);
        if( (usStand_Cnt * UNAUTHOR_BASE_TIME_INTERVAL) >= AUZ_TEST_FAIL_TIME_INTERVAL) ) {
            usStand_Cnt = 0;
            bStatus = TRUE;
        }
    }
}

/**
 * @berief: unauthorized process init
 *           create timer cb handle
 * @param {none}
 * @return: none
 * @retval: none
 */
STATIC VOID vUnauthorized_Proc(VOID)
{
    OPERATE_RET opRet = -1;

    opRet = sys_add_timer(Unauthor_time_cb, NULL, &PT_Handle.unauthor_timer);
    if( OPRT_OK != opRet ) {
        Light_Drive_Send(0,0,0,0,0);        /* light shut down */
        PR_ERR("unauthorized display init failure,light shut down!");
        return;
    }

    opRet = sys_start_timer(PT_Handle.unauthor_timer, UNAUTHOR_BASE_TIME_INTERVAL, TIMER_CYCLE);
    if( OPRT_OK != opRet ) {
        Light_Drive_Send(0,0,0,0,0);        /* light shut down */
        PR_ERR("unauthorized display init failure,light shut down!");
        return;
    }
}

/**
 * @berief: Production factory test check overall function display timer callback
 *          display styple
 *              C     ->  C blink 
 *              CW    ->  C > W blink 
 *              RGB   ->  R > G > B blink
 *              RGBC  ->  R > G > B > C blink
 *              RGBCW ->  R > G > B > C > W blink
 *          above display base on PROD_CHECK_BASE_INTERVAL frequency
 * @param {UINT_T timerID -> timerID
 *         PVOID pTimerArg -> timer param}
 * @return: none
 * @retval: none
 */
STATIC VOID vProdCheck_time_cb(UINT_T timerID, PVOID pTimerArg)
{
    OPERATE_RET opRet = -1;
    STATIC UCHAR_T *ucLED_LIST = NULL;
    CHAR_T cLED_Num = -1;
    STATIC CHAR_T cIndex = -1;
    STATIC BOOL_T bStatus = TRUE;
    STATIC USHORT_T usStand_cnt = 0;
    STATIC UINT_T ulCheck_cnt = 0;
    STATIC USHORT_T usBright_Max = 0;

    if( NULL == ucLED_LIST ) {
        ucLED_LIST = (UCHAR_T *) Malloc(LED_MAX);
        if( NULL == ucLED_LIST ) {
            PR_ERR("start production overall check malloc error,will try again!");
        }
        memset(ucLED_LIST, 0, LED_MAX);
        cLED_Num = cGetLight_Led(PROD_CHECK, ucLED_LIST);        
        if(cLED_Num < 1) {
            PR_ERR("programm internal error ！");
        }
        usBright_Max = usGetLight_MaxBright();          /* bright need to get from json */
    } 

    if( TRUE == bStatus ) {
        vProd_Send(ucLED_LIST[cIndex], usBright_Max);
        usStand_cnt++;
        if( (usStand_cnt * PROD_CHECK_BASE_INTERVAL) >= PORD_CHECK_ONOF_TIMER_INTERVAL) {
            usStand_cnt = 0;
            bStatus = FALSE;
        }
    } else {
        Prod_Send(ucLED_LIST[cIndex], 0);
        usStand_cnt++;
        if( (usStand_cnt * PROD_CHECK_BASE_INTERVAL) >= PORD_CHECK_ONOF_TIMER_INTERVAL) {
            usStand_cnt = 0;
            cIndex++;
            if(cIndex >= cLED_NUM) {
                cIndex = 0;
            }
            bStatus = TRUE;
        }   
    }

    ulCheck_cnt++;
    if( (ulCheck_cnt * PROD_CHECK_BASE_INTERVAL) >= PROD_CHECK_TIMER_INTERVAL ) {   /* overall check time over,the test mode change to aging mode! */
        
        opRet = opSetProdTest_Mode(PROD_AGING, 0);  /* save test mode，change to aging mode! */
        if( OPRT_OK == opRet ) {
            opRet = sys_stop_timer(PT_Handle.prodcheck_timer);
            if(OPRT_OK != opRet) {
                PR_ERR("Stop overall timer error!");
            }      
            vAgingRestart_Proc();       
        } 
        
    }
}

/**
 * @berief: production overall function check
 * @param {none} 
 * @return: none
 * @retval: none
 */
STATIC VOID vProdCheck_Proc(VOID)
{
    OPERATE_RET opRet = -1;

    opRet = sys_add_timer(vProdCheck_time_cb, NULL, &PT_Handle.prodcheck_timer);
    if( OPRT_OK != opRet ) {
        Light_Drive_Send(0,0,0,0,0);        /* light shut down */
        PR_ERR("production overall check display init failure,light shut down!");
        return;
    }

    opRet = sys_start_timer(PT_Handle.prodcheck_timer, PROD_CHECK_BASE_INTERVAL, TIMER_CYCLE);
    if( OPRT_OK != opRet ) {
        Light_Drive_Send(0,0,0,0,0);        /* light shut down */
        PR_ERR("production overall check display init failure,light shut down!");
        return;
    }
}

/**
 * @berief: Production factory test aging function start display timer callback
 *          display styple
 *              C     ->  C blink
 *              CW    ->  C W blink 
 *              RGB   ->  R G B blink
 *              RGBC  ->  R G B C blink
 *              RGBCW ->  R G B C W blink
 *          above display base on  frequency
 * @param {UINT_T timerID -> timerID
 *         PVOID pTimerArg -> timer param}
 * @return: none
 * @retval: none
 */
STATIC VOID vAgingStart_time_cb(UINT_T timerID, PVOID pTimerArg)
{
    OPERATE_RET opRet = -1;
    STATIC UCHAR_T *ucLED_LIST = NULL;
    CHAR_T cLED_Num = -1;
    STATIC CHAR_T cIndex = -1;
    STATIC BOOL_T bStatus = TRUE;
    STATIC USHORT_T usStand_cnt = 0;
    STATIC USHORT_T usCheck_cnt = 0;
    STATIC USHORT_T usBright_Max = 0;

    if( NULL == ucLED_LIST ) {
        ucLED_LIST = (UCHAR_T *) Malloc(LED_MAX);
        if( NULL == ucLED_LIST ) {
            PR_ERR("start production aging start check malloc error,will try again!");
        }
        memset(ucLED_LIST, 0, LED_MAX);
        cLED_Num = cGet_Light_Led(PROD_CHECK, ucLED_LIST);
        
        if(cLED_Num < 1) {
            PR_ERR("programm internal error ！");
        }
        usBright_Max = usGetLight_MaxBright();          /* bright need to get from json */
    } 

    if( TRUE == bStatus ) {
        vProd_Send(ucLED_LIST[cIndex], usBright_Max);
        usStand_cnt++;
        if( (usStand_cnt * PROD_AGING_START_BASE_INTERVAL) >= PROD_AGING_START_ONOFF_TIMER_INTERVAL) {
            usStand_cnt = 0;
            bStatus = FALSE;
        }
    } else {
        vProd_Send(ucLED_LIST[cIndex], 0);
        usStand_cnt++;
        if( (usStand_cnt * PROD_AGING_START_BASE_INTERVAL) >= PROD_AGING_START_ONOFF_TIMER_INTERVAL) {
            usStand_cnt = 0;
            cIndex++;
            if(cIndex >= cLED_NUM) {
                cIndex = 0;
                usCheck_cnt++;
            }
            bStatus = TRUE;
        }   
    }

    if( usCheck_cnt  >= PROD_AGING_START_LOOP_CNT ) {     
        opRet = opSetProdTest_Mode(PROD_AGING, 0);          /* save test mode，change to aging mode! */
        if( OPRT_OK == opRet ) {
            opRet = sys_stop_timer(PT_Handle.agingstart_timer);
            if(OPRT_OK != opRet) {
                PR_ERR("Stop overall timer error!");
            }      
            vProdAging_Proc();                              /* goto production aging proc */
        } 
    }

}

/**
 * @berief: production start aging check
 * @param : none
 * @return: none
 * @retval: none
 */
STATIC VOID vAgingStart_Proc(VOID)
{
    OPERATE_RET opRet = -1;
s
    opRet = sys_add_timer(vAgingStart_time_cb, NULL, &PT_Handle.agingstart_timer);
    if( OPRT_OK != opRet ) {
        Light_Drive_Send(0,0,0,0,0);        /* light shut down */
        PR_ERR("production aging check start display init failure,light shut down!");
        return;
    }

    opRet = sys_start_timer(PT_Handle.agingstart_timer, PROD_AGING_START_BASE_INTERVAL, TIMER_CYCLE);
    if( OPRT_OK != opRet ) {
        Light_Drive_Send(0,0,0,0,0);        /* light shut down */
        PR_ERR("production aging check start display init failure,light shut down!");
        return;
    }
}

/**
 * @berief: Production factory test aging function display timer callback
 *          display styple
 *              C     ->  C aging
 *              CW    ->  C aging > W aging 
 *              RGB   ->  RGB aging
 *              RGBC  ->  C aging > RGB aging
 *              RGBCW ->  C aging > W aging > RGB aging
 *          above display base on  frequency
 * @param {UINT_T timerID -> timerID
 *         PVOID pTimerArg -> timer param}
 * @return: none
 * @retval: none
 */
STATIC VOID vProdAging_time_cb(UINT_T timerID, PVOID pTimerArg)
{
    OPERATE_RET opRet = -1;
    STATIC USHORT_T *usAgingTime_list = NULL;
    STATIC CHAR_T cAgingLed_num = -1;
    STATIC USHORT_T usAging_time = 0;   /* unit:min */
    STATIC UINT_T ulStand_cnt = 0;
    STATIC UCHAR_T ucIndex = 0;
    STATIC USHORT_T usBright_Max = 0;
    STATIC USHORT_T usBright_Min = 0;

    if( NULL == usAgingTime_list ) {
        usAgingTime_list = (USHORT_T *) Malloc(AGING_MAX);
        if( NULL == usAgingTime_list ) {
            PR_ERR("start production aging check malloc error,will try again!");
        }
        memset(usAgingTime_list, 0, AGING_MAX);
        opRet = cGetAgingTarget_Time(usAgingTime_list);
        if( OPRT_OK != opRet ) {
            PR_ERR("Get aging target time error!");
        }
        usAging_time = cGetProdTest_AgingTime();      
        PR_NOTICE("already aging time:%d(min)!",usAging_time);

        usBright_Max = usGetLight_MaxBright();
        usBright_Min = usGetLight_MinBright();
    }
    
    ulStand_cnt++;

    if( (ulStand_cnt * PROD_AGING_BASE_INTERVAL) >=  (60 * 1000)) {      /* save aging time per minutes */
        usAging_time++;
        opRet = opSetProdTest_Mode(PROD_AGING, usAging_time);
        if( OPRT_OK != opRet ) {
            usAging_time--;
            PR_NOTICE("save aging time error!");
        } else {
            ulStand_cnt = 0;
            PR_NOTICE("aging has pass %d min!", usAging_time );
        } 
    }
   
    if( usAging_time >= (usAgingTime_list[0] + usAgingTime_list[1] + usAgingTime_list[2]) ) {   /* aging time satisfied! */

        opRet = sys_stop_timer(PT_Handle.prodaging_timer);
        if( OPRT_OK != opRet ) {
            PR_ERR("Stop aging timer error, will try again!");
        } else {
            opRet = opSetProdTest_Mode(PROD_REPEAT, usAging_time);  /* save test mode! */
    
            cLight_Num = cGetLight_Num();    /* directly display aging finnal status */
            if( 1 == cLight_Num ) {
                vProd_Send(LED_C, usBright_Min);
            } else if( 2 == cLight_Num ){
                vProd_Send(LED_CW, usBright_Min);
            } else if( cLight_Num >= 3) {
                vProd_Send(LED_G, usBright_Min);
            }
        }
        return;
    }

    switch (ucIndex){
        case AGING_C:       /* constant-expression */
            vProd_Send(LED_C, usBright_Max);
            if( usAging_time >= usAgingTime_list[0] ) {
                ucIndex = AGING_W;
            }
            /* code */
            break;

        case AGING_W:
            vProd_Send(LED_W, usBright_Max);
            if( usAging_time >= (usAgingTime_list[0] + usAgingTime_list[1]) ) {
                ucIndex = AGING_RGB;
            }
            break;

        case AGING_RGB:
            vProd_Send(LED_RGB, usBright_Max);
            if( usAging_time >= (usAgingTime_list[0] + usAgingTime_list[1] + usAgingTime_list[2]) ) {
                ;   /* it's impossible */
            }
            break;
            
        default:
            break;
    }

}

/**
 * @berief: production aging check
 * @param {none} 
 * @return: none
 * @retval: none
 */
STATIC VOID vProdAging_Proc(VOID)
{
    OPERATE_RET opRet = -1;

    opRet = sys_add_timer(vProdAging_time_cb, NULL, &PT_Handle.prodaging_timer);
    if( OPRT_OK != opRet ) {
        Light_Drive_Send(0,0,0,0,0);        /* light shut down */
        PR_ERR("production aging check display init failure,light shut down!");
        return;
    }

    opRet = sys_start_timer(PT_Handle.prodaging_timer, PROD_AGING_BASE_INTERVAL, TIMER_CYCLE);
    if( OPRT_OK != opRet ) {
        Light_Drive_Send(0,0,0,0,0);        /* light shut down */
        PR_ERR("production aging check display init failure,light shut down!");
        return;
    }


}

/**
 * @berief: Production factory test repeat function display timer callback
 *          display styple
 *              C     ->  C bringt up then bright down
 *              CW    ->  C bringt up then bright down > W 
 *              RGB   ->  R bringt up then bright down > G > B
 *              RGBC  ->  R bringt up then bright down > G > B > C
 *              RGBCW ->  R bringt up then bright down > G > B > C > W
 *          above display base on  frequency
 * @param {UINT_T timerID -> timerID
 *         PVOID pTimerArg -> timer param}
 * @return: none
 * @retval: none
 */
STATIC VOID vProdRepeat_time_cb(UINT_T timerID, PVOID pTimerArg)
{
    OPERATE_RET opRet = -1;
    STATIC UCHAR_T *ucLED_LIST = NULL;
    CHAR_T cLED_NUM = -1;
    STATIC CHAR_T cIndex = -1;
    STATIC BOOL_T bStatus = FALSE;
    STATIC USHORT_T usBright_Max = 0;
    STATIC USHORT_T usChange_Step = 0;
    STATIC SHORT_T sSend_Value = 0;
    
    if( NULL == ucLED_LIST ) {
        ucLED_LIST = (UCHAR_T *) Malloc(LED_MAX);
        if( NULL == ucLED_LIST ) {
            PR_ERR("start production repeat check malloc error,will try again!");
        }
        memset(ucLED_LIST, 0, LED_MAX);
        cLED_NUM = cGet_Light_Led(PROD_REPEAT, ucLED_LIST);
        
        usBright_Max = usGetLight_MaxBright();          /* bright need to get from json */
        
        usChange_Step = usBright_Max / (PROD_REPEAT_ONOFF_TIMER_INTERVAL/PROD_REPEAT_BASE_INERVAL);
        //bright need to get from json
    } 

    if( TRUE == bStatus ) {
        vProd_Send(ucLED_LIST[cIndex], sSend_Value);
        sSend_Value += usChange_Step;
        if( sSend_Value >= usBright_Max) {
            bStatus = FALSE;
        }
    } else {
        vProd_Send(ucLED_LIST[cIndex], sSend_Value);
        sSend_Value -= usChange_Step;
        if( sSend_Value <= 0) {
            bStatus = TRUE;
            cIndex++;
            if(cIndex >= cLED_NUM) {
                cIndex = 0;
            }
        }   
    }

}

/**
 * @berief: production factory repeat test -- test2
 * @param {none} 
 * @return: none
 * @retval: none
 */
STATIC VOID vProdRepeat_Proc(VOID)
{
    OPERATE_RET opRet = -1;

    opRet = sys_add_timer(vProdCheck_time_cb, NULL, &PT_Handle.repeat_timer);
    if( OPRT_OK != opRet ) {
        Light_Drive_Send(0,0,0,0,0);        /* light shut down */
        PR_ERR("production repeat check display init failure,light shut down!");
        return;
    }

    opRet = sys_start_timer(PT_Handle.prodaging_timer, PROD_REPEAT_BASE_INERVAL, TIMER_CYCLE);
    if( OPRT_OK != opRet ) {
        Light_Drive_Send(0,0,0,0,0);        /* light shut down */
        PR_ERR("production repeat check display init failure,light shut down!");
        return;
    }
}

/**
 * @berief: prodution test process
 * @param {BOOL_T bAuthorized -> authorized flag
 *         CHAR_T cSignal_strength -> singal strength } 
 * @return: none
 * @retval: none
 */
VOID vProdTest_Cb(BOOL_T bAuthorized, CHAR_T cSignal_strength)
{
    OPERATE_RET opRet = -1;
    CHAR_T cMode = -1;

    if( cSignal_strength < AUZ_TEST_WEAK_SIGNAL ) {  /* weak singal */
        PR_ERR("Weak singal!");
        vWeakSignal_Proc();
        return;
    }
    
    if( FALSE == bAuthorized ) {                     /* unauthorized ! */
        PR_ERR("Production unauthorized!");
        vUnauthorized_Proc();
        return;
    }

    PR_NOTICE("PROD FACTORY START!");
    cMode = cGetProdTest_Mode();                    /* get test mode and aging time */

    switch (cMode)
    {
        case PROD_CHECK:            /* production overall check */
            vProdCheck_Proc();      
            break;

        case PROD_AGINGSTART:
        case PROD_AGING:
            vAgingStart_Proc();
            break;
            
        case PROD_REPEAT:
            vProdRepeat_Proc();
            break;
        
        default:
            break;
    }
}

/**
 * @berief: prodution test process init
 * @param {
 *         CHAR_T cSignal_strength -> singal strength } 
 * @return: none
 * @retval: none
 */
VOID vLightProd_Init()
{
    UCHAR_T ucMthd = 0;

    ucMthd = ucGetConnection_Cfg();

    app_cfg_set(mthd, vProdTest_Cb);

    system_prod_int();
    

}