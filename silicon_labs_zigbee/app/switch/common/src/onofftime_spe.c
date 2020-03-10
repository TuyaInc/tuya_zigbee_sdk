#include "zigbee_dev_template.h"
#include "zigbee_cmd.h"
#include "zigbee_attr.h"
#include "mf_test_callback.h"
#include "zigbee_sdk.h"

#include "ty_def_cfg.h"

typedef struct
{
    uint16_t ontime;
    uint16_t offwaittime;
} zigbee_Ontime_Offwaittime_str;

extern uint8_t get_ep_sums(void);
extern uint8_t get_ep_value(uint8_t ep_num);
extern void __dev_report_onoff_msg(uint8_t ep);
extern void __dev_attr_change_report(uint8_t ep, DEV_IO_ST_T st);
extern void __dev_report_onoff_onofftime_msg(uint8_t ep);
extern void set_led_style_when_relay_onoff_attr(DEV_LED_ONOFF_RELAY_ONOFF_CFG_ST_T t_attr);
extern void dev_report_msg(uint8_t ep, uint16_t attr_id, ATTR_TYPE_T type, uint8_t value_size, uint8_t*value);
extern void *MALLOC_STATIC(uint16_t size);
extern void syn_led_mode_fun(uint8_t t_index);

static void syn_led_mode_by_attr(void);

static uint8_t *IsInOnTimeOrOffWaitTimeMode;

static uint8_t *IsOnTimeOrOffWaitTimeNeedReportFlag;

static zigbee_Ontime_Offwaittime_str *zigbee_Ontime_Offwaittime;

void Ontime_Offwaittime_str_init(void)
{
    IsInOnTimeOrOffWaitTimeMode = (uint8_t *)MALLOC_STATIC(get_ep_sums() * sizeof(uint8_t));
    if(IsInOnTimeOrOffWaitTimeMode)
    {
        memset(IsInOnTimeOrOffWaitTimeMode,0x00, get_ep_sums() * sizeof(uint8_t));
    }
    
    IsOnTimeOrOffWaitTimeNeedReportFlag = (uint8_t *)MALLOC_STATIC(get_ep_sums() * sizeof(uint8_t));
    if(IsOnTimeOrOffWaitTimeNeedReportFlag)
    {
        memset(IsOnTimeOrOffWaitTimeNeedReportFlag,0x01, get_ep_sums() * sizeof(uint8_t));/*上电初始化，第一次需要上报属性*/
    }

    zigbee_Ontime_Offwaittime = (zigbee_Ontime_Offwaittime_str *)MALLOC_STATIC(get_ep_sums() * sizeof(zigbee_Ontime_Offwaittime_str));
    if(zigbee_Ontime_Offwaittime)
    {
        memset(zigbee_Ontime_Offwaittime,0x00, get_ep_sums() * sizeof(zigbee_Ontime_Offwaittime_str));/*上电初始化，第一次需要上报属性*/
    }
} 
uint16_t get_ontime_or_offwaittime(uint8_t EP, uint8_t ustyle)
{
    uint16_t ret  = 0x0000;
    for(uint8_t ii = 0; ii < get_ep_sums();ii ++)
    {
        if(get_ep_value(ii) == EP)
        {
            if(ustyle)
            {
                ret = zigbee_Ontime_Offwaittime[ii].ontime;
            }
            else
            {
                ret = zigbee_Ontime_Offwaittime[ii].offwaittime;
            }
            return ret;
        }
    }
    return ret;
}
/*********************************************************************************
 * Function:       
 * Description:    本地倒计时直接清零
 * Input:          
 * Outpot:         
 * Return:         
 * History:  
 *******************************************************************************/ 
void clearOntimeOffwaittime(uint8_t EP)
{
    for(uint8_t ii = 0; ii < get_ep_sums();ii ++)
    {
        if(get_ep_value(ii) == EP)
        {
            if(zigbee_Ontime_Offwaittime[ii].offwaittime)
            {
                zigbee_Ontime_Offwaittime[ii].offwaittime = 0;
                dev_zigbee_write_attribute(
                    EP,
                    CLUSTER_ON_OFF_CLUSTER_ID,
                    ATTR_OFF_WAIT_TIME_ATTRIBUTE_ID,
                    &zigbee_Ontime_Offwaittime[ii].offwaittime,
                    ATTR_INT16U_ATTRIBUTE_TYPE
                );
            }
            if(zigbee_Ontime_Offwaittime[ii].ontime)
            {
                zigbee_Ontime_Offwaittime[ii].ontime = 0;
                dev_zigbee_write_attribute(
                    EP,
                    CLUSTER_ON_OFF_CLUSTER_ID,
                    ATTR_ON_TIME_ATTRIBUTE_ID,
                    &zigbee_Ontime_Offwaittime[ii].offwaittime,
                    ATTR_INT16U_ATTRIBUTE_TYPE
                );

            }
            break;
        }
    }
}

