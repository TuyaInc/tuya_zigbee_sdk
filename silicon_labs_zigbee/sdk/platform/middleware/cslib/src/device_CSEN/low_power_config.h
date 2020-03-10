/**************************************************************************//**
 * Copyright 2016 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#ifndef LOW_POWER_CONFIG_H
#define LOW_POWER_CONFIG_H

// Functions which must be defined with implementation-specific
// responsibilities.  These are called by LowPowerRoutines.c
void CSLIB_configureSensorForSleepModeCB(void);
void CSLIB_configureTimerForSleepModeCB(void);
void CSLIB_configureTimerForActiveModeCB(void);
void CSLIB_enterLowPowerStateCB(void);
void CSLIB_checkTimerCB(void);

#endif // LOW_POWER_CONFIG_H
