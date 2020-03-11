/****************************************Copyright (c)*************************
**                               版权所有 (C), 2015-2020, 涂鸦科技
**
**                                 http://www.tuya.com
**
**--------------文件信息-------------------------------------------------------*/

#include "string.h"
//#include "system.h"
//#include "serial_protocol.h"
#include "serial_protocol_callbacks.h"

/*****************************************************************************
函数名称 : hex_to_bcd
功能描述 : hex转bcd
输入参数 : Value_H:高字节/Value_L:低字节
返回参数 : bcd_value:转换完成后数据
*****************************************************************************/
unsigned char hex_to_bcd(unsigned char Value_H,unsigned char Value_L)
{
  unsigned char bcd_value;
  
  if((Value_H >= '0') && (Value_H <= '9'))
    Value_H -= '0';
  else if((Value_H >= 'A') && (Value_H <= 'F'))
    Value_H = Value_H - 'A' + 10;
  else if((Value_H >= 'a') && (Value_H <= 'f'))
    Value_H = Value_H - 'a' + 10;
   
  bcd_value = Value_H & 0x0f;
  
  bcd_value <<= 4;
  if((Value_L >= '0') && (Value_L <= '9'))
    Value_L -= '0';
  else if((Value_L >= 'A') && (Value_L <= 'F'))
    Value_L = Value_L - 'a' + 10;
  else if((Value_L >= 'a') && (Value_L <= 'f'))
    Value_L = Value_L - 'a' + 10;
  
  bcd_value |= Value_L & 0x0f;

  return bcd_value;
}


/*****************************************************************************
函数名称 : my_atoi
功能描述 : 将字符转换成十进制数
输入参数 : s:字符串地址
返回参数 : n:转换后的int值
*****************************************************************************/
int my_atoi(char s[])
{
    int i;
    int n = 0;
    for (i = 0; s[i] >= '0' && s[i] <= '9'; ++i)
    {
        n = 10 * n + (s[i] - '0');
    }
    return n;
}

/*****************************************************************************
函数名称 : my_strlen
功能描述 : 求字符串长度
输入参数 : src:源地址
返回参数 : len:数据长度
*****************************************************************************/
unsigned long my_strlen(unsigned char *str)  
{
  unsigned long len = 0;
  if(str == NULL)
  { 
    return 0;
  }
  
  for(len = 0; *str ++ != '\0'; )
  {
    len ++;
  }
  
  return len;
}
/*****************************************************************************
函数名称 : my_memset
功能描述 : 把src所指内存区域的前count个字节设置成字符c
输入参数 : src:源地址
           ch:设置字符
           count:设置数据长度
返回参数 : src:数据处理完后的源地址
*****************************************************************************/
void *my_memset(void *src,unsigned char ch,unsigned short count)
{
  unsigned char *tmp = (unsigned char *)src;
  
  if(src == NULL)
  {
    return NULL;
  }
  
  while(count --)
  {
    *tmp ++ = ch;
  }
  
  return src;
}
/*****************************************************************************
函数名称 : mymemcpy
功能描述 : 内存拷贝
输入参数 : dest:目标地址
           src:源地址
           count:数据拷贝数量
返回参数 : src:数据处理完后的源地址
*****************************************************************************/
void *my_memcpy(void *dest, const void *src, unsigned short count)  
{  
  unsigned char *pdest = (unsigned char *)dest;  
  const unsigned char *psrc  = (const unsigned char *)src;  
  unsigned short i;
  
  if(dest == NULL || src == NULL)
  { 
    return NULL;
  }
  
  if((pdest <= psrc) || (pdest > psrc + count))
  {  
    for(i = 0; i < count; i ++)
    {  
      pdest[i] = psrc[i];  
    }  
  }
  else
  {
    for(i = count; i > 0; i --)
    {  
      pdest[i - 1] = psrc[i - 1];  
    }  
  }  
  
  return dest;  
}
/*****************************************************************************
函数名称 : my_strcpy
功能描述 : 内存拷贝
输入参数 : s1:目标地址
           s2:源地址
返回参数 : 数据处理完后的源地址
*****************************************************************************/
char *my_strcpy(char *dest, const char *src)  
{
  char *p = dest;
  while(*src!='\0')
  {
    *dest++ = *src++;
  }
  *dest = '\0';
  return p;
}
/*****************************************************************************
函数名称 : my_strcmp
功能描述 : 内存拷贝
输入参数 : s1:字符串1
           s2:字符串2
返回参数 : 大小比较值，0:s1=s2; -1:s1<s2; 1:s1>s2
*****************************************************************************/
int my_strcmp(char *s1 , char *s2)
{
  while( *s1 && *s2 && *s1 == *s2 )
  {
    s1++;
    s2++;
  }
  return *s1 - *s2;
}
/*****************************************************************************
函数名称 : int_to_byte
功能描述 : 将int类型拆分四个字节
输入参数 : number:4字节原数据;value:处理完成后4字节数据
返回参数 :无
****************************************************************************/
void int_to_byte(unsigned long number,unsigned char value[4])
{
  value[0] = number >> 24;
  value[1] = number >> 16;
  value[2] = number >> 8;
  value[3] = number & 0xff;
}
/*****************************************************************************
函数名称 : byte_to_int
功能描述 : 将4字节合并为1个32bit变量
输入参数 : value:4字节数组
返回参数 : number:合并完成后的32bit变量
****************************************************************************/
unsigned long byte_to_int(const unsigned char value[4])
{
  unsigned long nubmer = 0;

  nubmer = (unsigned long)value[0];
  nubmer <<= 8;
  nubmer |= (unsigned long)value[1];
  nubmer <<= 8;
  nubmer |= (unsigned long)value[2];
  nubmer <<= 8;
  nubmer |= (unsigned long)value[3];
  
  return nubmer;
}

