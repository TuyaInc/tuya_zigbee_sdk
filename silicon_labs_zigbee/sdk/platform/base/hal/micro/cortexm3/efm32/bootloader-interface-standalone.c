/***************************************************************************//**
 * @file
 * @brief EM3XX-specific standalone bootloader HAL functions
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
#include "stack/include/ember-types.h"
#include "stack/include/error.h"
#include "stack/include/stack-info.h"
#include "hal/hal.h"
#include "hal/micro/cortexm3/memmap.h"
#include "hal/micro/cortexm3/diagnostic.h"

#include "api/btl_interface.h"

extern uint8_t emGetPhyRadioChannel(void);
extern int8_t emGetPhyRadioPower(void);

#if !defined _SILICON_LABS_32B_SERIES_1_CONFIG_1

#define NO_BAT

static bool bootloaderIsCommonBootloader(void)
{
  return true;
}

#else

static bool bootloaderIsCommonBootloader(void)
{
  if (halBootloaderAddressTable.baseTable.type == BOOTLOADER_ADDRESS_TABLE_TYPE) {
    return false;
  } else {
    return true;
  }
}

#endif

EmberStatus halLaunchStandaloneBootloader(uint8_t mode)
{
  if (bootloaderIsCommonBootloader()) {
    if (!(mainBootloaderTable->capabilities & BOOTLOADER_CAPABILITY_COMMUNICATION)) {
      return EMBER_ERR_FATAL;
    }
  } else {
#ifndef NO_BAT
    if (BOOTLOADER_BASE_TYPE(halBootloaderAddressTable.bootloaderType)
        != BL_TYPE_STANDALONE) {
      return EMBER_ERR_FATAL;
    }
#else
    return EMBER_ERR_FATAL;
#endif
  }
  if ((mode == STANDALONE_BOOTLOADER_NORMAL_MODE)
      || (mode == STANDALONE_BOOTLOADER_RECOVERY_MODE)) {
    // should never return
    // standard bootloader reset
    halInternalSysReset(RESET_BOOTLOADER_BOOTLOAD);
  }

  return EMBER_ERR_FATAL;
}

uint16_t halGetStandaloneBootloaderVersion(void)
{
  if (bootloaderIsCommonBootloader()) {
    return mainBootloaderTable->header.version >> 16;
  } else {
#ifndef NO_BAT
    if (BOOTLOADER_BASE_TYPE(halBootloaderAddressTable.bootloaderType)
        == BL_TYPE_STANDALONE) {
      return halGetBootloaderVersion();
    }
#endif
    return BOOTLOADER_INVALID_VERSION;
  }
}
