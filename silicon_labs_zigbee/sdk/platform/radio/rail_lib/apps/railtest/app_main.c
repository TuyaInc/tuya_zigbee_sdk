/***************************************************************************//**
 * @file app_main.c
 * @brief This is the base test application. It handles basic RAIL configuration
 * as well as transmit, receive, and various debug modes.
 * @copyright Copyright 2015 Silicon Laboratories, Inc. www.silabs.com
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rail.h"
#include "rail_config.h"
#include "rail_features.h"
#include "rail_ieee802154.h"
#include "rail_zwave.h"
#include "rail_types.h"
#include "rail_chip_specific.h"
#include "rail_assert_error_codes.h"

#include "em_chip.h"
#include "em_core.h"
#include "em_rmu.h"
#include "em_emu.h"

#include "retargetserial.h"
#include "gpiointerrupt.h"
#ifdef EMBER_AF_PLUGIN_LCD_GRAPHICS
#include "graphics.h"
#endif
#include "command_interpreter.h"
#include "response_print.h"
#include "buffer_pool_allocator.h"
#include "circular_queue.h"

#include "rail_config.h"

#ifdef CONFIGURATION_HEADER
#include CONFIGURATION_HEADER
#endif

#ifdef EMBER_AF_PLUGIN_LQI_OVERRIDE
#include "lqi_override.h"
#endif

#include "app_ci.h"
#include "app_common.h"
#include "app_trx.h"
#include "railapp.h"

#include "bsp.h"
#ifdef RAILAPP_RMR
#include "railapp_rmr.h"
#endif

// Includes for Silicon Labs-only, internal testing
#ifdef RPC_TESTING
#include "vs_rpc.h"
#else
#define RPC_Server_Init()
#define RPC_Server_Tick()
#endif

// Configuration defines
#ifndef EVENT_QUEUE_SIZE
#define EVENT_QUEUE_SIZE 10
#endif

// External control and status variables
Counters_t counters = { 0 };
bool receiveModeEnabled = false;
RAIL_RadioState_t rxSuccessTransition = RAIL_RF_STATE_IDLE;
uint8_t logLevel = PERIPHERAL_ENABLE | ASYNC_RESPONSE;
int32_t txCount = 0;
uint32_t continuousTransferPeriod = APP_CONTINUOUS_TRANSFER_PERIOD;
uint32_t txAfterRxDelay = 0;
int32_t txCancelDelay = -1;
bool skipCalibrations = false;
bool afterRxCancelAck = false;
bool afterRxUseTxBufferForAck = false;
bool schRxStopOnRxEvent = false;
uint32_t rssiDoneCount = 0; // HW rssi averaging
float averageRssi = -128;
bool printTxAck = false;
RAIL_VerifyConfig_t configVerify = { 0 };
int16_t lqiOffset = 0;

// Internal app state variables
static bool receivingPacket = false;
RAIL_Events_t lastTxStatus = 0;
RAIL_Events_t lastTxAckStatus = 0;
static uint32_t startTransmitCounter = 0;
uint32_t internalTransmitCounter = 0;
uint32_t failPackets = 0;
uint32_t failAckPackets = 0;
uint32_t sentAckPackets = 0;
static bool packetTx = false;
static bool finishTxSequence = false;
static bool finishTxAckSequence = false;
RAIL_ScheduleTxConfig_t nextPacketTxTime = {
  0,
  RAIL_TIME_ABSOLUTE,
  RAIL_SCHEDULED_TX_DURING_RX_POSTPONE_TX
};
Queue_t  rxPacketQueue;
static uint32_t railTimerExpireTime = 0;
static uint32_t railTimerConfigExpireTime = 0;
static bool     railTimerExpired = false;
static bool     calibrateRadio = false;
bool newTxError = false;
static bool     rxAckTimeout = false;
static uint32_t ackTimeoutDuration = 0;
EventData_t eventQueue[EVENT_QUEUE_SIZE];
uint8_t eventQueueMarker = 0;
uint32_t enablePrintEvents = 0;
bool printRxErrorPackets = false;
uint32_t eventsMissed = 0;
#ifndef HAL_PA_POWER
#define HAL_PA_POWER 100
#endif
uint8_t lastSetTxPowerLevel = HAL_PA_POWER;
static RAIL_TxPowerMode_t default2p4Pa =
#ifdef _SILICON_LABS_32B_SERIES_1
#if HAL_PA_2P4_LOWPOWER
  RAIL_TX_POWER_MODE_2P4_LP;
#else
  RAIL_TX_POWER_MODE_2P4_HP;
#endif
#else
  RAIL_TX_POWER_MODE_2P4_HP;
#endif

// Names of RAIL_EVENT defines. This should align with rail_types.h
const char* eventNames[] = {
  "RSSI_AVERAGE_DONE",
  "RX_ACK_TIMEOUT",
  "RX_FIFO_ALMOST_FULL",
  "RX_PACKET_RECEIVED",
  "RX_PREAMBLE_LOST",
  "RX_PREAMBLE_DETECT",
  "RX_SYNC1_DETECT",
  "RX_SYNC2_DETECT",
  "RX_FRAME_ERROR",
  "RX_FIFO_OVERFLOW",
  "RX_ADDRESS_FILTERED",
  "RX_TIMEOUT",
  "RX_SCHEDULED_RX_END",
  "RX_PACKET_ABORTED",
  "RX_FILTER_PASSED",
  "RX_TIMING_LOST",
  "RX_TIMING_DETECT",
  "IEEE802154_DATA_REQUEST_COMMAND",
  "TX_FIFO_ALMOST_EMPTY",
  "TX_PACKET_SENT",
  "TXACK_PACKET_SENT",
  "TX_ABORTED",
  "TXACK_ABORTED",
  "TX_BLOCKED",
  "TXACK_BLOCKED",
  "TX_UNDERFLOW",
  "TXACK_UNDERFLOW",
  "TX_CHANNEL_CLEAR",
  "TX_CHANNEL_BUSY",
  "TX_CCA_RETRY",
  "TX_START_CCA",
  "CONFIG_UNSCHEDULED",
  "CONFIG_SCHEDULED",
  "SCHEDULED_STATUS"
  "CAL_NEEDED",
};

// Allow local echo to be turned on/off for the command prompt
#ifdef DISABLE_LOCAL_ECHO
  #define localEcho 0
#else
  #define localEcho 1
#endif

#ifdef INTERNAL_COMMAND_HEADER
  #include INTERNAL_COMMAND_HEADER
#endif

//Command line variables
#define RAILCMD_ENTRY(command, args, callback, helpStr) \
  COMMAND_ENTRY(command, args, callback, helpStr),
#define RAILCMD_SEPARATOR(string) \
  COMMAND_SEPARATOR(string),
static CommandEntry_t commands[] = {
  APP_CI_COMMANDS
  RAILAPP_CI_COMMANDS
#ifdef RAILAPP_RMR
  RMR_CI_COMMANDS
#endif
#ifdef INTERNAL_COMMANDS
  INTERNAL_COMMANDS
#endif
  COMMAND_ENTRY(NULL, NULL, NULL, NULL)
};
#undef  RAILCMD_ENTRY
#undef  RAILCMD_SEPARATOR
static CommandState_t ciState;
static char ciBuffer[APP_COMMAND_INTERFACE_BUFFER_SIZE];

// Channel Variable
uint16_t channel = 0;

// Channel Config Selection Variable
uint8_t configIndex = 0;

RAIL_TxPowerConfig_t txPowerConfig = {
  .mode = RAIL_TX_POWER_MODE_NONE,
#ifndef BSP_PA_VOLTAGE // Work with older Gecko SDK
#define BSP_PA_VOLTAGE HAL_PA_VOLTAGE
#endif
  .voltage = BSP_PA_VOLTAGE,
  .rampTime = HAL_PA_RAMP,
};

// Generic
RAIL_Handle_t railHandle;

uint8_t txData[APP_MAX_PACKET_LENGTH] = {
  0x0F, 0x0E, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66,
  0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE,
};
uint16_t txDataLen = 16;

static uint8_t txFifo[TX_BUFFER_SIZE];

uint8_t ackData[RAIL_AUTOACK_MAX_LENGTH] = {
  0x0F, 0x0E, 0xF1, 0xE2, 0xD3, 0xC4, 0xB5, 0xA6,
  0x97, 0x88, 0x79, 0x6A, 0x5B, 0x4C, 0x3D, 0x2E,
};
uint8_t ackDataLen = 16;

// Static RAIL callbacks
static void RAILCb_RfReady(RAIL_Handle_t railHandle);
static void RAILCb_IEEE802154_DataRequestCommand(RAIL_Handle_t railHandle);
static void RAILCb_RssiAverageDone(RAIL_Handle_t railHandle);
static void RAILCb_Event(RAIL_Handle_t railHandle, RAIL_Events_t events);
uint8_t RAILCb_ConvertLqi(uint8_t lqi, int8_t rssi);

static RAIL_Config_t railCfg = {
  .eventsCallback = &RAILCb_Event
};

// Structure that holds txOptions
RAIL_TxOptions_t txOptions;

// If this pointer is NULL, pass in 0 to StartTx
RAIL_TxOptions_t *txOptionsPtr = NULL;

// Data Management
RAIL_DataConfig_t railDataConfig = {
  .txSource = TX_PACKET_DATA,
  .rxSource = RX_PACKET_DATA,
  .txMethod = PACKET_MODE,
  .rxMethod = PACKET_MODE,
};

// Called during main loop
void processInputCharacters(void);
void sendPacketIfPending(void);
void finishTxSequenceIfPending(void);
void changeAppModeIfPending(void);
void printReceivedPacket(void);
void printNewTxError(void);
void checkTimerExpiration(void);
void updateDisplay(void);
void processPendingCalibrations(void);
void printAckTimeout(void);
void printEvents(void);

int main(void)
{
  // Initialize the chip
  CHIP_Init();

  // Initialize the RPC Server
  RPC_Server_Init();

  // Grab the reset cause
  uint32_t resetCause = RMU_ResetCauseGet();
  RMU_ResetCauseClear(); // So resetCause is rational and not an accumulated mess
  // Release GPIOs that were held by EM4h to ensure proper startup
  EMU_UnlatchPinRetention();

  // Initialize hardware for application
  appHalInit();

  // Print app initialization information before starting up the radio
  printf("\n"APP_DEMO_STRING_INIT "\n");

  // Initialize Radio
  railHandle = RAIL_Init(&railCfg, &RAILCb_RfReady);

  // Set TX FIFO, and verify that the size is correct
  uint16_t fifoSize = RAIL_SetTxFifo(railHandle, txFifo, 0, TX_BUFFER_SIZE);
  if (fifoSize != TX_BUFFER_SIZE) {
    while (1) ;
  }

  // Configure Radio Calibrations
  RAIL_ConfigCal(railHandle, RAIL_CAL_ALL);

  // Load the channel configuration for the generated radio settings
  channel = RAIL_ConfigChannels(railHandle,
                                channelConfigs[configIndex],
                                &RAILCb_RadioConfigChanged);

  // Register an LQI conversion callback.
#ifdef EMBER_AF_PLUGIN_LQI_OVERRIDE
  LQI_SetOverrideEnabled(railHandle, true);
#else
  RAIL_ConvertLqi(railHandle, &RAILCb_ConvertLqi);
#endif

  // Configure all RAIL events with appended info
  RAIL_Events_t events = RAIL_EVENT_CAL_NEEDED
                         | RAIL_EVENT_RSSI_AVERAGE_DONE
                         | RAIL_EVENT_RX_ACK_TIMEOUT
                         | RAIL_EVENT_RX_FIFO_ALMOST_FULL
                         | RAIL_EVENT_TX_FIFO_ALMOST_EMPTY
                         | RAIL_EVENT_RX_PACKET_RECEIVED
                         | RAIL_EVENT_TX_PACKET_SENT
                         | RAIL_EVENT_TXACK_PACKET_SENT
                         | RAIL_EVENT_IEEE802154_DATA_REQUEST_COMMAND
                         // AKA RAIL_EVENT_ZWAVE_BEAM
                         | RAIL_EVENT_RX_PREAMBLE_DETECT
                         | RAIL_EVENT_RX_SYNC1_DETECT
                         | RAIL_EVENT_RX_SYNC2_DETECT
                         | RAIL_EVENT_RX_FRAME_ERROR
                         | RAIL_EVENT_RX_FIFO_OVERFLOW
                         | RAIL_EVENT_RX_ADDRESS_FILTERED
                         | RAIL_EVENT_RX_TIMEOUT
                         | RAIL_EVENT_RX_SCHEDULED_RX_END
                         | RAIL_EVENT_RX_PACKET_ABORTED
                         | RAIL_EVENT_RX_FILTER_PASSED
                         | RAIL_EVENT_TX_CHANNEL_BUSY
                         | RAIL_EVENT_TX_ABORTED
                         | RAIL_EVENT_TX_BLOCKED
                         | RAIL_EVENT_TX_UNDERFLOW
                         | RAIL_EVENT_TXACK_ABORTED
                         | RAIL_EVENT_TXACK_BLOCKED
                         | RAIL_EVENT_TXACK_UNDERFLOW
                         | RAIL_EVENT_TX_CHANNEL_CLEAR
                         | RAIL_EVENT_TX_CCA_RETRY
                         | RAIL_EVENT_TX_START_CCA;
  RAIL_ConfigEvents(railHandle, RAIL_EVENTS_ALL, events);
  RAIL_ConfigRxOptions(railHandle, RAIL_RX_OPTIONS_ALL, RAIL_RX_OPTIONS_DEFAULT);

  RAIL_StateTransitions_t transitions = {
    .success = RAIL_RF_STATE_RX,
    .error = RAIL_RF_STATE_RX
  };
  RAIL_SetRxTransitions(railHandle, &transitions);
  rxSuccessTransition = RAIL_RF_STATE_RX;
  RAIL_SetTxTransitions(railHandle, &transitions);
  // Initialize the queue we use for tracking packets
  if (!queueInit(&rxPacketQueue, MAX_QUEUE_LENGTH)) {
    while (1) ;
  }

#ifdef _SILICON_LABS_32B_SERIES_1
  if (resetCause & RMU_RSTCAUSE_EM4RST) {
    responsePrint("sleepWoke", "EM:4%c,SerialWakeup:No,RfSensed:%s",
                  (((EMU->EM4CTRL & EMU_EM4CTRL_EM4STATE)
                    == EMU_EM4CTRL_EM4STATE_EM4S) ? 's' : 'h'),
                  RAIL_IsRfSensed(railHandle) ? "Yes" : "No");
    // Always turn off RfSense when waking back up from EM4
    (void) RAIL_StartRfSense(railHandle, RAIL_RFSENSE_OFF, 0, NULL);
  }
#else
  (void) resetCause;
#endif

  updateDisplay();

  printf("> ");
  ciInitState(&ciState, ciBuffer, sizeof(ciBuffer), commands);

  // Fill out the txPacket with a useful pattern so it's not
  // all-zeros if user extends the Tx length.
  for (unsigned int i = txDataLen; i < sizeof(txData); i++) {
    txData[i] = i;
  }

  // Initialize autoack data
  RAIL_WriteAutoAckFifo(railHandle, ackData, ackDataLen);

  RAIL_StartRx(railHandle, channel, NULL); // Start in receive mode
  receiveModeEnabled = true;

  while (1) {
    RPC_Server_Tick();

    processInputCharacters();

    // Change app mode first so that any new actions can take effect this loop
    changeAppModeIfPending();

    rfSensedCheck();

    sendPacketIfPending();

    finishTxSequenceIfPending();

    printReceivedPacket();

    printNewTxError();

    printEvents();

    checkTimerExpiration();

    updateDisplay();

    processPendingCalibrations();

    printAckTimeout();
  }
} //main()

/******************************************************************************
 * RAIL Callback Implementation
 *****************************************************************************/
