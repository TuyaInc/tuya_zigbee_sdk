/*
 * @Author: wls
 * @email: wuls@tuya.com
 * @LastEditors:   
 * @file name: sm16726b.c
 * @Description: sm16726 IIC driver program
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-04-17 20:49:35
 * @LastEditTime: 2019-05-28 21:29:40
 */

#include "sm16726b.h"
#include "soc_i2c.h"
#include "light_printf.h"

#define IIC_DELAY               100         ///< IIC delay basic time

/**
 * @brief IIC enable ctrl gpio status enum
 * IIC IC enable ctrl gpio pin control status enum
 */
typedef enum
{
    CTRL_ON = 0,
    CTRL_OFF = 1, 
}CTRL_GPIO_STATUS_E;

/// sm16726 init parm
STATIC SM16726B_GPIO_S Init_Parm;
STATIC BOOL_T SM16726B_Init_Flag = FALSE;

/**
 * @berief: sm16726b IIC configurable delay proc 
 * @param {time -> delay time settings}
 * @return: none
 * @retval: none
 */
STATIC VOID SM16726B_Delayms(UINT_T time)
{
    volatile UCHAR_T i = 0;  

    while(time --)
    {
        i = IIC_DELAY;
        while(i --);
    }
}

/**
 * @berief: sm16726B IIC delay proc
 * @param {none} 
 * @return: none
 * @retval: none
 */
STATIC VOID SM16726B_I2C_DELAY(VOID)
{       
    volatile UCHAR_T j = 10; 
    
    while(j)
    {
        j --;
    }
}

/**
 * @berief: sm16726b IIC send byte
 * @param {SDA_IO -> SDA pin}
 * @param {SCL_IO -> SCL pin}
 * @param {SendByte -> send one byte}
 * @return: none
 * @retval: none
 */
STATIC VOID SM16726B_I2C_Send_Byte(UCHAR_T SendByte)
{
    UCHAR_T i = 0;

    for(i = 0; i < 8; i ++)
    {
        I2C_SCL_RESET();
        SM16726B_I2C_DELAY();

        if(SendByte & 0x80) {
            I2C_SDA_SET(); 
        } else {
            I2C_SDA_RESET();  
        }
    
        SendByte <<= 1;
        SM16726B_I2C_DELAY();

        I2C_SCL_SET();
        SM16726B_I2C_DELAY();  
    }
    I2C_SCL_RESET();
}

/**
 * @berief: sm16726b write data proc
 * @param {*pBuffer -> write RGB grayscale value buf(24bit)}
 * @param {NumByteToWrite -> write data len}
 * @return: OPERATE_RET
 * @retval: OPRT_INVALID_PARM
 * @retval: OPRT_COM_ERROR 
 * @retval: OPRT_OK 
 */
OPERATE_RET SM16726B_WritePage(UCHAR_T *pBuffer, USHORT_T NumByteToWrite)
{
    UCHAR_T start_bit_cnt = 6;

    while(start_bit_cnt--){
        SM16726B_I2C_Send_Byte(0x00);
    }
    SM16726B_I2C_Send_Byte(0x01);     /* send 50bits '0' firstly at least */

    while(NumByteToWrite --)
    {
        SM16726B_I2C_Send_Byte(*pBuffer ++);  
    }                                                         

    I2C_SDA_SET();       /* send action addition pulse pull sda pin high */
    SM16726B_I2C_DELAY();
    I2C_SCL_RESET();
    SM16726B_I2C_DELAY();
    I2C_SCL_SET();
    SM16726B_I2C_DELAY();  
    I2C_SCL_RESET();     /* send one addition pulse to scl pin */
    
    SM16726B_Delayms(2);
    return OPRT_OK;
}

/**
 * @berief: sm16726B init
 * @param {IN SM16726B_GPIO_S *Init_GPIO --> INIT Parm}
 * @return: none
 * @retval: none
 */
VOID SM16726B_CTRL(CTRL_GPIO_STATUS_E STATUS)
{
    if( Init_Parm.bCTRL_LV ){      /* HIGH LEVEL in effect */
        if( CTRL_ON == STATUS ) {
            I2C_CTRL_SET();
        } else {
            I2C_CTRL_RESET();
        }
    } else {                        /* LOW LEVEL in effect */
        if( CTRL_ON == STATUS ) {
            I2C_CTRL_RESET();
        } else {
            I2C_CTRL_SET();
        }
    }
} 

