#ifndef __TUYA_REPORT_ATTR_H__
#define __TUYA_REPORT_ATTR_H__

typedef struct {
    uint8_t src_endpoint;
    CLUSTER_ID_T cluster_id;
    uint16_t attr_id;
    ATTR_TYPE_T attr_type;
    uint8_t attr_size;
    uint8_t attr_data[40];
}Report_Attr_data_t;

typedef struct {
  /** The application profile ID that describes the format of the message. */
  uint16_t profileId;
  /** The cluster ID for this message. */
  uint16_t clusterId;
  /** The source endpoint. */
  uint8_t sourceEndpoint;
  /** The destination endpoint. */
  uint8_t destinationEndpoint;
  /** A bitmask of options from the enumeration above. */
  uint16_t options;
  /** The group ID for this message, if it is multicast mode. */
  uint16_t groupId;
  /** The sequence number. */
  uint8_t sequence;
  uint8_t radius;
} EmberApsFrame;


void Device_Report_Attr(Report_Attr_data_t ReportAttr);
void dev_report_onoff_msg(uint8_t ep,uint8_t dev_onoff);
void dev_report_level_msg(uint8_t ep,uint8_t dev_level);
void dev_report_ColorTemp_msg(uint8_t ep,uint8_t dev_colortemp);

void zclGlobalReportAttribute(uint16_t destination,uint8_t srcEndpoint,uint8_t dstEndpoint,\
          uint16_t clusterId,uint16_t attributeId,uint8_t type,uint8_t Reportdata[],uint8_t size);


#endif