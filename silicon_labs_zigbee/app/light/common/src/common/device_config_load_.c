/**
 * @Author: jinlu
 * @file name: device_config_load.c
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "device_config_load.h"

/********************************************************************************/
//Functions defined on the outside                                               
extern bool_t oem_config_load(uint16_t *len, uint8_t **data);
/*******************************************************************************/

#define CONFIG_DATA_LEN_MAX    1024
#define MAX_VALUE_SIZE         15

uint8_t device_config_table[CONFIG_DATA_LEN_MAX];

device_config_t g_device;

string_to_enum_t cmod_table[] = 
{
    {"c", CMOD_C},
    {"cw", CMOD_CW},
    {"rgb", CMOD_RGB},
    {"rgbc", CMOD_RGBC},
    {"rgbcw", CMOD_RGBCW}
};

string_to_enum_t wfcfg_table[] = 
{
    {"old", GWCM_OLD},
    {"low", GWCM_LOW_POWER},
    {"spcl", GWCM_SPCL_MODE},
    {"prod", GWCM_OLD_PROD}
};

string_to_enum_t  color_table[] = 
{
    {"c", COLOR_C},
    {"w", COLOR_W},
    {"r", COLOR_R},
    {"g", COLOR_G},
    {"b", COLOR_B},
};

key_value_t  json_table[] = 
{
    {"Jsonver", &g_device.json_version, sizeof(g_device.json_version), TYPE_STRING},
    //{"category", &g_device.category, TYPE_UINT8},

    {"iicr", &g_device.iic_r_index, sizeof(g_device.iic_r_index),TYPE_HEX},
    {"iicg", &g_device.iic_g_index, sizeof(g_device.iic_g_index),TYPE_HEX},
    {"iicb", &g_device.iic_b_index, sizeof(g_device.iic_b_index), TYPE_HEX},
    {"iicc", &g_device.iic_c_index, sizeof(g_device.iic_c_index),TYPE_HEX},
    {"iicw", &g_device.iic_w_index, sizeof(g_device.iic_w_index),TYPE_HEX},
    {"ctrl", &g_device.iic_ctrl_pin, sizeof(g_device.iic_ctrl_pin),TYPE_HEX},
    {"iicscl", &g_device.iic_scl_pin, sizeof(g_device.iic_scl_pin),TYPE_HEX},
    {"iicsda", &g_device.iic_sda_pin, sizeof(g_device.iic_sda_pin),TYPE_HEX},

    {"r_pin", &g_device.color_r_pin, sizeof(g_device.color_r_pin),TYPE_HEX},
    {"r_lv", &g_device.color_r_lv, sizeof(g_device.color_r_lv),TYPE_HEX},
    {"g_pin", &g_device.color_g_pin, sizeof(g_device.color_g_pin),TYPE_HEX},
    {"g_lv", &g_device.color_g_lv, sizeof(g_device.color_g_lv),TYPE_HEX},
    {"b_pin", &g_device.color_b_pin, sizeof(g_device.color_b_pin),TYPE_HEX},
    {"b_lv", &g_device.color_b_lv, sizeof(g_device.color_b_lv),TYPE_HEX},
    {"c_pin", &g_device.color_c_pin, sizeof(g_device.color_c_pin),TYPE_HEX},
    {"c_lv", &g_device.color_c_lv, sizeof(g_device.color_c_lv),TYPE_HEX},
    {"w_pin", &g_device.color_w_pin, sizeof(g_device.color_w_pin),TYPE_HEX},
    {"w_lv", &g_device.color_w_lv, sizeof(g_device.color_w_lv),TYPE_HEX},

    {"wr", &g_device.wr, sizeof(g_device.wr),TYPE_HEX},
    {"wg", &g_device.wg, sizeof(g_device.wg),TYPE_HEX},
    {"wb", &g_device.wb, sizeof(g_device.wb),TYPE_HEX},
    
    {"gr", &g_device.gr, sizeof(g_device.gr),TYPE_HEX},
    {"gg", &g_device.gg, sizeof(g_device.gg),TYPE_HEX},
    {"gb", &g_device.gb, sizeof(g_device.gb),TYPE_HEX},

    {"cmod", &g_device.cmod, sizeof(g_device.cmod),TYPE_STRING},
    {"dmod", &g_device.dmod, sizeof(g_device.dmod),TYPE_HEX},
    {"brightmin", &g_device.brightmin, sizeof(g_device.brightmin),TYPE_HEX},
    {"brightmax", &g_device.brightmax, sizeof(g_device.brightmax),TYPE_HEX},
    {"onoffmode", &g_device.onoffmode, sizeof(g_device.onoffmode),TYPE_HEX},
    {"module", &g_device.module,sizeof(g_device.module), TYPE_STRING},
    {"wfcfg", &g_device.wfcfg, sizeof(g_device.wfcfg),TYPE_STRING},
    {"pwmhz", &g_device.pwmhz, sizeof(g_device.pwmhz),TYPE_HEX},
    {"cwtype", &g_device.cwtype, sizeof(g_device.cwtype),TYPE_HEX},
    {"cwmaxp", &g_device.cwmaxp, sizeof(g_device.cwmaxp),TYPE_HEX},
    {"pmemory", &g_device.pmemory, sizeof(g_device.pmemory),TYPE_HEX},

    {"rstcor", &g_device.rstcor, sizeof(g_device.rstcor),TYPE_STRING},
    {"defcolor", &g_device.defcolor, sizeof(g_device.defcolor),TYPE_STRING},
    {"rstbr", &g_device.rstbr, sizeof(g_device.rstbr),TYPE_HEX},
    {"defbright", &g_device.defbright, sizeof(g_device.defbright),TYPE_HEX},
    {"rstmode", &g_device.rstmode, sizeof(g_device.rstmode),TYPE_HEX},
    {"rstnum", &g_device.rstnum, sizeof(g_device.rstnum),TYPE_HEX},
    {"key_pin", &g_device.key_pin, sizeof(g_device.key_pin),TYPE_HEX},
    {"key_lv", &g_device.key_lv, sizeof(g_device.key_lv),TYPE_HEX},
    {"ktime", &g_device.ktime, sizeof(g_device.ktime),TYPE_HEX},
};

