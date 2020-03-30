/**
 * @Author: jinlu
 * @file name: device_config_load.h
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 */
#ifndef __DEVICE_CONFIG_LOAD__
#define __DEVICE_CONFIG_LOAD__

#include "stdio.h"
#include "stdlib.h"
#include "zigbee_sdk.h"
#include "device_config_load.h"
#include "light_tools.h"

#ifndef OUT
#define OUT
#endif
#ifndef IN
#define IN
#endif

typedef enum {
    TYPE_HEX = 0,
    TYPE_STRING,
}VALUE_TYPE;

typedef enum{
    CMOD_C = 0x00,
    CMOD_CW = 0x01 ,
    CMOD_RGB = 0x02,
    CMOD_RGBC = 0x03,
    CMOD_RGBCW = 0x04
}CMODE_T;

typedef enum{
    DMOD_PWM = 0x00,
    DMOD_IIC  = 0x01
}DMODE_T;

typedef enum{
    GWCM_OLD = 0x00,
    GWCM_LOW_POWER = 0x01,
    GWCM_SPCL_MODE = 0x02,
    GWCM_OLD_PROD = 0x03
}WFCFG_T;

typedef enum{
    COLOR_C = 0x00,
    COLOR_W = 0x01,
    COLOR_R = 0x02,
    COLOR_G = 0x03,
    COLOR_B = 0x04
}COLOR_T;

typedef struct {
    char* key;
    void* value;
    uint8_t value_size;
    VALUE_TYPE type;
}key_value_t;

typedef struct {
    char*   string;
    uint8_t enum_value;
}string_to_enum_t;

typedef struct {
//common
    uint8_t json_version[5];
    uint8_t category;

//IIC pin config
    uint8_t iic_r_index;
    uint8_t iic_g_index;
    uint8_t iic_b_index;
    uint8_t iic_c_index;
    uint8_t iic_w_index;
    uint8_t iic_ctrl_pin;
    uint8_t iic_scl_pin;
    uint8_t iic_sda_pin;

//color pin config
    uint8_t color_r_pin;   // Red color control pin
    uint8_t color_r_lv;    // Red color control pin level
    uint8_t color_g_pin;   // Green color control pin
    uint8_t color_g_lv;    // Green color control pin level
    uint8_t color_b_pin;   // Blue  color control pin
    uint8_t color_b_lv;    // Blue  color control pin level
    uint8_t color_c_pin;   // Cold white color control pin
    uint8_t color_c_lv;    // Cold white color control pin level
    uint8_t color_w_pin;   // Warm white color control pin
    uint8_t color_w_lv;    // Warm white color control pin level

//white balance
    uint8_t wr;
    uint8_t wg;
    uint8_t wb;

// Light gamma coefficient 
    uint8_t gr;
    uint8_t gg;
    uint8_t gb;

//device function 
    char cmod[6];        // Color model: C/CW/RGB/RGBC/RGBCW
    bool_t dmod;        // Color driver mode: true:,  false:   .
    uint8_t brightmin;  // Minimum brightness: 0~100
    uint8_t brightmax;  // Maximum brightness: 0~100
    bool_t onoffmode;   // Is there a gradient when switching
    char module[15];    // module choosed for the light
    char wfcfg[5];      // Low power / flash
    uint32_t pwmhz;     // PWM frequency
    bool_t cwtype;      // Color temperature drive mode: true:   false:   .
    uint8_t cwmaxp;     // Maximum Power configuration of Cold and warm Light mixing
    bool_t pmemory;     // Is there a power-off memory:true/false

//connection
    char rstcor;        // light color while connecting
    char defcolor;      // light color after connected
    uint8_t rstbr;      // light brightness while connecting
    uint8_t defbright;  // light brightness after connected
    uint8_t rstmode;    // reset mode
    uint8_t rstnum;     // number of times required to reset by switching on and off
    uint8_t key_pin;    // reset button pin
    uint8_t key_lv;     // reset button level
    uint8_t ktime;      // seconds required to reset by long press

}device_config_t;

typedef struct {
    uint8_t json_version[5];
    uint8_t category;
}device_common_t;

typedef struct {
    uint8_t iic_r_index;
    uint8_t iic_g_index;
    uint8_t iic_b_index;
    uint8_t iic_c_index;
    uint8_t iic_w_index;
    uint8_t iic_ctrl_pin;
    uint8_t iic_scl_pin;
    uint8_t iic_sda_pin;
}device_iic_t;

