#include "zigbee_dev_template.h"
#include "zigbee_cmd.h"
#include "zigbee_attr.h"
#include "zigbee_sdk.h"
#include "common_def.h"
#include "Uart_Process.h"
#include "report_attr.h"


//const uint8_t Receive_Header[] = "setbri";
const uint8_t Receive_Header[] = "rptbri";
uint8_t data_offset = 0;
uint8_t header_offset = 0;
Uart_Data_State_t Data_State = UART_DATA_IDLE;
uint8_t Uart_receive_data[36];
Uart_Control_Data_t Control_Data;

void  str_to_hex(uint8_t *hex_str ,uint8_t *hex,uint8_t str_len) 
{
    uint16_t i = 0;
    uint16_t j = 0;

    for(i=0;i < str_len;i++)
    {

        if(hex_str[i] >= '0' && hex_str[i] <= '9') {
            hex[j] = (hex_str[i]-'0')<<4;
        } else if(hex_str[i] >= 'a' && hex_str[i] <= 'f') {
            hex[j] = (hex_str[i]-'a'+10)<<4;
        } else if(hex_str[i] >= 'A' && hex_str[i] <= 'F') {
            hex[j] = (hex_str[i]-'A'+10)<<4;
        } else {
            return ;
        }

        i++;

        if(hex_str[i] >= '0' && hex_str[i] <= '9') {
            hex[j] |= (hex_str[i]-'0');
        } else if(hex_str[i] >= 'a' && hex_str[i] <= 'f') {
            hex[j] |= (hex_str[i]-'a'+10);
        } else if(hex_str[i] >= 'A' && hex_str[i] <= 'F') {
            hex[j] |= (hex_str[i]-'A'+10);
        } else {
            return ;
        }
        i++;
        j++;
    }
}

void Uart_Parse_callback(uint8_t *data, uint16_t len)
{
    // user_uart_send(UART_ID_UART0,data, len); 
    uint16_t i=0;
    //uint8_t char_len[]="abc";
    //dev_led_start_blink(LED_ZIGBEE_ST_ID, 500, 500, len, DEV_IO_OFF);
    while(i < len)
    {
        //char_len[1] = len;
        //user_uart_send(UART_ID_UART0,char_len, 1);
        Uart_receive_data[data_offset] = data[i];
        data_offset++;
        if((Data_State == UART_DATA_IDLE) && (data_offset  > UART_HEADER_SIZE))
        {
          data_offset = 0;
        }
        if(data[i] == Receive_Header[2])
        {
            header_offset = 2;
        }
        if(data[i] == Receive_Header[header_offset])
        {
            if(header_offset == UART_HEADER_SIZE - 1)
            {
                Data_State = UART_GROUP_ID;
                data_offset = UART_HEADER_SIZE ;
            }
        }
        header_offset++;
        if(header_offset >= UART_HEADER_SIZE)
        {
            header_offset = 0;
        }
        switch(Data_State)
        {
        case UART_DATA_IDLE:
            //continue;
            break;
        case UART_GROUP_ID:
            if(data_offset >= (GROUP_ID_OFFSET + 2))
            {
                Data_State = UART_SET_LEVEL;
            }
            break;
        case UART_SET_LEVEL:
            if(data_offset >= (LEVEL_VAL_OFFSET + 2))
            {
                Data_State = UART_SET_TEMP;
            }
            break;
        case UART_SET_TEMP:
            if(data_offset >= (TEMP_VAL_OFFSET + 2))
            {
                Data_State = UART_SET_ONOFF;
            }
            break;        
        case UART_SET_ONOFF:
            if(data_offset >= (SET_VAL_OFFSET + 2))
            {
                Data_State = UART_DATA_END;
            }
            break;   
        case UART_DATA_END:
            
            if(data_offset >= (DATA_END_OFFSET + 2))
            {
                Data_State = UART_DATA_IDLE;
                data_offset = 0;
                if(Uart_receive_data[DATA_END_OFFSET] == '\r' &&
                   Uart_receive_data[DATA_END_OFFSET + 1] == '\n' )
                {
                    Uart_cmd_Process();
                }
            }
            break; 
        default:
            Data_State = UART_DATA_IDLE;
            data_offset = 0;
            break;
        }
        i++;
        
    }
}