#define JSON_TABLE_LEN  get_array_len(json_table)
#define CMOD_TABLE_LEN  get_array_len(cmod_table)
#define WFCFG_TABLE_LEN  get_array_len(wfcfg_table)
#define COLOR_TABLE_LEN  get_array_len(color_table)

/**
 * @description: get all config data from device_config_table[], and install it to
 * the g_device struct. make sure all keys of json_table[] can be find in device_config_table[]
 * @param {max_len} the max length of config data
 * @return: 0:failed; 1:success;
 */
static bool_t get_all_values(IN uint16_t max_len)
{     
    uint8_t i = 0;
    char* start_p;
    char* end_p;
    uint8_t value[MAX_VALUE_SIZE];
    uint8_t len;

    for(i=0;i<JSON_TABLE_LEN;i++){
        start_p = strstr(device_config_table, json_table[i].key);
        if(start_p==NULL){
            continue;
        }

        while(*start_p != ':'){
            start_p++;
            if(start_p - device_config_table > max_len-1){
                return 0;
            }
        }

        start_p++;
        end_p = start_p;
        while(*end_p != ','){
            end_p ++;
            if(end_p - device_config_table > max_len){
                return 0;
            }
        }

        len = end_p - start_p;

        if(len > MAX_VALUE_SIZE-1)
        {
            len = json_table[i].value_size;
        }
        
        memcpy(value, start_p, len);
        value[len] = '\0';

        if(json_table[i].type == TYPE_HEX){
             sscanf(value, "%d", json_table[i].value);
        } else {
             memcpy(json_table[i].value, value, len);
        }  
    }
    return 1;
}

/**
 * @description: load device configuration data.
 * @param {type} none
 * @return: 0: failed, 1:success 
 */
uint8_t device_config_data_load(void)
{
    uint16_t len = 0;
    uint8_t *data;
    uint8_t ret = 0;

    oem_config_load(&len, &data);
    if(len == 0)
      return 0;
    memset(device_config_table,0,sizeof(device_config_table));
    if(len <= CONFIG_DATA_LEN_MAX){
        memcpy(device_config_table, data, len);
    }else{
      return 0;
    }
     ret = get_all_values(len);
     return ret;
}