static void RAILCb_RfReady(RAIL_Handle_t railHandle)
{
  LedSet(0);
  LedSet(1);
}

void RAILCb_TimerExpired(RAIL_Handle_t railHandle)
{
  if (inAppMode(NONE, NULL)) {
    if (abortRxDelay != 0) {
      RAIL_Idle(railHandle, RAIL_IDLE_ABORT, true);
    } else {
      railTimerExpireTime = RAIL_GetTime();
      railTimerConfigExpireTime = RAIL_GetTimer(railHandle);
      railTimerExpired = true;
    }
  } else if (currentAppMode() == PER) {
    GPIO_PinOutToggle(PER_PORT, PER_PIN);
    counters.perTriggers += GPIO_PinOutGet(PER_PORT, PER_PIN);
    perCount -= GPIO_PinOutGet(PER_PORT, PER_PIN);
    if (perCount < 1) {
      GPIO_PinOutClear(PER_PORT, PER_PIN);
      enableAppMode(PER, false, NULL);
    } else {
      RAIL_SetTimer(railHandle, perDelay, RAIL_TIME_DELAY, &RAILCb_TimerExpired);
    }
  } else {
    pendPacketTx();
  }
}

uint8_t RAILCb_ConvertLqi(uint8_t lqi, int8_t rssi)
{
  (void)rssi;
  // Put any custom LQI conversion code here.
  // In this application, lqiOffset is between -255 and 255 but LQI is uint8_t:
  int16_t newLqi = lqiOffset + lqi;
  if (newLqi < 0) {
    newLqi = 0;     // uint8_t min
  } else if (newLqi > 0xFF) {
    newLqi = 0xFF;  // uint8_t max
  }
  return (uint8_t)newLqi;
}

