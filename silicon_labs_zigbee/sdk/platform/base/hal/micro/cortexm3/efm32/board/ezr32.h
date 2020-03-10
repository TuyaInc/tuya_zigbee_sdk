/** @file hal/micro/cortexm3/efm32/board/ezr32.h
 * See @ref board for detailed documentation.
 *
 * <!-- Copyright 2013 Silicon Laboratories, Inc.                        *80*-->
 */

/** @addtogroup board
 *  @brief Functions and definitions specific to the breakout board.
 *
 *@{
 */

#ifndef __BOARD_H__
#define __BOARD_H__

#include "bspconfig.h"
#include "em_device.h"

/** @brief PRO2+ SPI-Master configuration
 */
#if defined(CORTEXM3_EZR32_MICRO)
 #if defined(USARTRF0)
  #define PRO2_SPI_PORT USARTRF0
 #elif defined(USARTRF1)
  #define PRO2_SPI_PORT USARTRF1
 #else
  #error "No valid RF USART found"
 #endif
#elif defined(EFM32LG990F256)
 #define PRO2_SPI_PORT USART0
#endif

#if defined(PHY_EZR2)
//TODO: for some reason the r55 in EZR32 won't operate properly at frequencies
// higher than 1MHz.
 #define PRO2_SPI_MHZ    1      // EZR2 SPI clock rate in MHz
#elif defined(PHY_PRO2) || defined(PHY_PRO2PLUS)
 #define PRO2_SPI_MHZ    10     // PRO2 and PRO2+ SPI clock rate in MHz
#else
 #error Not supported PHY definition!
#endif

#include "hal/micro/cortexm3/efm32/board/ezr32-radio-boards.h"

/** @name Packet Trace
 *
 * When ::PACKET_TRACE is defined, ::GPIO_PACFGH will automatically be setup by
 * halInit() to enable Packet Trace support on PA4 and PA5,
 * in addition to the configuration specified below.
 *
 * @note This define will override any settings for PA4 and PA5.
 */
//@{

/**
 * @brief This define does not equate to anything.  It is used as a
 * trigger to enable Packet Trace support on the breakout board (dev0680).
 */
#define PACKET_TRACE  // We do have PACKET_TRACE support
//@} //END OF PACKET TRACE DEFINITIONS

/** @brief Radio HoldOff not supported
 */
#define halInternalInitRadioHoldOff() /* no-op */

#define halInternalPowerUpBoard()                                                   \
  do {                                                                              \
    /* MISO pull down */                                                            \
    GPIO_PinOutClear((GPIO_Port_TypeDef)RF_USARTRF_MISO_PORT, RF_USARTRF_MISO_PIN); \
  } while (0)

#define halInternalPowerDownBoard()                                               \
  do {                                                                            \
    /* MISO pull up */                                                            \
    GPIO_PinOutSet((GPIO_Port_TypeDef)RF_USARTRF_MISO_PORT, RF_USARTRF_MISO_PIN); \
  } while (0)

/**
 * @brief Initialize GPIO powerup configuration variables.
 */
#define DEFINE_SPI_CONFIG_VARIABLES()                               \
  const USART_TypeDef * pro2SpiPort = PRO2_SPI_PORT;                \
  const uint8_t pro2SpiClockMHz = PRO2_SPI_MHZ;                     \
  const uint8_t pro2GpioCfgPowerUp[] = { PRO2_POWERUP_GPIO_CFG() }; \
  int8_t phy2PtiGpio = PRO2_PTI_GPIO;

/**
 * GPIO definitions for WSTK BC serial
 */
// #define BSP_BCC_CLK  => defined in bspconfig.h

/**
 * Initialize GPIOs direction and default state for VCP.
 */
#if defined(BSP_BCC_USART)
#define halInternalInitVCPPins()  do {                                                \
    /* Configure GPIO pin for UART TX */                                              \
    /* To avoid false start, configure output as high. */                             \
    GPIO_PinModeSet(BSP_BCC_TXPORT, BSP_BCC_TXPIN, gpioModePushPull, 1u);             \
    /* Configure GPIO pin for UART RX */                                              \
    GPIO_PinModeSet(BSP_BCC_RXPORT, BSP_BCC_RXPIN, gpioModeInput, 1u);                \
    /* Enable the switch that enables UART communication. */                          \
    GPIO_PinModeSet(BSP_BCC_ENABLE_PORT, BSP_BCC_ENABLE_PIN, gpioModePushPull, 1u);   \
    BSP_BCC_USART->ROUTE |= USART_ROUTE_RXPEN | USART_ROUTE_TXPEN | BSP_BCC_LOCATION; \
} while (0)
#elif defined(BSP_BCC_LEUART)
#define halInternalInitVCPPins()  do {                                                   \
    /* Configure GPIO pin for UART TX */                                                 \
    /* To avoid false start, configure output as high. */                                \
    GPIO_PinModeSet(BSP_BCC_TXPORT, BSP_BCC_TXPIN, gpioModePushPull, 1u);                \
    /* Configure GPIO pin for UART RX */                                                 \
    GPIO_PinModeSet(BSP_BCC_RXPORT, BSP_BCC_RXPIN, gpioModeInput, 1u);                   \
    /* Enable the switch that enables UART communication. */                             \
    GPIO_PinModeSet(BSP_BCC_ENABLE_PORT, BSP_BCC_ENABLE_PIN, gpioModePushPull, 1u);      \
    BSP_BCC_LEUART->ROUTE |= LEUART_ROUTE_RXPEN | LEUART_ROUTE_TXPEN | BSP_BCC_LOCATION; \
} while (0)
#endif
// Enable COM port to use retarget serial configuration
#define COM_RETARGET_SERIAL

#endif //__BOARD_H__

/** @} END Board Specific Functions */

/** @} END addtogroup */
