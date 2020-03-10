/***************************************************************************//**
 * @file btl_gbl_custom_tags.c
 * @brief Custom tags for Gecko Bootloader
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
#include "btl_gbl_custom_tags.h"

#if defined(BTL_PARSER_SUPPORT_LZ4)
#include "plugin/parser/compression/btl_decompress_lz4.h"
#endif

#if defined(BTL_PARSER_SUPPORT_LZMA)
#include "plugin/parser/compression/btl_decompress_lzma.h"
#endif

#if defined(BTL_PARSER_SUPPORT_LZ4) || defined(BTL_PARSER_SUPPORT_LZMA)
// At least one custom tag is supported
#define HAVE_CUSTOM_TAGS
#endif

#if defined(HAVE_CUSTOM_TAGS)
const GblCustomTag_t gblCustomTags[] = {
#if defined(BTL_PARSER_SUPPORT_LZ4)
  {
    .tagId = 0xFD0505FDUL,
    .enterTag = gbl_lz4EnterProgTag,
    .parseTag = gbl_lz4ParseProgTag,
    .exitTag = gbl_lz4ExitProgTag,
    .numBytesRequired = gbl_lz4NumBytesRequired
  },
#endif
#if defined(BTL_PARSER_SUPPORT_LZMA)
  {
    .tagId = 0xFD0707FDUL,
    .enterTag = gbl_lzmaEnterProgTag,
    .parseTag = gbl_lzmaParseProgTag,
    .exitTag = gbl_lzmaExitProgTag,
    .numBytesRequired = gbl_lzmaNumBytesRequired
  },
#endif
};
#endif

bool gbl_isCustomTag(EblTagHeader_t *tagHeader)
{
#if defined(HAVE_CUSTOM_TAGS)
  for (size_t i = 0; i < sizeof(gblCustomTags) / sizeof(GblCustomTag_t); i++) {
    if (gblCustomTags[i].tagId == tagHeader->tagId) {
      return true;
    }
  }
#else
  (void) tagHeader;
#endif
  return false;
}

const GblCustomTag_t * gbl_getCustomTagProperties(uint32_t tagId)
{
#if defined(HAVE_CUSTOM_TAGS)
  for (size_t i = 0; i < sizeof(gblCustomTags) / sizeof(GblCustomTag_t); i++) {
    if (gblCustomTags[i].tagId == tagId) {
      return &gblCustomTags[i];
    }
  }
#else
  (void) tagId;
#endif
  return NULL;
}
