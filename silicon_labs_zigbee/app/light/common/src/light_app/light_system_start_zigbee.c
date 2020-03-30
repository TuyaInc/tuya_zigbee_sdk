/**************************************************************
 * @file common main file for light
 * @version 1.0.0
 * @author Jin Lu 
 * @data 2019.04.26
 *************************************************************/
#include "stdio.h"
#include "stdlib.h"
#include "mf_test_callback.h"
#include "zigbee_sdk.h"
#include "zigbee_dev_template.h"
#include "config.h"
#include "device_config_load.h"
#include "user_pwm.h"
#include "soc_i2c.h"
#include "sm2135.h"
#include "sm16726b.h"


pwm_gpio_t pwm_gpio_config[] = {
  {PORT_D,PIN_15, GPIO_LEVEL_HIGH, 1},
  {PORT_A,PIN_2, GPIO_LEVEL_HIGH, 0}, 
  {PORT_A,PIN_0, GPIO_LEVEL_HIGH, 0},
  {PORT_B,PIN_11, GPIO_LEVEL_LOW, 0},
  {PORT_F,PIN_4, GPIO_LEVEL_HIGH, 0}
};

static void dev_key_handle(uint32_t key_id, key_st_t key_st, uint32_t push_time);

/**
 * @description: device first power on, user can register device and init the gpio
 * @param {type} none
 * @return: none
 */

void dev_power_on_init(void)
{

}

/**
 * @description: system start on, zigbee stack is inited completely and
 * manufactury test is ended, the uart and the timer is ok, user can
 * init the application event and any other application variables;
 * @param {type} none
 * @return: none
 */
