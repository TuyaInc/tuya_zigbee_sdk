/***************************************************************************//**
 * @file
 * @brief HAL functions to control antenna mode
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/
#include PLATFORM_HEADER
#include "stack/include/ember.h"
#include "include/error.h"

#include "hal/hal.h" // includes antenna.h

#ifdef RTOS
  #include "rtos/rtos.h"
#endif

#ifdef DEBUG_ANT_DIV

extern uint32_t debugAntennaSelectGpio;
extern uint32_t debugAntennaNSelectGpio;
extern bool debugAntennaSelectEnabled;
extern bool debugAntennaNSelectEnabled;
extern uint8_t debugAntennaSelectLoc;
extern uint8_t debugAntennaNSelectLoc;

#ifndef ANTENNA_SELECT_GPIO
  #define ANTENNA_SELECT_GPIO  debugAntennaSelectGpio
  #define ANTENNA_SELECT_LOC   debugAntennaSelectLoc
#endif //!ANTENNA_SELECT_GPIO

#ifndef ANTENNA_nSELECT_GPIO
  #define ANTENNA_nSELECT_GPIO debugAntennaNSelectGpio
  #define ANTENNA_nSELECT_LOC  debugAntennaNSelectLoc
#endif //!ANTENNA_nSELECT_GPIO

#define ANTENNA_SELECT_ENABLED  debugAntennaSelectEnabled
#define ANTENNA_nSELECT_ENABLED debugAntennaNSelectEnabled

#else  //!DEBUG_ANT_DIV

// Flag any mis-configuration
#if     ANTENNA_USE_RAIL_SCHEME
  #if     (defined(ANTENNA_SELECT_GPIO) && !defined(BSP_ANTDIV_SEL_LOC))
    #error "Antenna Diversity needs BSP_ANTDIV_SEL_LOC specified"
  #endif//(defined(ANTENNA_SELECT_GPIO) && !defined(BSP_ANTDIV_SEL_LOC))
  #if     (defined(ANTENNA_nSELECT_GPIO) && !defined(BSP_ANTDIV_NSEL_LOC))
    #error "Antenna Diversity needs BSP_ANTDIV_NSEL_LOC specified"
  #endif//(defined(ANTENNA_nSELECT_GPIO) && !defined(BSP_ANTDIV_NSEL_LOC))
#else//!ANTENNA_USE_RAIL_SCHEME
  #if     (ANTENNA_RX_DEFAULT_MODE != HAL_ANTENNA_MODE_DISABLED)
    #if (PHY_RAIL || PHY_DUALRAIL) && !defined(_SILICON_LABS_32B_SERIES_1_CONFIG_1)
      #error "Rx Antenna Diversity requires BSP_ANTDIV_[N]SEL_PORT/PIN/LOC to be defined."
    #else
      #error "Rx Antenna Diversity is not supported on this platform or PHY."
    #endif
  #endif//(ANTENNA_RX_DEFAULT_MODE != HAL_ANTENNA_MODE_DISABLED)
#endif//ANTENNA_USE_RAIL_SCHEME

#define ANTENNA_SELECT_ENABLED  true
#define ANTENNA_SELECT_LOC      BSP_ANTDIV_SEL_LOC
#define ANTENNA_nSELECT_ENABLED true
#define ANTENNA_nSELECT_LOC     BSP_ANTDIV_NSEL_LOC

#endif //DEBUG_ANT_DIV

#if     (defined(ANTENNA_SELECT_GPIO) || defined(ANTENNA_nSELECT_GPIO))

static HalAntennaMode txAntennaMode = ANTENNA_TX_DEFAULT_MODE;
// Default to first antenna
static HalAntennaSelection txAntennaSelection = HAL_ANTENNA_SELECT_ANTENNA1;
static HalAntennaMode rxAntennaMode = ANTENNA_RX_DEFAULT_MODE;

#if     ANTENNA_USE_RAIL_SCHEME

// RAIL Antenna Diversity

#include "rail.h"
#include "rail_ieee802154.h"
#include "rail_chip_specific.h"

RAIL_Status_t halAntennaConfigRailAntenna(RAIL_Handle_t railHandle)
{
  // Tell RAIL to configure the antenna GPIOs
  RAIL_AntennaConfig_t antennaConfig = { false, }; // Zero out structure
 #ifdef  ANTENNA_SELECT_GPIO
  if (ANTENNA_SELECT_ENABLED) {
    antennaConfig.ant0PinEn = true;
    antennaConfig.ant0Port = GPIO_PORT(ANTENNA_SELECT_GPIO);
    antennaConfig.ant0Pin = GPIO_PIN(ANTENNA_SELECT_GPIO);
    antennaConfig.ant0Loc = ANTENNA_SELECT_LOC;
  }
 #endif//ANTENNA_SELECT_GPIO
 #ifdef  ANTENNA_nSELECT_GPIO
  if (ANTENNA_nSELECT_ENABLED) {
    antennaConfig.ant1PinEn = true;
    antennaConfig.ant1Port = GPIO_PORT(ANTENNA_nSELECT_GPIO);
    antennaConfig.ant1Pin = GPIO_PIN(ANTENNA_nSELECT_GPIO);
    antennaConfig.ant1Loc = ANTENNA_nSELECT_LOC;
  }
 #endif//ANTENNA_nSELECT_GPIO
  return RAIL_ConfigAntenna(railHandle, &antennaConfig);
}

#endif//ANTENNA_USE_RAIL_SCHEME

// Rx Antenna Diversity
extern EmberStatus emRadioConfigRxAntenna(HalAntennaMode mode);

HalAntennaMode halGetAntennaRxMode(void)
{
  return rxAntennaMode;
}

EmberStatus halSetAntennaRxMode(HalAntennaMode mode)
{
 #if     (ANTENNA_RX_DEFAULT_MODE != HAL_ANTENNA_MODE_DISABLED)
  // Tell RAIL what Rx antenna mode to use, if we can
  EmberStatus status = emRadioConfigRxAntenna(mode);
  if (status == EMBER_SUCCESS) {
    rxAntennaMode = mode;
  }
  return status;
 #else//!(ANTENNA_RX_DEFAULT_MODE != HAL_ANTENNA_MODE_DISABLED)
  return ((mode == HAL_ANTENNA_MODE_DISABLED) ? EMBER_SUCCESS : EMBER_BAD_ARGUMENT);
 #endif//(ANTENNA_RX_DEFAULT_MODE != HAL_ANTENNA_MODE_DISABLED)
}

// Tx Antenna Diversity

static void selectTxAntenna(HalAntennaSelection txAntenna)
{
  txAntennaSelection = txAntenna;
 #if     ANTENNA_USE_RAIL_SCHEME
  // PHY does selection when a transmit is begun
 #else//!ANTENNA_USE_RAIL_SCHEME
  if (txAntenna == HAL_ANTENNA_SELECT_ANTENNA1) {
   #ifdef  ANTENNA_nSELECT_GPIO
    if (ANTENNA_nSELECT_ENABLED) {
      halGpioClear(ANTENNA_nSELECT_GPIO);
    }
   #endif//ANTENNA_nSELECT_GPIO
   #ifdef  ANTENNA_SELECT_GPIO
    if (ANTENNA_SELECT_ENABLED) {
      halGpioSet(ANTENNA_SELECT_GPIO);
    }
   #endif//ANTENNA_SELECT_GPIO
  } else { // (txAntenna == HAL_ANTENNA_SELECT_ANTENNA2)
   #ifdef  ANTENNA_SELECT_GPIO
    if (ANTENNA_SELECT_ENABLED) {
      halGpioClear(ANTENNA_SELECT_GPIO);
    }
   #endif//ANTENNA_SELECT_GPIO
   #ifdef  ANTENNA_nSELECT_GPIO
    if (ANTENNA_nSELECT_ENABLED) {
      halGpioSet(ANTENNA_nSELECT_GPIO);
    }
   #endif//ANTENNA_nSELECT_GPIO
  }
 #endif//ANTENNA_USE_RAIL_SCHEME
}

HalAntennaSelection halGetAntennaSelected(void)
{
  return txAntennaSelection;
}

HalAntennaMode halGetAntennaMode(void)
{
  return txAntennaMode;
}

EmberStatus halSetAntennaMode(HalAntennaMode mode)
{
  EmberStatus status = EMBER_SUCCESS;

  switch (mode) {
    case HAL_ANTENNA_MODE_DISABLED: // Leave antenna untouched (e.g. from Rx)
      break;
    case HAL_ANTENNA_MODE_ENABLE1:
      selectTxAntenna(HAL_ANTENNA_SELECT_ANTENNA1);
      break;
    case HAL_ANTENNA_MODE_ENABLE2:
      selectTxAntenna(HAL_ANTENNA_SELECT_ANTENNA2);
      break;
    case HAL_ANTENNA_MODE_DIVERSITY: // Ensure current selection is effected
      selectTxAntenna(txAntennaSelection);
      break;
    default:
      status = EMBER_BAD_ARGUMENT;
      break;
  }

  if (status == EMBER_SUCCESS) {
    txAntennaMode = mode;
  }

  return status;
}

EmberStatus halToggleAntenna(void)
{
  if (txAntennaMode == HAL_ANTENNA_MODE_DIVERSITY) {
    selectTxAntenna(txAntennaSelection ^ 1);
    return EMBER_SUCCESS;
  }
  return EMBER_ERR_FATAL;
}

EmberStatus halSetAntenna(HalAntennaSelection txAntenna)
{
  if (txAntenna == txAntennaSelection) { // Already selected!
    return EMBER_SUCCESS;
  }
  return halToggleAntenna();
}

#else//!(defined(ANTENNA_SELECT_GPIO) || defined(ANTENNA_nSELECT_GPIO))

#ifndef JAMBUILD
#warning "Define either ANTENNA_SELECT_GPIO or ANTENNA_nSELECT_GPIO to enable antenna diversity"
#endif//JAMBUILD

// Stubs in case someone insists on referencing them

HalAntennaMode halGetAntennaRxMode(void)
{
  return HAL_ANTENNA_MODE_DISABLED;
}

EmberStatus halSetAntennaRxMode(HalAntennaMode mode)
{
  return ((mode == HAL_ANTENNA_MODE_DISABLED) ? EMBER_SUCCESS : EMBER_BAD_ARGUMENT);
}

HalAntennaSelection halGetAntennaSelected(void)
{
  return HAL_ANTENNA_SELECT_ANTENNA1;
}

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

EmberStatus halSetAntenna(HalAntennaSelection txAntenna)
{
  return ((txAntenna == HAL_ANTENNA_SELECT_ANTENNA1)
          ? EMBER_SUCCESS : EMBER_ERR_FATAL);
}

#endif//(defined(ANTENNA_SELECT_GPIO) || defined(ANTENNA_nSELECT_GPIO))