/*****************************************************************************
函数名称 : get_check_sum
功能描述 : 计算校验和
输入参数 : pack:数据源指针
           pack_len:计算校验和长度
返回参数 : 校验和
*****************************************************************************/
unsigned char get_check_sum(unsigned char *pack, unsigned short pack_len)
{
  unsigned short i;
  unsigned char check_sum = 0;
  
  for(i = 0; i < pack_len; i ++)
  {
    check_sum += *pack ++;
  }
  
  return check_sum;
}

/*****************************************************************************
函数名称 :  zigbee_protocol_init
功能描述 : 协议串口初始化函数
输入参数 : 无
返回参数 : 无
使用说明 : 必须在MCU初始化代码中调用该函数
*****************************************************************************/
void zigbee_protocol_init(void)
{
  queue_in = (unsigned char *)zigbee_queue_buf;
  queue_out = (unsigned char *)zigbee_queue_buf;
  queue_total_data = 0;
}

/*****************************************************************************
函数名称 : uart_receive_input
功能描述 : 收数据处理
输入参数 : value:串口收到字节数据
返回参数 : 无
使用说明 : 在MCU串口接收函数中调用该函数,并将接收到的数据作为参数传入
*****************************************************************************/
void uart_receive_input(unsigned char value)
{
  if(queue_total_data < sizeof(zigbee_queue_buf))
  {
    //队列不满
    if(queue_in >= (unsigned char *)(zigbee_queue_buf + sizeof(zigbee_queue_buf)))
    {
      queue_in = (unsigned char *)(zigbee_queue_buf);
    }
    
    *queue_in ++ = value;
    queue_total_data++;
  }
  else
  {
    //队列已满
    app_print("table full", sizeof("table full"));
  }
}


/*****************************************************************************
函数名称  : zigbee_uart_service
功能描述  : wifi串口处理服务
输入参数 : 无
返回参数 : 无
使用说明 : 在MCU主函数while循环中调用该函数
*****************************************************************************/
void serialDataReceiveHandler(unsigned char* payload, unsigned short payload_len)
{
    app_print("payload: %s, len: %d", payload, payload_len);
    
    uint8_t cmdVersion = payload[PROTOCOL_VERSION];

    if(SERIAL_PROTOCOL_VER >= cmdVersion)
    {
        serial_protocol_cmd_t serial_protocol_receive_cmd;

        memset(&serial_protocol_receive_cmd, 0, sizeof(serial_protocol_cmd_t));

        serial_protocol_receive_cmd.cmd_hdr = payload[HEAD_FIRST];
        serial_protocol_receive_cmd.cmd_hdr = serial_protocol_receive_cmd.cmd_hdr << 8 | payload[HEAD_SECOND];
        serial_protocol_receive_cmd.version = payload[PROTOCOL_VERSION];

        if(0x02 <= serial_protocol_receive_cmd.version)
        {
            /*version 2 has two bytes of seq num than version 1*/
            serial_protocol_receive_cmd.seq_num = payload[FRAME_TYPE] << 8;
            serial_protocol_receive_cmd.seq_num |= payload[FRAME_TYPE + 1];
            serial_protocol_receive_cmd.cmd = payload[FRAME_TYPE + 2];
            serial_protocol_receive_cmd.data_len = payload[LENGTH_HIGH + 2] << 8;
            serial_protocol_receive_cmd.data_len += payload[LENGTH_LOW + 2];
        }
        else
        {
            serial_protocol_receive_cmd.cmd = payload[FRAME_TYPE];
            serial_protocol_receive_cmd.data_len = payload[LENGTH_HIGH] << 8;
            serial_protocol_receive_cmd.data_len += payload[LENGTH_LOW];
        }
        
        if(serial_protocol_receive_cmd.data_len > MAX_SERIAL_PAYLOAD_SIZE)
        {
            return;
        }
        memcpy(serial_protocol_receive_cmd.data, &payload[DATA_START + 2], serial_protocol_receive_cmd.data_len);
  
        /*serial protocol command handler*/
        serialCommandRxHandler(&serial_protocol_receive_cmd);
        return;
    }
    //else
    {
        /*custom serial data receive handler*/
        ; /*NULL*/
    }
}

