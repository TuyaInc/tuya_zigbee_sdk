#ifndef __TUYA_LEVEL_CONTROL_H__
#define __TUYA_LEVEL_CONTROL_H__


bool_t Dev_Level_op(uint8_t ep, uint8_t  Levelval);
bool_t LevelControlClusterServerCommandParse(uint8_t endpoint,attr_value_t ControlCmd);

#endif