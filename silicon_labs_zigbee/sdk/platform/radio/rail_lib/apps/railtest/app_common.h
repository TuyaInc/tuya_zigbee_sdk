/***************************************************************************//**
 * @file app_common.h
 * @brief This header file defines variables to be shared between the main
 * test application and customer specific sections.
 * @copyright Copyright 2015 Silicon Laboratories, Inc. www.silabs.com
 ******************************************************************************/

#ifndef __APPS_COMMON_H__
#define __APPS_COMMON_H__

#include "em_gpio.h" // For ButtonArray definition
#include "circular_queue.h"
#ifdef CONFIGURATION_HEADER
#include CONFIGURATION_HEADER
#endif

#include "rail_types.h"

#include "pa_conversions_efr32.h"

/******************************************************************************
 * Constants
 *****************************************************************************/
#define COUNTOF(a) (sizeof(a) / sizeof(a[0]))
#define TX_CONTINUOUS_COUNT (0xFFFFFFFF)

/******************************************************************************
 * Variable Export
 *****************************************************************************/
#define PER_PORT (gpioPortC)
#define PER_PIN  (7)

typedef enum RailTxType{
  TX_TYPE_NORMAL,
  TX_TYPE_CSMA,
  TX_TYPE_LBT
} RailTxType_t;

typedef struct ButtonArray{
  GPIO_Port_TypeDef   port;
  unsigned int        pin;
} ButtonArray_t;

typedef struct RxPacketData {
  /**
   * A structure containing the extra information associated with this received
   * packet.
   */
  RAIL_RxPacketDetails_t appendedInfo;
  /**
   * The packet's status
   */
  RAIL_RxPacketStatus_t packetStatus;
  /**
   * The number of bytes that are in the dataPtr array.
   */
  uint16_t dataLength;
  /**
   * A variable length array holding the receive packet data bytes.
   */
  uint8_t dataPtr[];
} RxPacketData_t;

typedef struct Stats{
  uint32_t samples;
  int32_t min;
  int32_t max;
  float mean;
  float varianceTimesSamples;
} Stats_t;

typedef struct Counters{
  // Counts all successful user transmits
  // "user" in this and following variable names refers to
  // a transmit that a user initiated, i.e. not an ack
  uint32_t userTx;
  // Counts all successful ack transmits
  uint32_t ackTx;
  uint32_t userTxAborted;
  uint32_t ackTxAborted;
  uint32_t userTxBlocked;
  uint32_t ackTxBlocked;
  uint32_t userTxUnderflow;
  uint32_t ackTxUnderflow;

  // Channel busy doesn't differentiate
  // between ack/user packets
  uint32_t txChannelBusy;

  uint32_t receive;
  uint32_t syncDetect;
  uint32_t preambleLost;
  uint32_t preambleDetect;
  uint32_t frameError;
  uint32_t rxOfEvent;
  uint32_t addrFilterEvent;
  uint32_t rxFail;
  uint32_t calibrations;
  uint32_t noRxBuffer;
  uint32_t rfSensedEvent;
  uint32_t perTriggers;
  uint32_t ackTimeout;
  uint32_t lbtSuccess;
  uint32_t lbtRetry;
  uint32_t lbtStartCca;
  uint32_t txFifoAlmostEmpty;
  uint32_t rxFifoAlmostFull;
  uint32_t timingLost;
  uint32_t timingDetect;
  uint32_t radioConfigChanged;
  Stats_t rssi;
} Counters_t;

extern Counters_t counters;
extern int currentConfig;
extern bool receiveModeEnabled;
extern RAIL_RadioState_t rxSuccessTransition;
extern bool transmitting;
extern bool txParameterChanged;
extern uint16_t channel;
extern uint8_t configIndex;
extern uint32_t continuousTransferPeriod;
extern int32_t txCount;
extern uint32_t txAfterRxDelay;
extern int32_t txCancelDelay;
extern RAIL_ChannelConfigEntry_t channels[];
extern const RAIL_ChannelConfig_t channelConfig;
extern bool redrawDisplay;
extern bool skipCalibrations;
extern bool schRxStopOnRxEvent;
extern volatile bool serEvent;
extern uint32_t perCount;
extern uint32_t perDelay;
extern uint32_t rxOverflowDelay;
extern bool afterRxCancelAck;
extern bool afterRxUseTxBufferForAck;
extern bool newTxError;
extern uint32_t failPackets;
extern uint32_t enablePrintEvents;
extern bool printRxErrorPackets;
extern RAIL_VerifyConfig_t configVerify;
extern uint32_t internalTransmitCounter;

#define PERIPHERAL_ENABLE (0x01)
#define ASYNC_RESPONSE (0x02)
extern uint8_t logLevel;
extern uint8_t txData[APP_MAX_PACKET_LENGTH];
extern uint16_t txDataLen;

extern uint8_t ackData[RAIL_AUTOACK_MAX_LENGTH];
extern uint8_t ackDataLen;

extern RailTxType_t txType;
extern RAIL_LbtConfig_t *lbtConfig;
extern RAIL_CsmaConfig_t *csmaConfig;

// Variables used in script_ci.c
extern char script[SCRIPT_LENGTH];
extern uint16_t scriptedMarker;
extern uint16_t scriptLength;
extern uint32_t suspension;
extern uint32_t suspensionStartTime;

