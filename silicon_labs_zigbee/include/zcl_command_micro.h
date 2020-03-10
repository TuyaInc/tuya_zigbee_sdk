/*
 * @Author: Leon
 * @email: zhangpeng@tuya.com
 * @LastEditors: Leon
 * @file name: zcl_command_micro.h
 * @Description: zcl client command functions
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2019-05-05 12:14:08
 * @LastEditTime: 2019-05-07 10:35:13
 */

#ifndef __ZCL_COMMAND_MICRO_H__
#define __ZCL_COMMAND_MICRO_H__

#include "zigbee_sdk.h"

typedef enum
{
  /** Unicast sent directly to an EmberNodeId. */
  APP_OUTGOING_DIRECT,
  /** Unicast sent using an entry in the address table. */
  APP_OUTGOING_VIA_ADDRESS_TABLE,
  /** Unicast sent using an entry in the binding table. */
  APP_OUTGOING_VIA_BINDING,
  /** Multicast message.  This value is passed to emberMessageSentHandler() only.
   * It may not be passed to emberSendUnicast(). */
  APP_OUTGOING_MULTICAST,
  /** An aliased multicast message.  This value is passed to emberMessageSentHandler() only.
   * It may not be passed to emberSendUnicast(). */
  APP_OUTGOING_MULTICAST_WITH_ALIAS,
  /** An aliased Broadcast message.  This value is passed to emberMessageSentHandler() only.
   * It may not be passed to emberSendUnicast(). */
  APP_OUTGOING_BROADCAST_WITH_ALIAS,
  /** A broadcast message.  This value is passed to emberMessageSentHandler() only.
   * It may not be passed to emberSendUnicast(). */
  APP_OUTGOING_BROADCAST
}AppOutgoingMessageType;

// This is generated client API
typedef struct _frame_info_t
{
  uint8_t source_endpoint;
  uint8_t dest_endpoint;
  AppOutgoingMessageType type;
  uint16_t dest_address;
} frame_info_t;

extern bool_t send_command(frame_info_t *frame_info );

/** @brief Command description for GetSceneMembership
 *
 * Cluster: Scenes, Attributes and commands for scene configuration and manipulation.
 * Command: GetSceneMembership
 * @param groupId uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandScenesClusterGetSceneMembership(uint16_t groupId,
                                                  frame_info_t *frame_info );
 
/** @brief Command description for AddSceneResponse
 *
 * Cluster: Scenes, Attributes and commands for scene configuration and manipulation.
 * Command: AddSceneResponse
 * @param status uint8_t
 * @param groupId uint16_t
 * @param sceneId uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandScenesClusterAddSceneResponse(uint8_t status,
                                                uint16_t groupId,
                                                uint8_t sceneId,
                                                frame_info_t *frame_info );
 
/** @brief Command description for ViewSceneResponse
 *
 * Cluster: Scenes, Attributes and commands for scene configuration and manipulation.
 * Command: ViewSceneResponse
 * @param status uint8_t
 * @param groupId uint16_t
 * @param sceneId uint8_t
 * @param transitionTime uint16_t
 * @param sceneName uint8_t*
 * @param extensionFieldSets uint8_t*
 * @param extensionFieldSetsLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandScenesClusterViewSceneResponse(uint8_t status,
                                                 uint16_t groupId,
                                                 uint8_t sceneId,
                                                 uint16_t transitionTime,
                                                 uint8_t *sceneName,
                                                 uint8_t *extensionFieldSets,
                                                 uint16_t extensionFieldSetsLen,
                                                 frame_info_t *frame_info );
 
/** @brief Command description for RemoveSceneResponse
 *
 * Cluster: Scenes, Attributes and commands for scene configuration and manipulation.
 * Command: RemoveSceneResponse
 * @param status uint8_t
 * @param groupId uint16_t
 * @param sceneId uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandScenesClusterRemoveSceneResponse(uint8_t status,
                                                   uint16_t groupId,
                                                   uint8_t sceneId,
                                                   frame_info_t *frame_info );
 
/** @brief Command description for RemoveAllScenesResponse
 *
 * Cluster: Scenes, Attributes and commands for scene configuration and manipulation.
 * Command: RemoveAllScenesResponse
 * @param status uint8_t
 * @param groupId uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandScenesClusterRemoveAllScenesResponse(uint8_t status,
                                                       uint16_t groupId,
                                                       frame_info_t *frame_info );
 
/** @brief Command description for StoreSceneResponse
 *
 * Cluster: Scenes, Attributes and commands for scene configuration and manipulation.
 * Command: StoreSceneResponse
 * @param status uint8_t
 * @param groupId uint16_t
 * @param sceneId uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandScenesClusterStoreSceneResponse(uint8_t status,
                                                  uint16_t groupId,
                                                  uint8_t sceneId,
                                                  frame_info_t *frame_info );
 
/** @brief Command description for GetSceneMembershipResponse
 *
 * Cluster: Scenes, Attributes and commands for scene configuration and manipulation.
 * Command: GetSceneMembershipResponse
 * @param status uint8_t
 * @param capacity uint8_t
 * @param groupId uint16_t
 * @param sceneCount uint8_t
 * @param sceneList uint8_t*
 * @param sceneListLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandScenesClusterGetSceneMembershipResponse(uint8_t status,
                                                          uint8_t capacity,
                                                          uint16_t groupId,
                                                          uint8_t sceneCount,
                                                          uint8_t *sceneList,
                                                          uint16_t sceneListLen,
                                                          frame_info_t *frame_info );
 
/** @brief Command description for EnhancedAddScene
 *
 * Cluster: Scenes, Attributes and commands for scene configuration and manipulation.
 * Command: EnhancedAddScene
 * @param groupId uint16_t
 * @param sceneId uint8_t
 * @param transitionTime uint16_t
 * @param sceneName uint8_t*
 * @param extensionFieldSets uint8_t*
 * @param extensionFieldSetsLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandScenesClusterEnhancedAddScene(uint16_t groupId,
                                                uint8_t sceneId,
                                                uint16_t transitionTime,
                                                uint8_t *sceneName,
                                                uint8_t *extensionFieldSets,
                                                uint16_t extensionFieldSetsLen,
                                                frame_info_t *frame_info );
 
/** @brief Command description for EnhancedViewScene
 *
 * Cluster: Scenes, Attributes and commands for scene configuration and manipulation.
 * Command: EnhancedViewScene
 * @param groupId uint16_t
 * @param sceneId uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandScenesClusterEnhancedViewScene(uint16_t groupId,
                                                 uint8_t sceneId,
                                                 frame_info_t *frame_info );
 
/** @brief Command description for CopyScene
 *
 * Cluster: Scenes, Attributes and commands for scene configuration and manipulation.
 * Command: CopyScene
 * @param mode uint8_t
 * @param groupIdFrom uint16_t
 * @param sceneIdFrom uint8_t
 * @param groupIdTo uint16_t
 * @param sceneIdTo uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandScenesClusterCopyScene(uint8_t mode,
                                         uint16_t groupIdFrom,
                                         uint8_t sceneIdFrom,
                                         uint16_t groupIdTo,
                                         uint8_t sceneIdTo,
                                         frame_info_t *frame_info );
 
/** @brief Command description for EnhancedAddSceneResponse
 *
 * Cluster: Scenes, Attributes and commands for scene configuration and manipulation.
 * Command: EnhancedAddSceneResponse
 * @param status uint8_t
 * @param groupId uint16_t
 * @param sceneId uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandScenesClusterEnhancedAddSceneResponse(uint8_t status,
                                                        uint16_t groupId,
                                                        uint8_t sceneId,
                                                        frame_info_t *frame_info );
 
/** @brief Command description for EnhancedViewSceneResponse
 *
 * Cluster: Scenes, Attributes and commands for scene configuration and manipulation.
 * Command: EnhancedViewSceneResponse
 * @param status uint8_t
 * @param groupId uint16_t
 * @param sceneId uint8_t
 * @param transitionTime uint16_t
 * @param sceneName uint8_t*
 * @param extensionFieldSets uint8_t*
 * @param extensionFieldSetsLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandScenesClusterEnhancedViewSceneResponse(uint8_t status,
                                                         uint16_t groupId,
                                                         uint8_t sceneId,
                                                         uint16_t transitionTime,
                                                         uint8_t *sceneName,
                                                         uint8_t *extensionFieldSets,
                                                         uint16_t extensionFieldSetsLen,
                                                         frame_info_t *frame_info );
 
/** @brief Command description for CopySceneResponse
 *
 * Cluster: Scenes, Attributes and commands for scene configuration and manipulation.
 * Command: CopySceneResponse
 * @param status uint8_t
 * @param groupIdFrom uint16_t
 * @param sceneIdFrom uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandScenesClusterCopySceneResponse(uint8_t status,
                                                 uint16_t groupIdFrom,
                                                 uint8_t sceneIdFrom,
                                                 frame_info_t *frame_info );
 
/** @} END Scenes Commands */

/** @name On/off Commands */
 
/** @brief Command description for On
 *
 * Cluster: On/off, Attributes and commands for switching devices between 'On' and 'Off' states.
 * Command: On
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandOnOffClusterOn(frame_info_t *frame_info );
 
/** @brief Command description for Toggle
 *
 * Cluster: On/off, Attributes and commands for switching devices between 'On' and 'Off' states.
 * Command: Toggle
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandOnOffClusterToggle(frame_info_t *frame_info );
 
/** @brief Command description for OffWithEffect
 *
 * Cluster: On/off, Attributes and commands for switching devices between 'On' and 'Off' states.
 * Command: OffWithEffect
 * @param effectId uint8_t
 * @param effectVariant uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandOnOffClusterOffWithEffect(uint8_t effectId,
                                            uint8_t effectVariant,
                                            frame_info_t *frame_info );
 
/** @brief Command description for OnWithRecallGlobalScene
 *
 * Cluster: On/off, Attributes and commands for switching devices between 'On' and 'Off' states.
 * Command: OnWithRecallGlobalScene
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandOnOffClusterOnWithRecallGlobalScene(frame_info_t *frame_info );
 
/** @brief Command description for OnWithTimedOff
 *
 * Cluster: On/off, Attributes and commands for switching devices between 'On' and 'Off' states.
 * Command: OnWithTimedOff
 * @param onOffControl uint8_t
 * @param onTime uint16_t
 * @param offWaitTime uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandOnOffClusterOnWithTimedOff(uint8_t onOffControl,
                                             uint16_t onTime,
                                             uint16_t offWaitTime,
                                             frame_info_t *frame_info );
 
/** @brief Client command that turns the device off with a transition given
        by the transition time in the Ember Sample transition time attribute.
 *
 * Cluster: On/off, Attributes and commands for switching devices between 'On' and 'Off' states.
 * Command: SampleMfgSpecificOffWithTransition
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandOnOffClusterSampleMfgSpecificOffWithTransition(frame_info_t *frame_info );
 
/** @brief Client command that turns the device on with a transition given
        by the transition time in the Ember Sample transition time attribute.
 *
 * Cluster: On/off, Attributes and commands for switching devices between 'On' and 'Off' states.
 * Command: SampleMfgSpecificOnWithTransition
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandOnOffClusterSampleMfgSpecificOnWithTransition(frame_info_t *frame_info );
 
/** @brief Client command that toggles the device with a transition given
        by the transition time in the Ember Sample transition time attribute.
 *
 * Cluster: On/off, Attributes and commands for switching devices between 'On' and 'Off' states.
 * Command: SampleMfgSpecificToggleWithTransition
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandOnOffClusterSampleMfgSpecificToggleWithTransition(frame_info_t *frame_info );
 
/** @} END On/off Commands */

/** @name On/off Switch Configuration Commands */
/** @} END On/off Switch Configuration Commands */

/** @name Level Control Commands */
 
/** @brief Command description for Move
 *
 * Cluster: Level Control, Attributes and commands for controlling devices that can be set to a level between fully 'On' and fully 'Off.'
 * Command: Move
 * @param moveMode uint8_t
 * @param rate uint8_t
 * @param optionMask uint8_t
 * @param optionOverride uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandLevelControlClusterMove(uint8_t moveMode,
                                          uint8_t rate,
                                          uint8_t optionMask,
                                          uint8_t optionOverride,
                                          frame_info_t *frame_info );
 
/** @brief Command description for Step
 *
 * Cluster: Level Control, Attributes and commands for controlling devices that can be set to a level between fully 'On' and fully 'Off.'
 * Command: Step
 * @param stepMode uint8_t
 * @param stepSize uint8_t
 * @param transitionTime uint16_t
 * @param optionMask uint8_t
 * @param optionOverride uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandLevelControlClusterStep(uint8_t stepMode,
                                          uint8_t stepSize,
                                          uint16_t transitionTime,
                                          uint8_t optionMask,
                                          uint8_t optionOverride,
                                          frame_info_t *frame_info );
 
/** @brief Command description for Stop
 *
 * Cluster: Level Control, Attributes and commands for controlling devices that can be set to a level between fully 'On' and fully 'Off.'
 * Command: Stop
 * @param optionMask uint8_t
 * @param optionOverride uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandLevelControlClusterStop(uint8_t optionMask,
                                          uint8_t optionOverride,
                                          frame_info_t *frame_info );
 
/** @brief Command description for MoveToLevelWithOnOff
 *
 * Cluster: Level Control, Attributes and commands for controlling devices that can be set to a level between fully 'On' and fully 'Off.'
 * Command: MoveToLevelWithOnOff
 * @param level uint8_t
 * @param transitionTime uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandLevelControlClusterMoveToLevelWithOnOff(uint8_t level,
                                                          uint16_t transitionTime,
                                                          frame_info_t *frame_info );
 
/** @brief Command description for MoveWithOnOff
 *
 * Cluster: Level Control, Attributes and commands for controlling devices that can be set to a level between fully 'On' and fully 'Off.'
 * Command: MoveWithOnOff
 * @param moveMode uint8_t
 * @param rate uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandLevelControlClusterMoveWithOnOff(uint8_t moveMode,
                                                   uint8_t rate,
                                                   frame_info_t *frame_info );
 
/** @brief Command description for StepWithOnOff
 *
 * Cluster: Level Control, Attributes and commands for controlling devices that can be set to a level between fully 'On' and fully 'Off.'
 * Command: StepWithOnOff
 * @param stepMode uint8_t
 * @param stepSize uint8_t
 * @param transitionTime uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandLevelControlClusterStepWithOnOff(uint8_t stepMode,
                                                   uint8_t stepSize,
                                                   uint16_t transitionTime,
                                                   frame_info_t *frame_info );
 
/** @brief Command description for StopWithOnOff
 *
 * Cluster: Level Control, Attributes and commands for controlling devices that can be set to a level between fully 'On' and fully 'Off.'
 * Command: StopWithOnOff
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandLevelControlClusterStopWithOnOff(frame_info_t *frame_info );
 
/** @} END Level Control Commands */

/** @name Alarms Commands */
 
/** @brief Command description for ResetAllAlarms
 *
 * Cluster: Alarms, Attributes and commands for sending notifications and configuring alarm functionality.
 * Command: ResetAllAlarms
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandAlarmClusterResetAllAlarms(frame_info_t *frame_info );
 
/** @brief Command description for GetAlarm
 *
 * Cluster: Alarms, Attributes and commands for sending notifications and configuring alarm functionality.
 * Command: GetAlarm
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandAlarmClusterGetAlarm(frame_info_t *frame_info );
 
/** @brief Command description for ResetAlarmLog
 *
 * Cluster: Alarms, Attributes and commands for sending notifications and configuring alarm functionality.
 * Command: ResetAlarmLog
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandAlarmClusterResetAlarmLog(frame_info_t *frame_info );
 
/** @brief Command description for Alarm
 *
 * Cluster: Alarms, Attributes and commands for sending notifications and configuring alarm functionality.
 * Command: Alarm
 * @param alarmCode uint8_t
 * @param clusterId uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandAlarmClusterAlarm(uint8_t alarmCode,
                                    uint16_t clusterId,
                                    frame_info_t *frame_info );
 
/** @brief Command description for GetAlarmResponse
 *
 * Cluster: Alarms, Attributes and commands for sending notifications and configuring alarm functionality.
 * Command: GetAlarmResponse
 * @param status uint8_t
 * @param alarmCode uint8_t
 * @param clusterId uint16_t
 * @param timeStamp uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandAlarmClusterGetAlarmResponse(uint8_t status,
                                               uint8_t alarmCode,
                                               uint16_t clusterId,
                                               uint32_t timeStamp,
                                               frame_info_t *frame_info );
 
/** @} END Alarms Commands */

/** @name Time Commands */
/** @} END Time Commands */

/** @name RSSI Location Commands */
 
/** @brief Command description for SetDeviceConfiguration
 *
 * Cluster: RSSI Location, Attributes and commands that provide a means for exchanging location information and channel parameters among devices.
 * Command: SetDeviceConfiguration
 * @param power int16_t
 * @param pathLossExponent uint16_t
 * @param calculationPeriod uint16_t
 * @param numberRssiMeasurements uint8_t
 * @param reportingPeriod uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandRssiLocationClusterSetDeviceConfiguration(int16_t power,
                                                            uint16_t pathLossExponent,
                                                            uint16_t calculationPeriod,
                                                            uint8_t numberRssiMeasurements,
                                                            uint16_t reportingPeriod,
                                                            frame_info_t *frame_info );
 
/** @brief Command description for GetDeviceConfiguration
 *
 * Cluster: RSSI Location, Attributes and commands that provide a means for exchanging location information and channel parameters among devices.
 * Command: GetDeviceConfiguration
 * @param targetAddress uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandRssiLocationClusterGetDeviceConfiguration(uint8_t *targetAddress,
                                                            frame_info_t *frame_info );
 
/** @brief Command description for GetLocationData
 *
 * Cluster: RSSI Location, Attributes and commands that provide a means for exchanging location information and channel parameters among devices.
 * Command: GetLocationData
 * @param flags uint8_t
 * @param numberResponses uint8_t
 * @param targetAddress uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandRssiLocationClusterGetLocationData(uint8_t flags,
                                                     uint8_t numberResponses,
                                                     uint8_t *targetAddress,
                                                     frame_info_t *frame_info );
 
/** @brief Command description for RssiResponse
 *
 * Cluster: RSSI Location, Attributes and commands that provide a means for exchanging location information and channel parameters among devices.
 * Command: RssiResponse
 * @param replyingDevice uint8_t*
 * @param coordinate1 int16_t
 * @param coordinate2 int16_t
 * @param coordinate3 int16_t
 * @param rssi int8_t
 * @param numberRssiMeasurements uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandRssiLocationClusterRssiResponse(uint8_t *replyingDevice,
                                                  int16_t coordinate1,
                                                  int16_t coordinate2,
                                                  int16_t coordinate3,
                                                  int8_t rssi,
                                                  uint8_t numberRssiMeasurements,
                                                  frame_info_t *frame_info );
 
/** @brief Command description for SendPings
 *
 * Cluster: RSSI Location, Attributes and commands that provide a means for exchanging location information and channel parameters among devices.
 * Command: SendPings
 * @param targetAddress uint8_t*
 * @param numberRssiMeasurements uint8_t
 * @param calculationPeriod uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandRssiLocationClusterSendPings(uint8_t *targetAddress,
                                               uint8_t numberRssiMeasurements,
                                               uint16_t calculationPeriod,
                                               frame_info_t *frame_info );
 
/** @brief Command description for AnchorNodeAnnounce
 *
 * Cluster: RSSI Location, Attributes and commands that provide a means for exchanging location information and channel parameters among devices.
 * Command: AnchorNodeAnnounce
 * @param anchorNodeIeeeAddress uint8_t*
 * @param coordinate1 int16_t
 * @param coordinate2 int16_t
 * @param coordinate3 int16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandRssiLocationClusterAnchorNodeAnnounce(uint8_t *anchorNodeIeeeAddress,
                                                        int16_t coordinate1,
                                                        int16_t coordinate2,
                                                        int16_t coordinate3,
                                                        frame_info_t *frame_info );
 
/** @brief Command description for DeviceConfigurationResponse
 *
 * Cluster: RSSI Location, Attributes and commands that provide a means for exchanging location information and channel parameters among devices.
 * Command: DeviceConfigurationResponse
 * @param status uint8_t
 * @param power int16_t
 * @param pathLossExponent uint16_t
 * @param calculationPeriod uint16_t
 * @param numberRssiMeasurements uint8_t
 * @param reportingPeriod uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandRssiLocationClusterDeviceConfigurationResponse(uint8_t status,
                                                                 int16_t power,
                                                                 uint16_t pathLossExponent,
                                                                 uint16_t calculationPeriod,
                                                                 uint16_t numberRssiMeasurements,
                                                                 uint16_t reportingPeriod,
                                                                 frame_info_t *frame_info );
 
/** @brief Command description for LocationDataResponse
 *
 * Cluster: RSSI Location, Attributes and commands that provide a means for exchanging location information and channel parameters among devices.
 * Command: LocationDataResponse
 * @param status uint8_t
 * @param locationType uint8_t
 * @param coordinate1 int16_t
 * @param coordinate2 int16_t
 * @param coordinate3 int16_t
 * @param power int16_t
 * @param pathLossExponent uint16_t
 * @param locationMethod uint8_t
 * @param qualityMeasure uint8_t
 * @param locationAge uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandRssiLocationClusterLocationDataResponse(uint8_t status,
                                                          uint8_t locationType,
                                                          int16_t coordinate1,
                                                          int16_t coordinate2,
                                                          int16_t coordinate3,
                                                          int16_t power,
                                                          uint16_t pathLossExponent,
                                                          uint8_t locationMethod,
                                                          uint8_t qualityMeasure,
                                                          uint16_t locationAge,
                                                          frame_info_t *frame_info );
 
/** @brief Command description for LocationDataNotification
 *
 * Cluster: RSSI Location, Attributes and commands that provide a means for exchanging location information and channel parameters among devices.
 * Command: LocationDataNotification
 * @param locationType uint8_t
 * @param coordinate1 int16_t
 * @param coordinate2 int16_t
 * @param coordinate3 int16_t
 * @param power int16_t
 * @param pathLossExponent uint16_t
 * @param locationMethod uint8_t
 * @param qualityMeasure uint8_t
 * @param locationAge uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandRssiLocationClusterLocationDataNotification(uint8_t locationType,
                                                              int16_t coordinate1,
                                                              int16_t coordinate2,
                                                              int16_t coordinate3,
                                                              int16_t power,
                                                              uint16_t pathLossExponent,
                                                              uint8_t locationMethod,
                                                              uint8_t qualityMeasure,
                                                              uint16_t locationAge,
                                                              frame_info_t *frame_info );
 
/** @brief Command description for CompactLocationDataNotification
 *
 * Cluster: RSSI Location, Attributes and commands that provide a means for exchanging location information and channel parameters among devices.
 * Command: CompactLocationDataNotification
 * @param locationType uint8_t
 * @param coordinate1 int16_t
 * @param coordinate2 int16_t
 * @param coordinate3 int16_t
 * @param qualityMeasure uint8_t
 * @param locationAge uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandRssiLocationClusterCompactLocationDataNotification(uint8_t locationType,
                                                                     int16_t coordinate1,
                                                                     int16_t coordinate2,
                                                                     int16_t coordinate3,
                                                                     uint8_t qualityMeasure,
                                                                     uint16_t locationAge,
                                                                     frame_info_t *frame_info );
 
/** @brief Command description for RssiPing
 *
 * Cluster: RSSI Location, Attributes and commands that provide a means for exchanging location information and channel parameters among devices.
 * Command: RssiPing
 * @param locationType uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandRssiLocationClusterRssiPing(uint8_t locationType,
                                              frame_info_t *frame_info );
 
/** @brief Command description for RssiRequest
 *
 * Cluster: RSSI Location, Attributes and commands that provide a means for exchanging location information and channel parameters among devices.
 * Command: RssiRequest
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandRssiLocationClusterRssiRequest(frame_info_t *frame_info );
 
/** @brief Command description for ReportRssiMeasurements
 *
 * Cluster: RSSI Location, Attributes and commands that provide a means for exchanging location information and channel parameters among devices.
 * Command: ReportRssiMeasurements
 * @param measuringDevice uint8_t*
 * @param neighbors uint8_t
 * @param neighborsInfo uint8_t*
 * @param neighborsInfoLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandRssiLocationClusterReportRssiMeasurements(uint8_t *measuringDevice,
                                                            uint8_t neighbors,
                                                            uint8_t *neighborsInfo,
                                                            uint16_t neighborsInfoLen,
                                                            frame_info_t *frame_info );
 
/** @brief Command description for RequestOwnLocation
 *
 * Cluster: RSSI Location, Attributes and commands that provide a means for exchanging location information and channel parameters among devices.
 * Command: RequestOwnLocation
 * @param blindNode uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandRssiLocationClusterRequestOwnLocation(uint8_t *blindNode,
                                                        frame_info_t *frame_info );
 
/** @} END RSSI Location Commands */

/** @name Binary Input (Basic ); Commands */
/** @} END Binary Input (Basic ); Commands */

/** @name Commissioning Commands */
 
/** @brief Command description for SaveStartupParameters
 *
 * Cluster: Commissioning, Attributes and commands for commissioning and managing a ZigBee device.
 * Command: SaveStartupParameters
 * @param options uint8_t
 * @param index uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandCommissioningClusterSaveStartupParameters(uint8_t options,
                                                            uint8_t index,
                                                            frame_info_t *frame_info );
 
/** @brief Command description for RestoreStartupParameters
 *
 * Cluster: Commissioning, Attributes and commands for commissioning and managing a ZigBee device.
 * Command: RestoreStartupParameters
 * @param options uint8_t
 * @param index uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandCommissioningClusterRestoreStartupParameters(uint8_t options,
                                                               uint8_t index,
                                                               frame_info_t *frame_info );
 
/** @brief Command description for ResetStartupParameters
 *
 * Cluster: Commissioning, Attributes and commands for commissioning and managing a ZigBee device.
 * Command: ResetStartupParameters
 * @param options uint8_t
 * @param index uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandCommissioningClusterResetStartupParameters(uint8_t options,
                                                             uint8_t index,
                                                             frame_info_t *frame_info );
 
/** @brief Command description for RestartDeviceResponse
 *
 * Cluster: Commissioning, Attributes and commands for commissioning and managing a ZigBee device.
 * Command: RestartDeviceResponse
 * @param status uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandCommissioningClusterRestartDeviceResponse(uint8_t status,
                                                            frame_info_t *frame_info );
 
/** @brief Command description for SaveStartupParametersResponse
 *
 * Cluster: Commissioning, Attributes and commands for commissioning and managing a ZigBee device.
 * Command: SaveStartupParametersResponse
 * @param status uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandCommissioningClusterSaveStartupParametersResponse(uint8_t status,
                                                                    frame_info_t *frame_info );
 
/** @brief Command description for RestoreStartupParametersResponse
 *
 * Cluster: Commissioning, Attributes and commands for commissioning and managing a ZigBee device.
 * Command: RestoreStartupParametersResponse
 * @param status uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandCommissioningClusterRestoreStartupParametersResponse(uint8_t status,
                                                                       frame_info_t *frame_info );
 
/** @brief Command description for ResetStartupParametersResponse
 *
 * Cluster: Commissioning, Attributes and commands for commissioning and managing a ZigBee device.
 * Command: ResetStartupParametersResponse
 * @param status uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandCommissioningClusterResetStartupParametersResponse(uint8_t status,
                                                                     frame_info_t *frame_info );
 
/** @} END Commissioning Commands */

/** @name Partition Commands */
 
/** @brief The ReadHandshakeParam command is used in order to read the appropriate set of parameters for each transaction to be performed by the Partition Cluster.
 *
 * Cluster: Partition, Commands and attributes for enabling partitioning of large frame to be carried from other clusters of ZigBee devices.
 * Command: ReadHandshakeParam
 * @param partitionedClusterId uint16_t
 * @param attributeList uint8_t*
 * @param attributeListLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPartitionClusterReadHandshakeParam(uint16_t partitionedClusterId,
                                                     uint8_t *attributeList,
                                                     uint16_t attributeListLen,
                                                     frame_info_t *frame_info );
 
/** @brief The WriteHandshakeParam command is used during the handshake phase in order to write the appropriate parameters for each transaction to be performed by the Partition Cluster.
 *
 * Cluster: Partition, Commands and attributes for enabling partitioning of large frame to be carried from other clusters of ZigBee devices.
 * Command: WriteHandshakeParam
 * @param partitionedClusterId uint16_t
 * @param writeAttributeRecords uint8_t*
 * @param writeAttributeRecordsLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPartitionClusterWriteHandshakeParam(uint16_t partitionedClusterId,
                                                      uint8_t *writeAttributeRecords,
                                                      uint16_t writeAttributeRecordsLen,
                                                      frame_info_t *frame_info );
 
/** @brief MultipleACK command.
 *
 * Cluster: Partition, Commands and attributes for enabling partitioning of large frame to be carried from other clusters of ZigBee devices.
 * Command: MultipleAck
 * @param ackOptions uint8_t
 * @param firstFrameIdAndNackList uint8_t*
 * @param firstFrameIdAndNackListLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPartitionClusterMultipleAck(uint8_t ackOptions,
                                              uint8_t *firstFrameIdAndNackList,
                                              uint16_t firstFrameIdAndNackListLen,
                                              frame_info_t *frame_info );
 
/** @brief The ReadHandshakeParamResponse command is used in order to response to the corresponding ReadHandshakeParam command in order to communicate the appropriate set of parameters configured for each transaction to be performed by the Partition Cluster.
 *
 * Cluster: Partition, Commands and attributes for enabling partitioning of large frame to be carried from other clusters of ZigBee devices.
 * Command: ReadHandshakeParamResponse
 * @param partitionedClusterId uint16_t
 * @param readAttributeStatusRecords uint8_t*
 * @param readAttributeStatusRecordsLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPartitionClusterReadHandshakeParamResponse(uint16_t partitionedClusterId,
                                                             uint8_t *readAttributeStatusRecords,
                                                             uint16_t readAttributeStatusRecordsLen,
                                                             frame_info_t *frame_info );
 
/** @} END Partition Commands */

/** @name Over the Air Bootloading Commands */
 
/** @brief This command is generated upon receipt of an Image Notify command to indicate that the client is looking for the next firmware image to upgrade to.  The client may also choose to send the command periodically to the server.
 *
 * Cluster: Over the Air Bootloading, This cluster contains commands and attributes that act as an interface for ZigBee Over-the-air bootloading.
 * Command: QueryNextImageRequest
 * @param fieldControl uint8_t
 * @param manufacturerId uint16_t
 * @param imageType uint16_t
 * @param currentFileVersion uint32_t
 * @param hardwareVersion uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandOtaBootloadClusterQueryNextImageRequest(uint8_t fieldControl,
                                                          uint16_t manufacturerId,
                                                          uint16_t imageType,
                                                          uint32_t currentFileVersion,
                                                          uint16_t hardwareVersion,
                                                          frame_info_t *frame_info );
 
/** @brief This command is generated upon receipt of an QueryNextImageRequest command to response whether the server has a valid OTA upgrade image for the client or not.  If the server has the file, information regarding the file and OTA upgrade process will be included in the command.
 *
 * Cluster: Over the Air Bootloading, This cluster contains commands and attributes that act as an interface for ZigBee Over-the-air bootloading.
 * Command: QueryNextImageResponse
 * @param status uint8_t
 * @param manufacturerId uint16_t
 * @param imageType uint16_t
 * @param fileVersion uint32_t
 * @param imageSize uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandOtaBootloadClusterQueryNextImageResponse(uint8_t status,
                                                           uint16_t manufacturerId,
                                                           uint16_t imageType,
                                                           uint32_t fileVersion,
                                                           uint32_t imageSize,
                                                           frame_info_t *frame_info );
 
/** @brief This command is generated by the client to request blocks of OTA upgrade file data.
 *
 * Cluster: Over the Air Bootloading, This cluster contains commands and attributes that act as an interface for ZigBee Over-the-air bootloading.
 * Command: ImageBlockRequest
 * @param fieldControl uint8_t
 * @param manufacturerId uint16_t
 * @param imageType uint16_t
 * @param fileVersion uint32_t
 * @param fileOffset uint32_t
 * @param maxDataSize uint8_t
 * @param requestNodeAddress uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandOtaBootloadClusterImageBlockRequest(uint8_t fieldControl,
                                                      uint16_t manufacturerId,
                                                      uint16_t imageType,
                                                      uint32_t fileVersion,
                                                      uint32_t fileOffset,
                                                      uint8_t maxDataSize,
                                                      uint8_t *requestNodeAddress,
                                                      frame_info_t *frame_info );
 
/** @brief This command is generated by the client to request pages of OTA upgrade file data. A page would contain multiple blocks of data.
 *
 * Cluster: Over the Air Bootloading, This cluster contains commands and attributes that act as an interface for ZigBee Over-the-air bootloading.
 * Command: ImagePageRequest
 * @param fieldControl uint8_t
 * @param manufacturerId uint16_t
 * @param imageType uint16_t
 * @param fileVersion uint32_t
 * @param fileOffset uint32_t
 * @param maxDataSize uint8_t
 * @param pageSize uint16_t
 * @param responseSpacing uint16_t
 * @param requestNodeAddress uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandOtaBootloadClusterImagePageRequest(uint8_t fieldControl,
                                                     uint16_t manufacturerId,
                                                     uint16_t imageType,
                                                     uint32_t fileVersion,
                                                     uint32_t fileOffset,
                                                     uint8_t maxDataSize,
                                                     uint16_t pageSize,
                                                     uint16_t responseSpacing,
                                                     uint8_t *requestNodeAddress,
                                                     frame_info_t *frame_info );
 
/** @brief This command is generated by the server in response to the block or page request command.  If the server has the data available, it will reply back with a SUCCESS status.  For other error cases, it may reply with status WAIT_FOR_DATA (server does not have the data available yet ); or ABORT (invalid requested parameters or other failure cases );.
 *
 * Cluster: Over the Air Bootloading, This cluster contains commands and attributes that act as an interface for ZigBee Over-the-air bootloading.
 * Command: ImageBlockResponse
 * @param status uint8_t
 * @param manufacturerId uint16_t
 * @param imageType uint16_t
 * @param fileVersion uint32_t
 * @param fileOffset uint32_t
 * @param dataSize uint8_t
 * @param imageData uint8_t*
 * @param imageDataLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandOtaBootloadClusterImageBlockResponse(uint8_t status,
                                                       uint16_t manufacturerId,
                                                       uint16_t imageType,
                                                       uint32_t fileVersion,
                                                       uint32_t fileOffset,
                                                       uint8_t dataSize,
                                                       uint8_t *imageData,
                                                       uint16_t imageDataLen,
                                                       frame_info_t *frame_info );
 
/** @brief This command is generated by the client to notify the server of the end of the upgrade process.  The process may end with success or error status.
 *
 * Cluster: Over the Air Bootloading, This cluster contains commands and attributes that act as an interface for ZigBee Over-the-air bootloading.
 * Command: UpgradeEndRequest
 * @param status uint8_t
 * @param manufacturerId uint16_t
 * @param imageType uint16_t
 * @param fileVersion uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandOtaBootloadClusterUpgradeEndRequest(uint8_t status,
                                                      uint16_t manufacturerId,
                                                      uint16_t imageType,
                                                      uint32_t fileVersion,
                                                      frame_info_t *frame_info );
 
/** @brief This command is generated by the server in response to the upgrade request in order to let the client know when to upgrade to running new firmware image.
 *
 * Cluster: Over the Air Bootloading, This cluster contains commands and attributes that act as an interface for ZigBee Over-the-air bootloading.
 * Command: UpgradeEndResponse
 * @param manufacturerId uint16_t
 * @param imageType uint16_t
 * @param fileVersion uint32_t
 * @param currentTime uint32_t
 * @param upgradeTime uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandOtaBootloadClusterUpgradeEndResponse(uint16_t manufacturerId,
                                                       uint16_t imageType,
                                                       uint32_t fileVersion,
                                                       uint32_t currentTime,
                                                       uint32_t upgradeTime,
                                                       frame_info_t *frame_info );
 
/** @brief This command is generated by the client to request a file that is specific to itself.  The intention is to provide a way for the client to request non-OTA upgrade file.
 *
 * Cluster: Over the Air Bootloading, This cluster contains commands and attributes that act as an interface for ZigBee Over-the-air bootloading.
 * Command: QuerySpecificFileRequest
 * @param requestNodeAddress uint8_t*
 * @param manufacturerId uint16_t
 * @param imageType uint16_t
 * @param fileVersion uint32_t
 * @param currentZigbeeStackVersion uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandOtaBootloadClusterQuerySpecificFileRequest(uint8_t *requestNodeAddress,
                                                             uint16_t manufacturerId,
                                                             uint16_t imageType,
                                                             uint32_t fileVersion,
                                                             uint16_t currentZigbeeStackVersion,
                                                             frame_info_t *frame_info );
 
/** @brief This command is generated upon receipt of an QuerySpecificFileRequest command to response whether the server has a valid file for the client or not.  If the server has the file, information regarding the file and OTA process will be included in the command.
 *
 * Cluster: Over the Air Bootloading, This cluster contains commands and attributes that act as an interface for ZigBee Over-the-air bootloading.
 * Command: QuerySpecificFileResponse
 * @param status uint8_t
 * @param manufacturerId uint16_t
 * @param imageType uint16_t
 * @param fileVersion uint32_t
 * @param imageSize uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandOtaBootloadClusterQuerySpecificFileResponse(uint8_t status,
                                                              uint16_t manufacturerId,
                                                              uint16_t imageType,
                                                              uint32_t fileVersion,
                                                              uint32_t imageSize,
                                                              frame_info_t *frame_info );
 
/** @} END Over the Air Bootloading Commands */

