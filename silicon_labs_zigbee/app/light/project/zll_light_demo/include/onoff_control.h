#ifndef __TUYA_ONOFF_CONTROL_H__
#define __TUYA_ONOFF_CONTROL_H__

bool_t Dev_OnOff_op(uint8_t ep, bool_t Onoff);
bool_t OnOffClusterServerCommandParse(uint8_t endpoint,attr_value_t ControlCmd);

#endif