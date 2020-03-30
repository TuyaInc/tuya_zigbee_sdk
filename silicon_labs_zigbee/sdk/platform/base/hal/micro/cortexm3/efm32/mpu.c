/***************************************************************************//**
 * @file
 * @brief This file contains the definitions, data table and software used to
 * setup the MPU.
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
#include PLATFORM_HEADER
#include "hal/micro/cortexm3/efm32/mpu.h"
#include "hal/micro/micro.h"

#if defined(__MPU_PRESENT) && (__MPU_PRESENT == 1U) && (__CORTEX_M <= 7U)
// Load the base address and attributes of all 8 MPU regions, then enable
// the MPU. Even though interrupts should be disabled when this function is
// called, the region loading is performed in an atomic block in case they
// are not disabled. After the regions have been defined, the MPU is enabled.
// To be safe, memory barrier instructions are included to make sure that
// the new MPU setup is in effect before returning to the caller.
//
// Note that the PRIVDEFENA bit is not set in the MPU_CTRL register so the
// default privileged memory map is not enabled. Disabling the default
// memory map enables faults on core accesses (other than vector reads) to
// the address ranges shown below.
//

// This table contains 5 MPU region definitions.
static const ARM_MPU_Region_t mpuTable[] = {
  // *INDENT-OFF*
  //            RegionNo    BaseAddr
  //            DisableExec AccessPermission TypeExtField IsShareable IsCacheable IsBufferable SubRegionDisable Size

  { ARM_MPU_RBAR(0UL,       FLASH_REGION),
    ARM_MPU_RASR(0UL,       ARM_MPU_AP_URO,  1UL,         0UL,        1UL,        1UL,         0xE8UL,          ARM_MPU_REGION_SIZE_512KB) },

  { ARM_MPU_RBAR(1UL,       PERIPH_REGION),
    ARM_MPU_RASR(0UL,       ARM_MPU_AP_URO,  2UL,         0UL,        0UL,        0UL,         0x10UL,          ARM_MPU_REGION_SIZE_64KB) },

  { ARM_MPU_RBAR(2UL,       USERPER_REGION),
    ARM_MPU_RASR(0UL,       ARM_MPU_AP_FULL, 1UL,         0UL,        1UL,        1UL,         0x03UL,          ARM_MPU_REGION_SIZE_32KB) },

  { ARM_MPU_RBAR(3UL,       SRAM_REGION),
    ARM_MPU_RASR(0UL,       ARM_MPU_AP_FULL, 1UL,         0UL,        1UL,        1UL,         0xC0UL,          ARM_MPU_REGION_SIZE_32KB) },

  { ARM_MPU_RBAR(4UL,       GUARD_REGION),
    ARM_MPU_RASR(0UL,       ARM_MPU_AP_NONE, 1UL,         0UL,        0UL,        0UL,         0x00UL,          ARM_MPU_REGION_SIZE_32B) }
  // *INDENT-ON*
};

void halInternalMPUClrRegion(uint32_t regionNumber)
{
  ARM_MPU_ClrRegion(regionNumber);
}

void halInternalMPUInit(void)
{
  uint32_t i;
  uint32_t disRegion[] = { 1UL, 2UL, 4UL };
  for (i = 0UL; i < (sizeof(disRegion) / sizeof(*disRegion)); i++) {
    halInternalMPUClrRegion(disRegion[i]);
  }
}

void halInternalEnableMPU(void)
{
  DECLARE_INTERRUPT_STATE;
  DISABLE_INTERRUPTS();
  ARM_MPU_Disable();
  halInternalLoadMPU();
  halInternalMPUInit();
  ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk);
  RESTORE_INTERRUPTS();
}

void halInternalLoadMPU(void)
{
  ARM_MPU_Load(mpuTable, NUM_MPU_REGIONS);        /* Load all MPU settings from the table */
}

void halInternalDisableMPU(void)
{
  ARM_MPU_Disable();
}

void halInternalSetMPUGuardRegionStart(uint32_t baseAddress)
{
  uint32_t rbar, rasr;

  rbar = ARM_MPU_RBAR(0xF, baseAddress);

  // If the base address is below the reset info then something weird is
  // going on so just turn off the guard region
  if (baseAddress < (uint32_t)(uint8_t *)_RESETINFO_SEGMENT_END) {
    rasr = 0U;
  } else {
    rasr = ARM_MPU_RASR(1U, ARM_MPU_AP_NONE, 1U, 0U, 1U, 1U, 0x00U, ARM_MPU_REGION_SIZE_32B);
  }

  DECLARE_INTERRUPT_STATE;
  DISABLE_INTERRUPTS();
  ARM_MPU_SetRegion(rbar, rasr);
  RESTORE_INTERRUPTS();
}

bool halInternalIAmAnEmulator(void)
{
  return 0;
}

#endif // defined(__MPU_PRESENT) && (__MPU_PRESENT == 1U) && (__CORTEX_M <= 7U)