/** @name Power Profile Commands */
 
/** @brief The PowerProfileStateRequest Command is generated in order to retrieve the identifiers of current Power Profiles.
 *
 * Cluster: Power Profile, This cluster provides an interface for transferring power profile information from a device (e.g. Whitegood ); to a controller (e.g. the Home Gateway );.  The Power Profile transferred can be solicited by client side (request command ); or can be notified directly from the device (server side );.
 * Command: PowerProfileStateRequest
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPowerProfileClusterPowerProfileStateRequest(
    frame_info_t *frame_info );
 
/** @brief The GetPowerProfilePriceResponse command allows a device (client ); to communicate the cost associated to the selected Power Profile to another device (server ); requesting it.
 *
 * Cluster: Power Profile, This cluster provides an interface for transferring power profile information from a device (e.g. Whitegood ); to a controller (e.g. the Home Gateway );.  The Power Profile transferred can be solicited by client side (request command ); or can be notified directly from the device (server side );.
 * Command: GetPowerProfilePriceResponse
 * @param powerProfileId uint8_t
 * @param currency uint16_t
 * @param price uint32_t
 * @param priceTrailingDigit uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPowerProfileClusterGetPowerProfilePriceResponse(uint8_t powerProfileId,
                                                                  uint16_t currency,
                                                                  uint32_t price,
                                                                  uint8_t priceTrailingDigit,
                                                                  frame_info_t *frame_info );
 
/** @brief The GetOverallSchedulePriceResponse command allows a device (client ); to communicate the overall cost associated to all Power Profiles scheduled to another device (server ); requesting it.
 *
 * Cluster: Power Profile, This cluster provides an interface for transferring power profile information from a device (e.g. Whitegood ); to a controller (e.g. the Home Gateway );.  The Power Profile transferred can be solicited by client side (request command ); or can be notified directly from the device (server side );.
 * Command: GetOverallSchedulePriceResponse
 * @param currency uint16_t
 * @param price uint32_t
 * @param priceTrailingDigit uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPowerProfileClusterGetOverallSchedulePriceResponse(uint16_t currency,
                                                                     uint32_t price,
                                                                     uint8_t priceTrailingDigit,
                                                                     frame_info_t *frame_info );
 
/** @brief The EnergyPhasesScheduleNotification Command is generated by a device supporting the client side of the Power Profile cluster in order to schedule the start of the selected Power Profile and its phases.
 *
 * Cluster: Power Profile, This cluster provides an interface for transferring power profile information from a device (e.g. Whitegood ); to a controller (e.g. the Home Gateway );.  The Power Profile transferred can be solicited by client side (request command ); or can be notified directly from the device (server side );.
 * Command: EnergyPhasesScheduleNotification
 * @param powerProfileId uint8_t
 * @param numOfScheduledPhases uint8_t
 * @param scheduledPhases uint8_t*
 * @param scheduledPhasesLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPowerProfileClusterEnergyPhasesScheduleNotification(uint8_t powerProfileId,
                                                                      uint8_t numOfScheduledPhases,
                                                                      uint8_t *scheduledPhases,
                                                                      uint16_t scheduledPhasesLen,
                                                                      frame_info_t *frame_info );
 
/** @brief This command is generated by the client side of Power Profile cluster as a reply to the EnergyPhasesScheduleRequest command.
 *
 * Cluster: Power Profile, This cluster provides an interface for transferring power profile information from a device (e.g. Whitegood ); to a controller (e.g. the Home Gateway );.  The Power Profile transferred can be solicited by client side (request command ); or can be notified directly from the device (server side );.
 * Command: EnergyPhasesScheduleResponse
 * @param powerProfileId uint8_t
 * @param numOfScheduledPhases uint8_t
 * @param scheduledPhases uint8_t*
 * @param scheduledPhasesLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPowerProfileClusterEnergyPhasesScheduleResponse(uint8_t powerProfileId,
                                                                  uint8_t numOfScheduledPhases,
                                                                  uint8_t *scheduledPhases,
                                                                  uint16_t scheduledPhasesLen,
                                                                  frame_info_t *frame_info );
 
/** @brief The PowerProfileScheduleConstraintsRequest Command is generated by a device supporting the client side of the Power Profile cluster in order to request the constraints -if set- of Power Profile of a client device, in order to set the proper boundaries for the scheduling when calculating the schedules.
 *
 * Cluster: Power Profile, This cluster provides an interface for transferring power profile information from a device (e.g. Whitegood ); to a controller (e.g. the Home Gateway );.  The Power Profile transferred can be solicited by client side (request command ); or can be notified directly from the device (server side );.
 * Command: PowerProfileScheduleConstraintsRequest
 * @param powerProfileId uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPowerProfileClusterPowerProfileScheduleConstraintsRequest(uint8_t powerProfileId,
                                                                            frame_info_t *frame_info );
 
/** @brief The EnergyPhasesScheduleStateRequest  Command is generated by a device supporting the client side of the Power Profile cluster to check the states of the scheduling of a power profile, which is supported in the device implementing the server side of Power Profile cluster.
 *
 * Cluster: Power Profile, This cluster provides an interface for transferring power profile information from a device (e.g. Whitegood ); to a controller (e.g. the Home Gateway );.  The Power Profile transferred can be solicited by client side (request command ); or can be notified directly from the device (server side );.
 * Command: EnergyPhasesScheduleStateRequest
 * @param powerProfileId uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPowerProfileClusterEnergyPhasesScheduleStateRequest(uint8_t powerProfileId,
                                                                      frame_info_t *frame_info );
 
/** @brief The Get Power Profile Price Extended Response command allows a device (client ); to communicate the cost associated to all Power Profiles scheduled to another device (server ); requesting it according to the specific options contained in the Get Power Profile Price Extended Response.
 *
 * Cluster: Power Profile, This cluster provides an interface for transferring power profile information from a device (e.g. Whitegood ); to a controller (e.g. the Home Gateway );.  The Power Profile transferred can be solicited by client side (request command ); or can be notified directly from the device (server side );.
 * Command: GetPowerProfilePriceExtendedResponse
 * @param powerProfileId uint8_t
 * @param currency uint16_t
 * @param price uint32_t
 * @param priceTrailingDigit uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPowerProfileClusterGetPowerProfilePriceExtendedResponse(uint8_t powerProfileId,
                                                                          uint16_t currency,
                                                                          uint32_t price,
                                                                          uint8_t priceTrailingDigit,
                                                                          frame_info_t *frame_info );
 
/** @brief The PowerProfileNotification Command is generated by a device supporting the server side of the Power Profile cluster in order to send the information of the specific parameters (such as Peak power and others ); belonging to each phase.
 *
 * Cluster: Power Profile, This cluster provides an interface for transferring power profile information from a device (e.g. Whitegood ); to a controller (e.g. the Home Gateway );.  The Power Profile transferred can be solicited by client side (request command ); or can be notified directly from the device (server side );.
 * Command: PowerProfileNotification
 * @param totalProfileNum uint8_t
 * @param powerProfileId uint8_t
 * @param numOfTransferredPhases uint8_t
 * @param transferredPhases uint8_t*
 * @param transferredPhasesLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPowerProfileClusterPowerProfileNotification(uint8_t totalProfileNum,
                                                              uint8_t powerProfileId,
                                                              uint8_t numOfTransferredPhases,
                                                              uint8_t *transferredPhases,
                                                              uint16_t transferredPhasesLen,
                                                              frame_info_t *frame_info );
 
/** @brief This command is generated by the server side of Power Profile cluster as a reply to the PowerProfileRequest command.
 *
 * Cluster: Power Profile, This cluster provides an interface for transferring power profile information from a device (e.g. Whitegood ); to a controller (e.g. the Home Gateway );.  The Power Profile transferred can be solicited by client side (request command ); or can be notified directly from the device (server side );.
 * Command: PowerProfileResponse
 * @param totalProfileNum uint8_t
 * @param powerProfileId uint8_t
 * @param numOfTransferredPhases uint8_t
 * @param transferredPhases uint8_t*
 * @param transferredPhasesLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPowerProfileClusterPowerProfileResponse(uint8_t totalProfileNum,
                                                          uint8_t powerProfileId,
                                                          uint8_t numOfTransferredPhases,
                                                          uint8_t *transferredPhases,
                                                          uint16_t transferredPhasesLen,
                                                          frame_info_t *frame_info );
 
/** @brief The PowerProfileStateResponse command allows a device (server ); to communicate its current Power Profile(s ); to another device (client ); that previously requested them.
 *
 * Cluster: Power Profile, This cluster provides an interface for transferring power profile information from a device (e.g. Whitegood ); to a controller (e.g. the Home Gateway );.  The Power Profile transferred can be solicited by client side (request command ); or can be notified directly from the device (server side );.
 * Command: PowerProfileStateResponse
 * @param powerProfileCount uint8_t
 * @param powerProfileRecords uint8_t*
 * @param powerProfileRecordsLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPowerProfileClusterPowerProfileStateResponse(uint8_t powerProfileCount,
                                                               uint8_t *powerProfileRecords,
                                                               uint16_t powerProfileRecordsLen,
                                                               frame_info_t *frame_info );
 
/** @brief The GetPowerProfilePrice Command is generated by the server (e.g. White goods ); in order to retrieve the cost associated to a specific Power profile.
 *
 * Cluster: Power Profile, This cluster provides an interface for transferring power profile information from a device (e.g. Whitegood ); to a controller (e.g. the Home Gateway );.  The Power Profile transferred can be solicited by client side (request command ); or can be notified directly from the device (server side );.
 * Command: GetPowerProfilePrice
 * @param powerProfileId uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPowerProfileClusterGetPowerProfilePrice(uint8_t powerProfileId,
                                                          frame_info_t *frame_info );
 
/** @brief The PowerProfileStateNotification Command is generated by the server (e.g. White goods ); in order to update the state of the power profile and the current energy phase.
 *
 * Cluster: Power Profile, This cluster provides an interface for transferring power profile information from a device (e.g. Whitegood ); to a controller (e.g. the Home Gateway );.  The Power Profile transferred can be solicited by client side (request command ); or can be notified directly from the device (server side );.
 * Command: PowerProfilesStateNotification
 * @param powerProfileCount uint8_t
 * @param powerProfileRecords uint8_t*
 * @param powerProfileRecordsLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPowerProfileClusterPowerProfilesStateNotification(uint8_t powerProfileCount,
                                                                    uint8_t *powerProfileRecords,
                                                                    uint16_t powerProfileRecordsLen,
                                                                    frame_info_t *frame_info );
 
/** @brief The GetOverallSchedulePrice Command is generated by the server (e.g. White goods ); in order to retrieve the overall cost associated to all the Power Profiles scheduled by the scheduler (the device supporting the Power Profile cluster client side ); for the next 24 hours.
 *
 * Cluster: Power Profile, This cluster provides an interface for transferring power profile information from a device (e.g. Whitegood ); to a controller (e.g. the Home Gateway );.  The Power Profile transferred can be solicited by client side (request command ); or can be notified directly from the device (server side );.
 * Command: GetOverallSchedulePrice
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPowerProfileClusterGetOverallSchedulePrice(frame_info_t *frame_info );
 
/** @brief The EnergyPhasesScheduleRequest Command is generated by the server (e.g. White goods ); in order to retrieve from the scheduler (e.g. Home Gateway ); the schedule (if available ); associated to the specific Power Profile carried in the payload.
 *
 * Cluster: Power Profile, This cluster provides an interface for transferring power profile information from a device (e.g. Whitegood ); to a controller (e.g. the Home Gateway );.  The Power Profile transferred can be solicited by client side (request command ); or can be notified directly from the device (server side );.
 * Command: EnergyPhasesScheduleRequest
 * @param powerProfileId uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPowerProfileClusterEnergyPhasesScheduleRequest(uint8_t powerProfileId,
                                                                 frame_info_t *frame_info );
 
/** @brief The EnergyPhasesScheduleStateResponse Command is generated by the server (e.g. White goods ); in order to reply to a EnergyPhasesScheduleStateRequest command about the scheduling states that are set in the server side.
 *
 * Cluster: Power Profile, This cluster provides an interface for transferring power profile information from a device (e.g. Whitegood ); to a controller (e.g. the Home Gateway );.  The Power Profile transferred can be solicited by client side (request command ); or can be notified directly from the device (server side );.
 * Command: EnergyPhasesScheduleStateResponse
 * @param powerProfileId uint8_t
 * @param numOfScheduledPhases uint8_t
 * @param scheduledPhases uint8_t*
 * @param scheduledPhasesLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPowerProfileClusterEnergyPhasesScheduleStateResponse(uint8_t powerProfileId,
                                                                       uint8_t numOfScheduledPhases,
                                                                       uint8_t *scheduledPhases,
                                                                       uint16_t scheduledPhasesLen,
                                                                       frame_info_t *frame_info );
 
/** @brief The EnergyPhasesScheduleStateNotification Command is generated by the server (e.g. White goods ); in order to notify (un-solicited command ); a client side about the scheduling states that are set in the server side.
 *
 * Cluster: Power Profile, This cluster provides an interface for transferring power profile information from a device (e.g. Whitegood ); to a controller (e.g. the Home Gateway );.  The Power Profile transferred can be solicited by client side (request command ); or can be notified directly from the device (server side );.
 * Command: EnergyPhasesScheduleStateNotification
 * @param powerProfileId uint8_t
 * @param numOfScheduledPhases uint8_t
 * @param scheduledPhases uint8_t*
 * @param scheduledPhasesLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPowerProfileClusterEnergyPhasesScheduleStateNotification(uint8_t powerProfileId,
                                                                           uint8_t numOfScheduledPhases,
                                                                           uint8_t *scheduledPhases,
                                                                           uint16_t scheduledPhasesLen,
                                                                           frame_info_t *frame_info );
 
/** @brief The PowerProfileScheduleConstraintsNotification Command is generated by a device supporting the server side of the Power Profile cluster to notify the client side of this cluster about the imposed constraints and let the scheduler (i.e. the entity supporting the Power Profile cluster client side ); to set the proper boundaries for the scheduling.
 *
 * Cluster: Power Profile, This cluster provides an interface for transferring power profile information from a device (e.g. Whitegood ); to a controller (e.g. the Home Gateway );.  The Power Profile transferred can be solicited by client side (request command ); or can be notified directly from the device (server side );.
 * Command: PowerProfileScheduleConstraintsNotification
 * @param powerProfileId uint8_t
 * @param startAfter uint16_t
 * @param stopBefore uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPowerProfileClusterPowerProfileScheduleConstraintsNotification(uint8_t powerProfileId,
                                                                                 uint16_t startAfter,
                                                                                 uint16_t stopBefore,
                                                                                 frame_info_t *frame_info );
 
/** @brief The PowerProfileScheduleConstraintsResponse Command is generated by a device supporting the server side of the Power Profile cluster to reply to a client side of this cluster which sent a PowerProfileScheduleConstraintsRequest.
 *
 * Cluster: Power Profile, This cluster provides an interface for transferring power profile information from a device (e.g. Whitegood ); to a controller (e.g. the Home Gateway );.  The Power Profile transferred can be solicited by client side (request command ); or can be notified directly from the device (server side );.
 * Command: PowerProfileScheduleConstraintsResponse
 * @param powerProfileId uint8_t
 * @param startAfter uint16_t
 * @param stopBefore uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPowerProfileClusterPowerProfileScheduleConstraintsResponse(uint8_t powerProfileId,
                                                                             uint16_t startAfter,
                                                                             uint16_t stopBefore,
                                                                             frame_info_t *frame_info );
 
/** @brief The Get Power Profile Price Extended command is generated by the server (e.g., White Goods ); in order to retrieve the cost associated to a specific Power profile considering specific conditions described in the option field (e.g., a specific time );.
 *
 * Cluster: Power Profile, This cluster provides an interface for transferring power profile information from a device (e.g. Whitegood ); to a controller (e.g. the Home Gateway );.  The Power Profile transferred can be solicited by client side (request command ); or can be notified directly from the device (server side );.
 * Command: GetPowerProfilePriceExtended
 * @param options uint8_t
 * @param powerProfileId uint8_t
 * @param powerProfileStartTime uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPowerProfileClusterGetPowerProfilePriceExtended(uint8_t options,
                                                                  uint8_t powerProfileId,
                                                                  uint16_t powerProfileStartTime,
                                                                  frame_info_t *frame_info );
 
/** @} END Power Profile Commands */

/** @name Appliance Control Commands */
 
/** @brief This basic message is used to retrieve Household Appliances status.
 *
 * Cluster: Appliance Control, This cluster provides an interface to remotely control and to program household appliances.
 * Command: SignalState
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandApplianceControlClusterSignalState(
    frame_info_t *frame_info );
 
/** @brief This basic message is used to set appliance functions, i.e. information regarding the execution of an appliance cycle.  Condition parameters such as start time or finish time information could be provided through this command.
 *
 * Cluster: Appliance Control, This cluster provides an interface to remotely control and to program household appliances.
 * Command: WriteFunctions
 * @param functionId uint16_t
 * @param functionDataType uint8_t
 * @param functionData uint8_t*
 * @param functionDataLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandApplianceControlClusterWriteFunctions(uint16_t functionId,
                                                        uint8_t functionDataType,
                                                        uint8_t *functionData,
                                                        uint16_t functionDataLen,
                                                        frame_info_t *frame_info );
 
/** @brief This command shall be used to resume the normal behavior of a household appliance being in pause mode after receiving a Overload Pause command.
 *
 * Cluster: Appliance Control, This cluster provides an interface to remotely control and to program household appliances.
 * Command: OverloadPauseResume
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandApplianceControlClusterOverloadPauseResume(
    frame_info_t *frame_info );
 
/** @brief This command shall be used to pause the household appliance as a consequence of an imminent overload event.
 *
 * Cluster: Appliance Control, This cluster provides an interface to remotely control and to program household appliances.
 * Command: OverloadPause
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandApplianceControlClusterOverloadPause(
    frame_info_t *frame_info );
 
/** @brief This basic message is used to send warnings the household appliance as a consequence of a possible overload event, or the notification of the end of the warning state.
 *
 * Cluster: Appliance Control, This cluster provides an interface to remotely control and to program household appliances.
 * Command: OverloadWarning
 * @param warningEvent uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandApplianceControlClusterOverloadWarning(uint8_t warningEvent,
                                                         frame_info_t *frame_info );
 
/** @brief This command shall be used to 
	return household appliance status, according to Appliance Status Values and Remote Enable Flags Values.
 *
 * Cluster: Appliance Control, This cluster provides an interface to remotely control and to program household appliances.
 * Command: SignalStateResponse
 * @param applianceStatus uint8_t
 * @param remoteEnableFlagsAndDeviceStatus2 uint8_t
 * @param applianceStatus2 uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandApplianceControlClusterSignalStateResponse(uint8_t applianceStatus,
                                                             uint8_t remoteEnableFlagsAndDeviceStatus2,
                                                             uint32_t applianceStatus2,
                                                             frame_info_t *frame_info );
 
/** @brief This command shall be used to 
	return household appliance status, automatically when appliance status changes.
 *
 * Cluster: Appliance Control, This cluster provides an interface to remotely control and to program household appliances.
 * Command: SignalStateNotification
 * @param applianceStatus uint8_t
 * @param remoteEnableFlagsAndDeviceStatus2 uint8_t
 * @param applianceStatus2 uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandApplianceControlClusterSignalStateNotification(uint8_t applianceStatus,
                                                                 uint8_t remoteEnableFlagsAndDeviceStatus2,
                                                                 uint32_t applianceStatus2,
                                                                 frame_info_t *frame_info );
 
/** @} END Appliance Control Commands */

/** @name Poll Control Commands */
 
/** @brief The Check-in Response is sent in response to the receipt of a Check-in command.
 *
 * Cluster: Poll Control, This cluster provides a mechanism for the management of an end device's MAC Data Poll rate.  For the purposes of this cluster, the term "poll" always refers to the sending of a MAC Data Poll from the end device to the end device's parent.
 * Command: CheckInResponse
 * @param startFastPolling uint8_t
 * @param fastPollTimeout uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPollControlClusterCheckInResponse(uint8_t startFastPolling,
                                                    uint16_t fastPollTimeout,
                                                    frame_info_t *frame_info );
 
/** @brief The Fast Poll Stop command is used to stop the fast poll mode initiated by the Check-in response.
 *
 * Cluster: Poll Control, This cluster provides a mechanism for the management of an end device's MAC Data Poll rate.  For the purposes of this cluster, the term "poll" always refers to the sending of a MAC Data Poll from the end device to the end device's parent.
 * Command: FastPollStop
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPollControlClusterFastPollStop(frame_info_t *frame_info );
 
/** @brief The Set Long Poll Interval command is used to set the read only Long Poll Interval Attribute.
 *
 * Cluster: Poll Control, This cluster provides a mechanism for the management of an end device's MAC Data Poll rate.  For the purposes of this cluster, the term "poll" always refers to the sending of a MAC Data Poll from the end device to the end device's parent.
 * Command: SetLongPollInterval
 * @param newLongPollInterval uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPollControlClusterSetLongPollInterval(uint32_t newLongPollInterval,
                                                        frame_info_t *frame_info );
 
/** @brief The Set Short Poll Interval command is used to set the read only Short Poll Interval Attribute.
 *
 * Cluster: Poll Control, This cluster provides a mechanism for the management of an end device's MAC Data Poll rate.  For the purposes of this cluster, the term "poll" always refers to the sending of a MAC Data Poll from the end device to the end device's parent.
 * Command: SetShortPollInterval
 * @param newShortPollInterval uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPollControlClusterSetShortPollInterval(uint16_t newShortPollInterval,
                                                         frame_info_t *frame_info );
 
/** @} END Poll Control Commands */

/** @name Green Power Commands */
 
/** @brief From GPP to GPSs in entire network to get pairing indication related to GPD for Proxy Table update.
 *
 * Cluster: Green Power, The Green Power cluster defines the format of the commands exchanged when handling GPDs.
 * Command: GpPairingSearch
 * @param options uint16_t
 * @param gpdSrcId uint32_t
 * @param gpdIeee uint8_t*
 * @param endpoint uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandGreenPowerClusterGpPairingSearch(uint16_t options,
                                                   uint32_t gpdSrcId,
                                                   uint8_t *gpdIeee,
                                                   uint8_t endpoint,
                                                   frame_info_t *frame_info );
 
/** @brief From GPP to neighbor GPPs to indicate GP Notification sent in unicast mode.
 *
 * Cluster: Green Power, The Green Power cluster defines the format of the commands exchanged when handling GPDs.
 * Command: GpTunnelingStop
 * @param options uint8_t
 * @param gpdSrcId uint32_t
 * @param gpdIeee uint8_t*
 * @param endpoint uint8_t
 * @param gpdSecurityFrameCounter uint32_t
 * @param gppShortAddress uint16_t
 * @param gppDistance int8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandGreenPowerClusterGpTunnelingStop(uint8_t options,
                                                   uint32_t gpdSrcId,
                                                   uint8_t *gpdIeee,
                                                   uint8_t endpoint,
                                                   uint32_t gpdSecurityFrameCounter,
                                                   uint16_t gppShortAddress,
                                                   int8_t gppDistance,
                                                   frame_info_t *frame_info );
 
/** @brief From GPP to GPS to tunnel GPD commissioning data.
 *
 * Cluster: Green Power, The Green Power cluster defines the format of the commands exchanged when handling GPDs.
 * Command: GpCommissioningNotification
 * @param options uint16_t
 * @param gpdSrcId uint32_t
 * @param gpdIeee uint8_t*
 * @param endpoint uint8_t
 * @param gpdSecurityFrameCounter uint32_t
 * @param gpdCommandId uint8_t
 * @param gpdCommandPayload uint8_t*
 * @param gppShortAddress uint16_t
 * @param gppLink uint8_t
 * @param mic uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandGreenPowerClusterGpCommissioningNotification(uint16_t options,
                                                               uint32_t gpdSrcId,
                                                               uint8_t *gpdIeee,
                                                               uint8_t endpoint,
                                                               uint32_t gpdSecurityFrameCounter,
                                                               uint8_t gpdCommandId,
                                                               uint8_t *gpdCommandPayload,
                                                               uint16_t gppShortAddress,
                                                               uint8_t gppLink,
                                                               uint32_t mic,
                                                               frame_info_t *frame_info );
 
/** @brief To enable commissioning mode of the sink, over the air
 *
 * Cluster: Green Power, The Green Power cluster defines the format of the commands exchanged when handling GPDs.
 * Command: GpSinkCommissioningMode
 * @param options uint8_t
 * @param gpmAddrForSecurity uint16_t
 * @param gpmAddrForPairing uint16_t
 * @param sinkEndpoint uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandGreenPowerClusterGpSinkCommissioningMode(uint8_t options,
                                                           uint16_t gpmAddrForSecurity,
                                                           uint16_t gpmAddrForPairing,
                                                           uint8_t sinkEndpoint,
                                                           frame_info_t *frame_info );
 
/** @brief To configure GPD Command Translation Table.
 *
 * Cluster: Green Power, The Green Power cluster defines the format of the commands exchanged when handling GPDs.
 * Command: GpTranslationTableUpdate
 * @param options uint16_t
 * @param gpdSrcId uint32_t
 * @param gpdIeee uint8_t*
 * @param endpoint uint8_t
 * @param translations uint8_t*
 * @param translationsLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandGreenPowerClusterGpTranslationTableUpdate(uint16_t options,
                                                            uint32_t gpdSrcId,
                                                            uint8_t *gpdIeee,
                                                            uint8_t endpoint,
                                                            uint8_t *translations,
                                                            uint16_t translationsLen,
                                                            frame_info_t *frame_info );
 
/** @brief To provide GPD Command Translation Table content.
 *
 * Cluster: Green Power, The Green Power cluster defines the format of the commands exchanged when handling GPDs.
 * Command: GpTranslationTableRequest
 * @param startIndex uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandGreenPowerClusterGpTranslationTableRequest(uint8_t startIndex,
                                                             frame_info_t *frame_info );
 
/** @brief To configure Sink Table.
 *
 * Cluster: Green Power, The Green Power cluster defines the format of the commands exchanged when handling GPDs.
 * Command: GpPairingConfiguration
 * @param actions uint8_t
 * @param options uint16_t
 * @param gpdSrcId uint32_t
 * @param gpdIeee uint8_t*
 * @param endpoint uint8_t
 * @param deviceId uint8_t
 * @param groupListCount uint8_t
 * @param groupList uint8_t*
 * @param groupListLen uint16_t
 * @param gpdAssignedAlias uint16_t
 * @param groupcastRadius uint8_t
 * @param securityOptions uint8_t
 * @param gpdSecurityFrameCounter uint32_t
 * @param gpdSecurityKey uint8_t*
 * @param numberOfPairedEndpoints uint8_t
 * @param pairedEndpoints uint8_t*
 * @param pairedEndpointsLen uint16_t
 * @param applicationInformation uint8_t
 * @param manufacturerId uint16_t
 * @param modeId uint16_t
 * @param numberOfGpdCommands uint8_t
 * @param gpdCommandIdList uint8_t*
 * @param gpdCommandIdListLen uint16_t
 * @param clusterIdListCount uint8_t
 * @param clusterListServer uint8_t*
 * @param clusterListServerLen uint16_t
 * @param clusterListClient uint8_t*
 * @param clusterListClientLen uint16_t
 * @param switchInformationLength uint8_t
 * @param switchConfiguration uint8_t
 * @param currentContactStatus uint8_t
 * @param totalNumberOfReports uint8_t
 * @param numberOfReports uint8_t
 * @param reportDescriptor uint8_t*
 * @param reportDescriptorLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandGreenPowerClusterGpPairingConfiguration(uint8_t actions,
                                                          uint16_t options,
                                                          uint32_t gpdSrcId,
                                                          uint8_t *gpdIeee,
                                                          uint8_t endpoint,
                                                          uint8_t deviceId,
                                                          uint8_t groupListCount,
                                                          uint8_t *groupList,
                                                          uint16_t groupListLen,
                                                          uint16_t gpdAssignedAlias,
                                                          uint8_t groupcastRadius,
                                                          uint8_t securityOptions,
                                                          uint32_t gpdSecurityFrameCounter,
                                                          uint8_t *gpdSecurityKey,
                                                          uint8_t numberOfPairedEndpoints,
                                                          uint8_t *pairedEndpoints,
                                                          uint16_t pairedEndpointsLen,
                                                          uint8_t applicationInformation,
                                                          uint16_t manufacturerId,
                                                          uint16_t modeId,
                                                          uint8_t numberOfGpdCommands,
                                                          uint8_t *gpdCommandIdList,
                                                          uint16_t gpdCommandIdListLen,
                                                          uint8_t clusterIdListCount,
                                                          uint8_t *clusterListServer,
                                                          uint16_t clusterListServerLen,
                                                          uint8_t *clusterListClient,
                                                          uint16_t clusterListClientLen,
                                                          uint8_t switchInformationLength,
                                                          uint8_t switchConfiguration,
                                                          uint8_t currentContactStatus,
                                                          uint8_t totalNumberOfReports,
                                                          uint8_t numberOfReports,
                                                          uint8_t *reportDescriptor,
                                                          uint16_t reportDescriptorLen,
                                                          frame_info_t *frame_info );
 
/** @brief To read out selected Sink Table Entries, by index or by GPD ID.
 *
 * Cluster: Green Power, The Green Power cluster defines the format of the commands exchanged when handling GPDs.
 * Command: GpSinkTableRequest
 * @param options uint8_t
 * @param gpdSrcId uint32_t
 * @param gpdIeee uint8_t*
 * @param endpoint uint8_t
 * @param index uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandGreenPowerClusterGpSinkTableRequest(uint8_t options,
                                                      uint32_t gpdSrcId,
                                                      uint8_t *gpdIeee,
                                                      uint8_t endpoint,
                                                      uint8_t index,
                                                      frame_info_t *frame_info );
 
/** @brief To reply with read-out Proxy Table entries, by index or by GPD ID.
 *
 * Cluster: Green Power, The Green Power cluster defines the format of the commands exchanged when handling GPDs.
 * Command: GpProxyTableResponse
 * @param status uint8_t
 * @param totalNumberOfNonEmptyProxyTableEntries uint8_t
 * @param startIndex uint8_t
 * @param entriesCount uint8_t
 * @param proxyTableEntries uint8_t*
 * @param proxyTableEntriesLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandGreenPowerClusterGpProxyTableResponse(uint8_t status,
                                                        uint8_t totalNumberOfNonEmptyProxyTableEntries,
                                                        uint8_t startIndex,
                                                        uint8_t entriesCount,
                                                        uint8_t *proxyTableEntries,
                                                        uint16_t proxyTableEntriesLen,
                                                        frame_info_t *frame_info );
 
/** @brief From GPS to GPP to acknowledge GP Notification received in unicast mode.
 *
 * Cluster: Green Power, The Green Power cluster defines the format of the commands exchanged when handling GPDs.
 * Command: GpNotificationResponse
 * @param options uint8_t
 * @param gpdSrcId uint32_t
 * @param gpdIeee uint8_t*
 * @param endpoint uint8_t
 * @param gpdSecurityFrameCounter uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandGreenPowerClusterGpNotificationResponse(uint8_t options,
                                                          uint32_t gpdSrcId,
                                                          uint8_t *gpdIeee,
                                                          uint8_t endpoint,
                                                          uint32_t gpdSecurityFrameCounter,
                                                          frame_info_t *frame_info );
 
/** @brief From GPS to the entire network to (de );register for tunneling service, or for removing GPD from the network.
 *
 * Cluster: Green Power, The Green Power cluster defines the format of the commands exchanged when handling GPDs.
 * Command: GpPairing
 * @param options uint32_t
 * @param gpdSrcId uint32_t
 * @param gpdIeee uint8_t*
 * @param endpoint uint8_t
 * @param sinkIeeeAddress uint8_t*
 * @param sinkNwkAddress uint16_t
 * @param sinkGroupId uint16_t
 * @param deviceId uint8_t
 * @param gpdSecurityFrameCounter uint32_t
 * @param gpdKey uint8_t*
 * @param assignedAlias uint16_t
 * @param groupcastRadius uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandGreenPowerClusterGpPairing(uint32_t options,
                                             uint32_t gpdSrcId,
                                             uint8_t *gpdIeee,
                                             uint8_t endpoint,
                                             uint8_t *sinkIeeeAddress,
                                             uint16_t sinkNwkAddress,
                                             uint16_t sinkGroupId,
                                             uint8_t deviceId,
                                             uint32_t gpdSecurityFrameCounter,
                                             uint8_t *gpdKey,
                                             uint16_t assignedAlias,
                                             uint8_t groupcastRadius,
                                             frame_info_t *frame_info );
 
/** @brief From GPS to GPPs in the whole network to indicate commissioning mode.
 *
 * Cluster: Green Power, The Green Power cluster defines the format of the commands exchanged when handling GPDs.
 * Command: GpProxyCommissioningMode
 * @param options uint8_t
 * @param commissioningWindow uint16_t
 * @param channel uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandGreenPowerClusterGpProxyCommissioningMode(uint8_t options,
                                                            uint16_t commissioningWindow,
                                                            uint8_t channel,
                                                            frame_info_t *frame_info );
 
/** @brief From GPS to selected GPP, to provide data to be transmitted to Rx-capable GPD.
 *
 * Cluster: Green Power, The Green Power cluster defines the format of the commands exchanged when handling GPDs.
 * Command: GpResponse
 * @param options uint8_t
 * @param tempMasterShortAddress uint16_t
 * @param tempMasterTxChannel uint8_t
 * @param gpdSrcId uint32_t
 * @param gpdIeee uint8_t*
 * @param endpoint uint8_t
 * @param gpdCommandId uint8_t
 * @param gpdCommandPayload uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandGreenPowerClusterGpResponse(uint8_t options,
                                              uint16_t tempMasterShortAddress,
                                              uint8_t tempMasterTxChannel,
                                              uint32_t gpdSrcId,
                                              uint8_t *gpdIeee,
                                              uint8_t endpoint,
                                              uint8_t gpdCommandId,
                                              uint8_t *gpdCommandPayload,
                                              frame_info_t *frame_info );
 
/** @brief To provide GPD Command Translation Table content.
 *
 * Cluster: Green Power, The Green Power cluster defines the format of the commands exchanged when handling GPDs.
 * Command: GpTranslationTableResponse
 * @param status uint8_t
 * @param options uint8_t
 * @param totalNumberOfEntries uint8_t
 * @param startIndex uint8_t
 * @param entriesCount uint8_t
 * @param translationTableList uint8_t*
 * @param translationTableListLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandGreenPowerClusterGpTranslationTableResponse(uint8_t status,
                                                              uint8_t options,
                                                              uint8_t totalNumberOfEntries,
                                                              uint8_t startIndex,
                                                              uint8_t entriesCount,
                                                              uint8_t *translationTableList,
                                                              uint16_t translationTableListLen,
                                                              frame_info_t *frame_info );
 
/** @brief To selected Proxy Table entries, by index or by GPD ID.
 *
 * Cluster: Green Power, The Green Power cluster defines the format of the commands exchanged when handling GPDs.
 * Command: GpSinkTableResponse
 * @param status uint8_t
 * @param totalNumberofNonEmptySinkTableEntries uint8_t
 * @param startIndex uint8_t
 * @param sinkTableEntriesCount uint8_t
 * @param sinkTableEntries uint8_t*
 * @param sinkTableEntriesLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandGreenPowerClusterGpSinkTableResponse(uint8_t status,
                                                       uint8_t totalNumberofNonEmptySinkTableEntries,
                                                       uint8_t startIndex,
                                                       uint8_t sinkTableEntriesCount,
                                                       uint8_t *sinkTableEntries,
                                                       uint16_t sinkTableEntriesLen,
                                                       frame_info_t *frame_info );
 
/** @brief To request selected Proxy Table entries, by index or by GPD ID.
 *
 * Cluster: Green Power, The Green Power cluster defines the format of the commands exchanged when handling GPDs.
 * Command: GpProxyTableRequest
 * @param options uint8_t
 * @param gpdSrcId uint32_t
 * @param gpdIeee uint8_t*
 * @param endpoint uint8_t
 * @param index uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandGreenPowerClusterGpProxyTableRequest(uint8_t options,
                                                       uint32_t gpdSrcId,
                                                       uint8_t *gpdIeee,
                                                       uint8_t endpoint,
                                                       uint8_t index,
                                                       frame_info_t *frame_info );
 
/** @} END Green Power Commands */