void dev_system_on_init(void)
{
    uint8_t ret = 0;
    uint8_t js_ver[5] = {0};
    uint32_t pwm = 0;


    device_iic_t iic_config;
    device_color_pin_t device_color_pin; 
    device_white_balance_t white_balance;
    device_gamma_t gamma;

    ret = device_config_data_load();
    get_jsonver(js_ver);
    get_iic_rgbcw_index(&iic_config);
    get_color_pin_config(&device_color_pin);
    get_white_balance_config(&white_balance);
    get_gamma_config(&gamma);

    ret = get_color_mode();
    
    ret = get_wfcfg_config();
    ret = get_reset_color();
    ret = get_connected_color();
    ret = get_cw_max_power();
    ret = get_connectting_btight();
    ret = get_connected_btight();
    ret = get_reset_mode();
    ret = get_reset_num();
    ret = get_key_pin();
    ret = get_key_lv();
    ret = get_reset_time();
    pwm = get_pwmhz_config();

/*   
//hal iic (SDK API)
    i2c_gpio_t i2c_gpio = {
        .sda.port = PORT_D,
        .sda.pin = PIN_15,
        .scl.port = PORT_A,
        .scl.pin = PIN_2,
        .power.port = PORT_A,
        .power.pin = PIN_3,
        .power_pin_enable = TRUE,
    };

    uint8_t iic_id = 0;
    hal_i2c_init(&i2c_gpio, iic_id);

    hal_i2c_power_enable(iic_id);
    
    hal_i2c_start(iic_id);
    hal_i2c_send_byte(iic_id, 0x5A);
    hal_i2c_stop(iic_id);
    
    hal_i2c_start(iic_id);
    hal_i2c_send_byte(iic_id, 0x68);
    hal_i2c_stop(iic_id);
    
    hal_i2c_start(iic_id);
    hal_i2c_send_byte(iic_id, 0x5A);
    hal_i2c_stop(iic_id);
    
    hal_i2c_power_disable(iic_id);
*/
  
/*
//soc iic
    i2c_pin_t i2c_config = {
        .sda_pin_num = 3,
        .scl_pin_num = 12,
        .ctrl_pin_num = 5,
    };
    vSocI2CInit(i2c_config);
    
    I2C_SDA_RESET();
    I2C_SDA_SET();
    I2C_SDA_RESET();
    I2C_SDA_SET();
    I2C_SDA_RESET();
    I2C_SDA_SET();
    
    I2C_SCL_RESET();
    I2C_SCL_SET();
    I2C_SCL_RESET();
    I2C_SCL_SET();
    I2C_SCL_RESET();
    I2C_SCL_SET();
    I2C_SCL_RESET();
    I2C_SCL_SET();
    
    
    I2C_CTRL_RESET();
    I2C_CTRL_SET();
    I2C_CTRL_RESET();
    I2C_CTRL_SET();
    I2C_CTRL_RESET();
    I2C_CTRL_SET();
    I2C_CTRL_RESET();
    I2C_CTRL_SET();
    I2C_CTRL_RESET();
    I2C_CTRL_SET();    
*/

/*
// sm2135.c 
    SM2135_GPIO_S config = {
        .cSDA_IO = 12,
        .cSCL_IO = 3,
        .ucList = {2,1,0,3,4},
        .ucRGB_Current = 20,
        .ucCW_Current = 25

    };
    SM2135_SendData_S color = {
        .usR_Value = 10,       ///< R value,rang from 0 to 256
        .usG_Value = 100,      ///< G value,rang from 0 to 256
        .usB_Value = 255,      ///< B value,rang from 0 to 256
        .usC_Value = 255,      ///< C value,rang from 0 to 256
        .usW_Value = 10,      ///< W value,rang from 0 to 256
        .ucMode = SM2135_RGB_Mode,        ///< output mode
    };

    opSM2135_Init(config);
    opSM2135_SendOut(color);
*/
// sm16726.c
    SM16726B_GPIO_S config = {
        .uSDA_IO = 12,
        .uSCL_IO = 3,
        .uCTRL_IO = 5,
        .bCTRL_LV = 1,
        .ucList = {0,1,2},
    };
    SM16726B_ColoerData_S color = {
        .usR_Value = 0,       ///< R value,rang from 0 to 256
        .usG_Value = 0,      ///< G value,rang from 0 to 256
        .usB_Value = 255,      ///< B value,rang from 0 to 256
    };

    opSM16726B_Init(config);
    opSM16726B_SetColor(color);



    
    
/*
//hal pwm (SDK API)
    hal_pwm_init(pwm_gpio_config, get_array_len(pwm_gpio_config),10000);
    uint16_t max_value = get_max_pwm_value();
    uint16_t value = max_value/2;
    set_pwm_value(0, value);
    set_pwm_value(1, value);
    set_pwm_value(2, value);
    set_pwm_value(3, value);
    set_pwm_value(4, value);
*/
/*
//soc pwm
    char module[20]={0};
    uint8_t len = 0;
    memset(module,0,sizeof(module));
    get_module_string(module, &len);
    
    ret = bSocGpioModuleInit(module,len);
    uint8_t list[5] = {5,9,10,11,12};
    vSocPwmInit(10000,0,5,list);
    vSocPwmSetDuty(50,0);
    vSocPwmSetDuty(20,1);
    vSocPwmSetDuty(20,2);
    vSocPwmSetDuty(20,3);
    vSocPwmSetDuty(80,4);
*/
/*
//user pwm
    char module[20]={0};
    uint8_t len = 0;
    memset(module,0,sizeof(module));
    get_module_string(module, &len);
    ret = bSocGpioModuleInit(module,len);
    
    uint8_t pin_list[] = {3,12,7,5,13};

    USER_PWM_INIT_S pwm_init ={
        .usFreq = 10000,
        .usDuty = 100,
        .ucList = pin_list,
        .ucListNum = size_get(pin_list),
        .bPolarity = 1,
    };
    
    USER_PWM_Init(&pwm_init);

    USER_PWM_SendData_S out_data = {
       .R_Value = 50,       ///< R value,rang from 0 to 1000
       .G_Value = 100,       ///< G value,rang from 0 to 1000
       .B_Value = 200,       ///< B value,rang from 0 to 1000
       .C_Value = 300,       ///< C value,rang from 0 to 1000
       .W_Value = 400,       ///< W value,rang from 0 to 1000
    };

    USER_PWM_SendOut(out_data);
*/
    
    
    return;
}

/**
 * @description: network state changed callback, when network state is changed,
 * the stack will call this function to notify the user;
 * @param {state} the new network state
 * @return: none
 */
void nwk_state_changed_callback(NET_EVT_T state)
{
    switch(state) {
        case NET_POWER_ON_LEAVE: {
            break;
        }

        case NET_JOIN_START: {
            break;
        }
        case NET_JOIN_TIMEOUT: {
            break;
        }
        case NET_POWER_ON_ONLINE:
        case NET_JOIN_OK: {
            break;
        }
        case NET_LOST: {
            break;
        }
        case NET_REMOTE_LEAVE: {
            break;
        }
        case NET_LOCAL_LEAVE: {
            break;
        }
        default: {
            break;
        }
    }
}

