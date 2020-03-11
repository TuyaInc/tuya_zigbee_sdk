#include "zigbee_dev_template.h"
#include "zigbee_cmd.h"
#include "zigbee_attr.h"
#include "zigbee_sdk.h"
#include "common_def.h"
#include "level_control.h"
#include "report_attr.h"

bool_t LevelControlClusterServerCommandParse(uint8_t endpoint,attr_value_t ControlCmd)
{
    switch(ControlCmd.cmd) {
        case CMD_MOVE_TO_LEVEL_COMMAND_ID:
          {
            uint8_t payloadOffset=0;
            uint8_t level;  
            uint16_t transitionTime;
            level = ControlCmd.value[payloadOffset++];
            transitionTime = ControlCmd.value[payloadOffset+1];
            transitionTime <<= 8;
            transitionTime |= ControlCmd.value[payloadOffset];
            Dev_Level_op(endpoint,level);
           }
            break;
        case CMD_MOVE_COMMAND_ID: 

            break;
        
        case CMD_STEP_COMMAND_ID: 

            break;
        case CMD_STOP_COMMAND_ID: 

            break;
        case CMD_MOVE_TO_LEVEL_WITH_ON_OFF_COMMAND_ID: 
            break;        
        default: 
            break; 
    }
    SceneLevel_Flag = SCENE_TRUE; 
    dev_timer_start_with_callback(SCENE_LEVEL_EVENT_ID, SCENE_CONTROL_TIMEOUT, SceneControlLevelEventCallback);
    dev_report_level_msg(endpoint,Dev_level_status[endpoint-1]);
    return TRUE;
}