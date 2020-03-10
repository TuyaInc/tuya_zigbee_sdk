/**************************************************************************//**
 * Copyright 2016 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#include "retargetserial.h"

#include "em_device.h"
#include "em_chip.h"
#include "bsp_trace.h"
#include "em_device.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "device_init.h"
#include "cslib.h"

void systemInit(void)
{
  CHIP_Init();

  // Ensure core frequency has been updated
  SystemCoreClockUpdate();
}