static void RAILCb_IEEE802154_DataRequestCommand(RAIL_Handle_t railHandle)
{
  RAIL_IEEE802154_Address_t address;
  if (RAIL_IEEE802154_GetAddress(railHandle, &address)
      != RAIL_STATUS_NO_ERROR) {
    return;
  }
  // Placeholder validation for when a data request should have the frame
  // pending bit set in the ACK.
  if (address.length == RAIL_IEEE802154_LongAddress) {
    if (address.longAddress[0] == 0xAA) {
      RAIL_IEEE802154_SetFramePending(railHandle);
    }
  } else {
    if ((address.shortAddress & 0xFF) == 0xAA) {
      RAIL_IEEE802154_SetFramePending(railHandle);
    }
  }
}

static void RAILCb_ZWAVE_BeamFrame(RAIL_Handle_t railHandle)
{
  RAIL_ZWAVE_NodeId_t beamNodeId;
  if (RAIL_ZWAVE_GetBeamNodeId(railHandle, &beamNodeId)
      != RAIL_STATUS_NO_ERROR) {
    return;
  }
  responsePrint("ZWaveBeamFrame", "nodeId:0x%x", beamNodeId);
}

static void RAILCb_RssiAverageDone(RAIL_Handle_t railHandle)
{
  char bufAverageRssi[10];
  int16_t avgRssi = RAIL_GetAverageRssi(railHandle);
  rssiDoneCount++;
  averageRssi = (float)avgRssi / 4;
  if (avgRssi == RAIL_RSSI_INVALID) {
    responsePrint("getAvgRssi", "Could not read RSSI.");
    return;
  }

  sprintfFloat(bufAverageRssi, sizeof(bufAverageRssi), averageRssi, 2);
  responsePrint("getAvgRssi", "rssi:%s", bufAverageRssi);
}

