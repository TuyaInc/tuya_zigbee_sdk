/**
 * @author Leon
 * @par email:
 *      zhangpeng@tuya.com
 * @file hal_spi.h
 * @brief This file abstracts all the operations of SPI
 * @copyright HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @par company
 *      http://www.tuya.com
 * @date 2020-03-20 17:29:33
 */

#ifndef __HAL_SPI_H__
#define __HAL_SPI_H__

#include "type_def.h"
#include "hal_gpio.h"
#include "hal_uart.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @enum SPI role
 */
typedef enum {
  SPI_MASTER = 0,               ///< Act as an SPI master.
  SPI_SLAVE  = 1                ///< Act as an SPI slave.
} SPI_TYPE_T;

/**
 * @enum SPI bus bit order
 */
typedef enum {
  SPI_LSB_FIRST = 0,     ///< LSB bit is transmitted first.
  SPI_MSB_FIRST = 1      ///< MSB bit is transmitted first.
} SPI_BIT_ORDER_T;

/**
 * @enum SPI clock mode (clock polarity and phase)
 */
typedef enum {
  SPI_CLOCK_MODE0  = 0,           ///< SPI mode 0: CLKPOL=0, CLKPHA=0.
  SPI_CLOCK_MODE1  = 1,           ///< SPI mode 1: CLKPOL=0, CLKPHA=1.
  SPI_CLOCK_MODE2  = 2,           ///< SPI mode 2: CLKPOL=1, CLKPHA=0.
  SPI_CLOCK_MODE3  = 3            ///< SPI mode 3: CLKPOL=1, CLKPHA=1.
} SPI_CLOCK_MODE_T;

/**
 * @enum SPI master chip select (CS) control scheme
 */
typedef enum {
  SPI_CS_CONTROL_AUTO  = 0,        ///< CS controlled by the SPI driver.
  SPI_CS_CONTROL_APP  = 1  ///< CS controlled by the application.
} SPI_CSCONTROL_T ;

/**
 * @enum SPI slave transfer start scheme
 */
typedef enum {
  SPI_SLAVE_START_IMMEDIATE  = 0,  ///< Transfer starts immediately.
  SPI_SLAVE_START_DELAYED  = 1     ///< Transfer starts when the bus is idle (CS deasserted).
} SPI_SLAVE_START_T ;

/**
 * @enum SPI SPI byte length
 */
typedef enum {
  SPI_FRAME4  = 4,  ///< a byte use 4 bits
  SPI_FRAME5  = 5,  ///< a byte use 5 bits
  SPI_FRAME6  = 6,
  SPI_FRAME7  = 7,
  SPI_FRAME8  = 8,
  SPI_FRAME9  = 9,
  SPI_FRAME10  = 10,
  SPI_FRAME11  = 11,
  SPI_FRAME12  = 12,
  SPI_FRAME13  = 13,
  SPI_FRAME14  = 14,
  SPI_FRAME15  = 15,
  SPI_FRAME16  = 16
} SPI_FRAME_LENGTH_T;

/**
 * @note An SPI driver instance initialization structure
 */
typedef struct {
  UART_ID_T           port_id;            ///< The USART used for SPI.
  GPIO_LOC_T          port_mosi_loc;   ///< A location number for the SPI Tx pin.
  GPIO_LOC_T          port_miso_loc;   ///< A location number for the SPI Rx pin.
  GPIO_LOC_T          port_clk_loc;  ///< A location number for the SPI Clk pin.
  GPIO_LOC_T          port_cs_loc;   ///< A location number for the SPI Cs pin.
  GPIO_PORT_PIN_T     cs;
  uint32_t            bitRate;          ///< An SPI bitrate.
  SPI_FRAME_LENGTH_T  frame_length;      ///< An SPI framelength, valid numbers are 4..16
  SPI_TYPE_T          type;             ///< An SPI type, master or slave.
  SPI_BIT_ORDER_T     bit_order;         ///< A bit order on the SPI bus, MSB or LSB first.
  SPI_CLOCK_MODE_T    clock_mode;        ///< SPI mode, CLKPOL/CLKPHASE setting.
  SPI_CSCONTROL_T     cs_control;        ///< A select master mode chip select (CS) control scheme.
  SPI_SLAVE_START_T   slave_start_mode;   ///< A slave mode transfer start scheme.
} spi_init_config_t ;


/**
 * @remarks Configuration data for SPI Master Init
 */
#define DEFAULT_SPI_CONFIG                                                     \
  {                                                                    \
    UART_ID_UART1,          /* USART port                       */ \
    LOC_23,                 /* USART Tx/MOSI pin location number default PD15 */ \
    LOC_4,                  /* USART Rx/MISO pin location number default PA5 */ \
    LOC_0,                  /* USART Clk pin location number default PA2*/ \
    LOC_23,                 /* USART Cs pin location number  default PF2*/ \
    {PORT_F, PIN_2},\
    50000,                  /* Bitrate                          */ \
    SPI_FRAME8,             /* Frame length                     */ \
    SPI_MASTER,             /* SPI mode                         */ \
    SPI_MSB_FIRST,          /* Bit order on bus                 */ \
    SPI_CLOCK_MODE0,        /* SPI clock/phase mode             */ \
    SPI_CS_CONTROL_APP,     /* CS controlled by aplication      */ \
    SPI_SLAVE_START_IMMEDIATE   /* Slave start transfers immediately*/ \
  }

/**
 * @note Initializes the SPI communication peripheral.
 * @param[in] {spi_config} - spi initializes parameters
 * @return success 1  failed 0.
 */
bool_t spi_init_config(spi_init_config_t *spi_config);

/**
 * @note SPI deinit 
 * @param none.
 * @return none.
 */
void spi_deinit(void);

/**
 * @note SPI Writes and read data 
 * @param[in] {data_len} write data len.
 * @param[in] {data_buffer} spi write data 
 * @return none.
 */
void spi_write_data(uint8_t data_len,uint8_t data_buffer[]);

/**
 * @note SPI read data 
 * @param[in] {data_len}  read data len.
 * @param[out] {data_buffer}  spi read data 
 * @return none.
 */
void spi_read_data(uint8_t data_len,uint8_t data_buffer[]);

/**
 * @note SPI Writes and read data 
 * @param[in] {write_data_len} write data len.
 * @param[in] {write_data_buffer} spi write data 
 * @param[in] {read_data_len} read data len.
 * @param[out] read_data_buffer} spi read data 
 * @return none.
 */
void spi_write_and_read_data(uint8_t write_data_len,
                                        uint8_t write_data_buffer[],
                                        uint8_t read_data_len,
                                        uint8_t read_data_buffer[]);



#ifdef __cplusplus
}
#endif

#endif