/** @name Keep-Alive Commands */
/** @} END Keep-Alive Commands */

/** @name Shade Configuration Commands */
/** @} END Shade Configuration Commands */

/** @name Door Lock Commands */
 
/** @brief Unlocks the door
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: UnlockDoor
 * @param PIN uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterUnlockDoor(uint8_t *PIN,
                                            frame_info_t *frame_info );
 
/** @brief Toggles the door lock from its current state to the opposite state locked or unlocked.
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: Toggle
 * @param pin uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterToggle(uint8_t *pin,
                                        frame_info_t *frame_info );
 
/** @brief Unlock the door with a timeout. When the timeout expires, the door will automatically re-lock.
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: UnlockWithTimeout
 * @param timeoutInSeconds uint16_t
 * @param pin uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterUnlockWithTimeout(uint16_t timeoutInSeconds,
                                                   uint8_t *pin,
                                                   frame_info_t *frame_info );
 
/** @brief Retrieve a log record at a specified index.
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: GetLogRecord
 * @param logIndex uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterGetLogRecord(uint16_t logIndex,
                                              frame_info_t *frame_info );
 
/** @brief Set the PIN for a specified user id.
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: SetPin
 * @param userId uint16_t
 * @param userStatus uint8_t
 * @param userType uint8_t
 * @param pin uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterSetPin(uint16_t userId,
                                        uint8_t userStatus,
                                        uint8_t userType,
                                        uint8_t *pin,
                                        frame_info_t *frame_info );
 
/** @brief Retrieve PIN information for a user with a specific user ID.
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: GetPin
 * @param userId uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterGetPin(uint16_t userId,
                                        frame_info_t *frame_info );
 
/** @brief Clear the PIN for a user with a specific user ID
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: ClearPin
 * @param userId uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterClearPin(uint16_t userId,
                                          frame_info_t *frame_info );
 
/** @brief Clear all PIN codes on the lock for all users.
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: ClearAllPins
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterClearAllPins(
    frame_info_t *frame_info );
 
/** @brief Set the status value for a specified user ID.
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: SetUserStatus
 * @param userId uint16_t
 * @param userStatus uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterSetUserStatus(uint16_t userId,
                                               uint8_t userStatus,
                                               frame_info_t *frame_info );
 
/** @brief Retrieve the status byte for a specific user.
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: GetUserStatus
 * @param userId uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterGetUserStatus(uint16_t userId,
                                               frame_info_t *frame_info );
 
/** @brief Set the schedule of days during the week that the associated user based on the user ID will have access to the lock and will be able to operate it.
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: SetWeekdaySchedule
 * @param scheduleId uint8_t
 * @param userId uint16_t
 * @param daysMask uint8_t
 * @param startHour uint8_t
 * @param startMinute uint8_t
 * @param endHour uint8_t
 * @param endMinute uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterSetWeekdaySchedule(uint8_t scheduleId,
                                                    uint16_t userId,
                                                    uint8_t daysMask,
                                                    uint8_t startHour,
                                                    uint8_t startMinute,
                                                    uint8_t endHour,
                                                    uint8_t endMinute,
                                                    frame_info_t *frame_info );
 
/** @brief Retrieve a weekday schedule for doorlock user activation for a specific schedule id and user id.
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: GetWeekdaySchedule
 * @param scheduleId uint8_t
 * @param userId uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterGetWeekdaySchedule(uint8_t scheduleId,
                                                    uint16_t userId,
                                                    frame_info_t *frame_info );
 
/** @brief Clear a weekday schedule for doorlock user activation for a specific schedule id and user id.
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: ClearWeekdaySchedule
 * @param scheduleId uint8_t
 * @param userId uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterClearWeekdaySchedule(uint8_t scheduleId,
                                                      uint16_t userId,
                                                      frame_info_t *frame_info );
 
/** @brief Set a door lock user id activation schedule according to a specific absolute local start and end time
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: SetYeardaySchedule
 * @param scheduleId uint8_t
 * @param userId uint16_t
 * @param localStartTime uint32_t
 * @param localEndTime uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterSetYeardaySchedule(uint8_t scheduleId,
                                                    uint16_t userId,
                                                    uint32_t localStartTime,
                                                    uint32_t localEndTime,
                                                    frame_info_t *frame_info );
 
/** @brief Retrieve a yearday schedule for a specific scheduleId and userId
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: GetYeardaySchedule
 * @param scheduleId uint8_t
 * @param userId uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterGetYeardaySchedule(uint8_t scheduleId,
                                                    uint16_t userId,
                                                    frame_info_t *frame_info );
 
/** @brief Clear a yearday schedule for a specific scheduleId and userId
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: ClearYeardaySchedule
 * @param scheduleId uint8_t
 * @param userId uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterClearYeardaySchedule(uint8_t scheduleId,
                                                      uint16_t userId,
                                                      frame_info_t *frame_info );
 
/** @brief Set the holiday schedule for a specific user
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: SetHolidaySchedule
 * @param scheduleId uint8_t
 * @param localStartTime uint32_t
 * @param localEndTime uint32_t
 * @param operatingModeDuringHoliday uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterSetHolidaySchedule(uint8_t scheduleId,
                                                    uint32_t localStartTime,
                                                    uint32_t localEndTime,
                                                    uint8_t operatingModeDuringHoliday,
                                                    frame_info_t *frame_info );
 
/** @brief Retrieve a holiday schedule for a specific scheduleId
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: GetHolidaySchedule
 * @param scheduleId uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterGetHolidaySchedule(uint8_t scheduleId,
                                                    frame_info_t *frame_info );
 
/** @brief Clear a holiday schedule for a specific scheduleId
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: ClearHolidaySchedule
 * @param scheduleId uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterClearHolidaySchedule(uint8_t scheduleId,
                                                      frame_info_t *frame_info );
 
/** @brief Set the type value for a user based on user ID.
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: SetUserType
 * @param userId uint16_t
 * @param userType uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterSetUserType(uint16_t userId,
                                             uint8_t userType,
                                             frame_info_t *frame_info );
 
/** @brief Retrieve the type for a specific user based on the user ID.
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: GetUserType
 * @param userId uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterGetUserType(uint16_t userId,
                                             frame_info_t *frame_info );
 
/** @brief Set the PIN for a specified user id.
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: SetRfid
 * @param userId uint16_t
 * @param userStatus uint8_t
 * @param userType uint8_t
 * @param id uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterSetRfid(uint16_t userId,
                                         uint8_t userStatus,
                                         uint8_t userType,
                                         uint8_t *id,
                                         frame_info_t *frame_info );
 
/** @brief Retrieve RFID ID information for a user with a specific user ID.
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: GetRfid
 * @param userId uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterGetRfid(uint16_t userId,
                                         frame_info_t *frame_info );
 
/** @brief Clear the RFID ID for a user with a specific user ID
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: ClearRfid
 * @param userId uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterClearRfid(uint16_t userId,
                                           frame_info_t *frame_info );
 
/** @brief Clear all RFID ID codes on the lock for all users.
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: ClearAllRfids
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterClearAllRfids(
    frame_info_t *frame_info );
 
/** @brief Indicates lock success or failure
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: LockDoorResponse
 * @param status uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterLockDoorResponse(uint8_t status,
                                                  frame_info_t *frame_info );
 
/** @brief Indicates unlock success or failure
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: UnlockDoorResponse
 * @param status uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterUnlockDoorResponse(uint8_t status,
                                                    frame_info_t *frame_info );
 
/** @brief Response provided to the toggle command, indicates whether the toggle was successful or not.
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: ToggleResponse
 * @param status uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterToggleResponse(uint8_t status,
                                                frame_info_t *frame_info );
 
/** @brief Response provided to unlock with specific timeout. This command indicates whether the unlock command was successful or not.
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: UnlockWithTimeoutResponse
 * @param status uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterUnlockWithTimeoutResponse(uint8_t status,
                                                           frame_info_t *frame_info );
 
/** @brief Returns the specific log record requested.
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: GetLogRecordResponse
 * @param logEntryId uint16_t
 * @param timestamp uint32_t
 * @param eventType uint8_t
 * @param source uint8_t
 * @param eventIdOrAlarmCode uint8_t
 * @param userId uint16_t
 * @param pin uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterGetLogRecordResponse(uint16_t logEntryId,
                                                      uint32_t timestamp,
                                                      uint8_t eventType,
                                                      uint8_t source,
                                                      uint8_t eventIdOrAlarmCode,
                                                      uint16_t userId,
                                                      uint8_t *pin,
                                                      frame_info_t *frame_info );
 
/** @brief Indicates whether the setting of the PIN was successful or not.
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: SetPinResponse
 * @param status uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterSetPinResponse(uint8_t status,
                                                frame_info_t *frame_info );
 
/** @brief Returns the PIN requested according to the user ID passed.
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: GetPinResponse
 * @param userId uint16_t
 * @param userStatus uint8_t
 * @param userType uint8_t
 * @param pin uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterGetPinResponse(uint16_t userId,
                                                uint8_t userStatus,
                                                uint8_t userType,
                                                uint8_t *pin,
                                                frame_info_t *frame_info );
 
/** @brief Returns success or failure depending on whether the PIN was cleared or not.
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: ClearPinResponse
 * @param status uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterClearPinResponse(uint8_t status,
                                                  frame_info_t *frame_info );
 
/** @brief Returns success or failure depending on whether the PINs were cleared or not.
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: ClearAllPinsResponse
 * @param status uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterClearAllPinsResponse(uint8_t status,
                                                      frame_info_t *frame_info );
 
/** @brief Returns success or failure depending on whether the user status was set or not.
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: SetUserStatusResponse
 * @param status uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterSetUserStatusResponse(uint8_t status,
                                                       frame_info_t *frame_info );
 
/** @brief Returns the user status.
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: GetUserStatusResponse
 * @param userId uint16_t
 * @param status uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterGetUserStatusResponse(uint16_t userId,
                                                       uint8_t status,
                                                       frame_info_t *frame_info );
 
/** @brief Returns the status of setting the weekday schedule
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: SetWeekdayScheduleResponse
 * @param status uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterSetWeekdayScheduleResponse(uint8_t status,
                                                            frame_info_t *frame_info );
 
/** @brief Returns the weekday schedule requested.
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: GetWeekdayScheduleResponse
 * @param scheduleId uint8_t
 * @param userId uint16_t
 * @param status uint8_t
 * @param daysMask uint8_t
 * @param startHour uint8_t
 * @param startMinute uint8_t
 * @param endHour uint8_t
 * @param endMinute uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterGetWeekdayScheduleResponse(uint8_t scheduleId,
                                                            uint16_t userId,
                                                            uint8_t status,
                                                            uint8_t daysMask,
                                                            uint8_t startHour,
                                                            uint8_t startMinute,
                                                            uint8_t endHour,
                                                            uint8_t endMinute,
                                                            frame_info_t *frame_info );
 
/** @brief Returns the status of clearing the weekday schedule
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: ClearWeekdayScheduleResponse
 * @param status uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterClearWeekdayScheduleResponse(uint8_t status,
                                                              frame_info_t *frame_info );
 
/** @brief Returns success or failure depending on whether the yearday schedule was set or not.
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: SetYeardayScheduleResponse
 * @param status uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterSetYeardayScheduleResponse(uint8_t status,
                                                            frame_info_t *frame_info );
 
/** @brief Returns the yearday schedule requested
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: GetYeardayScheduleResponse
 * @param scheduleId uint8_t
 * @param userId uint16_t
 * @param status uint8_t
 * @param localStartTime uint32_t
 * @param localEndTime uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterGetYeardayScheduleResponse(uint8_t scheduleId,
                                                            uint16_t userId,
                                                            uint8_t status,
                                                            uint32_t localStartTime,
                                                            uint32_t localEndTime,
                                                            frame_info_t *frame_info );
 
/** @brief Returns success or failure depending on whether the yearday schedule was removed or not.
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: ClearYeardayScheduleResponse
 * @param status uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterClearYeardayScheduleResponse(uint8_t status,
                                                              frame_info_t *frame_info );
 
/** @brief Returns success or failure depending on whether the holiday schedule was set or not.
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: SetHolidayScheduleResponse
 * @param status uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterSetHolidayScheduleResponse(uint8_t status,
                                                            frame_info_t *frame_info );
 
/** @brief Returns the holiday schedule requested
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: GetHolidayScheduleResponse
 * @param scheduleId uint8_t
 * @param status uint8_t
 * @param localStartTime uint32_t
 * @param localEndTime uint32_t
 * @param operatingModeDuringHoliday uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterGetHolidayScheduleResponse(uint8_t scheduleId,
                                                            uint8_t status,
                                                            uint32_t localStartTime,
                                                            uint32_t localEndTime,
                                                            uint8_t operatingModeDuringHoliday,
                                                            frame_info_t *frame_info );
 
/** @brief Returns success or failure depending on whether the holiday schedule was removed or not.
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: ClearHolidayScheduleResponse
 * @param status uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterClearHolidayScheduleResponse(uint8_t status,
                                                              frame_info_t *frame_info );
 
/** @brief 
	returns success or failure depending on whether the user type was set or not.
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: SetUserTypeResponse
 * @param status uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterSetUserTypeResponse(uint8_t status,
                                                     frame_info_t *frame_info );
 
/** @brief Returns the user type for the user ID requested.
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: GetUserTypeResponse
 * @param userId uint16_t
 * @param userType uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterGetUserTypeResponse(uint16_t userId,
                                                     uint8_t userType,
                                                     frame_info_t *frame_info );
 
/** @brief Indicates whether the setting of the RFID ID was successful or not.
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: SetRfidResponse
 * @param status uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterSetRfidResponse(uint8_t status,
                                                 frame_info_t *frame_info );
 
/** @brief Returns the RFID ID requested according to the user ID passed.
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: GetRfidResponse
 * @param userId uint16_t
 * @param userStatus uint8_t
 * @param userType uint8_t
 * @param rfid uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterGetRfidResponse(uint16_t userId,
                                                 uint8_t userStatus,
                                                 uint8_t userType,
                                                 uint8_t *rfid,
                                                 frame_info_t *frame_info );
 
/** @brief Returns success or failure depending on whether the RFID ID was cleared or not.
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: ClearRfidResponse
 * @param status uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterClearRfidResponse(uint8_t status,
                                                   frame_info_t *frame_info );
 
/** @brief Returns success or failure depending on whether the RFID IDs were cleared or not.
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: ClearAllRfidsResponse
 * @param status uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterClearAllRfidsResponse(uint8_t status,
                                                       frame_info_t *frame_info );
 
/** @brief Indicates that an operation event has taken place. Includes the associated event information.
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: OperationEventNotification
 * @param source uint8_t
 * @param eventCode uint8_t
 * @param userId uint16_t
 * @param pin uint8_t*
 * @param timeStamp uint32_t
 * @param data uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterOperationEventNotification(uint8_t source,
                                                            uint8_t eventCode,
                                                            uint16_t userId,
                                                            uint8_t *pin,
                                                            uint32_t timeStamp,
                                                            uint8_t *data,
                                                            frame_info_t *frame_info );
 
/** @brief Indicates that a programming event has taken place. Includes the associated programming event information.
 *
 * Cluster: Door Lock, Provides an interface into a generic way to secure a door.
 * Command: ProgrammingEventNotification
 * @param source uint8_t
 * @param eventCode uint8_t
 * @param userId uint16_t
 * @param pin uint8_t*
 * @param userType uint8_t
 * @param userStatus uint8_t
 * @param timeStamp uint32_t
 * @param data uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDoorLockClusterProgrammingEventNotification(uint8_t source,
                                                              uint8_t eventCode,
                                                              uint16_t userId,
                                                              uint8_t *pin,
                                                              uint8_t userType,
                                                              uint8_t userStatus,
                                                              uint32_t timeStamp,
                                                              uint8_t *data,
                                                              frame_info_t *frame_info );
 
/** @} END Door Lock Commands */

/** @name Window Covering Commands */
 
/** @brief Moves window covering to InstalledClosedLimit - Lift and InstalledCloseLimit - Tilt
 *
 * Cluster: Window Covering, Provides an interface for controlling and adjusting automatic window coverings.
 * Command: WindowCoveringDownClose
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandWindowCoveringClusterWindowCoveringDownClose(
    frame_info_t *frame_info );
 
/** @brief Stop any adjusting of window covering
 *
 * Cluster: Window Covering, Provides an interface for controlling and adjusting automatic window coverings.
 * Command: WindowCoveringStop
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandWindowCoveringClusterWindowCoveringStop(
    frame_info_t *frame_info );
 
/** @brief Goto lift value specified
 *
 * Cluster: Window Covering, Provides an interface for controlling and adjusting automatic window coverings.
 * Command: WindowCoveringGoToLiftValue
 * @param liftValue uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandWindowCoveringClusterWindowCoveringGoToLiftValue(uint16_t liftValue,
                                                                   frame_info_t *frame_info );
 
/** @brief Goto lift percentage specified
 *
 * Cluster: Window Covering, Provides an interface for controlling and adjusting automatic window coverings.
 * Command: WindowCoveringGoToLiftPercentage
 * @param percentageLiftValue uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandWindowCoveringClusterWindowCoveringGoToLiftPercentage(uint8_t percentageLiftValue,
                                                                        frame_info_t *frame_info );
 
/** @brief Goto tilt value specified
 *
 * Cluster: Window Covering, Provides an interface for controlling and adjusting automatic window coverings.
 * Command: WindowCoveringGoToTiltValue
 * @param tiltValue uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandWindowCoveringClusterWindowCoveringGoToTiltValue(uint16_t tiltValue,
                                                                   frame_info_t *frame_info );
 
/** @brief Goto tilt percentage specified
 *
 * Cluster: Window Covering, Provides an interface for controlling and adjusting automatic window coverings.
 * Command: WindowCoveringGoToTiltPercentage
 * @param percentageTiltValue uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandWindowCoveringClusterWindowCoveringGoToTiltPercentage(uint8_t percentageTiltValue,
                                                                        frame_info_t *frame_info );
 
/** @} END Window Covering Commands */

/** @name Barrier Control Commands */
 
/** @brief Command that instructs the barrier to stop moving.
 *
 * Cluster: Barrier Control, This cluster provides control of a barrier (garage door );.
 * Command: BarrierControlStop
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandBarrierControlClusterBarrierControlStop(
    frame_info_t *frame_info );
 
/** @} END Barrier Control Commands */

/** @name Pump Configuration and Control Commands */
/** @} END Pump Configuration and Control Commands */

/** @name Thermostat Commands */
 
/** @brief Command description for SetWeeklySchedule
 *
 * Cluster: Thermostat, An interface for configuring and controlling the functionality of a thermostat.
 * Command: SetWeeklySchedule
 * @param numberOfTransitionsForSequence uint8_t
 * @param dayOfWeekForSequence uint8_t
 * @param modeForSequence uint8_t
 * @param payload uint8_t*
 * @param payloadLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandThermostatClusterSetWeeklySchedule(uint8_t numberOfTransitionsForSequence,
                                                     uint8_t dayOfWeekForSequence,
                                                     uint8_t modeForSequence,
                                                     uint8_t *payload,
                                                     uint16_t payloadLen,
                                                     frame_info_t *frame_info );
 
/** @brief Command description for GetWeeklySchedule
 *
 * Cluster: Thermostat, An interface for configuring and controlling the functionality of a thermostat.
 * Command: GetWeeklySchedule
 * @param daysToReturn uint8_t
 * @param modeToReturn uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandThermostatClusterGetWeeklySchedule(uint8_t daysToReturn,
                                                     uint8_t modeToReturn,
                                                     frame_info_t *frame_info );
 
/** @brief The Clear Weekly Schedule command is used to clear the weekly schedule.
 *
 * Cluster: Thermostat, An interface for configuring and controlling the functionality of a thermostat.
 * Command: ClearWeeklySchedule
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandThermostatClusterClearWeeklySchedule(
    frame_info_t *frame_info );
 
/** @brief The Get Relay Status Log command is used to query the thermostat internal relay status log.
 *
 * Cluster: Thermostat, An interface for configuring and controlling the functionality of a thermostat.
 * Command: GetRelayStatusLog
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandThermostatClusterGetRelayStatusLog(
    frame_info_t *frame_info );
 
/** @brief The Current Weekly Schedule Command is sent from the server in response to the Get Weekly Schedule Command.
 *
 * Cluster: Thermostat, An interface for configuring and controlling the functionality of a thermostat.
 * Command: CurrentWeeklySchedule
 * @param numberOfTransitionsForSequence uint8_t
 * @param dayOfWeekForSequence uint8_t
 * @param modeForSequence uint8_t
 * @param payload uint8_t*
 * @param payloadLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandThermostatClusterCurrentWeeklySchedule(uint8_t numberOfTransitionsForSequence,
                                                         uint8_t dayOfWeekForSequence,
                                                         uint8_t modeForSequence,
                                                         uint8_t *payload,
                                                         uint16_t payloadLen,
                                                         frame_info_t *frame_info );
 
/** @brief This command is sent from the thermostat cluster server in response to the Get  Relay Status Log.
 *
 * Cluster: Thermostat, An interface for configuring and controlling the functionality of a thermostat.
 * Command: RelayStatusLog
 * @param timeOfDay uint16_t
 * @param relayStatus uint16_t
 * @param localTemperature int16_t
 * @param humidityInPercentage uint8_t
 * @param setpoint int16_t
 * @param unreadEntries uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandThermostatClusterRelayStatusLog(uint16_t timeOfDay,
                                                  uint16_t relayStatus,
                                                  int16_t localTemperature,
                                                  uint8_t humidityInPercentage,
                                                  int16_t setpoint,
                                                  uint16_t unreadEntries,
                                                  frame_info_t *frame_info );
 
/** @} END Thermostat Commands */

/** @name Fan Control Commands */
/** @} END Fan Control Commands */

/** @name Dehumidification Control Commands */
/** @} END Dehumidification Control Commands */

/** @name Thermostat User Interface Configuration Commands */
/** @} END Thermostat User Interface Configuration Commands */

/** @name Color Control Commands */
 
/** @brief Move hue up or down at specified rate.
 *
 * Cluster: Color Control, Attributes and commands for controlling the color properties of a color-capable light.
 * Command: MoveHue
 * @param moveMode uint8_t
 * @param rate uint8_t
 * @param optionsMask uint8_t
 * @param optionsOverride uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandColorControlClusterMoveHue(uint8_t moveMode,
                                             uint8_t rate,
                                             uint8_t optionsMask,
                                             uint8_t optionsOverride,
                                             frame_info_t *frame_info );
 
/** @brief Step hue up or down by specified size at specified rate.
 *
 * Cluster: Color Control, Attributes and commands for controlling the color properties of a color-capable light.
 * Command: StepHue
 * @param stepMode uint8_t
 * @param stepSize uint8_t
 * @param transitionTime uint8_t
 * @param optionsMask uint8_t
 * @param optionsOverride uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandColorControlClusterStepHue(uint8_t stepMode,
                                             uint8_t stepSize,
                                             uint8_t transitionTime,
                                             uint8_t optionsMask,
                                             uint8_t optionsOverride,
                                             frame_info_t *frame_info );
 
/** @brief Move to specified saturation.
 *
 * Cluster: Color Control, Attributes and commands for controlling the color properties of a color-capable light.
 * Command: MoveToSaturation
 * @param saturation uint8_t
 * @param transitionTime uint16_t
 * @param optionsMask uint8_t
 * @param optionsOverride uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandColorControlClusterMoveToSaturation(uint8_t saturation,
                                                      uint16_t transitionTime,
                                                      uint8_t optionsMask,
                                                      uint8_t optionsOverride,
                                                      frame_info_t *frame_info );
 
/** @brief Move saturation up or down at specified rate.
 *
 * Cluster: Color Control, Attributes and commands for controlling the color properties of a color-capable light.
 * Command: MoveSaturation
 * @param moveMode uint8_t
 * @param rate uint8_t
 * @param optionsMask uint8_t
 * @param optionsOverride uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandColorControlClusterMoveSaturation(uint8_t moveMode,
                                                    uint8_t rate,
                                                    uint8_t optionsMask,
                                                    uint8_t optionsOverride,
                                                    frame_info_t *frame_info );
 
/** @brief Step saturation up or down by specified size at specified rate.
 *
 * Cluster: Color Control, Attributes and commands for controlling the color properties of a color-capable light.
 * Command: StepSaturation
 * @param stepMode uint8_t
 * @param stepSize uint8_t
 * @param transitionTime uint8_t
 * @param optionsMask uint8_t
 * @param optionsOverride uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandColorControlClusterStepSaturation(uint8_t stepMode,
                                                    uint8_t stepSize,
                                                    uint8_t transitionTime,
                                                    uint8_t optionsMask,
                                                    uint8_t optionsOverride,
                                                    frame_info_t *frame_info );
 
/** @brief Move to hue and saturation.
 *
 * Cluster: Color Control, Attributes and commands for controlling the color properties of a color-capable light.
 * Command: MoveToHueAndSaturation
 * @param hue uint8_t
 * @param saturation uint8_t
 * @param transitionTime uint16_t
 * @param optionsMask uint8_t
 * @param optionsOverride uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandColorControlClusterMoveToHueAndSaturation(uint8_t hue,
                                                            uint8_t saturation,
                                                            uint16_t transitionTime,
                                                            uint8_t optionsMask,
                                                            uint8_t optionsOverride,
                                                            frame_info_t *frame_info );
 
/** @brief Move to specified color.
 *
 * Cluster: Color Control, Attributes and commands for controlling the color properties of a color-capable light.
 * Command: MoveToColor
 * @param colorX uint16_t
 * @param colorY uint16_t
 * @param transitionTime uint16_t
 * @param optionsMask uint8_t
 * @param optionsOverride uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandColorControlClusterMoveToColor(uint16_t colorX,
                                                 uint16_t colorY,
                                                 uint16_t transitionTime,
                                                 uint8_t optionsMask,
                                                 uint8_t optionsOverride,
                                                 frame_info_t *frame_info );
 
/** @brief Moves the color.
 *
 * Cluster: Color Control, Attributes and commands for controlling the color properties of a color-capable light.
 * Command: MoveColor
 * @param rateX int16_t
 * @param rateY int16_t
 * @param optionsMask uint8_t
 * @param optionsOverride uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandColorControlClusterMoveColor(int16_t rateX,
                                               int16_t rateY,
                                               uint8_t optionsMask,
                                               uint8_t optionsOverride,
                                               frame_info_t *frame_info );
 
/** @brief Steps the lighting to a specific color.
 *
 * Cluster: Color Control, Attributes and commands for controlling the color properties of a color-capable light.
 * Command: StepColor
 * @param stepX int16_t
 * @param stepY int16_t
 * @param transitionTime uint16_t
 * @param optionsMask uint8_t
 * @param optionsOverride uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandColorControlClusterStepColor(int16_t stepX,
                                               int16_t stepY,
                                               uint16_t transitionTime,
                                               uint8_t optionsMask,
                                               uint8_t optionsOverride,
                                               frame_info_t *frame_info );
 
/** @brief Move to a specific color temperature.
 *
 * Cluster: Color Control, Attributes and commands for controlling the color properties of a color-capable light.
 * Command: MoveToColorTemperature
 * @param colorTemperature uint16_t
 * @param transitionTime uint16_t
 * @param optionsMask uint8_t
 * @param optionsOverride uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandColorControlClusterMoveToColorTemperature(uint16_t colorTemperature,
                                                            uint16_t transitionTime,
                                                            uint8_t optionsMask,
                                                            uint8_t optionsOverride,
                                                            frame_info_t *frame_info );
 
/** @brief Command description for EnhancedMoveToHue
 *
 * Cluster: Color Control, Attributes and commands for controlling the color properties of a color-capable light.
 * Command: EnhancedMoveToHue
 * @param enhancedHue uint16_t
 * @param direction uint8_t
 * @param transitionTime uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandColorControlClusterEnhancedMoveToHue(uint16_t enhancedHue,
                                                       uint8_t direction,
                                                       uint16_t transitionTime,
                                                       frame_info_t *frame_info );
 
/** @brief Command description for EnhancedMoveHue
 *
 * Cluster: Color Control, Attributes and commands for controlling the color properties of a color-capable light.
 * Command: EnhancedMoveHue
 * @param moveMode uint8_t
 * @param rate uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandColorControlClusterEnhancedMoveHue(uint8_t moveMode,
                                                     uint16_t rate,
                                                     frame_info_t *frame_info );
 
/** @brief Command description for EnhancedStepHue
 *
 * Cluster: Color Control, Attributes and commands for controlling the color properties of a color-capable light.
 * Command: EnhancedStepHue
 * @param stepMode uint8_t
 * @param stepSize uint16_t
 * @param transitionTime uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandColorControlClusterEnhancedStepHue(uint8_t stepMode,
                                                     uint16_t stepSize,
                                                     uint16_t transitionTime,
                                                     frame_info_t *frame_info );
 
/** @brief Command description for EnhancedMoveToHueAndSaturation
 *
 * Cluster: Color Control, Attributes and commands for controlling the color properties of a color-capable light.
 * Command: EnhancedMoveToHueAndSaturation
 * @param enhancedHue uint16_t
 * @param saturation uint8_t
 * @param transitionTime uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandColorControlClusterEnhancedMoveToHueAndSaturation(uint16_t enhancedHue,
                                                                    uint8_t saturation,
                                                                    uint16_t transitionTime,
                                                                    frame_info_t *frame_info );
 
/** @brief Command description for ColorLoopSet
 *
 * Cluster: Color Control, Attributes and commands for controlling the color properties of a color-capable light.
 * Command: ColorLoopSet
 * @param updateFlags uint8_t
 * @param action uint8_t
 * @param direction uint8_t
 * @param time uint16_t
 * @param startHue uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandColorControlClusterColorLoopSet(uint8_t updateFlags,
                                                  uint8_t action,
                                                  uint8_t direction,
                                                  uint16_t time,
                                                  uint16_t startHue,
                                                  frame_info_t *frame_info );
 
/** @brief Command description for StopMoveStep
 *
 * Cluster: Color Control, Attributes and commands for controlling the color properties of a color-capable light.
 * Command: StopMoveStep
 * @param optionsMask uint8_t
 * @param optionsOverride uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandColorControlClusterStopMoveStep(uint8_t optionsMask,
                                                  uint8_t optionsOverride,
                                                  frame_info_t *frame_info );
 
/** @brief Command description for MoveColorTemperature
 *
 * Cluster: Color Control, Attributes and commands for controlling the color properties of a color-capable light.
 * Command: MoveColorTemperature
 * @param moveMode uint8_t
 * @param rate uint16_t
 * @param colorTemperatureMinimum uint16_t
 * @param colorTemperatureMaximum uint16_t
 * @param optionsMask uint8_t
 * @param optionsOverride uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandColorControlClusterMoveColorTemperature(uint8_t moveMode,
                                                          uint16_t rate,
                                                          uint16_t colorTemperatureMinimum,
                                                          uint16_t colorTemperatureMaximum,
                                                          uint8_t optionsMask,
                                                          uint8_t optionsOverride,
                                                          frame_info_t *frame_info );
 
/** @brief Command description for StepColorTemperature
 *
 * Cluster: Color Control, Attributes and commands for controlling the color properties of a color-capable light.
 * Command: StepColorTemperature
 * @param stepMode uint8_t
 * @param stepSize uint16_t
 * @param transitionTime uint16_t
 * @param colorTemperatureMinimum uint16_t
 * @param colorTemperatureMaximum uint16_t
 * @param optionsMask uint8_t
 * @param optionsOverride uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandColorControlClusterStepColorTemperature(uint8_t stepMode,
                                                          uint16_t stepSize,
                                                          uint16_t transitionTime,
                                                          uint16_t colorTemperatureMinimum,
                                                          uint16_t colorTemperatureMaximum,
                                                          uint8_t optionsMask,
                                                          uint8_t optionsOverride,
                                                          frame_info_t *frame_info );
 
/** @} END Color Control Commands */

/** @name Ballast Configuration Commands */
/** @} END Ballast Configuration Commands */

/** @name Illuminance Measurement Commands */
/** @} END Illuminance Measurement Commands */

/** @name Illuminance Level Sensing Commands */
/** @} END Illuminance Level Sensing Commands */

/** @name Temperature Measurement Commands */
/** @} END Temperature Measurement Commands */

/** @name Pressure Measurement Commands */
/** @} END Pressure Measurement Commands */

/** @name Flow Measurement Commands */
/** @} END Flow Measurement Commands */

/** @name Relative Humidity Measurement Commands */
/** @} END Relative Humidity Measurement Commands */

/** @name Occupancy Sensing Commands */
/** @} END Occupancy Sensing Commands */

/** @name Carbon Monoxide Concentration Measurement Commands */
/** @} END Carbon Monoxide Concentration Measurement Commands */

/** @name Carbon Dioxide Concentration Measurement Commands */
/** @} END Carbon Dioxide Concentration Measurement Commands */

/** @name Ethylene Concentration Measurement Commands */
/** @} END Ethylene Concentration Measurement Commands */

/** @name Ethylene Oxide Concentration Measurement Commands */
/** @} END Ethylene Oxide Concentration Measurement Commands */

/** @name Hydrogen Concentration Measurement Commands */
/** @} END Hydrogen Concentration Measurement Commands */

/** @name Hydrogen Sulphide Concentration Measurement Commands */
/** @} END Hydrogen Sulphide Concentration Measurement Commands */

/** @name Nitric Oxide Concentration Measurement Commands */
/** @} END Nitric Oxide Concentration Measurement Commands */

/** @name Nitrogen Dioxide Concentration Measurement Commands */
/** @} END Nitrogen Dioxide Concentration Measurement Commands */

/** @name Oxygen Concentration Measurement Commands */
/** @} END Oxygen Concentration Measurement Commands */

/** @name Ozone Concentration Measurement Commands */
/** @} END Ozone Concentration Measurement Commands */

/** @name Sulfur Dioxide Concentration Measurement Commands */
/** @} END Sulfur Dioxide Concentration Measurement Commands */

/** @name Dissolved Oxygen Concentration Measurement Commands */
/** @} END Dissolved Oxygen Concentration Measurement Commands */

/** @name Bromate Concentration Measurement Commands */
/** @} END Bromate Concentration Measurement Commands */

/** @name Chloramines Concentration Measurement Commands */
/** @} END Chloramines Concentration Measurement Commands */

/** @name Chlorine Concentration Measurement Commands */
/** @} END Chlorine Concentration Measurement Commands */

/** @name Fecal coliform and E. Coli Concentration Measurement Commands */
/** @} END Fecal coliform and E. Coli Concentration Measurement Commands */

/** @name Fluoride Concentration Measurement Commands */
/** @} END Fluoride Concentration Measurement Commands */

