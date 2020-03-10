/** @file hal/micro/cortexm3/efm32/sleep-efm32.c
 *
 * @brief EFM32/EFR32 specific sleep functions.
 *
 * <!-- Copyright 2016 Silicon Laboratories, Inc.                       *80* -->
 */

#include PLATFORM_HEADER
#include "sleep-efm32.h"
#include "hal/micro/micro.h"
#include "em_emu.h"
#include "em_gpio.h"
#include "serial/com.h"

#if defined(RTCC_PRESENT) && (RTCC_COUNT == 1)
#define SYSTIMER_IRQ_N   RTCC_IRQn
#else
#define SYSTIMER_IRQ_N   RTC_IRQn
#endif

static WakeEvents wakeInfo;
static Em4WakeupCause_t em4WakeupCause;

WakeEvents halGetWakeInfo(void)
{
  return wakeInfo;
}

const Em4WakeupCause_t * halInternalEm4WakeupCause(void)
{
  return &em4WakeupCause;
}

void halInternalEm4Wakeup(void)
{
  uint32_t gpioEm4Flags = 0;

#if defined(_GPIO_IF_EM4WU_MASK)
  gpioEm4Flags = GPIO_IntGet() & _GPIO_IF_EM4WU_MASK;
  GPIO_IntClear(gpioEm4Flags);
#elif defined(_GPIO_EM4WUCAUSE_MASK)
  gpioEm4Flags = GPIO->EM4WUCAUSE & _GPIO_EM4WUCAUSE_MASK;
#endif

  em4WakeupCause.gpioFlags = gpioEm4Flags;
}

/**
 * @brief Check if an interrupt is triggered
 *
 * @details This function checks if an interrupt is enabled and pending
 *
 * @param  [in] IRQn Interrupt number
 *
 * @return true   Interrupt is triggered
 *         false  Interrupt is not triggered
 */
static bool irqTriggered(IRQn_Type IRQn)
{
  return NVIC_GetPendingIRQ(IRQn) && NVIC_GetEnableIRQ(IRQn);
}

static void halInternalSleepHelper(SleepModes sleepMode, bool preserveIntState)
{
  bool watchdogDisableInSleep;

  // SLEEPMODE_POWERDOWN and SLEEPMODE_POWERSAVE are deprecated.  Remap them
  // to their appropriate, new mode name.
  if (sleepMode == SLEEPMODE_POWERDOWN) {
    sleepMode = SLEEPMODE_MAINTAINTIMER;
  } else if (sleepMode == SLEEPMODE_POWERSAVE) {
    sleepMode = SLEEPMODE_WAKETIMER;
  }

  // Disable and restore watchdog if already on and going for EM1 sleep,
  // since we can't clear it asleep in EM1. The watchdog is frozen in
  // EM2 and 3 and disabling it while in EM2 and 3 sleep is not needed.
  watchdogDisableInSleep = halInternalWatchDogEnabled()
                           && (sleepMode == SLEEPMODE_IDLE);

  if (watchdogDisableInSleep) {
    halInternalDisableWatchDog(MICRO_DISABLE_WATCH_DOG_KEY);
  }

  // BASEPRI is set to 0 in order to allow interrupts to wakeup the MCU
  // PRIMASK is set to 1 in order to block interrupt handler when MCU wakes up.
  uint32_t primask = __get_PRIMASK();
  __set_PRIMASK(1);
#if (__CORTEX_M >= 3)
  uint32_t basepri = __get_BASEPRI();
  __set_BASEPRI(0);
#endif

  COM_InternalPowerDown(sleepMode == SLEEPMODE_IDLE);
  wakeInfo = 0;

#ifdef BOOTLOADER_OPEN
  #error no bootloader support yet
#endif

  switch (sleepMode) {
    case SLEEPMODE_IDLE:
      EMU_EnterEM1();
      break;
    // there is no difference between wake/maintain timer
    case SLEEPMODE_WAKETIMER:
    case SLEEPMODE_MAINTAINTIMER:
      EMU_EnterEM2(true);
      break;
    case SLEEPMODE_NOTIMER:
      EMU_EnterEM3(true);
      break;
    case SLEEPMODE_HIBERNATE:
      EMU_EnterEM4();
      break;
    default:
      //Oops!  Invalid sleepMode parameter.
      assert(0);
      break;
  }

  wakeInfo = GPIO_IntGetEnabled() & _GPIO_IF_EXT_MASK;
  if (irqTriggered(GPIO_EVEN_IRQn) || irqTriggered(GPIO_ODD_IRQn)) {
    wakeInfo |= WAKE_IRQ_GPIO;
  }
  if (irqTriggered(SYSTIMER_IRQ_N)) {
    wakeInfo |= WAKE_IRQ_SYSTIMER;
  }
#if defined(RFSENSE_PRESENT) && (RFSENSE_COUNT == 1)
  if (irqTriggered(RFSENSE_IRQn)) {
    wakeInfo |= WAKE_IRQ_RFSENSE;
  }
#endif

  // restart watchdog if it was running when we entered sleep
  // do this before dispatching interrupts while we still have tight
  // control of code execution
  if (watchdogDisableInSleep) {
    halInternalEnableWatchDog();
  }

  COM_InternalPowerUp(sleepMode == SLEEPMODE_IDLE);

  if (preserveIntState) { // RTOS-friendly scheme to preserve interrupt state
    // Restore BASEPRI and PRIMASK to previous levels.
  #if (__CORTEX_M >= 3)
    __set_BASEPRI(basepri);
  #endif
    __set_PRIMASK(primask);
  } else { // emberHAL-compatible scheme enabling interrupts per API expectation
    // Clear PRIMASK to enable all interrupts. Note that after this
    // point BASEPRI=0 and PRIMASK=0 which means that all interrupts
    // are enabled. The interrupt state is not saved/restored due to
    // historical API restrictions.
    __set_PRIMASK(0);
  }
}

