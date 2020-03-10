/*
 * @Author: Deven
 * @email: liming@tuya.com
 * @LastEditors: Deven
 * @file name: &file name&
 * @Description: 
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-04-26 17:03:11
 * @LastEditTime: 2019-06-07 10:27:03
 */
#include "stdio.h"
#include "stdlib.h"
#include "zigbee_sdk.h"
#include "system.h"
#include "zigbee_dev_template.h"
#include "ias_zone_server.h"
#include "app_common.h"
#include "gpio_sensor.h"
#include "app_sensor.h"
#include "zcl_command_micro.h"

#define IAS_ZONE_INVALID_ZONE_ID            0XFF


static uint8_t get_ias_zone_enroll_state(uint8_t ep);
static void set_ias_zone_id(uint8_t ep,uint8_t zoneid);
static uint8_t get_ias_zone_id(uint8_t ep);
static void updata_ias_zone_enroll_state(uint8_t ep,uint8_t enrollstate);

/**
 * @description: when ias zone send status fail will be rejoin
 * @param: {} 
 * @return: none
 */
void ias_zone_send_result_fail(SEND_ST_T send_status, dev_send_data_t *send_data)
{
    if( send_status==SEND_ST_ERR )
    {
        if( nwk_state_get()==NET_LOST )   //nwk lost try to rejoin
        {
            zg_rejoin_manual();
        }
    }
}

/**
 * @description:report onoff status
 * @param: {ep} endpiont id
 * @return: none
 */
static bool_t  zig_ias_status_attr_report( uint8_t ep,
                                        uint16_t newstatus,
                                        uint16_t dtime,
                                        SEND_QOS_T qos
                                        )
{
    tuya_print("IAS----send data\r\n");
    frame_info_t zone_frame_st;
    zone_frame_st.dest_address = 0x0000;
    zone_frame_st.dest_endpoint = 0x01;
    zone_frame_st.source_endpoint = 0x01;
    zone_frame_st.type = APP_OUTGOING_DIRECT;
    bool_t send_status = sendCommandIasZoneClusterZoneStatusChangeNotification(  newstatus,
                                                            0,
                                                            get_ias_zone_id( ep ),
                                                            dtime,
                                                            &zone_frame_st );
    // dev_send_data_t send_data;

    // memset(&send_data, 0, sizeof(dev_send_data_t));

    // send_data.qos = QOS_1;
    // send_data.direction = ZCL_DATA_DIRECTION_SERVER_TO_CLIENT;
    // send_data.command_id = CMD_ZONE_STATUS_CHANGE_NOTIFICATION_COMMAND_ID;
    // send_data.commmand_type = ZCL_COMMAND_CLUSTER_SPEC_CMD;
    // send_data.addr.mode = SEND_MODE_GW;
    // send_data.addr.type.gw.cluster_id = CLUSTER_IAS_ZONE_CLUSTER_ID;
    // send_data.addr.type.gw.src_ep = IAS_ZONE_EP;
    // send_data.delay_time = 0;
    // send_data.random_time = 0;
    // send_data.data.private.data[i++] = newstatus;
    // send_data.data.private.data[i++] = newstatus>>8;
    // send_data.data.private.data[i++] = 0;
    // send_data.data.private.data[i++] = get_ias_zone_id(1);
    // send_data.data.private.data[i++] = 0;
    // send_data.data.private.data[i++] = 0;
    // send_data.data.private.len = i;

    // dev_zigbee_send_data( &send_data, &ias_zone_send_result_fail, 2000 );
    return send_status;
}

/**
 * @description: ias zone status send func
 * @param {type} none
 * @return: none
 */
void ias_zone_status_report(uint8_t ep,uint16_t newstatus,uint16_t dtime)
{
    bool_t send_status;
    send_status = zig_ias_status_attr_report( ep,
                                            newstatus,
                                            dtime,
                                            QOS_1
                                            );
    if( !send_status )
    {
        if( nwk_state_get()==NET_LOST )   //nwk lost try to rejoin
        {
            zg_rejoin_manual();
        }
    }
}