void dev_turn_to_init_st(void)
{
    uint8_t umode = 1;/*默认是状态*/
    for(uint8_t i = 0; i < get_ep_sums(); i ++)
    {
        dev_zigbee_write_attribute(
            get_ep_value(i),
            CLUSTER_ON_OFF_CLUSTER_ID,
            LIGHT_MODE_ATTR_ID,
            &umode,
            ATTR_ENUM8_ATTRIBUTE_TYPE
        );
        clearOntimeOffwaittime(get_ep_value(i));
    }
    set_led_style_when_relay_onoff_attr(DEV_LED_ON_RELAY_ON);/*恢复变量*/
    syn_led_mode_by_attr();/*同步led*/
}

/*********************************************************************************
 * Function:       
 * Description:    写属性，操作硬件
 * Input:          
 * Outpot:         
 * Return:         
 * History:  
 *******************************************************************************/ 
void write_attr_toggle_led_and_relay(uint8_t endpoint)
{
    uint8_t onOffValue;

    dev_zigbee_read_attribute(
        endpoint,
        CLUSTER_ON_OFF_CLUSTER_ID,
        ATTR_ON_OFF_ATTRIBUTE_ID,
        &onOffValue,
        1
    );
    onOffValue = !onOffValue;
    __dev_attr_change_report(endpoint, (DEV_IO_ST_T)onOffValue);
}
void clear_time_attr_operate_hal(uint8_t EP)
{
    clearOntimeOffwaittime(EP);
    write_attr_toggle_led_and_relay(EP);/*延迟时间到了*/
}
void cancel_delay_report_event(uint8_t endpoint)
{
    if(endpoint == get_ep_value(0))
    {
        dev_timer_stop(ONOFFTIME_EVE_USE1);
    }
    else if(endpoint == get_ep_value(1))
    {
        dev_timer_stop(ONOFFTIME_EVE_USE2);
    }
    else if(endpoint == get_ep_value(2))
    {
        dev_timer_stop(ONOFFTIME_EVE_USE3);
    }
    else if(endpoint == get_ep_value(3))
    {
        dev_timer_stop(ONOFFTIME_EVE_USE4);
    }
}

/*********************************************************************************
 * Function:       
 * Description:    当前是否处于本地开关延迟
 * Input:          
 * Outpot:         
 * Return:         
 * History:  
 *******************************************************************************/ 
uint8_t GetOnTimeOrOffWaitTimeModeProc(uint8_t endpoint)
{
    uint8_t ii, ret;
    ret = 0;
    cancel_delay_report_event(endpoint);/*取消延迟开关事件*/
    for(ii = 0; ii < get_ep_sums(); ii ++)
    {
        if(get_ep_value(ii) == endpoint)
        {
            if(IsInOnTimeOrOffWaitTimeMode[ii])
            {
                IsInOnTimeOrOffWaitTimeMode[ii] = 0;
                ret = 1;
            }
            break;
        }
    }
    return ret;
}
uint8_t if_need_report_ontime_offwaittime(uint8_t endpoint)
{
    uint8_t ret;
    ret = 0;
    for(uint8_t i = 0; i < get_ep_sums(); i ++)
    {
        if(endpoint == get_ep_value(i))
        {
            if(IsOnTimeOrOffWaitTimeNeedReportFlag[i])
            {
                IsOnTimeOrOffWaitTimeNeedReportFlag[i] = 0;
                ret = 1;
            }
            break;
        }
    }
    return ret;
}

/*********************************************************************************
 * Function:       
 * Description:    是否需要在上报中带上onofftime属性标志处理
 * Input:          
 * Outpot:         
 * Return:         
 * History:  
 *******************************************************************************/ 
