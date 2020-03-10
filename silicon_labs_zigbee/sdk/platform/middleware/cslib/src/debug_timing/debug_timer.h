/**************************************************************************//**
 * Copyright 2016 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#ifndef DEBUG_TIMER_H
#define DEBUG_TIMER_H
void debugTimer_setup(void);
void debugTimer_start();
uint32_t debugTimer_getTime();
void debugTimer_stop();

#endif // DEBUG_TIMER_H
