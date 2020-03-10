/***************************************************************************//**
 * @file btl_debug_swo.c
 * @brief SWO debug plugin for Silicon Labs Bootloader.
 * @author Silicon Labs
 * @version 1.7.0
 *******************************************************************************
 * @section License
 * <b>Copyright 2016 Silicon Laboratories, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/

#include "btl_debug.h"

#include "em_device.h"
#include "em_cmu.h"

void btl_debugInit(void)
{
  uint32_t tpiu_prescaler_val;

#if defined(_CMU_HFBUSCLKEN0_GPIO_MASK)
  CMU->HFBUSCLKEN0 |= CMU_HFBUSCLKEN0_GPIO;
#endif
#if defined(_CMU_HFPERCLKEN0_GPIO_MASK)
  CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_GPIO;
#endif

  // Enable Serial wire output pin
#if defined (_GPIO_ROUTE_MASK)
  GPIO->ROUTE |= GPIO_ROUTE_SWOPEN;
#endif
#if defined (_GPIO_ROUTEPEN_MASK)
  GPIO->ROUTEPEN |= GPIO_ROUTEPEN_SWVPEN;
#endif

  // TODO: Make pinout/location configurable
#if defined(_SILICON_LABS_GECKO_INTERNAL_SDID_71) || defined(_SILICON_LABS_GECKO_INTERNAL_SDID_73)
  // Set location 1
  GPIO->ROUTE = (GPIO->ROUTE & ~(_GPIO_ROUTE_SWLOCATION_MASK))
                | GPIO_ROUTE_SWLOCATION_LOC1;
  // Enable output on pin
  GPIO->P[2].MODEH &= ~(_GPIO_P_MODEH_MODE15_MASK);
  GPIO->P[2].MODEH |= GPIO_P_MODEH_MODE15_PUSHPULL;
  // Set TPIU prescaler to 16 (14 MHz / 16 = 875 kHz SWO speed)
  tpiu_prescaler_val = 16 - 1;
#elif defined(_SILICON_LABS_GECKO_INTERNAL_SDID_72) || defined(_SILICON_LABS_GECKO_INTERNAL_SDID_74)
  // Set location 0
  GPIO->ROUTE = (GPIO->ROUTE & ~(_GPIO_ROUTE_SWLOCATION_MASK))
                | GPIO_ROUTE_SWLOCATION_LOC0;
  // Enable output on pin
  GPIO->P[5].MODEL &= ~(_GPIO_P_MODEL_MODE2_MASK);
  GPIO->P[5].MODEL |= GPIO_P_MODEL_MODE2_PUSHPULL;
  // Set TPIU prescaler to 16 (14 MHz / 16 = 875 kHz SWO speed)
  tpiu_prescaler_val = 16 - 1;
#elif defined(_SILICON_LABS_32B_SERIES_1)
  // Set location 0
  GPIO->ROUTELOC0 = (GPIO->ROUTELOC0 & ~(_GPIO_ROUTELOC0_SWVLOC_MASK))
                    | GPIO_ROUTELOC0_SWVLOC_LOC0;
  // Enable output on pin
  GPIO->P[5].MODEL &= ~(_GPIO_P_MODEL_MODE2_MASK);
  GPIO->P[5].MODEL |= 4UL << 8u;

  // Set TPIU prescaler to 22 (19 MHz / 22 = 863.63 kHz SWO speed)
  tpiu_prescaler_val = 22 - 1;
#elif defined(_SILICON_LABS_32B_SERIES_2)

  /* Enable output on pin */
  GPIO->P[GPIO_SWV_PORT].MODEL &= ~(_GPIO_P_MODEL_MODE0_MASK << (GPIO_SWV_PIN * 4));
  GPIO->P[GPIO_SWV_PORT].MODEL |= _GPIO_P_MODEL_MODE0_PUSHPULL << (GPIO_SWV_PIN * 4);
  GPIO->TRACEROUTEPEN |= GPIO_TRACEROUTEPEN_SWVPEN;

  /* Select HFRCOEM23 as source for TRACECLK */
  CMU_ClockSelectSet(cmuClock_TRACECLK, cmuSelect_HFRCOEM23);
  /* Set TPIU prescaler to get a 863.63 kHz SWO speed */
  tpiu_prescaler_val = CMU_ClockFreqGet(cmuClock_TRACECLK) / 863630 - 1;

#else
#error Unknown device family!
#endif

#if (_SILICON_LABS_32B_SERIES < 2)
  // Enable debug clock AUXHFRCO
  CMU->OSCENCMD = CMU_OSCENCMD_AUXHFRCOEN;

  while ((CMU->STATUS & CMU_STATUS_AUXHFRCORDY) == 0UL) {
  }
#endif

  // Enable trace in core debug
  CoreDebug->DHCSR |= 1UL;
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

  // Enable PC and IRQ sampling output
  DWT->CTRL = 0x400113FFUL;

  // Set TPIU Prescaler
  TPI->ACPR = tpiu_prescaler_val;

  // Set protocol to NRZ
  TPI->SPPR = 2;

  // Disable continuous formatting
  TPI->FFCR = 0x100;

  // Unlock ITM and output data
  ITM->LAR = 0xC5ACCE55UL;
  ITM->TCR = 0x10009UL;

  // ITM Channel 0 is used for UART output
  ITM->TER |= (1UL << 0);
}

void btl_debugWriteChar(char c)
{
  (void) ITM_SendChar((uint32_t)c);
}