// Structure that holds txOptions
extern RAIL_TxOptions_t txOptions;
// If this pointer is NULL, pass in 0 to StartTx
extern RAIL_TxOptions_t *txOptionsPtr;

// Data Management
extern Queue_t  rxPacketQueue;
extern RAIL_DataConfig_t railDataConfig;

// Fifo mode Test bits
extern bool rxFifoManual;
extern bool txFifoManual;

// RAIL instance handle
extern RAIL_Handle_t railHandle;

// Indicator of whether or not to print tx acks as they happens
extern bool printTxAck;

// Indicator of last power level requested for use
extern uint8_t lastSetTxPowerLevel;

// LQI offset variable
extern int16_t lqiOffset;

/**
 * @enum AppMode
 * @brief Enumeration of RailTest transmit states.
 */
typedef enum AppMode{
  NONE = 0,           /**< RAILtest is not doing anything special */
  TX_STREAM = 1,      /**< Send a stream of pseudo-random bits */
  TX_TONE = 2,        /**< Send a tone at the carrier frequency */
  TX_CONTINUOUS = 3,  /**< Send an unending stream of packets*/
  DIRECT = 4,         /**< Send data to and from a GPIO, without any packet handling */
  TX_N_PACKETS = 5,   /**< Send a specific number of packets */
  TX_SCHEDULED = 6,   /**< Send one packet scheduled in the future */
  SCHTX_AFTER_RX = 7, /**< Schedule a TX for a fixed delay after receiving a packet */
  RX_OVERFLOW = 8,    /**< Cause overflow on receive */
  TX_UNDERFLOW = 9,   /**< Cause underflows on the next TX sequence */
  TX_CANCEL = 10,     /**< Cancel a single packet transmit to force an error event */
  RF_SENSE = 11,      /**< Sense RF energy to wake the radio */
  PER = 12,           /**< Packet Error Rate test mode */
  BER = 13,           /**< Bit Error Rate test mode */
  RX_SCHEDULED = 14,  /**< Enable receive at a time scheduled in the future */
} AppMode_t;

typedef struct EventData{
  RAIL_Events_t events;
  uint32_t timestamp;
  RAIL_Handle_t handle;
} EventData_t;

/**
 * @enum RailTxType
 * @brief Enumeration of the types of tx available in RAIL
 *
 * These are used to decide which type of tx to do, based on
 * what's been configured in railtest. Scheduled is not included
 * as railtest handles it somewhat separately.
 */

void RAILCb_TimerExpired(RAIL_Handle_t railHandle);
AppMode_t previousAppMode(void);
AppMode_t currentAppMode(void);
void enableAppMode(AppMode_t appMode, bool enable, char *command);
bool enableAppModeSync(AppMode_t appMode, bool enable, char *command);
void setNextAppMode(AppMode_t appMode, char *command);
void changeAppModeIfPending();
const char *appModeNames(AppMode_t appMode);
bool inAppMode(AppMode_t appMode, char *command);
bool inRadioState(RAIL_RadioState_t state, char *command);
bool parseTimeModeFromString(char *str, RAIL_TimeMode_t *mode);
const char *configuredRxAntenna(RAIL_RxOptions_t rxOptions);

void updateStats(int32_t newValue, Stats_t *stats);
void rfSensedCheck(void);
void updateDisplay(void);

void changeChannel(uint32_t i);
void pendPacketTx(void);
void pendFinishTxSequence(void);
void pendFinishTxAckSequence(void);
void radioTransmit(uint32_t iterations, char *command);
void configureTxAfterRx(uint32_t delay, bool enable, char *command);
void scheduleNextTx(void);
void printPacket(char *cmdName,
                 uint8_t *data,
                 uint16_t dataLength,
                 RxPacketData_t *packetInfo);

void appHalInit(void);
void deinitButtons(void);
void initButtons(void);
void initGraphics(void);
void LedSet(int led);
void LedToggle(int led);
void PeripheralDisable(void);
void LedsDisable(void);
void disableGraphics(void);
void PeripheralEnable(void);
void enableGraphics(void);
void usDelay(uint32_t microseconds);
void serialWaitForTxIdle(void);
void enqueueEvents(RAIL_Events_t events);
void rxFifoPrep(void);
RAIL_Status_t chooseTxType(bool reuseCcaConfig);
const char *getRfStateName(RAIL_RadioState_t state);
const char *getStatusMessage(RAIL_Status_t status);

void RAILCb_TxPacketSent(RAIL_Handle_t railHandle, bool isAck);
void RAILCb_RxPacketAborted(RAIL_Handle_t railHandle);
void RAILCb_RxPacketReceived(RAIL_Handle_t railHandle);
void RAILCb_TxFifoAlmostEmpty(RAIL_Handle_t railHandle);
void RAILCb_RxFifoAlmostFull(RAIL_Handle_t railHandle);
void RAILCb_AssertFailed(RAIL_Handle_t railHandle, uint32_t errorCode);
void RAILCb_RadioConfigChanged(RAIL_Handle_t railHandle,
                               const RAIL_ChannelConfigEntry_t * entry);

#endif // __APPS_COMMON_H__
