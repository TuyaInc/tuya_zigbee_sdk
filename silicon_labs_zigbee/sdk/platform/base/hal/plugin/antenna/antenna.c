/*
 * File: antenna.c
 * Description: HAL functions to control antenna mode
 *
 *
 * <!-- Copyright 2015 Silicon Laboratories, Inc.                        *80*-->
 */


#include PLATFORM_HEADER
#include "stack/include/ember.h"
#include "include/error.h"

#include "hal/hal.h"

#ifdef RTOS
  #include "rtos/rtos.h"
#endif

#ifdef DEBUG_ANT_DIV
extern uint32_t debugAntennaSelectGpio;
extern uint32_t debugAntennaNSelectGpio;
extern bool debugAntennaSelectEnabled;
extern bool debugAntennaNSelectEnabled;

#ifndef ANTENNA_SELECT_GPIO
#define ANTENNA_SELECT_GPIO  debugAntennaSelectGpio
#endif //!ANTENNA_SELECT_GPIO

#ifndef ANTENNA_nSELECT_GPIO
#define ANTENNA_nSELECT_GPIO debugAntennaNSelectGpio
#endif //!ANTENNA_nSELECT_GPIO

#define ANTENNA_SELECT_ENABLED  debugAntennaSelectEnabled
#define ANTENNA_nSELECT_ENABLED debugAntennaNSelectEnabled
#else  //!DEBUG_ANT_DIV

#define ANTENNA_SELECT_ENABLED  true
#define ANTENNA_nSELECT_ENABLED true
#endif //DEBUG_ANT_DIV

#ifdef ANTENNA_SELECT_GPIO

static HalAntennaMode antennaMode = HAL_ANTENNA_MODE_DEFAULT;
static uint8_t antennaSelection;

static void selectAntenna(uint8_t antenna)
{
  antennaSelection = antenna;
  if (ANTENNA_SELECT_ENABLED) {
    if (antennaSelection == 0) {
      halGpioSet(ANTENNA_SELECT_GPIO);
  #ifdef ANTENNA_nSELECT_GPIO
      if (ANTENNA_nSELECT_ENABLED) {
        halGpioClear(ANTENNA_nSELECT_GPIO);
      }
  #endif
    } else if (antennaSelection == 1) {
      halGpioClear(ANTENNA_SELECT_GPIO);
  #ifdef ANTENNA_nSELECT_GPIO
      if (ANTENNA_nSELECT_ENABLED) {
        halGpioSet(ANTENNA_nSELECT_GPIO);
      }
  #endif
    }
  }
}

HalAntennaMode halGetAntennaMode(void)
{
  return antennaMode;
}

EmberStatus halSetAntennaMode(HalAntennaMode mode)
{
  EmberStatus status = EMBER_SUCCESS;

  switch (mode) {
    case HAL_ANTENNA_MODE_ENABLE1:
      selectAntenna(0);
      break;
    case HAL_ANTENNA_MODE_ENABLE2:
      selectAntenna(1);
      break;
    case HAL_ANTENNA_MODE_DIVERSITY:
      break;
    default:
      status = EMBER_BAD_ARGUMENT;
      break;
  }

  if (status == EMBER_SUCCESS) {
    antennaMode = mode;
  }

  return status;
}

EmberStatus halToggleAntenna(void)
{
  if (antennaMode == HAL_ANTENNA_MODE_DIVERSITY) {
    selectAntenna(halGpioRead(ANTENNA_SELECT_GPIO));
    return EMBER_SUCCESS;
  }

  return EMBER_ERR_FATAL;
}

#else //ANTENNA_SELECT_GPIO
#ifndef JAMBUILD
#warning "ANTENNA_SELECT_GPIO is undefined.  Define ANTENNA_SELECT_GPIO to enable antenna diversity"
#endif//JAMBUILD

// Stubs in case someone insists on referencing them

HalAntennaMode halGetAntennaMode(void)
{
  return HAL_ANTENNA_MODE_ENABLE1;
}

EmberStatus halSetAntennaMode(HalAntennaMode mode)
{
  return ((mode == HAL_ANTENNA_MODE_ENABLE1) ? EMBER_SUCCESS : EMBER_BAD_ARGUMENT);
}

EmberStatus halToggleAntenna(void)
{
  return EMBER_ERR_FATAL;
}

#endif//ANTENNA_SELECT_GPIO
