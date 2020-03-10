/*************************************************************************************/
/* IAS ZONE SERVER ! */
/*************************************************************************************/
#ifndef __IAS_ZONE_SERVER_H__
#define __IAS_ZONE_SERVER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "zigbee_sdk.h"


typedef enum {
  IAS_ENROLL_RESPONSE_CODE_SUCCESS          = 0x00,
  IAS_ENROLL_RESPONSE_CODE_NOT_SUPPORTED    = 0x01,
  IAS_ENROLL_RESPONSE_CODE_NO_ENROLL_PERMIT = 0x02,
  IAS_ENROLL_RESPONSE_CODE_TOO_MANY_ZONES   = 0x03,
} iasenrollresponsecode;

typedef enum {
  IAS_ZONE_STATE_NOT_ENROLLED = 0x00,
  IAS_ZONE_STATE_ENROLLED     = 0x01,
} iaszonestate;

void ias_zone_status_report(uint8_t ep,uint16_t newstatus,uint16_t dtime);
void ias_zone_info_init_callback(uint8_t ep);
void ias_zone_stack_status_callback(uint8_t nwkstatus);
void ias_zone_unenroll(uint8_t ep);

#ifdef __cplusplus
}
#endif
#endif
