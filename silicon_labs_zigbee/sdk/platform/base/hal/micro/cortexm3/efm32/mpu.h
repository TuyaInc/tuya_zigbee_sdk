/***************************************************************************//**
 * @file
 * @brief
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/
#ifndef __MPU_H__
#define __MPU_H__

#include "hal/micro/micro.h"

#if defined(__MPU_PRESENT) && (__MPU_PRESENT == 1U) && (__CORTEX_M <= 7U)

// Number of MPU regions on the Cortex M3
#define NUM_MPU_REGIONS 5

#define FLASH_REGION    (0x00000000)
#define PERIPH_REGION   (0x40000000)
#define USERPER_REGION  (0x40000000)
#define SRAM_REGION     (0x20000000)
#define GUARD_REGION    (0x20000000)
#define SPARE0_REGION   (0x20000000)
#define SPARE1_REGION   (0x20000000)
#define SPARE2_REGION   (0x20000000)

//A simple utility macro for temporarily turning off the MPU.  Turning off
//the MPU is dangerous and should only be done in critical situations, such
//as executing from RAM and writing Flash.
#ifdef _HAL_MPU_UNUSED_  // bootloaders and min hal don't use MPU
  #define BYPASS_MPU(blah) blah
#else
  #define BYPASS_MPU(blah)                                 \
  {                                                        \
    uint32_t mpuEnabled = MPU->CTRL & MPU_CTRL_ENABLE_Msk; \
    if (mpuEnabled) {                                      \
      halInternalDisableMPU();                             \
    }                                                      \
    { blah }                                               \
    if (mpuEnabled) {                                      \
      halInternalEnableMPU();                              \
    }                                                      \
  }
#endif

void halInternalLoadMPU(void);
void halInternalEnableMPU(void);
void halInternalDisableMPU(void);
void halInternalSetMPUGuardRegionStart(uint32_t baseAddress);





#endif // defined(__MPU_PRESENT) && (__MPU_PRESENT == 1U) && (__CORTEX_M <= 7U)

#endif//__MPU_H__