void RAILCb_AssertFailed(RAIL_Handle_t railHandle, uint32_t errorCode)
{
  static const char* railErrorMessages[] = RAIL_ASSERT_ERROR_MESSAGES;
  const char *errorMessage = "Unknown";

  // If this error code is within the range of known error messages then use
  // the appropriate error message.
  if (errorCode < (sizeof(railErrorMessages) / sizeof(char*))) {
    errorMessage = railErrorMessages[errorCode];
  }
  // Print a message about the assert that triggered
  responsePrint("assert",
                "code:%d,message:%s",
                errorCode,
                errorMessage);
  // Reset the chip since an assert is a fatal error
  NVIC_SystemReset();
}

void RAILCb_RadioConfigChanged(RAIL_Handle_t railHandle,
                               const RAIL_ChannelConfigEntry_t *entry)
{
  bool subGhz = (entry->baseFrequency < 1000000000UL);

  // If the user is set to use sub-gig on a 2.4-only part, make sure we throw an error
#if RAIL_FEAT_SUBGIG_RADIO
  RAIL_TxPowerMode_t newTxMode = (subGhz) ? RAIL_TX_POWER_MODE_SUBGIG : default2p4Pa;
#else
  RAIL_TxPowerMode_t newTxMode = (subGhz) ? RAIL_TX_POWER_MODE_NONE : default2p4Pa;
#endif

  // Ensure that the correct PA is configured for use. If it is correct,
  // we don't need to do anything as RAIL library takes care of setting
  // the power level according to channel limits. If the PA needs to change
  // however, the app needs to make that change explicitly and re-set the
  // power.
  if (newTxMode != txPowerConfig.mode) {
    // Get the power that the user wanted, regardless of channel power limit
    // We don't care what was actually set, because that could have been some
    // value limited by the previous channel
    RAIL_TxPower_t lastSetTxPower =
      RAIL_ConvertRawToDbm(railHandle, txPowerConfig.mode, lastSetTxPowerLevel);

    txPowerConfig.mode = newTxMode;
    RAIL_Status_t retStatus = RAIL_ConfigTxPower(railHandle, &txPowerConfig);
    if (retStatus != RAIL_STATUS_NO_ERROR) {
      // Report the PA configuration error
      responsePrint("configRadio",
                    "status:0x%0.2x,message:Invalid PA selected,baseFrequencyHz:%u",
                    retStatus,
                    entry->baseFrequency);
    } else {
      // If you change the PA's, you must explicitly set the power to the
      // desired value
      if (lastSetTxPower > entry->maxPower) {
        RAIL_SetTxPowerDbm(railHandle, entry->maxPower);
      } else {
        RAIL_SetTxPowerDbm(railHandle, lastSetTxPower);
      }
    }
  }

  // Now that the radio config changed, verify the new data contents.
  RAIL_ConfigVerification(railHandle, &configVerify, NULL, NULL);
  if (RAIL_Verify(&configVerify, RAIL_VERIFY_DURATION_MAX, true)
      == RAIL_STATUS_INVALID_STATE) {
    responsePrint("verifyRadio", "Radio data corruption detected!");
    while (1) ;
  }

  counters.radioConfigChanged++;
}