/**
 * @description: send enroll request func
 * @param {type} none
 * @return: none
 */
void zig_ias_enroll_request_with_client(void)
{
    tuya_print("send enroll request data\r\n");
    dev_send_data_t send_data;
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
    send_data.data.private.data[i++] = (uint8_t)IAS_ZONE_TYPE;
    send_data.data.private.data[i++] = IAS_ZONE_TYPE >> 8;
    send_data.data.private.data[i++] = (uint8_t)MANUFACTURER_CODE;
    send_data.data.private.data[i++] = MANUFACTURER_CODE >> 8;
    send_data.data.private.len = i;

    dev_zigbee_send_data( &send_data, NULL, 2000 );  
}

/**
 * @description: ias zone status change func 
 * @param {type} none
 * @return: none
 */
uint8_t ias_zone_server_update_zone_status(uint8_t ep,uint16_t newstatus)
{
    uint16_t delay_time = 0;
    NET_EVT_T nwkstatus = nwk_state_get();
    
    dev_zigbee_write_attribute(
            ep,
            CLUSTER_IAS_ZONE_CLUSTER_ID,
            ATTR_ZONE_STATUS_ATTRIBUTE_ID,
            &newstatus,
            ATTR_BITMAP16_ATTRIBUTE_TYPE
        );
    /* send enroll request only nwk up  */
    if( (nwkstatus==NET_JOIN_OK)||(nwkstatus==NET_REJOIN_OK) )
    {
        if ( get_ias_zone_enroll_state(ep)==IAS_ZONE_STATE_NOT_ENROLLED )
        {
            zig_ias_enroll_request_with_client();
            delay_time = 1500;  // sned enroll request and delay to send ias status
        }
    }
    ias_zone_status_report( ep, newstatus, delay_time );
    return 0;
}

/**
 * @description: ias zone info init
 * @param {type} none
 * @return: none
 */
void ias_zone_info_init_callback(uint8_t ep)
{
    uint16_t zone_type = IAS_ZONE_TYPE;
    uint16_t ias_zone_status = 0;   //
    // write ias zone type
    dev_zigbee_write_attribute(
        ep,
        CLUSTER_IAS_ZONE_CLUSTER_ID,
        ATTR_ZONE_TYPE_ATTRIBUTE_ID,
        &zone_type,
        ATTR_ENUM16_ATTRIBUTE_TYPE
    );
    /* 以下部分可以不要，在此之前已经写入过ias zone status 了 */
    // gpiostate alarm1pin= get_gpio_sensor_value();
    // tuya_print( "---alarm1pin %d\r\n",alarm1pin );
    // if( alarm1pin==GPIO_SENSOR_ACTIVE ){
    //     ias_zone_status = ias_zone_status | IAS_ZONE_SENSOR_STATUS_ALARM1;
    // }
    // else{
    //     ias_zone_status = ias_zone_status & (~IAS_ZONE_SENSOR_STATUS_ALARM1);
    // }
    // //write ias zone status
    // dev_zigbee_write_attribute(
    //     ep,
    //     CLUSTER_IAS_ZONE_CLUSTER_ID,
    //     ATTR_ZONE_STATUS_ATTRIBUTE_ID,
    //     &ias_zone_status,
    //     ATTR_BITMAP16_ATTRIBUTE_TYPE
    // );
}

/**
 * @description: unenroll to clear ias zone info
 * @param {type} none
 * @return: none
 */
void ias_zone_unenroll(uint8_t ep)
{
    uint8_t ieee_address[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    uint16_t zone_type = IAS_ZONE_TYPE;
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
        &zone_type,
        ATTR_ENUM16_ATTRIBUTE_TYPE
    );
    set_ias_zone_id( ep, IAS_ZONE_INVALID_ZONE_ID );
    updata_ias_zone_enroll_state(  ep, IAS_ZONE_STATE_NOT_ENROLLED );
}

