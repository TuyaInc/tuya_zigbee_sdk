/**************************************************************************//**
 * @file coexistence_hal.h
 * @brief This file contains the EFR32 radio coexistence interface.
 * @copyright Copyright 2017 Silicon Laboratories, Inc. www.silabs.com
 *****************************************************************************/

#ifndef __COEXISTENCE_HAL_H__
#define __COEXISTENCE_HAL_H__

#ifdef HAL_CONFIG
  #include "hal-config.h"
#endif // HAL_CONFIG
#include "coexistence/common/coexistence.h"
#include "em_gpio.h"
#include "gpiointerrupt.h"

typedef struct COEX_HAL_GpioConfig {
  /** GPIO port */
  uint8_t port;

  /** GPIO pin */
  uint8_t pin;

  /** GPIO assert polarity */
  bool polarity;

  /** GPIO PWM enabled */
  bool pwmEnabled;

  /** GPIO mode */
  uint8_t mode;

  /** GPIO ISR */
  GPIOINT_IrqCallbackPtr_t isr;

  /** GPIO config */
  COEX_GpioConfig_t config;
} COEX_HAL_GpioConfig_t;

void COEX_HAL_Init(void);
bool COEX_HAL_ConfigRequest(COEX_HAL_GpioConfig_t *gpioConfig);
bool COEX_HAL_ConfigPwmRequest(COEX_HAL_GpioConfig_t *gpioConfig);
bool COEX_HAL_ConfigRadioHoldOff(COEX_HAL_GpioConfig_t *gpioConfig);
bool COEX_HAL_ConfigPriority(COEX_HAL_GpioConfig_t *gpioConfig);
bool COEX_HAL_ConfigGrant(COEX_HAL_GpioConfig_t *gpioConfig);

#if defined(BSP_COEX_REQ_PORT) && defined(BSP_COEX_PRI_PORT) && defined(PER_BITSET_MEM_BASE)
#define COEX_HAL_FAST_REQUEST 1
#define COEX_HAL_ReadGpio(port,                  \
                          pin,                   \
                          polarity)              \
  (!!GPIO_PinInGet((GPIO_Port_TypeDef)port, pin) \
   == !!polarity)

#define COEX_HAL_SetGpio(port,                                         \
                         pin,                                          \
                         polarity)                                     \
  do {                                                                 \
    uint32_t address = (uint32_t)&GPIO->P[port].DOUT - PER_MEM_BASE;   \
    address += (polarity ? PER_BITSET_MEM_BASE : PER_BITCLR_MEM_BASE); \
    *((volatile uint32_t *)address) = 1U << pin;                       \
  } while (0)

#define COEX_HAL_ReadRequest()         \
  COEX_HAL_ReadGpio(BSP_COEX_REQ_PORT, \
                    BSP_COEX_REQ_PIN,  \
                    BSP_COEX_REQ_ASSERT_LEVEL)
#define COEX_HAL_SetRequest()         \
  COEX_HAL_SetGpio(BSP_COEX_REQ_PORT, \
                   BSP_COEX_REQ_PIN,  \
                   BSP_COEX_REQ_ASSERT_LEVEL)
#define COEX_HAL_ClearRequest()       \
  COEX_HAL_SetGpio(BSP_COEX_REQ_PORT, \
                   BSP_COEX_REQ_PIN,  \
                   !BSP_COEX_REQ_ASSERT_LEVEL)
#define COEX_HAL_SetPriority()        \
  COEX_HAL_SetGpio(BSP_COEX_PRI_PORT, \
                   BSP_COEX_PRI_PIN,  \
                   BSP_COEX_PRI_ASSERT_LEVEL)
#define COEX_HAL_ClearPriority()      \
  COEX_HAL_SetGpio(BSP_COEX_PRI_PORT, \
                   BSP_COEX_PRI_PIN,  \
                   !BSP_COEX_PRI_ASSERT_LEVEL)

#endif //defined(BSP_COEX_REQ_PORT) && defined(BSP_COEX_PRI_PORT) && defined(PER_BITSET_MEM_BASE)
#endif  // __COEXISTENCE_HAL_H__