/** @name Haloacetic Acids Concentration Measurement Commands */
/** @} END Haloacetic Acids Concentration Measurement Commands */

/** @name Total Trihalomethanes Concentration Measurement Commands */
/** @} END Total Trihalomethanes Concentration Measurement Commands */

/** @name Total Coliform Bacteria Concentration Measurement Commands */
/** @} END Total Coliform Bacteria Concentration Measurement Commands */

/** @name Turbidity Concentration Measurement Commands */
/** @} END Turbidity Concentration Measurement Commands */

/** @name Copper Concentration Measurement Commands */
/** @} END Copper Concentration Measurement Commands */

/** @name Lead Concentration Measurement Commands */
/** @} END Lead Concentration Measurement Commands */

/** @name Manganese Concentration Measurement Commands */
/** @} END Manganese Concentration Measurement Commands */

/** @name Sulfate Concentration Measurement Commands */
/** @} END Sulfate Concentration Measurement Commands */

/** @name Bromodichloromethane Concentration Measurement Commands */
/** @} END Bromodichloromethane Concentration Measurement Commands */

/** @name Bromoform Concentration Measurement Commands */
/** @} END Bromoform Concentration Measurement Commands */

/** @name Chlorodibromomethane Concentration Measurement Commands */
/** @} END Chlorodibromomethane Concentration Measurement Commands */

/** @name Chloroform Concentration Measurement Commands */
/** @} END Chloroform Concentration Measurement Commands */

/** @name Sodium Concentration Measurement Commands */
/** @} END Sodium Concentration Measurement Commands */

/** @name IAS Zone Commands */
 
/** @brief Used to tell the IAS Zone server to commence normal operation mode
 *
 * Cluster: IAS Zone, Attributes and commands for IAS security zone devices.
 * Command: InitiateNormalOperationMode
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandIasZoneClusterInitiateNormalOperationMode(
    frame_info_t *frame_info );
 
/** @brief Certain IAS Zone servers may have operational configurations that could be configured OTA or locally on the device. This command enables them to be remotely placed into a test mode so that the user or installer may configure their field of view, sensitivity, and other operational parameters.
 *
 * Cluster: IAS Zone, Attributes and commands for IAS security zone devices.
 * Command: InitiateTestMode
 * @param testModeDuration uint8_t
 * @param currentZoneSensitivityLevel uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandIasZoneClusterInitiateTestMode(uint8_t testModeDuration,
                                                 uint8_t currentZoneSensitivityLevel,
                                                 frame_info_t *frame_info );
 
/** @brief Command description for zoneStatusChangeNotification
 *
 * Cluster: IAS Zone, Attributes and commands for IAS security zone devices.
 * Command: ZoneStatusChangeNotification
 * @param zoneStatus uint16_t
 * @param extendedStatus uint8_t
 * @param zoneId uint8_t
 * @param delay uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandIasZoneClusterZoneStatusChangeNotification(uint16_t zoneStatus,
                                                             uint8_t extendedStatus,
                                                             uint8_t zoneId,
                                                             uint16_t delay,
                                                             frame_info_t *frame_info );
 
/** @brief Command description for zoneEnrollRequest
 *
 * Cluster: IAS Zone, Attributes and commands for IAS security zone devices.
 * Command: ZoneEnrollRequest
 * @param zoneType uint16_t
 * @param manufacturerCode uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandIasZoneClusterZoneEnrollRequest(uint16_t zoneType,
                                                  uint16_t manufacturerCode,
                                                  frame_info_t *frame_info );
 
/** @brief Confirms that the IAS Zone server has commenced normal operation mode.
 *
 * Cluster: IAS Zone, Attributes and commands for IAS security zone devices.
 * Command: InitiateNormalOperationModeResponse
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandIasZoneClusterInitiateNormalOperationModeResponse(
    frame_info_t *frame_info );
 
/** @brief Confirms that the IAS Zone server has commenced test mode and that the IAS Zone client should treat any Zone Status Change Notification commands received from the sending IAS Zone server as being in response to test events.
 *
 * Cluster: IAS Zone, Attributes and commands for IAS security zone devices.
 * Command: InitiateTestModeResponse
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandIasZoneClusterInitiateTestModeResponse(
    frame_info_t *frame_info );
 
/** @} END IAS Zone Commands */

/** @name IAS ACE Commands */
 
/** @brief Command description for Bypass
 *
 * Cluster: IAS ACE, Attributes and commands for IAS Ancillary Control Equipment.
 * Command: Bypass
 * @param numberOfZones uint8_t
 * @param zoneIds uint8_t*
 * @param zoneIdsLen uint16_t
 * @param armDisarmCode uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandIasAceClusterBypass(uint8_t numberOfZones,
                                      uint8_t *zoneIds,
                                      uint16_t zoneIdsLen,
                                      uint8_t *armDisarmCode,
                                      frame_info_t *frame_info );
 
/** @brief Command description for Emergency
 *
 * Cluster: IAS ACE, Attributes and commands for IAS Ancillary Control Equipment.
 * Command: Emergency
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandIasAceClusterEmergency(
    frame_info_t *frame_info );
 
/** @brief Command description for Fire
 *
 * Cluster: IAS ACE, Attributes and commands for IAS Ancillary Control Equipment.
 * Command: Fire
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandIasAceClusterFire(
    frame_info_t *frame_info );
 
/** @brief Command description for Panic
 *
 * Cluster: IAS ACE, Attributes and commands for IAS Ancillary Control Equipment.
 * Command: Panic
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandIasAceClusterPanic(
    frame_info_t *frame_info );
 
/** @brief Command description for GetZoneIdMap
 *
 * Cluster: IAS ACE, Attributes and commands for IAS Ancillary Control Equipment.
 * Command: GetZoneIdMap
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandIasAceClusterGetZoneIdMap(
    frame_info_t *frame_info );
 
/** @brief Command description for GetZoneInformation
 *
 * Cluster: IAS ACE, Attributes and commands for IAS Ancillary Control Equipment.
 * Command: GetZoneInformation
 * @param zoneId uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandIasAceClusterGetZoneInformation(uint8_t zoneId,
                                                  frame_info_t *frame_info );
 
/** @brief Used by the ACE client to request an update to the status of the ACE server
 *
 * Cluster: IAS ACE, Attributes and commands for IAS Ancillary Control Equipment.
 * Command: GetPanelStatus
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandIasAceClusterGetPanelStatus(
    frame_info_t *frame_info );
 
/** @brief Used by the ACE client to retrieve the bypassed zones
 *
 * Cluster: IAS ACE, Attributes and commands for IAS Ancillary Control Equipment.
 * Command: GetBypassedZoneList
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandIasAceClusterGetBypassedZoneList(
    frame_info_t *frame_info );
 
/** @brief Used by the ACE client to request an update to the zone status of the ACE server
 *
 * Cluster: IAS ACE, Attributes and commands for IAS Ancillary Control Equipment.
 * Command: GetZoneStatus
 * @param startingZoneId uint8_t
 * @param maxNumberOfZoneIds uint8_t
 * @param zoneStatusMaskFlag uint8_t
 * @param zoneStatusMask uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandIasAceClusterGetZoneStatus(uint8_t startingZoneId,
                                             uint8_t maxNumberOfZoneIds,
                                             uint8_t zoneStatusMaskFlag,
                                             uint16_t zoneStatusMask,
                                             frame_info_t *frame_info );
 
/** @brief Command description for ArmResponse
 *
 * Cluster: IAS ACE, Attributes and commands for IAS Ancillary Control Equipment.
 * Command: ArmResponse
 * @param armNotification uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandIasAceClusterArmResponse(uint8_t armNotification,
                                           frame_info_t *frame_info );
 
/** @brief Command description for GetZoneIdMapResponse
 *
 * Cluster: IAS ACE, Attributes and commands for IAS Ancillary Control Equipment.
 * Command: GetZoneIdMapResponse
 * @param section0 uint16_t
 * @param section1 uint16_t
 * @param section2 uint16_t
 * @param section3 uint16_t
 * @param section4 uint16_t
 * @param section5 uint16_t
 * @param section6 uint16_t
 * @param section7 uint16_t
 * @param section8 uint16_t
 * @param section9 uint16_t
 * @param section10 uint16_t
 * @param section11 uint16_t
 * @param section12 uint16_t
 * @param section13 uint16_t
 * @param section14 uint16_t
 * @param section15 uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandIasAceClusterGetZoneIdMapResponse(uint16_t section0,
                                                    uint16_t section1,
                                                    uint16_t section2,
                                                    uint16_t section3,
                                                    uint16_t section4,
                                                    uint16_t section5,
                                                    uint16_t section6,
                                                    uint16_t section7,
                                                    uint16_t section8,
                                                    uint16_t section9,
                                                    uint16_t section10,
                                                    uint16_t section11,
                                                    uint16_t section12,
                                                    uint16_t section13,
                                                    uint16_t section14,
                                                    uint16_t section15,
                                                    frame_info_t *frame_info );
 
/** @brief Command description for GetZoneInformationResponse
 *
 * Cluster: IAS ACE, Attributes and commands for IAS Ancillary Control Equipment.
 * Command: GetZoneInformationResponse
 * @param zoneId uint8_t
 * @param zoneType uint16_t
 * @param ieeeAddress uint8_t*
 * @param zoneLabel uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandIasAceClusterGetZoneInformationResponse(uint8_t zoneId,
                                                          uint16_t zoneType,
                                                          uint8_t *ieeeAddress,
                                                          uint8_t *zoneLabel,
                                                          frame_info_t *frame_info );
 
/** @brief This command updates ACE clients in the system of changes to zone status recorded by the ACE server (e.g., IAS CIE device );.
 *
 * Cluster: IAS ACE, Attributes and commands for IAS Ancillary Control Equipment.
 * Command: ZoneStatusChanged
 * @param zoneId uint8_t
 * @param zoneStatus uint16_t
 * @param audibleNotification uint8_t
 * @param zoneLabel uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandIasAceClusterZoneStatusChanged(uint8_t zoneId,
                                                 uint16_t zoneStatus,
                                                 uint8_t audibleNotification,
                                                 uint8_t *zoneLabel,
                                                 frame_info_t *frame_info );
 
/** @brief This command updates ACE clients in the system of changes to panel status recorded by the ACE server (e.g., IAS CIE device );.
 *
 * Cluster: IAS ACE, Attributes and commands for IAS Ancillary Control Equipment.
 * Command: PanelStatusChanged
 * @param panelStatus uint8_t
 * @param secondsRemaining uint8_t
 * @param audibleNotification uint8_t
 * @param alarmStatus uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandIasAceClusterPanelStatusChanged(uint8_t panelStatus,
                                                  uint8_t secondsRemaining,
                                                  uint8_t audibleNotification,
                                                  uint8_t alarmStatus,
                                                  frame_info_t *frame_info );
 
/** @brief Command updates requesting IAS ACE clients in the system of changes to the security panel status recorded by the ACE server.
 *
 * Cluster: IAS ACE, Attributes and commands for IAS Ancillary Control Equipment.
 * Command: GetPanelStatusResponse
 * @param panelStatus uint8_t
 * @param secondsRemaining uint8_t
 * @param audibleNotification uint8_t
 * @param alarmStatus uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandIasAceClusterGetPanelStatusResponse(uint8_t panelStatus,
                                                      uint8_t secondsRemaining,
                                                      uint8_t audibleNotification,
                                                      uint8_t alarmStatus,
                                                      frame_info_t *frame_info );
 
/** @brief Sets the list of bypassed zones on the IAS ACE client
 *
 * Cluster: IAS ACE, Attributes and commands for IAS Ancillary Control Equipment.
 * Command: SetBypassedZoneList
 * @param numberOfZones uint8_t
 * @param zoneIds uint8_t*
 * @param zoneIdsLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandIasAceClusterSetBypassedZoneList(uint8_t numberOfZones,
                                                   uint8_t *zoneIds,
                                                   uint16_t zoneIdsLen,
                                                   frame_info_t *frame_info );
 
/** @brief Provides the response of the security panel to the request from the IAS ACE client to bypass zones via a Bypass command.
 *
 * Cluster: IAS ACE, Attributes and commands for IAS Ancillary Control Equipment.
 * Command: BypassResponse
 * @param numberOfZones uint8_t
 * @param bypassResult uint8_t*
 * @param bypassResultLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandIasAceClusterBypassResponse(uint8_t numberOfZones,
                                              uint8_t *bypassResult,
                                              uint16_t bypassResultLen,
                                              frame_info_t *frame_info );
 
/** @brief This command updates requesting IAS ACE clients in the system of changes to the IAS Zone server statuses recorded by the ACE server (e.g., IAS CIE device );.
 *
 * Cluster: IAS ACE, Attributes and commands for IAS Ancillary Control Equipment.
 * Command: GetZoneStatusResponse
 * @param zoneStatusComplete uint8_t
 * @param numberOfZones uint8_t
 * @param zoneStatusResult uint8_t*
 * @param zoneStatusResultLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandIasAceClusterGetZoneStatusResponse(uint8_t zoneStatusComplete,
                                                     uint8_t numberOfZones,
                                                     uint8_t *zoneStatusResult,
                                                     uint16_t zoneStatusResultLen,
                                                     frame_info_t *frame_info );
 
/** @} END IAS ACE Commands */

/** @name IAS WD Commands */
 
/** @brief Command description for Squawk
 *
 * Cluster: IAS WD, Attributes and commands for IAS Warning Devices.
 * Command: Squawk
 * @param squawkInfo uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandIasWdClusterSquawk(uint8_t squawkInfo,
                                     frame_info_t *frame_info );
 
/** @} END IAS WD Commands */

/** @name Generic Tunnel Commands */
 
/** @brief This command is generated upon receipt of a Match Protocol Address command to indicate that the Protocol Address was successfully matched.
 *
 * Cluster: Generic Tunnel, The minimum common commands and attributes required to tunnel any protocol.
 * Command: MatchProtocolAddressResponse
 * @param deviceIeeeAddress uint8_t*
 * @param protocolAddress uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandGenericTunnelClusterMatchProtocolAddressResponse(uint8_t *deviceIeeeAddress,
                                                                   uint8_t *protocolAddress,
                                                                   frame_info_t *frame_info );
 
/** @brief This command is typically sent upon startup, and whenever the ProtocolAddress attribute changes. It is typically multicast to a group of inter-communicating Generic Tunnel clusters.
 *
 * Cluster: Generic Tunnel, The minimum common commands and attributes required to tunnel any protocol.
 * Command: AdvertiseProtocolAddress
 * @param protocolAddress uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandGenericTunnelClusterAdvertiseProtocolAddress(uint8_t *protocolAddress,
                                                               frame_info_t *frame_info );
 
/** @} END Generic Tunnel Commands */

/** @name BACnet Protocol Tunnel Commands */
 
/** @} END BACnet Protocol Tunnel Commands */

/** @name 11073 Protocol Tunnel Commands */
 
/** @brief This command is generated when an Health Care client wishes to connect to a Health Care server for the purposes of transmitting 11073 APDUs across the 11073 tunnel.
 *
 * Cluster: 11073 Protocol Tunnel, Attributes and commands for the 11073 protocol tunnel used for ZigBee Health Care.
 * Command: ConnectRequest
 * @param connectControl uint8_t
 * @param idleTimeout uint16_t
 * @param managerTarget uint8_t*
 * @param managerEndpoint uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommand11073ProtocolTunnelClusterConnectRequest(uint8_t connectControl,
                                                           uint16_t idleTimeout,
                                                           uint8_t *managerTarget,
                                                           uint8_t managerEndpoint,
                                                           frame_info_t *frame_info );
 
/** @brief This command is generated when an Health Care client wishes to disconnect from a Health Care server.
 *
 * Cluster: 11073 Protocol Tunnel, Attributes and commands for the 11073 protocol tunnel used for ZigBee Health Care.
 * Command: DisconnectRequest
 * @param managerIEEEAddress uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommand11073ProtocolTunnelClusterDisconnectRequest(uint8_t *managerIEEEAddress,
                                                              frame_info_t *frame_info );
 
/** @brief Generated in response to requests related to connection or any event that causes the tunnel to become disconnected.
 *
 * Cluster: 11073 Protocol Tunnel, Attributes and commands for the 11073 protocol tunnel used for ZigBee Health Care.
 * Command: ConnectStatusNotification
 * @param connectStatus uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommand11073ProtocolTunnelClusterConnectStatusNotification(uint8_t connectStatus,
                                                                      frame_info_t *frame_info );
 
/** @} END 11073 Protocol Tunnel Commands */

/** @name ISO 7816 Protocol Tunnel Commands */
 
/** @brief Command description for TransferApdu
 *
 * Cluster: ISO 7816 Protocol Tunnel, Commands and attributes for mobile office solutions including ZigBee devices.
 * Command: TransferApdu
 * @param apdu uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandIso7816ProtocolTunnelClusterClientToServerTransferApdu(uint8_t *apdu,
                                                                         frame_info_t *frame_info );
 
/** @brief Command description for InsertSmartCard
 *
 * Cluster: ISO 7816 Protocol Tunnel, Commands and attributes for mobile office solutions including ZigBee devices.
 * Command: InsertSmartCard
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandIso7816ProtocolTunnelClusterInsertSmartCard(
    frame_info_t *frame_info );
 
/** @brief Command description for ExtractSmartCard
 *
 * Cluster: ISO 7816 Protocol Tunnel, Commands and attributes for mobile office solutions including ZigBee devices.
 * Command: ExtractSmartCard
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandIso7816ProtocolTunnelClusterExtractSmartCard(
    frame_info_t *frame_info );
 
/** @} END ISO 7816 Protocol Tunnel Commands */

/** @name Price Commands */
 
/** @brief The PublishBlockPeriod command is generated in response to receiving a GetBlockPeriod(s ); command or when an update to the block tariff schedule is available from the commodity provider.
 *
 * Cluster: Price, The Price Cluster provides the mechanism for communicating Gas, Energy, or Water pricing information within the premises.
 * Command: PublishBlockPeriod
 * @param providerId uint32_t
 * @param issuerEventId uint32_t
 * @param blockPeriodStartTime uint32_t
 * @param blockPeriodDuration uint32_t
 * @param blockPeriodControl uint8_t
 * @param blockPeriodDurationType uint8_t
 * @param tariffType uint8_t
 * @param tariffResolutionPeriod uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPriceClusterPublishBlockPeriod(uint32_t providerId,
                                                 uint32_t issuerEventId,
                                                 uint32_t blockPeriodStartTime,
                                                 uint32_t blockPeriodDuration,
                                                 uint8_t blockPeriodControl,
                                                 uint8_t blockPeriodDurationType,
                                                 uint8_t tariffType,
                                                 uint8_t tariffResolutionPeriod,
                                                 frame_info_t *frame_info );
 
/** @brief The PublishConversionFactor command is sent in response to a GetConversionFactor command or if a new Conversion factor is available.
 *
 * Cluster: Price, The Price Cluster provides the mechanism for communicating Gas, Energy, or Water pricing information within the premises.
 * Command: PublishConversionFactor
 * @param issuerEventId uint32_t
 * @param startTime uint32_t
 * @param conversionFactor uint32_t
 * @param conversionFactorTrailingDigit uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPriceClusterPublishConversionFactor(uint32_t issuerEventId,
                                                      uint32_t startTime,
                                                      uint32_t conversionFactor,
                                                      uint8_t conversionFactorTrailingDigit,
                                                      frame_info_t *frame_info );
 
/** @brief The PublishCalorificValue command is sent in response to a GetCalorificValue command or if a new calorific value is available.
 *
 * Cluster: Price, The Price Cluster provides the mechanism for communicating Gas, Energy, or Water pricing information within the premises.
 * Command: PublishCalorificValue
 * @param issuerEventId uint32_t
 * @param startTime uint32_t
 * @param calorificValue uint32_t
 * @param calorificValueUnit uint8_t
 * @param calorificValueTrailingDigit uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPriceClusterPublishCalorificValue(uint32_t issuerEventId,
                                                    uint32_t startTime,
                                                    uint32_t calorificValue,
                                                    uint8_t calorificValueUnit,
                                                    uint8_t calorificValueTrailingDigit,
                                                    frame_info_t *frame_info );
 
/** @brief The PublishTariffInformation command is sent in response to a GetTariffInformation command or if new tariff information is available (including price matrix and block thresholds );.
 *
 * Cluster: Price, The Price Cluster provides the mechanism for communicating Gas, Energy, or Water pricing information within the premises.
 * Command: PublishTariffInformation
 * @param providerId uint32_t
 * @param issuerEventId uint32_t
 * @param issuerTariffId uint32_t
 * @param startTime uint32_t
 * @param tariffTypeChargingScheme uint8_t
 * @param tariffLabel uint8_t*
 * @param numberOfPriceTiersInUse uint8_t
 * @param numberOfBlockThresholdsInUse uint8_t
 * @param unitOfMeasure uint8_t
 * @param currency uint16_t
 * @param priceTrailingDigit uint8_t
 * @param standingCharge uint32_t
 * @param tierBlockMode uint8_t
 * @param blockThresholdMultiplier uint32_t
 * @param blockThresholdDivisor uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPriceClusterPublishTariffInformation(uint32_t providerId,
                                                       uint32_t issuerEventId,
                                                       uint32_t issuerTariffId,
                                                       uint32_t startTime,
                                                       uint8_t tariffTypeChargingScheme,
                                                       uint8_t *tariffLabel,
                                                       uint8_t numberOfPriceTiersInUse,
                                                       uint8_t numberOfBlockThresholdsInUse,
                                                       uint8_t unitOfMeasure,
                                                       uint16_t currency,
                                                       uint8_t priceTrailingDigit,
                                                       uint32_t standingCharge,
                                                       uint8_t tierBlockMode,
                                                       uint32_t blockThresholdMultiplier,
                                                       uint32_t blockThresholdDivisor,
                                                       frame_info_t *frame_info );
 
/** @brief PublishPriceMatrix command is used to publish the Block Price Information Set (up to 15 tiers x 15 blocks ); and the Extended Price Information Set (up to 48 tiers );.  The PublishPriceMatrix command is sent in response to a GetPriceMatrix command.
 *
 * Cluster: Price, The Price Cluster provides the mechanism for communicating Gas, Energy, or Water pricing information within the premises.
 * Command: PublishPriceMatrix
 * @param providerId uint32_t
 * @param issuerEventId uint32_t
 * @param startTime uint32_t
 * @param issuerTariffId uint32_t
 * @param commandIndex uint8_t
 * @param numberOfCommands uint8_t
 * @param subPayloadControl uint8_t
 * @param payload uint8_t*
 * @param payloadLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPriceClusterPublishPriceMatrix(uint32_t providerId,
                                                 uint32_t issuerEventId,
                                                 uint32_t startTime,
                                                 uint32_t issuerTariffId,
                                                 uint8_t commandIndex,
                                                 uint8_t numberOfCommands,
                                                 uint8_t subPayloadControl,
                                                 uint8_t *payload,
                                                 uint16_t payloadLen,
                                                 frame_info_t *frame_info );
 
/** @brief The PublishBlockThreshold command is sent in response to a GetBlockThreshold command.
 *
 * Cluster: Price, The Price Cluster provides the mechanism for communicating Gas, Energy, or Water pricing information within the premises.
 * Command: PublishBlockThresholds
 * @param providerId uint32_t
 * @param issuerEventId uint32_t
 * @param startTime uint32_t
 * @param issuerTariffId uint32_t
 * @param commandIndex uint8_t
 * @param numberOfCommands uint8_t
 * @param subPayloadControl uint8_t
 * @param payload uint8_t*
 * @param payloadLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPriceClusterPublishBlockThresholds(uint32_t providerId,
                                                     uint32_t issuerEventId,
                                                     uint32_t startTime,
                                                     uint32_t issuerTariffId,
                                                     uint8_t commandIndex,
                                                     uint8_t numberOfCommands,
                                                     uint8_t subPayloadControl,
                                                     uint8_t *payload,
                                                     uint16_t payloadLen,
                                                     frame_info_t *frame_info );
 
/** @brief The PublishCO2Value command is sent in response to a GetCO2Value command or if a new CO2 conversion factor is available.
 *
 * Cluster: Price, The Price Cluster provides the mechanism for communicating Gas, Energy, or Water pricing information within the premises.
 * Command: PublishCO2Value
 * @param providerId uint32_t
 * @param issuerEventId uint32_t
 * @param startTime uint32_t
 * @param tariffType uint8_t
 * @param cO2Value uint32_t
 * @param cO2ValueUnit uint8_t
 * @param cO2ValueTrailingDigit uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPriceClusterPublishCO2Value(uint32_t providerId,
                                              uint32_t issuerEventId,
                                              uint32_t startTime,
                                              uint8_t tariffType,
                                              uint32_t cO2Value,
                                              uint8_t cO2ValueUnit,
                                              uint8_t cO2ValueTrailingDigit,
                                              frame_info_t *frame_info );
 
/** @brief The PublishTierLabels command is generated in response to receiving a GetTierLabels command or when there is a tier label change.
 *
 * Cluster: Price, The Price Cluster provides the mechanism for communicating Gas, Energy, or Water pricing information within the premises.
 * Command: PublishTierLabels
 * @param providerId uint32_t
 * @param issuerEventId uint32_t
 * @param issuerTariffId uint32_t
 * @param commandIndex uint8_t
 * @param numberOfCommands uint8_t
 * @param numberOfLabels uint8_t
 * @param tierLabelsPayload uint8_t*
 * @param tierLabelsPayloadLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPriceClusterPublishTierLabels(uint32_t providerId,
                                                uint32_t issuerEventId,
                                                uint32_t issuerTariffId,
                                                uint8_t commandIndex,
                                                uint8_t numberOfCommands,
                                                uint8_t numberOfLabels,
                                                uint8_t *tierLabelsPayload,
                                                uint16_t tierLabelsPayloadLen,
                                                frame_info_t *frame_info );
 
/** @brief The PublishBillingPeriod command is generated in response to receiving a GetBillingPeriod(s ); command or when an update to the Billing schedule is available from the commodity Supplier.
 *
 * Cluster: Price, The Price Cluster provides the mechanism for communicating Gas, Energy, or Water pricing information within the premises.
 * Command: PublishBillingPeriod
 * @param providerId uint32_t
 * @param issuerEventId uint32_t
 * @param billingPeriodStartTime uint32_t
 * @param billingPeriodDuration uint32_t
 * @param billingPeriodDurationType uint8_t
 * @param tariffType uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPriceClusterPublishBillingPeriod(uint32_t providerId,
                                                   uint32_t issuerEventId,
                                                   uint32_t billingPeriodStartTime,
                                                   uint32_t billingPeriodDuration,
                                                   uint8_t billingPeriodDurationType,
                                                   uint8_t tariffType,
                                                   frame_info_t *frame_info );
 
/** @brief The PublishConsolidatedBill command is used to make consolidated billing information of previous billing periods available to other end devices.  This command is issued in response to a GetConsolidatedBill command or if new billing information is available.
 *
 * Cluster: Price, The Price Cluster provides the mechanism for communicating Gas, Energy, or Water pricing information within the premises.
 * Command: PublishConsolidatedBill
 * @param providerId uint32_t
 * @param issuerEventId uint32_t
 * @param billingPeriodStartTime uint32_t
 * @param billingPeriodDuration uint32_t
 * @param billingPeriodDurationType uint8_t
 * @param tariffType uint8_t
 * @param consolidatedBill uint32_t
 * @param currency uint16_t
 * @param billTrailingDigit uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPriceClusterPublishConsolidatedBill(uint32_t providerId,
                                                      uint32_t issuerEventId,
                                                      uint32_t billingPeriodStartTime,
                                                      uint32_t billingPeriodDuration,
                                                      uint8_t billingPeriodDurationType,
                                                      uint8_t tariffType,
                                                      uint32_t consolidatedBill,
                                                      uint16_t currency,
                                                      uint8_t billTrailingDigit,
                                                      frame_info_t *frame_info );
 
/** @brief The PublishCPPEvent command is sent from an ESI to its price clients to notify them of a Critical Peak Pricing event.
 *
 * Cluster: Price, The Price Cluster provides the mechanism for communicating Gas, Energy, or Water pricing information within the premises.
 * Command: PublishCppEvent
 * @param providerId uint32_t
 * @param issuerEventId uint32_t
 * @param startTime uint32_t
 * @param durationInMinutes uint16_t
 * @param tariffType uint8_t
 * @param cppPriceTier uint8_t
 * @param cppAuth uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPriceClusterPublishCppEvent(uint32_t providerId,
                                              uint32_t issuerEventId,
                                              uint32_t startTime,
                                              uint16_t durationInMinutes,
                                              uint8_t tariffType,
                                              uint8_t cppPriceTier,
                                              uint8_t cppAuth,
                                              frame_info_t *frame_info );
 
/** @brief The PublishCreditPayment command is used to update the credit payment information is available.
 *
 * Cluster: Price, The Price Cluster provides the mechanism for communicating Gas, Energy, or Water pricing information within the premises.
 * Command: PublishCreditPayment
 * @param providerId uint32_t
 * @param issuerEventId uint32_t
 * @param creditPaymentDueDate uint32_t
 * @param creditPaymentOverDueAmount uint32_t
 * @param creditPaymentStatus uint8_t
 * @param creditPayment uint32_t
 * @param creditPaymentDate uint32_t
 * @param creditPaymentRef uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPriceClusterPublishCreditPayment(uint32_t providerId,
                                                   uint32_t issuerEventId,
                                                   uint32_t creditPaymentDueDate,
                                                   uint32_t creditPaymentOverDueAmount,
                                                   uint8_t creditPaymentStatus,
                                                   uint32_t creditPayment,
                                                   uint32_t creditPaymentDate,
                                                   uint8_t *creditPaymentRef,
                                                   frame_info_t *frame_info );
 
/** @brief The PublishCurrencyConversion command is sent in response to a GetCurrencyConversion command or when a new currency becomes available.
 *
 * Cluster: Price, The Price Cluster provides the mechanism for communicating Gas, Energy, or Water pricing information within the premises.
 * Command: PublishCurrencyConversion
 * @param providerId uint32_t
 * @param issuerEventId uint32_t
 * @param startTime uint32_t
 * @param oldCurrency uint16_t
 * @param newCurrency uint16_t
 * @param conversionFactor uint32_t
 * @param conversionFactorTrailingDigit uint8_t
 * @param currencyChangeControlFlags uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPriceClusterPublishCurrencyConversion(uint32_t providerId,
                                                        uint32_t issuerEventId,
                                                        uint32_t startTime,
                                                        uint16_t oldCurrency,
                                                        uint16_t newCurrency,
                                                        uint32_t conversionFactor,
                                                        uint8_t conversionFactorTrailingDigit,
                                                        uint32_t currencyChangeControlFlags,
                                                        frame_info_t *frame_info );
 
/** @brief The CancelTariff command indicates that all data associated with a particular tariff instance should be discarded.
 *
 * Cluster: Price, The Price Cluster provides the mechanism for communicating Gas, Energy, or Water pricing information within the premises.
 * Command: CancelTariff
 * @param providerId uint32_t
 * @param issuerTariffId uint32_t
 * @param tariffType uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPriceClusterCancelTariff(uint32_t providerId,
                                           uint32_t issuerTariffId,
                                           uint8_t tariffType,
                                           frame_info_t *frame_info );
 
/** @brief The GetCurrentPrice command initiates a PublishPrice command for the current time.
 *
 * Cluster: Price, The Price Cluster provides the mechanism for communicating Gas, Energy, or Water pricing information within the premises.
 * Command: GetCurrentPrice
 * @param commandOptions uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPriceClusterGetCurrentPrice(uint8_t commandOptions,
                                              frame_info_t *frame_info );
 
/** @brief The GetScheduledPrices command initiates a PublishPrice command for available price events.
 *
 * Cluster: Price, The Price Cluster provides the mechanism for communicating Gas, Energy, or Water pricing information within the premises.
 * Command: GetScheduledPrices
 * @param startTime uint32_t
 * @param numberOfEvents uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPriceClusterGetScheduledPrices(uint32_t startTime,
                                                 uint8_t numberOfEvents,
                                                 frame_info_t *frame_info );
 
/** @brief The PriceAcknowledgement command described provides the ability to acknowledge a previously sent PublishPrice command.
 *
 * Cluster: Price, The Price Cluster provides the mechanism for communicating Gas, Energy, or Water pricing information within the premises.
 * Command: PriceAcknowledgement
 * @param providerId uint32_t
 * @param issuerEventId uint32_t
 * @param priceAckTime uint32_t
 * @param control uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPriceClusterPriceAcknowledgement(uint32_t providerId,
                                                   uint32_t issuerEventId,
                                                   uint32_t priceAckTime,
                                                   uint8_t control,
                                                   frame_info_t *frame_info );
 
/** @brief The GetBlockPeriods command initiates a PublishBlockPeriod command for the currently scheduled block periods.
 *
 * Cluster: Price, The Price Cluster provides the mechanism for communicating Gas, Energy, or Water pricing information within the premises.
 * Command: GetBlockPeriods
 * @param startTime uint32_t
 * @param numberOfEvents uint8_t
 * @param tariffType uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPriceClusterGetBlockPeriods(uint32_t startTime,
                                              uint8_t numberOfEvents,
                                              uint8_t tariffType,
                                              frame_info_t *frame_info );
 
/** @brief The GetConversionFactor command initiates a PublishConversionFactor command for the scheduled conversion factor updates.
 *
 * Cluster: Price, The Price Cluster provides the mechanism for communicating Gas, Energy, or Water pricing information within the premises.
 * Command: GetConversionFactor
 * @param earliestStartTime uint32_t
 * @param minIssuerEventId uint32_t
 * @param numberOfCommands uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPriceClusterGetConversionFactor(uint32_t earliestStartTime,
                                                  uint32_t minIssuerEventId,
                                                  uint8_t numberOfCommands,
                                                  frame_info_t *frame_info );
 
/** @brief The GetCalorificValue command initiates a PublishCalorificValue command for the scheduled conversion factor updates.
 *
 * Cluster: Price, The Price Cluster provides the mechanism for communicating Gas, Energy, or Water pricing information within the premises.
 * Command: GetCalorificValue
 * @param earliestStartTime uint32_t
 * @param minIssuerEventId uint32_t
 * @param numberOfCommands uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPriceClusterGetCalorificValue(uint32_t earliestStartTime,
                                                uint32_t minIssuerEventId,
                                                uint8_t numberOfCommands,
                                                frame_info_t *frame_info );
 
/** @brief The GetTariffInformation command initiates a PublishTariffInformation command for the scheduled tariff updates.
 *
 * Cluster: Price, The Price Cluster provides the mechanism for communicating Gas, Energy, or Water pricing information within the premises.
 * Command: GetTariffInformation
 * @param earliestStartTime uint32_t
 * @param minIssuerEventId uint32_t
 * @param numberOfCommands uint8_t
 * @param tariffType uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPriceClusterGetTariffInformation(uint32_t earliestStartTime,
                                                   uint32_t minIssuerEventId,
                                                   uint8_t numberOfCommands,
                                                   uint8_t tariffType,
                                                   frame_info_t *frame_info );
 
/** @brief The GetPriceMatrix command initiates a PublishPriceMatrix command for the scheduled Price Matrix updates.
 *
 * Cluster: Price, The Price Cluster provides the mechanism for communicating Gas, Energy, or Water pricing information within the premises.
 * Command: GetPriceMatrix
 * @param issuerTariffId uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPriceClusterGetPriceMatrix(uint32_t issuerTariffId,
                                             frame_info_t *frame_info );
 
/** @brief The GetBlockThresholds command initiates a PublishBlockThreshold command for the scheduled Block Threshold updates.
 *
 * Cluster: Price, The Price Cluster provides the mechanism for communicating Gas, Energy, or Water pricing information within the premises.
 * Command: GetBlockThresholds
 * @param issuerTariffId uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPriceClusterGetBlockThresholds(uint32_t issuerTariffId,
                                                 frame_info_t *frame_info );
 
/** @brief The GetCO2Value command initiates a PublishCO2Value command for the scheduled CO2 conversion factor updates.
 *
 * Cluster: Price, The Price Cluster provides the mechanism for communicating Gas, Energy, or Water pricing information within the premises.
 * Command: GetCO2Value
 * @param earliestStartTime uint32_t
 * @param minIssuerEventId uint32_t
 * @param numberOfCommands uint8_t
 * @param tariffType uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPriceClusterGetCO2Value(uint32_t earliestStartTime,
                                          uint32_t minIssuerEventId,
                                          uint8_t numberOfCommands,
                                          uint8_t tariffType,
                                          frame_info_t *frame_info );
 
/** @brief The GetTierLabels command allows a client to retrieve the tier labels associated with a given tariff; this command initiates a PublishTierLabels command from the server.
 *
 * Cluster: Price, The Price Cluster provides the mechanism for communicating Gas, Energy, or Water pricing information within the premises.
 * Command: GetTierLabels
 * @param issuerTariffId uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPriceClusterGetTierLabels(uint32_t issuerTariffId,
                                            frame_info_t *frame_info );
 
/** @brief The GetBillingPeriod command initiates one or more PublishBillingPeriod commands for the currently scheduled billing periods.
 *
 * Cluster: Price, The Price Cluster provides the mechanism for communicating Gas, Energy, or Water pricing information within the premises.
 * Command: GetBillingPeriod
 * @param earliestStartTime uint32_t
 * @param minIssuerEventId uint32_t
 * @param numberOfCommands uint8_t
 * @param tariffType uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPriceClusterGetBillingPeriod(uint32_t earliestStartTime,
                                               uint32_t minIssuerEventId,
                                               uint8_t numberOfCommands,
                                               uint8_t tariffType,
                                               frame_info_t *frame_info );
 
/** @brief The GetConsolidatedBill command initiates one or more PublishConsolidatedBill commands with the requested billing information.
 *
 * Cluster: Price, The Price Cluster provides the mechanism for communicating Gas, Energy, or Water pricing information within the premises.
 * Command: GetConsolidatedBill
 * @param earliestStartTime uint32_t
 * @param minIssuerEventId uint32_t
 * @param numberOfCommands uint8_t
 * @param tariffType uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPriceClusterGetConsolidatedBill(uint32_t earliestStartTime,
                                                  uint32_t minIssuerEventId,
                                                  uint8_t numberOfCommands,
                                                  uint8_t tariffType,
                                                  frame_info_t *frame_info );
 
/** @brief The CPPEventResponse command is sent from a Client (IHD ); to the ESI to notify it of a Critical Peak Pricing event authorization.
 *
 * Cluster: Price, The Price Cluster provides the mechanism for communicating Gas, Energy, or Water pricing information within the premises.
 * Command: CppEventResponse
 * @param issuerEventId uint32_t
 * @param cppAuth uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPriceClusterCppEventResponse(uint32_t issuerEventId,
                                               uint8_t cppAuth,
                                               frame_info_t *frame_info );
 
/** @brief The GetCreditPayment command initiates PublishCreditPayment commands for the requested credit payment information.
 *
 * Cluster: Price, The Price Cluster provides the mechanism for communicating Gas, Energy, or Water pricing information within the premises.
 * Command: GetCreditPayment
 * @param latestEndTime uint32_t
 * @param numberOfRecords uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPriceClusterGetCreditPayment(uint32_t latestEndTime,
                                               uint8_t numberOfRecords,
                                               frame_info_t *frame_info );
 
/** @brief The GetCurrencyConversionCommand command initiates a PublishCurrencyConversion command for the currency conversion factor updates. A server shall be capable of storing both the old and the new currencies.
 *
 * Cluster: Price, The Price Cluster provides the mechanism for communicating Gas, Energy, or Water pricing information within the premises.
 * Command: GetCurrencyConversionCommand
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPriceClusterGetCurrencyConversionCommand(
    frame_info_t *frame_info );
 
/** @brief The GetTariffCancellation command initiates the 
	return of the last CancelTariff command held on the associated server.
 *
 * Cluster: Price, The Price Cluster provides the mechanism for communicating Gas, Energy, or Water pricing information within the premises.
 * Command: GetTariffCancellation
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPriceClusterGetTariffCancellation(
    frame_info_t *frame_info );
 
/** @} END Price Commands */

