/*************************************************************************//**
 * @file glib_bitmap.c
 * @brief Silicon Labs Graphics Library: Bitmap Drawing Routines
 ******************************************************************************
 * # License
 * <b>Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/

/* Standard C header files */
#include <stdint.h>

/* EM types */
#include "em_types.h"

/* GLIB header files */
#include "glib.h"

/**************************************************************************//**
*  @brief
*  Draws a bitmap
*
*  Sets up a bitmap that starts at x0,y0 and draws bitmap.
*
*  For monochrome displays, each 8-bit element contains 8 pixels values.
*
*  For 3-bit RGB displays, each bit in the array are one color component (red,
*  green and blue) of the pixel, so that 3 bits represent one pixel (0xBGR).
*  Pixel 0: Bits 2:0 (0bBGR) of byte 0
*  Pixel 1: Bits 5:3 (0bBGR) of byte 0
*  Pixel 2: Bits 7:6 (0bGR) of byte 0 and bit 0 (0bB) of byte 1
*  Pixel 3: Bits 3:1 (0bBGR) of byte 1
*  ...
*
*  For RGB displays with 8-bits per color, each pixel is represented by 24-bits,
*  with one byte for each of the red, green and blue components. The data has to
*  be organized like this: picData = { R, G, B, R, G, B, R, G, B ... }
*
*  The pixels are ordered by increasing x coordinate, after the last pixel of a
*  row, the next pixel will be the first pixel on the next row.
*
*  @param pContext
*  Pointer to a GLIB_Context_t in which the bitmap is drawn.
*  @param x
*  Start x-coordinate for bitmap
*  @param y
*  Start y-coordinate for bitmap
*  @param width
*  Width of picture
*  @param height
*  Height of picture
*  @param picData
*  Bitmap data
*
*  @return
*  Returns GLIB_OK on success, or else error code
******************************************************************************/
EMSTATUS GLIB_drawBitmap(GLIB_Context_t *pContext, int32_t x, int32_t y,
                         uint32_t width, uint32_t height, const uint8_t *picData)
{
  EMSTATUS status;

  /* Set display clipping area for bitmap */
  status = DMD_setClippingArea(x, y, width, height);
  if (status != DMD_OK) {
    return status;
  }

  /* Write bitmap to display */
  status = DMD_writeData(0, 0, picData, width * height);
  if (status != DMD_OK) {
    return status;
  }

  /* Reset driver clipping area to GLIB clipping region */
  return GLIB_applyClippingRegion(pContext);
}
