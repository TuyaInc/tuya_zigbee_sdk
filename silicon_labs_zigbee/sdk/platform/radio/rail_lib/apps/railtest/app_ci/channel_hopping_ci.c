/***************************************************************************//**
 * @file autoack.c
 * @brief This file implements the autoack commands in RAIL test apps.
 * @copyright Copyright 2015 Silicon Laboratories, Inc. www.silabs.com
 ******************************************************************************/
#include <stdio.h>
#include <string.h>

#include "command_interpreter.h"
#include "response_print.h"

#include "rail.h"
#include "app_common.h"
#include "app_ci.h"

#define MAX_NUMBER_CHANNELS 4

// 100 32 bit words per channel should be plenty
#define CHANNEL_HOPPING_BUFFER_SIZE 400

RAIL_RxChannelHoppingConfigEntry_t channelHoppingEntries[MAX_NUMBER_CHANNELS];
uint32_t channelHoppingBuffer[CHANNEL_HOPPING_BUFFER_SIZE];
RAIL_RxChannelHoppingConfig_t channelHoppingConfig = {
  .entries = channelHoppingEntries,
  .buffer = channelHoppingBuffer,
  .bufferLength = CHANNEL_HOPPING_BUFFER_SIZE,
  .numberOfChannels = 0
};

void configRxChannelHopping(int argc, char **argv)
{
  uint8_t i;
  for (i = 1; i * 4 < argc; i++) {
    if (ciGetUnsigned(argv[(i * 4) - 3]) > (uint32_t)UINT16_MAX) {
      responsePrintError(argv[0], 0x16, "Channel must be a 16 bit value.");
      return;
    }
    channelHoppingEntries[i - 1].channel = ciGetUnsigned(argv[(i * 4) - 3]);
    channelHoppingEntries[i - 1].mode = ciGetUnsigned(argv[(i * 4) - 2]);
    if (ciGetUnsigned(argv[i * 4 - 2]) > RAIL_RX_CHANNEL_HOPPING_MODE_PREAMBLE_SENSE) {
      responsePrintError(argv[0], 0x17, "Mode must be a valid RAIL_RxChannelHoppingMode_t value.");
      return;
    }
    channelHoppingEntries[i - 1].parameter = ciGetUnsigned(argv[i * 4 - 1]);
    channelHoppingEntries[i - 1].delay = ciGetUnsigned(argv[i * 4]);
    channelHoppingEntries[i - 1].delayMode = RAIL_RX_CHANNEL_HOPPING_DELAY_MODE_STATIC;
  }
  channelHoppingConfig.numberOfChannels = i - 1;

  RAIL_Status_t status = RAIL_ConfigRxChannelHopping(railHandle, &channelHoppingConfig);
  responsePrint(argv[0], "numberOfChannels:%d,buffer:0x%x,Success:%s", i - 1, channelHoppingBuffer, status == RAIL_STATUS_NO_ERROR ? "True" : "False");
}

void enableRxChannelHopping(int argc, char **argv)
{
  bool enable = !!ciGetUnsigned(argv[1]);

  bool reset = false;
  if (argc > 2) {
    reset = !!ciGetUnsigned(argv[2]);
  }

  RAIL_Status_t status = RAIL_EnableRxChannelHopping(railHandle, enable, reset);
  responsePrint(argv[0], "Success:%s", status == RAIL_STATUS_NO_ERROR ? "True" : "False");
}
