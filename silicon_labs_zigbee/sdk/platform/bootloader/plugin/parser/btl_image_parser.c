/***************************************************************************//**
 * @file btl_image_parser.c
 * @brief Image parser for Silicon Labs bootloader
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

#include <stdbool.h>
#include "api/btl_interface.h"

#include "em_common.h"

bool parser_requireAuthenticity(void)
{
#if defined(BOOTLOADER_ENFORCE_SIGNED_UPGRADE)
  return true;
#else
  return false;
#endif
}

bool parser_requireConfidentiality(void)
{
#if defined(BOOTLOADER_ENFORCE_ENCRYPTED_UPGRADE)
  return true;
#else
  return false;
#endif
}

uint32_t parser_getApplicationAddress(void)
{
  return (uint32_t)(&(mainBootloaderTable->startOfAppSpace->stackTop));
}

uint32_t parser_getBootloaderUpgradeAddress(void)
{
#if defined(BOOTLOADER_HAS_FIRST_STAGE)
  if (firstBootloaderTable->header.type == BOOTLOADER_MAGIC_FIRST_STAGE) {
    return (uint32_t)(firstBootloaderTable->upgradeLocation);
  } else {
    return 0UL;
  }
#else
  // Not supported
  return 0xFFFFFFFFUL;
#endif
}

SL_WEAK bool parser_applicationUpgradeValidCallback(ApplicationData_t *app)
{
  (void) app;
  // By default, all applications are considered valid
  return true;
}