/********************************************************************************
*                                   common config
********************************************************************************/
/**
 * @description: get json version string
 * @param {out} *js_ver: address to store json version string
 * @return: none
 */
void get_jsonver(OUT uint8_t *js_ver)
{
    memcpy(js_ver,g_device.json_version,5);
}

/**
 * @description: get json config data :category
 * @param {type} none
 * @return: category
 */
uint8_t get_category(void)
{
    return g_device.category;    
}
/********************************************************************************
*                                   IIC Config
********************************************************************************/
/**
 * @description: get json config data: iic, include r、g、b、c、w index,and IIC
 * pins num
 * @param {out}  iic_cfg
 * @return: none
 */
void get_iic_rgbcw_index(OUT device_iic_t *iic_cfg)
{   
   *iic_cfg = *(device_iic_t*)(&g_device.iic_r_index);
}

/**
 * @description: get iic r pin index
 * @param {type} none
 * @return: r pin index
 */
uint8_t get_iic_r_index(void)
{
    return g_device.iic_r_index;
}

/**
 * @description: get iic g pin index
 * @param {type} none
 * @return: g pin index
 */
uint8_t get_iic_g_index(void)
{   
    return g_device.iic_g_index;
}

/**
 * @description: get iic b pin index
 * @param {type} none
 * @return: b pin index
 */
uint8_t get_iic_b_index(void)
{   
    return g_device.iic_b_index;
}

/**
 * @description: get iic c pin index
 * @param {type} none
 * @return: c pin index
 */
uint8_t get_iic_c_index(void)
{   
    return g_device.iic_c_index;
}

/**
 * @description: get iic w pin index
 * @param {type} none
 * @return: w pin index
 */
uint8_t get_iic_w_index(void)
{   
    return g_device.iic_w_index;
}

/**
 * @description: get iic ctrl pin number
 * @param {type} none
 * @return: ctrl pin number
 */
uint8_t get_iic_ctrl_pin(void)
{
    return g_device.iic_ctrl_pin;
}

/**
 * @description: get iic scl pin number
 * @param {type} none
 * @return: scl pin number
 */
uint8_t get_iic_scl_pin(void)
{
    return g_device.iic_scl_pin;
}

/**
 * @description: get iic sda pin number
 * @param {type} none
 * @return: sda pin number
 */
uint8_t get_iic_sda_pin(void)
{
    return g_device.iic_sda_pin;
}
/********************************************************************************
*                                   color pin Config
********************************************************************************/
/**
 * @description: get json config: color set, include r、g、b、c、w pin number and
 * effective level.
 * @param {OUT} color_pin_config color pins config set
 * @return: none
 */
void get_color_pin_config(OUT device_color_pin_t *color_pin_config)
{   
   *color_pin_config = *(device_color_pin_t*)(&g_device.color_r_pin);
}

/**
 * @description: get json config data: red pin number
 * @param {type} none
 * @return: red pin number
 */
uint8_t get_color_red_pin(void)
{
    return g_device.color_r_pin;
}

/**
 * @description: get json config data: red pin effective level.
 * @param {type} none
 * @return: red pin effective level.
 */
uint8_t get_color_red_pin_level(void)
{
    return g_device.color_r_lv;
}

/**
 * @description: get json config data: green pin number
 * @param {type} none
 * @return: green pin number
 */
uint8_t get_color_green_pin(void)
{
    return g_device.color_g_pin;
}
/**
 * @description: get json config data: green pin effective level.
 * @param {type} none
 * @return: green pin effective level.
 */
uint8_t get_color_green_pin_level(void)
{
    return g_device.color_g_lv;
}
/**
 * @description: get json config data: blue pin number
 * @param {type} none
 * @return: blue pin number
 */
uint8_t get_color_blue_pin(void)
{
    return g_device.color_b_pin;
}
/**
 * @description: get json config data: blue pin effective level.
 * @param {type} none
 * @return: blue pin effective level.
 */
uint8_t get_color_blue_pin_level(void)
{
    return g_device.color_b_lv;
}
/**
 * @description: get json config data: cold white pin number
 * @param {type} none
 * @return: cold white pin number
 */