/** @name Demand Response and Load Control Commands */
 
/** @brief Command description for CancelLoadControlEvent
 *
 * Cluster: Demand Response and Load Control, This cluster provides an interface to the functionality of Smart Energy Demand Response and Load Control. Devices targeted by this cluster include thermostats and devices that support load control.
 * Command: CancelLoadControlEvent
 * @param issuerEventId uint32_t
 * @param deviceClass uint16_t
 * @param utilityEnrollmentGroup uint8_t
 * @param cancelControl uint8_t
 * @param effectiveTime uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDemandResponseLoadControlClusterCancelLoadControlEvent(uint32_t issuerEventId,
                                                                         uint16_t deviceClass,
                                                                         uint8_t utilityEnrollmentGroup,
                                                                         uint8_t cancelControl,
                                                                         uint32_t effectiveTime,
                                                                         frame_info_t *frame_info );
 
/** @brief Command description for CancelAllLoadControlEvents
 *
 * Cluster: Demand Response and Load Control, This cluster provides an interface to the functionality of Smart Energy Demand Response and Load Control. Devices targeted by this cluster include thermostats and devices that support load control.
 * Command: CancelAllLoadControlEvents
 * @param cancelControl uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDemandResponseLoadControlClusterCancelAllLoadControlEvents(uint8_t cancelControl,
                                                                             frame_info_t *frame_info );
 
/** @brief Command description for ReportEventStatus
 *
 * Cluster: Demand Response and Load Control, This cluster provides an interface to the functionality of Smart Energy Demand Response and Load Control. Devices targeted by this cluster include thermostats and devices that support load control.
 * Command: ReportEventStatus
 * @param issuerEventId uint32_t
 * @param eventStatus uint8_t
 * @param eventStatusTime uint32_t
 * @param criticalityLevelApplied uint8_t
 * @param coolingTemperatureSetPointApplied uint16_t
 * @param heatingTemperatureSetPointApplied uint16_t
 * @param averageLoadAdjustmentPercentageApplied int8_t
 * @param dutyCycleApplied uint8_t
 * @param eventControl uint8_t
 * @param signatureType uint8_t
 * @param signature uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDemandResponseLoadControlClusterReportEventStatus(uint32_t issuerEventId,
                                                                    uint8_t eventStatus,
                                                                    uint32_t eventStatusTime,
                                                                    uint8_t criticalityLevelApplied,
                                                                    uint16_t coolingTemperatureSetPointApplied,
                                                                    uint16_t heatingTemperatureSetPointApplied,
                                                                    int8_t averageLoadAdjustmentPercentageApplied,
                                                                    uint8_t dutyCycleApplied,
                                                                    uint8_t eventControl,
                                                                    uint8_t signatureType,
                                                                    uint8_t *signature,
                                                                    frame_info_t *frame_info );
 
/** @brief Command description for GetScheduledEvents
 *
 * Cluster: Demand Response and Load Control, This cluster provides an interface to the functionality of Smart Energy Demand Response and Load Control. Devices targeted by this cluster include thermostats and devices that support load control.
 * Command: GetScheduledEvents
 * @param startTime uint32_t
 * @param numberOfEvents uint8_t
 * @param issuerEventId uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDemandResponseLoadControlClusterGetScheduledEvents(uint32_t startTime,
                                                                     uint8_t numberOfEvents,
                                                                     uint32_t issuerEventId,
                                                                     frame_info_t *frame_info );
 
/** @} END Demand Response and Load Control Commands */

/** @name Simple Metering Commands */
 
/** @brief This command is used to request the ESI to mirror Metering Device data.
 *
 * Cluster: Simple Metering, The Metering Cluster provides a mechanism to retrieve usage information from Electric, Gas, Water, and potentially Thermal metering devices.
 * Command: RequestMirror
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSimpleMeteringClusterRequestMirror(
    frame_info_t *frame_info );
 
/** @brief This command is used to request the ESI to remove its mirror of Metering Device data.
 *
 * Cluster: Simple Metering, The Metering Cluster provides a mechanism to retrieve usage information from Electric, Gas, Water, and potentially Thermal metering devices.
 * Command: RemoveMirror
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSimpleMeteringClusterRemoveMirror(
    frame_info_t *frame_info );
 
/** @brief This command is generated when the client command Request Fast Poll Mode is received.
 *
 * Cluster: Simple Metering, The Metering Cluster provides a mechanism to retrieve usage information from Electric, Gas, Water, and potentially Thermal metering devices.
 * Command: RequestFastPollModeResponse
 * @param appliedUpdatePeriod uint8_t
 * @param fastPollModeEndtime uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSimpleMeteringClusterRequestFastPollModeResponse(uint8_t appliedUpdatePeriod,
                                                                   uint32_t fastPollModeEndtime,
                                                                   frame_info_t *frame_info );
 
/** @brief This command is generated in response to a ScheduleSnapshot command, and is sent to confirm whether the requested snapshot schedule has been set up.
 *
 * Cluster: Simple Metering, The Metering Cluster provides a mechanism to retrieve usage information from Electric, Gas, Water, and potentially Thermal metering devices.
 * Command: ScheduleSnapshotResponse
 * @param issuerEventId uint32_t
 * @param snapshotResponsePayload uint8_t*
 * @param snapshotResponsePayloadLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSimpleMeteringClusterScheduleSnapshotResponse(uint32_t issuerEventId,
                                                                uint8_t *snapshotResponsePayload,
                                                                uint16_t snapshotResponsePayloadLen,
                                                                frame_info_t *frame_info );
 
/** @brief This command is generated in response to a TakeSnapshot command, and is sent to confirm whether the requested snapshot has been accepted and successfully taken.
 *
 * Cluster: Simple Metering, The Metering Cluster provides a mechanism to retrieve usage information from Electric, Gas, Water, and potentially Thermal metering devices.
 * Command: TakeSnapshotResponse
 * @param snapshotId uint32_t
 * @param snapshotConfirmation uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSimpleMeteringClusterTakeSnapshotResponse(uint32_t snapshotId,
                                                            uint8_t snapshotConfirmation,
                                                            frame_info_t *frame_info );
 
/** @brief This command is generated in response to a GetSnapshot command. It is used to 
	return a single snapshot to the client.
 *
 * Cluster: Simple Metering, The Metering Cluster provides a mechanism to retrieve usage information from Electric, Gas, Water, and potentially Thermal metering devices.
 * Command: PublishSnapshot
 * @param snapshotId uint32_t
 * @param snapshotTime uint32_t
 * @param totalSnapshotsFound uint8_t
 * @param commandIndex uint8_t
 * @param totalCommands uint8_t
 * @param snapshotCause uint32_t
 * @param snapshotPayloadType uint8_t
 * @param snapshotPayload uint8_t*
 * @param snapshotPayloadLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSimpleMeteringClusterPublishSnapshot(uint32_t snapshotId,
                                                       uint32_t snapshotTime,
                                                       uint8_t totalSnapshotsFound,
                                                       uint8_t commandIndex,
                                                       uint8_t totalCommands,
                                                       uint32_t snapshotCause,
                                                       uint8_t snapshotPayloadType,
                                                       uint8_t *snapshotPayload,
                                                       uint16_t snapshotPayloadLen,
                                                       frame_info_t *frame_info );
 
/** @brief This command is used to send the requested sample data to the client. It is generated in response to a GetSampledData command.
 *
 * Cluster: Simple Metering, The Metering Cluster provides a mechanism to retrieve usage information from Electric, Gas, Water, and potentially Thermal metering devices.
 * Command: GetSampledDataResponse
 * @param sampleId uint16_t
 * @param sampleStartTime uint32_t
 * @param sampleType uint8_t
 * @param sampleRequestInterval uint16_t
 * @param numberOfSamples uint16_t
 * @param samples uint8_t*
 * @param samplesLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSimpleMeteringClusterGetSampledDataResponse(uint16_t sampleId,
                                                              uint32_t sampleStartTime,
                                                              uint8_t sampleType,
                                                              uint16_t sampleRequestInterval,
                                                              uint16_t numberOfSamples,
                                                              uint8_t *samples,
                                                              uint16_t samplesLen,
                                                              frame_info_t *frame_info );
 
/** @brief ConfigureMirror is sent to the mirror once the mirror has been created. The command deals with the operational configuration of the Mirror.
 *
 * Cluster: Simple Metering, The Metering Cluster provides a mechanism to retrieve usage information from Electric, Gas, Water, and potentially Thermal metering devices.
 * Command: ConfigureMirror
 * @param issuerEventId uint32_t
 * @param reportingInterval uint32_t
 * @param mirrorNotificationReporting uint8_t
 * @param notificationScheme uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSimpleMeteringClusterConfigureMirror(uint32_t issuerEventId,
                                                       uint32_t reportingInterval,
                                                       uint8_t mirrorNotificationReporting,
                                                       uint8_t notificationScheme,
                                                       frame_info_t *frame_info );
 
/** @brief The ConfigureNotificationScheme is sent to the mirror once the mirror has been created. The command deals with the operational configuration of the Mirror and the device that reports to the mirror. No default schemes are allowed to be overwritten.
 *
 * Cluster: Simple Metering, The Metering Cluster provides a mechanism to retrieve usage information from Electric, Gas, Water, and potentially Thermal metering devices.
 * Command: ConfigureNotificationScheme
 * @param issuerEventId uint32_t
 * @param notificationScheme uint8_t
 * @param notificationFlagOrder uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSimpleMeteringClusterConfigureNotificationScheme(uint32_t issuerEventId,
                                                                   uint8_t notificationScheme,
                                                                   uint32_t notificationFlagOrder,
                                                                   frame_info_t *frame_info );
 
/** @brief The ConfigureNotificationFlags command is used to set the commands relating to the bit value for each NotificationFlags attribute that the scheme is proposing to use.
 *
 * Cluster: Simple Metering, The Metering Cluster provides a mechanism to retrieve usage information from Electric, Gas, Water, and potentially Thermal metering devices.
 * Command: ConfigureNotificationFlags
 * @param issuerEventId uint32_t
 * @param notificationScheme uint8_t
 * @param notificationFlagAttributeId uint16_t
 * @param clusterId uint16_t
 * @param manufacturerCode uint16_t
 * @param numberOfCommands uint8_t
 * @param commandIds uint8_t*
 * @param commandIdsLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSimpleMeteringClusterConfigureNotificationFlags(uint32_t issuerEventId,
                                                                  uint8_t notificationScheme,
                                                                  uint16_t notificationFlagAttributeId,
                                                                  uint16_t clusterId,
                                                                  uint16_t manufacturerCode,
                                                                  uint8_t numberOfCommands,
                                                                  uint8_t *commandIds,
                                                                  uint16_t commandIdsLen,
                                                                  frame_info_t *frame_info );
 
/** @brief The GetNotifiedMessage command is used only when a BOMD is being mirrored. This command provides a method for the BOMD to notify the Mirror message queue that it wants to receive commands that the Mirror has queued. The Notification flags set within the command shall inform the mirror of the commands that the BOMD is requesting.
 *
 * Cluster: Simple Metering, The Metering Cluster provides a mechanism to retrieve usage information from Electric, Gas, Water, and potentially Thermal metering devices.
 * Command: GetNotifiedMessage
 * @param notificationScheme uint8_t
 * @param notificationFlagAttributeId uint16_t
 * @param notificationFlagsN uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSimpleMeteringClusterGetNotifiedMessage(uint8_t notificationScheme,
                                                          uint16_t notificationFlagAttributeId,
                                                          uint32_t notificationFlagsN,
                                                          frame_info_t *frame_info );
 
/** @brief This command is transmitted by a Metering Device in response to a ChangeSupply command.
 *
 * Cluster: Simple Metering, The Metering Cluster provides a mechanism to retrieve usage information from Electric, Gas, Water, and potentially Thermal metering devices.
 * Command: SupplyStatusResponse
 * @param providerId uint32_t
 * @param issuerEventId uint32_t
 * @param implementationDateTime uint32_t
 * @param supplyStatus uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSimpleMeteringClusterSupplyStatusResponse(uint32_t providerId,
                                                            uint32_t issuerEventId,
                                                            uint32_t implementationDateTime,
                                                            uint8_t supplyStatus,
                                                            frame_info_t *frame_info );
 
/** @brief This command is transmitted by a Metering Device in response to a StartSampling command.
 *
 * Cluster: Simple Metering, The Metering Cluster provides a mechanism to retrieve usage information from Electric, Gas, Water, and potentially Thermal metering devices.
 * Command: StartSamplingResponse
 * @param sampleId uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSimpleMeteringClusterStartSamplingResponse(uint16_t sampleId,
                                                             frame_info_t *frame_info );
 
/** @brief The GetProfile command is generated when a client device wishes to retrieve a list of captured Energy, Gas or water consumption for profiling purposes.
 *
 * Cluster: Simple Metering, The Metering Cluster provides a mechanism to retrieve usage information from Electric, Gas, Water, and potentially Thermal metering devices.
 * Command: GetProfile
 * @param intervalChannel uint8_t
 * @param endTime uint32_t
 * @param numberOfPeriods uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSimpleMeteringClusterGetProfile(uint8_t intervalChannel,
                                                  uint32_t endTime,
                                                  uint8_t numberOfPeriods,
                                                  frame_info_t *frame_info );
 
/** @brief The Request Mirror Response Command allows the ESI to inform a sleepy Metering Device it has the ability to store and mirror its data.
 *
 * Cluster: Simple Metering, The Metering Cluster provides a mechanism to retrieve usage information from Electric, Gas, Water, and potentially Thermal metering devices.
 * Command: RequestMirrorResponse
 * @param endpointId uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSimpleMeteringClusterRequestMirrorResponse(uint16_t endpointId,
                                                             frame_info_t *frame_info );
 
/** @brief The Mirror Removed Command allows the ESI to inform a sleepy Metering Device mirroring support has been removed or halted.
 *
 * Cluster: Simple Metering, The Metering Cluster provides a mechanism to retrieve usage information from Electric, Gas, Water, and potentially Thermal metering devices.
 * Command: MirrorRemoved
 * @param endpointId uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSimpleMeteringClusterMirrorRemoved(uint16_t endpointId,
                                                     frame_info_t *frame_info );
 
/** @brief The Request Fast Poll Mode command is generated when the metering client wishes to receive near real-time updates of InstantaneousDemand.
 *
 * Cluster: Simple Metering, The Metering Cluster provides a mechanism to retrieve usage information from Electric, Gas, Water, and potentially Thermal metering devices.
 * Command: RequestFastPollMode
 * @param fastPollUpdatePeriod uint8_t
 * @param duration uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSimpleMeteringClusterRequestFastPollMode(uint8_t fastPollUpdatePeriod,
                                                           uint8_t duration,
                                                           frame_info_t *frame_info );
 
/** @brief This command is used to set up a schedule of when the device shall create snapshot data.
 *
 * Cluster: Simple Metering, The Metering Cluster provides a mechanism to retrieve usage information from Electric, Gas, Water, and potentially Thermal metering devices.
 * Command: ScheduleSnapshot
 * @param issuerEventId uint32_t
 * @param commandIndex uint8_t
 * @param commandCount uint8_t
 * @param snapshotSchedulePayload uint8_t*
 * @param snapshotSchedulePayloadLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSimpleMeteringClusterScheduleSnapshot(uint32_t issuerEventId,
                                                        uint8_t commandIndex,
                                                        uint8_t commandCount,
                                                        uint8_t *snapshotSchedulePayload,
                                                        uint16_t snapshotSchedulePayloadLen,
                                                        frame_info_t *frame_info );
 
/** @brief This command is used to instruct the cluster server to take a single snapshot.
 *
 * Cluster: Simple Metering, The Metering Cluster provides a mechanism to retrieve usage information from Electric, Gas, Water, and potentially Thermal metering devices.
 * Command: TakeSnapshot
 * @param snapshotCause uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSimpleMeteringClusterTakeSnapshot(uint32_t snapshotCause,
                                                    frame_info_t *frame_info );
 
/** @brief This command is used to request snapshot data from the cluster server.
 *
 * Cluster: Simple Metering, The Metering Cluster provides a mechanism to retrieve usage information from Electric, Gas, Water, and potentially Thermal metering devices.
 * Command: GetSnapshot
 * @param earliestStartTime uint32_t
 * @param latestEndTime uint32_t
 * @param snapshotOffset uint8_t
 * @param snapshotCause uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSimpleMeteringClusterGetSnapshot(uint32_t earliestStartTime,
                                                   uint32_t latestEndTime,
                                                   uint8_t snapshotOffset,
                                                   uint32_t snapshotCause,
                                                   frame_info_t *frame_info );
 
/** @brief The sampling mechanism allows a set of samples of the specified type of data to be taken, commencing at the stipulated start time. This mechanism may run concurrently with the capturing of profile data, and may refer the same parameters, albeit possibly at a different sampling rate.
 *
 * Cluster: Simple Metering, The Metering Cluster provides a mechanism to retrieve usage information from Electric, Gas, Water, and potentially Thermal metering devices.
 * Command: StartSampling
 * @param issuerEventId uint32_t
 * @param startSamplingTime uint32_t
 * @param sampleType uint8_t
 * @param sampleRequestInterval uint16_t
 * @param maxNumberOfSamples uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSimpleMeteringClusterStartSampling(uint32_t issuerEventId,
                                                     uint32_t startSamplingTime,
                                                     uint8_t sampleType,
                                                     uint16_t sampleRequestInterval,
                                                     uint16_t maxNumberOfSamples,
                                                     frame_info_t *frame_info );
 
/** @brief This command is used to request sampled data from the server. Note that it is the responsibility of the client to ensure that it does not request more samples than can be held in a single command payload.
 *
 * Cluster: Simple Metering, The Metering Cluster provides a mechanism to retrieve usage information from Electric, Gas, Water, and potentially Thermal metering devices.
 * Command: GetSampledData
 * @param sampleId uint16_t
 * @param earliestSampleTime uint32_t
 * @param sampleType uint8_t
 * @param numberOfSamples uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSimpleMeteringClusterGetSampledData(uint16_t sampleId,
                                                      uint32_t earliestSampleTime,
                                                      uint8_t sampleType,
                                                      uint16_t numberOfSamples,
                                                      frame_info_t *frame_info );
 
/** @brief This command is sent in response to the ReportAttribute command when the MirrorReporting attribute is set.
 *
 * Cluster: Simple Metering, The Metering Cluster provides a mechanism to retrieve usage information from Electric, Gas, Water, and potentially Thermal metering devices.
 * Command: MirrorReportAttributeResponse
 * @param notificationScheme uint8_t
 * @param notificationFlags uint8_t*
 * @param notificationFlagsLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSimpleMeteringClusterMirrorReportAttributeResponse(uint8_t notificationScheme,
                                                                     uint8_t *notificationFlags,
                                                                     uint16_t notificationFlagsLen,
                                                                     frame_info_t *frame_info );
 
/** @brief The ResetLoadLimitCounter command shall cause the LoadLimitCounter attribute to be reset.
 *
 * Cluster: Simple Metering, The Metering Cluster provides a mechanism to retrieve usage information from Electric, Gas, Water, and potentially Thermal metering devices.
 * Command: ResetLoadLimitCounter
 * @param providerId uint32_t
 * @param issuerEventId uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSimpleMeteringClusterResetLoadLimitCounter(uint32_t providerId,
                                                             uint32_t issuerEventId,
                                                             frame_info_t *frame_info );
 
/** @brief This command is sent from the Head-end or ESI to the Metering Device to instruct it to change the status of the valve or load switch, i.e. the supply.
 *
 * Cluster: Simple Metering, The Metering Cluster provides a mechanism to retrieve usage information from Electric, Gas, Water, and potentially Thermal metering devices.
 * Command: ChangeSupply
 * @param providerId uint32_t
 * @param issuerEventId uint32_t
 * @param requestDateTime uint32_t
 * @param implementationDateTime uint32_t
 * @param proposedSupplyStatus uint8_t
 * @param supplyControlBits uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSimpleMeteringClusterChangeSupply(uint32_t providerId,
                                                    uint32_t issuerEventId,
                                                    uint32_t requestDateTime,
                                                    uint32_t implementationDateTime,
                                                    uint8_t proposedSupplyStatus,
                                                    uint8_t supplyControlBits,
                                                    frame_info_t *frame_info );
 
/** @brief This command is a simplified version of the ChangeSupply command, intended to be sent from an IHD to a meter as the consequence of a user action on the IHD. Its purpose is to provide a local disconnection/reconnection button on the IHD in addition to the one on the meter.
 *
 * Cluster: Simple Metering, The Metering Cluster provides a mechanism to retrieve usage information from Electric, Gas, Water, and potentially Thermal metering devices.
 * Command: LocalChangeSupply
 * @param proposedSupplyStatus uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSimpleMeteringClusterLocalChangeSupply(uint8_t proposedSupplyStatus,
                                                         frame_info_t *frame_info );
 
/** @brief This command is used to specify the required status of the supply following the occurance of certain events on the meter.
 *
 * Cluster: Simple Metering, The Metering Cluster provides a mechanism to retrieve usage information from Electric, Gas, Water, and potentially Thermal metering devices.
 * Command: SetSupplyStatus
 * @param issuerEventId uint32_t
 * @param supplyTamperState uint8_t
 * @param supplyDepletionState uint8_t
 * @param supplyUncontrolledFlowState uint8_t
 * @param loadLimitSupplyState uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSimpleMeteringClusterSetSupplyStatus(uint32_t issuerEventId,
                                                       uint8_t supplyTamperState,
                                                       uint8_t supplyDepletionState,
                                                       uint8_t supplyUncontrolledFlowState,
                                                       uint8_t loadLimitSupplyState,
                                                       frame_info_t *frame_info );
 
/** @brief This command is used to update the 'Uncontrolled Flow Rate' configuration data used by flow meters.
 *
 * Cluster: Simple Metering, The Metering Cluster provides a mechanism to retrieve usage information from Electric, Gas, Water, and potentially Thermal metering devices.
 * Command: SetUncontrolledFlowThreshold
 * @param providerId uint32_t
 * @param issuerEventId uint32_t
 * @param uncontrolledFlowThreshold uint16_t
 * @param unitOfMeasure uint8_t
 * @param multiplier uint16_t
 * @param divisor uint16_t
 * @param stabilisationPeriod uint8_t
 * @param measurementPeriod uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSimpleMeteringClusterSetUncontrolledFlowThreshold(uint32_t providerId,
                                                                    uint32_t issuerEventId,
                                                                    uint16_t uncontrolledFlowThreshold,
                                                                    uint8_t unitOfMeasure,
                                                                    uint16_t multiplier,
                                                                    uint16_t divisor,
                                                                    uint8_t stabilisationPeriod,
                                                                    uint16_t measurementPeriod,
                                                                    frame_info_t *frame_info );
 
/** @} END Simple Metering Commands */

/** @name Messaging Commands */
 
/** @brief The CancelMessage command provides the ability to cancel the sending or acceptance of previously sent messages.
 *
 * Cluster: Messaging, This cluster provides an interface for passing text messages between SE devices.
 * Command: CancelMessage
 * @param messageId uint32_t
 * @param messageControl uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandMessagingClusterCancelMessage(uint32_t messageId,
                                                uint8_t messageControl,
                                                frame_info_t *frame_info );
 
/** @brief The DisplayProtected Message command is for use with messages that are protected by a password or PIN.
 *
 * Cluster: Messaging, This cluster provides an interface for passing text messages between SE devices.
 * Command: DisplayProtectedMessage
 * @param messageId uint32_t
 * @param messageControl uint8_t
 * @param startTime uint32_t
 * @param durationInMinutes uint16_t
 * @param message uint8_t*
 * @param optionalExtendedMessageControl uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandMessagingClusterDisplayProtectedMessage(uint32_t messageId,
                                                          uint8_t messageControl,
                                                          uint32_t startTime,
                                                          uint16_t durationInMinutes,
                                                          uint8_t *message,
                                                          uint8_t optionalExtendedMessageControl,
                                                          frame_info_t *frame_info );
 
/** @brief The CancelAllMessages command indicates to a client device that it should cancel all display messages currently held by it.
 *
 * Cluster: Messaging, This cluster provides an interface for passing text messages between SE devices.
 * Command: CancelAllMessages
 * @param implementationDateTime uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandMessagingClusterCancelAllMessages(uint32_t implementationDateTime,
                                                    frame_info_t *frame_info );
 
/** @brief Command description for GetLastMessage
 *
 * Cluster: Messaging, This cluster provides an interface for passing text messages between SE devices.
 * Command: GetLastMessage
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandMessagingClusterGetLastMessage(
    frame_info_t *frame_info );
 
/** @brief The Message Confirmation command provides an indication that a Utility Customer has acknowledged and/or accepted the contents of a previously sent message.  Enhanced Message Confirmation commands shall contain an answer of 'NO', 'YES' and/or a message confirmation string.
 *
 * Cluster: Messaging, This cluster provides an interface for passing text messages between SE devices.
 * Command: MessageConfirmation
 * @param messageId uint32_t
 * @param confirmationTime uint32_t
 * @param messageConfirmationControl uint8_t
 * @param messageResponse uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandMessagingClusterMessageConfirmation(uint32_t messageId,
                                                      uint32_t confirmationTime,
                                                      uint8_t messageConfirmationControl,
                                                      uint8_t *messageResponse,
                                                      frame_info_t *frame_info );
 
/** @brief This command initiates the 
	return of the first (and maybe only ); Cancel All Messages command held on the associated server, and which has an implementation time equal to or later than the value indicated in the payload.
 *
 * Cluster: Messaging, This cluster provides an interface for passing text messages between SE devices.
 * Command: GetMessageCancellation
 * @param earliestImplementationTime uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandMessagingClusterGetMessageCancellation(uint32_t earliestImplementationTime,
                                                         frame_info_t *frame_info );
 
/** @} END Messaging Commands */

/** @name Tunneling Commands */
 
/** @brief Client command used to close the tunnel with the server. The parameter in the payload specifies the tunnel identifier of the tunnel that has to be closed. The server leaves the tunnel open and the assigned resources allocated until the client sends the CloseTunnel command or the CloseTunnelTimeout fires.
 *
 * Cluster: Tunneling, The tunneling cluster provides an interface for tunneling protocols.
 * Command: CloseTunnel
 * @param tunnelId uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandTunnelingClusterCloseTunnel(uint16_t tunnelId,
                                              frame_info_t *frame_info );
 
/** @brief Command that indicates (if received ); that the client has sent data to the server. The data itself is contained within the payload.
 *
 * Cluster: Tunneling, The tunneling cluster provides an interface for tunneling protocols.
 * Command: TransferDataClientToServer
 * @param tunnelId uint16_t
 * @param data uint8_t*
 * @param dataLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandTunnelingClusterTransferDataClientToServer(uint16_t tunnelId,
                                                             uint8_t *data,
                                                             uint16_t dataLen,
                                                             frame_info_t *frame_info );
 
/** @brief This command is generated by the receiver of a TransferData command if the tunnel status indicates that something is wrong. There are two three cases in which TransferDataError is sent: (1 ); The TransferData received contains a TunnelID that does not match to any of the active tunnels of the receiving device. This could happen if a (sleeping ); device sends a TransferData command to a tunnel that has been closed by the server after the CloseTunnelTimeout.  (2 ); The TransferData received contains a proper TunnelID of an active tunnel, but the device sending the data does not match to it.  (3 ); The TransferData received contains more data than indicated by the MaximumIncomingTransferSize of the receiving device.
 *
 * Cluster: Tunneling, The tunneling cluster provides an interface for tunneling protocols.
 * Command: TransferDataErrorClientToServer
 * @param tunnelId uint16_t
 * @param transferDataStatus uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandTunnelingClusterTransferDataErrorClientToServer(uint16_t tunnelId,
                                                                  uint8_t transferDataStatus,
                                                                  frame_info_t *frame_info );
 
/** @brief Command sent in response to each TransferData command in case - and only in case - flow control has been requested by the client in the TunnelRequest command and is supported by both tunnel endpoints. The response payload indicates the number of octets that may still be received by the receiver.
 *
 * Cluster: Tunneling, The tunneling cluster provides an interface for tunneling protocols.
 * Command: AckTransferDataClientToServer
 * @param tunnelId uint16_t
 * @param numberOfBytesLeft uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandTunnelingClusterAckTransferDataClientToServer(uint16_t tunnelId,
                                                                uint16_t numberOfBytesLeft,
                                                                frame_info_t *frame_info );
 
/** @brief The ReadyData command is generated - after a receiver had to stop the dataflow using the AckTransferData(0 ); command - to indicate that the device is now ready to continue receiving data. The parameter NumberOfOctetsLeft gives a hint on how much space is left for the next data transfer. The ReadyData command is only issued if flow control is enabled.
 *
 * Cluster: Tunneling, The tunneling cluster provides an interface for tunneling protocols.
 * Command: ReadyDataClientToServer
 * @param tunnelId uint16_t
 * @param numberOfOctetsLeft uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandTunnelingClusterReadyDataClientToServer(uint16_t tunnelId,
                                                          uint16_t numberOfOctetsLeft,
                                                          frame_info_t *frame_info );
 
/** @brief Get Supported Tunnel Protocols is the client command used to determine the Tunnel protocols supported on another device.
 *
 * Cluster: Tunneling, The tunneling cluster provides an interface for tunneling protocols.
 * Command: GetSupportedTunnelProtocols
 * @param protocolOffset uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandTunnelingClusterGetSupportedTunnelProtocols(uint8_t protocolOffset,
                                                              frame_info_t *frame_info );
 
/** @brief RequestTunnelResponse is sent by the server in response to a RequestTunnel command previously received from the client. The response contains the status of the RequestTunnel command and a tunnel identifier corresponding to the tunnel that has been set-up in the server in case of success.
 *
 * Cluster: Tunneling, The tunneling cluster provides an interface for tunneling protocols.
 * Command: RequestTunnelResponse
 * @param tunnelId uint16_t
 * @param tunnelStatus uint8_t
 * @param maximumIncomingTransferSize uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandTunnelingClusterRequestTunnelResponse(uint16_t tunnelId,
                                                        uint8_t tunnelStatus,
                                                        uint16_t maximumIncomingTransferSize,
                                                        frame_info_t *frame_info );
 
/** @brief Command that transfers data from server to the client. The data itself has to be placed within the payload.
 *
 * Cluster: Tunneling, The tunneling cluster provides an interface for tunneling protocols.
 * Command: TransferDataServerToClient
 * @param tunnelId uint16_t
 * @param data uint8_t*
 * @param dataLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandTunnelingClusterTransferDataServerToClient(uint16_t tunnelId,
                                                             uint8_t *data,
                                                             uint16_t dataLen,
                                                             frame_info_t *frame_info );
 
/** @brief This command is generated by the receiver of a TransferData command if the tunnel status indicates that something is wrong. There are two three cases in which TransferDataError is sent: (1 ); The TransferData received contains a TunnelID that does not match to any of the active tunnels of the receiving device. This could happen if a (sleeping ); device sends a TransferData command to a tunnel that has been closed by the server after the CloseTunnelTimeout.  (2 ); The TransferData received contains a proper TunnelID of an active tunnel, but the device sending the data does not match to it.  (3 ); The TransferData received contains more data than indicated by the MaximumIncomingTransferSize of the receiving device.
 *
 * Cluster: Tunneling, The tunneling cluster provides an interface for tunneling protocols.
 * Command: TransferDataErrorServerToClient
 * @param tunnelId uint16_t
 * @param transferDataStatus uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandTunnelingClusterTransferDataErrorServerToClient(uint16_t tunnelId,
                                                                  uint8_t transferDataStatus,
                                                                  frame_info_t *frame_info );
 
/** @brief Command sent in response to each TransferData command in case - and only in case - flow control has been requested by the client in the TunnelRequest command and is supported by both tunnel endpoints. The response payload indicates the number of octets that may still be received by the receiver.
 *
 * Cluster: Tunneling, The tunneling cluster provides an interface for tunneling protocols.
 * Command: AckTransferDataServerToClient
 * @param tunnelId uint16_t
 * @param numberOfBytesLeft uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandTunnelingClusterAckTransferDataServerToClient(uint16_t tunnelId,
                                                                uint16_t numberOfBytesLeft,
                                                                frame_info_t *frame_info );
 
/** @brief The ReadyData command is generated - after a receiver had to stop the dataflow using the AckTransferData(0 ); command - to indicate that the device is now ready to continue receiving data. The parameter NumberOfOctetsLeft gives a hint on how much space is left for the next data transfer. The ReadyData command is only issued if flow control is enabled.
 *
 * Cluster: Tunneling, The tunneling cluster provides an interface for tunneling protocols.
 * Command: ReadyDataServerToClient
 * @param tunnelId uint16_t
 * @param numberOfOctetsLeft uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandTunnelingClusterReadyDataServerToClient(uint16_t tunnelId,
                                                          uint16_t numberOfOctetsLeft,
                                                          frame_info_t *frame_info );
 
/** @brief Supported Tunnel Protocol Response is sent in response to a Get Supported Tunnel Protocols command previously received. The response contains a list of Tunnel protocols supported by the device; the payload of the response should be capable of holding up to 16 protocols.
 *
 * Cluster: Tunneling, The tunneling cluster provides an interface for tunneling protocols.
 * Command: SupportedTunnelProtocolsResponse
 * @param protocolListComplete uint8_t
 * @param protocolCount uint8_t
 * @param protocolList uint8_t*
 * @param protocolListLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandTunnelingClusterSupportedTunnelProtocolsResponse(uint8_t protocolListComplete,
                                                                   uint8_t protocolCount,
                                                                   uint8_t *protocolList,
                                                                   uint16_t protocolListLen,
                                                                   frame_info_t *frame_info );
 
/** @brief TunnelClosureNotification is sent by the server to indicate that a tunnel has been closed due to expiration of a CloseTunnelTimeout.
 *
 * Cluster: Tunneling, The tunneling cluster provides an interface for tunneling protocols.
 * Command: TunnelClosureNotification
 * @param tunnelId uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandTunnelingClusterTunnelClosureNotification(uint16_t tunnelId,
                                                            frame_info_t *frame_info );
 
/** @} END Tunneling Commands */

