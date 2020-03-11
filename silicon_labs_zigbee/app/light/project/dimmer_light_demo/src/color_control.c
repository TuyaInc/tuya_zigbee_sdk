#include "zigbee_dev_template.h"
#include "zigbee_cmd.h"
#include "zigbee_attr.h"
#include "zigbee_sdk.h"
#include "common_def.h"
#include "color_control.h"
#include "report_attr.h"

bool_t ColorControlClusterServerCommandParse(uint8_t endpoint,attr_value_t ControlCmd)
{
    switch(ControlCmd.cmd) {
        case CMD_MOVE_TO_COLOR_TEMPERATURE_COMMAND_ID: 
          {
            uint8_t payloadOffset = 0;
            uint16_t colorTemperature;  
            uint16_t transitionTime;  
            colorTemperature = ControlCmd.value[payloadOffset+1];
            colorTemperature <<= 8;
            colorTemperature |= ControlCmd.value[payloadOffset];
            payloadOffset += 2;
            transitionTime = ControlCmd.value[payloadOffset+1];
            transitionTime <<= 8;
            transitionTime |= ControlCmd.value[payloadOffset];
            Dev_ColorTemp_op(endpoint,colorTemperature);
          }
            break;      
        default: 
            break; 
    }
    SceneColorTemp_Flag = SCENE_TRUE; 
    dev_timer_start_with_callback(SCENE_COLORTEMP_EVENT_ID, SCENE_CONTROL_TIMEOUT, SceneControlColorTempEventCallback);
    dev_report_ColorTemp_msg(endpoint,Dev_colortemp_status[endpoint-1]);
    return TRUE;
}