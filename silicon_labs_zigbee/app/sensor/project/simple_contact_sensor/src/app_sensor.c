/*
 * @file: 
 * @brief: 
 * @author: Deven
 * @date: 2019-11-13 20:17:48
 * @email: liming@tuya.com
 * @copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @company: http://www.tuya.com
 */

#include "stdio.h"
#include "stdlib.h"
#include "tuya_zigbee_sdk.h"
#include "config.h"
#include "app_common.h"
#include "app_sensor.h"
#include "gpio_sensor.h"
#include "ias_zone_server.h"

#define IAS_ZONE_ENDPOINT               0x01
#define IAS_ZONE_STATUS_BIT_ACTIVE      0x01
#define IAS_ZONE_STATUS_BIT_INACTIVE    0x00

union {
    uint16_t ias_zone_status;
    struct {
        uint16_t alarm1_status_bit0:        1;
        uint16_t alarm2_status_bit1:        1;
        uint16_t tamper_status_bit2:        1;
        uint16_t battery_status_bit3:       1;
        uint16_t supervision_status_bit4:   1;
        uint16_t restore_status_bit5:       1;
        uint16_t trouble_status_bit6:       1;
        uint16_t ac_mains_status_bit7:      1;
    }zone_bits;
}ias_zone_status_bits;

/**
 * @description: ias zone sensor alarm1 status change callback
 * @param {in} status: ias zone status
 * @return: none
 */
VIRTUAL_FUNC void ias_zone_alarm1_pre_change_callback(uint16_t status)
{
    tuya_print( "ias_zone_alarm1_pre_change_callback %d\r\n",status );
}


/**
 * @description: ias zone sensor status pre-report callback
 * @param {in} status: ias zone status
 * @return: none
 */
VIRTUAL_FUNC bool_t ias_zone_status_pre_update_callback(uint16_t status)
{
    dev_zigbee_write_attribute(
        IAS_ZONE_ENDPOINT,
        CLUSTER_IAS_ZONE_CLUSTER_ID,
        ATTR_ZONE_STATUS_ATTRIBUTE_ID,
        &status,
        ATTR_BITMAP16_ATTRIBUTE_TYPE
    );
    return TRUE;
}

void ias_zone_status_init(void)
{
    ias_zone_status_bits.ias_zone_status = 0;
}

void quicken_arrive_lose_status_handler(uint8_t evt)
{
    ias_zone_status_update();
}

void zig_poll_status_evt_callback(cb_args_t *data_st)
{
    if( data_st->args.poll.result == POLL_OK ) {
        dev_timer_stop( QUICKEN_ARRIVE_LOSE_STATUS_EVT_ID );
    }
}

/**
 * @description: when ias zone send status fail will be rejoin
 * @param: {in} send_status: send status
 * @param: {in} send_data: send data point
 * @return: none
 */
void ias_zone_send_result_fail(SEND_ST_T send_status, dev_send_data_t *send_data)
{
    static uint16_t cunt = 0;

    NET_EVT_T nwkstatus = nwk_state_get();
    if( send_status == SEND_ST_ERR && 
        (nwkstatus == NET_JOIN_OK || nwkstatus == NET_REJOIN_OK ) ) {
            
        if( send_data->addr.type.gw.cluster_id == CLUSTER_IAS_ZONE_CLUSTER_ID &&
            send_data->command_id==CMD_ZONE_STATUS_CHANGE_NOTIFICATION_COMMAND_ID ) {
            dev_timer_start_with_callback( QUICKEN_ARRIVE_LOSE_STATUS_EVT_ID, 2000, quicken_arrive_lose_status_handler );
        }
        cunt++;
    }
}

/**
 * @description:report onoff status
 * @param: {in} ep: endpiont
 * @param: {in} status: ias zone status
 * @param: {in} dtime: delay time send
 * @param: {in} qos: service quality
 * @return: none
 */
static bool_t  zig_ias_status_attr_report( uint8_t ep,
                                        uint16_t status,
                                        uint16_t dtime,
                                        SEND_QOS_T qos
                                        )
{
    bool_t send_status = FALSE;
    uint8_t i = 0;
    dev_send_data_t send_data;

    memset(&send_data, 0, sizeof(dev_send_data_t));

    send_data.qos = qos;
    send_data.direction = ZCL_DATA_DIRECTION_SERVER_TO_CLIENT;
    send_data.command_id = CMD_ZONE_STATUS_CHANGE_NOTIFICATION_COMMAND_ID;
    send_data.commmand_type = ZCL_COMMAND_CLUSTER_SPEC_CMD;
    send_data.addr.mode = SEND_MODE_GW;
    send_data.addr.type.gw.cluster_id = CLUSTER_IAS_ZONE_CLUSTER_ID;
    send_data.addr.type.gw.src_ep = ep;
    send_data.delay_time = 0;
    send_data.random_time = 0;
    send_data.data.private.data[i++] = status;
    send_data.data.private.data[i++] = status>>8;
    send_data.data.private.data[i++] = 0;
    send_data.data.private.data[i++] = get_ias_zone_id(1);
    send_data.data.private.data[i++] = 0;
    send_data.data.private.data[i++] = 0;
    send_data.data.private.len = i;

    dev_zigbee_send_data( &send_data, &ias_zone_send_result_fail, dtime );
    return send_status;
}

