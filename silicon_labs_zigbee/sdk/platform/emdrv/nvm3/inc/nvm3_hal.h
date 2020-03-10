/***************************************************************************//**
 * @file nvm3_hal.h
 * @brief NVM3 driver HAL
 * @version 5.6.0
 *******************************************************************************
 * # License
 * <b>(C) Copyright 2017 Silicon Labs, www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/

#ifndef NVM3_HAL_H
#define NVM3_HAL_H

#include "ecode.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup emdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup NVM3
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup NVM3Hal
 * @brief NVM3 hal module
 * @{
 * @details
 * This module provides the interface to the NVM. By having all NVM access
 * functions in a separate file, it is possible to support different hardware
 * by just substituting the functions in this module.
 *
 * @note These functions are used by the NVM3 and should not be used by
 * any applications.
 ******************************************************************************/

/******************************************************************************
 ******************************    MACROS    **********************************
 *****************************************************************************/

#ifdef NVM3_HOST_BUILD
#define __STATIC_INLINE static inline
#endif

#define NVM3_HAL_WRITE_SIZE_32    0     ///< Only single writes are allowed
#define NVM3_HAL_WRITE_SIZE_16    1     ///< Two writes are allowed

/******************************************************************************
 ******************************   TYPEDEFS   **********************************
 *****************************************************************************/

/// @brief Pointer to NVM
typedef void   *nvm3_HalPtr_t;

/// @brief Write size capabilities in the NVM

typedef struct nvm3_HalDeviceInfo {
  uint8_t deviceFamily;
  uint8_t writeSize;
  uint8_t memoryMapped;
  size_t pageSize;
} nvm3_HalDeviceInfo_t;

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

//************************************
// Include some target specific stuff

#ifdef NVM3_HOST_BUILD
#include "nvm3_hal_host.h"
#else
# include "em_assert.h"
# include "em_common.h"
#endif

/***************************************************************************//**
 * @brief
 *  Open the NVM3 hal for usage.
 *
 * @details
 *   This function must be run at initialization, before any other functions
 *   are called. It is used to call necessary startup routines before the
 *   hardware can be accessed.
 *
 * @param[in] nvmAdr
 *   A pointer to the destination in NVM.
 *
 * @param[in] nvmSize
 *   The total size of the NVM.
 *
 * @return
 *   The result of the open call.
 *   @ref ECODE_NVM3_OK on success or a NVM3 @ref Ecode_t on failure.
 ******************************************************************************/
Ecode_t nvm3_halOpen(nvm3_HalPtr_t nvmAdr, size_t nvmSize);

/***************************************************************************//**
 * @brief
 *  Close the NVM3 hal for usage.
 *
 * @details
 *   This function should be called at program termination.
 *   Should be done before any graceful halts.
 ******************************************************************************/
void    nvm3_halClose(void);

/***************************************************************************//**
 * @brief
 *   Retrieve device information.
 *
 * @details
 *   This function is used to retrieve information about the device properties
 *   like the device family, write size, whether the NVM is memory mapped or
 *   not, and finally the NVM page size.
 *
 * @param[in] deviceInfo
 *   A pointer to a struct that will receive the device information.
 ******************************************************************************/
void nvm3_halGetDeviceInfo(nvm3_HalDeviceInfo_t *deviceInfo);

/***************************************************************************//**
 * @brief
 *   Erase a page in the NVM.
 *
 * @details
 *   This function is used to erase an NVM page.
 *
 * @param[in] nvmAdr
 *   A memory address pointing to the start of the page to erase.
 *
 * @return
 *   The result of the erase operation.
 ******************************************************************************/
Ecode_t nvm3_halPageErase(nvm3_HalPtr_t nvmAdr);

/***************************************************************************//**
 * @brief
 *   Read data from NVM.
 *
 * @details
 *   This function is used to read data from the NVM. It will be a
 *   blocking call, since the thread asking for data to be read cannot continue
 *   without the data.
 *
 * @param[in] nvmAdr
 *   A memory address in NVM where the data shall be read.
 *
 * @param[in] *pDst
 *   A pointer to the destination buffer.
 *
 * @param[in] cnt
 *   The number of bytes to read.
 ******************************************************************************/
void nvm3_halReadBytes(nvm3_HalPtr_t nvmAdr, uint8_t *pDst, size_t cnt);

/***************************************************************************//**
 * @brief
 *   Read data from NVM.
 *
 * @details
 *   This function is used to read data from the NVM. It will be a
 *   blocking call, since the thread asking for data to be read cannot continue
 *   without the data.
 *
 * @param[in] nvmAdr
 *   A memory address in NVM where the data shall be read.
 *
 * @param[in] *pDst
 *   A pointer to the destination buffer.
 *
 * @param[in] cnt
 *   The number of words to read.
 ******************************************************************************/
void nvm3_halReadWords(nvm3_HalPtr_t nvmAdr, uint32_t *pDst, size_t cnt);

/***************************************************************************//**
 * @brief
 *  Write data to NVM.
 *
 * @param[in] nvmAdr
 *   A memory address in NVM where the data shall be written.
 *
 * @param[in] pSrc
 *   A pointer to the source data.
 *
 * @param[in] cnt
 *   The number of bytes to write.
 *
 * @return
 *   The result of the write operation.
 *   @ref ECODE_NVM3_OK on success or a NVM3 @ref Ecode_t on failure.
 ******************************************************************************/
Ecode_t nvm3_halWriteBytes(nvm3_HalPtr_t nvmAdr, void const *pSrc, size_t cnt);

/***************************************************************************//**
 * @brief
 *   Write data to NVM.
 *
 * @details
 *   This function is used to write data to the NVM. This is a blocking
 *   function.
 *
 * @param[in] nvmAdr
 *   A memory address in NVM where the data shall be written.
 *
 * @param[in] *pSrc
 *   A pointer to the source data.
 *
 * @param[in] cnt
 *   The number of words to write.
 *
 * @return
 *   The result of the write operation.
 *   @ref ECODE_NVM3_OK on success or a NVM3 @ref Ecode_t on failure.
 ******************************************************************************/
Ecode_t nvm3_halWriteWords(nvm3_HalPtr_t nvmAdr, void const *pSrc, size_t cnt);

/** @} (end addtogroup NVM3Hal) */
/** @} (end addtogroup NVM3) */
/** @} (end addtogroup emdrv) */

#ifdef __cplusplus
}
#endif

#endif /* NVM3_HAL_H */