static void RAILCb_Event(RAIL_Handle_t railHandle, RAIL_Events_t events)
{
  enqueueEvents(events);
  if (events & RAIL_EVENT_CAL_NEEDED) {
    calibrateRadio = true;
  }
  if (events & RAIL_EVENT_RSSI_AVERAGE_DONE) {
    RAILCb_RssiAverageDone(railHandle);
  }

  // RX Events
  if (events & RAIL_EVENT_RX_TIMING_DETECT) {
    counters.timingDetect++;
  }
  if (events & RAIL_EVENT_RX_TIMING_LOST) {
    counters.timingLost++;
  }
  if (events & RAIL_EVENT_RX_PREAMBLE_LOST) {
    counters.preambleLost++;
  }
  if (events & RAIL_EVENT_RX_PREAMBLE_DETECT) {
    counters.preambleDetect++;
  }
  if (events & RAIL_EVENT_RX_SYNC1_DETECT) {
    receivingPacket = true;
    counters.syncDetect++;
    rxFifoPrep();
    if (abortRxDelay != 0) {
      RAIL_SetTimer(railHandle, abortRxDelay, RAIL_TIME_DELAY, &RAILCb_TimerExpired);
    }
  }
  if (events & RAIL_EVENT_IEEE802154_DATA_REQUEST_COMMAND) {
    if (RAIL_ZWAVE_IsEnabled(railHandle)) {
      // Really was RAIL_EVENT_ZWAVE_BEAM
      RAILCb_ZWAVE_BeamFrame(railHandle);
    } else {
      RAILCb_IEEE802154_DataRequestCommand(railHandle);
    }
  }
  if (events & RAIL_EVENT_RX_FIFO_ALMOST_FULL) {
    RAILCb_RxFifoAlmostFull(railHandle);
  }
  if (events & (RAIL_EVENT_RX_FIFO_OVERFLOW
                | RAIL_EVENT_RX_ADDRESS_FILTERED
                | RAIL_EVENT_RX_PACKET_ABORTED
                | RAIL_EVENT_RX_FRAME_ERROR
                | RAIL_EVENT_RX_PACKET_RECEIVED)) {
    // All of the above events cause a packet to not be received
    receivingPacket = false;
    if (rxFifoManual && (railDataConfig.rxMethod != PACKET_MODE)) {
      (void) RAIL_HoldRxPacket(railHandle);
    }
    if (events & RAIL_EVENT_RX_FIFO_OVERFLOW) {
      counters.rxOfEvent++;
      RAILCb_RxPacketAborted(railHandle);
    }
    if (events & RAIL_EVENT_RX_ADDRESS_FILTERED) {
      counters.addrFilterEvent++;
      RAILCb_RxPacketAborted(railHandle);
    }
    if (events & RAIL_EVENT_RX_PACKET_ABORTED) {
      counters.rxFail++;
      RAILCb_RxPacketAborted(railHandle);
    }
    if (events & RAIL_EVENT_RX_FRAME_ERROR) {
      counters.frameError++;
      RAILCb_RxPacketAborted(railHandle);
    }
    if (events & RAIL_EVENT_RX_PACKET_RECEIVED) {
      RAILCb_RxPacketReceived(railHandle);
    }
  }
  if (events & RAIL_EVENT_RX_ACK_TIMEOUT) {
    counters.ackTimeout++;
    rxAckTimeout = true;
    ackTimeoutDuration = RAIL_GetTime()
                         - previousTxAppendedInfo.timeSent.packetTime;
  }
  // End scheduled receive mode if an appropriate end or error event is received
  if ((events & RAIL_EVENT_RX_SCHEDULED_RX_END)
      || ((schRxStopOnRxEvent && inAppMode(RX_SCHEDULED, NULL))
          && (events & RAIL_EVENT_RX_ADDRESS_FILTERED
              || events & RAIL_EVENT_RX_FIFO_OVERFLOW
              || events & RAIL_EVENT_RX_FRAME_ERROR))) {
    enableAppMode(RX_SCHEDULED, false, NULL);
  }

  // TX Events
  if (events & RAIL_EVENT_TX_START_CCA) {
    counters.lbtStartCca++;
  }
  if (events & RAIL_EVENT_TX_CCA_RETRY) {
    counters.lbtRetry++;
  }
  if (events & RAIL_EVENT_TX_CHANNEL_CLEAR) {
    counters.lbtSuccess++;
  }
  if (events & (RAIL_EVENT_TX_ABORTED
                | RAIL_EVENT_TX_BLOCKED
                | RAIL_EVENT_TX_UNDERFLOW
                | RAIL_EVENT_TX_CHANNEL_BUSY)) {
    lastTxStatus = events;
    newTxError = true;
    failPackets++;
    scheduleNextTx();

    // Increment counters for TX events
    if (events & RAIL_EVENT_TX_ABORTED) {
      counters.userTxAborted++;
    }
    if (events & RAIL_EVENT_TX_BLOCKED) {
      counters.userTxBlocked++;
    }
    if (events & RAIL_EVENT_TX_UNDERFLOW) {
      counters.userTxUnderflow++;
    }
  }
  // Put this here too so that we do these things twice
  // in the case that an ack and a non ack failure have
  // been queued up
  if (events & (RAIL_EVENT_TXACK_ABORTED
                | RAIL_EVENT_TXACK_BLOCKED
                | RAIL_EVENT_TXACK_UNDERFLOW)) {
    lastTxAckStatus = events;
    failAckPackets++;
    pendFinishTxAckSequence();

    // Increment counters for TXACK events
    if (events & RAIL_EVENT_TXACK_ABORTED) {
      counters.ackTxAborted++;
    }
    if (events & RAIL_EVENT_TXACK_BLOCKED) {
      counters.ackTxBlocked++;
    }
    if (events & RAIL_EVENT_TXACK_UNDERFLOW) {
      counters.ackTxUnderflow++;
    }
  }
  if (events & RAIL_EVENT_TX_FIFO_ALMOST_EMPTY) {
    RAILCb_TxFifoAlmostEmpty(railHandle);
  }
  if (events & RAIL_EVENT_TX_PACKET_SENT) {
    counters.userTx++;
    RAILCb_TxPacketSent(railHandle, false);
  }
  if (events & RAIL_EVENT_TXACK_PACKET_SENT) {
    counters.ackTx++;
    RAILCb_TxPacketSent(railHandle, true);
  }
}
/******************************************************************************
 * Application Helper Functions
 *****************************************************************************/