/**
 * @description: ias zone status send func
 * @param: {in} ep: endpiont
 * @param: {in} status: ias zone status
 * @param: {in} dtime: delay time send
 * @return: none
 */
void ias_zone_status_report(uint8_t ep,uint16_t status,uint16_t dtime)
{
    bool_t send_status;
    send_status = zig_ias_status_attr_report( ep, status, dtime, QOS_1 );
}

/**
 * @description: ias zone status change func 
 * @param: {in} ep: endpiont
 * @param: {in} status: ias zone status
 * @return: none
 */
uint8_t ias_zone_server_update_zone_status(uint8_t ep,uint16_t status)
{
    NET_EVT_T nwkstatus = nwk_state_get();
    /* send enroll request only nwk up  */
    if( (nwkstatus == NET_JOIN_OK) || 
        (nwkstatus == NET_REJOIN_OK) ) {

        if ( get_ias_zone_enroll_state(ep) == IAS_ZONE_STATE_NOT_ENROLLED ) {
            zig_ias_enroll_request_with_client();
        }
    }

    ias_zone_status_report( ep, status, 3000 );

    return 0;
}

/**
 * @description: updata ias zone status
 * @param {type} none
 * @return: none
 */
void ias_zone_status_update(void)
{
    // update status only if conditions are met
    if( ias_zone_status_pre_update_callback( ias_zone_status_bits.ias_zone_status ) )
    {
        tuya_print( "ias_zone_status_update %d\r\n",ias_zone_status_bits.ias_zone_status );
        ias_zone_server_update_zone_status( IAS_ZONE_ENDPOINT,
                                            ias_zone_status_bits.ias_zone_status );
    }
}

/**
 * @description: ias zone sensor alarm1 status active
 * @param {type} none
 * @return: none
 */
void alarm_switch_alarm1_active(void)
{
    ias_zone_status_bits.zone_bits.alarm1_status_bit0 = IAS_ZONE_STATUS_BIT_ACTIVE;
    ias_zone_alarm1_pre_change_callback( ias_zone_status_bits.zone_bits.alarm1_status_bit0 );
    ias_zone_status_update();
}

/**
 * @description: ias zone sensor alarm1 status inactive
 * @param {type} none
 * @return: none
 */
void alarm_switch_alarm1_inactive(void)
{
    ias_zone_status_bits.zone_bits.alarm1_status_bit0 = IAS_ZONE_STATUS_BIT_INACTIVE;
    ias_zone_alarm1_pre_change_callback( ias_zone_status_bits.zone_bits.alarm1_status_bit0 );
    ias_zone_status_update();
}


/**
 * @description: ias zone sensor status sync storage
 * @param: {in} status: ias zone status
 * @return: none
 */
void ias_zone_sensor_state_sync_callback(uint8_t status)
{
    // tuya_print("Security new state:  %x\r\n", status);
    if ((status & IAS_ZONE_SENSOR_STATUS_ALARM1) == IAS_ZONE_SENSOR_STATUS_ALARM1) {
        ias_zone_status_bits.ias_zone_status |= IAS_ZONE_SENSOR_STATUS_ALARM1;
    } else {
        ias_zone_status_bits.ias_zone_status &= (~IAS_ZONE_SENSOR_STATUS_ALARM1);
    }
    
    dev_zigbee_write_attribute(
        IAS_ZONE_EP,
        CLUSTER_IAS_ZONE_CLUSTER_ID,
        ATTR_ZONE_STATUS_ATTRIBUTE_ID,
        &ias_zone_status_bits.ias_zone_status,
        ATTR_BITMAP16_ATTRIBUTE_TYPE
    );
}

/**
 * @description: when joned nwk or rejoined nwk will be sync ias zone status 
 * @param {type} none
 * @return: none
 */
void ias_sensor_status_sync(void)
{
    static uint16_t ias_zone_status;

    if( !gpio_raw_input_read_status( alarm_info.sensor_io_st.port,
                                    alarm_info.sensor_io_st.pin) ) {
        if ( !alarm_info.gpio_polarity )
            ias_zone_status |= IAS_ZONE_SENSOR_STATUS_ALARM1;
        else
            ias_zone_status &= (~IAS_ZONE_SENSOR_STATUS_ALARM1);
    }
    else {
        if ( !alarm_info.gpio_polarity )
            ias_zone_status &= (~IAS_ZONE_SENSOR_STATUS_ALARM1);
        else
            ias_zone_status |= IAS_ZONE_SENSOR_STATUS_ALARM1;
    }

    ias_zone_sensor_state_sync_callback( ias_zone_status );
}

/**
 * @description: ias zone gpio status change callback
 * @param {in} gpstatus: gpio status
 * @return: none
 */
VIRTUAL_FUNC void ias_zone_alarm1_gpio_status_change_callback(uint8_t gpstatus)
{
    if( gpstatus==GPIO_SENSOR_INACTIVE ) {
        alarm_switch_alarm1_inactive();
    }
    else {
        alarm_switch_alarm1_active();
    }
}




