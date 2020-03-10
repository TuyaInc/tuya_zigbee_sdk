/***************************************************************************//**
 * @file btl_gbl_format.h
 * @brief Definitions for the Silicon Labs GBL format
 * @author Silicon Labs
 * @version 1.7.0
 *******************************************************************************
 * # License
 * <b>Copyright 2016 Silicon Laboratories, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/
#ifndef BTL_GBL_FORMAT_H
#define BTL_GBL_FORMAT_H

#include <stdint.h>
#include "api/application_properties.h"

/***************************************************************************//**
 * @addtogroup Plugin
 * @{
 * @addtogroup ImageParser Image Parser
 * @{
 * @addtogroup EblParser GBL Parser
 * @{
 * @addtogroup EblParserFormat GBL Format
 * @{
 ******************************************************************************/

// -------------------------------
// Structs

/// GBL tag header. Must be the first element in all GBL tags.
typedef struct {
  uint32_t  tagId;            ///< Tag ID
  uint32_t  length;           ///< Length (in bytes) of the rest of the tag
} EblTagHeader_t;

/// GBL header tag type.
typedef struct {
  EblTagHeader_t header;      ///< Tag ID and length
  uint32_t       version;     ///< Version of the GBL spec used in this file
  uint32_t       type;        ///< Type of GBL
} EblHeader_t;

/// GBL application tag type.
typedef struct {
  EblTagHeader_t    header;   ///< Tag ID and length
  ApplicationData_t appInfo;  ///< Information about the application
} EblApplication_t;

/// GBL bootloader tag type.
typedef struct {
  EblTagHeader_t header;             ///< Tag ID and length
  uint32_t       bootloaderVersion;  ///< Version number of the bootloader
  uint32_t       address;            ///< Address of the bootloader
  uint8_t        *data;              ///< Array of data for bootloader upgrade
} EblBootloader_t;

/// GBL metadata tag type.
typedef struct {
  EblTagHeader_t header;      ///< Tag ID and length
  uint8_t        *metaData;   ///< Array of metadata
} EblMetadata_t;

/// GBL flash program tag type.
typedef struct {
  EblTagHeader_t header;            ///< Tag ID and length
  uint32_t       flashStartAddress; ///< Address to start flashing
  uint8_t        *data;             ///< Array of data to flash
} EblProg_t;

/// GBL end tag type.
typedef struct {
  EblTagHeader_t header;      ///< Tag ID and length
  uint32_t       eblCrc;      ///< CRC32 of the entire GBL file.
} EblEnd_t;

/// GBL encryption header tag type.
typedef struct {
  EblTagHeader_t header;          ///< Tag ID and length
  uint32_t       version;         ///< Version of the GBL spec used in this file
  uint32_t       magicWord;       ///< Magic word (GBL_IMAGE_MAGIC_WORD)
  uint32_t       encryptionType;  ///< Type of encryption used. 1 = AES-CTR
} EblEncryptionHeader_t;

/// GBL encryption init tag type. Used with AES-CCM encryption.
typedef struct {
  EblTagHeader_t header;      ///< Tag ID and length
  uint32_t       msgLen;      ///< Length of the cipher text in bytes
  uint8_t        nonce[12];   ///< @brief Random nonce used for AES-CTR in
                              ///< this message
} EblEncryptionInitAesCcm_t;

/// GBL encryption data tag type.
typedef struct {
  EblTagHeader_t header;            ///< Tag ID and length
  uint8_t        *encryptedEblData; ///< @brief Encrypted data
                                    ///< @details After decryption, this data
                                    ///< must represent one or more complete
                                    ///< unencrypted GBL tags.
} EblEncryptionData_t;

/// @brief GBL encryption AES-CCM MAC tag type.
/// @deprecated This tag is deprecated. Use ECDSA instead for signature.
typedef struct {
  EblTagHeader_t header;      ///< Tag ID and length
  uint8_t        eblMac[16];  ///< AES-CCM MAC
} EblEncryptionAesCcmSignature_t;

/// GBL ECDSA secp256r1 signature tag type.
typedef struct {
  EblTagHeader_t header;  ///< Tag ID and length
  uint8_t        r[32];   ///< R-point of ECDSA secp256r1 signature
  uint8_t        s[32];   ///< S-point of ECDSA secp256r1 signature
} EblSignatureEcdsaP256_t;

/** @} addtogroup EblParserFormat */
/** @} addtogroup EblParser */
/** @} addtogroup ImageParser */
/** @} addtogroup Plugin */

#endif // BTL_GBL_FORMAT_H
