/***************************************************************************//**
 * @file rail_zwave.h
 * @brief The Z-Wave specific header file for the RAIL library.
 * @copyright Copyright 2018 Silicon Laboratories, Inc. www.silabs.com
 ******************************************************************************/

#ifndef __RAIL_ZWAVE_H__
#define __RAIL_ZWAVE_H__

#include "rail_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Z_Wave Z-Wave
/// @ingroup Protocol_Specific
/// @brief Z-Wave configuration routines
///
/// The functions in this group configure RAIL Z-Wave hardware
/// acceleration features.
///
/// To configure Z-Wave functionality, the application must first set up
/// a RAIL instance with RAIL_Init() and other setup functions.
///
/// @code{.c}
///
/// // Main RAIL_EVENT callback
/// static void RAILCb_Event(RAIL_Handle_t railHandle, RAIL_Events_t events)
/// {
///   // Get beamNodeId from Beam Packet
///   if (events & RAIL_EVENT_ZWAVE_BEAM) {
///     if (RAIL_ZWAVE_IsEnabled(railHandle)) {
///       RAIL_ZWAVE_NodeId_t beamNodeId;
///       if (RAIL_ZWAVE_GetBeamNodeId(railHandle, &gRecentBeamNode)
///           != RAIL_STATUS_NO_ERROR) {
///         return;
///       }
///     }
///   }
/// }
///
/// static const RAIL_ZWAVE_Config_t zwaveConfig = {
///   .options = RAIL_ZWAVE_OPTIONS_DEFAULT
/// };
///
/// RAIL_Status_t zwaveInit(void)
/// {
///   // initialize Z-Wave
///   RAIL_Status_t status = RAIL_ZWAVE_Init(railHandle, &zwaveConfig);
///
///   if (status != RAIL_STATUS_NO_ERROR) {
///     return status;
///   }
///
///   uint8_t myHomeId[4] = { 0xDE, 0xAD, 0xBE, 0xEF };
///   RAIL_ZWAVE_SetNodeId(railHandle, RAIL_ZWAVE_NODE_ID_DEFAULT);
///   RAIL_ZWAVE_SetHomeId(railHandle, myHomeId, RAIL_ZWAVE_HOME_ID_HASH_DONT_CARE);
///
///   // configure region to EU(European Union)
///   return RAIL_ZWAVE_ConfigRegion(railHandle, RAIL_ZWAVE_REGION_EU);
/// }
/// @endcode
///
/// @{

/**
 * @enum RAIL_ZWAVE_Options_t
 * @brief Z-Wave options.
 */
RAIL_ENUM_GENERIC(RAIL_ZWAVE_Options_t, uint32_t) {
  // Z-Wave Option Bit Shifts

  /** Shift position of \ref RAIL_ZWAVE_OPTION_PROMISCUOUS_MODE bit */
  RAIL_ZWAVE_OPTION_PROMISCUOUS_MODE_SHIFT = 0,
  /** Shift position of \ref RAIL_ZWAVE_OPTION_DETECT_BEAM_FRAMES bit */
  RAIL_ZWAVE_OPTION_DETECT_BEAM_FRAMES_SHIFT,
};

// RAIL_ZWAVE_Options_t bitmasks

/** A value representing no options */
#define RAIL_ZWAVE_OPTIONS_NONE 0U

/** All options are disabled by default. */
#define RAIL_ZWAVE_OPTIONS_DEFAULT RAIL_ZWAVE_OPTIONS_NONE

/**
 * An option to configure promiscuous mode, accepting packets regardless
 * of HomeId. Defaults to false, filtering packets based on the HomeId.
 * When true, such filtering is disabled.
 */
#define RAIL_ZWAVE_OPTION_PROMISCUOUS_MODE \
  (1u << RAIL_ZWAVE_OPTION_PROMISCUOUS_MODE_SHIFT)
