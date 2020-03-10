/**************************************************************************//**
 * Copyright 2016 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#include "em_device.h"
#include "em_chip.h"

#include "cslib_hwconfig.h"
#include "cslib_config.h"
#include "cslib.h"
#include "hardware_routines.h"
#include "low_power_config.h"
#include "em_emu.h"
#include "em_device.h"
#include "em_cmu.h"
#include "em_csen.h"

uint8_t timerTick;
extern uint8_t CSENtimerTick;
CSEN_SingleSel_TypeDef CSLIB_findAPortForIndex(uint8_t, uint32_t, uint32_t);
void setupCSLIBClock(uint32_t, CSEN_Init_TypeDef*);

/**************************************************************************//**
 * Configure sensor for sleep mode
 *
 * Configures sensor peripheral(s) for sleep mode scanning
 *
 *****************************************************************************/
void CSLIB_configureSensorForSleepModeCB(void)
{
  CSEN_Init_TypeDef csen_init = CSEN_INIT_DEFAULT;

  static const CSEN_InitMode_TypeDef sleep_mode = CSEN_SLEEPMODE_DEFAULT;

  setupCSLIBClock(DEF_SLEEP_MODE_PERIOD, &csen_init);

  // Set converter and scan configuration
  CMU_ClockEnable(cmuClock_CSEN_HF, true);
  CSEN_Init(CSEN, &csen_init);

  CSEN_InitMode(CSEN, &sleep_mode);

  CMU_ClockEnable(cmuClock_LDMA, true);

  CSEN_Enable(CSEN);
}

void InitSleepModeBaseline(void)
{
  uint32_t value;
  CSEN_InitMode_TypeDef sleep_mode = CSEN_SLEEPMODE_DEFAULT;

  sleep_mode.trigSel = csenTrigSelStart;

  CSEN_Disable(CSEN);
  CSEN->IFC = CSEN->IF;
  CSEN_InitMode(CSEN, &sleep_mode);

  CSEN_Enable(CSEN);

  value = executeConversion();

  CSEN->EMA = value;

  CSEN->IFC = CSEN->IF;

  CSEN_IntEnable(CSEN, CSEN_IEN_CMP);
  NVIC_ClearPendingIRQ(CSEN_IRQn);
  NVIC_EnableIRQ(CSEN_IRQn);
  CSLIB_configureSensorForSleepModeCB();
}

/**************************************************************************//**
 * Configure to sleep mode
 *
 * Re-enable and get system ready for active mode
 *
 *****************************************************************************/
void CSLIB_enterLowPowerStateCB(void)
{
  EMU_EnterEM2(true);
}

/**************************************************************************//**
 * Configure timer for active mode
 *
 * This is a top-level call to configure the timer to active mode, one of the
 * two defined modes of operation in the system.  This instance of the function
 * configures the SmaRTClock to the frequency defined in cslib_config.h.
 *
 *****************************************************************************/
void CSLIB_configureTimerForActiveModeCB(void)
{
  // For CSEN, active mode timing is handled through the CSEN timer
  // and so it is configured in the configureSensor routine
}

/**************************************************************************//**
 * Configure timer for sleep mode
 *
 * This is a top-level call to configure the timer to sleep mode, one of the
 * two defined modes of operation in the system.  This instance of the function
 * configures the SmaRTClock to the frequency defined in cslib_config.h.
 *
 *****************************************************************************/
void CSLIB_configureTimerForSleepModeCB(void)
{
  // For CSEN, sleep mode timing is handled through the CSEN timer
  // and so it is configured in the configureSensor routine
}

/**************************************************************************//**
 * Check timer
 *
 *****************************************************************************/
void CSLIB_checkTimerCB(void)
{
  if (CSENtimerTick == 1) {
    CSENtimerTick = 0;
    timerTick = 1;
  }
}
