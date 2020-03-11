#ifndef __TUYA_COMMON_DEF_H__
#define __TUYA_COMMON_DEF_H__

#define ENDPOINT_COUTER     1

#define LED_ZIGBEE_ST_ID    0
#define LED_LIGHT_ST_ID     1

#define SCENE_ONOFF_EVENT_ID        DEV_EVT_4 
#define SCENE_LEVEL_EVENT_ID        DEV_EVT_5
#define SCENE_COLORTEMP_EVENT_ID    DEV_EVT_6

#define SCENE_TRUE      1
#define SCENE_FALSE     0

#define SCENE_CONTROL_TIMEOUT       15000

extern uint8_t Dev_onoff_status[ENDPOINT_COUTER];
extern uint8_t Dev_level_status[ENDPOINT_COUTER];
extern uint16_t Dev_colortemp_status[ENDPOINT_COUTER];
extern const dev_description_t g_dev_des[];


extern uint8_t SceneOnOff_Flag;
extern uint8_t SceneLevel_Flag;
extern uint8_t SceneColorTemp_Flag;
extern void SceneControlOnOffEventCallback(uint8_t evnent_id);
extern void SceneControlLevelEventCallback(uint8_t evnent_id);
extern void SceneControlColorTempEventCallback(uint8_t evnent_id);


#define DIMMER_ON_OFF_LIGHT_ATTR_LIST \
    { 0x0000, ATTR_BOOLEAN_ATTRIBUTE_TYPE, 1, (ATTR_MASK_WRITABLE), { (uint8_t*)0x00 } }, /* 20 / On/off / on/off*/\

#define DIMMER_LEVEL_CONTROL_ATTR_LIST \
    { 0x0000, ATTR_INT8U_ATTRIBUTE_TYPE, 1, (ATTR_MASK_WRITABLE), { (uint8_t*)0x00 } }, /* 20 / Level Control / Current Level*/\

#define DIMMER_COLOR_CONTROL_ATTR_LIST \
    { 0x0000, ATTR_INT8U_ATTRIBUTE_TYPE, 1, (ATTR_MASK_WRITABLE), { (uint8_t*)0x10 } }, /* 41 / Color Control / current hue*/\
    { 0x0001, ATTR_INT8U_ATTRIBUTE_TYPE, 1, (ATTR_MASK_WRITABLE), { (uint8_t*)0x10 } }, /* 42 / Color Control / current saturation*/\
    { 0x0002, ATTR_INT16U_ATTRIBUTE_TYPE, 2, (0x00), { (uint8_t*)0x0000 } }, /* 43 / Color Control / remaining time*/\
    { 0x0003, ATTR_INT16U_ATTRIBUTE_TYPE, 2, (ATTR_MASK_WRITABLE), { (uint8_t*)0x0000 } }, /* 44 / Color Control / current x*/\
    { 0x0004, ATTR_INT16U_ATTRIBUTE_TYPE, 2, (ATTR_MASK_WRITABLE), { (uint8_t*)0x0000 } }, /* 45 / Color Control / current y*/\
    { 0x0007, ATTR_INT16U_ATTRIBUTE_TYPE, 2, (ATTR_MASK_WRITABLE), { (uint8_t*)0x0000 } }, /* 46 / Color Control / color temperature*/\
    { 0x0008, ATTR_ENUM8_ATTRIBUTE_TYPE, 1, (ATTR_MASK_WRITABLE), { (uint8_t*)0x02 } }, /* 47 / Color Control / color mode*/\

#endif