void halInternalSleep(SleepModes sleepMode)
{
  halInternalSleepHelper(sleepMode, false);
}

// Series 1 devices use _GPIO_IEN_EXT_MASK and series 2 devices use and
// identical mask renamed to _GPIO_IEN_EXTIEN_MASK.
#if defined (_SILICON_LABS_32B_SERIES_2) && !defined (_GPIO_IEN_EXT_MASK)
#define _GPIO_IEN_EXT_MASK _GPIO_IEN_EXTIEN_MASK
#endif

void halSleepWithOptions(SleepModes sleepMode, WakeMask wakeMask)
{
  uint32_t gpioIen = GPIO->IEN & _GPIO_IEN_EXT_MASK;
  GPIO->IEN = (GPIO->IEN & ~(_GPIO_IEN_EXT_MASK))
              | (wakeMask & _GPIO_IEN_EXT_MASK);
  halInternalSleep(sleepMode);
  GPIO->IEN = (GPIO->IEN & ~(_GPIO_IEN_EXT_MASK))
              | (gpioIen & _GPIO_IEN_EXT_MASK);
}

void halSleep(SleepModes sleepMode)
{
  halInternalSleep(sleepMode);
}

// RTOS-friendly APIs that restore interrupt state upon waking
void halSleepWithOptionsPreserveInts(SleepModes sleepMode, WakeMask wakeMask)
{
  uint32_t gpioIen = GPIO->IEN & _GPIO_IEN_EXT_MASK;
  GPIO->IEN = (GPIO->IEN & ~(_GPIO_IEN_EXT_MASK))
              | (wakeMask & _GPIO_IEN_EXT_MASK);
  halInternalSleepHelper(sleepMode, true);
  GPIO->IEN = (GPIO->IEN & ~(_GPIO_IEN_EXT_MASK))
              | (gpioIen & _GPIO_IEN_EXT_MASK);
  // N.B. If interrupts were disabled upon entry, any GPIOs in wakeMask
  // that were newly enabled (not in gpioIen) and caused the wakeup
  // will *not* trigger their ISR handler because interrupts remain
  // disabled here and those wakeMask GPIOs were just disabled by the
  // GPIO IEN restoration above.  They will, however, be represented
  // by WAKE_IRQ_GPIO in halGetWakeInfo().
}

void halSleepPreserveInts(SleepModes sleepMode)
{
  halInternalSleepHelper(sleepMode, true);
}
