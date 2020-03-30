/*
 * @Author: wls
 * @email: wuls@tuya.com
 * @LastEditors:   
 * @file name: sm2135.c
 * @Description: SM2135 IIC driver program
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-04-16 11:44:21
 * @LastEditTime: 2019-05-28 21:26:13
 */

#include "sm2135.h"
#include "soc_i2c.h"
#include "light_printf.h"

#define IIC_DELAY                       100         ///< IIC delay basic time

#define SM2135_Current_Address          0xC0        ///< sm2135 set current address
#define SM2135_RGBCW_Choose_Address     0xC1        ///< sm2135 RGB or CW func choose address
#define SM2135_R_Set_Address            0xC2        ///< sm2135 set R grayscale address
#define SM2135_G_Set_Address            0xC3        ///< sm2135 set G grayscale address
#define SM2135_B_Set_Address            0xC4        ///< sm2135 set B grayscale address
#define SM2135_C_Set_Address            0xC5        ///< sm2135 set C grayscale address
#define SM2135_W_Set_Address            0xC6        ///< sm2135 set W grayscale address

#define SM2135_RGB_MAXCurrent           45          ///< sm2135 RGB mode configurable max current (unit mA)
#define SM2135_CW_MAXCurrent            60          ///< sm2135 CW mode configurable max current (unit mA)

STATIC SM2135_GPIO_S Init_Parm;
BOOL_T SM2135_Init_Flag = FALSE;


/**
 * @berief: sm2135 IIC configurable delay proc 
 * @param {time -> delay time settings}
 * @return: none
 * @retval: none
 */
STATIC VOID SM2135_Delayms(UINT_T time)
{
    volatile UCHAR_T i = 0;  

    while(time --)
    {
        i = IIC_DELAY;
        while(i --);
    }
}

/**
 * @berief: sm2135 IIC delay proc
 * @param {none} 
 * @return: none
 * @retval: none
 */
STATIC VOID SM2135_I2C_Delay(VOID)
{       
    volatile UCHAR_T j = 10; 
    while(j)
    {
        j --;
    }
}

/**
 * @berief: sm2135 IIC start transport
 * @param {SDA_IO -> SDA pin}
 * @param {SCL_IO -> SCL pin}
 * @return: operate result
 * @retval: OPRT_COM_ERROR
 * @retval: OPRT_OK
 */
STATIC OPERATE_RET SM2135_I2C_Start(VOID)
{
    I2C_SCL_SET();
    I2C_SDA_SET();

    SM2135_I2C_Delay();

    if(!I2C_SDA_OUTPUT_READ()) {    /* confirm SDA status is ready */
        return OPRT_COM_ERROR;
    }

    I2C_SDA_RESET();
    SM2135_I2C_Delay();

    if(I2C_SDA_OUTPUT_READ()) {    /* confirm SDA status is set correct */
        return OPRT_COM_ERROR;
    }

    return OPRT_OK;
}

/**
 * @berief: sm2135 IIC stop transport
 * @param {SDA_IO -> SDA pin}
 * @param {SCL_IO -> SCL pin}
 * @return: none
 * @retval: none
 */
STATIC VOID SM2135_I2C_Stop(VOID)
{
    I2C_SDA_RESET();
    I2C_SCL_RESET();

    SM2135_I2C_Delay();

    I2C_SCL_SET();
    SM2135_I2C_Delay();

    I2C_SDA_SET();
    SM2135_I2C_Delay();
}

/**
 * @berief: sm2135 IIC wait ACK
 * @param {SDA_IO -> SDA pin}
 * @param {SCL_IO -> SCL pin}
 * @return: OPERATE_RET
 * @retval: OPRT_COM_ERROR
 * @retval: OPRT_OK
 */
STATIC OPERATE_RET SM2135_I2C_Wait_Ack(VOID)
{
    I2C_SCL_RESET();
    SM2135_I2C_Delay();

    I2C_SCL_SET();
    SM2135_I2C_Delay();
    if(I2C_SDA_INPUT_READ()) {     /* receive ACK NG */
        return OPRT_COM_ERROR;
    }
    SM2135_I2C_Delay();
    return OPRT_OK;
}

