/***************************************************************************//**
 * @file rail_ble.h
 * @brief The BLE specific header file for the RAIL library.
 * @copyright Copyright 2016 Silicon Laboratories, Inc. www.silabs.com
 ******************************************************************************/

#ifndef __RAIL_BLE_H__
#define __RAIL_BLE_H__

// Get the standard include types
#include <stdint.h>
#include <stdbool.h>

// Get the RAIL specific structures and types
#include "rail_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup BLE
/// @ingroup Protocol_Specific
/// Accelerator routines for Bluetooth Low Energy (BLE).
///
/// The APIs in this module configure the radio for BLE
/// operation and provide additional helper routines necessary for
/// normal BLE send/receive that aren't available directly in RAIL.
/// RAIL APIs should be used to set up the application. However,
/// RAIL_ConfigChannels() and RAIL_ConfigRadio() should not be called to set up
/// the PHY. Instead, RAIL_BLE_Config* APIs should be used to set up the
/// 1 Mbps, 2 Mbps, or Coded PHY configurations needed by the application. These
/// APIs will configure the hardware and also configure the set of valid BLE
/// channels.
///
/// To implement a standard BLE link layer, you will also need to handle tight
/// turnaround times and send packets at specific instants. This can all be
/// managed through general RAIL functions, such as RAIL_ScheduleTx(),
/// RAIL_ScheduleRx(), and RAIL_SetStateTiming(). See RAIL APIs for more
/// useful functions.
///
/// A simple example to set up the application to be in BLE mode is shown
/// below. Note that this will put the radio on the first advertising channel
/// with the advertising Access Address. In any full-featured BLE application you
/// will need to use the RAIL_BLE_ConfigChannelRadioParams() function to change
/// the sync word and other parameters as needed based on your connection.
///
/// @code{.c}
///
/// // RAIL Handle set at initialization time.
/// static RAIL_Handle_t railHandle = NULL;
///
/// // Set the radio to receive on the first BLE advertising channel.
/// int bleAdvertiseEnable(void)
/// {
///   // Calls the BLE initialization function to load the right radio configuration.
///   RAIL_BLE_Init(railHandle);
///
///   // Always choose the Viterbi PHY configuration if available on your chip
///   // for performance reasons.
///   RAIL_BLE_ConfigPhy1MbpsViterbi(railHandle);
///
///   // Configures us for the first advertising channel (Physical: 0, Logical: 37).
///   // The CRC init value and Access Address come from the BLE specification.
///   RAIL_BLE_ConfigChannelRadioParams(railHandle,
///                                     0x555555,
///                                     0x8E89BED6,
///                                     37,
///                                     false);
///
///   // Starts receiving on physical channel 0 (logical channel 37).
///   RAIL_StartRx(railHandle, 0, NULL);
///  }
/// @endcode
///
/// @{

/**
 * @enum RAIL_BLE_Coding_t
 * @brief The variant of the BLE Coded PHY.
 */
RAIL_ENUM(RAIL_BLE_Coding_t) {
  /** Enables the 125 kbps variant of the BLE Coded PHY */
  RAIL_BLE_Coding_125kbps = 0,
  /** \deprecated Will be removed in a future version of RAIL */
  RAIL_BLE_Coding_125kbps_DSA = 1,
  /** Enables the 500 kbps variant of the BLE Coded PHY */
  RAIL_BLE_Coding_500kbps = 2,
  /** \deprecated Will be removed in a future version of RAIL */
  RAIL_BLE_Coding_500kbps_DSA = 3,
};

/**
 * @struct RAIL_BLE_State_t
 * @brief A state structure for BLE.
 *
 * This structure must be allocated in application global read-write memory
 * that persists for the duration of BLE usage. It cannot be allocated
 * in read-only memory or on the call stack.
 */
typedef struct RAIL_BLE_State {
  uint32_t crcInit; /**< The value used to initialize the CRC algorithm. */
  uint32_t accessAddress; /**< The access address used for the connection. */
  uint16_t channel; /**< The logical channel used. */
  bool disableWhitening; /**< Indicates whether the whitening engine should be off. */
} RAIL_BLE_State_t;

/**
 * Configures RAIL to run in BLE mode.
 *
 * @param[in] railHandle A handle for RAIL instance.
 * This function will change your radio, channel configuration, and other
 * parameters to match what is needed for BLE. To switch back to a
 * default RAIL mode, call RAIL_BLE_Deinit() first. This function
 * will configure the protocol output on PTI to \ref RAIL_PTI_PROTOCOL_BLE.
 */
void RAIL_BLE_Init(RAIL_Handle_t railHandle);

/**
 * Takes RAIL out of BLE mode.
 *
 * @param[in] railHandle A handle for RAIL instance.
 * This function will undo some of the configuration that happens when you call
 * RAIL_BLE_Init(). After this you can safely run your normal radio
 * initialization code to use a non-BLE configuration. This function will \b
 * not change back your radio or channel configurations so you must do this by
 * manually reinitializing. This also resets the protocol output on PTI to \ref
 * RAIL_PTI_PROTOCOL_CUSTOM.
 */
