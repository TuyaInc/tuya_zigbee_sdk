/*
 * @Author: wls
 * @email: wuls@tuya.com
 * @LastEditors: wls
 * @file name: light_config.h
 * @Description: light production oem configuration include file
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-04-26 13:55:40
 * @LastEditTime: 2019-05-15 15:10:24
 */

#ifndef __LIHGT_CONFIG_H__
#define __LIHGT_CONFIG_H__

/* this need to think globally  */
enum {
    Send_R = 0;
    Send_G,
    Send_B,
    Send_C,
    Send_W
};


#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/**
 * @berief: get oem light network configuration led
 * @param {none} 
 * @return: ucNetConfigLED -> net config LED LED index
 * @retval: none
 */
UCHAR_T ucGetLight_NetConfigLED(VOID);

/**
 * @berief: get oem light access network configuration bright
 * @param {none} 
 * @return: usNetConfigBright -> Light netconfig bright value (rang: 0 ~ 1000)
 * @retval: none
 */
USHORT_T usGetLight_NetConfigBright(VOID);


/**
 * @berief: get light sm2135 RGB current set
 * @param {type} none
 * @return: none
 * @retval: none
 */
UCHAR_T ucGetLight_SM2135RGBCurrent(VOID);

/**
 * @berief: get light sm2135 CW current set
 * @param {type} none
 * @return: none
 * @retval: none
 */
UCHAR_T ucGetLight_SM2135CWCurrent(VOID);

/**
 * @berief: get light sm2135 output channel list
 * @param {type} none
 * @return: none
 * @retval: none
 */
VOID vGetLight_SM2135List(UCHAR_T *ucResList);

/**
 * @berief: get light sm16726b power ctrl IO
 * @param {type} none
 * @return: none
 * @retval: none
 */
VOID vGetLight_SM16726BPowerCtrl(CHAR_T *cCtrl_IO, BOOL_T *bLevel);

/**
 * @berief: get light sm16726b channel list
 * @param {type} none
 * @return: none
 * @retval: none
 */
VOID vGetLight_SM16726BList( UCHAR_T * ucResList );

/**
 * @berief: get light IIC SCL
 * @param {type} none
 * @return: none
 * @retval: none
 */
CHAR_T cGetLight_I2CSCL(VOID);

/**
 * @berief: get light IIC SDA
 * @param {type} none
 * @return: none
 * @retval: none
 */
CHAR_T cGetLight_I2CSDA(VOID);


/**
 * @berief: get oem light configuration led number
 * @param {none} 
 * @return: cLight_Num -> LED number
 * @retval: none
 */
CHAR_T cGetLight_Num(VOID);

/**
 * @berief: get light pwm gpio list
 * @param {type} none
 * @return: none
 * @retval: none
 */
VOID vGetLight_PWMGPIOList(UCHAR_T *ucList);

/**
 * @berief: get light pwm drive polarity
 * @param {type} none
 * @return: none
 * @retval: none
 */
BOOL_T bGetLight_PWMPolarity(VOID);

/**
 * @berief: get light pwm drive duty
 * @param {type} none
 * @return: none
 * @retval: none
 */
USHORT_T usGetLight_PWMDuty(VOID);

/**
 * @berief: get light pwm drive frequency
 * @param {type} none
 * @return: none
 * @retval: none
 */
USHORT_T usGetLight_PWMFreq(VOID);

/**
 * @berief: get light wifi connect configuration
 * @param {type} none
 * @return: none
 * @retval: none
 */
UCHAR_T ucGetLight_WifiCFG(VOID);

/**
 * @berief: light configuartion load 
 * @param {none} 
 * @return: none
 * @retval: none
 */
OPERATE_RET opLightConfigLoad(VOID);





#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif  /* __LIHGT_CONFIG_H__ */
