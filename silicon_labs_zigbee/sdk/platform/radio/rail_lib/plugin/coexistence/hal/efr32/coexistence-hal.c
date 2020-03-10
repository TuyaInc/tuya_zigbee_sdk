// -----------------------------------------------------------------------------
/// @file
/// @brief Radio coexistence EFR32 utilities
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
#include "em_core.h"
#include "em_cmu.h"
#include "coexistence-hal.h"

#ifdef BSP_COEX_GNT_PORT
static COEX_HAL_GpioConfig_t ptaGntCfg = {
  .port = BSP_COEX_GNT_PORT,
  .pin = BSP_COEX_GNT_PIN,
  .polarity = BSP_COEX_GNT_ASSERT_LEVEL
};
#endif //BSP_COEX_GNT_PORT

#ifdef BSP_COEX_PRI_PORT
static COEX_HAL_GpioConfig_t ptaPriCfg = {
  .port = BSP_COEX_PRI_PORT,
  .pin = BSP_COEX_PRI_PIN,
  .polarity = BSP_COEX_PRI_ASSERT_LEVEL
};
#endif //BSP_COEX_PRI_PORT

#ifdef BSP_COEX_REQ_PORT
static COEX_HAL_GpioConfig_t ptaReqCfg = {
  .port = BSP_COEX_REQ_PORT,
  .pin = BSP_COEX_REQ_PIN,
  .polarity = BSP_COEX_REQ_ASSERT_LEVEL
};
#endif //BSP_COEX_REQ_PORT

#ifdef BSP_COEX_PWM_REQ_PORT
static COEX_HAL_GpioConfig_t ptaPwmReqCfg = {
  .port = BSP_COEX_PWM_REQ_PORT,
  .pin = BSP_COEX_PWM_REQ_PIN,
  .polarity = BSP_COEX_PWM_REQ_ASSERT_LEVEL
};
#endif //BSP_COEX_PWM_REQ_PORT

#ifdef BSP_COEX_RHO_PORT
static COEX_HAL_GpioConfig_t rhoCfg = {
  .port = BSP_COEX_RHO_PORT,
  .pin = BSP_COEX_RHO_PIN,
  .polarity = BSP_COEX_RHO_ASSERT_LEVEL
};
#endif //BSP_COEX_RHO_PORT

#define GPIO_FLAG(x) (1ul << x)

static void (*reqCallback)(void) = NULL;
static void (*gntCallback)(void) = NULL;
static void (*rhoCallback)(void) = NULL;

static void COEX_HAL_REQ_ISR(uint8_t pin)
{
  (void)pin;
  reqCallback();
}

static void COEX_HAL_GNT_ISR(uint8_t pin)
{
  (void)pin;
  gntCallback();
}

static void COEX_HAL_RHO_ISR(uint8_t pin)
{
  (void)pin;
  rhoCallback();
}

static void setGpioConfig(COEX_GpioHandle_t gpioHandle)
{
  if (gpioHandle != NULL) {
    COEX_HAL_GpioConfig_t *gpio = (COEX_HAL_GpioConfig_t*)gpioHandle;

    GPIO_PinModeSet((GPIO_Port_TypeDef)gpio->port,
                    gpio->pin,
                    (GPIO_Mode_TypeDef)gpio->mode,
                    GPIO_PinOutGet((GPIO_Port_TypeDef)gpio->port,
                                   gpio->pin));
  }
}

static bool isGpioInSet(COEX_GpioHandle_t gpioHandle, bool defaultValue);

static void enableGpioInt(COEX_GpioHandle_t gpioHandle,
                          bool enabled,
                          bool *wasAsserted)
{
  if (gpioHandle != NULL) {
    COEX_HAL_GpioConfig_t *gpio = (COEX_HAL_GpioConfig_t*)gpioHandle;
    COEX_GpioConfig_t *coexGpio = &(gpio->config);

    bool intAsserted = (coexGpio->options & COEX_GPIO_OPTION_INT_ASSERTED) != 0U;
    bool intDeasserted = (coexGpio->options & COEX_GPIO_OPTION_INT_DEASSERTED) != 0U;

    if (enabled) {
      // Disable triggering and clear any stale events
      GPIO_IntConfig((GPIO_Port_TypeDef)gpio->port, gpio->pin,
                     false, false, false);
      if (wasAsserted != NULL) {
        *wasAsserted = false; // Ensures we won't miss GNT assertion
      }
      // Register callbacks before setting up and enabling pin interrupt
      GPIOINT_CallbackRegister(gpio->pin, gpio->isr);
      // Enable both edges' interrupt
      GPIO_IntConfig((GPIO_Port_TypeDef)gpio->port,
                     gpio->pin,
                     gpio->polarity ? intAsserted : intDeasserted,
                     gpio->polarity ? intDeasserted : intAsserted,
                     true);
    } else {
      GPIO_IntDisable(GPIO_FLAG(gpio->pin));
      GPIO_IntClear(GPIO_FLAG(gpio->pin));
    }
  }
}

static void setGpio(COEX_GpioHandle_t gpioHandle, bool enabled)
{
  if (gpioHandle != NULL) {
    COEX_HAL_GpioConfig_t *gpio = (COEX_HAL_GpioConfig_t*)gpioHandle;

    if (enabled == gpio->polarity) {
      GPIO_PinOutSet((GPIO_Port_TypeDef)gpio->port, gpio->pin);
    } else {
      GPIO_PinOutClear((GPIO_Port_TypeDef)gpio->port, gpio->pin);
    }
  }
}