/**
 * @berief: sm16726B init
 * @param {IN SM16726B_GPIO_S Init_GPIO -> INIT Parm
 *          INT_T SDA_IO -> SDA IO
 *          INT_T SCL_IO -> SCL IO
 *          SM16726B_CTRL_S CTRL_PARM -> CTRL IO Parm
 *              INT_T CTRL_IO -> CTRL IO
 *              BOOL_T LEVEL -> HIGH/LOW LEVEL
 *          UCHAR_T *ucList -> RGB output channel config ucList}
 * @attention LEVEL setting
 *              TRUE    --> HIGH LEVEL effective
 *              FALSE   --> LOW LEVEL effective
 * @attention *ucList parm set
 *              RGB defalut output channel is R->0 G->1 B->2
 * @return: none
 * @retval: none
 */
OPERATE_RET opSM16726B_Init(IN SM16726B_GPIO_S Init_GPIO)
{
    if(Init_GPIO.uSDA_IO == Init_GPIO.uSCL_IO) {
        PR_ERR("SM16726B init is invalid!");
        return OPRT_INVALID_PARM;
    }

    if( (Init_GPIO.ucList[0] + Init_GPIO.ucList[1] + Init_GPIO.ucList[2]) != 3 ) {
        PR_ERR("SM16726B init is invalid!");
        return OPRT_INVALID_PARM;
    }
    
    if( PIN_NOEXIST == Init_GPIO.uCTRL_IO ) {
        PR_NOTICE("SM16726B NO CTRL PIN!");
    } 
 
    memcpy(&Init_Parm, &Init_GPIO, SIZEOF(SM16726B_GPIO_S)) ;
    
    i2c_pin_t i2c_config = {
        .sda_pin_num = Init_GPIO.uSDA_IO,
        .scl_pin_num = Init_GPIO.uSCL_IO,
        .ctrl_pin_num = Init_GPIO.uCTRL_IO, 
    };

    vSocI2CInit(i2c_config);          /* SDA&SCL GPIO INIT */

    if( TRUE == Init_GPIO.bCTRL_LV) {
        I2C_CTRL_RESET();
    } else {
        I2C_CTRL_SET();
    }
    
    SM16726B_Init_Flag = TRUE;
    return OPRT_OK;
}

/**
 * @berief: sm16726b send data
 * @param {IN SM16726B_SendData -> need write data structure to sm16726B
 *          SendData.R_Value -> R value,rang from 0 to 256
 *          SendData.G_Value -> G value,rang from 0 to 256
 *          SendData.B_Value -> B value,rang from 0 to 256}
 * @return: OPERATE_RET
 * @retval: OPRT_INVALID_PARM -> invalid parm
 * @retval: OPRT_OK ->operate ok
 */
OPERATE_RET opSM16726B_SetColor(IN SM16726B_ColoerData_S SendData)
{
    UCHAR_T IIC_Sendbuf[3] = {0,0,0};
    OPERATE_RET ret = OPRT_OK;

    if( SM16726B_Init_Flag != TRUE ) {
        PR_ERR("SM16726B not init!");
        return OPRT_INVALID_PARM;
    }
    
    if( PIN_NOEXIST != Init_Parm.uCTRL_IO ) {
        if(( 0 == SendData.usR_Value ) && ( 0 == SendData.usG_Value ) && ( 0 == SendData.usB_Value )){
            SM16726B_CTRL(CTRL_OFF);
        } else {
            SM16726B_CTRL(CTRL_ON);
        }
               
        SM16726B_Delayms(10);   /* delay wait  */ 
    }
    
    if(( 0 == SendData.usR_Value ) && ( 0 == SendData.usG_Value ) && ( 0 == SendData.usB_Value )) {
        I2C_SCL_RESET();        /* when no RGB data, pull down scl&sdas */
        I2C_SDA_RESET();
    } else {
        IIC_Sendbuf[Init_Parm.ucList[0]] = SendData.usR_Value;
        IIC_Sendbuf[Init_Parm.ucList[1]] = SendData.usG_Value;
        IIC_Sendbuf[Init_Parm.ucList[2]] = SendData.usB_Value;
        ret = SM16726B_WritePage(IIC_Sendbuf, 3);
        if( OPRT_OK != ret) {
            PR_ERR("SM16726B Write Error!"); 
        }
    }

    return OPRT_OK;

}