/*****************************************************************************
函数名称  : zigbee_uart_service
功能描述  : zigbee串口处理服务
输入参数 : 无
返回参数 : 无
使用说明 : 在MCU主函数while循环中调用该函数
*****************************************************************************/
//extern void serialDataReceiveHandler(unsigned char* payload, unsigned char payload_len);

void zigbee_uart_service(void)
{
    static unsigned short rx_in = 0;
    unsigned short offset = 0;
    unsigned short rx_value_len = 0;     //数据帧长度
    unsigned short protocol_version = 0, protocol_head_len = PROTOCOL_HEAD;
    unsigned char crc_value = 0;

    while((rx_in < sizeof(zigbee_uart_rx_buf)) && (get_queue_total_data() > 0))
    {
        zigbee_uart_rx_buf[rx_in++] = Queue_Read_Byte();
    }

    if(rx_in < PROTOCOL_HEAD)
    {
        return;
    }

    while((rx_in - offset) >= PROTOCOL_HEAD)
    {
        if(zigbee_uart_rx_buf[offset + HEAD_FIRST] != FIRST_FRAME_HEAD)
        {
            offset++;
            continue;
        }

        if(zigbee_uart_rx_buf[offset + HEAD_SECOND] != SECOND_FRAME_HEAD)
        {
            offset++;
            continue;
        }

        protocol_version = zigbee_uart_rx_buf[offset + PROTOCOL_VERSION];
        if(protocol_version > 0x02)
        {
            offset += 2; 
            continue;
        }

        if(protocol_version == 0x02) /*version 2 is more than two byte  sequence number than version 1*/
        {
            rx_value_len = zigbee_uart_rx_buf[offset + LENGTH_HIGH + 2] << 8;
            rx_value_len |= (zigbee_uart_rx_buf[offset + LENGTH_LOW + 2] + PROTOCOL_HEAD);
            protocol_head_len = PROTOCOL_HEAD;
            rx_value_len += 2;  /*two bytes sequence number*/
        }
        else
        {
            rx_value_len = zigbee_uart_rx_buf[offset + LENGTH_HIGH] << 8;
            rx_value_len |= (zigbee_uart_rx_buf[offset + LENGTH_LOW] + PROTOCOL_HEAD);
            protocol_head_len = PROTOCOL_HEAD - 2;
        }

        if(rx_value_len > sizeof(zigbee_uart_rx_buf) + protocol_head_len)
        {
            offset += 3;
            continue;
        }

        if((rx_in - offset) < rx_value_len)
        {
            break;
        }

        //数据接收完成, 进行数据校验
        if( (crc_value = get_check_sum((unsigned char *)zigbee_uart_rx_buf + offset, rx_value_len - 1)) != zigbee_uart_rx_buf[offset + rx_value_len - 1])
        {
            //数据校验失败
            app_print("CRC error，right CRC = %d", crc_value);
            offset += 3; 
            continue;
        }

        serialDataReceiveHandler((unsigned char*)&zigbee_uart_rx_buf[offset], rx_value_len);
        offset += rx_value_len;
    }
    rx_in -= offset;

    if(rx_in > 0)
    {
        if((offset == 0)  && (rx_in >= sizeof(zigbee_uart_rx_buf)))
        {
            //如果缓存队列已满并且offset=0，此时队列已无法添加数据，但是接收数据长度不够，则丢掉第一个数据； 否则会陷入死循环
            offset++;
            rx_in -= offset;
            my_memcpy((char*)zigbee_uart_rx_buf, (const char *)zigbee_uart_rx_buf + offset, rx_in);
        }
        else
        {
            my_memcpy((char*)zigbee_uart_rx_buf, (const char *)zigbee_uart_rx_buf + offset, rx_in);
        }
    }
}
