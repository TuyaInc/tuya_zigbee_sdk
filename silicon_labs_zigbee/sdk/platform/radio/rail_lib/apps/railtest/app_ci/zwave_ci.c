/***************************************************************************//**
 * @file zwave_ci.c
 * @brief This file implements commands for configuring ZWAVE RAIL options
 * relevant to receiving packets
 * @copyright Copyright 2018 Silicon Laboratories, Inc. www.silabs.com
 ******************************************************************************/
#include <stdio.h>
#include <string.h>
#ifdef CONFIGURATION_HEADER
#include CONFIGURATION_HEADER
#endif
#include "em_core.h"
#include "command_interpreter.h"
#include "response_print.h"
#include "rail.h"
#include "rail_zwave.h"
#include "app_common.h"
#include "app_ci.h"
#include "bsp.h"

// Macro to determine array size.
#define COMMON_UTILS_COUNTOF(a) (sizeof(a) / sizeof((a)[0]))

static RAIL_ZWAVE_Config_t config = {
  .options = RAIL_ZWAVE_OPTIONS_NONE,
};

typedef struct ZWAVE_Region {
  const char *name;
  RAIL_ZWAVE_ChannelConfig_t const * config;
} ZWAVE_Region_t;

/******************* Local functions *******************/
static const ZWAVE_Region_t zwaveRegionTable[] = {
  { "EU-European Union", RAIL_ZWAVE_REGION_EU },
  { "US-United States", RAIL_ZWAVE_REGION_US },
  { "ANZ-Australia/New Zealand", RAIL_ZWAVE_REGION_ANZ },
  { "HK-Hong Kong", RAIL_ZWAVE_REGION_HK },
  { "MY-Malaysia", RAIL_ZWAVE_REGION_MY },
  { "IN-India", RAIL_ZWAVE_REGION_IN },
  { "JP-Japan", RAIL_ZWAVE_REGION_JP },
  { "RU-Russia", RAIL_ZWAVE_REGION_RU },
  { "IL-Israel", RAIL_ZWAVE_REGION_IL },
  { "KR-Korea", RAIL_ZWAVE_REGION_KR },
  { "CN-China", RAIL_ZWAVE_REGION_CN },
};

#define ZWAVE_REGION_UNDEFINED (COMMON_UTILS_COUNTOF(zwaveRegionTable))
static uint8_t configuredRegion = ZWAVE_REGION_UNDEFINED;

static const char* baudrateNames[] = {
  "9600bps",
  "40Kbps",
  "100Kbps"
};

void zwaveListRegions(int argc, char **argv)
{
  uint8_t i;
  responsePrintStart(argv[0]);
  for (i = 0;
       i < (uint8_t)ZWAVE_REGION_UNDEFINED - 1;
       ++i) {
    responsePrintContinue("%i:%s", i, zwaveRegionTable[i].name);
  }
  responsePrintEnd("%i:%s", i, zwaveRegionTable[(uint8_t)ZWAVE_REGION_UNDEFINED - 1].name);
}

void zwaveStatus(int argc, char **argv)
{
  bool enabled = RAIL_ZWAVE_IsEnabled(railHandle);

  // Report the current enabled status for ZWAVE
  responsePrint(argv[0],
                "ZWAVE:%s,"
                "Promiscuous:%s,"
                "BeamDetect:%s",
                enabled ? "Enabled" : "Disabled",
                ((config.options & RAIL_ZWAVE_OPTION_PROMISCUOUS_MODE) != 0U)
                ? "Enabled" : "Disabled",
                ((config.options & RAIL_ZWAVE_OPTION_DETECT_BEAM_FRAMES) != 0U)
                ? "Enabled" : "Disabled");
}

void zwaveEnable(int argc, char **argv)
{
  if (!inRadioState(RAIL_RF_STATE_IDLE, argv[0])) {
    return;
  }

  bool enable = !!ciGetUnsigned(argv[1]);
  if (argc > 2) {
    config.options = ciGetUnsigned(argv[2]);
  }

  // Turn ZWAVE mode on or off as requested
  if (enable) {
    RAIL_ZWAVE_Init(railHandle, &config);
  } else {
    RAIL_ZWAVE_Deinit(railHandle);
  }

  // Report the current status of ZWAVE mode
  zwaveStatus(1, argv);
}

void zwaveGetRegion(int argc, char **argv)
{
  if (configuredRegion < ZWAVE_REGION_UNDEFINED) {
    responsePrint(argv[0],
                  "ZWaveRegion:%s,ZWaveRegionIndex:%i",
                  zwaveRegionTable[configuredRegion].name,
                  configuredRegion);
  } else {
    responsePrint(argv[0],
                  "ZWaveRegion:Undefined,ZWaveRegionIndex:%i",
                  configuredRegion);
  }
}

void zwaveSetRegion(int argc, char **argv)
{
  if (!inRadioState(RAIL_RF_STATE_IDLE, argv[0])) {
    return;
  }
  uint8_t region = (uint8_t)ciGetUnsigned(argv[1]);
  if (region >= ZWAVE_REGION_UNDEFINED) {
    responsePrintError(argv[0], 0x25,
                       "Unsupported Z-Wave Region.");
    zwaveListRegions(argc, argv);
    return;
  }
  configuredRegion = region;
  if (RAIL_ZWAVE_ConfigRegion(railHandle, zwaveRegionTable[configuredRegion].config) != RAIL_STATUS_NO_ERROR) {
    configuredRegion = ZWAVE_REGION_UNDEFINED;
  }
  zwaveGetRegion(argc, argv);
}

void zwaveSetNodeId(int argc, char **argv)
{
  RAIL_ZWAVE_NodeId_t nodeId = ciGetUnsigned(argv[1]);
  RAIL_Status_t status = RAIL_ZWAVE_SetNodeId(railHandle, nodeId);
  responsePrint(argv[0], "Status:%s", status ? "Error" : "Set");
}

void zwaveSetHomeId(int argc, char **argv)
{
  RAIL_ZWAVE_HomeId_t homeId = ciGetUnsigned(argv[1]);
  RAIL_ZWAVE_HomeId_t homeIdHash = ciGetUnsigned(argv[2]);
  RAIL_Status_t status = RAIL_ZWAVE_SetHomeId(railHandle, homeId, homeIdHash);
  responsePrint(argv[0], "Status:%s", status ? "Error" : "Set");
}

void zwaveGetBaudRate(int argc, char **argv)
{
  uint16_t channel = -1;
  RAIL_GetChannel(railHandle, &channel);
  if (channel < 3 && configuredRegion < ZWAVE_REGION_UNDEFINED) {
    responsePrint(argv[0],
                  "baudrate:%s",
                  baudrateNames[zwaveRegionTable[configuredRegion].config[channel].baudRate]);
  } else {
    responsePrint(argv[0],
                  "baudrate:Undefined");
  }
}