/** @name Prepayment Commands */
 
/** @brief The ChangeDebt command is send to the Metering Device to change the fuel or Non fuel debt values.
 *
 * Cluster: Prepayment, The Prepayment Cluster provides the facility to pass messages relating to prepayment between devices on the HAN.
 * Command: ChangeDebt
 * @param issuerEventId uint32_t
 * @param debtLabel uint8_t*
 * @param debtAmount uint32_t
 * @param debtRecoveryMethod uint8_t
 * @param debtAmountType uint8_t
 * @param debtRecoveryStartTime uint32_t
 * @param debtRecoveryCollectionTime uint16_t
 * @param debtRecoveryFrequency uint8_t
 * @param debtRecoveryAmount uint32_t
 * @param debtRecoveryBalancePercentage uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPrepaymentClusterChangeDebt(uint32_t issuerEventId,
                                              uint8_t *debtLabel,
                                              uint32_t debtAmount,
                                              uint8_t debtRecoveryMethod,
                                              uint8_t debtAmountType,
                                              uint32_t debtRecoveryStartTime,
                                              uint16_t debtRecoveryCollectionTime,
                                              uint8_t debtRecoveryFrequency,
                                              uint32_t debtRecoveryAmount,
                                              uint16_t debtRecoveryBalancePercentage,
                                              frame_info_t *frame_info );
 
/** @brief This command is a method to set up the parameters for the emergency credit.
 *
 * Cluster: Prepayment, The Prepayment Cluster provides the facility to pass messages relating to prepayment between devices on the HAN.
 * Command: EmergencyCreditSetup
 * @param issuerEventId uint32_t
 * @param startTime uint32_t
 * @param emergencyCreditLimit uint32_t
 * @param emergencyCreditThreshold uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPrepaymentClusterEmergencyCreditSetup(uint32_t issuerEventId,
                                                        uint32_t startTime,
                                                        uint32_t emergencyCreditLimit,
                                                        uint32_t emergencyCreditThreshold,
                                                        frame_info_t *frame_info );
 
/** @brief The ConsumerTopUp command is used by the IPD and the ESI as a method of applying credit top up values to the prepayment meter.
 *
 * Cluster: Prepayment, The Prepayment Cluster provides the facility to pass messages relating to prepayment between devices on the HAN.
 * Command: ConsumerTopUp
 * @param originatingDevice uint8_t
 * @param topUpCode uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPrepaymentClusterConsumerTopUp(uint8_t originatingDevice,
                                                 uint8_t *topUpCode,
                                                 frame_info_t *frame_info );
 
/** @brief The CreditAdjustment command is sent to update the accounting base for the Prepayment meter.
 *
 * Cluster: Prepayment, The Prepayment Cluster provides the facility to pass messages relating to prepayment between devices on the HAN.
 * Command: CreditAdjustment
 * @param issuerEventId uint32_t
 * @param startTime uint32_t
 * @param creditAdjustmentType uint8_t
 * @param creditAdjustmentValue uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPrepaymentClusterCreditAdjustment(uint32_t issuerEventId,
                                                    uint32_t startTime,
                                                    uint8_t creditAdjustmentType,
                                                    uint32_t creditAdjustmentValue,
                                                    frame_info_t *frame_info );
 
/** @brief This command is sent to a Metering Device to instruct it to change its mode of operation. i.e. from Credit to Prepayment.
 *
 * Cluster: Prepayment, The Prepayment Cluster provides the facility to pass messages relating to prepayment between devices on the HAN.
 * Command: ChangePaymentMode
 * @param providerId uint32_t
 * @param issuerEventId uint32_t
 * @param implementationDateTime uint32_t
 * @param proposedPaymentControlConfiguration uint16_t
 * @param cutOffValue uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPrepaymentClusterChangePaymentMode(uint32_t providerId,
                                                     uint32_t issuerEventId,
                                                     uint32_t implementationDateTime,
                                                     uint16_t proposedPaymentControlConfiguration,
                                                     uint32_t cutOffValue,
                                                     frame_info_t *frame_info );
 
/** @brief This command is used to request the cluster server for snapshot data.
 *
 * Cluster: Prepayment, The Prepayment Cluster provides the facility to pass messages relating to prepayment between devices on the HAN.
 * Command: GetPrepaySnapshot
 * @param earliestStartTime uint32_t
 * @param latestEndTime uint32_t
 * @param snapshotOffset uint8_t
 * @param snapshotCause uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPrepaymentClusterGetPrepaySnapshot(uint32_t earliestStartTime,
                                                     uint32_t latestEndTime,
                                                     uint8_t snapshotOffset,
                                                     uint32_t snapshotCause,
                                                     frame_info_t *frame_info );
 
/** @brief This command is sent to the Metering Device to retrieve the log of Top Up codes received by the meter.
 *
 * Cluster: Prepayment, The Prepayment Cluster provides the facility to pass messages relating to prepayment between devices on the HAN.
 * Command: GetTopUpLog
 * @param latestEndTime uint32_t
 * @param numberOfRecords uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPrepaymentClusterGetTopUpLog(uint32_t latestEndTime,
                                               uint8_t numberOfRecords,
                                               frame_info_t *frame_info );
 
/** @brief This command is sent from client to a Prepayment server to set the warning level for low credit.
 *
 * Cluster: Prepayment, The Prepayment Cluster provides the facility to pass messages relating to prepayment between devices on the HAN.
 * Command: SetLowCreditWarningLevel
 * @param lowCreditWarningLevel uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPrepaymentClusterSetLowCreditWarningLevel(uint32_t lowCreditWarningLevel,
                                                            frame_info_t *frame_info );
 
/** @brief This command is used to request the contents of the repayment log.
 *
 * Cluster: Prepayment, The Prepayment Cluster provides the facility to pass messages relating to prepayment between devices on the HAN.
 * Command: GetDebtRepaymentLog
 * @param latestEndTime uint32_t
 * @param numberOfDebts uint8_t
 * @param debtType uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPrepaymentClusterGetDebtRepaymentLog(uint32_t latestEndTime,
                                                       uint8_t numberOfDebts,
                                                       uint8_t debtType,
                                                       frame_info_t *frame_info );
 
/** @brief This command is sent from a client to the Prepayment server to set the maximum credit level allowed in the meter.
 *
 * Cluster: Prepayment, The Prepayment Cluster provides the facility to pass messages relating to prepayment between devices on the HAN.
 * Command: SetMaximumCreditLimit
 * @param providerId uint32_t
 * @param issuerEventId uint32_t
 * @param implementationDateTime uint32_t
 * @param maximumCreditLevel uint32_t
 * @param maximumCreditPerTopUp uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPrepaymentClusterSetMaximumCreditLimit(uint32_t providerId,
                                                         uint32_t issuerEventId,
                                                         uint32_t implementationDateTime,
                                                         uint32_t maximumCreditLevel,
                                                         uint32_t maximumCreditPerTopUp,
                                                         frame_info_t *frame_info );
 
/** @brief This command is sent from a client to the Prepayment server to set the overall debt cap allowed in the meter.
 *
 * Cluster: Prepayment, The Prepayment Cluster provides the facility to pass messages relating to prepayment between devices on the HAN.
 * Command: SetOverallDebtCap
 * @param providerId uint32_t
 * @param issuerEventId uint32_t
 * @param implementationDateTime uint32_t
 * @param overallDebtCap uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPrepaymentClusterSetOverallDebtCap(uint32_t providerId,
                                                     uint32_t issuerEventId,
                                                     uint32_t implementationDateTime,
                                                     uint32_t overallDebtCap,
                                                     frame_info_t *frame_info );
 
/** @brief This command is generated in response to a GetPrepaySnapshot command. It is used to 
	return a single snapshot to the client.
 *
 * Cluster: Prepayment, The Prepayment Cluster provides the facility to pass messages relating to prepayment between devices on the HAN.
 * Command: PublishPrepaySnapshot
 * @param snapshotId uint32_t
 * @param snapshotTime uint32_t
 * @param totalSnapshotsFound uint8_t
 * @param commandIndex uint8_t
 * @param totalNumberOfCommands uint8_t
 * @param snapshotCause uint32_t
 * @param snapshotPayloadType uint8_t
 * @param snapshotPayload uint8_t*
 * @param snapshotPayloadLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPrepaymentClusterPublishPrepaySnapshot(uint32_t snapshotId,
                                                         uint32_t snapshotTime,
                                                         uint8_t totalSnapshotsFound,
                                                         uint8_t commandIndex,
                                                         uint8_t totalNumberOfCommands,
                                                         uint32_t snapshotCause,
                                                         uint8_t snapshotPayloadType,
                                                         uint8_t *snapshotPayload,
                                                         uint16_t snapshotPayloadLen,
                                                         frame_info_t *frame_info );
 
/** @brief This command is send in response to the ChangePaymentMode Command.
 *
 * Cluster: Prepayment, The Prepayment Cluster provides the facility to pass messages relating to prepayment between devices on the HAN.
 * Command: ChangePaymentModeResponse
 * @param friendlyCredit uint8_t
 * @param friendlyCreditCalendarId uint32_t
 * @param emergencyCreditLimit uint32_t
 * @param emergencyCreditThreshold uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPrepaymentClusterChangePaymentModeResponse(uint8_t friendlyCredit,
                                                             uint32_t friendlyCreditCalendarId,
                                                             uint32_t emergencyCreditLimit,
                                                             uint32_t emergencyCreditThreshold,
                                                             frame_info_t *frame_info );
 
/** @brief This command is send in response to the ConsumerTopUp Command.
 *
 * Cluster: Prepayment, The Prepayment Cluster provides the facility to pass messages relating to prepayment between devices on the HAN.
 * Command: ConsumerTopUpResponse
 * @param resultType uint8_t
 * @param topUpValue uint32_t
 * @param sourceOfTopUp uint8_t
 * @param creditRemaining uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPrepaymentClusterConsumerTopUpResponse(uint8_t resultType,
                                                         uint32_t topUpValue,
                                                         uint8_t sourceOfTopUp,
                                                         uint32_t creditRemaining,
                                                         frame_info_t *frame_info );
 
/** @brief This command is used to send the Top Up Code Log entries to the client.
 *
 * Cluster: Prepayment, The Prepayment Cluster provides the facility to pass messages relating to prepayment between devices on the HAN.
 * Command: PublishTopUpLog
 * @param commandIndex uint8_t
 * @param totalNumberOfCommands uint8_t
 * @param topUpPayload uint8_t*
 * @param topUpPayloadLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPrepaymentClusterPublishTopUpLog(uint8_t commandIndex,
                                                   uint8_t totalNumberOfCommands,
                                                   uint8_t *topUpPayload,
                                                   uint16_t topUpPayloadLen,
                                                   frame_info_t *frame_info );
 
/** @brief This command is used to send the contents of the Repayment Log.
 *
 * Cluster: Prepayment, The Prepayment Cluster provides the facility to pass messages relating to prepayment between devices on the HAN.
 * Command: PublishDebtLog
 * @param commandIndex uint8_t
 * @param totalNumberOfCommands uint8_t
 * @param debtPayload uint8_t*
 * @param debtPayloadLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPrepaymentClusterPublishDebtLog(uint8_t commandIndex,
                                                  uint8_t totalNumberOfCommands,
                                                  uint8_t *debtPayload,
                                                  uint16_t debtPayloadLen,
                                                  frame_info_t *frame_info );
 
/** @} END Prepayment Commands */

/** @name Energy Management Commands */
 
/** @brief The Manage Event command allows a remote device (such as an IHD or web portal ); to change the behavior of a DRLC cluster client when responding to a DRLC Load Control Event.
 *
 * Cluster: Energy Management, This cluster provides attributes and commands to assist applications in creating resource monitoring protocols.
 * Command: ManageEvent
 * @param issuerEventId uint32_t
 * @param deviceClass uint16_t
 * @param utilityEnrollmentGroup uint8_t
 * @param actionRequired uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandEnergyManagementClusterManageEvent(uint32_t issuerEventId,
                                                     uint16_t deviceClass,
                                                     uint8_t utilityEnrollmentGroup,
                                                     uint8_t actionRequired,
                                                     frame_info_t *frame_info );
 
/** @} END Energy Management Commands */

/** @name Calendar Commands */
 
/** @brief The PublishDayProfile command is published in response to a GetDayProfile command.
 *
 * Cluster: Calendar, This cluster provides attributes and commands to assist applications in developing time and date based protocol.
 * Command: PublishDayProfile
 * @param providerId uint32_t
 * @param issuerEventId uint32_t
 * @param issuerCalendarId uint32_t
 * @param dayId uint8_t
 * @param totalNumberOfScheduleEntries uint8_t
 * @param commandIndex uint8_t
 * @param totalNumberOfCommands uint8_t
 * @param calendarType uint8_t
 * @param dayScheduleEntries uint8_t*
 * @param dayScheduleEntriesLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandCalendarClusterPublishDayProfile(uint32_t providerId,
                                                   uint32_t issuerEventId,
                                                   uint32_t issuerCalendarId,
                                                   uint8_t dayId,
                                                   uint8_t totalNumberOfScheduleEntries,
                                                   uint8_t commandIndex,
                                                   uint8_t totalNumberOfCommands,
                                                   uint8_t calendarType,
                                                   uint8_t *dayScheduleEntries,
                                                   uint16_t dayScheduleEntriesLen,
                                                   frame_info_t *frame_info );
 
/** @brief The PublishWeekProfile command is published in response to a GetWeekProfile command.
 *
 * Cluster: Calendar, This cluster provides attributes and commands to assist applications in developing time and date based protocol.
 * Command: PublishWeekProfile
 * @param providerId uint32_t
 * @param issuerEventId uint32_t
 * @param issuerCalendarId uint32_t
 * @param weekId uint8_t
 * @param dayIdRefMonday uint8_t
 * @param dayIdRefTuesday uint8_t
 * @param dayIdRefWednesday uint8_t
 * @param dayIdRefThursday uint8_t
 * @param dayIdRefFriday uint8_t
 * @param dayIdRefSaturday uint8_t
 * @param dayIdRefSunday uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandCalendarClusterPublishWeekProfile(uint32_t providerId,
                                                    uint32_t issuerEventId,
                                                    uint32_t issuerCalendarId,
                                                    uint8_t weekId,
                                                    uint8_t dayIdRefMonday,
                                                    uint8_t dayIdRefTuesday,
                                                    uint8_t dayIdRefWednesday,
                                                    uint8_t dayIdRefThursday,
                                                    uint8_t dayIdRefFriday,
                                                    uint8_t dayIdRefSaturday,
                                                    uint8_t dayIdRefSunday,
                                                    frame_info_t *frame_info );
 
/** @brief The PublishSeasons command is published in response to a GetSeason command.
 *
 * Cluster: Calendar, This cluster provides attributes and commands to assist applications in developing time and date based protocol.
 * Command: PublishSeasons
 * @param providerId uint32_t
 * @param issuerEventId uint32_t
 * @param issuerCalendarId uint32_t
 * @param commandIndex uint8_t
 * @param totalNumberOfCommands uint8_t
 * @param seasonEntries uint8_t*
 * @param seasonEntriesLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandCalendarClusterPublishSeasons(uint32_t providerId,
                                                uint32_t issuerEventId,
                                                uint32_t issuerCalendarId,
                                                uint8_t commandIndex,
                                                uint8_t totalNumberOfCommands,
                                                uint8_t *seasonEntries,
                                                uint16_t seasonEntriesLen,
                                                frame_info_t *frame_info );
 
/** @brief The PublishSpecialDays command is published in response to a GetSpecialDays command or if a calendar update is available.
 *
 * Cluster: Calendar, This cluster provides attributes and commands to assist applications in developing time and date based protocol.
 * Command: PublishSpecialDays
 * @param providerId uint32_t
 * @param issuerEventId uint32_t
 * @param issuerCalendarId uint32_t
 * @param startTime uint32_t
 * @param calendarType uint8_t
 * @param totalNumberOfSpecialDays uint8_t
 * @param commandIndex uint8_t
 * @param totalNumberOfCommands uint8_t
 * @param specialDayEntries uint8_t*
 * @param specialDayEntriesLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandCalendarClusterPublishSpecialDays(uint32_t providerId,
                                                    uint32_t issuerEventId,
                                                    uint32_t issuerCalendarId,
                                                    uint32_t startTime,
                                                    uint8_t calendarType,
                                                    uint8_t totalNumberOfSpecialDays,
                                                    uint8_t commandIndex,
                                                    uint8_t totalNumberOfCommands,
                                                    uint8_t *specialDayEntries,
                                                    uint16_t specialDayEntriesLen,
                                                    frame_info_t *frame_info );
 
/** @brief The CancelCalendar command indicates that all data associated with a particular calendar instance should be discarded.
 *
 * Cluster: Calendar, This cluster provides attributes and commands to assist applications in developing time and date based protocol.
 * Command: CancelCalendar
 * @param providerId uint32_t
 * @param issuerCalendarId uint32_t
 * @param calendarType uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandCalendarClusterCancelCalendar(uint32_t providerId,
                                                uint32_t issuerCalendarId,
                                                uint8_t calendarType,
                                                frame_info_t *frame_info );
 
/** @brief This command initiates PublishCalendar command(s ); for scheduled Calendar updates.
 *
 * Cluster: Calendar, This cluster provides attributes and commands to assist applications in developing time and date based protocol.
 * Command: GetCalendar
 * @param earliestStartTime uint32_t
 * @param minIssuerEventId uint32_t
 * @param numberOfCalendars uint8_t
 * @param calendarType uint8_t
 * @param providerId uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandCalendarClusterGetCalendar(uint32_t earliestStartTime,
                                             uint32_t minIssuerEventId,
                                             uint8_t numberOfCalendars,
                                             uint8_t calendarType,
                                             uint32_t providerId,
                                             frame_info_t *frame_info );
 
/** @brief This command initiates one or more PublishDayProfile commands for the referenced Calendar.
 *
 * Cluster: Calendar, This cluster provides attributes and commands to assist applications in developing time and date based protocol.
 * Command: GetDayProfiles
 * @param providerId uint32_t
 * @param issuerCalendarId uint32_t
 * @param startDayId uint8_t
 * @param numberOfDays uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandCalendarClusterGetDayProfiles(uint32_t providerId,
                                                uint32_t issuerCalendarId,
                                                uint8_t startDayId,
                                                uint8_t numberOfDays,
                                                frame_info_t *frame_info );
 
/** @brief This command initiates one or more PublishWeekProfile commands for the referenced Calendar.
 *
 * Cluster: Calendar, This cluster provides attributes and commands to assist applications in developing time and date based protocol.
 * Command: GetWeekProfiles
 * @param providerId uint32_t
 * @param issuerCalendarId uint32_t
 * @param startWeekId uint8_t
 * @param numberOfWeeks uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandCalendarClusterGetWeekProfiles(uint32_t providerId,
                                                 uint32_t issuerCalendarId,
                                                 uint8_t startWeekId,
                                                 uint8_t numberOfWeeks,
                                                 frame_info_t *frame_info );
 
/** @brief This command initiates one or more PublishSeasons commands for the referenced Calendar.
 *
 * Cluster: Calendar, This cluster provides attributes and commands to assist applications in developing time and date based protocol.
 * Command: GetSeasons
 * @param providerId uint32_t
 * @param issuerCalendarId uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandCalendarClusterGetSeasons(uint32_t providerId,
                                            uint32_t issuerCalendarId,
                                            frame_info_t *frame_info );
 
/** @brief This command initiates one or more PublishSpecialDays commands for the scheduled Special Day Table updates.
 *
 * Cluster: Calendar, This cluster provides attributes and commands to assist applications in developing time and date based protocol.
 * Command: GetSpecialDays
 * @param startTime uint32_t
 * @param numberOfEvents uint8_t
 * @param calendarType uint8_t
 * @param providerId uint32_t
 * @param issuerCalendarId uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandCalendarClusterGetSpecialDays(uint32_t startTime,
                                                uint8_t numberOfEvents,
                                                uint8_t calendarType,
                                                uint32_t providerId,
                                                uint32_t issuerCalendarId,
                                                frame_info_t *frame_info );
 
/** @brief This command initiates the 
	return of the last CancelCalendar command held on the associated server.
 *
 * Cluster: Calendar, This cluster provides attributes and commands to assist applications in developing time and date based protocol.
 * Command: GetCalendarCancellation
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandCalendarClusterGetCalendarCancellation(
    frame_info_t *frame_info );
 
/** @} END Calendar Commands */

/** @name Device Management Commands */
 
/** @brief This command is used to request the ESI to respond with information regarding any available change of supplier.
 *
 * Cluster: Device Management, This cluster provides attributes and commands to support device-cognisant application layer protocols.
 * Command: GetChangeOfSupplier
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDeviceManagementClusterGetChangeOfSupplier(
    frame_info_t *frame_info );
 
/** @brief This command is used to request the current password from the server.
 *
 * Cluster: Device Management, This cluster provides attributes and commands to support device-cognisant application layer protocols.
 * Command: RequestNewPassword
 * @param passwordType uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDeviceManagementClusterRequestNewPassword(uint8_t passwordType,
                                                            frame_info_t *frame_info );
 
/** @brief This command is used to request the ESI to respond with information regarding any pending change of Site ID.
 *
 * Cluster: Device Management, This cluster provides attributes and commands to support device-cognisant application layer protocols.
 * Command: GetSiteId
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDeviceManagementClusterGetSiteId(frame_info_t *frame_info );
 
/** @brief This command is sent in response to a GetEventConfiguration command.
 *
 * Cluster: Device Management, This cluster provides attributes and commands to support device-cognisant application layer protocols.
 * Command: ReportEventConfiguration
 * @param commandIndex uint8_t
 * @param totalCommands uint8_t
 * @param eventConfigurationPayload uint8_t*
 * @param eventConfigurationPayloadLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDeviceManagementClusterReportEventConfiguration(uint8_t commandIndex,
                                                                  uint8_t totalCommands,
                                                                  uint8_t *eventConfigurationPayload,
                                                                  uint16_t eventConfigurationPayloadLen,
                                                                  frame_info_t *frame_info );
 
/** @brief This command is used to request the ESI to respond with information regarding any pending change of Customer ID Number.
 *
 * Cluster: Device Management, This cluster provides attributes and commands to support device-cognisant application layer protocols.
 * Command: GetCIN
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDeviceManagementClusterGetCIN(
    frame_info_t *frame_info );
 
/** @brief This command is used to change the tenancy of a meter.
 *
 * Cluster: Device Management, This cluster provides attributes and commands to support device-cognisant application layer protocols.
 * Command: PublishChangeOfTenancy
 * @param providerId uint32_t
 * @param issuerEventId uint32_t
 * @param tariffType uint8_t
 * @param implementationDateTime uint32_t
 * @param proposedTenancyChangeControl uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDeviceManagementClusterPublishChangeOfTenancy(uint32_t providerId,
                                                                uint32_t issuerEventId,
                                                                uint8_t tariffType,
                                                                uint32_t implementationDateTime,
                                                                uint32_t proposedTenancyChangeControl,
                                                                frame_info_t *frame_info );
 
/** @brief This command is used to change the Supplier (energy supplier ); that is supplying the meter (s );.
 *
 * Cluster: Device Management, This cluster provides attributes and commands to support device-cognisant application layer protocols.
 * Command: PublishChangeOfSupplier
 * @param currentProviderId uint32_t
 * @param issuerEventId uint32_t
 * @param tariffType uint8_t
 * @param proposedProviderId uint32_t
 * @param providerChangeImplementationTime uint32_t
 * @param providerChangeControl uint32_t
 * @param proposedProviderName uint8_t*
 * @param proposedProviderContactDetails uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDeviceManagementClusterPublishChangeOfSupplier(uint32_t currentProviderId,
                                                                 uint32_t issuerEventId,
                                                                 uint8_t tariffType,
                                                                 uint32_t proposedProviderId,
                                                                 uint32_t providerChangeImplementationTime,
                                                                 uint32_t providerChangeControl,
                                                                 uint8_t *proposedProviderName,
                                                                 uint8_t *proposedProviderContactDetails,
                                                                 frame_info_t *frame_info );
 
/** @brief This command is used to send the current password to the client.
 *
 * Cluster: Device Management, This cluster provides attributes and commands to support device-cognisant application layer protocols.
 * Command: RequestNewPasswordResponse
 * @param issuerEventId uint32_t
 * @param implementationDateTime uint32_t
 * @param durationInMinutes uint16_t
 * @param passwordType uint8_t
 * @param password uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDeviceManagementClusterRequestNewPasswordResponse(uint32_t issuerEventId,
                                                                    uint32_t implementationDateTime,
                                                                    uint16_t durationInMinutes,
                                                                    uint8_t passwordType,
                                                                    uint8_t *password,
                                                                    frame_info_t *frame_info );
 
/** @brief This command is used to set the siteID.
 *
 * Cluster: Device Management, This cluster provides attributes and commands to support device-cognisant application layer protocols.
 * Command: UpdateSiteId
 * @param issuerEventId uint32_t
 * @param siteIdTime uint32_t
 * @param providerId uint32_t
 * @param siteId uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDeviceManagementClusterUpdateSiteId(uint32_t issuerEventId,
                                                      uint32_t siteIdTime,
                                                      uint32_t providerId,
                                                      uint8_t *siteId,
                                                      frame_info_t *frame_info );
 
/** @brief This command provides a method to set the event configuration attributes, held in a client device.
 *
 * Cluster: Device Management, This cluster provides attributes and commands to support device-cognisant application layer protocols.
 * Command: SetEventConfiguration
 * @param issuerEventId uint32_t
 * @param startDateTime uint32_t
 * @param eventConfiguration uint8_t
 * @param configurationControl uint8_t
 * @param eventConfigurationPayload uint8_t*
 * @param eventConfigurationPayloadLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDeviceManagementClusterSetEventConfiguration(uint32_t issuerEventId,
                                                               uint32_t startDateTime,
                                                               uint8_t eventConfiguration,
                                                               uint8_t configurationControl,
                                                               uint8_t *eventConfigurationPayload,
                                                               uint16_t eventConfigurationPayloadLen,
                                                               frame_info_t *frame_info );
 
/** @brief This command allows the server to request details of event configurations.
 *
 * Cluster: Device Management, This cluster provides attributes and commands to support device-cognisant application layer protocols.
 * Command: GetEventConfiguration
 * @param eventId uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDeviceManagementClusterGetEventConfiguration(uint16_t eventId,
                                                               frame_info_t *frame_info );
 
/** @brief This command is used to set the CustomerIDNumber attribute held in the Metering cluster.
 *
 * Cluster: Device Management, This cluster provides attributes and commands to support device-cognisant application layer protocols.
 * Command: UpdateCIN
 * @param issuerEventId uint32_t
 * @param implementationTime uint32_t
 * @param providerId uint32_t
 * @param customerIdNumber uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDeviceManagementClusterUpdateCIN(uint32_t issuerEventId,
                                                   uint32_t implementationTime,
                                                   uint32_t providerId,
                                                   uint8_t *customerIdNumber,
                                                   frame_info_t *frame_info );
 
/** @} END Device Management Commands */

/** @name Events Commands */
 
/** @brief The ClearEventLogRequest command requests that an Events server device clear the specified event log(s );.
 *
 * Cluster: Events, This cluster provides an interface on which applications can use event-based protocols.
 * Command: ClearEventLogRequest
 * @param logId uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandEventsClusterClearEventLogRequest(uint8_t logId,
                                                    frame_info_t *frame_info );
 
/** @brief The PublishEvent command is generated upon an event trigger from within the reporting device and, if supported, the associated Event Configuration attribute in the Device Management cluster.
 *
 * Cluster: Events, This cluster provides an interface on which applications can use event-based protocols.
 * Command: PublishEvent
 * @param logId uint8_t
 * @param eventId uint16_t
 * @param eventTime uint32_t
 * @param eventControl uint8_t
 * @param eventData uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandEventsClusterPublishEvent(uint8_t logId,
                                            uint16_t eventId,
                                            uint32_t eventTime,
                                            uint8_t eventControl,
                                            uint8_t *eventData,
                                            frame_info_t *frame_info );
 
/** @brief This command is generated on receipt of a GetEventLog command. The command 
	returns the most recent event first and up to the number of events requested.
 *
 * Cluster: Events, This cluster provides an interface on which applications can use event-based protocols.
 * Command: PublishEventLog
 * @param totalNumberOfEvents uint16_t
 * @param commandIndex uint8_t
 * @param totalCommands uint8_t
 * @param logPayloadControl uint8_t
 * @param logPayload uint8_t*
 * @param logPayloadLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandEventsClusterPublishEventLog(uint16_t totalNumberOfEvents,
                                               uint8_t commandIndex,
                                               uint8_t totalCommands,
                                               uint8_t logPayloadControl,
                                               uint8_t *logPayload,
                                               uint16_t logPayloadLen,
                                               frame_info_t *frame_info );
 
/** @brief This command is generated on receipt of a Clear Event Log Request command.
 *
 * Cluster: Events, This cluster provides an interface on which applications can use event-based protocols.
 * Command: ClearEventLogResponse
 * @param clearedEventsLogs uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandEventsClusterClearEventLogResponse(uint8_t clearedEventsLogs,
                                                     frame_info_t *frame_info );
 
/** @} END Events Commands */

/** @name MDU Pairing Commands */
 
/** @brief The Pairing Request command allows a device joining a MDU network to determine the devices that will constitute the 'virtual HAN' for the household in which it is to operate.
 *
 * Cluster: MDU Pairing, This cluster seeks to assist in the commissioning of networks that include multi-dwelling units (MDUs );.
 * Command: PairingRequest
 * @param localPairingInformationVersion uint32_t
 * @param eui64OfRequestingDevice uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandMduPairingClusterPairingRequest(uint32_t localPairingInformationVersion,
                                                  uint8_t *eui64OfRequestingDevice,
                                                  frame_info_t *frame_info );
 
/** @} END MDU Pairing Commands */

/** @name Sub-GHz Commands */
 
/** @brief The client sends it to determine the current status of its ZCL communications from the server.
 *
 * Cluster: Sub-GHz, Used by the Smart Energy profile for duty cycle monitoring and frequency agility.
 * Command: GetSuspendZclMessagesStatus
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSubGhzClusterGetSuspendZclMessagesStatus(
    frame_info_t *frame_info );
 
/** @} END Sub-GHz Commands */

/** @name Key Establishment Commands */
 
/** @brief Command description for EphemeralDataRequest
 *
 * Cluster: Key Establishment, Key Establishment cluster
 * Command: EphemeralDataRequest
 * @param ephemeralData uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandKeyEstablishmentClusterEphemeralDataRequest(uint8_t *ephemeralData,
                                                              frame_info_t *frame_info );
 
/** @brief Command description for ConfirmKeyDataRequest
 *
 * Cluster: Key Establishment, Key Establishment cluster
 * Command: ConfirmKeyDataRequest
 * @param secureMessageAuthenticationCode uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandKeyEstablishmentClusterConfirmKeyDataRequest(uint8_t *secureMessageAuthenticationCode,
                                                               frame_info_t *frame_info );
 
/** @brief Command description for TerminateKeyEstablishment
 *
 * Cluster: Key Establishment, Key Establishment cluster
 * Command: TerminateKeyEstablishment
 * @param statusCode uint8_t
 * @param waitTime uint8_t
 * @param keyEstablishmentSuite uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandKeyEstablishmentClusterServerToClientTerminateKeyEstablishment(uint8_t statusCode,
                                                                                 uint8_t waitTime,
                                                                                 uint16_t keyEstablishmentSuite,
                                                                                 frame_info_t *frame_info );
 
/** @brief Command description for TerminateKeyEstablishment
 *
 * Cluster: Key Establishment, Key Establishment cluster
 * Command: TerminateKeyEstablishment
 * @param statusCode uint8_t
 * @param waitTime uint8_t
 * @param keyEstablishmentSuite uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandKeyEstablishmentClusterClientToServerTerminateKeyEstablishment(uint8_t statusCode,
                                                                                 uint8_t waitTime,
                                                                                 uint16_t keyEstablishmentSuite,
                                                                                 frame_info_t *frame_info );
 
/** @brief Command description for InitiateKeyEstablishmentResponse
 *
 * Cluster: Key Establishment, Key Establishment cluster
 * Command: InitiateKeyEstablishmentResponse
 * @param requestedKeyEstablishmentSuite uint16_t
 * @param ephemeralDataGenerateTime uint8_t
 * @param confirmKeyGenerateTime uint8_t
 * @param identity uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandKeyEstablishmentClusterInitiateKeyEstablishmentResponse(uint16_t requestedKeyEstablishmentSuite,
                                                                          uint8_t ephemeralDataGenerateTime,
                                                                          uint8_t confirmKeyGenerateTime,
                                                                          uint8_t *identity,
                                                                          frame_info_t *frame_info );
 
/** @brief Command description for EphemeralDataResponse
 *
 * Cluster: Key Establishment, Key Establishment cluster
 * Command: EphemeralDataResponse
 * @param ephemeralData uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandKeyEstablishmentClusterEphemeralDataResponse(uint8_t *ephemeralData,
                                                               frame_info_t *frame_info );
 
/** @brief Command description for ConfirmKeyDataResponse
 *
 * Cluster: Key Establishment, Key Establishment cluster
 * Command: ConfirmKeyDataResponse
 * @param secureMessageAuthenticationCode uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandKeyEstablishmentClusterConfirmKeyDataResponse(uint8_t *secureMessageAuthenticationCode,
                                                                frame_info_t *frame_info );
 
/** @} END Key Establishment Commands */

/** @name Information Commands */
 
