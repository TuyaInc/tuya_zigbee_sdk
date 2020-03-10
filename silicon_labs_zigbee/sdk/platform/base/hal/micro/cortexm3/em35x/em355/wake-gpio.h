/** @file hal/micro/cortexm3/em35x/em355/wake-gpio.h
 *
 * @brief
 * Condense WAKE_ON_P* macros into port-specific and monolithic macros.
 *
 * THIS IS A GENERATED FILE.  DO NOT EDIT.
 *
 * <!-- Copyright 2017 Silicon Laboratories, Inc.                        *80*-->
 */

#ifndef __WAKE_GPIO_H__
#define __WAKE_GPIO_H__

// create some handy macros for referring to whole ports
#define EMBER_WAKE_PORT_A                  \
  ((WAKE_ON_PA0 << _GPIO_P_IN_Px0_SHIFT)   \
   | (WAKE_ON_PA1 << _GPIO_P_IN_Px1_SHIFT) \
   | (WAKE_ON_PA2 << _GPIO_P_IN_Px2_SHIFT) \
   | (WAKE_ON_PA3 << _GPIO_P_IN_Px3_SHIFT) \
   | (WAKE_ON_PA4 << _GPIO_P_IN_Px4_SHIFT) \
   | (WAKE_ON_PA5 << _GPIO_P_IN_Px5_SHIFT) \
   | (WAKE_ON_PA7 << _GPIO_P_IN_Px7_SHIFT) \
  )

#define EMBER_WAKE_PORT_B                  \
  ((WAKE_ON_PB0 << _GPIO_P_IN_Px0_SHIFT)   \
   | (WAKE_ON_PB1 << _GPIO_P_IN_Px1_SHIFT) \
   | (WAKE_ON_PB2 << _GPIO_P_IN_Px2_SHIFT) \
   | (WAKE_ON_PB3 << _GPIO_P_IN_Px3_SHIFT) \
   | (WAKE_ON_PB4 << _GPIO_P_IN_Px4_SHIFT) \
   | (WAKE_ON_PB7 << _GPIO_P_IN_Px7_SHIFT) \
  )

#define EMBER_WAKE_PORT_C                  \
  ((WAKE_ON_PC0 << _GPIO_P_IN_Px0_SHIFT)   \
   | (WAKE_ON_PC1 << _GPIO_P_IN_Px1_SHIFT) \
   | (WAKE_ON_PC2 << _GPIO_P_IN_Px2_SHIFT) \
   | (WAKE_ON_PC3 << _GPIO_P_IN_Px3_SHIFT) \
   | (WAKE_ON_PC4 << _GPIO_P_IN_Px4_SHIFT) \
   | (WAKE_ON_PC5 << _GPIO_P_IN_Px5_SHIFT) \
   | (WAKE_ON_PC6 << _GPIO_P_IN_Px6_SHIFT) \
   | (WAKE_ON_PC7 << _GPIO_P_IN_Px7_SHIFT) \
  )

// A monolithic constant for all GPIO wake sources
#define EMBER_WAKE_GPIO_BITMASK_ARRAY { \
    EMBER_WAKE_PORT_A,                  \
    EMBER_WAKE_PORT_B,                  \
    EMBER_WAKE_PORT_C,                  \
    0,                                  \
    0,                                  \
    0,                                  \
}

#endif // __WAKE_GPIO_H__
