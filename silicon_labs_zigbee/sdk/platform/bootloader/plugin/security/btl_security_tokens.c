/***************************************************************************//**
 * @file btl_security_tokens.c
 * @brief Manufacturing token handling
 * @version 1.7.0
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2016 Silicon Laboratories, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/
#include "btl_security_tokens.h"
#include "em_device.h"

#if !defined(LOCKBITS_BASE)
#if defined(_SILICON_LABS_GECKO_INTERNAL_SDID_200)
// Lockbits are placed in the topmost flash page
#define LOCKBITS_BASE ((FLASH_BASE) + (FLASH_SIZE) -(FLASH_PAGE_SIZE))
#else
#error "Lockbits location unknown"
#endif
#endif

const uint8_t* btl_getSignedBootloaderKeyXPtr(void)
{
  return (const uint8_t*)(LOCKBITS_BASE + 0x34A);
}

const uint8_t* btl_getSignedBootloaderKeyYPtr(void)
{
  return (const uint8_t*)(LOCKBITS_BASE + 0x36A);
}

const uint8_t* btl_getImageFileEncryptionKeyPtr(void)
{
  return (const uint8_t*)(LOCKBITS_BASE + 0x286);
}
