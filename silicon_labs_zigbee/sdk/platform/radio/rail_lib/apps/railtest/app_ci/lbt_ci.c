/***************************************************************************//**
 * @file lbt_ci.c
 * @brief This file implements the LBT commands for RAIL test applications.
 * @copyright Copyright 2015 Silicon Laboratories, Inc. www.silabs.com
 ******************************************************************************/
#include <string.h>

#include "command_interpreter.h"
#include "response_print.h"

#include "rail.h"
#include "app_common.h"

RailTxType_t txType = TX_TYPE_NORMAL;
RAIL_LbtConfig_t *lbtConfig = NULL;
RAIL_CsmaConfig_t *csmaConfig = NULL;

static RAIL_LbtConfig_t lbtParams = RAIL_CSMA_CONFIG_802_15_4_2003_2p4_GHz_OQPSK_CSMA;

void setLbtMode(int argc, char **argv)
{
  if (argc > 1) {
    if (memcmp(argv[1], "off", 3) == 0) {
      txType = TX_TYPE_NORMAL;
    } else if (memcmp(argv[1], "csma", 4) == 0) {
      txType = TX_TYPE_CSMA;
      csmaConfig = (RAIL_CsmaConfig_t*)&lbtParams;
    } else if (memcmp(argv[1], "lbt", 3) == 0) {
      txType = TX_TYPE_LBT;
      lbtConfig = &lbtParams; // Used for CSMA and LBT
    } else {
      responsePrintError(argv[0], 0x70, "Unknown LBT mode specified.");
      return;
    }
  }
  if (txType == TX_TYPE_NORMAL) {
    responsePrint(argv[0], "LbtMode:off");
  } else if (txType == TX_TYPE_CSMA) {
    responsePrint(argv[0], "LbtMode:CSMA");
  } else if (txType == TX_TYPE_LBT) {
    responsePrint(argv[0], "LbtMode:LBT");
  }
}

void getLbtParams(int argc, char **argv)
{
  responsePrint(argv[0],
                "MinBo:%u,"
                "MaxBo:%u,"
                "Tries:%u,"
                "Threshold:%d,"
                "Backoff:%u,"
                "Duration:%u,"
                "Timeout:%u",
                lbtParams.lbtMinBoRand,
                lbtParams.lbtMaxBoRand,
                lbtParams.lbtTries,
                lbtParams.lbtThreshold,
                lbtParams.lbtBackoff,
                lbtParams.lbtDuration,
                lbtParams.lbtTimeout);
}

void setLbtParams(int argc, char **argv)
{
  lbtParams.lbtMinBoRand = ciGetUnsigned(argv[1]);
  lbtParams.lbtMaxBoRand = ciGetUnsigned(argv[2]);
  lbtParams.lbtTries     = ciGetUnsigned(argv[3]);
  lbtParams.lbtThreshold = ciGetSigned(argv[4]);
  lbtParams.lbtBackoff   = ciGetUnsigned(argv[5]);
  lbtParams.lbtDuration  = ciGetUnsigned(argv[6]);
  lbtParams.lbtTimeout   = ciGetUnsigned(argv[7]);

  getLbtParams(1, argv);
}