/** @brief Command description for PushInformationResponse
 *
 * Cluster: Information, Provides commands and attributes for information delivery service on ZigBee networks.
 * Command: PushInformationResponse
 * @param notificationList uint8_t*
 * @param notificationListLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandInformationClusterPushInformationResponse(uint8_t *notificationList,
                                                            uint16_t notificationListLen,
                                                            frame_info_t *frame_info );
 
/** @brief Command description for SendPreference
 *
 * Cluster: Information, Provides commands and attributes for information delivery service on ZigBee networks.
 * Command: SendPreference
 * @param preferenceType uint16_t
 * @param preferencePayload uint8_t*
 * @param preferencePayloadLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandInformationClusterSendPreference(uint16_t preferenceType,
                                                   uint8_t *preferencePayload,
                                                   uint16_t preferencePayloadLen,
                                                   frame_info_t *frame_info );
 
/** @brief Command description for RequestPreferenceResponse
 *
 * Cluster: Information, Provides commands and attributes for information delivery service on ZigBee networks.
 * Command: RequestPreferenceResponse
 * @param statusFeedback uint8_t
 * @param preferenceType uint16_t
 * @param preferencePayload uint8_t*
 * @param preferencePayloadLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandInformationClusterRequestPreferenceResponse(uint8_t statusFeedback,
                                                              uint16_t preferenceType,
                                                              uint8_t *preferencePayload,
                                                              uint16_t preferencePayloadLen,
                                                              frame_info_t *frame_info );
 
/** @brief Command description for Update
 *
 * Cluster: Information, Provides commands and attributes for information delivery service on ZigBee networks.
 * Command: Update
 * @param accessControl uint8_t
 * @param option uint8_t
 * @param contents uint8_t*
 * @param contentsLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandInformationClusterUpdate(uint8_t accessControl,
                                           uint8_t option,
                                           uint8_t *contents,
                                           uint16_t contentsLen,
                                           frame_info_t *frame_info );
 
/** @brief Command description for Delete
 *
 * Cluster: Information, Provides commands and attributes for information delivery service on ZigBee networks.
 * Command: Delete
 * @param deletionOptions uint8_t
 * @param contentIds uint8_t*
 * @param contentIdsLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandInformationClusterDelete(uint8_t deletionOptions,
                                           uint8_t *contentIds,
                                           uint16_t contentIdsLen,
                                           frame_info_t *frame_info );
 
/** @brief Command description for ConfigureNodeDescription
 *
 * Cluster: Information, Provides commands and attributes for information delivery service on ZigBee networks.
 * Command: ConfigureNodeDescription
 * @param description uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandInformationClusterConfigureNodeDescription(uint8_t *description,
                                                             frame_info_t *frame_info );
 
/** @brief Command description for ConfigureDeliveryEnable
 *
 * Cluster: Information, Provides commands and attributes for information delivery service on ZigBee networks.
 * Command: ConfigureDeliveryEnable
 * @param enable uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandInformationClusterConfigureDeliveryEnable(uint8_t enable,
                                                            frame_info_t *frame_info );
 
/** @brief Command description for ConfigurePushInformationTimer
 *
 * Cluster: Information, Provides commands and attributes for information delivery service on ZigBee networks.
 * Command: ConfigurePushInformationTimer
 * @param timer uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandInformationClusterConfigurePushInformationTimer(uint32_t timer,
                                                                  frame_info_t *frame_info );
 
/** @brief Command description for ConfigureSetRootId
 *
 * Cluster: Information, Provides commands and attributes for information delivery service on ZigBee networks.
 * Command: ConfigureSetRootId
 * @param rootId uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandInformationClusterConfigureSetRootId(uint16_t rootId,
                                                       frame_info_t *frame_info );
 
/** @brief Command description for RequestInformationResponse
 *
 * Cluster: Information, Provides commands and attributes for information delivery service on ZigBee networks.
 * Command: RequestInformationResponse
 * @param number uint8_t
 * @param buffer uint8_t*
 * @param bufferLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandInformationClusterRequestInformationResponse(uint8_t number,
                                                               uint8_t *buffer,
                                                               uint16_t bufferLen,
                                                               frame_info_t *frame_info );
 
/** @brief Command description for PushInformation
 *
 * Cluster: Information, Provides commands and attributes for information delivery service on ZigBee networks.
 * Command: PushInformation
 * @param contents uint8_t*
 * @param contentsLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandInformationClusterPushInformation(uint8_t *contents,
                                                    uint16_t contentsLen,
                                                    frame_info_t *frame_info );
 
/** @brief Command description for SendPreferenceResponse
 *
 * Cluster: Information, Provides commands and attributes for information delivery service on ZigBee networks.
 * Command: SendPreferenceResponse
 * @param statusFeedbackList uint8_t*
 * @param statusFeedbackListLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandInformationClusterSendPreferenceResponse(uint8_t *statusFeedbackList,
                                                           uint16_t statusFeedbackListLen,
                                                           frame_info_t *frame_info );
 
/** @brief Command description for ServerRequestPreference
 *
 * Cluster: Information, Provides commands and attributes for information delivery service on ZigBee networks.
 * Command: ServerRequestPreference
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandInformationClusterServerRequestPreference(
    frame_info_t *frame_info );
 
/** @brief Command description for RequestPreferenceConfirmation
 *
 * Cluster: Information, Provides commands and attributes for information delivery service on ZigBee networks.
 * Command: RequestPreferenceConfirmation
 * @param statusFeedbackList uint8_t*
 * @param statusFeedbackListLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandInformationClusterRequestPreferenceConfirmation(uint8_t *statusFeedbackList,
                                                                  uint16_t statusFeedbackListLen,
                                                                  frame_info_t *frame_info );
 
/** @brief Command description for UpdateResponse
 *
 * Cluster: Information, Provides commands and attributes for information delivery service on ZigBee networks.
 * Command: UpdateResponse
 * @param notificationList uint8_t*
 * @param notificationListLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandInformationClusterUpdateResponse(uint8_t *notificationList,
                                                   uint16_t notificationListLen,
                                                   frame_info_t *frame_info );
 
/** @brief Command description for DeleteResponse
 *
 * Cluster: Information, Provides commands and attributes for information delivery service on ZigBee networks.
 * Command: DeleteResponse
 * @param notificationList uint8_t*
 * @param notificationListLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandInformationClusterDeleteResponse(uint8_t *notificationList,
                                                   uint16_t notificationListLen,
                                                   frame_info_t *frame_info );
 
/** @} END Information Commands */

/** @name Data Sharing Commands */
 
/** @brief Command description for ReadRecordRequest
 *
 * Cluster: Data Sharing, Commands and attributes for small data sharing among ZigBee devices.
 * Command: ReadRecordRequest
 * @param fileIndex uint16_t
 * @param fileStartRecordAndRequestedRecordCount uint8_t*
 * @param fileStartRecordAndRequestedRecordCountLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDataSharingClusterReadRecordRequest(uint16_t fileIndex,
                                                      uint8_t *fileStartRecordAndRequestedRecordCount,
                                                      uint16_t fileStartRecordAndRequestedRecordCountLen,
                                                      frame_info_t *frame_info );
 
/** @brief Command description for WriteFileResponse
 *
 * Cluster: Data Sharing, Commands and attributes for small data sharing among ZigBee devices.
 * Command: WriteFileResponse
 * @param status uint8_t
 * @param fileIndex uint8_t*
 * @param fileIndexLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDataSharingClusterWriteFileResponse(uint8_t status,
                                                      uint8_t *fileIndex,
                                                      uint16_t fileIndexLen,
                                                      frame_info_t *frame_info );
 
/** @brief Command description for WriteFileRequest
 *
 * Cluster: Data Sharing, Commands and attributes for small data sharing among ZigBee devices.
 * Command: WriteFileRequest
 * @param writeOptions uint8_t
 * @param fileSize uint8_t*
 * @param fileSizeLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDataSharingClusterWriteFileRequest(uint8_t writeOptions,
                                                     uint8_t *fileSize,
                                                     uint16_t fileSizeLen,
                                                     frame_info_t *frame_info );
 
/** @brief Command description for ModifyFileRequest
 *
 * Cluster: Data Sharing, Commands and attributes for small data sharing among ZigBee devices.
 * Command: ModifyFileRequest
 * @param fileIndex uint16_t
 * @param fileStartPosition uint32_t
 * @param octetCount uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDataSharingClusterModifyFileRequest(uint16_t fileIndex,
                                                      uint32_t fileStartPosition,
                                                      uint32_t octetCount,
                                                      frame_info_t *frame_info );
 
/** @brief Command description for ModifyRecordRequest
 *
 * Cluster: Data Sharing, Commands and attributes for small data sharing among ZigBee devices.
 * Command: ModifyRecordRequest
 * @param fileIndex uint16_t
 * @param fileStartRecord uint16_t
 * @param recordCount uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDataSharingClusterModifyRecordRequest(uint16_t fileIndex,
                                                        uint16_t fileStartRecord,
                                                        uint16_t recordCount,
                                                        frame_info_t *frame_info );
 
/** @brief Command description for FileTransmission
 *
 * Cluster: Data Sharing, Commands and attributes for small data sharing among ZigBee devices.
 * Command: FileTransmission
 * @param transmitOptions uint8_t
 * @param buffer uint8_t*
 * @param bufferLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDataSharingClusterFileTransmission(uint8_t transmitOptions,
                                                     uint8_t *buffer,
                                                     uint16_t bufferLen,
                                                     frame_info_t *frame_info );
 
/** @brief Command description for RecordTransmission
 *
 * Cluster: Data Sharing, Commands and attributes for small data sharing among ZigBee devices.
 * Command: RecordTransmission
 * @param transmitOptions uint8_t
 * @param buffer uint8_t*
 * @param bufferLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDataSharingClusterRecordTransmission(uint8_t transmitOptions,
                                                       uint8_t *buffer,
                                                       uint16_t bufferLen,
                                                       frame_info_t *frame_info );
 
/** @} END Data Sharing Commands */

/** @name Gaming Commands */
 
/** @brief Command description for JoinGame
 *
 * Cluster: Gaming, Attributes and commands to support gaming functions of ZigBee-enabled mobile terminals.
 * Command: JoinGame
 * @param gameId uint16_t
 * @param joinAsMaster uint8_t
 * @param nameOfGame uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandGamingClusterJoinGame(uint16_t gameId,
                                        uint8_t joinAsMaster,
                                        uint8_t *nameOfGame,
                                        frame_info_t *frame_info );
 
/** @brief Command description for StartGame
 *
 * Cluster: Gaming, Attributes and commands to support gaming functions of ZigBee-enabled mobile terminals.
 * Command: StartGame
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandGamingClusterStartGame(
    frame_info_t *frame_info );
 
/** @brief Command description for PauseGame
 *
 * Cluster: Gaming, Attributes and commands to support gaming functions of ZigBee-enabled mobile terminals.
 * Command: PauseGame
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandGamingClusterPauseGame(
    frame_info_t *frame_info );
 
/** @brief Command description for ResumeGame
 *
 * Cluster: Gaming, Attributes and commands to support gaming functions of ZigBee-enabled mobile terminals.
 * Command: ResumeGame
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandGamingClusterResumeGame(
    frame_info_t *frame_info );
 
/** @brief Command description for QuitGame
 *
 * Cluster: Gaming, Attributes and commands to support gaming functions of ZigBee-enabled mobile terminals.
 * Command: QuitGame
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandGamingClusterQuitGame(
    frame_info_t *frame_info );
 
/** @brief Command description for EndGame
 *
 * Cluster: Gaming, Attributes and commands to support gaming functions of ZigBee-enabled mobile terminals.
 * Command: EndGame
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandGamingClusterEndGame(
    frame_info_t *frame_info );
 
/** @brief Command description for StartOver
 *
 * Cluster: Gaming, Attributes and commands to support gaming functions of ZigBee-enabled mobile terminals.
 * Command: StartOver
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandGamingClusterStartOver(
    frame_info_t *frame_info );
 
/** @brief Command description for ActionControl
 *
 * Cluster: Gaming, Attributes and commands to support gaming functions of ZigBee-enabled mobile terminals.
 * Command: ActionControl
 * @param actions uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandGamingClusterActionControl(uint32_t actions,
                                             frame_info_t *frame_info );
 
/** @brief Command description for DownloadGame
 *
 * Cluster: Gaming, Attributes and commands to support gaming functions of ZigBee-enabled mobile terminals.
 * Command: DownloadGame
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandGamingClusterDownloadGame(
    frame_info_t *frame_info );
 
/** @brief Command description for GameAnnouncement
 *
 * Cluster: Gaming, Attributes and commands to support gaming functions of ZigBee-enabled mobile terminals.
 * Command: GameAnnouncement
 * @param gameId uint16_t
 * @param gameMaster uint8_t
 * @param listOfGame uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandGamingClusterGameAnnouncement(uint16_t gameId,
                                                uint8_t gameMaster,
                                                uint8_t *listOfGame,
                                                frame_info_t *frame_info );
 
/** @brief Command description for GeneralResponse
 *
 * Cluster: Gaming, Attributes and commands to support gaming functions of ZigBee-enabled mobile terminals.
 * Command: GeneralResponse
 * @param commandId uint8_t
 * @param status uint8_t
 * @param message uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandGamingClusterGeneralResponse(uint8_t commandId,
                                               uint8_t status,
                                               uint8_t *message,
                                               frame_info_t *frame_info );
 
/** @} END Gaming Commands */

/** @name Data Rate Control Commands */
 
/** @brief Command description for DataRateNotification
 *
 * Cluster: Data Rate Control, This cluster seeks to give applications a means to managing data rate. It provides commands and attributes which form this interface.
 * Command: DataRateNotification
 * @param originatorAddress uint16_t
 * @param destinationAddress uint16_t
 * @param dataRate uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDataRateControlClusterDataRateNotification(uint16_t originatorAddress,
                                                             uint16_t destinationAddress,
                                                             uint8_t dataRate,
                                                             frame_info_t *frame_info );
 
/** @brief Command description for PathDeletion
 *
 * Cluster: Data Rate Control, This cluster seeks to give applications a means to managing data rate. It provides commands and attributes which form this interface.
 * Command: PathDeletion
 * @param originatorAddress uint16_t
 * @param destinationAddress uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDataRateControlClusterPathDeletion(uint16_t originatorAddress,
                                                     uint16_t destinationAddress,
                                                     frame_info_t *frame_info );
 
/** @brief Command description for DataRateControl
 *
 * Cluster: Data Rate Control, This cluster seeks to give applications a means to managing data rate. It provides commands and attributes which form this interface.
 * Command: DataRateControl
 * @param originatorAddress uint16_t
 * @param destinationAddress uint16_t
 * @param dataRate uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandDataRateControlClusterDataRateControl(uint16_t originatorAddress,
                                                        uint16_t destinationAddress,
                                                        uint8_t dataRate,
                                                        frame_info_t *frame_info );
 
/** @} END Data Rate Control Commands */

/** @name Voice over ZigBee Commands */
 
/** @brief Command description for VoiceTransmission
 *
 * Cluster: Voice over ZigBee, This cluster seeks to provide an interface to a voice over ZigBee protocol.
 * Command: VoiceTransmission
 * @param voiceData uint8_t*
 * @param voiceDataLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandVoiceOverZigbeeClusterVoiceTransmission(uint8_t *voiceData,
                                                          uint16_t voiceDataLen,
                                                          frame_info_t *frame_info );
 
/** @brief Command description for VoiceTransmissionCompletion
 *
 * Cluster: Voice over ZigBee, This cluster seeks to provide an interface to a voice over ZigBee protocol.
 * Command: VoiceTransmissionCompletion
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandVoiceOverZigbeeClusterVoiceTransmissionCompletion(
    frame_info_t *frame_info );
 
/** @brief Command description for ControlResponse
 *
 * Cluster: Voice over ZigBee, This cluster seeks to provide an interface to a voice over ZigBee protocol.
 * Command: ControlResponse
 * @param ackNack uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandVoiceOverZigbeeClusterControlResponse(uint8_t ackNack,
                                                        frame_info_t *frame_info );
 
/** @brief Command description for EstablishmentResponse
 *
 * Cluster: Voice over ZigBee, This cluster seeks to provide an interface to a voice over ZigBee protocol.
 * Command: EstablishmentResponse
 * @param ackNack uint8_t
 * @param codecType uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandVoiceOverZigbeeClusterEstablishmentResponse(uint8_t ackNack,
                                                              uint8_t codecType,
                                                              frame_info_t *frame_info );
 
/** @brief Command description for VoiceTransmissionResponse
 *
 * Cluster: Voice over ZigBee, This cluster seeks to provide an interface to a voice over ZigBee protocol.
 * Command: VoiceTransmissionResponse
 * @param sequenceNumber uint8_t
 * @param errorFlag uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandVoiceOverZigbeeClusterVoiceTransmissionResponse(uint8_t sequenceNumber,
                                                                  uint8_t errorFlag,
                                                                  frame_info_t *frame_info );
 
/** @brief Command description for Control
 *
 * Cluster: Voice over ZigBee, This cluster seeks to provide an interface to a voice over ZigBee protocol.
 * Command: Control
 * @param controlType uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandVoiceOverZigbeeClusterControl(uint8_t controlType,
                                                frame_info_t *frame_info );
 
/** @} END Voice over ZigBee Commands */

/** @name Chatting Commands */
 
/** @brief Command description for LeaveChatRequest
 *
 * Cluster: Chatting, Commands and attributes for sending chat messages among ZigBee devices.
 * Command: LeaveChatRequest
 * @param cid uint16_t
 * @param uid uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandChattingClusterLeaveChatRequest(uint16_t cid,
                                                  uint16_t uid,
                                                  frame_info_t *frame_info );
 
/** @brief Command description for SearchChatRequest
 *
 * Cluster: Chatting, Commands and attributes for sending chat messages among ZigBee devices.
 * Command: SearchChatRequest
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandChattingClusterSearchChatRequest(
    frame_info_t *frame_info );
 
/** @brief Command description for SwitchChairmanResponse
 *
 * Cluster: Chatting, Commands and attributes for sending chat messages among ZigBee devices.
 * Command: SwitchChairmanResponse
 * @param cid uint16_t
 * @param uid uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandChattingClusterSwitchChairmanResponse(uint16_t cid,
                                                        uint16_t uid,
                                                        frame_info_t *frame_info );
 
/** @brief Command description for StartChatRequest
 *
 * Cluster: Chatting, Commands and attributes for sending chat messages among ZigBee devices.
 * Command: StartChatRequest
 * @param name uint8_t*
 * @param uid uint16_t
 * @param nickname uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandChattingClusterStartChatRequest(uint8_t *name,
                                                  uint16_t uid,
                                                  uint8_t *nickname,
                                                  frame_info_t *frame_info );
 
/** @brief Command description for ChatMessage
 *
 * Cluster: Chatting, Commands and attributes for sending chat messages among ZigBee devices.
 * Command: ChatMessage
 * @param destinationUid uint16_t
 * @param sourceUid uint16_t
 * @param cid uint16_t
 * @param nickname uint8_t*
 * @param message uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandChattingClusterChatMessage(uint16_t destinationUid,
                                             uint16_t sourceUid,
                                             uint16_t cid,
                                             uint8_t *nickname,
                                             uint8_t *message,
                                             frame_info_t *frame_info );
 
/** @brief Command description for GetNodeInformationRequest
 *
 * Cluster: Chatting, Commands and attributes for sending chat messages among ZigBee devices.
 * Command: GetNodeInformationRequest
 * @param cid uint16_t
 * @param uid uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandChattingClusterGetNodeInformationRequest(uint16_t cid,
                                                           uint16_t uid,
                                                           frame_info_t *frame_info );
 
/** @brief Command description for StartChatResponse
 *
 * Cluster: Chatting, Commands and attributes for sending chat messages among ZigBee devices.
 * Command: StartChatResponse
 * @param status uint8_t
 * @param cid uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandChattingClusterStartChatResponse(uint8_t status,
                                                   uint16_t cid,
                                                   frame_info_t *frame_info );
 
/** @brief Command description for JoinChatResponse
 *
 * Cluster: Chatting, Commands and attributes for sending chat messages among ZigBee devices.
 * Command: JoinChatResponse
 * @param status uint8_t
 * @param cid uint16_t
 * @param chatParticipantList uint8_t*
 * @param chatParticipantListLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandChattingClusterJoinChatResponse(uint8_t status,
                                                  uint16_t cid,
                                                  uint8_t *chatParticipantList,
                                                  uint16_t chatParticipantListLen,
                                                  frame_info_t *frame_info );
 
/** @brief Command description for UserLeft
 *
 * Cluster: Chatting, Commands and attributes for sending chat messages among ZigBee devices.
 * Command: UserLeft
 * @param cid uint16_t
 * @param uid uint16_t
 * @param nickname uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandChattingClusterUserLeft(uint16_t cid,
                                          uint16_t uid,
                                          uint8_t *nickname,
                                          frame_info_t *frame_info );
 
/** @brief Command description for UserJoined
 *
 * Cluster: Chatting, Commands and attributes for sending chat messages among ZigBee devices.
 * Command: UserJoined
 * @param cid uint16_t
 * @param uid uint16_t
 * @param nickname uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandChattingClusterUserJoined(uint16_t cid,
                                            uint16_t uid,
                                            uint8_t *nickname,
                                            frame_info_t *frame_info );
 
/** @brief Command description for SearchChatResponse
 *
 * Cluster: Chatting, Commands and attributes for sending chat messages among ZigBee devices.
 * Command: SearchChatResponse
 * @param options uint8_t
 * @param chatRoomList uint8_t*
 * @param chatRoomListLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandChattingClusterSearchChatResponse(uint8_t options,
                                                    uint8_t *chatRoomList,
                                                    uint16_t chatRoomListLen,
                                                    frame_info_t *frame_info );
 
/** @brief Command description for SwitchChairmanRequest
 *
 * Cluster: Chatting, Commands and attributes for sending chat messages among ZigBee devices.
 * Command: SwitchChairmanRequest
 * @param cid uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandChattingClusterSwitchChairmanRequest(uint16_t cid,
                                                       frame_info_t *frame_info );
 
/** @brief Command description for SwitchChairmanConfirm
 *
 * Cluster: Chatting, Commands and attributes for sending chat messages among ZigBee devices.
 * Command: SwitchChairmanConfirm
 * @param cid uint16_t
 * @param nodeInformationList uint8_t*
 * @param nodeInformationListLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandChattingClusterSwitchChairmanConfirm(uint16_t cid,
                                                       uint8_t *nodeInformationList,
                                                       uint16_t nodeInformationListLen,
                                                       frame_info_t *frame_info );
 
/** @brief Command description for SwitchChairmanNotification
 *
 * Cluster: Chatting, Commands and attributes for sending chat messages among ZigBee devices.
 * Command: SwitchChairmanNotification
 * @param cid uint16_t
 * @param uid uint16_t
 * @param address uint16_t
 * @param endpoint uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandChattingClusterSwitchChairmanNotification(uint16_t cid,
                                                            uint16_t uid,
                                                            uint16_t address,
                                                            uint8_t endpoint,
                                                            frame_info_t *frame_info );
 
/** @brief Command description for GetNodeInformationResponse
 *
 * Cluster: Chatting, Commands and attributes for sending chat messages among ZigBee devices.
 * Command: GetNodeInformationResponse
 * @param status uint8_t
 * @param cid uint16_t
 * @param uid uint16_t
 * @param addressEndpointAndNickname uint8_t*
 * @param addressEndpointAndNicknameLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandChattingClusterGetNodeInformationResponse(uint8_t status,
                                                            uint16_t cid,
                                                            uint16_t uid,
                                                            uint8_t *addressEndpointAndNickname,
                                                            uint16_t addressEndpointAndNicknameLen,
                                                            frame_info_t *frame_info );
 
/** @} END Chatting Commands */

/** @name Payment Commands */
 
/** @brief Command description for AcceptPayment
 *
 * Cluster: Payment, Commands and attributes for payment scenarios including ZigBee devices.
 * Command: AcceptPayment
 * @param userId uint8_t*
 * @param userType uint16_t
 * @param serviceId uint16_t
 * @param goodId uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPaymentClusterAcceptPayment(uint8_t *userId,
                                              uint16_t userType,
                                              uint16_t serviceId,
                                              uint8_t *goodId,
                                              frame_info_t *frame_info );
 
/** @brief Command description for PaymentConfirm
 *
 * Cluster: Payment, Commands and attributes for payment scenarios including ZigBee devices.
 * Command: PaymentConfirm
 * @param serialNumber uint8_t*
 * @param transId uint16_t
 * @param transStatus uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPaymentClusterPaymentConfirm(uint8_t *serialNumber,
                                               uint16_t transId,
                                               uint8_t transStatus,
                                               frame_info_t *frame_info );
 
/** @brief Command description for BuyConfirm
 *
 * Cluster: Payment, Commands and attributes for payment scenarios including ZigBee devices.
 * Command: BuyConfirm
 * @param serialNumber uint8_t*
 * @param currency uint32_t
 * @param priceTrailingDigit uint8_t
 * @param price uint32_t
 * @param timestamp uint8_t*
 * @param transId uint16_t
 * @param transStatus uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPaymentClusterBuyConfirm(uint8_t *serialNumber,
                                           uint32_t currency,
                                           uint8_t priceTrailingDigit,
                                           uint32_t price,
                                           uint8_t *timestamp,
                                           uint16_t transId,
                                           uint8_t transStatus,
                                           frame_info_t *frame_info );
 
/** @brief Command description for ReceiptDelivery
 *
 * Cluster: Payment, Commands and attributes for payment scenarios including ZigBee devices.
 * Command: ReceiptDelivery
 * @param serialNumber uint8_t*
 * @param currency uint32_t
 * @param priceTrailingDigit uint8_t
 * @param price uint32_t
 * @param timestamp uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPaymentClusterReceiptDelivery(uint8_t *serialNumber,
                                                uint32_t currency,
                                                uint8_t priceTrailingDigit,
                                                uint32_t price,
                                                uint8_t *timestamp,
                                                frame_info_t *frame_info );
 
/** @brief Command description for TransactionEnd
 *
 * Cluster: Payment, Commands and attributes for payment scenarios including ZigBee devices.
 * Command: TransactionEnd
 * @param serialNumber uint8_t*
 * @param status uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandPaymentClusterTransactionEnd(uint8_t *serialNumber,
                                               uint8_t status,
                                               frame_info_t *frame_info );
 
/** @} END Payment Commands */

/** @name Billing Commands */
 
/** @brief Command description for Unsubscribe
 *
 * Cluster: Billing, Attributes and commands to enable billing of users for provided services through the use of a billing platform.
 * Command: Unsubscribe
 * @param userId uint8_t*
 * @param serviceId uint16_t
 * @param serviceProviderId uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandBillingClusterUnsubscribe(uint8_t *userId,
                                            uint16_t serviceId,
                                            uint16_t serviceProviderId,
                                            frame_info_t *frame_info );
 
/** @brief Command description for StartBillingSession
 *
 * Cluster: Billing, Attributes and commands to enable billing of users for provided services through the use of a billing platform.
 * Command: StartBillingSession
 * @param userId uint8_t*
 * @param serviceId uint16_t
 * @param serviceProviderId uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandBillingClusterStartBillingSession(uint8_t *userId,
                                                    uint16_t serviceId,
                                                    uint16_t serviceProviderId,
                                                    frame_info_t *frame_info );
 
/** @brief Command description for StopBillingSession
 *
 * Cluster: Billing, Attributes and commands to enable billing of users for provided services through the use of a billing platform.
 * Command: StopBillingSession
 * @param userId uint8_t*
 * @param serviceId uint16_t
 * @param serviceProviderId uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandBillingClusterStopBillingSession(uint8_t *userId,
                                                   uint16_t serviceId,
                                                   uint16_t serviceProviderId,
                                                   frame_info_t *frame_info );
 
/** @brief Command description for BillStatusNotification
 *
 * Cluster: Billing, Attributes and commands to enable billing of users for provided services through the use of a billing platform.
 * Command: BillStatusNotification
 * @param userId uint8_t*
 * @param status uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandBillingClusterBillStatusNotification(uint8_t *userId,
                                                       uint8_t status,
                                                       frame_info_t *frame_info );
 
/** @brief Command description for SessionKeepAlive
 *
 * Cluster: Billing, Attributes and commands to enable billing of users for provided services through the use of a billing platform.
 * Command: SessionKeepAlive
 * @param userId uint8_t*
 * @param serviceId uint16_t
 * @param serviceProviderId uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandBillingClusterSessionKeepAlive(uint8_t *userId,
                                                 uint16_t serviceId,
                                                 uint16_t serviceProviderId,
                                                 frame_info_t *frame_info );
 
/** @brief Command description for CheckBillStatus
 *
 * Cluster: Billing, Attributes and commands to enable billing of users for provided services through the use of a billing platform.
 * Command: CheckBillStatus
 * @param userId uint8_t*
 * @param serviceId uint16_t
 * @param serviceProviderId uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandBillingClusterCheckBillStatus(uint8_t *userId,
                                                uint16_t serviceId,
                                                uint16_t serviceProviderId,
                                                frame_info_t *frame_info );
 
/** @brief Command description for SendBillRecord
 *
 * Cluster: Billing, Attributes and commands to enable billing of users for provided services through the use of a billing platform.
 * Command: SendBillRecord
 * @param userId uint8_t*
 * @param serviceId uint16_t
 * @param serviceProviderId uint16_t
 * @param timestamp uint8_t*
 * @param duration uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandBillingClusterSendBillRecord(uint8_t *userId,
                                               uint16_t serviceId,
                                               uint16_t serviceProviderId,
                                               uint8_t *timestamp,
                                               uint16_t duration,
                                               frame_info_t *frame_info );
 
/** @} END Billing Commands */

/** @name Appliance Identification Commands */
/** @} END Appliance Identification Commands */

/** @name Meter Identification Commands */
/** @} END Meter Identification Commands */

/** @name Appliance Events and Alert Commands */
 
/** @brief This message is used to 
	return household appliance current alerts.
 *
 * Cluster: Appliance Events and Alert, Attributes and commands for transmitting or notifying the occurrence of an event, such as "temperature reached" and of an alert such as alarm, fault or warning.
 * Command: GetAlertsResponse
 * @param alertsCount uint8_t
 * @param alertStructures uint8_t*
 * @param alertStructuresLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandApplianceEventsAndAlertClusterGetAlertsResponse(uint8_t alertsCount,
                                                                  uint8_t *alertStructures,
                                                                  uint16_t alertStructuresLen,
                                                                  frame_info_t *frame_info );
 
/** @brief This message is used to notify the current modification of warning and/or fault conditions.
 *
 * Cluster: Appliance Events and Alert, Attributes and commands for transmitting or notifying the occurrence of an event, such as "temperature reached" and of an alert such as alarm, fault or warning.
 * Command: AlertsNotification
 * @param alertsCount uint8_t
 * @param alertStructures uint8_t*
 * @param alertStructuresLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandApplianceEventsAndAlertClusterAlertsNotification(uint8_t alertsCount,
                                                                   uint8_t *alertStructures,
                                                                   uint16_t alertStructuresLen,
                                                                   frame_info_t *frame_info );
 
/** @brief This message is used to notify an event occurred during the normal working of the appliance.
 *
 * Cluster: Appliance Events and Alert, Attributes and commands for transmitting or notifying the occurrence of an event, such as "temperature reached" and of an alert such as alarm, fault or warning.
 * Command: EventsNotification
 * @param eventHeader uint8_t
 * @param eventId uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandApplianceEventsAndAlertClusterEventsNotification(uint8_t eventHeader,
                                                                   uint8_t eventId,
                                                                   frame_info_t *frame_info );
 
/** @} END Appliance Events and Alert Commands */

/** @name Appliance Statistics Commands */
 
/** @brief The Appliance Statistics Cluster server sends out a Log Response command to respond to a Log Request command generated by the client side of the Appliance Statistics cluster.
 *
 * Cluster: Appliance Statistics, This cluster provides a mechanism for the transmitting appliance statistics to a collection unit (gateway );. The statistics can be in format of data logs. In case of statistic information that will not fit the single ZigBee payload, the Partition cluster should be used.
 * Command: LogResponse
 * @param timeStamp uint32_t
 * @param logId uint32_t
 * @param logLength uint32_t
 * @param logPayload uint8_t*
 * @param logPayloadLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandApplianceStatisticsClusterLogResponse(uint32_t timeStamp,
                                                        uint32_t logId,
                                                        uint32_t logLength,
                                                        uint8_t *logPayload,
                                                        uint16_t logPayloadLen,
                                                        frame_info_t *frame_info );
 
/** @brief The Log Queue Response command is generated as a response to a LogQueueRequest command in order to notify the client side of the Appliance statistics cluster about the logs stored in the server side (queue ); that can be retrieved by the client side of this cluster through a LogRequest command.
 *
 * Cluster: Appliance Statistics, This cluster provides a mechanism for the transmitting appliance statistics to a collection unit (gateway );. The statistics can be in format of data logs. In case of statistic information that will not fit the single ZigBee payload, the Partition cluster should be used.
 * Command: LogQueueResponse
 * @param logQueueSize uint8_t
 * @param logIds uint8_t*
 * @param logIdsLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandApplianceStatisticsClusterLogQueueResponse(uint8_t logQueueSize,
                                                             uint8_t *logIds,
                                                             uint16_t logIdsLen,
                                                             frame_info_t *frame_info );
 
/** @brief The Appliance Statistics Cluster server sends out a Statistic Available command to notify the client side of the Appliance Statistics cluster that there are statistics that can be retrieved by using the Log Request command.
 *
 * Cluster: Appliance Statistics, This cluster provides a mechanism for the transmitting appliance statistics to a collection unit (gateway );. The statistics can be in format of data logs. In case of statistic information that will not fit the single ZigBee payload, the Partition cluster should be used.
 * Command: StatisticsAvailable
 * @param logQueueSize uint8_t
 * @param logIds uint8_t*
 * @param logIdsLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandApplianceStatisticsClusterStatisticsAvailable(uint8_t logQueueSize,
                                                                uint8_t *logIds,
                                                                uint16_t logIdsLen,
                                                                frame_info_t *frame_info );
 
/** @brief The Log request command is sent from a device supporting the client side of the Appliance Statistics cluster (e.g., Home Gateway ); to retrieve the log from the device supporting the server side (e.g., appliance );.
 *
 * Cluster: Appliance Statistics, This cluster provides a mechanism for the transmitting appliance statistics to a collection unit (gateway );. The statistics can be in format of data logs. In case of statistic information that will not fit the single ZigBee payload, the Partition cluster should be used.
 * Command: LogRequest
 * @param logId uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandApplianceStatisticsClusterLogRequest(uint32_t logId,
                                                       frame_info_t *frame_info );
 
/** @brief The Log Queue Request command is send from a device supporting the client side of the Appliance Statistics cluster (e.g. Home Gateway ); to retrieve the information about the logs inserted in the queue, from the device supporting the server side (e.g. appliance );.
 *
 * Cluster: Appliance Statistics, This cluster provides a mechanism for the transmitting appliance statistics to a collection unit (gateway );. The statistics can be in format of data logs. In case of statistic information that will not fit the single ZigBee payload, the Partition cluster should be used.
 * Command: LogQueueRequest
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandApplianceStatisticsClusterLogQueueRequest(
    frame_info_t *frame_info );
 
/** @} END Appliance Statistics Commands */

/** @name Electrical Measurement Commands */
 
/** @brief A function which 
	returns the electricity measurement profile. The electricity measurement profile includes information regarding the amount of time used to capture data related to the flow of electricity as well as the intervals thes
 *
 * Cluster: Electrical Measurement, Attributes related to the electrical properties of a device. This cluster is used by power outlets and other devices that need to provide instantaneous data as opposed to metrology data which should be retrieved from the metering cluster..
 * Command: GetMeasurementProfileResponseCommand
 * @param startTime uint32_t
 * @param status uint8_t
 * @param profileIntervalPeriod uint8_t
 * @param numberOfIntervalsDelivered uint8_t
 * @param attributeId uint16_t
 * @param intervals uint8_t*
 * @param intervalsLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandElectricalMeasurementClusterGetMeasurementProfileResponseCommand(uint32_t startTime,
                                                                                   uint8_t status,
                                                                                   uint8_t profileIntervalPeriod,
                                                                                   uint8_t numberOfIntervalsDelivered,
                                                                                   uint16_t attributeId,
                                                                                   uint8_t *intervals,
                                                                                   uint16_t intervalsLen,
                                                                                   frame_info_t *frame_info );
 
/** @brief A function which retrieves the power profiling information from the electrical measurement server.
 *
 * Cluster: Electrical Measurement, Attributes related to the electrical properties of a device. This cluster is used by power outlets and other devices that need to provide instantaneous data as opposed to metrology data which should be retrieved from the metering cluster..
 * Command: GetProfileInfoCommand
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandElectricalMeasurementClusterGetProfileInfoCommand(
    frame_info_t *frame_info );
 
/** @brief A function which retrieves an electricity measurement profile from the electricity measurement server for a specific attribute Id requested.
 *
 * Cluster: Electrical Measurement, Attributes related to the electrical properties of a device. This cluster is used by power outlets and other devices that need to provide instantaneous data as opposed to metrology data which should be retrieved from the metering cluster..
 * Command: GetMeasurementProfileCommand
 * @param attributeId uint16_t
 * @param startTime uint32_t
 * @param numberOfIntervals uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandElectricalMeasurementClusterGetMeasurementProfileCommand(uint16_t attributeId,
                                                                           uint32_t startTime,
                                                                           uint8_t numberOfIntervals,
                                                                           frame_info_t *frame_info );
 
/** @} END Electrical Measurement Commands */

