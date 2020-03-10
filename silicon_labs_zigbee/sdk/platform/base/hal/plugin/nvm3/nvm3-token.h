/** @file hal/plugin/nvm3/nvm3-token.h
 * @brief NVM3 system for wear leveling token storage across flash.
 * See @ref nvm3 for documentation.
 *
 * <!-- Copyright 2008-2011 by Ember Corporation. All rights reserved.   *80*-->
 */
#ifndef __NVM3_TOKEN_H__
#define __NVM3_TOKEN_H__

#include "nvm3.h"

/** @addtogroup nvm3
 * The NVM3 system is designed to operate under the @ref tokens API and provide
 * a non-volatile storage system.  Since the flash write cycles
 * are finite, the NVM3's primary purpose is to perform wear
 * leveling across several hardware flash pages, ultimately increasing the
 * number of times tokens may be written before a hardware failure.
 *
 * See platform/emdrv/nvm3/inc/nvm3.h for source code.
 *@{
 */

//application functions

/** @brief The NVM3 callback function, implemented by the
 * application.
 *
 * @param status  An ::Ecode_t error code indicating one of the conditions
 * described in nvm3.h
 *
 * This callback will report any Ecode_t error codes passed back from the NVM3
 * API functions when they are called through the token API.
 *
 */
void halNvm3Callback(Ecode_t status);

#endif //__NVM3_TOKEN_H__

/**@} END nvm3 group
 */