/**
 * @berief: sm2135 IIC send byte
 * @param {SDA_IO -> SDA pin}
 * @param {SCL_IO -> SCL pin}
 * @param {SendByte -> send one byte}
 * @param {Send}
 * @return: none
 * @retval: none
 */
STATIC VOID SM2135_I2C_Send_Byte(UCHAR_T SendByte)
{
    UCHAR_T i = 0;

    for(i = 0; i < 8; i ++)
    {
        I2C_SCL_RESET();
        SM2135_I2C_Delay();

        if(SendByte & 0x80) {
            I2C_SDA_SET();
        } else {
            I2C_SDA_RESET();
        }

        I2C_SCL_SET();
        SM2135_I2C_Delay();

        SendByte <<= 1;
   }
}

/**
 * @berief: sm2135 write data proc
 * @param {*pBuffer -> write data buf}
 * @param {NumByteToWrite -> write data len}
 * @return: OPERATE_RET
 * @retval: OPRT_INVALID_PARM
 * @retval: OPRT_COM_ERROR 
 * @retval: OPRT_OK 
 */
STATIC OPERATE_RET SM2135_WritePage(UCHAR_T *pBuffer, USHORT_T NumByteToWrite)
{
    OPERATE_RET ret = OPRT_OK;

    if( NULL == pBuffer  ) {
        PR_ERR("SM2315 write data is invalid!");
        return OPRT_INVALID_PARM;
    }
 
    ret = SM2135_I2C_Start();     /* start transport */
    if( OPRT_OK != ret ) {
        PR_ERR("IIC is busy!");
        return OPRT_COM_ERROR;
    }
    
    while(NumByteToWrite --) {
        
        SM2135_I2C_Send_Byte(*pBuffer++);
        SM2135_I2C_Wait_Ack();
    }

    SM2135_I2C_Stop();
    SM2135_Delayms(10);

    return OPRT_OK;
}

/**
 * @berief: sm3125 set max current
 * @param {RGB_Set -> RGB mode max current settings}
 * @param {CW_Set -> CW mode max current settings}
 * @return: Result
 * @retval: Result -> the value of settings
 */
STATIC UCHAR_T SM2135_Set_Current(UCHAR_T RGB_Set, UCHAR_T CW_Set)
{
    UCHAR_T Result = 0;

    if( RGB_Set > SM2135_RGB_MAXCurrent ) {   
        RGB_Set = SM2135_RGB_MAXCurrent;
    }

    if( CW_Set > SM2135_CW_MAXCurrent ) {      
        CW_Set = SM2135_CW_MAXCurrent;
    }

    Result = ((( RGB_Set - 10 ) / 5 ) << 4 ) |(( CW_Set - 10 ) / 5 );

    return (Result);
}

/**
 * @berief: sm3125 init
 * @param {IN SM3125_GPIO_S Init_GPIO -> INIT Parm
 *          INT_T SDA_IO -> SDA IO
 *          INT_T SCL_IO -> SCL IO
 *          UCHAR_T ucList[5] -> RGBCW output channel config ucList
 *          RGB_Current -> RGB mode max current settings
 *          ucCW_Current -> CW mode max current settings}
 * @attention ucList[5] parm set
 *              RGB configurable output channel is 0~2
 *              CW configurable output channel is 3~4
* @attention ucRGB_Current parm set
 *              ucRGB_Current parm configurable max current <= 45mA
 * @attention ucCW_Current parm set
 *               ucCW_Current parm configurable max current <= 60mA
 * @return: OPERATE_RET
 * @retval: none
 */
