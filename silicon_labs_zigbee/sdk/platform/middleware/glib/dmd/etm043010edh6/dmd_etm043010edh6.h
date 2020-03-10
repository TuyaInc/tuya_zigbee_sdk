/*************************************************************************//**
 * @file dmd_etm043010edh6.h
 * @brief Dot matrix display driver for DISPLAY device driver interface.
 * @version 5.6.0
 ******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/

#ifndef __DMD_ETM043010EDH6_H__
#define __DMD_ETM043010EDH6_H__

#include "emstatus.h"

#define DMD_HORIZONTAL_SIZE         (480)
#define DMD_VERTICAL_SIZE           (272)
#define DMD_DISPLAY_BACKLIGHT_PORT  (gpioPortI)
#define DMD_DISPLAY_BACKLIGHT_PIN   (6)

EMSTATUS DMD_startDrawing(void);
EMSTATUS DMD_stopDrawing(void);

#endif /* __DMD_ETM043010EDH6_H__ */
