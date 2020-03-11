#ifndef __TUYA_COLOR_CONTROL_H__
#define __TUYA_COLOR_CONTROL_H__

bool_t Dev_ColorTemp_op(uint8_t ep, uint16_t  TempVal);
bool_t ColorControlClusterServerCommandParse(uint8_t endpoint,attr_value_t ControlCmd);

#endif