/**
 * @description: device manufactury test callback, when device is in manufactury test model,
 * sdk will use this callback to notify application the test item and test command;
 * @param {cmd} manufactury test type
 * @param {*args} manufactury test data
 * @param {arg_len} manufactury test data length
 * @return: none
 */
MF_TEST_RET_T dev_mf_test_callback(MF_TEST_CMD_T cmd, uint8_t *args, uint16_t arg_len)
{
    //TODO

    return MF_TEST_SUCCESS;
}

/**
 * @description: device manufactury beacon test callback, when device is in manufactury test mode,
 * sdk will use this callback to notify application to enter unsupervised production test;
 * @param {type} none
 * @return: none
 */
void dev_beacon_test_callback(void)
{
    // TODO

    return;
}

/**
 * @description: device report inforamtion when first power on(if need),;
 * @param {type} none
 * @return: none
 */
void dev_power_on_report_info(void)
{
    //TODO

    return;
}

/**
 * @description: device recovery to factury command handler
 * @param {type} none
 * @return: none
 */
void dev_recovery_factory(void)
{
    //TODO

    return;
}

/**
 * @description: device receive message callback
 * @param {*dev_msg} received message information
 * @return: ZCL_CMD_RET_T
 */
ZCL_CMD_RET_T dev_msg_recv_callback(dev_msg_t *dev_msg)
{
    //TODO

    return ZCL_CMD_RET_SUCCESS;
}

/**
 * @description: device recall scene command, recovery the scene add status
 * @param {endpoint} the endpoint of scene recall command
 * @param {*in_data} the data need to be handled of the device status
 * @return: none
 */
void dev_scene_recall_callback(uint8_t endpoint, const scene_save_data_t *in_data)
{
    //TODO

    return;
}

/**
 * @description: device scene add command, store the current status to the scene table
 * @param {endpoint} the endpoint of scene add command
 * param {*out_data} the data need to be store to scene table
 * param {*in_out_len} the data length of the store data
 * @return: none
 */
void dev_scene_add_callback(uint8_t endpoint, uint8_t *out_data, uint8_t *in_out_len)
{
    //TODO

    return;
}

/**
 * @description: write sttribute callback function
 * @param {endpoint} the endpoint of the write attribute
 * @param {cluster} the cluster id of the write attribute
 * @param {attr_id} the attribute id  of the write attribute
 * @return: none
 */
void dev_msg_write_attr_callback(uint8_t endpoint, CLUSTER_ID_T cluster, uint16_t attr_id)
{
    //TODO

    return;
}

/**
 * @description: key event handler 
 * @param {key_id} key  id
 * @param {key_st} key status
 * @param {push_time} key push time
 * @return: none
 */
static void dev_key_handle(uint32_t key_id, key_st_t key_st, uint32_t push_time)
{
    //TODO

    return;
}
/**
 * @description: mf test uart register, aotomatic generated, not edit
 * @param {void} none
 * @return: user_uart_config_t mf uart config information
 */
user_uart_config_t config;
user_uart_config_t* mf_test_uart_config(void)
{

    memset(&config, 0, sizeof(user_uart_config_t));

    if (MODULE_NAME == TYZS2R)
    {
        user_uart_config_t default_config = TYZS2R_USART_CONFIG_DEFAULT;
        memcpy(&config, &default_config, sizeof(user_uart_config_t));
    }
    else if(MODULE_NAME == TYZS5)
    {
        user_uart_config_t default_config = TYZS5_USART_CONFIG_DEFAULT;
        memcpy(&config, &default_config, sizeof(user_uart_config_t));
    }
	// else if(MODULE_NAME == TYZS2) //the current lib is efr32mg13p732, not support module TYZS2
	// {
	// 	user_uart_config_t default_config = TYZS2_USART_CONFIG_DEFAULT;
    //     memcpy(&config, &default_config, sizeof(user_uart_config_t));
	// }
    else
    {
        user_uart_config_t default_config = TYZS3_USART_CONFIG_DEFAULT;
        memcpy(&config, &default_config, sizeof(user_uart_config_t));
    }

    return &config;
}