/**
 * @description: ias zone info to init status when stack change to no nwk 
 * @param {type} none
 * @return: none
 */
void ias_zone_stack_status_callback(uint8_t nwkstatus)
{
    uint8_t epindex;
    // leave nwk must to unenroll
    if( (nwkstatus==NET_LOCAL_LEAVE)||(nwkstatus==NET_REMOTE_LEAVE) )
    {
        tuya_print( "ias zone unenroll %d\r\n",nwkstatus );
        for( epindex=1;epindex<=DEV_EP_SUM;epindex++ )
        {
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
    tuya_print("ias zone ice ieee write\r\n");
    // not enroll must to enroll request
    if ( get_ias_zone_enroll_state( IAS_ZONE_EP )==IAS_ZONE_STATE_NOT_ENROLLED )
    {
        zig_ias_enroll_request_with_client();
    }
}

/**
 * @description: get ias zone id 
 * @param {type} none
 * @return: none
 */
static uint8_t get_ias_zone_id(uint8_t ep)
{
    uint8_t iaszoneid;
    dev_zigbee_read_attribute(
                ep,
                CLUSTER_IAS_ZONE_CLUSTER_ID,
                ATTR_ZONE_ID_ATTRIBUTE_ID,
                &iaszoneid,
                sizeof(iaszoneid)
            );
    return iaszoneid;
}

/**
 * @description: set ias zone id after receive enroll response
 * @param {type} none
 * @return: none
 */
static void set_ias_zone_id(uint8_t ep,uint8_t zoneid)
{
    tuya_print("ias zone id write%d\r\n",zoneid);
    dev_zigbee_write_attribute(
        ep,
        CLUSTER_IAS_ZONE_CLUSTER_ID,
        ATTR_ZONE_ID_ATTRIBUTE_ID,
        &zoneid,
        ATTR_INT8U_ATTRIBUTE_TYPE
    );
}

/**
 * @description: set ias zone state after receive enroll response
 * @param {type} none
 * @return: none
 */
static void updata_ias_zone_enroll_state(uint8_t ep,uint8_t enrollstate)
{
    tuya_print(" ias state write %d\r\n",enrollstate);
    dev_zigbee_write_attribute(
        ep,
        CLUSTER_IAS_ZONE_CLUSTER_ID,
        ATTR_ZONE_STATE_ATTRIBUTE_ID,
        &enrollstate,
        ATTR_ENUM8_ATTRIBUTE_TYPE
    );
}
/**
 * @description: ias zone enroll response callback func
 * @param {type} none
 * @return: none
 */
void ias_zone_enroll_response_callback(uint8_t ep,uint8_t enrollcode,uint8_t zoneid)
{
    tuya_print("ias zone enroll response\r\n");
    if ( enrollcode == IAS_ENROLL_RESPONSE_CODE_SUCCESS ) {             //enroll response sucess
        updata_ias_zone_enroll_state(  ep, IAS_ZONE_STATE_ENROLLED );   //write ias zone state enrolled
        set_ias_zone_id( ep, zoneid );                                  //write valid zone id
    }else{
        updata_ias_zone_enroll_state(  ep, IAS_ZONE_STATE_NOT_ENROLLED );
        set_ias_zone_id( ep, IAS_ZONE_INVALID_ZONE_ID );
    }
}
/**
 * @description: get ias zone state
 * @param {type} none
 * @return: none
 */
static uint8_t get_ias_zone_enroll_state(uint8_t ep)
{
    uint8_t zonestate = IAS_ZONE_STATE_NOT_ENROLLED;
    dev_zigbee_read_attribute(
                ep,
                CLUSTER_IAS_ZONE_CLUSTER_ID,
                ATTR_ZONE_STATE_ATTRIBUTE_ID,
                &zonestate,
                sizeof(zonestate)
            );
    tuya_print("ias zone enroll status get %d\r\n",zonestate);
    return zonestate;
}