/**
 * An option to configure Beam frame recognition. Defaults to false.
 * When true, Beam frames that are broadcast or match the NodeId and
 * HomeIdHash values will trigger \ref RAIL_EVENT_ZWAVE_BEAM event,
 * in addition to \ref RAIL_EVENT_RX_PACKET_ABORTED which occurs for
 * every received Beam frame.
 *
 * @note This option takes precedence over \ref
 *   RAIL_ZWAVE_OPTION_PROMISCUOUS_MODE when receiving a beam frame.
 *   If this option is false, beam frames are not considered special
 *   and will be received as if they were normal Z-Wave frames, assuredly
 *   triggering \ref RAIL_EVENT_RX_FRAME_ERROR.
 */
#define RAIL_ZWAVE_OPTION_DETECT_BEAM_FRAMES \
  (1u << RAIL_ZWAVE_OPTION_DETECT_BEAM_FRAMES_SHIFT)

/** A value representing all options */
#define RAIL_ZWAVE_OPTIONS_ALL 0xFFFFFFFFU

/**
 * @enum RAIL_ZWAVE_NodeId_t
 * @brief A Z-Wave Node Id.
 *
 * @note Values 0xE9..0xFE are reserved.
 */
RAIL_ENUM(RAIL_ZWAVE_NodeId_t) {
  RAIL_ZWAVE_NODE_ID_NONE = 0x00U, /**< The unknown NodeId for uninitialized nodes. */
  RAIL_ZWAVE_NODE_ID_BROADCAST = 0xFFU, /**< The broadcast NodeId. */
  RAIL_ZWAVE_NODE_ID_DEFAULT
    = RAIL_ZWAVE_NODE_ID_BROADCAST, /**< Default to the broadcast NodeId. */
  // All other values between 0x00 and 0xFE are valid nodeId's.
};

/**
 * @enum RAIL_ZWAVE_HomeId_t
 * @brief A Z-Wave Home Id.
 *
 * @note Home Id's in the range 0x54000000..0x55FFFFFF are illegal.
 */
RAIL_ENUM_GENERIC(RAIL_ZWAVE_HomeId_t, uint32_t) {
  RAIL_ZWAVE_HOME_ID_UNKNOWN = 0x00000000U, /**< The unknown HomeId. */
  RAIL_ZWAVE_HOME_ID_DEFAULT = 0x54545454U, /**< An impossible and unlikely HomeId. */
};

/**
 * @enum RAIL_ZWAVE_HomeIdHash_t
 * @brief A Z-Wave Home Id hash.
 *
 * @note Certain values (as shown) are illegal.
 */
RAIL_ENUM(RAIL_ZWAVE_HomeIdHash_t) {
  RAIL_ZWAVE_HOME_ID_HASH_ILLEGAL_1 = 0x0AU, /**< An illegal HomeIdHash value. */
  RAIL_ZWAVE_HOME_ID_HASH_ILLEGAL_2 = 0x4AU, /**< An illegal HomeIdHash value. */
  RAIL_ZWAVE_HOME_ID_HASH_ILLEGAL_3 = 0x55U, /**< An illegal HomeIdHash value. */
  RAIL_ZWAVE_HOME_ID_HASH_DONT_CARE = 0x55U, /**< Illegal HomeIdHash value that
                                                  suppresses checking the
                                                  HomeIdHash field of Beam
                                                  packets. */
  RAIL_ZWAVE_HOME_ID_HASH_DEFAULT
    = RAIL_ZWAVE_HOME_ID_HASH_DONT_CARE, /**< Default to don't care. */
};

/**
 * @struct RAIL_ZWAVE_Config_t
 * @brief A configuration structure for Z-Wave in RAIL.
 */
typedef struct RAIL_ZWAVE_Config {
  RAIL_ZWAVE_Options_t options; /**< Z-Wave options. */
} RAIL_ZWAVE_Config_t;

/**
 * @enum RAIL_ZWAVE_Baud_t
 * @brief Z-Wave supported baudrates.
 */
RAIL_ENUM(RAIL_ZWAVE_Baud_t) {
  RAIL_ZWAVE_BAUD_9600, /**< 9.6kbps baudrate*/
  RAIL_ZWAVE_BAUD_40K,  /**< 40kbps baudrate*/
  RAIL_ZWAVE_BAUD_100K  /**< 100kbps baudrate*/
};