void RAIL_BLE_Deinit(RAIL_Handle_t railHandle);

/**
 * Determines whether BLE mode is enabled or not.
 *
 * @param[in] railHandle A handle for RAIL instance.
 * @return True if BLE mode is enabled and false otherwise.
 * This function returns the current status of RAIL's BLE mode. It is enabled by
 * a call to RAIL_BLE_Init() and disabled by a call to RAIL_BLE_Deinit().
 */
bool RAIL_BLE_IsEnabled(RAIL_Handle_t railHandle);

/**
 * Switches the Viterbi 1 Mbps BLE PHY.
 *
 * @param[in] railHandle A handle for RAIL instance.
 * @return A status code indicating success of the function call.
 *
 * Use this function to switch back to the defualt BLE 1 Mbps PHY if you
 * have switched to the 2 Mbps or another configuration. You may only call this
 * function after initializing BLE and while the radio is idle.
 */
RAIL_Status_t RAIL_BLE_ConfigPhy1MbpsViterbi(RAIL_Handle_t railHandle);

/**
 * Switches the legacy non-Viterbi 1 Mbps BLE PHY.
 *
 * @param[in] railHandle A handle for RAIL instance.
 * @return A status code indicating success of the function call.
 *
 * You can use this function to switch back to the legacy BLE 1 Mbps PHY if you
 * have switched to the 2 Mbps or another configuration. You may only call this
 * function after initializing BLE and while the radio is idle.
 */
RAIL_Status_t RAIL_BLE_ConfigPhy1Mbps(RAIL_Handle_t railHandle);

/**
 * Switches the Viterbi 2 Mbps BLE PHY.
 *
 * @param[in] railHandle A handle for RAIL instance.
 * @return A status code indicating success of the function call.
 *
 * You can use this function to switch back to the BLE 2 Mbps PHY from the
 * default 1 Mbps option. You may only call this function after initializing BLE
 * and while the radio is idle.
 *
 * @note Not all chips support the 2 Mbps PHY. See the reference
 * manual before trying this.
 */
RAIL_Status_t RAIL_BLE_ConfigPhy2MbpsViterbi(RAIL_Handle_t railHandle);

/**
 * Switches the legacy non-Viterbi 2 Mbps BLE PHY.
 *
 * @param[in] railHandle A handle for RAIL instance.
 * @return A status code indicating success of the function call.
 *
 * Use this function to switch back to legacy BLE 2Mbps PHY from the
 * default 1 Mbps option. You may only call this function after initializing BLE
 * and while the radio is idle.
 *
 * @note Not all chips support the 2 Mbps PHY. See the reference
 * manual before trying this.
 */
RAIL_Status_t RAIL_BLE_ConfigPhy2Mbps(RAIL_Handle_t railHandle);

/**
 * Switches to the BLE Coded PHY.
 *
 * @param[in] railHandle A handle for RAIL instance.
 * @param[in] bleCoding The RAIL_BLE_Coding_t to use
 * @return A status code indicating success of the function call.
 *
 * Use this function to switch back to BLE Coded PHY from the default
 * 1 Mbps option. You may only call this function after initializing BLE and
 * while the radio is idle. When using a BLE Coded PHY, the subPhy in
 * RAIL_AppendedInfo_t marks the coding of the received packet. A subPhy of 0
 * marks a 500 kbps packet, and a subPhy of 1 marks a 125 kbps packet.
 *
 * @note Not all chips support the BLE Coded PHY. See the reference
 * manual before trying this.
 */
RAIL_Status_t RAIL_BLE_ConfigPhyCoded(RAIL_Handle_t railHandle,
                                      RAIL_BLE_Coding_t bleCoding);

/**
 * Helper function to change BLE radio parameters.
 *
 * @param[in] railHandle A handle for RAIL instance.
 * @param[in] crcInit The value to use for CRC initialization.
 * @param[in] accessAddress The access address to use for the connection.
 * @param[in] channel The logical channel that you're changing to, which is used to
 * initialize the whitener if used.
 * @param[in] disableWhitening This can turn off the whitening engine and is useful
 * for sending BLE test mode packets that don't have this turned on.
 * @return A status code indicating success of the function call.
 *
 * This function can be used to switch radio parameters on every connection
 * and/or channel change. It is BLE-aware and will set the access address,
 * preamble, CRC initialization value, and whitening configuration without
 * requiring you to load a new radio configuration.
 */
RAIL_Status_t RAIL_BLE_ConfigChannelRadioParams(RAIL_Handle_t railHandle,
                                                uint32_t crcInit,
                                                uint32_t accessAddress,
                                                uint16_t channel,
                                                bool disableWhitening);

/** @} */ // end of BLE

#ifdef __cplusplus
}
#endif

#endif // __RAIL_BLE_H__
