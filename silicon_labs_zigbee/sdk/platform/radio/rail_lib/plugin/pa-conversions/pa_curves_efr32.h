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
#ifndef __PA_CURVES_EFR32_H_
#define __PA_CURVES_EFR32_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _SILICON_LABS_32B_SERIES_1
#include "pa_curves_efr32xg1x.h"
#elif defined (_SILICON_LABS_32B_SERIES_2)
#include "pa_curves_efr32xg21.h"
#endif

#ifdef __cplusplus
}
#endif

#endif
