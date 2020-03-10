/***************************************************************************//**
 * @file com-serial.h
 * @brief COM-Serial Transition Layer.
 * @version 0.01.0
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2016 Silicon Labs, www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/

// This file primarily exists to ensure cross-compatibility of legacy serial-
// style defines for COM port configuration. Serial style configuration will
// override COM style when applicable

#if (defined (EMBER_SERIAL1_XONXOFF) && defined (EMBER_SERIAL1_RTSCTS))
  #error Cannot define both SW and HW flow control
#endif

#ifdef EMBER_SERIAL1_RTSCTS
  #undef HAL_USART0_FLOW_CONTROL
  #define HAL_USART0_FLOW_CONTROL HAL_USART_FLOW_CONTROL_HWUART
#endif
#ifdef EMBER_SERIAL1_XONXOFF
  #undef HAL_USART0_FLOW_CONTROL
  #define HAL_USART0_FLOW_CONTROL HAL_USART_FLOW_CONTROL_SW
#endif

#ifdef EMBER_SERIAL1_RX_QUEUE_SIZE
  #undef HAL_USART0_RX_QUEUE_SIZE
  #define HAL_USART0_RX_QUEUE_SIZE EMBER_SERIAL1_RX_QUEUE_SIZE
#endif

#ifdef EMBER_SERIAL1_TX_QUEUE_SIZE
  #undef COM_USART0_TX_QUEUE_SIZE
  #define COM_USART0_TX_QUEUE_SIZE EMBER_SERIAL1_TX_QUEUE_SIZE
#endif

#if (defined (EMBER_SERIAL2_XONXOFF) && defined (EMBER_SERIAL2_RTSCTS))
  #error Cannot define both SW and HW flow control
#endif

#ifdef EMBER_SERIAL2_RTSCTS
  #undef HAL_USART1_FLOW_CONTROL
  #define HAL_USART1_FLOW_CONTROL HAL_USART_FLOW_CONTROL_HWUART
#endif
#ifdef EMBER_SERIAL2_XONXOFF
  #undef HAL_USART1_FLOW_CONTROL
  #define HAL_USART1_FLOW_CONTROL HAL_USART_FLOW_CONTROL_SW
#endif

#ifdef EMBER_SERIAL2_RX_QUEUE_SIZE
  #undef HAL_USART1_RX_QUEUE_SIZE
  #define HAL_USART1_RX_QUEUE_SIZE EMBER_SERIAL2_RX_QUEUE_SIZE
#endif

#ifdef EMBER_SERIAL2_TX_QUEUE_SIZE
  #undef COM_USART1_TX_QUEUE_SIZE
  #define COM_USART1_TX_QUEUE_SIZE EMBER_SERIAL2_TX_QUEUE_SIZE
#endif