uint8_t get_color_cold_white_pin(void)
{
    return g_device.color_c_pin;
}
/**
 * @description: get json config data: cold white pin pin effective level.
 * @param {type} none
 * @return: cold white pin effective level.
 */
uint8_t get_color_cold_white_pin_level(void)
{
    return g_device.color_c_lv;
}
/**
 * @description: get json config data: warm white pin number
 * @param {type} none
 * @return:  warm white pin number
 */
uint8_t get_color_warm_white_pin(void)
{
    return g_device.color_w_pin;
}
/**
 * @description: get json config data: warm white effective level.
 * @param {type} none
 * @return: warm white pin effective level.
 */
uint8_t get_color_warm_white_pin_level(void)
{
    return g_device.color_w_lv;
}

/********************************************************************************
*                                   white balance config
********************************************************************************/
/**
 * @description: get json config data: white balance parameter
 * @param {OUT} white_balance: include r、g、b balance parameter
 * @return: none
 */
void get_white_balance_config(OUT device_white_balance_t *white_balance)
{   
   *white_balance = *(device_white_balance_t*)(&g_device.wr);
}
/**
 * @description: get json config data: red color balance parameter
 * @param {type} none
 * @return: red color balance parameter 
 */
uint8_t get_white_balance_red(void)
{
    return g_device.wr;
}
/**
 * @description: get json config data: green color balance parameter
 * @param {type} none
 * @return: green color balance parameter 
 */
uint8_t get_white_balance_green(void)
{
    return g_device.wg;
}
/**
 * @description: get json config data: blue color balance parameter
 * @param {type} none
 * @return: blue color balance parameter 
 */
uint8_t get_white_balance_blue(void)
{
    return g_device.wb;
}

/********************************************************************************
*                                   gamma coefficient 
********************************************************************************/
/**
 * @description: get json config data: color gamma coefficient 
 * @param {OUT} gamma: include red,green,blue gamma coefficient 
 * @return: none
 */
void get_gamma_config(OUT device_gamma_t *gamma)
{   
   *gamma = *(device_gamma_t*)(&g_device.gr);
}
/**
 * @description: get json config data: red gamma coefficient 
 * @param {type} none
 * @return: red gamma coefficient 
 */
uint8_t get_gamma_red(void)
{
    return g_device.wb;
}
/**
 * @description: get json config data: green gamma coefficient 
 * @param {type} none
 * @return: green gamma coefficient 
 */
uint8_t get_gamma_green(void)
{
    return g_device.wb;
}
/**
 * @description: get json config data: blue gamma coefficient 
 * @param {type} none
 * @return: blue gamma coefficient 
 */
uint8_t get_gamma_blue(void)
{
    return g_device.wb;
}

/********************************************************************************
*                                   function  
********************************************************************************/
/**
 * @description: get json config data: color mode
 * @param {type} none
 * @return: 0x00~0x04 :color mode, 0xFF: can't find.
 */
uint8_t get_color_mode(void)
{
    uint8_t i;
    for(i=0;i<CMOD_TABLE_LEN;i++){
        if(string_compare(&g_device.cmod[0], cmod_table[i].string)){
            return cmod_table[i].enum_value;
        }
    }
    return 0xFF;
}

/**
 * @description: get json config data: driver mode
 * @param {type} none
 * @return: DMOD_IIC/DMOD_PWM
 */
uint8_t get_driver_mode(void)
{
    if(g_device.dmod){
        return DMOD_IIC;
    } else {
        return DMOD_PWM;
    }
}
/**
 * @description: get json config data: the minimum brightness 
 * @param {type} none
 * @return: minimum brightness 
 */
uint8_t get_light_min_bright(void)
{
    return g_device.brightmin;
}
/**
 * @description: get json config data: the greatest brilliancy
 * @param {type} none
 * @return: greatest brilliancy
 */
uint8_t get_light_max_bright(void)
{
    return g_device.brightmax;
}
/**
 * @description: get json config data: onoffmode
 * @param {type} none
 * @return: onoffmode
 */
bool_t get_onoffmode(void)
{
    return g_device.onoffmode;
}

