/***************************************************************************//**
 * @file nvm3_lock.h
 * @brief NVM3 data access lock API definition
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

#ifndef NVM3_LOCK_H
#define NVM3_LOCK_H

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
 * @addtogroup NVM3Lock
 * @brief NVM3 lock module
 * @{
 * @details
 * This module provides the data protection tools for NVM3.
 *
 * If an application wishes to use other protection tools than those
 * supplied with NVM3, the functions in this module can be substituted.
 *
 * @note These functions are used by the NVM3 and should not be used by
 * any applications.
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *  Begin a lock section.
 ******************************************************************************/
void nvm3_lockBegin(void);

/***************************************************************************//**
 * @brief
 *  End a lock section.
 ******************************************************************************/
void nvm3_lockEnd(void);

/** @} (end addtogroup NVM3Lock) */
/** @} (end addtogroup NVM3) */
/** @} (end addtogroup emdrv) */

#ifdef __cplusplus
}
#endif

#endif //NVM3_LOCK_H
