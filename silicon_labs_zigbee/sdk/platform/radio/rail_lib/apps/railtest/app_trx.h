/***************************************************************************//**
 * @file app_trx.h
 * @brief RAILTEST transmit and receive
 * @copyright Copyright 2015 Silicon Laboratories, Inc. www.silabs.com
 ******************************************************************************/
typedef struct BerStatus{
  uint32_t bytesTotal; /**< Number of bytes to receive */
  uint32_t bytesTested; /**< Number of bytes currently tested */
  uint32_t bitErrors; /**< Number of bits errors detected */
  int8_t   rssi; /**< Current RSSI value during pattern acquisition */
} BerStatus_t;

// Variables for TX
extern RAIL_Events_t lastTxStatus;
extern RAIL_Events_t lastTxAckStatus;
extern uint32_t sentAckPackets;
extern RAIL_TxPacketDetails_t previousTxAppendedInfo;
extern RAIL_TxPacketDetails_t previousTxAckAppendedInfo;

// Variables for RX
extern uint32_t abortRxDelay;
extern RAIL_ScheduleTxConfig_t nextPacketTxTime;

// Variables for BER testing
extern BerStatus_t berStats;
extern bool berTestModeEnabled;

void configRxLengthSetting(uint16_t rxLength);

void loadTxData(uint8_t *data, uint16_t dataLen);
void setNextPacketTime(RAIL_ScheduleTxConfig_t *scheduledTxOptions);
