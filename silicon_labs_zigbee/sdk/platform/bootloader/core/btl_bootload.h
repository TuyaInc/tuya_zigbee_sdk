/***************************************************************************//**
 * @file btl_bootload.h
 * @brief Bootloading functionality for the Silicon Labs bootloader
 * @author Silicon Labs
 * @version 1.7.0
 *******************************************************************************
 * # License
 * <b>Copyright 2016 Silicon Laboratories, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/
#ifndef BTL_BOOTLOAD_H
#define BTL_BOOTLOAD_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/***************************************************************************//**
 * @addtogroup Core Bootloader Core
 * @{
 * @addtogroup Bootload
 * @brief Methods to verify and bootload application images
 * @details
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * Verify the app starting at address startAddress
 *
 * If secure boot is enforced, the function will only return true if the
 * cryptographic signature of the application is valid. Else, the application
 * is validated according to the signature type present in the application
 * ApplicationProperties_t structure.
 *
 * Examples without secure boot:
 * - App has no signature: Valid if initial stack pointer and program counter
 *                         have reasonable values
 * - App has CRC checksum: Valid if checksum is valid
 * - App has ECDSA signature: Valid if ECDSA signature is valid
 *
 * @param[in] startAddress Starting address of the application
 *
 * @return True if the image is deemed valid
 ******************************************************************************/
bool bootload_verifyApplication(uint32_t startAddress);

/***************************************************************************//**
 * Bootloader upgrade callback implementation
 *
 * @param offset          Offset of bootloader data (byte counter
 *                        incrementing from 0)
 * @param data            Raw bootloader data
 * @param length          Size in bytes of raw bootloader data.
 * @param context         A context variable defined by the implementation that
 *                        is implementing this callback.
 ******************************************************************************/
void bootload_bootloaderCallback(uint32_t offset,
                                 uint8_t  data[],
                                 size_t   length,
                                 void     *context);

/***************************************************************************//**
 * Image data callback implementation
 *
 * @param address         Address (inside the raw image) the data starts at
 * @param data            Raw image data
 * @param length          Size in bytes of raw image data. Constrained to always
 *                        be a multiple of four.
 * @param context         A context variable defined by the implementation that
 *                        is implementing this callback.
 ******************************************************************************/
void bootload_applicationCallback(uint32_t address,
                                  uint8_t  data[],
                                  size_t   length,
                                  void     *context);

/***************************************************************************//**
 * Whether the bootloader should enforce secure boot
 *
 * @return  True if secure boot is to be enforced
 ******************************************************************************/
bool bootloader_enforceSecureBoot(void);

/** @} addtogroup bootload */
/** @} addtogroup core */

#endif // BTL_BOOTLOAD_H
