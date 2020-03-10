/***************************************************************************//**
 * @file nvm3_hal_flash.c
 * @brief Non-Volatile Memory Wear-Leveling driver HAL implementation
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

#include <stdbool.h>
#include <string.h>
#include "em_system.h"
#include "em_msc.h"
#include "nvm3_hal.h"
#include "nvm3.h"

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
 * @{
 ******************************************************************************/

/******************************************************************************
 ******************************    MACROS    **********************************
 *****************************************************************************/

#define CHECK_DATA  1           ///< Macro defining if data should be checked

/******************************************************************************
 ***************************   LOCAL VARIABLES   ******************************
 *****************************************************************************/

static uint32_t openCnt = 0;    ///< A variable that keeps track of the open count

/******************************************************************************
 ***************************   LOCAL FUNCTIONS   ******************************
 *****************************************************************************/

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

/***************************************************************************//**
 * @brief
 *   Convert return type.
 *
 * @details
 *   This function converts between the return type of the emlib and the
 *   NVM3 API.
 *
 * @param[in] result
 *   Operation result.
 *
 * @return
 *   Returns remapped status code.
 ******************************************************************************/
static Ecode_t convertMscStatusToNvm3Status(MSC_Status_TypeDef result)
{
  Ecode_t ret;

  switch (result) {
    case mscReturnOk:
      ret = ECODE_NVM3_OK;
      break;
    case mscReturnInvalidAddr:
      ret = ECODE_NVM3_ERR_INT_ADDR_INVALID;
      break;
    default:
      ret = ECODE_NVM3_ERR_INT_EMULATOR;
      break;
  }

  return ret;
}

static bool isErased(void *adr, size_t len)
{
  size_t i;
  size_t cnt;
  uint32_t *dat = adr;

  cnt = len / sizeof(uint32_t);
  for (i = 0U; i < cnt; i++) {
    if (*dat != 0xFFFFFFFFUL) {
      return false;
    }
    dat++;
  }

  return true;
}

/** @endcond */

/***************************************************************************//**
 * @brief
 *   Open the NVM3 flash driver.
 *
 * @details
 *
 ******************************************************************************/
Ecode_t nvm3_halOpen(nvm3_HalPtr_t nvmAdr, size_t flashSize)
{
  (void)nvmAdr;
  (void)flashSize;
  if (openCnt == 0U) {
    MSC_Init();
  }
  openCnt++;

  return ECODE_NVM3_OK;
}

void nvm3_halClose(void)
{
  if (openCnt > 0U) {
    openCnt--;
    if (openCnt == 0U) {
      MSC_Deinit();
    }
  }
}

void nvm3_halGetDeviceInfo(nvm3_HalDeviceInfo_t *deviceInfo)
{
  SYSTEM_ChipRevision_TypeDef chipRev;

  SYSTEM_ChipRevisionGet(&chipRev);
  deviceInfo->deviceFamily = chipRev.family;
  deviceInfo->memoryMapped = 1;
#if defined(_SILICON_LABS_32B_SERIES_2)
  deviceInfo->writeSize = NVM3_HAL_WRITE_SIZE_32;
#else
  deviceInfo->writeSize = NVM3_HAL_WRITE_SIZE_16;
#endif
  deviceInfo->pageSize = SYSTEM_GetFlashPageSize();
}

void nvm3_halReadBytes(nvm3_HalPtr_t nvmAdr, uint8_t *pDst, size_t cnt)
{
  uint8_t *pSrc = (uint8_t *)nvmAdr;

  while (cnt > 0U) {
    *pDst++ = *pSrc++;
    cnt--;
  }
}

void nvm3_halReadWords(nvm3_HalPtr_t nvmAdr, uint32_t *pDst, size_t cnt)
{
  uint32_t *pSrc = (uint32_t *)nvmAdr;

  while (cnt > 0U) {
    *pDst++ = *pSrc++;
    cnt--;
  }
}

Ecode_t nvm3_halWriteBytes(nvm3_HalPtr_t nvmAdr, void const *pSrc, size_t cnt)
{
  Ecode_t halSta = ECODE_NVM3_OK;
  size_t wordCnt = (cnt / sizeof(uint32_t));
  size_t byteCnt = cnt;
  const uint32_t *wordSrc = pSrc;

  if (wordCnt > 0U) {
    halSta = nvm3_halWriteWords(nvmAdr, wordSrc, wordCnt);
    byteCnt -= wordCnt * sizeof(uint32_t);
    nvmAdr = (nvm3_HalPtr_t)((size_t)nvmAdr + wordCnt * sizeof(uint32_t));
    wordSrc += wordCnt;
  }
  if ((byteCnt > 0U) && (halSta == ECODE_NVM3_OK)) {
    uint32_t lastWord = 0xFFFFFFFFU;

    (void)memcpy(&lastWord, (const void *)wordSrc, byteCnt);
    wordCnt = 1U;
    halSta = nvm3_halWriteWords(nvmAdr, &lastWord, wordCnt);
  }

  return halSta;
}

Ecode_t nvm3_halWriteWords(nvm3_HalPtr_t nvmAdr, void const *pSrc, size_t cnt)
{
  uint32_t *pDst = (uint32_t *)nvmAdr;
  MSC_Status_TypeDef mscSta;
  Ecode_t halSta;
  size_t bytes;
  const uint32_t *wordSrc = pSrc;

#if CHECK_DATA
  if (!isErased(nvmAdr, cnt * sizeof(uint32_t))) {
    return ECODE_NVM3_ERR_INT_WRITE_TO_NOT_ERASED;
  }
#endif

  bytes = cnt * sizeof(uint32_t);
  mscSta = MSC_WriteWord(pDst, wordSrc, bytes);
  halSta = convertMscStatusToNvm3Status(mscSta);

#if CHECK_DATA
  if (halSta == ECODE_NVM3_OK) {
    if (memcmp(pDst, wordSrc, bytes) != 0) {
      halSta = ECODE_NVM3_ERR_WRITE_FAILED;
    }
  }
#endif

  return halSta;
}

Ecode_t nvm3_halPageErase(nvm3_HalPtr_t nvmAdr)
{
  MSC_Status_TypeDef mscSta;
  Ecode_t halSta;

  mscSta = MSC_ErasePage((uint32_t *)nvmAdr);
  halSta = convertMscStatusToNvm3Status(mscSta);

#if CHECK_DATA
  if (halSta == ECODE_NVM3_OK) {
    if (!isErased(nvmAdr, SYSTEM_GetFlashPageSize())) {
      halSta = ECODE_NVM3_ERR_ERASE_FAILED;
    }
  }
#endif

  return halSta;
}

/** @} (end addtogroup NVM3Hal) */
/** @} (end addtogroup NVM3) */
/** @} (end addtogroup emdrv) */