static void configGpio(COEX_GpioHandle_t gpioHandle, COEX_GpioConfig_t *coexGpio)
{
  if (gpioHandle != NULL) {
    COEX_HAL_GpioConfig_t *gpio = (COEX_HAL_GpioConfig_t*)gpioHandle;
    bool defaultAsserted = (coexGpio->options & COEX_GPIO_OPTION_DEFAULT_ASSERTED) != 0U;
    gpio->config = *coexGpio;

    if ((coexGpio->options & COEX_GPIO_OPTION_SHARED) != 0U) {
      gpio->mode = gpio->polarity ? gpioModeWiredOr : gpioModeWiredAnd;
    } else if ((coexGpio->options & COEX_GPIO_OPTION_OUTPUT) != 0U) {
      gpio->mode = gpioModePushPull;
    } else {
      gpio->mode = gpioModeInputPull;
    }
    setGpioConfig(gpio);
    setGpio(gpio, defaultAsserted);
  }
}

static void setGpioFlag(COEX_GpioHandle_t gpioHandle, bool enabled)
{
  if (gpioHandle != NULL) {
    COEX_HAL_GpioConfig_t *gpio = (COEX_HAL_GpioConfig_t*)gpioHandle;

    if (enabled) {
      GPIO_IntSet(GPIO_FLAG(gpio->pin));
    } else {
      GPIO_IntClear(GPIO_FLAG(gpio->pin));
    }
  }
}

static bool isGpioOutSet(COEX_GpioHandle_t gpioHandle, bool defaultValue)
{
  if (gpioHandle != NULL) {
    COEX_HAL_GpioConfig_t *gpio = (COEX_HAL_GpioConfig_t*)gpioHandle;
    return !!GPIO_PinOutGet((GPIO_Port_TypeDef)gpio->port,
                            gpio->pin) == !!gpio->polarity;
  } else {
    return defaultValue;
  }
}

static bool isGpioInSet(COEX_GpioHandle_t gpioHandle, bool defaultValue)
{
  if (gpioHandle != NULL) {
    COEX_HAL_GpioConfig_t *gpio = (COEX_HAL_GpioConfig_t*)gpioHandle;
    return !!GPIO_PinInGet((GPIO_Port_TypeDef)gpio->port,
                           gpio->pin) == !!gpio->polarity;
  } else {
    return defaultValue;
  }
}

static const COEX_HalCallbacks_t coexHalCallbacks = {
  .setGpio = &setGpio,
  .setGpioFlag = &setGpioFlag,
  .enableGpioInt = &enableGpioInt,
  .configGpio = &configGpio,
  .isGpioOutSet = &isGpioOutSet,
  .isGpioInSet = &isGpioInSet
};

static void GPIOINT_InitSafe(void)
{
  // Enable GPIO clock for configuring interrupts
  CMU_ClockEnable(cmuClock_GPIO, true);

  // Turn on GPIO interrupts only if they weren't enabled elsewhere
  if (CORE_NvicIRQDisabled(GPIO_ODD_IRQn)
      || CORE_NvicIRQDisabled(GPIO_EVEN_IRQn)) {
    GPIOINT_Init();
  }
}

void COEX_HAL_CallAtomic(COEX_AtomicCallback_t cb, void *arg)
{
  CORE_CRITICAL_SECTION((*cb)(arg); )
}

bool COEX_HAL_ConfigRequest(COEX_HAL_GpioConfig_t *gpioConfig)
{
  bool status = false;

  gpioConfig->isr = &COEX_HAL_REQ_ISR;
  status = COEX_ConfigRequest(gpioConfig);
  if (status) {
    reqCallback = gpioConfig->config.cb;
  }
  return status;
}

bool COEX_HAL_ConfigRadioHoldOff(COEX_HAL_GpioConfig_t *gpioConfig)
{
  bool status = false;

  gpioConfig->isr = &COEX_HAL_RHO_ISR;
  status = COEX_ConfigRadioHoldOff(gpioConfig);
  if (status) {
    rhoCallback = gpioConfig->config.cb;
  }
  return status;
}

bool COEX_HAL_ConfigPriority(COEX_HAL_GpioConfig_t *gpioConfig)
{
  return COEX_ConfigPriority(gpioConfig);
}

bool COEX_HAL_ConfigGrant(COEX_HAL_GpioConfig_t *gpioConfig)
{
  bool status = false;

  gpioConfig->isr = &COEX_HAL_GNT_ISR;
  status = COEX_ConfigGrant(gpioConfig);
  if (status) {
    gntCallback = gpioConfig->config.cb;
  }
  return status;
}

bool COEX_HAL_ConfigPwmRequest(COEX_HAL_GpioConfig_t *gpioConfig)
{
  return COEX_ConfigPwmRequest(gpioConfig);
}

void COEX_HAL_Init(void)
{
  COEX_SetHalCallbacks(&coexHalCallbacks);
  COEX_InitHalConfigOptions();
  GPIOINT_InitSafe();

  #ifdef BSP_COEX_REQ_PORT
  COEX_HAL_ConfigRequest(&ptaReqCfg);
  #endif //BSP_COEX_REQ_PORT
  #ifdef BSP_COEX_PRI_PORT
  COEX_HAL_ConfigPriority(&ptaPriCfg);
  #endif //BSP_COEX_PRI_PORT
  #ifdef BSP_COEX_PWM_REQ_PORT
  COEX_HAL_ConfigPwmRequest(&ptaPwmReqCfg);
  #endif //BSP_COEX_PWM_REQ_PORT
  #ifdef BSP_COEX_GNT_PORT
  COEX_HAL_ConfigGrant(&ptaGntCfg);
  #endif //BSP_COEX_GNT_PORT
  #ifdef BSP_COEX_RHO_PORT
  COEX_HAL_ConfigRadioHoldOff(&rhoCfg);
  #endif //BSP_COEX_RHO_PORT
}
