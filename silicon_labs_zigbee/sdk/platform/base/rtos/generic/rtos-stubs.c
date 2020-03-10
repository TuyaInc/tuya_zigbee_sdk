/** @file rtos/generic/rtos-stubs.c
 *
 * @brief This file implements functions required by the RTOS that are
 * expected to be replaced with customer specific versions. In a final
 * customer application this should be removed and replaced with your
 * own version.
 *
 * Copyright 2014 by Silicon Laboratories. All rights reserved.            *80*
 */

// Declaration for the standard watch dog API
void halInternalResetWatchDog(void);

// Simple watchdog reset that just calls the regular HAL function. Most
// people will want to implement a more thread aware watchdog that makes
// sure all threads are running and checking in and not just one thread
// constantly resetting the watchdog.
void rtosResetWatchdog(void)
{
  halInternalResetWatchDog();
}
