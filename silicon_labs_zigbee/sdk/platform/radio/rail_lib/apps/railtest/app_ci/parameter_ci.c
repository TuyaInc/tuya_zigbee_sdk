/***************************************************************************//**
 * @file parameter_ci.c
 * @brief This file implements the parameter commands for RAIL test applications.
 * @copyright Copyright 2015 Silicon Laboratories, Inc. www.silabs.com
 ******************************************************************************/
#include <string.h>
#include <stdio.h>

#include "command_interpreter.h"
#include "response_print.h"

#include "rail.h"
#include "rail_types.h"
#include "app_common.h"

#include "rail_config.h"

void getConfigIndex(int argc, char **argv)
{
  responsePrint(argv[0], "configIndex:%d", configIndex);
}

void setConfigIndex(int argc, char **argv)
{
  uint8_t proposedIndex = ciGetUnsigned(argv[1]);

  // Be sure that the proposed index is valid. Scan through all possible
  // indexes and check for the last NULL parameter since you can't
  // use sizeof on an extern-ed array without an explicit index.
  for (uint8_t i = 0; i <= proposedIndex; i++) {
    if (channelConfigs[i] == NULL) {
      responsePrintError(argv[0], 0x11,
                         "Invalid radio config index '%d'",
                         proposedIndex);
      return;
    }
  }

  configIndex = proposedIndex;
  RAIL_Idle(railHandle, RAIL_IDLE_ABORT, true);
  // Load the channel configuration for the specified index.
  channel = RAIL_ConfigChannels(railHandle,
                                channelConfigs[configIndex],
                                &RAILCb_RadioConfigChanged);
  responsePrint(argv[0], "configIndex:%d,firstAvailableChannel:%d",
                configIndex,
                channel);
}

void getChannel(int argc, char **argv)
{
  if (RAIL_GetDebugMode(railHandle) & RAIL_DEBUG_MODE_FREQ_OVERRIDE) {
    responsePrintError(argv[0], 0x12, "Channels are not valid in Debug Mode");
  } else {
    responsePrint(argv[0], "channel:%d", channel);
  }
}

void setChannel(int argc, char **argv)
{
  uint16_t proposedChannel = ciGetUnsigned(argv[1]);
  bool success = false;

  // Make sure this is a valid channel
  if (RAIL_IsValidChannel(railHandle, proposedChannel)
      == RAIL_STATUS_NO_ERROR) {
    changeChannel(proposedChannel);
    success = true;
  }

  if (!success) {
    responsePrintError(argv[0], 0x11, "Invalid channel '%d'", proposedChannel);
    return;
  }

  getChannel(1, argv);
}

void getPower(int argc, char **argv)
{
  responsePrint(argv[0],
                "powerLevel:%d,power:%d",
                RAIL_GetTxPower(railHandle),
                RAIL_GetTxPowerDbm(railHandle));
}

void setPower(int argc, char **argv)
{
  if (!inRadioState(RAIL_RF_STATE_IDLE, argv[0])) {
    return;
  }
  RAIL_TxPowerLevel_t powerLevel;
  RAIL_TxPower_t power;

  if (argc >= 3 && strcmp(argv[2], "raw") == 0) {
    RAIL_SetTxPower(railHandle, ciGetUnsigned(argv[1]));
  } else {
    RAIL_SetTxPowerDbm(railHandle, ciGetSigned(argv[1]));
  }

  powerLevel = RAIL_GetTxPower(railHandle);
  lastSetTxPowerLevel = powerLevel;
  power = RAIL_GetTxPowerDbm(railHandle);

  responsePrint(argv[0], "powerLevel:%d,power:%d", powerLevel, power);
}

void sweepTxPower(int argc, char **argv)
{
  responsePrint(argv[0], "Sweeping:Started,Instructions:'q' to quit or 'enter' to continue.");
  RAIL_TxPowerConfig_t txPowerConfig;

  RAIL_GetTxPowerConfig(railHandle, &txPowerConfig);

  RAIL_TxPowerLevel_t start = 1;
  RAIL_TxPowerLevel_t end = 255;

  switch (txPowerConfig.mode) {
    case RAIL_TX_POWER_MODE_2P4_HP:
      //start = RAIL_TX_POWER_LEVEL_HP_MIN;
      end = RAIL_TX_POWER_LEVEL_HP_MAX;
      break;
    case RAIL_TX_POWER_MODE_2P4_LP:
      //start = RAIL_TX_POWER_LEVEL_LP_MIN;
      end = RAIL_TX_POWER_LEVEL_LP_MAX;
      break;
    #ifdef RAIL_TX_POWER_MODE_SUBGIG
    case RAIL_TX_POWER_MODE_SUBGIG:
      //start = RAIL_TX_POWER_LEVEL_SUBGIG_MIN;
      end = RAIL_TX_POWER_LEVEL_SUBGIG_MAX;
      break;
    #endif
    default:
      responsePrintError(argv[0], 0x21, "PA not configured.");
      return;
  }

  char input;
  RAIL_TxPowerLevel_t i;

  for (i = start; i <= end; i++) {
    responsePrint(argv[0], "PowerLevel:%u", i);
    RAIL_Idle(railHandle, RAIL_IDLE_FORCE_SHUTDOWN_CLEAR_FLAGS, true);
    RAIL_SetTxPower(railHandle, i);
    RAIL_StartTxStream(railHandle, channel, RAIL_STREAM_CARRIER_WAVE);

    input = getchar();

    while (1) {
      if (input == '\n' || input == '\r') {
        break;
      }
      if (input == 'q') {
        responsePrintError(argv[0], 0x20, "Sweep Aborted.");
        return;
      }
      input = getchar();
    }

    RAIL_Idle(railHandle, RAIL_IDLE_FORCE_SHUTDOWN_CLEAR_FLAGS, true);
  }

  responsePrint(argv[0], "Sweeping:Complete");
}

