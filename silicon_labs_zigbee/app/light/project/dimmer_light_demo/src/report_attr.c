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
#if 1
    dev_zigbee_send_data(&send_data, NULL, 1000);    
#else
    zclGlobalReportAttribute(0x0000,ReportAttr.src_endpoint,0x01,\
          ReportAttr.cluster_id,ReportAttr.attr_id,ReportAttr.attr_type,ReportAttr.attr_data,ReportAttr.attr_size);
    
    
#endif
}

#if 1
extern uint8_t emberAfNextSequence(void);
extern uint8_t emberAfSendUnicast(uint8_t type,
                               uint16_t indexOrDestination,
                               EmberApsFrame *apsFrame,
                               uint16_t messageLength,
                               uint8_t *message);
//mask : 1 ZCL_FRAME_CONTROL_SERVER_TO_CLIENT   0  ZCL_FRAME_CONTROL_CLIENT_TO_SERVER
void zclGlobalReportAttribute(uint16_t destination,uint8_t srcEndpoint,uint8_t dstEndpoint,\
          uint16_t clusterId,uint16_t attributeId,uint8_t type,uint8_t Reportdata[],uint8_t size)
{
  EmberApsFrame ReportApsFrame;
  uint8_t zclReportBuffer[128];
  uint8_t ZclReportBufferLen;
  uint16_t ReportmfgSpecificId = 0x0000;//EMBER_AF_NULL_MANUFACTURER_CODE;
  
  ReportApsFrame.options = 0x0040 | 0x0100 | 0x1000 ;//EMBER_APS_OPTION_RETRY;
  ReportApsFrame.clusterId = clusterId;
  ReportApsFrame.sourceEndpoint = srcEndpoint; 
  ReportApsFrame.destinationEndpoint = dstEndpoint;
  
  uint8_t index = 0;
//  emAfApsFrameClusterIdSetup(clusterId);
//  zclReportBuffer[index++] = (ZCL_GLOBAL_COMMAND | ZCL_FRAME_CONTROL_SERVER_TO_CLIENT
//                           | ZCL_FRAME_CONTROL_CLIENT_TO_SERVER
//                           | (ReportmfgSpecificId != EMBER_AF_NULL_MANUFACTURER_CODE
//                              ? ZCL_MANUFACTURER_SPECIFIC_MASK
//                              : 0)
//                           | (DisableDefaultResponse
//                              ? ZCL_DISABLE_DEFAULT_RESPONSE_MASK
//                              : 0));
  
  zclReportBuffer[index++] = (0x0 | (1<<3)| 0x0| 0| (1<<4));  
  zclReportBuffer[index++] = emberAfNextSequence();
  zclReportBuffer[index++] = 0x0A;//ZCL_REPORT_ATTRIBUTES_COMMAND_ID;
  zclReportBuffer[index++] = (attributeId>>0)&0xff;
  zclReportBuffer[index++] = (attributeId>>8)&0xff;
  zclReportBuffer[index++] = type;
  ZclReportBufferLen = index;

  memcpy(zclReportBuffer + ZclReportBufferLen, Reportdata, size);

  ZclReportBufferLen += size;  
  
  emberAfSendUnicast(0x0,
                     destination,
                     &ReportApsFrame,
                     ZclReportBufferLen,
                     zclReportBuffer);
}

#endif

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