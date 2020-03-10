/***************************************************************************//**
 * @file btl_ebl_parser.h
 * @brief EBL image file parser.
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
#ifndef BTL_EBL_PARSER_H
#define BTL_EBL_PARSER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "plugin/parser/btl_image_parser.h"

#include "plugin/security/btl_security_aes.h"
#include "plugin/security/btl_security_sha256.h"

/***************************************************************************//**
 * @addtogroup Plugin
 * @{
 * @addtogroup ImageParser Image Parser
 * @{
 * @addtogroup EblParser GBL Parser
 * @{
 * @brief GBL parser implementation.
 * @details
 *   Image parser for GBL files. Parses GBL files based on the
 *   [GBL file format specification](@ref EblParserFormat). Callbacks are used
 *   to present data and metadata contents of the GBL file to the bootloader.
 ******************************************************************************/

/// GBL file is encrypted
#define PARSER_FLAG_ENCRYPTED               (1U << 0U)
/// File is an EBL (version 2)
#define PARSER_FLAG_IS_EBLV2                (1U << 3U)
/// Parse custom tags rather than silently traversing them
#define PARSER_FLAG_PARSE_CUSTOM_TAGS       (1U << 5U)

/// Some flags are public, some are internal to the parser
#define PARSER_FLAGS_PUBLIC_MASK            (PARSER_FLAG_PARSE_CUSTOM_TAGS)

/// State in the EBL parser state machine
typedef enum {
  EblParserStateInit,                 ///< Initial state
  EblParserStateIdle,                 ///< Idle state
  EblParserStateHeader,               ///< Parsing header tag
  EblParserStateHeaderV2,             ///< Parsing V2 header tag
  EblParserStateBootloader,           ///< Parsing bootloader tag
  EblParserStateBootloaderData,       ///< Parsing bootloader tag data
  EblParserStateApplication,          ///< Parsing application tag
  EblParserStateMetadata,             ///< Parsing metadata tag
  EblParserStateMetadataData,         ///< Parsing metadata tag data
  EblParserStateProg,                 ///< Parsing flash program tag
  EblParserStateProgData,             ///< Parsing flash program tag data
  EblParserStateEraseProg,            ///< Parsing flash erase&program tag
  EblParserStateFinalize,             ///< Finalizing file
  EblParserStateDone,                 ///< Parsing complete
  EblParserStateEncryptionInit,       ///< Parsing encryption init tag
  EblParserStateEncryptionContainer,  ///< Parsing encryption data tag
  EblParserStateSignature,            ///< Parsing signature tag
  EblParserStateCustomTag,            ///< Parsing custom tag
  EblParserStateError                 ///< Error state
} EblParserState_t;

/// Image parser context definition
typedef struct {
  /// Buffer contents
  uint8_t             internalBuffer[64];
  /// Amount of bytes present in buffer
  uint8_t             bytesInInternalBuffer;
  /// Current reading offset into the buffer (circular)
  uint8_t             internalBufferOffset;
  /// Parser flags
  uint8_t             flags;
  /// Parser is currently inside an encrypted tag
  bool                inEncryptedContainer;
  /// Parser has received and verified signature
  bool                gotSignature;
  /// Parser has received bootloader upgrade tag
  bool                gotBootloader;
  /// State of the EBL parser state machine
  EblParserState_t    internalState;
  /// AES-CCM decryption (= AES-CTR) context
  void                *aesContext;
  /// SHA256 hashing context
  void                *shaContext;
  /// Total length of the tag currently being parsed
  size_t              lengthOfTag;
  /// Current offset into tag being parsed
  size_t              offsetInTag;
  /// Total length of current encrypted data block
  size_t              lengthOfEncryptedTag;
  /// Offset into current encrypted data block
  size_t              offsetInEncryptedTag;
  /// Current address the image needs to be written to
  uint32_t            programmingAddress;
  /// Current offset of metadata/bootloader being handled (starts at 0)
  uint32_t            tagAddress;
  /// Withheld application data
  uint8_t             withheldApplicationVectors[24];
  /// Withheld bootloader upgrade data during app parsing
  uint8_t             withheldUpgradeVectors[4];
  /// Withheld bootloader upgrade data during bootloader parsing
  uint8_t             withheldBootloaderVectors[4];
  /// Running CRC-32 over the incoming EBL file
  uint32_t            fileCrc;
  /// Context for custom tag
  uint32_t            customTagId;
} ParserContext_t;

/***************************************************************************//**
 * Write application data. This function is called when parsing any tag with
 * @ref EblProg_t structured content.
 *
 * @param context     GBL parser context
 * @param buffer      Input buffer containing data to be written
 * @param length      Size of input buffer
 * @param callbacks   GBL Parser callbacks for writing data
 *
 * @return Error code
 ******************************************************************************/
int32_t gbl_writeProgData(ParserContext_t *context,
                          uint8_t buffer[],
                          size_t length,
                          const BootloaderParserCallbacks_t *callbacks);

/** @} addtogroup EblParser */
/** @} addtogroup ImageParser */
/** @} addtogroup Plugin */

#endif // BTL_EBL_PARSER_H