/**
 * @description: get json config data: module name string.
 * @param {out} module: module name string.
 * @param {out} len: length of module name string.
 * @return: none
 */
void get_module_string(OUT char * module, OUT uint8_t* len)
{
    *len = strlen(g_device.module);
    memcpy(module, &g_device.module[0], strlen(g_device.module));
}

/**
 * @description: get json config data: low power config
 * @param {type} none
 * @return: enum of WFCFG_T, 0xFF can't find.
 */
uint8_t get_wfcfg_config(void)
{
    uint8_t i;
    for(i=0;i<WFCFG_TABLE_LEN;i++){
        if(string_compare(&g_device.wfcfg[0], wfcfg_table[i].string)){
            return wfcfg_table[i].enum_value;
        }
    }
    return 0xFF;
}
/**
 * @description: get json config data: pwm frequency 
 * @param {type} none
 * @return: pwm frequency 
 */
uint32_t get_pwmhz_config(void)
{
    return g_device.pwmhz;
}
/**
 * @description: get json config data: cwtype
 * @param {type} none
 * @return: cwtype
 */
bool_t get_cwtype_config(void)
{
    return g_device.cwtype;
}

/**
 * @description: get json config data: white max power
 * @param {type} none
 * @return: white max power
 */
uint8_t get_cw_max_power(void)
{
    return g_device.cwmaxp;
}
/**
 * @description: get json config data: pmemory
 * @param {type} none
 * @return: pmemory
 */
bool_t get_pmemory_config(void)
{
    return g_device.pmemory;
}

/**
 * @description: get json config data: color set when connectting.
 * @param {type} none
 * @return: 0x00~0x04: r/g/b/c/w
 */
uint8_t get_reset_color(void)
{
    uint8_t i;
    char color[2]={0,0};
    
    color[0] = g_device.rstcor;
    for(i=0;i<COLOR_TABLE_LEN;i++){ 
       if(string_compare(color, color_table[i].string)){
          return color_table[i].enum_value;
       }
    }
    return 0xFF;
}
/**
 * @description: get json config data: color set when connected.
 * @param {type} none
 * @return: 0x00~0x04: r/g/b/c/w , 0xFF: error , can not find enum value.
 */
uint8_t get_connected_color(void)
{
    uint8_t i;
    char color[2]={0,0}; //the second value is end of string: "\0"
    
    color[0] = g_device.defcolor;
    for(i=0;i<COLOR_TABLE_LEN;i++){ 
       if(string_compare(color, color_table[i].string)){
          return color_table[i].enum_value;
       }
    }
    return 0xFF;
}
/**
 * @description: get json config data: brightness when connectting.
 * @param {type} none
 * @return: 10~100
 */
uint8_t get_connectting_btight(void)
{
    uint8_t ret = g_device.rstbr;
    if(ret<10) ret = 10;
    if(ret>100) ret = 100;
    return ret;
}
/**
 * @description: get json config data: brightness when connected.
 * @param {type} none
 * @return: 10~100
 */
uint8_t get_connected_btight(void)
{
    uint8_t ret = g_device.defbright;
    if(ret<10) ret = 10;
    if(ret>100) ret = 100;
    return ret;
}
/**
 * @description: get json config data: reset mode.
 * @param {type} none
 * @return: rstmode
 */
uint8_t get_reset_mode(void)
{
    return g_device.rstmode;
}
/**
 * @description: get json config data: reset number to connectting network.
 * @param {type} none
 * @return: reset number
 */
uint8_t get_reset_num(void)
{
    return g_device.rstnum;
}

/**
 * @description: get json config data: device button pin number
 * @param {type} none
 * @return: device button pin number
 */
uint8_t get_key_pin(void)
{
    return g_device.key_pin;
}

/**
 * @description: get json config data: device button pin effective level.
 * @param {type} none
 * @return: 0/1
 */
uint8_t get_key_lv(void)
{
    return g_device.key_lv;
}
/**
 * @description: get json config data: long press time(seconds) to connectting network..
 * @param {type} none
 * @return: long press time(seconds)
 */
uint8_t get_reset_time(void)
{
    return g_device.ktime;
}













