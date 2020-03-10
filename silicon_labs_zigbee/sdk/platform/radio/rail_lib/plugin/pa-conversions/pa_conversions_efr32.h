/***************************************************************************//**
 * @file pa_conversions_efr32.h
 * @brief PA power conversion functions provided to the customer as source for
 *        highest level of customization.
 * @details
 * This file contains the curves and logic that convert PA power levels
 * to dBm powers.
 *
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2017 Silicon Labs, www.silabs.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Labs has no
 * obligation to support this Software. Silicon Labs is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Silicon Labs will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 ******************************************************************************/
#ifndef __PA_CONVERSIONS_EFR32_H_
#define __PA_CONVERSIONS_EFR32_H_

#include "rail_types.h"
#include "rail_chip_specific.h"

#ifdef HAL_PA_CURVE_HEADER
#include HAL_PA_CURVE_HEADER
#else
#include "pa_curves_efr32.h"
#endif

#ifdef RAIL_PA_CURVE_TYPES
#include RAIL_PA_CURVE_TYPES
#else
#include "pa_curve_types_efr32.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/// The curves to be used when battery voltage powers transmission
extern const RAIL_TxPowerCurvesConfigAlt_t RAIL_TxPowerCurvesVbat;

/// The curves to be used when the DC-DC converter powers transmission
extern const RAIL_TxPowerCurvesConfigAlt_t RAIL_TxPowerCurvesDcdc;

/**
 * Initialize TxPower curves.
 *
 * @param[in] txPowerCurvesConfig Struct containing pointers to custom
 * tx power curves.
 * @return RAIL_Status_t indicating success or an error.
 *
 * @note: This function is deprecated, and will no longer be supported
 * for any chips released after EFRXG1X parts. Please use
 * RAIL_InitTxPowerCurvesAlt instead.
 */
RAIL_Status_t RAIL_InitTxPowerCurves(const RAIL_TxPowerCurvesConfig_t *config);

/**
 * Initialize TxPower curves.
 *
 * @param[in] txPowerCurvesConfig Struct containing pointers to custom
 * tx power curves.
 * @return RAIL_Status_t indicating success or an error.
 *
 */
RAIL_Status_t RAIL_InitTxPowerCurvesAlt(const RAIL_TxPowerCurvesConfigAlt_t *config);

/**
 * Gets the curve that should be used for conversion functions based on the
 * current PA configuration.
 *
 * @param[in] mode PA mode whose curves are needed.
 * @return RAIL_TxPowerCurves_t that should be used for conversion functions.
 *
 */
RAIL_TxPowerCurves_t const * RAIL_GetTxPowerCurve(RAIL_TxPowerMode_t mode);

#ifdef __cplusplus
}
#endif

#endif
