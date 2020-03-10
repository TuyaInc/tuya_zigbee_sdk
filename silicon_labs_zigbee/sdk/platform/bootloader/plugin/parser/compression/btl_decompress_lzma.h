/***************************************************************************//**
 * @file btl_decompress_lzma.h
 * @brief LZMA decompression functionality for Gecko Bootloader
 * @author Silicon Labs
 * @version 1.7.0
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/
#ifndef BTL_DECOMPRESS_LZMA_H
#define BTL_DECOMPRESS_LZMA_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "plugin/parser/ebl/btl_ebl_parser.h"
#include "api/btl_errorcode.h"

#include "lzma/LzmaDec.h"

/**
 * @addtogroup Plugin
 * @{
 * @addtogroup ImageParser
 * @{
 * @addtogroup EblParser
 * @{
 * @addtogroup CustomTags Custom GBL Tags
 * @{
 * @addtogroup LzmaProgTag LZMA Programming Tag
 * @brief Tag to handle LZMA compressed programming data
 * @details
 * @{
 */

#ifndef LZMA_COUNTER_SIZE_KB
/// @brief Maximum size of array holding probability model counters.
/// The size given here sets the limit for the size of the lc and lp constants
/// used by the LZMA compressor. The necessary size of the counter array can be
/// found from size = 4 KiB + 1.5 KiB * (1 << (lc + lp)).
/// LZMA payloads with too large lc+lp can not be decompressed.
#define LZMA_COUNTER_SIZE_KB        (10UL)
#endif

#ifndef LZMA_DICT_SIZE_KB
/// @brief Maximum size of dictionary.
/// The size given here sets the limit for the size of the dictionary used by
/// the LZMA compressor.
/// LZMA payloads with too large dictionary can not be decompressed.
#define LZMA_DICT_SIZE_KB           (8UL)
#endif

#ifndef LZMA_SUPPORT_NO_END_MARK
/// @brief Support not using an end mark in the LZMA compressed file
/// Not having an end mark in the LZMA file requires more bookkeeping
/// during decompression. Disable this option to save some code space.
#define LZMA_SUPPORT_NO_END_MARK    (0)
#endif

/***************************************************************************//**
 * Enter an LZMA compressed programming tag
 * @param ctx Parser context
 *
 * @return Error code
 ******************************************************************************/
int32_t gbl_lzmaEnterProgTag(ParserContext_t *ctx);

/***************************************************************************//**
 * Parse a chunk of data from an LZMA compressed programming tag
 * @param ctx       Parser context
 * @param data      Input data to parse
 * @param length    Length of data
 * @param callbacks Callbacks to call with parsed data
 *
 * @return Error code
 ******************************************************************************/
int32_t gbl_lzmaParseProgTag(ParserContext_t                   *ctx,
                             void                              *data,
                             size_t                            length,
                             const BootloaderParserCallbacks_t *callbacks);

/***************************************************************************//**
 * Exit an LZMA compressed programming tag
 * @param ctx       Parser context
 * @param callbacks Callbacks to call with parsed data
 *
 * @return Error code
 ******************************************************************************/
int32_t gbl_lzmaExitProgTag(ParserContext_t                   *ctx,
                            const BootloaderParserCallbacks_t *callbacks);

/***************************************************************************//**
 * Number of bytes needed for next stage of parsing
 * @param ctx Parser context
 *
 * @return Number of bytes required
 ******************************************************************************/
size_t gbl_lzmaNumBytesRequired(ParserContext_t *ctx);

/** @} addtogroup LzmaProgTag */
/** @} addtogroup CustomTags */
/** @} addtogroup EblParser */
/** @} addtogroup ImageParser */
/** @} addtogroup Plugin */

#endif // BTL_DECOMPRESS_LZMA_H
