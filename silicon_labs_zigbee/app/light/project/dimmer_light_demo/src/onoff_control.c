#include "zigbee_dev_template.h"
#include "zigbee_cmd.h"
#include "zigbee_attr.h"
#include "zigbee_sdk.h"
#include "common_def.h"
#include "onoff_control.h"
#include "report_attr.h"


bool_t OnOffClusterServerCommandParse(uint8_t endpoint,attr_value_t ControlCmd)
{
    switch(ControlCmd.cmd) {
        case CMD_OFF_COMMAND_ID: 
            Dev_OnOff_op(endpoint, DEV_IO_OFF);
            break;
        
        case CMD_ON_COMMAND_ID: 
            Dev_OnOff_op(endpoint, DEV_IO_ON);
            break;
        
        case CMD_TOGGLE_COMMAND_ID: 
            Dev_OnOff_op(endpoint, (DEV_IO_ST_T)!Dev_onoff_status[endpoint-1]);
            break;
        
        default: 
            break; 
    }
    SceneOnOff_Flag = SCENE_TRUE; 
    dev_timer_start_with_callback(SCENE_ONOFF_EVENT_ID, SCENE_CONTROL_TIMEOUT, SceneControlOnOffEventCallback);
    dev_report_onoff_msg(endpoint,Dev_onoff_status[endpoint-1]);
    return TRUE;
}