uint8_t Device_Onoff = 0xff;
uint8_t Restart_flag = 0;

void Uart_cmd_Process(void)
{
  
  str_to_hex(&Uart_receive_data[GROUP_ID_OFFSET],&(Control_Data.Group_id),2);
  str_to_hex(&Uart_receive_data[LEVEL_VAL_OFFSET],&(Control_Data.Level_val),2);
  str_to_hex(&Uart_receive_data[TEMP_VAL_OFFSET],&(Control_Data.Temp_val),2);
  if(Uart_receive_data[SET_VAL_OFFSET] == 'c' && Uart_receive_data[SET_VAL_OFFSET + 1] == 'l') 
  {
    Control_Data.Set_val = 0;
    Device_Onoff  = 0;
    //Dev_onoff_status[0] = 0;
  }
  else if(Uart_receive_data[SET_VAL_OFFSET] == 'o' && Uart_receive_data[SET_VAL_OFFSET + 1] == 'p') 
  {
    Control_Data.Set_val = 1;
    Device_Onoff  = 1;
    //Dev_onoff_status[0] = 1;
  }
  else if(Uart_receive_data[SET_VAL_OFFSET] == 'b' && Uart_receive_data[SET_VAL_OFFSET + 1] == 'r')
  {
    Control_Data.Set_val = 2;
  }
  else
  {
    Control_Data.Set_val = 0xff;
  }
  if((Device_Onoff != Dev_onoff_status[0]) || (!(Restart_flag &0x01)))
  {
    Restart_flag |= 0x01;
    Dev_onoff_status[0] = Device_Onoff;
    dev_report_onoff_msg(g_dev_des[0].ep,Dev_onoff_status[0]);
  }
  if((Dev_level_status[0] != Control_Data.Level_val)  || (!(Restart_flag &0x02)))
  {
    Restart_flag |= 0x02;
    Dev_level_status[0] = Control_Data.Level_val;
    dev_report_level_msg(g_dev_des[0].ep,Dev_level_status[0]);
  }
  if((Dev_colortemp_status[0] != (uint16_t)Control_Data.Temp_val)  || (!(Restart_flag &0x04)))
  {
    Restart_flag |= 0x04;
    Dev_colortemp_status[0] = Control_Data.Temp_val;
    dev_report_ColorTemp_msg(g_dev_des[0].ep,Dev_colortemp_status[0]);
  }
  dev_io_op(LED_LIGHT_ST_ID, (DEV_IO_ST_T)!Dev_onoff_status[0]);
  
}

void Send_Control_Light(uint8_t index)
{
    uint8_t Send_data[16] = "setbri00000000\r\n";
    uint8_t color_temp = (uint8_t)Dev_colortemp_status[index];
    hex_to_hexstr(LETTER_TYPE_LOWER,&Dev_level_status[index],1,&Send_data[LEVEL_VAL_OFFSET]);
    hex_to_hexstr(LETTER_TYPE_LOWER,&color_temp,1,&Send_data[TEMP_VAL_OFFSET]);
    dev_io_op(LED_LIGHT_ST_ID, (DEV_IO_ST_T)!Dev_onoff_status[index]);
    if(Dev_onoff_status[index] == 0)
    {
      Send_data[SET_VAL_OFFSET] = 'c';
      Send_data[SET_VAL_OFFSET + 1] = 'l';
      
    }
    else if(Dev_onoff_status[index] == 1)
    {
      Send_data[SET_VAL_OFFSET] = 'o';
      Send_data[SET_VAL_OFFSET + 1] = 'p';      
    }
    else
    {
      Send_data[SET_VAL_OFFSET] = 'c';
      Send_data[SET_VAL_OFFSET + 1] = 'l';
    }
    user_uart_send(UART_ID_UART0,Send_data, 16);

}



