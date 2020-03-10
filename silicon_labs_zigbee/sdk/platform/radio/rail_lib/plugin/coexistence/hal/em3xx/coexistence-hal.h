/**************************************************************************//**
 * @file coexistence_hal.h
 * @brief This file contains the EM3XX radio coexistence interface.
 * @copyright Copyright 2017 Silicon Laboratories, Inc. www.silabs.com
 *****************************************************************************/

#ifndef __COEXISTENCE_HAL_H__
#define __COEXISTENCE_HAL_H__

#include PLATFORM_HEADER
#include "stack/include/ember.h"
#include "stack/include/ember-types.h"
#include "include/error.h"

#include "hal/hal.h"

#ifdef RTOS
  #include "rtos/rtos.h"
#endif

#include "coexistence/common/coexistence.h"

typedef void (*HalGpioIsr_t)(void);
typedef void (*HalGpioIntSel_t)(void);

typedef struct COEX_HAL_GpioConfig {
  /** GPIO port and pin */
  uint32_t gpio;

  /** GPIO assert polarity */
  bool polarity;

  /** GPIO mode */
  uint32_t mode;

  /** GPIO ISR */
  HalGpioIsr_t isr;

  /** GPIO interrupt enable bit */
  uint32_t intEnBit;

  /** GPIO EVENT_MISS bit */
  uint32_t intMissBit;

  /** GPIO interrupt flag bit */
  uint32_t flagBit;

  /** GPIO interrupt config */
  volatile uint32_t *intCfg;

  /** GPIO interrupt select */
  HalGpioIntSel_t intSel;

  /** GPIO interrupt triggers */
  uint32_t interruptEdges;

  /** GPIO config */
  COEX_GpioConfig_t config;
} COEX_HAL_GpioConfig_t;

void COEX_HAL_Init(void);
bool COEX_HAL_ConfigRequest(COEX_HAL_GpioConfig_t *gpioConfig);
bool COEX_HAL_ConfigRadioHoldOff(COEX_HAL_GpioConfig_t *gpioConfig);
bool COEX_HAL_ConfigPriority(COEX_HAL_GpioConfig_t *gpioConfig);
bool COEX_HAL_ConfigGrant(COEX_HAL_GpioConfig_t *gpioConfig);

#endif  // __COEXISTENCE_HAL_H__