void getCtune(int argc, char **argv)
{
  uint32_t ctune = RAIL_GetTune(railHandle);

  responsePrint(argv[0], "CTUNE:0x%.3x", ctune);
}

void setCtune(int argc, char **argv)
{
  if (!inRadioState(RAIL_RF_STATE_IDLE, argv[0])) {
    return;
  }

  RAIL_SetTune(railHandle, ciGetUnsigned(argv[1]));

  // Read out and print the current CTUNE value
  getCtune(1, argv);
}

void setPaCtune(int argc, char **argv)
{
  RAIL_Status_t status;
  uint8_t txVal = ciGetUnsigned(argv[1]);
  uint8_t rxVal = ciGetUnsigned(argv[2]);

  status = RAIL_SetPaCTune(railHandle, txVal, rxVal);

  if (status == RAIL_STATUS_NO_ERROR) {
    responsePrint(argv[0], "PACTUNETX:%d,PACTUNERX:%d", txVal, rxVal);
  } else {
    responsePrintError(argv[0], status, "Error");
  }
}

void enablePaCal(int argc, char **argv)
{
  uint8_t enable = ciGetUnsigned(argv[1]);
  RAIL_EnablePaCal(enable);
  responsePrint(argv[0], "paCal:%s", (enable ? "Enabled" : "Disabled"));
}

// Helper to convert two strings to two RAIL RadioStates
static int8_t stringsToStates(char **strings, RAIL_RadioState_t *states)
{
  for (int i = 0; i < 2; i++) {
    switch (strings[i][0]) {
      case 'i': case 'I':
        states[i] =  RAIL_RF_STATE_IDLE;
        break;

      case 'r': case 'R':
        states[i] =  RAIL_RF_STATE_RX;
        break;

      case 't': case 'T':
        states[i] =  RAIL_RF_STATE_TX;
        break;

      default:
        return 1;
    }
  }
  return 0;
}

void setTxTransitions(int argc, char **argv)
{
  RAIL_RadioState_t states[2];
  if (stringsToStates(&argv[1], &states[0])) {
    responsePrintError(argv[0], 0x16, "Invalid states");
    return;
  }

  RAIL_StateTransitions_t transitions = {
    .success = states[0],
    .error = states[1]
  };

  RAIL_Status_t ret = RAIL_SetTxTransitions(railHandle, &transitions);
  responsePrint(argv[0], "TxTransitions:%s", (ret ? "Error" : "Set"));
}

void setRxTransitions(int argc, char **argv)
{
  if (!inRadioState(RAIL_RF_STATE_IDLE, argv[0])) {
    return;
  }
  RAIL_RadioState_t states[2];
  if (stringsToStates(&argv[1], &states[0])) {
    responsePrintError(argv[0], 0x16, "Invalid states");
    return;
  }
  RAIL_StateTransitions_t transitions = {
    .success = states[0],
    .error = states[1]
  };
  RAIL_Status_t ret = RAIL_SetRxTransitions(railHandle, &transitions);
  if (ret == RAIL_STATUS_NO_ERROR) {
    rxSuccessTransition = states[0];
  }
  responsePrint(argv[0], "RxTransitions:%s", (ret ? "Error" : "Set"));
}

void setTimings(int argc, char **argv)
{
  uint16_t timing[6] = { 0 };
  for (int i = 1; i < argc; i++) {
    timing[i - 1] = ciGetUnsigned(argv[i]);
  }
  RAIL_StateTiming_t timings =
  { timing[0], timing[1], timing[2], timing[3], timing[4], timing[5] };
  if (!RAIL_SetStateTiming(railHandle, &timings)) {
    responsePrint(argv[0], "IdleToRx:%u,RxToTx:%u,IdleToTx:%u,TxToRx:%u,"
                           "RxSearch:%u,Tx2RxSearch:%u",
                  timings.idleToRx, timings.rxToTx, timings.idleToTx,
                  timings.txToRx, timings.rxSearchTimeout, timings.txToRxSearchTimeout);
  } else {
    responsePrintError(argv[0], 0x18, "Setting timings failed");
  }
}

void setTxFifoThreshold(int argc, char **argv)
{
  if (railDataConfig.txMethod != FIFO_MODE) {
    responsePrintError(argv[0], 0x19, "Tx is not in FIFO mode");
    return;
  }

  uint16_t txFifoThreshold = ciGetUnsigned(argv[1]);
  txFifoThreshold = RAIL_SetTxFifoThreshold(railHandle, txFifoThreshold);
  responsePrint(argv[0], "TxFifoThreshold:%d", txFifoThreshold);
}

void setRxFifoThreshold(int argc, char **argv)
{
  uint16_t rxFifoThreshold = ciGetUnsigned(argv[1]);
  rxFifoThreshold = RAIL_SetRxFifoThreshold(railHandle, rxFifoThreshold);
  responsePrint(argv[0], "RxFifoThreshold:%d", rxFifoThreshold);
}

void setEventConfig(int argc, char **argv)
{
  RAIL_Events_t eventMask = ciGetUnsigned(argv[1]);
  RAIL_Events_t eventConfig = ciGetUnsigned(argv[2]);

  RAIL_ConfigEvents(railHandle, eventMask, eventConfig);
  responsePrint(argv[0], "Mask:0x%llx,Values:0x%llx", eventMask, eventConfig);
}
