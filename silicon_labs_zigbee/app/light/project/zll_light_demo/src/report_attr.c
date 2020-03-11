#include "zigbee_dev_template.h"
#include "zigbee_cmd.h"
#include "zigbee_attr.h"
#include "zigbee_sdk.h"
#include "report_attr.h"


void Device_Report_Attr(Report_Attr_data_t ReportAttr)
{
    dev_send_data_t send_data;

    memset(&send_data, 0, sizeof(dev_send_data_t));

    send_data.zcl_id = 0;//++zcl_seq;
    send_data.qos = QOS_1;
    send_data.direction = ZCL_DATA_DIRECTION_SERVER_TO_CLIENT;
    send_data.command_id = CMD_REPORT_ATTRIBUTES_COMMAND_ID;
    send_data.addr.mode = SEND_MODE_GW;
    send_data.addr.type.gw.cluster_id = ReportAttr.cluster_id;
    send_data.addr.type.gw.src_ep = ReportAttr.src_endpoint;
    send_data.delay_time = 0;
    send_data.random_time = 0;
    send_data.data.zg.attr_sum = 1;
    send_data.data.zg.attr[0].attr_id = ReportAttr.attr_id;
    send_data.data.zg.attr[0].type = ReportAttr.attr_type;
    send_data.data.zg.attr[0].value_size = ReportAttr.attr_size;
    memcpy(send_data.data.zg.attr[0].value,ReportAttr.attr_data,ReportAttr.attr_size);
    dev_zigbee_send_data(&send_data, NULL, 1000);    
}


void dev_report_onoff_msg(uint8_t ep,uint8_t dev_onoff)
{
    Report_Attr_data_t OnOffAttr;
    OnOffAttr.src_endpoint = ep;
    OnOffAttr.cluster_id = CLUSTER_ON_OFF_CLUSTER_ID;
    OnOffAttr.attr_id = ATTR_ON_OFF_ATTRIBUTE_ID;
    OnOffAttr.attr_type = ATTR_BOOLEAN_ATTRIBUTE_TYPE;
    OnOffAttr.attr_size = 1;
    OnOffAttr.attr_data[0] = dev_onoff;
    Device_Report_Attr(OnOffAttr);   
    dev_zigbee_write_attribute(
            OnOffAttr.src_endpoint,
            OnOffAttr.cluster_id,
            OnOffAttr.attr_id,
            OnOffAttr.attr_data,
            OnOffAttr.attr_type
        );
}

void dev_report_level_msg(uint8_t ep,uint8_t dev_level)
{
    Report_Attr_data_t LevelAttr;
    LevelAttr.src_endpoint = ep;
    LevelAttr.cluster_id = CLUSTER_LEVEL_CONTROL_CLUSTER_ID;
    LevelAttr.attr_id = ATTR_CURRENT_LEVEL_ATTRIBUTE_ID;
    LevelAttr.attr_type = ATTR_INT8U_ATTRIBUTE_TYPE;
    LevelAttr.attr_size = 1;
    LevelAttr.attr_data[0]= dev_level;
    Device_Report_Attr(LevelAttr);   
    dev_zigbee_write_attribute(
            LevelAttr.src_endpoint,
            LevelAttr.cluster_id,
            LevelAttr.attr_id,
            LevelAttr.attr_data,
            LevelAttr.attr_type
        );
}

void dev_report_ColorTemp_msg(uint8_t ep,uint8_t dev_colortemp)
{
    Report_Attr_data_t ColorTempAttr;
    ColorTempAttr.src_endpoint = ep;
    ColorTempAttr.cluster_id = CLUSTER_COLOR_CONTROL_CLUSTER_ID;
    ColorTempAttr.attr_id = ATTR_COLOR_CONTROL_COLOR_TEMPERATURE_ATTRIBUTE_ID;
    ColorTempAttr.attr_type = ATTR_INT16U_ATTRIBUTE_TYPE;
    ColorTempAttr.attr_size = 2;
    ColorTempAttr.attr_data[0] = (uint8_t)(dev_colortemp>>0)&0xff;
    ColorTempAttr.attr_data[1] = (uint8_t)(dev_colortemp>>8)&0xff;
    Device_Report_Attr(ColorTempAttr);   
    dev_zigbee_write_attribute(
            ColorTempAttr.src_endpoint,
            ColorTempAttr.cluster_id,
            ColorTempAttr.attr_id,
            ColorTempAttr.attr_data,
            ColorTempAttr.attr_type
        );
}