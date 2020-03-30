/*
 * @file: 
 * @brief: 
 * @author: Deven
 * @date: 2019-11-13 20:17:48
 * @email: liming@tuya.com
 * @copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @company: http://www.tuya.com
 */

/*
 * ias device enroll step(Auto-Enroll-Request):
 *              GW                                                                     DEV
 * 
 *                                            (IEEE Address)
 *      IAS zone (write Attribute)             ----------->
 *                                            (sucess/fail)
 *                                             <-----------                 IAS zone (write Attribute response)
 * 
 *                                                                          if(sucess)
 *                                       (zone type + manufctuer code)
 *                                             <-----------                 IAS zone (Zone Enroll request)
 *                                               (zone id)
 *      IAS zone (Zone Enroll response)         ------------>
 *                                             (IEEE Address)
 *      IAS zone (read Attribute)               ------------>       
 *                                              (IEEE Address)
 *                                             <-----------                 IAS zone (read Attribute response)
 *                                         (zone state + zone type)
 *      IAS zone (read Attribute)               ------------>
 *                                        (enrolled/not enroll + type )
 *                                             <-----------                 IAS zone (read Attribute response)
 *                                              (zone status)
 *      IAS zone (read Attribute)               ------------>
 *                                              (zone status)
 *                                             <-----------                 IAS zone (read Attribute response)
 * 
*/

#include "stdio.h"
#include "stdlib.h"
#include "config.h"
#include "tuya_zigbee_sdk.h"
#include "app_common.h"
#include "ias_zone_server.h"
#include "zigbee_dev_template.h"

#define IAS_ZONE_INVALID_ZONE_ID            0XFF


uint8_t get_ias_zone_enroll_state(uint8_t ep);
static void set_ias_zone_id(uint8_t ep,uint8_t zoneid);
static void updata_ias_zone_enroll_state(uint8_t ep,uint8_t enrollstate);

/**
 * @description: send enroll request func
 * @param {type} none
 * @return: none
 */
void zig_ias_enroll_request_with_client(void)
{
    dev_send_data_t send_data;
    uint16_t ias_zone_type = IAS_ZONE_TYPE;
    uint8_t i = 0;
    memset( &send_data, 0, sizeof(dev_send_data_t) );

    send_data.qos = QOS_1;
    send_data.direction = ZCL_DATA_DIRECTION_SERVER_TO_CLIENT;
    send_data.command_id = CMD_ZONE_ENROLL_REQUEST_COMMAND_ID;
    send_data.commmand_type = ZCL_COMMAND_CLUSTER_SPEC_CMD;
    send_data.addr.mode = SEND_MODE_GW;
    send_data.addr.type.gw.cluster_id = CLUSTER_IAS_ZONE_CLUSTER_ID;
    send_data.addr.type.gw.src_ep = 1;
    send_data.delay_time = 0;
    send_data.random_time = 0;
    send_data.data.private.data[i++] = (uint8_t)ias_zone_type;
    send_data.data.private.data[i++] = ias_zone_type >> 8;
    send_data.data.private.data[i++] = (uint8_t)MANUFACTURER_CODE;
    send_data.data.private.data[i++] = MANUFACTURER_CODE >> 8;
    send_data.data.private.len = i;

    dev_zigbee_send_data( &send_data, NULL, 3000 );             
}

/**
 * @description: ias zone info init
 * @param {in} ep: endpoind
 * @param {in} zone_type: ias zone type
 * @return: none
 */
void ias_zone_info_init_callback(uint8_t ep,uint16_t zone_type)
{
    uint16_t ias_zone_status = 0;
    // write ias zone type
    dev_zigbee_write_attribute(
        ep,
        CLUSTER_IAS_ZONE_CLUSTER_ID,
        ATTR_ZONE_TYPE_ATTRIBUTE_ID,
        &zone_type,
        ATTR_ENUM16_ATTRIBUTE_TYPE
    );

    //write ias zone status
    dev_zigbee_write_attribute(
        ep,
        CLUSTER_IAS_ZONE_CLUSTER_ID,
        ATTR_ZONE_STATUS_ATTRIBUTE_ID,
        &ias_zone_status,
        ATTR_BITMAP16_ATTRIBUTE_TYPE
    );
}

/**
 * @description: unenroll to clear ias zone info
 * @param {in} ep: endpoind
 * @return: none
 */