OPERATE_RET opSM2135_Init(IN SM2135_GPIO_S Init_GPIO)
{
    if(Init_GPIO.cSDA_IO == Init_GPIO.cSCL_IO)  {
        PR_ERR("SM2135 gpio init is invalid!");
        return OPRT_INVALID_PARM;
    }

    if( (Init_GPIO.ucList[0] + Init_GPIO.ucList[1] + Init_GPIO.ucList[2]) != 3 || \
        (Init_GPIO.ucList[3] + Init_GPIO.ucList[4] != 7)){      /* RGB configurable output channel must be 0~2 */
        PR_ERR("SM2135 gpio ucList is error!");
        return OPRT_INVALID_PARM;
    }

    if( SM2135_RGB_MAXCurrent < Init_GPIO.ucRGB_Current ) {     /* RGB max current setting judge */
        PR_ERR("RGB MAX Current Set Error!");
        return OPRT_INVALID_PARM;
    }

    if( SM2135_CW_MAXCurrent < Init_GPIO.ucCW_Current ) {       /* CW max current setting judge */
        PR_ERR("CW MAX Current Set Error!");
        return OPRT_INVALID_PARM;
    }

    memcpy(&Init_Parm, &Init_GPIO, SIZEOF(SM2135_GPIO_S));        /* save sm2135 init parm */

    i2c_pin_t i2c_config = {
        .sda_pin_num = Init_GPIO.cSDA_IO,
        .scl_pin_num = Init_GPIO.cSCL_IO,
        .ctrl_pin_num = 0xFF, // SM2135 Do not need ctrl pin
    };
    vSocI2CInit(i2c_config);          /* SDA&SCL GPIO INIT */

    SM2135_Init_Flag = TRUE;
    
    return OPRT_OK;
}

/**
 * @berief: sm2135 send data
 * @param {IN SM2135_SendData -> need write data structure to sm2135
 *          usR_Value -> R value,rang from 0 to 256
 *          usG_Value -> G value,rang from 0 to 256
 *          usB_Value -> B value,rang from 0 to 256
 *          usC_Value -> C value,rang from 0 to 256
 *          usW_Value -> W value,rang from 0 to 256
 *          ucMode -> output ucMode}
 * @attention sm2135 send ucMode must be SM2135_White_Mode or SM2135_RGB_Mode 
 *              at same moment 
 * @return: OPERATE_RET
 * @retval: OPRT_INVALID_PARM -> invalid parm
 * @retval: OPRT_COM_ERROR  -> send data error
 */
OPERATE_RET opSM2135_SetColor(IN SM2135_ColoerData_S SendData)
{
    UCHAR_T IIC_Sendbuf[6] = {0,0,0,0,0,0};
    OPERATE_RET ret = OPRT_OK;

    if( SM2135_Init_Flag != TRUE ) {          /* sm2135 not init ! */
        PR_ERR("SM2135 not init!");
        return OPRT_INVALID_PARM;
    }

    if( SendData.ucMode > SM2135_RGB_Mode ) {
        PR_ERR("SM2135 Work Mode Error!");
        return OPRT_INVALID_PARM;
    }

    if( SendData.ucMode == SM2135_White_Mode ) {
        IIC_Sendbuf[0] = SM2135_Current_Address;        /* stuff register address */
        IIC_Sendbuf[1] = SM2135_Set_Current(Init_Parm.ucRGB_Current, Init_Parm.ucCW_Current);
        IIC_Sendbuf[2] = 0x80 ;
        ret = SM2135_WritePage(IIC_Sendbuf, 3);
        if( OPRT_OK != ret) {
            PR_ERR("SM2135 Write Error!");
            return OPRT_COM_ERROR; 
        }
        IIC_Sendbuf[0] = SM2135_C_Set_Address;           /* stuff register address */
        IIC_Sendbuf[ Init_Parm.ucList[3] - 2 ] = SendData.usC_Value;
        IIC_Sendbuf[ Init_Parm.ucList[4] - 2 ] = SendData.usW_Value;
        ret = SM2135_WritePage(IIC_Sendbuf, 3);
        if( OPRT_OK != ret) {
            PR_ERR("SM2135 Write Error!"); 
            return OPRT_COM_ERROR;
        }

    } else {
        IIC_Sendbuf[0] = SM2135_Current_Address;         /* stuff register address */
        IIC_Sendbuf[1] = SM2135_Set_Current(Init_Parm.ucRGB_Current, Init_Parm.ucCW_Current);
        IIC_Sendbuf[2] = 0x00;
        IIC_Sendbuf[ 3 + Init_Parm.ucList[0] ] = SendData.usR_Value;
        IIC_Sendbuf[ 3 + Init_Parm.ucList[1] ] = SendData.usG_Value;
        IIC_Sendbuf[ 3 + Init_Parm.ucList[2] ] = SendData.usB_Value;
        ret = SM2135_WritePage(IIC_Sendbuf, 6);
        if( OPRT_OK != ret) {
            PR_ERR("SM2135 Write Error!"); 
            return OPRT_COM_ERROR;
        }
    }
    
    return OPRT_OK;
}
