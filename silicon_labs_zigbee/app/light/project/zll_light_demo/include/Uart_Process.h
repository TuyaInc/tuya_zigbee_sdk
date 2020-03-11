#ifndef __TUYA_UART_PROCESS_H__
#define __TUYA_UART_PROCESS_H__

typedef enum
{
    UART_DATA_IDLE,
    UART_DATA_HEADER,
    UART_GROUP_ID,
    UART_SET_LEVEL,  //LEVEL
    UART_SET_TEMP,  //TEMP
    UART_SET_ONOFF,  //
    UART_DATA_END
}Uart_Data_State_t;

#define UART_HEADER_SIZE    6
#define GROUP_ID_OFFSET     6
#define LEVEL_VAL_OFFSET    8
#define TEMP_VAL_OFFSET     10
#define SET_VAL_OFFSET      12
#define DATA_END_OFFSET     14


typedef  struct {
    uint8_t Group_id;
    uint8_t Level_val;
    uint8_t Temp_val;
    uint8_t Set_val;
}Uart_Control_Data_t;

void Uart_cmd_Process(void);
void Uart_Parse_callback(uint8_t *data, uint16_t len);
void Send_Control_Light(uint8_t index);

#endif