void onofftime_report_flg_proc(uint8_t endpoint)
{
    uint8_t i;

    for(i = 0; i < get_ep_sums(); i ++)
    {
        if(endpoint == get_ep_value(i))
        {
            if(GetOnTimeOrOffWaitTimeModeProc(endpoint))
            {
                clearOntimeOffwaittime(endpoint);
                IsOnTimeOrOffWaitTimeNeedReportFlag[i] = 1;/*调用report的时候，需要同时上报onofftime属性*/
            }
            break;
        }
    }

}
uint8_t check_ep_onoff_status(uint8_t endpoint)
{
    ATTR_CMD_RET_T status;
    uint8_t onOffValue = 0;
    uint8_t ret = 0xFF;
    
    status  = dev_zigbee_read_attribute(endpoint, CLUSTER_ON_OFF_CLUSTER_ID, ATTR_ON_OFF_ATTRIBUTE_ID, &onOffValue, 1);
    if(status == ATTR_CMD_RET_SUCCESS)
    {
        ret = onOffValue;
    }
    return ret;
}

void report_ontime_attr_proc(uint8_t endpoint, uint16_t onTime, uint16_t offWaitTime)
{
    if(check_ep_onoff_status(endpoint) != 0xFF)
    {
        if(check_ep_onoff_status(endpoint))/*当前为开*/
        {
            dev_report_msg(endpoint, ATTR_OFF_WAIT_TIME_ATTRIBUTE_ID, ATTR_INT16U_ATTRIBUTE_TYPE, 2, (uint8_t *)&offWaitTime);
        }
        else
        {
            dev_report_msg(endpoint, ATTR_ON_TIME_ATTRIBUTE_ID, ATTR_INT16U_ATTRIBUTE_TYPE, 2, (uint8_t *)&onTime);
        }
    }
}
void IsInOnTimeOrOffWaitTimeModeProc(uint8_t endpoint)
{
    uint8_t i = 0;
    for(i = 0; i < get_ep_sums(); i++)
    {
        if(get_ep_value(i) == endpoint)
        {
            IsInOnTimeOrOffWaitTimeMode[i] = 1;
            return;
        }
    }

}
/*********************************************************************************
 * Function:       1s钟递减函数
 * Description:    
 * Input:          
 * Outpot:         
 * Return:         
 * History:  
 *******************************************************************************/ 
static void OneSecDecFlgFun(void)
{
    for(uint8_t ii = 0; ii < get_ep_sums();ii ++)
    {
        if(zigbee_Ontime_Offwaittime[ii].offwaittime)
        {
            --zigbee_Ontime_Offwaittime[ii].offwaittime;
            dev_zigbee_write_attribute(
                get_ep_value(ii),
                CLUSTER_ON_OFF_CLUSTER_ID,
                ATTR_OFF_WAIT_TIME_ATTRIBUTE_ID,
                &zigbee_Ontime_Offwaittime[ii].offwaittime,
                ATTR_INT16U_ATTRIBUTE_TYPE
            );
        }
        if(zigbee_Ontime_Offwaittime[ii].ontime)
        {
            --zigbee_Ontime_Offwaittime[ii].ontime;
            dev_zigbee_write_attribute(
                get_ep_value(ii),
                CLUSTER_ON_OFF_CLUSTER_ID,
                ATTR_ON_TIME_ATTRIBUTE_ID,
                &zigbee_Ontime_Offwaittime[ii].offwaittime,
                ATTR_INT16U_ATTRIBUTE_TYPE
            );
        }
    }
}
static void __dev_report_ledmode_msg(uint8_t ep, DEV_LED_ONOFF_RELAY_ONOFF_CFG_ST_T t_attr)
{
    dev_report_msg(ep, LIGHT_MODE_ATTR_ID, ATTR_ENUM8_ATTRIBUTE_TYPE, 1, (uint8_t *)&t_attr);
}
static void syn_led_mode_by_attr(void)
{
    uint8_t t_index;
    for(t_index = 0; t_index < get_ep_sums(); t_index ++)
    {
        syn_led_mode_fun(t_index);
    }
}
void syn_led_mode_power_on(void)
{
    DEV_LED_ONOFF_RELAY_ONOFF_CFG_ST_T t_attr;
    if(ATTR_CMD_RET_SUCCESS == dev_zigbee_read_attribute(1, CLUSTER_ON_OFF_CLUSTER_ID, LIGHT_MODE_ATTR_ID, &t_attr, 1))
    {
        set_led_style_when_relay_onoff_attr(t_attr);/*恢复变量*/
        __dev_report_ledmode_msg(1, t_attr);
    }
}

