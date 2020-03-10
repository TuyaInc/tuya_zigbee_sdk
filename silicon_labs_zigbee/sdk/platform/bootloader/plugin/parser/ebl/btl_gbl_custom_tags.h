/***************************************************************************//**
 * @file btl_gbl_custom_tags.h
 * @brief Custom tags for Gecko Bootloader GBL parser
 * @author Silicon Labs
 * @version 1.7.0
 *******************************************************************************
 * # License
 * <b>Copyright 2017 Silicon Laboratories, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/
#ifndef BTL_GBL_CUSTOM_TAGS_H
#define BTL_GBL_CUSTOM_TAGS_H

#include "btl_ebl_parser.h"
#include "btl_gbl_format.h"

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * @addtogroup Plugin
 * @{
 * @addtogroup ImageParser
 * @{
 * @addtogroup EblParser
 * @{
 * @addtogroup CustomTags Custom GBL Tags
 * @brief Handle custom GBL tags for added functionality in the GBL parser
 * @details
 * @{
 */

/// Custom tag descriptor
typedef struct {
  /// GBL Tag ID of the custom tag
  uint32_t tagId;
  /// Function to call upon entering the tag
  int32_t (*enterTag)(ParserContext_t *ctx);
  /// Function to call while parsing the tag
  int32_t (*parseTag)(ParserContext_t *ctx,
                      void *data,
                      size_t length,
                      const BootloaderParserCallbacks_t *callbacks);
  /// Function to call upon exiting the tag
  int32_t (*exitTag)(ParserContext_t *ctx,
                     const BootloaderParserCallbacks_t *callbacks);
  /// @brief Function returning how many bytes should be collected before
  ///        calling parseTag the next time
  size_t (*numBytesRequired)(ParserContext_t *ctx);
} GblCustomTag_t;

/***************************************************************************//**
 * Whether the GBL tag described by tagHeader is a custom tag
 * @param tagHeader Pointer to the GBL tag header
 *
 * @return True if the tag is a custom tag, else false
 ******************************************************************************/
bool gbl_isCustomTag(EblTagHeader_t *tagHeader);

/***************************************************************************//**
 * Get properties for a custom GBL tag
 * @param tagId GBL Tag ID of the custom tag
 *
 * @return Pointer to the custom tag descriptor
 ******************************************************************************/
const GblCustomTag_t * gbl_getCustomTagProperties(uint32_t tagId);

/** @} addtogroup CustomTags */
/** @} addtogroup EblParser */
/** @} addtogroup ImageParser */
/** @} addtogroup Plugin */

#endif // BTL_GBL_CUSTOM_TAGS_H
