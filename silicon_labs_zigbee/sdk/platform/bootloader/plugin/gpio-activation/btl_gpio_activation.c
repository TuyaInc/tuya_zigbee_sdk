/***************************************************************************//**
 * @file btl_gpio_activation.c
 * @brief GPIO Activation plugin for Silicon Labs bootloader.
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

#include "config/btl_config.h"

#include "em_device.h"
#include "em_gpio.h"
#include "btl_gpio_activation.h"

// Map GPIO activation polarity settings to GPIO pin states
#define HIGH 0
#define LOW  1

bool gpio_enterBootloader(void)
{
  bool pressed;

#if defined(CMU_HFBUSCLKEN0_GPIO)
  // Enable GPIO clock
  CMU->HFBUSCLKEN0 |= CMU_HFBUSCLKEN0_GPIO;
#endif

  // Since the button may have decoupling caps, they may not be charged
  // after a power-on and could give a false positive result. To avoid
  // this issue, drive the output as an output for a short time to charge
  // them up as quickly as possible.
  GPIO_PinModeSet(BSP_BTL_BUTTON_PORT,
                  BSP_BTL_BUTTON_PIN,
                  gpioModePushPull,
                  BTL_GPIO_ACTIVATION_POLARITY);
  for (volatile int i = 0; i < 100; i++) {
    // Do nothing
  }

  // Reconfigure as an input with pull(up|down) to read the button state
  GPIO_PinModeSet(BSP_BTL_BUTTON_PORT,
                  BSP_BTL_BUTTON_PIN,
                  gpioModeInputPull,
                  BTL_GPIO_ACTIVATION_POLARITY);

  // We have to delay again here so that if the button is depressed the
  // cap has time to discharge again after being charged up by the above delay
  for (volatile int i = 0; i < 500; i++) {
    // Do nothing
  }

  pressed = GPIO_PinInGet(BSP_BTL_BUTTON_PORT, BSP_BTL_BUTTON_PIN)
            != BTL_GPIO_ACTIVATION_POLARITY;

  // Disable GPIO pin
  GPIO_PinModeSet(BSP_BTL_BUTTON_PORT,
                  BSP_BTL_BUTTON_PIN,
                  gpioModeDisabled,
                  BTL_GPIO_ACTIVATION_POLARITY);

#if defined(CMU_HFBUSCLKEN0_GPIO)
  // Disable GPIO clock
  CMU->HFBUSCLKEN0 &= ~CMU_HFBUSCLKEN0_GPIO;
#endif

  return pressed;
}