void syn_led_mode(void)
{
    DEV_LED_ONOFF_RELAY_ONOFF_CFG_ST_T t_attr;
    if(ATTR_CMD_RET_SUCCESS == dev_zigbee_read_attribute(1, CLUSTER_ON_OFF_CLUSTER_ID, LIGHT_MODE_ATTR_ID, &t_attr, 1))
    {
       set_led_style_when_relay_onoff_attr(t_attr);/*恢复变量*/
       syn_led_mode_by_attr();/*同步led*/
       __dev_report_ledmode_msg(1, t_attr);
    }
}
void dev_onofftime_ledmode_evt_callback(uint8_t evt)
{
    switch(evt)
    {
        case ONOFFTIME_EVE_USE1:
        {
            clear_time_attr_operate_hal(get_ep_value(0));/*延迟时间到了*/
            break;
        }
        case ONOFFTIME_EVE_USE2:
        {
            clear_time_attr_operate_hal(get_ep_value(1));/*延迟时间到了*/
            break;
        }
        case ONOFFTIME_EVE_USE3:
        {
            clear_time_attr_operate_hal(get_ep_value(2));/*延迟时间到了*/
            break;
        }
        case ONOFFTIME_EVE_USE4:
        {
            clear_time_attr_operate_hal(get_ep_value(3));/*延迟时间到了*/
            break;
        }
        case ONESECDE_EVE_USE:
        {
            OneSecDecFlgFun();
            dev_timer_start_with_callback(ONESECDE_EVE_USE, 1000, dev_onofftime_ledmode_evt_callback);/*100ms超时*/
            break;
        }
        case LED_MODE_EVT_USE:
        {
            syn_led_mode();
            break;
        }
        default:
            break;
    }
}
void zg_light_mode_proc(uint8_t endpoint, CLUSTER_ID_T cluster, uint16_t attr_id)
{
    if(endpoint == 1)
    {
        if(cluster == CLUSTER_ON_OFF_CLUSTER_ID)
        {
            if(attr_id == LIGHT_MODE_ATTR_ID)
            {
                dev_timer_start_with_callback(LED_MODE_EVT_USE, 1, dev_onofftime_ledmode_evt_callback);
            }
        }
    }
}
void Set_Delay_Open_Closed(uint8_t endpoint, uint16_t onTime, uint16_t offWaitTime)
{
    uint16_t delay_time;
    for(uint8_t ii = 0; ii < get_ep_sums();ii ++)
    {
        if(get_ep_value(ii) == endpoint)
        {
            if(check_ep_onoff_status(endpoint))
            {
                zigbee_Ontime_Offwaittime[ii].offwaittime = offWaitTime;
                zigbee_Ontime_Offwaittime[ii].ontime = offWaitTime;
                delay_time = offWaitTime;
            }
            else
            {
                zigbee_Ontime_Offwaittime[ii].ontime = onTime;
                zigbee_Ontime_Offwaittime[ii].offwaittime = offWaitTime;
                delay_time = onTime;
            }
            if(delay_time != 0)
            {
                if(ii == 0)
                {
                    dev_timer_start_with_callback(ONOFFTIME_EVE_USE1, delay_time * 1000, dev_onofftime_ledmode_evt_callback);/*100ms超时*/
                }
                else if(ii == 1)
                {
                    dev_timer_start_with_callback(ONOFFTIME_EVE_USE2, delay_time * 1000, dev_onofftime_ledmode_evt_callback);/*100ms超时*/
                }
                else if(ii == 2)
                {
                    dev_timer_start_with_callback(ONOFFTIME_EVE_USE3, delay_time * 1000, dev_onofftime_ledmode_evt_callback);/*100ms超时*/
                }
                else if(ii == 3)
                {
                    dev_timer_start_with_callback(ONOFFTIME_EVE_USE4, delay_time * 1000, dev_onofftime_ledmode_evt_callback);/*100ms超时*/
                }
            }
            if(!dev_timer_get_valid_flag(ONESECDE_EVE_USE))
            {
                dev_timer_start_with_callback(ONESECDE_EVE_USE, 1000, dev_onofftime_ledmode_evt_callback);/*100ms超时*/
            }
            break;
        }
    }
}

void zigbee_custom_delay_onoff_proc(uint8_t endpoint, uint16_t onTime, uint16_t offWaitTime)
{
    report_ontime_attr_proc(endpoint, onTime, offWaitTime);/*上报ontime or offwaittime*/
    Set_Delay_Open_Closed(endpoint, onTime, offWaitTime);/*延迟开关事件开启，一秒钟时间刷新事件开启*/
    IsInOnTimeOrOffWaitTimeModeProc(endpoint);/*下次主动上报onoff时，需要上报ontime and offwaittime*/
}

