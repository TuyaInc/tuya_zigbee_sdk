/***************************************************************************//**
 * @file btl_storage.c
 * @brief Storage plugin for the Gecko Bootloader.
 * @author Silicon Labs
 * @version 1.7.0
 *******************************************************************************
 * @section License
 * <b>Copyright 2016 Silicon Laboratories, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/
#include "config/btl_config.h"

#include "api/btl_interface.h"
#include "btl_storage.h"

#include "core/btl_reset.h"
#include "core/btl_parse.h"
#include "plugin/debug/btl_debug.h"

// --------------------------------
// Prototypes

static int32_t installImageFromSlot(int32_t slotId);

// --------------------------------
// Function implementations

uint32_t storage_getBaseAddress(void)
{
#ifdef BTL_STORAGE_BASE_ADDRESS
  return BTL_STORAGE_BASE_ADDRESS;
#else
  return 0;
#endif
}

int32_t storage_main(void)
{
#if BTL_STORAGE_BOOTLOAD_LIST_LENGTH == 1
  return installImageFromSlot(0);
#else
  int32_t ret;

  int32_t slotIds[BTL_STORAGE_BOOTLOAD_LIST_LENGTH] = { -1 };

  ret = storage_getBootloadList(slotIds, BTL_STORAGE_BOOTLOAD_LIST_LENGTH);
  if (ret != BOOTLOADER_OK) {
    BTL_DEBUG_PRINTLN("BI err");
    return ret;
  }

  // Attempt to bootload given images in sequence
  for (size_t id = 0; id < BTL_STORAGE_BOOTLOAD_LIST_LENGTH; id++) {
    if (slotIds[id] == -1) {
      // Invalid slot ID; try the next one
      continue;
    }

    ret = installImageFromSlot(slotIds[id]);
    if (ret == BOOTLOADER_OK) {
      break;
    }
  }

  return ret;
#endif
}

static int32_t installImageFromSlot(int32_t slotId)
{
  BootloaderParserContext_t parseContext;
  int32_t ret;

  BTL_DEBUG_PRINT("Slot: ");
  BTL_DEBUG_PRINT_WORD_HEX(slotId);
  BTL_DEBUG_PRINT_LF();

  // Get info about the image marked for bootloading
  storage_initParseSlot(slotId,
                        &parseContext,
                        sizeof(BootloaderParserContext_t));

  ret = BOOTLOADER_ERROR_PARSE_CONTINUE;
  do {
    ret = storage_verifySlot(&parseContext, NULL);
  } while (ret == BOOTLOADER_ERROR_PARSE_CONTINUE);

  if (ret != BOOTLOADER_ERROR_PARSE_SUCCESS) {
    // Image in slot is non-existant or corrupt.
    // Continue to next image
    BTL_DEBUG_PRINTLN("Verify fail");
    return BOOTLOADER_ERROR_STORAGE_BOOTLOAD;
  }

  if (parseContext.imageProperties.imageContainsBootloader
      && !parseContext.imageProperties.imageContainsApplication) {
    BTL_DEBUG_PRINTLN("BL upg with no app");
    return BOOTLOADER_ERROR_STORAGE_BOOTLOAD;
  }

  if (parseContext.imageProperties.imageContainsBootloader) {
    BTL_DEBUG_PRINT("BL upg ");
    BTL_DEBUG_PRINT_WORD_HEX(mainBootloaderTable->header.version);
    BTL_DEBUG_PRINT(" -> ");
    BTL_DEBUG_PRINT_WORD_HEX(parseContext.imageProperties.bootloaderVersion);
    BTL_DEBUG_PRINT_LF();
  }

  if (parseContext.imageProperties.imageContainsBootloader
      && (parseContext.imageProperties.bootloaderVersion
          > mainBootloaderTable->header.version)) {
    // This is a bootloader upgrade, and we also have an application
    // available for after the bootloader upgrade is complete
    if (!storage_bootloadBootloaderFromSlot(
          parseContext.slotId,
          parseContext.imageProperties.bootloaderVersion)) {
      // Bootloader upgrade failed; not a valid image
      BTL_DEBUG_PRINTLN("Btl bootload fail");
    } else {
      // Bootloader successfully written to internal flash; reboot and apply
      reset_resetWithReason(BOOTLOADER_RESET_REASON_UPGRADE);
    }
    return BOOTLOADER_ERROR_STORAGE_BOOTLOAD;
  } else {
    // This should be an application upgrade
    if (!parseContext.imageProperties.imageContainsApplication) {
      // ...but there is no app in the EBL
      BTL_DEBUG_PRINTLN("No app in slot");
      // Continue to next image
      return BOOTLOADER_ERROR_STORAGE_BOOTLOAD;
    } else if (!storage_bootloadApplicationFromSlot(
                 parseContext.slotId,
                 parseContext.imageProperties.application.version)) {
      // App upgrade failed.
      BTL_DEBUG_PRINTLN("App bootload fail");
      // Continue to next image
      return BOOTLOADER_ERROR_STORAGE_BOOTLOAD;
    } else {
      // Application was updated. Reboot into new image.
    }
  }

  return BOOTLOADER_OK;
}
