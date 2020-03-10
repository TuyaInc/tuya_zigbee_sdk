/**************************************************************************//**
 * @file rail_features.h
 * @brief Auxiliary header for the RAIL library. Includes consistent definitions
 *        of features available across different chips.
 * @copyright Copyright 2018 Silicon Laboratories, Inc. www.silabs.com
 *****************************************************************************/

#ifndef __RAIL_FEATURES_H__
#define __RAIL_FEATURES_H__

#ifdef __cplusplus
extern "C" {
#endif

// Radio capabilities
// Provides a consistent define to know whether a device supports the SubGHz, 2P4GHZ or
// both bands
#define RAIL_FEAT_DUAL_BAND_RADIO                                                                            \
  ((_SILICON_LABS_32B_SERIES == 1) && (_SILICON_LABS_EFR32_RADIO_TYPE == _SILICON_LABS_EFR32_RADIO_DUALBAND) \
   || ((FEAT_RF_2G4 == 1) && (FEAT_RF_SUBG == 1)))

#define RAIL_FEAT_SUBGIG_RADIO (((_SILICON_LABS_32B_SERIES == 1)                                               \
                                 && ((_SILICON_LABS_EFR32_RADIO_TYPE == _SILICON_LABS_EFR32_RADIO_DUALBAND)    \
                                     || (_SILICON_LABS_EFR32_RADIO_TYPE == _SILICON_LABS_EFR32_RADIO_SUBGHZ))) \
                                || (FEAT_RF_SUBG == 1))

#define RAIL_FEAT_2G4_RADIO (((_SILICON_LABS_32B_SERIES == 1)                                              \
                              && ((_SILICON_LABS_EFR32_RADIO_TYPE == _SILICON_LABS_EFR32_RADIO_DUALBAND)   \
                                  || (_SILICON_LABS_EFR32_RADIO_TYPE == _SILICON_LABS_EFR32_RADIO_2G4HZ))) \
                             || (FEAT_RF_2G4 == 1))

// Antenna Diversity
#define RAIL_FEAT_ANTENNA_DIVERSITY ((_SILICON_LABS_32B_SERIES_1_CONFIG >= 2) \
                                     || (_SILICON_LABS_32B_SERIES == 2))

#ifdef __cplusplus
}
#endif

#endif // __RAIL_FEATURES_H__