void processPendingCalibrations()
{
  // Only calibrate the radio when not currently transmitting or in a
  // transmit mode. Also don't try to calibrate while receiving a packet
  bool calsInMode = inAppMode(NONE, NULL);
  if (calibrateRadio && calsInMode && !skipCalibrations && !receivingPacket) {
    RAIL_CalMask_t pendingCals = RAIL_GetPendingCal(railHandle);

    counters.calibrations++;
    calibrateRadio = false;

    // Perform the necessary calibrations and don't save the results
    if (pendingCals & RAIL_CAL_TEMP_VCO) {
      RAIL_CalibrateTemp(railHandle);
    }

    // Disable the radio if we have to do IRCAL
    if (pendingCals & RAIL_CAL_ONETIME_IRCAL) {
      RAIL_Idle(railHandle, RAIL_IDLE_ABORT, false);
      RAIL_CalibrateIr(railHandle, NULL);
      if (receiveModeEnabled) {
        RAIL_StartRx(railHandle, channel, NULL);
      }
    }
  }
}

void checkTimerExpiration()
{
  if (railTimerExpired) {
    railTimerExpired = false;
    responsePrint("timerCb",
                  "TimerExpiredCallback:%u,ConfiguredExpireTime:%u",
                  railTimerExpireTime,
                  railTimerConfigExpireTime);
  }
}

void printNewTxError()
{
  if (newTxError) {
    newTxError = false;
    if (lastTxStatus & RAIL_EVENT_TX_UNDERFLOW) {
      if (logLevel & ASYNC_RESPONSE) {
        responsePrint("txPacket",
                      "txStatus:Error,"
                      "errorReason:Tx underflow or abort,"
                      "errorCode:0x%x",
                      lastTxStatus);
      }
    }
    if (lastTxStatus & RAIL_EVENT_TX_CHANNEL_BUSY) {
      if (logLevel & ASYNC_RESPONSE) {
        responsePrint("txPacket",
                      "txStatus:Error,"
                      "errorReason:Tx channel busy,"
                      "errorCode:0x%x",
                      lastTxStatus);
      }
      counters.txChannelBusy++;
    }
  }
}

void printAckTimeout()
{
  if (rxAckTimeout) {
    rxAckTimeout = false;
    responsePrint("rxAckTimeout",
                  "ackTimeoutDuration:%d",
                  ackTimeoutDuration);
  }
}

void changeChannel(uint32_t i)
{
  channel = i;
  redrawDisplay = true;
  // Automatically apply the new channel to future Tx/Rx
  if (receiveModeEnabled) {
    RAIL_StartRx(railHandle, channel, NULL);
  }
}

void pendPacketTx()
{
  packetTx = true;
}

RAIL_Status_t chooseTxType(bool reuseCcaConfig)
{
  RAIL_TxOptions_t currentOptions = txOptionsPtr == NULL ? 0 : *txOptionsPtr;
  if (currentAppMode() == TX_SCHEDULED || currentAppMode() == SCHTX_AFTER_RX) {
    return RAIL_StartScheduledTx(railHandle, channel, currentOptions, &nextPacketTxTime, NULL);
  } else if (txType == TX_TYPE_LBT) {
    // Sending NULL for txPreTxOpArgs reuses previous arguments, which
    // saves computation.
    return RAIL_StartCcaLbtTx(railHandle, channel, currentOptions, reuseCcaConfig ? NULL : lbtConfig, NULL);
  } else if (txType == TX_TYPE_CSMA) {
    return RAIL_StartCcaCsmaTx(railHandle, channel, currentOptions, reuseCcaConfig ? NULL : csmaConfig, NULL);
  } else {
    return RAIL_StartTx(railHandle, channel, currentOptions, NULL);
  }
}