void ias_zone_unenroll(uint8_t ep)
{
    uint8_t ieee_address[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    uint16_t ias_zone_type = IAS_ZONE_TYPE;

    dev_zigbee_write_attribute(
        ep,
        CLUSTER_IAS_ZONE_CLUSTER_ID,
        ATTR_IAS_CIE_ADDRESS_ATTRIBUTE_ID,
        (uint8_t *)ieee_address,
        ATTR_IEEE_ADDRESS_ATTRIBUTE_TYPE
    );

   dev_zigbee_write_attribute(
        ep,
        CLUSTER_IAS_ZONE_CLUSTER_ID,
        ATTR_ZONE_TYPE_ATTRIBUTE_ID,
        &ias_zone_type,
        ATTR_ENUM16_ATTRIBUTE_TYPE
    );

    set_ias_zone_id( ep, IAS_ZONE_INVALID_ZONE_ID );
    updata_ias_zone_enroll_state(  ep, IAS_ZONE_STATE_NOT_ENROLLED );
}

/**
 * @description: ias zone info to init status when stack change to no nwk 
 * @param {in} nwkstatus : zigbee network status
 * @return: none
 */
void ias_zone_stack_status_callback(uint8_t nwkstatus)
{
    uint8_t epindex;

    // leave nwk must to unenroll
    if( (nwkstatus==NET_LOCAL_LEAVE)||(nwkstatus==NET_REMOTE_LEAVE) || (nwkstatus==NET_MF_TEST_LEAVE) )
    {
        for( epindex=1;epindex<=DEV_EP_SUM;epindex++ ) {
           ias_zone_unenroll( epindex );
        }
    }
}

/**
 * @description: send enroll request atfer gtw write ieee address 
 * @param {type} none
 * @return: none
 */
void ias_zone_cie_address_write_callback(void)
{
    // tuya_print("ias zone ice ieee write\r\n");
    // not enroll must to enroll request
    if ( get_ias_zone_enroll_state( IAS_ZONE_EP )==IAS_ZONE_STATE_NOT_ENROLLED )
    {
        zig_ias_enroll_request_with_client();
    }
}

/**
 * @description: get ias zone id 
 * @param {in} ep: endpoind
 * @return: none
 */
uint8_t get_ias_zone_id(uint8_t ep)
{
    uint8_t ias_zone_id;

    dev_zigbee_read_attribute(
                ep,
                CLUSTER_IAS_ZONE_CLUSTER_ID,
                ATTR_ZONE_ID_ATTRIBUTE_ID,
                &ias_zone_id,
                sizeof(ias_zone_id)
            );
    
    return ias_zone_id;
}

/**
 * @description: set ias zone id after receive enroll response
 * @param {in} ep: endpoind
 * @param {in} zoneid: ias_zone id
 * @return: none
 */
static void set_ias_zone_id(uint8_t ep,uint8_t zone_id)
{
    // tuya_print("ias zone id write %d\r\n",zoneid);
    dev_zigbee_write_attribute(
        ep,
        CLUSTER_IAS_ZONE_CLUSTER_ID,
        ATTR_ZONE_ID_ATTRIBUTE_ID,
        &zone_id,
        ATTR_INT8U_ATTRIBUTE_TYPE
    );
}

/**
 * @description: set ias zone state after receive enroll response
 * @param {in} ep: endpoind
 * @param {in} enroll_state: enroll state
 * @return: none
 */
static void updata_ias_zone_enroll_state(uint8_t ep,uint8_t enroll_state)
{
    // tuya_print(" ias state write %d\r\n",enrollstate);
    dev_zigbee_write_attribute(
        ep,
        CLUSTER_IAS_ZONE_CLUSTER_ID,
        ATTR_ZONE_STATE_ATTRIBUTE_ID,
        &enroll_state,
        ATTR_ENUM8_ATTRIBUTE_TYPE
    );
}
/**
 * @description: ias zone enroll response callback func
 * @param {in} ep: endpoind
 * @param {in} enroll_code: enroll code
 * @param {in} zone_id: ias_zone id
 * @return: none
 */
void ias_zone_enroll_response_callback(uint8_t ep,uint8_t enroll_code,uint8_t zone_id)
{
    // tuya_print("ias zone enroll response\r\n");
    if ( enroll_code == IAS_ENROLL_RESPONSE_CODE_SUCCESS ) {            //enroll response sucess
        updata_ias_zone_enroll_state(  ep, IAS_ZONE_STATE_ENROLLED );   //write ias zone state enrolled
        set_ias_zone_id( ep, zone_id );                                 //write valid zone id
    }else{
        updata_ias_zone_enroll_state(  ep, IAS_ZONE_STATE_NOT_ENROLLED );
        set_ias_zone_id( ep, IAS_ZONE_INVALID_ZONE_ID );
    }
}
/**
 * @description: get ias zone state
 * @param {in} ep: endpoind
 * @return: none
 */
uint8_t get_ias_zone_enroll_state(uint8_t ep)
{
    uint8_t zonestate = IAS_ZONE_STATE_NOT_ENROLLED;
    dev_zigbee_read_attribute(
                ep,
                CLUSTER_IAS_ZONE_CLUSTER_ID,
                ATTR_ZONE_STATE_ATTRIBUTE_ID,
                &zonestate,
                sizeof(zonestate)
            );
    // tuya_print("ias zone enroll status get %d\r\n",zonestate);
    return zonestate;
}

