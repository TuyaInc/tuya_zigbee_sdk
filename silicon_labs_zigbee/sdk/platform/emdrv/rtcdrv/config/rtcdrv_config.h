/***************************************************************************//**
 * @file rtcdrv_config.h
 * @brief RTCDRV configuration file.
 * @version 5.6.0
 *******************************************************************************
 * # License
 * <b>(C) Copyright 2015 Silicon Labs, www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/
#ifndef SILICON_LABS_RTCDRV_CONFIG_H
#define SILICON_LABS_RTCDRV_CONFIG_H

/// @cond DO_NOT_INCLUDE_WITH_DOXYGEN
#if defined(HAL_CONFIG)
#include "hal-config.h"
#if defined(HAL_CLK_LFECLK_SOURCE)
#if (HAL_CLK_LFECLK_SOURCE == HAL_CLK_LFCLK_SOURCE_LFRCO)
#define EMDRV_RTCDRV_USE_LFRCO 1
#endif
#else
#if (HAL_CLK_LFACLK_SOURCE == HAL_CLK_LFCLK_SOURCE_LFRCO)
#define EMDRV_RTCDRV_USE_LFRCO 1
#endif
#endif
#endif
/// @endcond

/***************************************************************************//**
 * @addtogroup emdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup RTCDRV
 * @{
 ******************************************************************************/

/// @brief Define the number of timers the application needs.
#define EMDRV_RTCDRV_NUM_TIMERS     (4)

/// @brief Define to include wallclock functionality.
//#define EMDRV_RTCDRV_WALLCLOCK_CONFIG

/// @brief Define to enable integration with SLEEP driver.
//#define EMDRV_RTCDRV_SLEEPDRV_INTEGRATION

/// @brief Define to configure RTCDRV for LFRCO. The default is LFXO.
//#define EMDRV_RTCDRV_USE_LFRCO

/** @} (end addtogroup RTCDRV) */
/** @} (end addtogroup emdrv) */

#endif /* SILICON_LABS_RTCDRV_CONFIG_H */
