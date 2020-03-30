/*
 * @Author: wls
 * @email: wuls@tuya.com
 * @LastEditors: wls
 * @file name: light_config.c
 * @Description: light production oem configuration get
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-05-06 10:57:08
 * @LastEditTime: 2019-05-15 14:58:49
 */

#include "light_types.h"
#include "light_flash.h"

/**
 * @berief: get oem light configuration led number
 * @param {none} 
 * @return: cLight_Num -> LED number
 * @retval: none
 */
CHAR_T cGetLight_Num(VOID)
{
    CHAR_T cLight_Num = -1;


    return (cGetLight_Num);
}

/**
 * @berief: get oem light network configuration led
 * @param {none} 
 * @return: ucNetConfigLED -> net config LED LED index
 * @retval: none
 */
UCHAR_T ucGetLight_NetConfigLED(VOID)
{
    UCHAR_T ucNetConfigLED = 0;

    return(ucNetConfigLED);
}

/**
 * @berief: get oem light configuration max bright
 * @param {none} 
 * @return: usMaxBright -> Light max bright value (rang: 0 ~ 1000)
 * @retval: none
 */
USHORT_T usGetLight_MaxBright(VOID)
{
    USHORT_T usMaxBright = 0;


    return(usMaxBright);
}

/**
 * @berief: get oem light configuration min bright
 * @param {none} 
 * @return: usMinBright -> Light min bright value (rang: 0 ~ 1000)
 * @retval: none
 */
USHORT_T usGetLight_MinBright(VOID)
{
    USHORT_T usMinBright = 0xFFFF;

    return(usMinBright);
}

/**
 * @berief: get oem light access network configuration bright
 * @param {none} 
 * @return: usNetConfigBright -> Light netconfig bright value (rang: 0 ~ 1000)
 * @retval: none
 */
USHORT_T usGetLight_NetConfigBright(VOID)
{   
    USHORT_T usNetConfigBright = 0;

    return(usNetConfigBright);
    
}


/**
 * @berief: get oem light configuration default bright
 * @param：none 
 * @return: 
 * @retval: none
 */
USHORT_T usGetLight_DefBright(VOID)
{

}

/**
 * @berief: get light sm2135 RGB current set
 * @param {type} none
 * @return: none
 * @retval: none
 */
UCHAR_T ucGetLight_SM2135RGBCurrent(VOID)
{

}

/**
 * @berief: get light sm2135 CW current set
 * @param {type} none
 * @return: none
 * @retval: none
 */
UCHAR_T ucGetLight_SM2135CWCurrent(VOID)
{
    
}

/**
 * @berief: get light sm2135 output channel list
 * @param {type} none
 * @return: none
 * @retval: none
 */
VOID vGetLight_SM2135List(UCHAR_T *ucResList)
{
    
}

/**
 * @berief: get light sm16726b power ctrl IO
 * @param {type} none
 * @return: none
 * @retval: none
 */
VOID vGetLight_SM16726BPowerCtrl(CHAR_T *cCtrl_IO, BOOL_T *bLevel)
{
    
}

/**
 * @berief: get light sm16726b channel list
 * @param {type} none
 * @return: none
 * @retval: none
 */
VOID vGetLight_SM16726BList(UCHAR_T * ucResList)
{
    
}

/**
 * @berief: get light IIC SCL
 * @param {type} none
 * @return: none
 * @retval: none
 */
CHAR_T cGetLight_I2CSCL(VOID)
{
    
}

/**
 * @berief: get light IIC SDA
 * @param {type} none
 * @return: none
 * @retval: none
 */
CHAR_T cGetLight_I2CSDA(VOID)
{
    
}


/**
 * @berief: get light pwm gpio list
 * @param {type} none
 * @return: none
 * @retval: none
 */
VOID vGetLight_PWMGPIOList(UCHAR_T *ucList)

/**
 * @berief: get light pwm drive polarity
 * @param {type} none
 * @return: none
 * @retval: none
 */
BOOL_T bGetLight_PWMPolarity(VOID)
{
    
}

/**
 * @berief: get light pwm drive duty
 * @param {type} none
 * @return: none
 * @retval: none
 */
USHORT_T usGetLight_PWMDuty(VOID)
{
    
}

/**
 * @berief: get light pwm drive frequency
 * @param {type} none
 * @return: none
 * @retval: none
 */
USHORT_T usGetLight_PWMFreq(VOID)
{
    
}

/**
 * @berief: get light drive mode
 * @param {type} none
 * @return: none
 * @retval: none
 */
UCHAR_T ucGetLight_DriveMode(VOID)
{
    // PWM SM16726B SM2135
    
    /* */
}

/**
 * @berief: get light wifi connect configuration
 * @param {type} none
 * @return: none
 * @retval: none
 */
UCHAR_T ucGetLight_WifiCFG(VOID)
{
    UCHAR_T ucWIFI_CFG = 0;

    
}

/**
 * @berief: light configuartion load 
 * @param {none} 
 * @return: none
 * @retval: none
 */
OPERATE_RET opLightConfigLoad(VOID)
{
    /* write flash to get oem !!! */

} 