void sendPacketIfPending()
{
  if (packetTx) {
    packetTx = false;
    uint8_t txStatus;
    uint32_t storedTransmitCounter = internalTransmitCounter;

    // Don't decrement in continuous mode
    if (currentAppMode() != TX_CONTINUOUS) {
      txCount--;
    }

    // Generate the payload and start transmitting
    if (currentAppMode() != TX_UNDERFLOW) { // Force underflows in this mode
      // Load packet data before transmitting if manual loading is not enabled
      if (!txFifoManual) {
        loadTxData(txData, txDataLen);
      }
    }

    bool reuseCcaConfig = !((startTransmitCounter == storedTransmitCounter)
                            && (failPackets == 0));

    txStatus = chooseTxType(reuseCcaConfig);

    if (txStatus != 0) {
      lastTxStatus = txStatus;
      failPackets++;
      scheduleNextTx(); // No callback will fire, so fake it
    } else if (currentAppMode() == TX_CANCEL) {
      usDelay(txCancelDelay);
      RAIL_Idle(railHandle, RAIL_IDLE_ABORT, false);
    }
  }
}

void pendFinishTxSequence()
{
  finishTxSequence = true;
}

void pendFinishTxAckSequence()
{
  finishTxAckSequence = true;
}

void finishTxSequenceIfPending()
{
  if (finishTxSequence) {
    finishTxSequence = false;

    if (logLevel & ASYNC_RESPONSE) {
      // Compute the number of packets sent
      uint32_t sentPackets = internalTransmitCounter - startTransmitCounter;
      // Print the number of sent and failed packets
      responsePrint("txEnd",
                    "txStatus:%s,"
                    "transmitted:%u,"
                    "lastTxTime:%u,"
                    "failed:%u,"
                    "lastTxStatus:0x%x,"
                    "isAck:False",
                    failPackets == 0
                    ? "Complete"
                    : (sentPackets == 0 ? "Error" : "Partial"),
                    sentPackets,
                    previousTxAppendedInfo.timeSent.packetTime,
                    failPackets,
                    lastTxStatus);
    }
    startTransmitCounter = internalTransmitCounter;
    failPackets = 0;
    lastTxStatus = 0;
  }
  if (finishTxAckSequence) {
    finishTxAckSequence = false;

    if ((logLevel & ASYNC_RESPONSE) && printTxAck) {
      // Print the number of sent and failed packets
      responsePrint("txEnd",
                    "txStatus:%s,"
                    "transmitted:%u,"
                    "lastTxTime:%u,"
                    "failed:%u,"
                    "lastTxStatus:0x%x,"
                    "isAck:True",
                    failAckPackets == 0
                    ? "Complete"
                    : (sentAckPackets == 0 ? "Error" : "Partial"),
                    sentAckPackets,
                    previousTxAckAppendedInfo.timeSent.packetTime,
                    failAckPackets,
                    lastTxAckStatus);
    }
    sentAckPackets = 0;
    failAckPackets = 0;
    lastTxAckStatus = 0;
  }
}

void printReceivedPacket()
{
  // Print any newly received packets
  if (!queueIsEmpty(&rxPacketQueue)) {
    void *rxPacketHandle = queueRemove(&rxPacketQueue);
    RxPacketData_t *rxPacketData =
      (RxPacketData_t*) memoryPtrFromHandle(rxPacketHandle);

    // Print the received packet and appended info
    if (rxPacketData != NULL) {
      char *cmdName;
      uint8_t *dataPtr = NULL;
      switch (rxPacketData->packetStatus) {
        case RAIL_RX_PACKET_ABORT_FORMAT:
          cmdName = "rxErrFmt";
          break;
        case RAIL_RX_PACKET_ABORT_FILTERED:
          cmdName = "rxErrFlt";
          break;
        case RAIL_RX_PACKET_ABORT_ABORTED:
          cmdName = "rxErrAbt";
          break;
        case RAIL_RX_PACKET_ABORT_OVERFLOW:
          cmdName = "rxErrOvf";
          break;
        case RAIL_RX_PACKET_ABORT_CRC_ERROR:
          cmdName = "rxErrCrc";
          break;
        case RAIL_RX_PACKET_READY_CRC_ERROR:
        case RAIL_RX_PACKET_READY_SUCCESS:
          cmdName = "rxPacket";
          dataPtr = rxPacketData->dataPtr;
          break;
        default:
          cmdName = "rxErr???";
          break;
      }
      printPacket(cmdName,
                  dataPtr,
                  rxPacketData->dataLength,
                  rxPacketData);
    }
    // Free the memory allocated for this packet since we're now done with it
    memoryFree(rxPacketHandle);
  }
}

void printPacket(char *cmdName,
                 uint8_t *data,
                 uint16_t dataLength,
                 RxPacketData_t *packetData)
{
  // Print out a length 0 packet message if no packet was given
  if ((data == NULL) && (packetData == NULL)) {
    responsePrint(cmdName, "len:0");
    return;
  }

  // If this is an Rx packet print the appended info
  responsePrintStart(cmdName);
  if (packetData != NULL) {
    responsePrintContinue(
      "len:%d,timeUs:%u,crc:%s,rssi:%d,lqi:%d,phy:%d,"
      "isAck:%s,syncWordId:%d,antenna:%d,channelHopIdx:%d",
      packetData->dataLength,
      packetData->appendedInfo.timeReceived.packetTime,
      (packetData->appendedInfo.crcPassed) ? "Pass" : "Fail",
      packetData->appendedInfo.rssi,
      packetData->appendedInfo.lqi,
      packetData->appendedInfo.subPhyId,
      packetData->appendedInfo.isAck ? "True" : "False",
      packetData->appendedInfo.syncWordId,
      packetData->appendedInfo.antennaId,
      packetData->appendedInfo.channelHoppingChannelIndex);
  } else {
    responsePrintContinue("len:%d", dataLength);
  }
  if (data != NULL) {
    // Manually print out payload bytes iteratively, so that we don't need to
    // reserve a RAM buffer. Finish the response here.
    printf("{payload:");
    for (int i = 0; i < dataLength; i++) {
      printf(" 0x%.2x", data[i]);
    }
    printf("}");
  }
  printf("}\n");
}

