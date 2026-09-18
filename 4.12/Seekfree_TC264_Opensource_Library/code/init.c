/*
 * init.c
 *
 *  Created on: 2025年4月4日
 *      Author: 王开阳
 */


#include "init.h"

#define IPS200_TYPE     (IPS200_TYPE_SPI)
void all_init(void)
{
    small_driver_uart_init();
    pwm_init(PWM_CH1, 300, 0);                                                // 初始化 PWM 通道 频率 17KHz 初始占空比 0%
    pwm_init(PWM_CH2, 300, 0);                                                // 初始化 PWM 通道 频率 17KHz 初始占空比 0%
    pwm_init(PWM_CH3, 300, 0);                                                // 初始化 PWM 通道 频率 17KHz 初始占空比 0%
    pwm_init(PWM_CH4, 300, 0);
     imu660ra_init();
     ips200_set_dir(IPS200_PORTAIT);
     ips200_set_color(RGB565_WHITE, RGB565_BLACK);
     ips200_init(IPS200_TYPE);
    // balance_init();


}

unsigned char data_to_send[50];



void ANO_DT_send_int16byte16(short data1, short data2, short data3, short data4, short data5, short data6, short data7, short data8 )
{
  unsigned char  _cnt=0;
  unsigned char  sum = 0, i = 0;

  data_to_send[_cnt++] = 0xAA;
  data_to_send[_cnt++] = 0xAA;
  data_to_send[_cnt++] = 0xF1;
  data_to_send[_cnt++] = 16;

  data_to_send[_cnt++]=(unsigned short)(data1>>8);
  data_to_send[_cnt++]=(unsigned char)(data1);

  data_to_send[_cnt++]=(unsigned short)(data2>>8);
  data_to_send[_cnt++]=(unsigned char)(data2);

  data_to_send[_cnt++]=(unsigned short)(data3>>8);
  data_to_send[_cnt++]=(unsigned char)(data3);

  data_to_send[_cnt++]=(unsigned short)(data4>>8);
  data_to_send[_cnt++]=(unsigned char)(data4);

  data_to_send[_cnt++]=(unsigned short)(data5>>8);
  data_to_send[_cnt++]=(unsigned char)(data5);

  data_to_send[_cnt++]=(unsigned short)(data6>>8);
  data_to_send[_cnt++]=(unsigned char)(data6);

  data_to_send[_cnt++]=(unsigned short)(data7>>8);
  data_to_send[_cnt++]=(unsigned char)(data7);

  data_to_send[_cnt++]=(unsigned short)(data8>>8);
  data_to_send[_cnt++]=(unsigned char)(data8);

  sum = 0;
  for(i=0;i<_cnt;i++)
    sum += data_to_send[i];
  data_to_send[_cnt++]=sum;

  uart_write_buffer(UART_2, data_to_send, _cnt);
}
void ANO_DT_send_int16(short data1, short data2, short data3, short data4, short data5, short data6, short data7, short data8 )
{
  unsigned char  _cnt=0;
  unsigned char  sum = 0, i = 0;

  data_to_send[_cnt++] = 0xAA;
  data_to_send[_cnt++] = 0xAA;
  data_to_send[_cnt++] = 0xF1;
  data_to_send[_cnt++] = 16;

  data_to_send[_cnt++]=(unsigned short)(data1>>8);
  data_to_send[_cnt++]=(unsigned char)(data1);

  data_to_send[_cnt++]=(unsigned short)(data2>>8);
  data_to_send[_cnt++]=(unsigned char)(data2);

  data_to_send[_cnt++]=(unsigned short)(data3>>8);
  data_to_send[_cnt++]=(unsigned char)(data3);

  data_to_send[_cnt++]=(unsigned short)(data4>>8);
  data_to_send[_cnt++]=(unsigned char)(data4);

  data_to_send[_cnt++]=(unsigned short)(data5>>8);
  data_to_send[_cnt++]=(unsigned char)(data5);

  data_to_send[_cnt++]=(unsigned short)(data6>>8);
  data_to_send[_cnt++]=(unsigned char)(data6);

  data_to_send[_cnt++]=(unsigned short)(data7>>8);
  data_to_send[_cnt++]=(unsigned char)(data7);

  data_to_send[_cnt++]=(unsigned short)(data8>>8);
  data_to_send[_cnt++]=(unsigned char)(data8);

  sum = 0;
  for(i=0;i<_cnt;i++)
    sum += data_to_send[i];
  data_to_send[_cnt++]=sum;

  uart_write_buffer(UART_2, data_to_send, _cnt);
}
void ANO_DT_send_int16byte18(short data1, short data2, short data3, short data4, short data5, short data6, short data7, short data8, short data9 )
{
  unsigned char  _cnt=0;
  unsigned char  sum = 0, i = 0;

  data_to_send[_cnt++] = 0xAA;
  data_to_send[_cnt++] = 0xAA;
  data_to_send[_cnt++] = 0xF1;
  data_to_send[_cnt++] = 18;

  data_to_send[_cnt++]=(unsigned short)(data1>>8);
  data_to_send[_cnt++]=(unsigned char)(data1);

  data_to_send[_cnt++]=(unsigned short)(data2>>8);
  data_to_send[_cnt++]=(unsigned char)(data2);

  data_to_send[_cnt++]=(unsigned short)(data3>>8);
  data_to_send[_cnt++]=(unsigned char)(data3);

  data_to_send[_cnt++]=(unsigned short)(data4>>8);
  data_to_send[_cnt++]=(unsigned char)(data4);

  data_to_send[_cnt++]=(unsigned short)(data5>>8);
  data_to_send[_cnt++]=(unsigned char)(data5);

  data_to_send[_cnt++]=(unsigned short)(data6>>8);
  data_to_send[_cnt++]=(unsigned char)(data6);

  data_to_send[_cnt++]=(unsigned short)(data7>>8);
  data_to_send[_cnt++]=(unsigned char)(data7);

  data_to_send[_cnt++]=(unsigned short)(data8>>8);
  data_to_send[_cnt++]=(unsigned char)(data8);

  data_to_send[_cnt++]=(unsigned short)(data9>>8);
  data_to_send[_cnt++]=(unsigned char)(data9);

  sum = 0;
  for(i=0;i<_cnt;i++)
    sum += data_to_send[i];
  data_to_send[_cnt++]=sum;

  uart_write_buffer(UART_2, data_to_send, _cnt);
}

