// -----------------------------------------------------------------------------
/// @file
/// @brief Radio coexistence BLE utilities
///
/// @author Silicon Laboratories Inc.
/// @version 1.0.0
///
/// @section License
/// <b>(C) Copyright 2017 Silicon Laboratories, http://www.silabs.com</b>
///
/// This file is licensed under the Silabs License Agreement. See the file
/// "Silabs_License_Agreement.txt" for details. Before using this software for
/// any purpose, you must agree to the terms of that agreement.
///
// -----------------------------------------------------------------------------
#include "coexistence-ble.h"

static RAIL_Handle_t myRailHandle;
static COEX_ReqState_t txReq;
static COEX_ReqState_t rxReq;
static COEX_ReqState_t swReq;

extern uint16_t RFRAND_GetPseudoRandom(void);

static void randomDelayCb(uint16_t randomDelayMaskUs)
{
  uint32_t startTime = RAIL_GetTime();
  uint16_t delay = RFRAND_GetPseudoRandom() & randomDelayMaskUs;

  while ((uint16_t)(RAIL_GetTime() - startTime) > delay) ;
}

static void eventsCb(COEX_Events_t events)
{
  if ((events & COEX_EVENT_GRANT_RELEASED) != 0U
      && (COEX_GetOptions() & COEX_OPTION_TX_ABORT) != 0U
      && (txReq.coexReq & COEX_REQ_ON) != 0U) {
    COEX_SetRequest(&txReq, COEX_REQ_OFF, NULL);
    (void)RAIL_StopTx(myRailHandle, RAIL_STOP_MODE_ACTIVE);
  }
  if ((events & COEX_EVENT_HOLDOFF_ENABLED) != 0U) {
    RAIL_EnableTxHoldOff(myRailHandle, true);
  } else if ((events & COEX_EVENT_HOLDOFF_DISABLED) != 0U) {
    RAIL_EnableTxHoldOff(myRailHandle, false);
  }
}

bool COEX_BLE_SetTxRequest(COEX_Req_t coexReq, COEX_ReqCb_t cb)
{
  return COEX_SetRequest(&txReq, coexReq, cb);
}

bool COEX_BLE_SetRxRequest(COEX_Req_t coexReq, COEX_ReqCb_t cb)
{
  return COEX_SetRequest(&rxReq, coexReq, cb);
}

bool COEX_BLE_SetSwRequest(COEX_Req_t coexReq, COEX_ReqCb_t cb)
{
  return COEX_SetRequest(&swReq, coexReq, cb);
}

void COEX_BLE_Init(RAIL_Handle_t railHandle)
{
  myRailHandle = railHandle;

  COEX_SetRandomDelayCallback(&randomDelayCb);
  COEX_SetRadioCallback(&eventsCb);
  COEX_HAL_Init();
}