typedef struct {
    uint8_t color_r_pin;   // Red color control pin
    uint8_t color_r_lv;    // Red color control pin level
    uint8_t color_g_pin;   // Green color control pin
    uint8_t color_g_lv;    // Green color control pin level
    uint8_t color_b_pin;   // Blue  color control pin
    uint8_t color_b_lv;    // Blue  color control pin level
    uint8_t color_c_pin;   // Cold white color control pin
    uint8_t color_c_lv;    // Cold white color control pin level
    uint8_t color_w_pin;   // Warm white color control pin
    uint8_t color_w_lv;    // Warm white color control pin level
}device_color_pin_t;

typedef struct {
    uint8_t wr; 
    uint8_t wg;
    uint8_t wb;
}device_white_balance_t;

typedef struct {
    uint8_t gr;
    uint8_t gg;
    uint8_t gb;
}device_gamma_t;

typedef struct {
    char cmod[6];       // Color model: C/CW/RGB/RGBC/RGBCW
    bool_t dmod;        // Color driver mode: true:,  false:   .
    uint8_t brightmin;  // Minimum brightness: 0~100
    uint8_t brightmax;  // Maximum brightness: 0~100
    bool_t onoffmode;   // Is there a gradient when switching
    char module[15];    // module choosed for the light
    char wfcfg[5];      // Low power / flash
    uint32_t pwmhz;     // PWM frequency
    bool_t cwtype;      // Color temperature drive mode: true:   false:   .
    uint8_t cwmaxp;     // Maximum Power configuration of Cold and warm Light mixing
    bool_t pmemory;     // Is there a power-off memory:true/false
}device_function_t;

typedef struct {
    char rstcor;        // light color while connecting
    char defcolor;      // light color after connected
    uint8_t rstbr;      // light brightness while connecting
    uint8_t defbright;  // light brightness after connected
    uint8_t rstmode;    // reset mode
    uint8_t rstnum;     // number of times required to reset by switching on and off
    uint8_t key_pin;    // reset button pin
    uint8_t key_lv;     // reset button level
    uint8_t ktime;      // seconds required to reset by long press
}device_connection_t;

#define get_array_len(x) (sizeof(x)/sizeof(x[0]))
extern device_config_t g_device;

uint8_t device_config_data_load(void);

void get_jsonver(uint8_t *js_ver);
uint8_t get_category(void);

void get_iic_rgbcw_index(device_iic_t *iic_cfg);
uint8_t get_iic_r_index(void);
uint8_t get_iic_g_index(void);
uint8_t get_iic_b_index(void);
uint8_t get_iic_c_index(void);
uint8_t get_iic_w_index(void);
uint8_t get_iic_ctrl_pin(void);
uint8_t get_iic_scl_pin(void);
uint8_t get_iic_sda_pin(void);

void get_color_pin_config(device_color_pin_t *color_pin_config);
uint8_t get_color_red_pin(void);
uint8_t get_color_red_pin_level(void);
uint8_t get_color_green_pin(void);
uint8_t get_color_green_pin_level(void);
uint8_t get_color_blue_pin(void);
uint8_t get_color_blue_pin_level(void);
uint8_t get_color_cold_pin(void);
uint8_t get_color_cold_pin_level(void);
uint8_t get_color_warm_pin(void);
uint8_t get_color_warm_pin_level(void);

void get_white_balance_config(device_white_balance_t *white_balance);
uint8_t get_white_balance_red(void);
uint8_t get_white_balance_green(void);
uint8_t get_white_balance_blue(void);

void get_gamma_config(device_gamma_t *gamma);
uint8_t get_gamma_red(void);
uint8_t get_gamma_green(void);
uint8_t get_gamma_blue(void);

uint8_t get_color_mode(void);
uint8_t get_driver_mode(void);
uint8_t get_light_min_bright(void);
uint8_t get_light_max_bright(void);
bool_t get_onoffmode(void);
void get_module_string(char * module, uint8_t* len);

uint8_t get_wfcfg_config(void);

uint32_t get_pwmhz_config(void);
bool_t get_cwtype_config(void);
uint8_t get_cw_max_power(void);
bool_t get_pmemory_config(void);
uint8_t get_reset_color(void);
uint8_t get_connected_color(void);
uint8_t get_connectting_btight(void);
uint8_t get_connected_btight(void);
uint8_t get_reset_mode(void);
uint8_t get_reset_num(void);
uint8_t get_key_pin(void);
uint8_t get_key_lv(void);
uint8_t get_reset_time(void);


#endif
