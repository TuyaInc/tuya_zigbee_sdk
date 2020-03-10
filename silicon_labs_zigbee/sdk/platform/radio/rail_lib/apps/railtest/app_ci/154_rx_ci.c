/***************************************************************************//**
 * @file 154_rx_ci.c
 * @brief This file implements commands for configuring 802.15.4 options
 * relevant to receiving packets
 * @copyright Copyright 2015 Silicon Laboratories, Inc. www.silabs.com
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command_interpreter.h"
#include "response_print.h"

#include "rail.h"
#include "rail_ieee802154.h"

#include "app_common.h"

void ieee802154Enable(int argc, char **argv)
{
  if (!inRadioState(RAIL_RF_STATE_IDLE, argv[0])) {
    return;
  }

  RAIL_StateTiming_t timings = {
    .idleToTx = 100,
    .idleToRx = 100,
    .rxToTx = 192,
    // Make txToRx slightly lower than desired to make sure we get to
    // RX in time
    .txToRx = 192 - 10,
    .rxSearchTimeout = 0,
    .txToRxSearchTimeout = 0
  };

  RAIL_IEEE802154_Config_t config = {
    .addresses = NULL,
    .ackConfig = {
      .enable = true,
      .ackTimeout = 1000,
      .rxTransitions = {
        .success = RAIL_RF_STATE_RX,
        .error = RAIL_RF_STATE_RX // ignored
      },
      .txTransitions = {
        .success = RAIL_RF_STATE_RX,
        .error = RAIL_RF_STATE_RX // ignored
      }
    },
    .timings = timings,
    .framesMask = RAIL_IEEE802154_ACCEPT_STANDARD_FRAMES,
    .promiscuousMode = false,
    .isPanCoordinator = false
  };

  if (memcmp(argv[1], "idle", 4) == 0) {
    config.ackConfig.rxTransitions.success = RAIL_RF_STATE_IDLE;
    config.ackConfig.txTransitions.success = RAIL_RF_STATE_IDLE;
  } else if (memcmp(argv[1], "rx", 2) == 0) {
    config.ackConfig.rxTransitions.success = RAIL_RF_STATE_RX;
    config.ackConfig.txTransitions.success = RAIL_RF_STATE_RX;
  } else {
    responsePrintError(argv[0], 0x20, "Unknown auto ack default state.");
    return;
  }

  uint16_t timing = ciGetUnsigned(argv[2]);
  if (timing > 13000) {
    responsePrintError(argv[0], 0x21, "Invalid idle timing.");
    return;
  } else {
    config.timings.idleToTx = timing;
    config.timings.idleToRx = timing;
  }

  timing = ciGetUnsigned(argv[3]);
  if (timing > 13000) {
    responsePrintError(argv[0], 0x22, "Invalid turnaround timing");
    return;
  } else {
    config.timings.rxToTx = timing;
    config.timings.txToRx = timing - 10;
  }

  timing = ciGetUnsigned(argv[4]);
  config.ackConfig.ackTimeout = timing;

  RAIL_Status_t status = RAIL_IEEE802154_Init(railHandle, &config);
  if (status != RAIL_STATUS_NO_ERROR) {
    responsePrintError(argv[0], status, "Call to RAIL_IEEE802154_Init returned an error");
  } else {
    responsePrint(argv[0],
                  "802.15.4:%s,"
                  "rxDefaultState:%s,"
                  "txDefaultState:%s,"
                  "idleTiming:%d,"
                  "turnaroundTime:%d,"
                  "ackTimeout:%d",
                  status ? "Disabled" : "Enabled",
                  getRfStateName(config.ackConfig.txTransitions.success),
                  getRfStateName(config.ackConfig.rxTransitions.success),
                  config.timings.idleToTx,
                  config.timings.rxToTx,
                  config.ackConfig.ackTimeout);
  }
}

void config2p4Ghz802154(int argc, char **argv)
{
  RAIL_Status_t status;
  bool antDivConfig = false;
  if (!inRadioState(RAIL_RF_STATE_IDLE, argv[0])) {
    return;
  }
  if (argc > 1) {
    antDivConfig = ciGetUnsigned(argv[1]);
  }
  if (antDivConfig) {
    status = RAIL_IEEE802154_Config2p4GHzRadioAntDiv(railHandle);
  } else {
    status = RAIL_IEEE802154_Config2p4GHzRadio(railHandle);
  }
  if (status == RAIL_STATUS_NO_ERROR) {
    changeChannel(11);
  }
  responsePrint(argv[0], "802.15.4:%s", status ? "Disabled" : "Enabled");
}

void config863Mhz802154(int argc, char **argv)
{
  if (!inRadioState(RAIL_RF_STATE_IDLE, argv[0])) {
    return;
  }

  RAIL_Status_t status = RAIL_IEEE802154_ConfigGB863MHzRadio(railHandle);
  changeChannel(0x80);
  responsePrint(argv[0], "802.15.4:%s", status ? "Disabled" : "Enabled");
}

void config915Mhz802154(int argc, char **argv)
{
  if (!inRadioState(RAIL_RF_STATE_IDLE, argv[0])) {
    return;
  }

  RAIL_Status_t status = RAIL_IEEE802154_ConfigGB915MHzRadio(railHandle);
  changeChannel(0xE0);
  responsePrint(argv[0], "802.15.4:%s", status ? "Disabled" : "Enabled");
}

void ieee802154AcceptFrames(int argc, char **argv)
{
  uint8_t framesEnable = 0;
  bool commandFrame = ciGetUnsigned(argv[1]);
  bool ackFrame = ciGetUnsigned(argv[2]);
  bool dataFrame = ciGetUnsigned(argv[3]);
  bool beaconFrame = ciGetUnsigned(argv[4]);

  // Command
  if (commandFrame) {
    framesEnable |= RAIL_IEEE802154_ACCEPT_COMMAND_FRAMES;
  }
  if (ackFrame) {
    framesEnable |= RAIL_IEEE802154_ACCEPT_ACK_FRAMES;
  }
  if (dataFrame) {
    framesEnable |= RAIL_IEEE802154_ACCEPT_DATA_FRAMES;
  }
  if (beaconFrame) {
    framesEnable |= RAIL_IEEE802154_ACCEPT_BEACON_FRAMES;
  }

  RAIL_Status_t status = RAIL_IEEE802154_AcceptFrames(railHandle, framesEnable);
  if (status != RAIL_STATUS_NO_ERROR) {
    responsePrintError(argv[0], 0x23, "Failed to set which frames to accept.");
  } else {
    responsePrint(argv[0],
                  "CommandFrame:%s,"
                  "AckFrame:%s,"
                  "DataFrame:%s,"
                  "BeaconFrame:%s",
                  commandFrame ? "Enabled" : "Disabled",
                  ackFrame ? "Enabled" : "Disabled",
                  dataFrame ? "Enabled" : "Disabled",
                  beaconFrame ? "Enabled" : "Disabled");
  }
}

void ieee802154SetPromiscuousMode(int argc, char **argv)
{
  bool promiscuous = ciGetUnsigned(argv[1]);
  RAIL_Status_t status = RAIL_IEEE802154_SetPromiscuousMode(railHandle, promiscuous);
  if (status != RAIL_STATUS_NO_ERROR) {
    responsePrintError(argv[0], 0x24, "Failed to (un)set promiscuous mode.");
  } else {
    responsePrint(argv[0], "PromiscuousMode:%s",
                  promiscuous ? "Enabled" : "Disabled");
  }
}

void ieee802154SetPanCoordinator(int argc, char **argv)
{
  bool panCoord = ciGetUnsigned(argv[1]);
  RAIL_Status_t status = RAIL_IEEE802154_SetPanCoordinator(railHandle, panCoord);
  if (status != RAIL_STATUS_NO_ERROR) {
    responsePrintError(argv[0], 0x24, "Failed to (un)set PAN Coordinator.");
  } else {
    responsePrint(argv[0], "PanCoordinator:%s",
                  panCoord ? "Enabled" : "Disabled");
  }
}

void ieee802154SetPanId(int argc, char **argv)
{
  uint16_t panId = ciGetUnsigned(argv[1]);
  uint8_t index = (argc > 2) ? ciGetUnsigned(argv[2]) : 0;
  RAIL_Status_t status = RAIL_IEEE802154_SetPanId(railHandle, panId, index);
  responsePrint(argv[0], "802.15.4PanId:%s", getStatusMessage(status));
}

void ieee802154SetShortAddress(int argc, char **argv)
{
  uint16_t shortAddr = ciGetUnsigned(argv[1]);
  uint8_t index = (argc > 2) ? ciGetUnsigned(argv[2]) : 0;
  RAIL_Status_t status = RAIL_IEEE802154_SetShortAddress(railHandle,
                                                         shortAddr,
                                                         index);
  responsePrint(argv[0], "802.15.4ShortAddress:%s", getStatusMessage(status));
}

void ieee802154SetLongAddress(int argc, char **argv)
{
  uint8_t longAddr[8];
  for (int i = 0; i < 8; i++) {
    longAddr[i] = ciGetUnsigned(argv[i + 1]);
  }
  uint8_t index = (argc > 9) ? ciGetUnsigned(argv[9]) : 0;
  RAIL_Status_t status = RAIL_IEEE802154_SetLongAddress(railHandle,
                                                        longAddr,
                                                        index);
  responsePrint(argv[0], "802.15.4LongAddress:%s", getStatusMessage(status));
}

void ieee802154SetAddresses(int argc, char **argv)
{
  RAIL_IEEE802154_AddrConfig_t addresses = {
    { 0xFFFF, 0xFFFF, 0xFFFF },
    { 0xFFFF, 0xFFFF, 0xFFFF },
    { { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
      { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
      { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }
  };

  uint8_t argIndex = 1; // Skip the command argument
  for (int i = 0; i < RAIL_IEEE802154_MAX_ADDRESSES; i++) {
    if (argIndex >= argc) {
      break;
    }
    addresses.panId[i] = ciGetUnsigned(argv[argIndex]);
    argIndex++;

    if (argIndex >= argc) {
      break;
    }
    addresses.shortAddr[i] = ciGetUnsigned(argv[argIndex]);
    argIndex++;

    if (argIndex >= argc) {
      break;
    }
    uint64_t longAddr = strtoull(argv[argIndex], NULL, 0);
    for (int j = 0; j < 8; j++) {
      addresses.longAddr[i][j] = (longAddr >> (i * 8)) & 0xFF;
    }
    argIndex++;
  }
  RAIL_Status_t status = RAIL_IEEE802154_SetAddresses(railHandle, &addresses);
  responsePrint(argv[0], "802.15.4Addresses:%s", getStatusMessage(status));
}