/**
 * @struct RAIL_ZWAVE_ChannelConfig_t
 * @brief A configuration structure for Z-Wave channels.
 */
typedef struct RAIL_ZWAVE_ChannelConfig {
  uint32_t frequency;         /**< Channel frequency in hertz*/
  RAIL_ZWAVE_Baud_t baudRate; /**< Channel baud rate index*/
  RAIL_TxPower_t maxPower;    /**< The maximum power allowed on this channel*/
} RAIL_ZWAVE_ChannelConfig_t;

/**
 * @typedef RAIL_ZWAVE_RegionConfig_t
 * @brief Each Z-Wave region supports 3 channels.
 */
typedef RAIL_ZWAVE_ChannelConfig_t RAIL_ZWAVE_RegionConfig_t[3];

/**
 * Switch the Z-Wave region.
 *
 * @param[in] railHandle A handle of RAIL instance.
 * @param[in] regionCfg Z-Wave channel configuration for the selected region
 * @return Status code indicating success of the function call.
 */
RAIL_Status_t RAIL_ZWAVE_ConfigRegion(RAIL_Handle_t railHandle,
                                      const RAIL_ZWAVE_RegionConfig_t regionCfg);

/**
 * Initializes RAIL for Z-Wave features.
 *
 * @param[in] railHandle A handle of RAIL instance.
 * @param[in] config A Z-Wave configuration structure.
 * @return A status code indicating success of the function call.
 *
 * This function is the entry point for working with ZWave within
 * RAIL. It will set up relevant hardware acceleration for Z-Wave
 * specific features like HomeId filtering and beam packets (as
 * specified in the config) and allows users to select the
 * relevant Z-Wave region-specific phy via \ref RAIL_ZWAVE_ConfigRegion.
 */
RAIL_Status_t RAIL_ZWAVE_Init(RAIL_Handle_t railHandle,
                              const RAIL_ZWAVE_Config_t *config);

/**
 * De-initializes Z-Wave hardware acceleration.
 *
 * @param[in] railHandle A handle of RAIL instance.
 * @return A status code indicating success of the function call.
 *
 * Disables and resets all Z-Wave hardware acceleration features. This
 * function should only be called when the radio is IDLE.
 */
RAIL_Status_t RAIL_ZWAVE_Deinit(RAIL_Handle_t railHandle);

/**
 * Returns whether Z-Wave hardware acceleration is currently enabled.
 *
 * @param[in] railHandle A handle of RAIL instance.
 * @return True if Z-Wave hardware acceleration was enabled to start with
 * and false otherwise.
 */
bool RAIL_ZWAVE_IsEnabled(RAIL_Handle_t railHandle);

/**
 * Configures Z-Wave options
 *
 * @param[in] railHandle A handle of RAIL instance.
 * @param[in] mask A bitmask containing which options should be modified.
 * @param[in] options A bitmask containing desired configuration settings.
 *   Bit positions for each option are found in the \ref RAIL_ZWAVE_Options_t.
 * @return Status code indicating success of the function call.
 */
RAIL_Status_t RAIL_ZWAVE_ConfigOptions(RAIL_Handle_t railHandle,
                                       RAIL_ZWAVE_Options_t mask,
                                       RAIL_ZWAVE_Options_t options);

/**
 * Informs RAIL of the Z-Wave node's NodeId for receive filtering purposes.
 *
 * @param[in] railHandle A handle of RAIL instance.
 * @param[in] nodeId A Z-Wave Node Id.
 * @return Status code indicating success of the function call.
 *
 * @note Until this API is called, RAIL will assume the NodeId is
 *   \ref RAIL_ZWAVE_NODE_ID_DEFAULT.
 */
RAIL_Status_t RAIL_ZWAVE_SetNodeId(RAIL_Handle_t railHandle,
                                   RAIL_ZWAVE_NodeId_t nodeId);

