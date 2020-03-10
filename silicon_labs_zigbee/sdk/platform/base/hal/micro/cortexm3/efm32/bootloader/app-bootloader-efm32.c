/***************************************************************************//**
 * @file app-bootloader-efm32.c
 * @brief Application bootloader
 * @version 3.20.2
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2016 Silicon Labs, www.silabs.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Labs has no
 * obligation to support this Software. Silicon Labs is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Silicon Labs will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 ******************************************************************************/
#include PLATFORM_HEADER
#include "bootloader-common.h"
#include "bootloader-serial.h"
#include "bootloader-gpio.h"
#include "app-bootloader.h"
#include "hal/micro/micro.h"
#include "hal.h"

#include "em_cmu.h"
#include "em_emu.h"
#ifdef BSP_STK
#include "bsp.h"
#endif

void bootloaderInitCustom(void)
{
#if BSP_DCDC_PRESENT
  EMU_DCDCInit_TypeDef dcdcInit = BSP_DCDC_INIT;
  #if HAL_DCDC_BYPASS
  dcdcInit.dcdcMode = emuDcdcMode_Bypass;
  #endif
  EMU_DCDCInit(&dcdcInit);
#else
  EMU_DCDCPowerOff();
#endif
  /* HFXO */
#if (HAL_CLK_HFCLK_SOURCE == HAL_CLK_HFCLK_SOURCE_HFXO)
  CMU_HFXOInit_TypeDef hfxoInit = BSP_CLK_HFXO_INIT;
  CMU_HFXOInit(&hfxoInit);

  /* Enable HFXO oscillator, and wait for it to be stable */
  CMU_OscillatorEnable(cmuOsc_HFXO, true, true);

  /* Setting system HFXO frequency */
  SystemHFXOClockSet(BSP_CLK_HFXO_FREQ);

  /* Using HFXO as high frequency clock, HFCLK */
  CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);

  /* HFRCO not needed when using HFXO */
  CMU_OscillatorEnable(cmuOsc_HFRCO, false, false);
#elif (HAL_CLK_HFCLK_SOURCE == HAL_CLK_HFCLK_SOURCE_HFRCO)
  CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFRCO);
#endif
}