/** @name Diagnostics Commands */
/** @} END Diagnostics Commands */

/** @name ZLL Commissioning Commands */
 
/** @brief Command description for DeviceInformationRequest
 *
 * Cluster: ZLL Commissioning, The ZLL commissioning cluster provides commands to support touch link commissioning.
 * Command: DeviceInformationRequest
 * @param transaction uint32_t
 * @param startIndex uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandZllCommissioningClusterDeviceInformationRequest(uint32_t transaction,
                                                                  uint8_t startIndex,
                                                                  frame_info_t *frame_info );
 
/** @brief Command description for IdentifyRequest
 *
 * Cluster: ZLL Commissioning, The ZLL commissioning cluster provides commands to support touch link commissioning.
 * Command: IdentifyRequest
 * @param transaction uint32_t
 * @param identifyDuration uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandZllCommissioningClusterIdentifyRequest(uint32_t transaction,
                                                         uint16_t identifyDuration,
                                                         frame_info_t *frame_info );
 
/** @brief Command description for ResetToFactoryNewRequest
 *
 * Cluster: ZLL Commissioning, The ZLL commissioning cluster provides commands to support touch link commissioning.
 * Command: ResetToFactoryNewRequest
 * @param transaction uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandZllCommissioningClusterResetToFactoryNewRequest(uint32_t transaction,
                                                                  frame_info_t *frame_info );
 
/** @brief Command description for NetworkStartRequest
 *
 * Cluster: ZLL Commissioning, The ZLL commissioning cluster provides commands to support touch link commissioning.
 * Command: NetworkStartRequest
 * @param transaction uint32_t
 * @param extendedPanId uint8_t*
 * @param keyIndex uint8_t
 * @param encryptedNetworkKey uint8_t*
 * @param logicalChannel uint8_t
 * @param panId uint16_t
 * @param networkAddress uint16_t
 * @param groupIdentifiersBegin uint16_t
 * @param groupIdentifiersEnd uint16_t
 * @param freeNetworkAddressRangeBegin uint16_t
 * @param freeNetworkAddressRangeEnd uint16_t
 * @param freeGroupIdentifierRangeBegin uint16_t
 * @param freeGroupIdentifierRangeEnd uint16_t
 * @param initiatorIeeeAddress uint8_t*
 * @param initiatorNetworkAddress uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandZllCommissioningClusterNetworkStartRequest(uint32_t transaction,
                                                             uint8_t *extendedPanId,
                                                             uint8_t keyIndex,
                                                             uint8_t *encryptedNetworkKey,
                                                             uint8_t logicalChannel,
                                                             uint16_t panId,
                                                             uint16_t networkAddress,
                                                             uint16_t groupIdentifiersBegin,
                                                             uint16_t groupIdentifiersEnd,
                                                             uint16_t freeNetworkAddressRangeBegin,
                                                             uint16_t freeNetworkAddressRangeEnd,
                                                             uint16_t freeGroupIdentifierRangeBegin,
                                                             uint16_t freeGroupIdentifierRangeEnd,
                                                             uint8_t *initiatorIeeeAddress,
                                                             uint16_t initiatorNetworkAddress,
                                                             frame_info_t *frame_info );
 
/** @brief Command description for NetworkJoinRouterRequest
 *
 * Cluster: ZLL Commissioning, The ZLL commissioning cluster provides commands to support touch link commissioning.
 * Command: NetworkJoinRouterRequest
 * @param transaction uint32_t
 * @param extendedPanId uint8_t*
 * @param keyIndex uint8_t
 * @param encryptedNetworkKey uint8_t*
 * @param networkUpdateId uint8_t
 * @param logicalChannel uint8_t
 * @param panId uint16_t
 * @param networkAddress uint16_t
 * @param groupIdentifiersBegin uint16_t
 * @param groupIdentifiersEnd uint16_t
 * @param freeNetworkAddressRangeBegin uint16_t
 * @param freeNetworkAddressRangeEnd uint16_t
 * @param freeGroupIdentifierRangeBegin uint16_t
 * @param freeGroupIdentifierRangeEnd uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandZllCommissioningClusterNetworkJoinRouterRequest(uint32_t transaction,
                                                                  uint8_t *extendedPanId,
                                                                  uint8_t keyIndex,
                                                                  uint8_t *encryptedNetworkKey,
                                                                  uint8_t networkUpdateId,
                                                                  uint8_t logicalChannel,
                                                                  uint16_t panId,
                                                                  uint16_t networkAddress,
                                                                  uint16_t groupIdentifiersBegin,
                                                                  uint16_t groupIdentifiersEnd,
                                                                  uint16_t freeNetworkAddressRangeBegin,
                                                                  uint16_t freeNetworkAddressRangeEnd,
                                                                  uint16_t freeGroupIdentifierRangeBegin,
                                                                  uint16_t freeGroupIdentifierRangeEnd,
                                                                  frame_info_t *frame_info );
 
/** @brief Command description for NetworkJoinEndDeviceRequest
 *
 * Cluster: ZLL Commissioning, The ZLL commissioning cluster provides commands to support touch link commissioning.
 * Command: NetworkJoinEndDeviceRequest
 * @param transaction uint32_t
 * @param extendedPanId uint8_t*
 * @param keyIndex uint8_t
 * @param encryptedNetworkKey uint8_t*
 * @param networkUpdateId uint8_t
 * @param logicalChannel uint8_t
 * @param panId uint16_t
 * @param networkAddress uint16_t
 * @param groupIdentifiersBegin uint16_t
 * @param groupIdentifiersEnd uint16_t
 * @param freeNetworkAddressRangeBegin uint16_t
 * @param freeNetworkAddressRangeEnd uint16_t
 * @param freeGroupIdentifierRangeBegin uint16_t
 * @param freeGroupIdentifierRangeEnd uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandZllCommissioningClusterNetworkJoinEndDeviceRequest(uint32_t transaction,
                                                                     uint8_t *extendedPanId,
                                                                     uint8_t keyIndex,
                                                                     uint8_t *encryptedNetworkKey,
                                                                     uint8_t networkUpdateId,
                                                                     uint8_t logicalChannel,
                                                                     uint16_t panId,
                                                                     uint16_t networkAddress,
                                                                     uint16_t groupIdentifiersBegin,
                                                                     uint16_t groupIdentifiersEnd,
                                                                     uint16_t freeNetworkAddressRangeBegin,
                                                                     uint16_t freeNetworkAddressRangeEnd,
                                                                     uint16_t freeGroupIdentifierRangeBegin,
                                                                     uint16_t freeGroupIdentifierRangeEnd,
                                                                     frame_info_t *frame_info );
 
/** @brief Command description for NetworkUpdateRequest
 *
 * Cluster: ZLL Commissioning, The ZLL commissioning cluster provides commands to support touch link commissioning.
 * Command: NetworkUpdateRequest
 * @param transaction uint32_t
 * @param extendedPanId uint8_t*
 * @param networkUpdateId uint8_t
 * @param logicalChannel uint8_t
 * @param panId uint16_t
 * @param networkAddress uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandZllCommissioningClusterNetworkUpdateRequest(uint32_t transaction,
                                                              uint8_t *extendedPanId,
                                                              uint8_t networkUpdateId,
                                                              uint8_t logicalChannel,
                                                              uint16_t panId,
                                                              uint16_t networkAddress,
                                                              frame_info_t *frame_info );
 
/** @brief Command description for GetGroupIdentifiersRequest
 *
 * Cluster: ZLL Commissioning, The ZLL commissioning cluster provides commands to support touch link commissioning.
 * Command: GetGroupIdentifiersRequest
 * @param startIndex uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandZllCommissioningClusterGetGroupIdentifiersRequest(uint8_t startIndex,
                                                                    frame_info_t *frame_info );
 
/** @brief Command description for GetEndpointListRequest
 *
 * Cluster: ZLL Commissioning, The ZLL commissioning cluster provides commands to support touch link commissioning.
 * Command: GetEndpointListRequest
 * @param startIndex uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandZllCommissioningClusterGetEndpointListRequest(uint8_t startIndex,
                                                                frame_info_t *frame_info );
 
/** @brief Command description for ScanResponse
 *
 * Cluster: ZLL Commissioning, The ZLL commissioning cluster provides commands to support touch link commissioning.
 * Command: ScanResponse
 * @param transaction uint32_t
 * @param rssiCorrection uint8_t
 * @param zigbeeInformation uint8_t
 * @param zllInformation uint8_t
 * @param keyBitmask uint16_t
 * @param responseId uint32_t
 * @param extendedPanId uint8_t*
 * @param networkUpdateId uint8_t
 * @param logicalChannel uint8_t
 * @param panId uint16_t
 * @param networkAddress uint16_t
 * @param numberOfSubDevices uint8_t
 * @param totalGroupIds uint8_t
 * @param endpointId uint8_t
 * @param profileId uint16_t
 * @param deviceId uint16_t
 * @param version uint8_t
 * @param groupIdCount uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandZllCommissioningClusterScanResponse(uint32_t transaction,
                                                      uint8_t rssiCorrection,
                                                      uint8_t zigbeeInformation,
                                                      uint8_t zllInformation,
                                                      uint16_t keyBitmask,
                                                      uint32_t responseId,
                                                      uint8_t *extendedPanId,
                                                      uint8_t networkUpdateId,
                                                      uint8_t logicalChannel,
                                                      uint16_t panId,
                                                      uint16_t networkAddress,
                                                      uint8_t numberOfSubDevices,
                                                      uint8_t totalGroupIds,
                                                      uint8_t endpointId,
                                                      uint16_t profileId,
                                                      uint16_t deviceId,
                                                      uint8_t version,
                                                      uint8_t groupIdCount,
                                                      frame_info_t *frame_info );
 
/** @brief Command description for DeviceInformationResponse
 *
 * Cluster: ZLL Commissioning, The ZLL commissioning cluster provides commands to support touch link commissioning.
 * Command: DeviceInformationResponse
 * @param transaction uint32_t
 * @param numberOfSubDevices uint8_t
 * @param startIndex uint8_t
 * @param deviceInformationRecordCount uint8_t
 * @param deviceInformationRecordList uint8_t*
 * @param deviceInformationRecordListLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandZllCommissioningClusterDeviceInformationResponse(uint32_t transaction,
                                                                   uint8_t numberOfSubDevices,
                                                                   uint8_t startIndex,
                                                                   uint8_t deviceInformationRecordCount,
                                                                   uint8_t *deviceInformationRecordList,
                                                                   uint16_t deviceInformationRecordListLen,
                                                                   frame_info_t *frame_info );
 
/** @brief Command description for NetworkStartResponse
 *
 * Cluster: ZLL Commissioning, The ZLL commissioning cluster provides commands to support touch link commissioning.
 * Command: NetworkStartResponse
 * @param transaction uint32_t
 * @param status uint8_t
 * @param extendedPanId uint8_t*
 * @param networkUpdateId uint8_t
 * @param logicalChannel uint8_t
 * @param panId uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandZllCommissioningClusterNetworkStartResponse(uint32_t transaction,
                                                              uint8_t status,
                                                              uint8_t *extendedPanId,
                                                              uint8_t networkUpdateId,
                                                              uint8_t logicalChannel,
                                                              uint16_t panId,
                                                              frame_info_t *frame_info );
 
/** @brief Command description for NetworkJoinRouterResponse
 *
 * Cluster: ZLL Commissioning, The ZLL commissioning cluster provides commands to support touch link commissioning.
 * Command: NetworkJoinRouterResponse
 * @param transaction uint32_t
 * @param status uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandZllCommissioningClusterNetworkJoinRouterResponse(uint32_t transaction,
                                                                   uint8_t status,
                                                                   frame_info_t *frame_info );
 
/** @brief Command description for NetworkJoinEndDeviceResponse
 *
 * Cluster: ZLL Commissioning, The ZLL commissioning cluster provides commands to support touch link commissioning.
 * Command: NetworkJoinEndDeviceResponse
 * @param transaction uint32_t
 * @param status uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandZllCommissioningClusterNetworkJoinEndDeviceResponse(uint32_t transaction,
                                                                      uint8_t status,
                                                                      frame_info_t *frame_info );
 
/** @brief Command description for EndpointInformation
 *
 * Cluster: ZLL Commissioning, The ZLL commissioning cluster provides commands to support touch link commissioning.
 * Command: EndpointInformation
 * @param ieeeAddress uint8_t*
 * @param networkAddress uint16_t
 * @param endpointId uint8_t
 * @param profileId uint16_t
 * @param deviceId uint16_t
 * @param version uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandZllCommissioningClusterEndpointInformation(uint8_t *ieeeAddress,
                                                             uint16_t networkAddress,
                                                             uint8_t endpointId,
                                                             uint16_t profileId,
                                                             uint16_t deviceId,
                                                             uint8_t version,
                                                             frame_info_t *frame_info );
 
/** @brief Command description for GetGroupIdentifiersResponse
 *
 * Cluster: ZLL Commissioning, The ZLL commissioning cluster provides commands to support touch link commissioning.
 * Command: GetGroupIdentifiersResponse
 * @param total uint8_t
 * @param startIndex uint8_t
 * @param count uint8_t
 * @param groupInformationRecordList uint8_t*
 * @param groupInformationRecordListLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandZllCommissioningClusterGetGroupIdentifiersResponse(uint8_t total,
                                                                     uint8_t startIndex,
                                                                     uint8_t count,
                                                                     uint8_t *groupInformationRecordList,
                                                                     uint16_t groupInformationRecordListLen,
                                                                     frame_info_t *frame_info );
 
/** @brief Command description for GetEndpointListResponse
 *
 * Cluster: ZLL Commissioning, The ZLL commissioning cluster provides commands to support touch link commissioning.
 * Command: GetEndpointListResponse
 * @param total uint8_t
 * @param startIndex uint8_t
 * @param count uint8_t
 * @param endpointInformationRecordList uint8_t*
 * @param endpointInformationRecordListLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandZllCommissioningClusterGetEndpointListResponse(uint8_t total,
                                                                 uint8_t startIndex,
                                                                 uint8_t count,
                                                                 uint8_t *endpointInformationRecordList,
                                                                 uint16_t endpointInformationRecordListLen,
                                                                 frame_info_t *frame_info );
 
/** @} END ZLL Commissioning Commands */

/** @name Sample Mfg Specific Cluster Commands */
 
/** @} END Sample Mfg Specific Cluster Commands */

/** @name Configuration Cluster Commands */
 
/** @brief Command to lock the token values.
 *
 * Cluster: Configuration Cluster, This cluster allows for the OTA configuration of firmware
	  parameters.
 * Command: LockTokens
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandOtaConfigurationClusterLockTokens(
    frame_info_t *frame_info );
 
/** @brief Command to read a token value.
 *
 * Cluster: Configuration Cluster, This cluster allows for the OTA configuration of firmware
	  parameters.
 * Command: ReadTokens
 * @param token uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandOtaConfigurationClusterReadTokens(uint16_t token,
                                                    frame_info_t *frame_info );
 
/** @brief Command to unlock tokens with a device-specific password (if allowed );.
 *
 * Cluster: Configuration Cluster, This cluster allows for the OTA configuration of firmware
	  parameters.
 * Command: UnlockTokens
 * @param data uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandOtaConfigurationClusterUnlockTokens(uint8_t *data,
                                                      frame_info_t *frame_info );
 
/** @brief Response to a token value read.
 *
 * Cluster: Configuration Cluster, This cluster allows for the OTA configuration of firmware
	  parameters.
 * Command: ReturnToken
 * @param token uint16_t
 * @param data uint8_t*
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandOtaConfigurationClusterReturnToken(uint16_t token,
                                                     uint8_t *data,
                                                     frame_info_t *frame_info );
 
/** @} END Configuration Cluster Commands */

/** @name MFGLIB Cluster Commands */
 
/** @brief Command to put the device into tone mode.
 *
 * Cluster: MFGLIB Cluster, This cluster provides commands to kick off MFGLIB actions 
	  over the air.
 * Command: tone
 * @param channel uint8_t
 * @param power int8_t
 * @param time uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandMfglibClustertone(uint8_t channel,
                                    int8_t power,
                                    uint16_t time,
                                    frame_info_t *frame_info );
 
/** @brief Command to put the device into RX mode.
 *
 * Cluster: MFGLIB Cluster, This cluster provides commands to kick off MFGLIB actions 
	  over the air.
 * Command: rxMode
 * @param channel uint8_t
 * @param power int8_t
 * @param time uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandMfglibClusterrxMode(uint8_t channel,
                                      int8_t power,
                                      uint16_t time,
                                      frame_info_t *frame_info );
 
/** @} END MFGLIB Cluster Commands */

/** @name SL Works With All Hubs Commands */
 
/** @brief Disable enforcement of APS-level security for all cluster commands.
 *
 * Cluster: SL Works With All Hubs, Silicon Labs proprietary attributes and commands for Works With All Hubs functional extensions.
 * Command: DisableApsLinkKeyAuthorization
 * @param numberExemptClusters uint8_t
 * @param clusterId uint8_t*
 * @param clusterIdLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSlWwahClusterDisableApsLinkKeyAuthorization(uint8_t numberExemptClusters,
                                                              uint8_t *clusterId,
                                                              uint16_t clusterIdLen,
                                                              frame_info_t *frame_info );
 
/** @brief Query status of APS-level security enforcement for a specified cluster.
 *
 * Cluster: SL Works With All Hubs, Silicon Labs proprietary attributes and commands for Works With All Hubs functional extensions.
 * Command: ApsLinkKeyAuthorizationQuery
 * @param clusterId uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSlWwahClusterApsLinkKeyAuthorizationQuery(uint16_t clusterId,
                                                            frame_info_t *frame_info );
 
/** @brief Trigger device to request a new APS link key from the Trust Center.
 *
 * Cluster: SL Works With All Hubs, Silicon Labs proprietary attributes and commands for Works With All Hubs functional extensions.
 * Command: RequestNewApsLinkKey
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSlWwahClusterRequestNewApsLinkKey(
    frame_info_t *frame_info );
 
/** @brief Enable WWAH App Event retry algorithm.
 *
 * Cluster: SL Works With All Hubs, Silicon Labs proprietary attributes and commands for Works With All Hubs functional extensions.
 * Command: EnableWwahAppEventRetryAlgorithm
 * @param firstBackoffTimeSeconds uint8_t
 * @param backoffSeqCommonRatio uint8_t
 * @param maxBackoffTimeSeconds uint32_t
 * @param maxRedeliveryAttempts uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSlWwahClusterEnableWwahAppEventRetryAlgorithm(uint8_t firstBackoffTimeSeconds,
                                                                uint8_t backoffSeqCommonRatio,
                                                                uint32_t maxBackoffTimeSeconds,
                                                                uint8_t maxRedeliveryAttempts,
                                                                frame_info_t *frame_info );
 
/** @brief Disable WWAH App Event retry algorithm.
 *
 * Cluster: SL Works With All Hubs, Silicon Labs proprietary attributes and commands for Works With All Hubs functional extensions.
 * Command: DisableWwahAppEventRetryAlgorithm
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSlWwahClusterDisableWwahAppEventRetryAlgorithm(
    frame_info_t *frame_info );
 
/** @brief Trigger device to request current attribute values from Time Cluster server.
 *
 * Cluster: SL Works With All Hubs, Silicon Labs proprietary attributes and commands for Works With All Hubs functional extensions.
 * Command: RequestTime
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSlWwahClusterRequestTime(
    frame_info_t *frame_info );
 
/** @brief Enable WWAH rejoin algorithm.
 *
 * Cluster: SL Works With All Hubs, Silicon Labs proprietary attributes and commands for Works With All Hubs functional extensions.
 * Command: EnableWwahRejoinAlgorithm
 * @param fastRejoinTimeoutSeconds uint16_t
 * @param durationBetweenRejoinsSeconds uint16_t
 * @param fastRejoinFirstBackoffSeconds uint16_t
 * @param maxBackoffTimeSeconds uint16_t
 * @param maxBackoffIterations uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSlWwahClusterEnableWwahRejoinAlgorithm(uint16_t fastRejoinTimeoutSeconds,
                                                         uint16_t durationBetweenRejoinsSeconds,
                                                         uint16_t fastRejoinFirstBackoffSeconds,
                                                         uint16_t maxBackoffTimeSeconds,
                                                         uint16_t maxBackoffIterations,
                                                         frame_info_t *frame_info );
 
/** @brief Disable WWAH rejoin algorithm.
 *
 * Cluster: SL Works With All Hubs, Silicon Labs proprietary attributes and commands for Works With All Hubs functional extensions.
 * Command: DisableWwahRejoinAlgorithm
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSlWwahClusterDisableWwahRejoinAlgorithm(
    frame_info_t *frame_info );
 
/** @brief Set the enrollment method of an IAS Zone server.
 *
 * Cluster: SL Works With All Hubs, Silicon Labs proprietary attributes and commands for Works With All Hubs functional extensions.
 * Command: SetIasZoneEnrollmentMethod
 * @param enrollmentMode uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSlWwahClusterSetIasZoneEnrollmentMethod(uint8_t enrollmentMode,
                                                          frame_info_t *frame_info );
 
/** @brief Clear the binding table.
 *
 * Cluster: SL Works With All Hubs, Silicon Labs proprietary attributes and commands for Works With All Hubs functional extensions.
 * Command: ClearBindingTable
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSlWwahClusterClearBindingTable(
    frame_info_t *frame_info );
 
/** @brief Enable device to periodically check connectivity with Zigbee Coordinator.
 *
 * Cluster: SL Works With All Hubs, Silicon Labs proprietary attributes and commands for Works With All Hubs functional extensions.
 * Command: EnablePeriodicRouterCheckIns
 * @param checkInInterval uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSlWwahClusterEnablePeriodicRouterCheckIns(uint16_t checkInInterval,
                                                            frame_info_t *frame_info );
 
/** @brief Disable device from periodically checking connectivity with Zigbee Coordinator.
 *
 * Cluster: SL Works With All Hubs, Silicon Labs proprietary attributes and commands for Works With All Hubs functional extensions.
 * Command: DisablePeriodicRouterCheckIns
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSlWwahClusterDisablePeriodicRouterCheckIns(
    frame_info_t *frame_info );
 
/** @brief Set MAC poll CCA wait time.
 *
 * Cluster: SL Works With All Hubs, Silicon Labs proprietary attributes and commands for Works With All Hubs functional extensions.
 * Command: SetMacPollCcaWaitTime
 * @param waitTime uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSlWwahClusterSetMacPollCcaWaitTime(uint8_t waitTime,
                                                     frame_info_t *frame_info );
 
/** @brief Set pending network update parameters.
 *
 * Cluster: SL Works With All Hubs, Silicon Labs proprietary attributes and commands for Works With All Hubs functional extensions.
 * Command: SetPendingNetworkUpdate
 * @param channel uint8_t
 * @param panId uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSlWwahClusterSetPendingNetworkUpdate(uint8_t channel,
                                                       uint16_t panId,
                                                       frame_info_t *frame_info );
 
/** @brief Require all unicast commands to have APS ACKs enabled.
 *
 * Cluster: SL Works With All Hubs, Silicon Labs proprietary attributes and commands for Works With All Hubs functional extensions.
 * Command: RequireApsAcksOnUnicasts
 * @param numberExemptClusters uint8_t
 * @param clusterId uint8_t*
 * @param clusterIdLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSlWwahClusterRequireApsAcksOnUnicasts(uint8_t numberExemptClusters,
                                                        uint8_t *clusterId,
                                                        uint16_t clusterIdLen,
                                                        frame_info_t *frame_info );
 
/** @brief Roll back changes made by Require APS ACK on Unicasts.
 *
 * Cluster: SL Works With All Hubs, Silicon Labs proprietary attributes and commands for Works With All Hubs functional extensions.
 * Command: RemoveApsAcksOnUnicastsRequirement
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSlWwahClusterRemoveApsAcksOnUnicastsRequirement(
    frame_info_t *frame_info );
 
/** @brief Query whether unicast commands are required to have APS ACKs enabled.
 *
 * Cluster: SL Works With All Hubs, Silicon Labs proprietary attributes and commands for Works With All Hubs functional extensions.
 * Command: ApsAckRequirementQuery
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSlWwahClusterApsAckRequirementQuery(
    frame_info_t *frame_info );
 
/** @brief Query for specified debug report.
 *
 * Cluster: SL Works With All Hubs, Silicon Labs proprietary attributes and commands for Works With All Hubs functional extensions.
 * Command: DebugReportQuery
 * @param debugReportId uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSlWwahClusterDebugReportQuery(uint8_t debugReportId,
                                                frame_info_t *frame_info );
 
/** @brief Causes device to perform a scan for beacons advertising the device's network.
 *
 * Cluster: SL Works With All Hubs, Silicon Labs proprietary attributes and commands for Works With All Hubs functional extensions.
 * Command: SurveyBeacons
 * @param standardBeacons uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSlWwahClusterSurveyBeacons(uint8_t standardBeacons,
                                             frame_info_t *frame_info );
 
/** @brief Disallow OTA downgrade of all device firmware components.
 *
 * Cluster: SL Works With All Hubs, Silicon Labs proprietary attributes and commands for Works With All Hubs functional extensions.
 * Command: DisableOtaDowngrades
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSlWwahClusterDisableOtaDowngrades(
    frame_info_t *frame_info );
 
/** @brief Causes device to ignore MGMT Leave Without Rejoin commands.
 *
 * Cluster: SL Works With All Hubs, Silicon Labs proprietary attributes and commands for Works With All Hubs functional extensions.
 * Command: DisableMgmtLeaveWithoutRejoin
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSlWwahClusterDisableMgmtLeaveWithoutRejoin(
    frame_info_t *frame_info );
 
/** @brief Causes device to ignore Touchlink Interpan messages.
 *
 * Cluster: SL Works With All Hubs, Silicon Labs proprietary attributes and commands for Works With All Hubs functional extensions.
 * Command: DisableTouchlinkInterpanMessageSupport
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSlWwahClusterDisableTouchlinkInterpanMessageSupport(
    frame_info_t *frame_info );
 
/** @brief Enable WWAH Parent Classification advertisements.
 *
 * Cluster: SL Works With All Hubs, Silicon Labs proprietary attributes and commands for Works With All Hubs functional extensions.
 * Command: EnableWwahParentClassification
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSlWwahClusterEnableWwahParentClassification(
    frame_info_t *frame_info );
 
/** @brief Disable WWAH Parent Classification advertisements.
 *
 * Cluster: SL Works With All Hubs, Silicon Labs proprietary attributes and commands for Works With All Hubs functional extensions.
 * Command: DisableWwahParentClassification
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSlWwahClusterDisableWwahParentClassification(
    frame_info_t *frame_info );
 
/** @brief Process only network key rotation commands sent via unicast and encrypted by Trust Center Link Key.
 *
 * Cluster: SL Works With All Hubs, Silicon Labs proprietary attributes and commands for Works With All Hubs functional extensions.
 * Command: EnableTcSecurityOnNtwkKeyRotation
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSlWwahClusterEnableTcSecurityOnNtwkKeyRotation(
    frame_info_t *frame_info );
 
/** @brief Enable WWAH Bad Parent Recovery feature.
 *
 * Cluster: SL Works With All Hubs, Silicon Labs proprietary attributes and commands for Works With All Hubs functional extensions.
 * Command: EnableWwahBadParentRecovery
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSlWwahClusterEnableWwahBadParentRecovery(
    frame_info_t *frame_info );
 
/** @brief Disable WWAH Bad Parent Recovery feature.
 *
 * Cluster: SL Works With All Hubs, Silicon Labs proprietary attributes and commands for Works With All Hubs functional extensions.
 * Command: DisableWwahBadParentRecovery
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSlWwahClusterDisableWwahBadParentRecovery(
    frame_info_t *frame_info );
 
/** @brief Enable Configuration Mode.
 *
 * Cluster: SL Works With All Hubs, Silicon Labs proprietary attributes and commands for Works With All Hubs functional extensions.
 * Command: EnableConfigurationMode
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSlWwahClusterEnableConfigurationMode(
    frame_info_t *frame_info );
 
/** @brief Disable Configuration Mode.
 *
 * Cluster: SL Works With All Hubs, Silicon Labs proprietary attributes and commands for Works With All Hubs functional extensions.
 * Command: DisableConfigurationMode
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSlWwahClusterDisableConfigurationMode(
    frame_info_t *frame_info );
 
/** @brief Use only the Trust Center as cluster server for the set of clusters specified.
 *
 * Cluster: SL Works With All Hubs, Silicon Labs proprietary attributes and commands for Works With All Hubs functional extensions.
 * Command: UseTrustCenterForClusterServer
 * @param numberOfClusters uint8_t
 * @param clusterId uint8_t*
 * @param clusterIdLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSlWwahClusterUseTrustCenterForClusterServer(uint8_t numberOfClusters,
                                                              uint8_t *clusterId,
                                                              uint16_t clusterIdLen,
                                                              frame_info_t *frame_info );
 
/** @brief Causes device to send an appropriate Trust Center for Cluster Server Query Response command.
 *
 * Cluster: SL Works With All Hubs, Silicon Labs proprietary attributes and commands for Works With All Hubs functional extensions.
 * Command: TrustCenterForClusterServerQuery
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSlWwahClusterTrustCenterForClusterServerQuery(
    frame_info_t *frame_info );
 
/** @brief Command description for SlAPSLinkKeyAuthorizationQueryResponse
 *
 * Cluster: SL Works With All Hubs, Silicon Labs proprietary attributes and commands for Works With All Hubs functional extensions.
 * Command: ApsLinkKeyAuthorizationQueryResponse
 * @param clusterId uint16_t
 * @param apsLinkKeyAuthStatus uint8_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSlWwahClusterApsLinkKeyAuthorizationQueryResponse(uint16_t clusterId,
                                                                    uint8_t apsLinkKeyAuthStatus,
                                                                    frame_info_t *frame_info );
 
/** @brief Command description for SlPoweringOffNotification
 *
 * Cluster: SL Works With All Hubs, Silicon Labs proprietary attributes and commands for Works With All Hubs functional extensions.
 * Command: PoweringOffNotification
 * @param powerNotificationReason uint8_t
 * @param manufacturerId uint16_t
 * @param manufacturerReasonLength uint8_t
 * @param manufacturerReason uint8_t*
 * @param manufacturerReasonLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSlWwahClusterPoweringOffNotification(uint8_t powerNotificationReason,
                                                       uint16_t manufacturerId,
                                                       uint8_t manufacturerReasonLength,
                                                       uint8_t *manufacturerReason,
                                                       uint16_t manufacturerReasonLen,
                                                       frame_info_t *frame_info );
 
/** @brief Command description for SlPoweringOnNotification
 *
 * Cluster: SL Works With All Hubs, Silicon Labs proprietary attributes and commands for Works With All Hubs functional extensions.
 * Command: PoweringOnNotification
 * @param powerNotificationReason uint8_t
 * @param manufacturerId uint16_t
 * @param manufacturerReasonLength uint8_t
 * @param manufacturerReason uint8_t*
 * @param manufacturerReasonLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSlWwahClusterPoweringOnNotification(uint8_t powerNotificationReason,
                                                      uint16_t manufacturerId,
                                                      uint8_t manufacturerReasonLength,
                                                      uint8_t *manufacturerReason,
                                                      uint16_t manufacturerReasonLen,
                                                      frame_info_t *frame_info );
 
/** @brief Command description for SlShortAddressChange
 *
 * Cluster: SL Works With All Hubs, Silicon Labs proprietary attributes and commands for Works With All Hubs functional extensions.
 * Command: ShortAddressChange
 * @param deviceEui64 uint8_t*
 * @param deviceShort uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSlWwahClusterShortAddressChange(uint8_t *deviceEui64,
                                                  uint16_t deviceShort,
                                                  frame_info_t *frame_info );
 
/** @brief Command description for SlAPSAckEnablementQueryResponse
 *
 * Cluster: SL Works With All Hubs, Silicon Labs proprietary attributes and commands for Works With All Hubs functional extensions.
 * Command: ApsAckEnablementQueryResponse
 * @param numberExemptClusters uint8_t
 * @param clusterId uint8_t*
 * @param clusterIdLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSlWwahClusterApsAckEnablementQueryResponse(uint8_t numberExemptClusters,
                                                             uint8_t *clusterId,
                                                             uint16_t clusterIdLen,
                                                             frame_info_t *frame_info );
 
/** @brief Command description for SlPowerDescriptorChange
 *
 * Cluster: SL Works With All Hubs, Silicon Labs proprietary attributes and commands for Works With All Hubs functional extensions.
 * Command: PowerDescriptorChange
 * @param currentPowerMode uint32_t
 * @param availablePowerSources uint32_t
 * @param currentPowerSource uint32_t
 * @param currentPowerSourceLevel uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSlWwahClusterPowerDescriptorChange(uint32_t currentPowerMode,
                                                     uint32_t availablePowerSources,
                                                     uint32_t currentPowerSource,
                                                     uint32_t currentPowerSourceLevel,
                                                     frame_info_t *frame_info );
 
/** @brief Command description for SlNewDebugReportNotification
 *
 * Cluster: SL Works With All Hubs, Silicon Labs proprietary attributes and commands for Works With All Hubs functional extensions.
 * Command: NewDebugReportNotification
 * @param debugReportId uint8_t
 * @param debugReportSize uint32_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSlWwahClusterNewDebugReportNotification(uint8_t debugReportId,
                                                          uint32_t debugReportSize,
                                                          frame_info_t *frame_info );
 
/** @brief Command description for SlDebugReportQueryResponse
 *
 * Cluster: SL Works With All Hubs, Silicon Labs proprietary attributes and commands for Works With All Hubs functional extensions.
 * Command: DebugReportQueryResponse
 * @param debugReportId uint8_t
 * @param debugReportData uint8_t*
 * @param debugReportDataLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSlWwahClusterDebugReportQueryResponse(uint8_t debugReportId,
                                                        uint8_t *debugReportData,
                                                        uint16_t debugReportDataLen,
                                                        frame_info_t *frame_info );
 
/** @brief Command description for SlTrustCenterForClusterServerQueryResponse
 *
 * Cluster: SL Works With All Hubs, Silicon Labs proprietary attributes and commands for Works With All Hubs functional extensions.
 * Command: TrustCenterForClusterServerQueryResponse
 * @param numberOfClusters uint8_t
 * @param clusterId uint8_t*
 * @param clusterIdLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSlWwahClusterTrustCenterForClusterServerQueryResponse(uint8_t numberOfClusters,
                                                                        uint8_t *clusterId,
                                                                        uint16_t clusterIdLen,
                                                                        frame_info_t *frame_info );
 
/** @brief Command description for SlSurveyBeaconsResponse
 *
 * Cluster: SL Works With All Hubs, Silicon Labs proprietary attributes and commands for Works With All Hubs functional extensions.
 * Command: SurveyBeaconsResponse
 * @param numberOfBeacons uint8_t
 * @param beacon uint8_t*
 * @param beaconLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSlWwahClusterSurveyBeaconsResponse(uint8_t numberOfBeacons,
                                                     uint8_t *beacon,
                                                     uint16_t beaconLen,
                                                     frame_info_t *frame_info );
 
/** @brief Command description for SlUseTrustCenterForClusterServerResponse
 *
 * Cluster: SL Works With All Hubs, Silicon Labs proprietary attributes and commands for Works With All Hubs functional extensions.
 * Command: UseTrustCenterForClusterServerResponse
 * @param status uint8_t
 * @param clusterStatusLength uint8_t
 * @param clusterStatus uint8_t*
 * @param clusterStatusLen uint16_t
 * @param frame_info: command frame information(endpoint, send type, destnation address etc );
 */
extern bool_t sendCommandSlWwahClusterUseTrustCenterForClusterServerResponse(uint8_t status,
                                                                      uint8_t clusterStatusLength,
                                                                      uint8_t *clusterStatus,
                                                                      uint16_t clusterStatusLen,
                                                                      frame_info_t *frame_info );
 #endif