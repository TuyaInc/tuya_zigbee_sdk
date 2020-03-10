/***************************************************************************//**
 * @file pa_curves_efr32.h
 * @brief PA power conversion curves used by Silicon Labs PA power conversion
 * functions
 * @details
 * This file contains the curves needed convert PA power levels
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
#ifndef __PA_CURVES_EFR32XG21_H_
#define __PA_CURVES_EFR32XG21_H_

#ifdef __cplusplus
extern "C" {
#endif

#define RAIL_PA_CURVES_PIECEWISE_SEGMENTS (8U)

#define RAIL_PA_CURVES_2P4_HP_VBAT_MAX_POWER      200
#define RAIL_PA_CURVES_2P4_HP_VBAT_MIN_POWER      -260
#define RAIL_PA_CURVES_2P4_HP_VBAT_CURVES \
  { { 180, 3100, -371424 },               \
    { 104, 1521, -128414 },               \
    { 59, 518, -4825 },                   \
    { 36, 298, 12565 },                   \
    { 23, 200, 16095 },                   \
    { 16, 130, 16107 },                   \
    { 7, 16, 8418 },                      \
    { 7, 80, 14145 } }

#define RAIL_PA_CURVES_2P4_MP_VBAT_MAX_POWER      100
#define RAIL_PA_CURVES_2P4_MP_VBAT_MIN_POWER      -260
#define RAIL_PA_CURVES_2P4_MP_VBAT_CURVES \
  { { -1, 0, 0 },                         \
    { -1, 0, 0 },                         \
    { 90, 2066, -107116 },                \
    { 48, 698, -3957 },                   \
    { 25, 224, 15680 },                   \
    { 15, 118, 15315 },                   \
    { 10, 83, 13608 },                    \
    { 7, 44, 10588 } }

#define RAIL_PA_CURVES_2P4_LP_VBAT_MAX_POWER      0
#define RAIL_PA_CURVES_2P4_LP_VBAT_MIN_POWER      -260
#define RAIL_PA_CURVES_2P4_LP \
  { { -1, 0, 0 },             \
    { -1, 0, 0 },             \
    { -1, 0, 0 },             \
    { -1, 0, 0 },             \
    { -1, 0, 0 },             \
    { 65, 1000, 76069 },      \
    { 33, 382, 49606 },       \
    { 19, 180, 33380 } }

// *INDENT-OFF*
// Macro to declare the variables needed to initialize RAIL_TxPowerCurvesConfig_t for use in
// RAIL_InitTxPowerCurves, assuming battery powered operation
#define RAIL_DECLARE_TX_POWER_VBAT_CURVES_ALT                                  \
  static const RAIL_TxPowerCurveAlt_t RAIL_piecewiseDataHp = {                 \
    RAIL_PA_CURVES_2P4_HP_VBAT_MAX_POWER,                                      \
    RAIL_PA_CURVES_2P4_HP_VBAT_MIN_POWER,                                      \
    RAIL_PA_CURVES_2P4_HP_VBAT_CURVES,                                         \
  };                                                                           \
  static const RAIL_TxPowerCurveAlt_t RAIL_piecewiseDataMp = {                 \
    RAIL_PA_CURVES_2P4_MP_VBAT_MAX_POWER,                                      \
    RAIL_PA_CURVES_2P4_MP_VBAT_MIN_POWER,                                      \
    RAIL_PA_CURVES_2P4_MP_VBAT_CURVES,                                         \
  };                                                                           \
  static const RAIL_TxPowerCurveAlt_t RAIL_piecewiseDataLp = {                 \
    RAIL_PA_CURVES_2P4_LP_VBAT_MAX_POWER,                                      \
    RAIL_PA_CURVES_2P4_LP_VBAT_MIN_POWER,                                      \
    RAIL_PA_CURVES_2P4_LP,                                                     \
  };
// *INDENT-OFF*

#define RAIL_DECLARE_TX_POWER_CURVES_CONFIG_ALT                                \
  {                                                                            \
    .curves = {                                                                \
      {                                                                        \
        .algorithm = RAIL_PA_ALGORITHM_PIECEWISE_LINEAR,                       \
        .segments = RAIL_PA_CURVES_PIECEWISE_SEGMENTS,                         \
        .min = RAIL_TX_POWER_LEVEL_HP_MIN,                                     \
        .max = RAIL_TX_POWER_LEVEL_HP_MAX,                                     \
        .conversion = { .powerCurve = &RAIL_piecewiseDataHp },                 \
      },                                                                       \
      {                                                                        \
        .algorithm = RAIL_PA_ALGORITHM_PIECEWISE_LINEAR,                       \
        .segments = RAIL_PA_CURVES_PIECEWISE_SEGMENTS,                         \
        .min = RAIL_TX_POWER_LEVEL_MP_MIN,                                     \
        .max = RAIL_TX_POWER_LEVEL_MP_MAX,                                     \
        .conversion = { .powerCurve = &RAIL_piecewiseDataMp },                 \
      },                                                                       \
      {                                                                        \
        .algorithm = RAIL_PA_ALGORITHM_PIECEWISE_LINEAR,                       \
        .segments = RAIL_PA_CURVES_PIECEWISE_SEGMENTS,                         \
        .min = RAIL_TX_POWER_LEVEL_LP_MIN,                                     \
        .max = RAIL_TX_POWER_LEVEL_LP_MAX,                                     \
        .conversion = { .powerCurve = &RAIL_piecewiseDataLp },               \
      },                                                                       \
    }                                                                          \
  }
// *INDENT-OFF*

#ifdef __cplusplus
}
#endif

#endif
