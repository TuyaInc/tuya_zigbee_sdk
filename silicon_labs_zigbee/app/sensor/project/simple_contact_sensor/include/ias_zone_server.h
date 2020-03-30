/*************************************************************************************/
/* IAS ZONE SERVER ! */
/*************************************************************************************/
#ifndef __IAS_ZONE_SERVER_H__
#define __IAS_ZONE_SERVER_H__

#include "tuya_zigbee_sdk.h"

#ifdef __cplusplus
extern "C" {
#endif

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

uint8_t get_ias_zone_id(uint8_t ep);
void ias_zone_info_init_callback(uint8_t ep,uint16_t zone_type);
void ias_zone_stack_status_callback(uint8_t nwkstatus);
void ias_zone_unenroll(uint8_t ep);
void zig_ias_enroll_request_with_client(void);
uint8_t get_ias_zone_enroll_state(uint8_t ep);
void ias_zone_cie_address_write_callback(void);
void ias_zone_enroll_response_callback(uint8_t ep,uint8_t enroll_code,uint8_t zone_id);

#ifdef __cplusplus
}
#endif
#endif
