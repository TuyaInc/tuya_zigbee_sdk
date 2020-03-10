// File: uart-link.c
//
// Description: low-level driver for sending typed blocks of data on EFM
//  or in simulation.
//
// Copyright 2015 Silicon Laboratories, Inc.                                *80*

#include PLATFORM_HEADER
#if defined(EMBER_STACK_CONNECT)
#include "stack/include/ember.h"
#else
#include "stack/core/ember-stack.h"
#endif
#include "hal/hal.h"
#include "hal/micro/uart-link.h"
#include "hal/micro/generic/ash-v3.h"
#include "serial/com.h"
#include "uart_link_config.h"

#define BUFFER_SIZE 100

void halHostSerialInit(void)
{
  COM_Init_t initData = (COM_Init_t) NCP_COM_INIT;
  COM_Init(UART_PORT, &initData);
}

void halHostSerialTick(void)
{
  uint8_t byte;
  uint8_t ashInput[BUFFER_SIZE] = { 0 };
  uint8_t i = 0;

  while (COM_ReadByte(UART_PORT, &byte) == EMBER_SUCCESS
         && i < BUFFER_SIZE) {
    ashInput[i] = byte;
    i++;
  }

  if (COM_InternalTxIsIdle(UART_PORT)) {
    emAshNotifyTxComplete();
  }

  if (i > 0) {
    emProcessAshRxInput(ashInput, i);
  }
}

void halHostUartLinkTx(const uint8_t *data, uint16_t length)
{
  COM_WriteData(UART_PORT, (uint8_t *)data, length);
}

bool halHostUartTxIdle(void)
{
  return (COM_InternalTxIsIdle(UART_PORT));
}