/**
 * Informs RAIL of the Z-Wave node's HomeId and its hash for receive filtering
 * purposes.
 *
 * @param[in] railHandle A handle of RAIL instance.
 * @param[in] homeId A Z-Wave Home Id.
 * @param[in] homeIdHash The hash of the Home Id expected in Beam frames.
 *   If this is \ref RAIL_ZWAVE_HOME_ID_HASH_DONT_CARE, Beam frame detection
 *   will not check the HomeIdHash in a received Beam frame at all, and
 *   \ref RAIL_EVENT_ZWAVE_BEAM will trigger based solely on the nodeId
 *   in the Beam frame.
 * @return Status code indicating success of the function call.
 *
 * @note Until this API is called, RAIL will assume the HomeId is an
 *   illegal one of \ref RAIL_ZWAVE_HOME_ID_DEFAULT, and its hash is \ref
 *   RAIL_ZWAVE_HOME_ID_HASH_DONT_CARE.
 */
RAIL_Status_t RAIL_ZWAVE_SetHomeId(RAIL_Handle_t railHandle,
                                   RAIL_ZWAVE_HomeId_t homeId,
                                   RAIL_ZWAVE_HomeIdHash_t homeIdHash);

/**
 * Gets the NodeId of the most recently seen Beam frame that targeted this
 * node.
 *
 * @param[in] railHandle A handle of RAIL instance.
 * @param[out] pNodeId A pointer to \ref RAIL_ZWAVE_NodeId_t to populate.
 * @return RAIL_STATUS_NO_ERROR if called while handling the \ref
 *   RAIL_EVENT_ZWAVE_BEAM event, otherwise RAIL_STATUS_INVALID_STATE
 *   (in which case pNodeId will not be populated).
 */
RAIL_Status_t RAIL_ZWAVE_GetBeamNodeId(RAIL_Handle_t railHandle,
                                       RAIL_ZWAVE_NodeId_t *pNodeId);

/** EU-European Union", RAIL_ZWAVE_REGION_EU */
extern const RAIL_ZWAVE_RegionConfig_t RAIL_ZWAVE_REGION_EU;

/** US-United States", RAIL_ZWAVE_REGION_US */
extern const RAIL_ZWAVE_RegionConfig_t RAIL_ZWAVE_REGION_US;

/** ANZ-Australia/New Zealand", RAIL_ZWAVE_REGION_ANZ */
extern const RAIL_ZWAVE_RegionConfig_t RAIL_ZWAVE_REGION_ANZ;

/** HK-Hong Kong", RAIL_ZWAVE_REGION_HK */
extern const RAIL_ZWAVE_RegionConfig_t RAIL_ZWAVE_REGION_HK;

/** MY-Malaysia", RAIL_ZWAVE_REGION_MY */
extern const RAIL_ZWAVE_RegionConfig_t RAIL_ZWAVE_REGION_MY;

/** IN-India", RAIL_ZWAVE_REGION_IN */
extern const RAIL_ZWAVE_RegionConfig_t RAIL_ZWAVE_REGION_IN;

/** JP-Japan", RAIL_ZWAVE_REGION_JP */
extern const RAIL_ZWAVE_RegionConfig_t RAIL_ZWAVE_REGION_JP;

/** RU-Russia", RAIL_ZWAVE_REGION_RU */
extern const RAIL_ZWAVE_RegionConfig_t RAIL_ZWAVE_REGION_RU;

/** IL-Israel", RAIL_ZWAVE_REGION_IL */
extern const RAIL_ZWAVE_RegionConfig_t RAIL_ZWAVE_REGION_IL;

/** KR-Korea", RAIL_ZWAVE_REGION_KR */
extern const RAIL_ZWAVE_RegionConfig_t RAIL_ZWAVE_REGION_KR;

/** CN-China", RAIL_ZWAVE_REGION_CN */
extern const RAIL_ZWAVE_RegionConfig_t RAIL_ZWAVE_REGION_CN;

/** @} */ // end of Z_Wave

#ifdef __cplusplus
}
#endif

#endif // __RAIL_ZWAVE_H__