void processInputCharacters()
{
  char input;

  // Scripted mode is in effect when scriptedMarker is less than scriptedLength
  // (scriptedMarker indicates where to read from in the script entered via
  // enterScript).
  // If we're in scripted mode, read from the script instead of from the CI.
  input = scriptedMarker < scriptLength ? script[scriptedMarker] : getchar();

  while ((input != '\0') && (input != 0xFF) && ((RAIL_GetTime() - suspensionStartTime) >= suspension)) {
    suspension = 0;
    suspensionStartTime = 0;

    // Increment the marker if we're going to process it
    if (scriptedMarker < scriptLength) {
      scriptedMarker++;
    }

    if (localEcho) {
      if (input != '\n') {
        printf("%c", input);
        if (input == '\r') { // retargetserial no longer does CR => CRLF
          printf("\n");
        }
      }
    }
    if (ciProcessInput(&ciState, &input, 1) > 0) {
      printf("> ");
      return;
    }
    input = scriptedMarker < scriptLength ? script[scriptedMarker] : getchar();
  }
}

void enqueueEvents(RAIL_Events_t events)
{
  if (enablePrintEvents & events) {
    // Disable interrupts to avoid the risk of trying to enqueueing
    // two callbacks at once.
    CORE_DECLARE_IRQ_STATE;
    CORE_ENTER_CRITICAL();
    if (eventQueueMarker < EVENT_QUEUE_SIZE) {
      eventQueue[eventQueueMarker].timestamp = RAIL_GetTime();
      eventQueue[eventQueueMarker].events = events;
      eventQueueMarker++;
    } else {
      eventsMissed++;
    }
    CORE_EXIT_CRITICAL();
  }
}

// Allows for a consistent way of printing events in the order
// they were called, regardless of when they occur in the main
// loop.
void printEvents(void)
{
  EventData_t cacheEventQueue[EVENT_QUEUE_SIZE];
  uint8_t cacheEventQueueMarker = 0;
  uint32_t cacheEventsMissed = 0;

  if (eventQueueMarker) {
    // We don't want more callbacks added as we print
    CORE_DECLARE_IRQ_STATE;
    CORE_ENTER_CRITICAL();

    // cache the eventQueue
    memcpy(cacheEventQueue, eventQueue, sizeof(eventQueue));

    // cache the eventQueueMarker
    cacheEventQueueMarker = eventQueueMarker;

    // resetting the eventQueueMarker - don't need to clear
    // the actual queue, resetting the marker will just let us overwrite it
    eventQueueMarker = 0;

    cacheEventsMissed = eventsMissed;
    eventsMissed = 0;

    CORE_EXIT_CRITICAL();

    for (int position = 0; position < cacheEventQueueMarker; position++) {
      // Within each entry, check each potential bit
      RAIL_Events_t events = cacheEventQueue[position].events;
      for (int i = 0; (events >> i) != 0; i++) {
        if (events & (1 << i)) {
          responsePrint("event",
                        "timestamp:%d,eventName:RAIL_EVENT_%s",
                        cacheEventQueue[position].timestamp,
                        eventNames[i]);
        }
      }
    }
    if (cacheEventsMissed) {
      responsePrintError("printEventsMissed",
                         0x36,
                         "Event queue limited to %d callbacks. %d callbacks not enqueued.",
                         EVENT_QUEUE_SIZE,
                         cacheEventsMissed);
    }
  }
}

/******************************************************************************
 * Command Interpreter Override Functions
 *****************************************************************************/

// Error callback function using responsePrint
void ciErrorCallback(char* command, CommandError_t error)
{
  if (error == CI_UNKNOWN_COMMAND) {
    responsePrintError("ci",
                       CI_UNKNOWN_COMMAND,
                       "Unknown command '%s'",
                       command);
  } else if (error == CI_MAX_ARGUMENTS) {
    responsePrintError("ci",
                       CI_MAX_ARGUMENTS,
                       "CI_MAX_ARGUMENTS '%s'",
                       command);
  } else if (error == CI_INVALID_ARGUMENTS) {
    responsePrintError("ci",
                       CI_INVALID_ARGUMENTS,
                       "Invalid arguments for '%s'",
                       command);
  }
}

#ifdef USE_RESPONSE_HELP
// Help callback function using responsePrint
bool ciPrintHelp(CommandEntry_t *commands)
{
  responsePrintHeader("help", "command:%s,args:%s,help:%s");
  while (commands != NULL && commands->command != NULL) {
    char *args = commands->arguments;
    char *helpStr = commands->helpStr;

    if (commands->callback != NULL) {
      // Make sure that we print something for NULL pointers
      if (args == NULL) {
        args = "";
      }
      if (helpStr == NULL) {
        helpStr = "";
      }

      // Print the multiline response for this entry
      responsePrintMulti("command:%s,args:%s,help:%s",
                         commands->command,
                         args,
                         helpStr);
    }
    commands++;
  }
  return true;
}
#endif //USE_RESPONSE_HELP
