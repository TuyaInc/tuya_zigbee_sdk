/***************************************************************************//**
 * @file mode_helpers.c
 * @brief This file contains helpers for transitioning into the various
 *   AppModes
 * @copyright Copyright 2015 Silicon Laboratories, Inc. www.silabs.com
 ******************************************************************************/

#include "app_common.h"

#include "response_print.h"
#include "command_interpreter.h"

#include "rail.h"

const char *getRfStateName(RAIL_RadioState_t state)
{
  switch (state) {
    case RAIL_RF_STATE_IDLE:
      return "Idle";
    case RAIL_RF_STATE_RX:
      return "Rx";
    case RAIL_RF_STATE_TX:
      return "Tx";
    case RAIL_RF_STATE_RX_ACTIVE:
      return "RxActive";
    case RAIL_RF_STATE_TX_ACTIVE:
      return "TxActive";
    case RAIL_RF_STATE_INACTIVE:
      return "Inactive";
    default:
      //Check individual rail state bits if RAIL state is unknown
      return "Unknown";
  }
}

// Guard for CI functions to ensure a certain radio state before running
bool inRadioState(RAIL_RadioState_t state, char *command)
{
  RAIL_RadioState_t currentState = RAIL_GetRadioState(railHandle);
  bool ret;
  switch (state) {
    case RAIL_RF_STATE_INACTIVE:  // Match exactly
    case RAIL_RF_STATE_TX_ACTIVE: // Match exactly
    case RAIL_RF_STATE_RX_ACTIVE: // Match exactly
      ret = (currentState == state);
      break;
    case RAIL_RF_STATE_IDLE:      // Match IDLE or INACTIVE
      ret = (currentState <= RAIL_RF_STATE_IDLE);
      break;
    case RAIL_RF_STATE_RX:        // Match RX or RX_ACTIVE
    case RAIL_RF_STATE_TX:        // Match TX or TX_ACTIVE
      ret = ((currentState & state) == state);
      break;
    default:                      // Illegal state!?
      ret = false;
      break;
  }
  if (!ret && command) {
    responsePrintError(command, 0x17,
                       "Need to be in %s radio state for this command",
                       getRfStateName(state));
  }
  return ret;
}

const char *configuredRxAntenna(RAIL_RxOptions_t rxOptions)
{
  switch (rxOptions & (RAIL_RX_OPTION_ANTENNA_AUTO)) {
    case (RAIL_RX_OPTION_ANTENNA_AUTO): {
      return "Auto";
      break;
    }
    case (RAIL_RX_OPTION_ANTENNA0): {
      return "Antenna0";
      break;
    }
    case (RAIL_RX_OPTION_ANTENNA1): {
      return "Antenna1";
      break;
    }
    default: {
      return "Any";
      break;
    }
  }
}
