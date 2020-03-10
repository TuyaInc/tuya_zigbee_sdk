/** @file rtos/generic/rtos-generic.c
 *
 * @brief Basic RTOS API wrapper that includes pieces which don't
 * really fit cleanly anywhere else.
 *
 * Copyright 2014 by Silicon Laboratories. All rights reserved.            *80*
 */

// Include the RTOS API header
#include "../rtos.h"

// Ember includes
#include PLATFORM_HEADER

// These initialization functions are defined in other rtos api files
// but we call them here so extern them.
extern void sleepManagerInit(void);
extern void threadLocalStorageInit(void);

// Call the various API initialization functions that we know about
void rtosApiInit(void)
{
  // Initialize the RTOS thread local storage
  threadLocalStorageInit();

  // Initialize the RTOS sleep manager
  sleepManagerInit();
}
