/***************************************************************************//**
 * @file
 * @brief stub implementation of antenna control HAL
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
#include "stack/include/ember-types.h"
#include "hal/hal.h"

#ifdef EMBER_STACK_CONNECT
#include "stack/include/error.h"
#endif

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
