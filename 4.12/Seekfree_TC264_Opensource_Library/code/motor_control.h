/*
 * motor_control.h
 *
 *  Created on: 2025年4月4日
 *      Author: 王开阳
 */

#ifndef CODE_MOTOR_CONTROL_H_
#define CODE_MOTOR_CONTROL_H_


#include "zf_common_headfile.h"


#define SMALL_DRIVER_UART                       (UART_3        )

#define SMALL_DRIVER_BAUDRATE                   (460800        )

#define SMALL_DRIVER_RX                         (UART3_TX_P15_6)

#define SMALL_DRIVER_TX                         (UART3_RX_P15_7)

typedef struct
{
        unsigned char send_data_buffer[7];                  // 发送缓冲数组

        unsigned char receive_data_buffer[7];               // 接收缓冲数组

        unsigned char receive_data_count;                   // 接收计数

        unsigned char sum_check_data;                       // 校验位

        signed short int receive_left_speed_data;              // 接收到的左侧电机速度数据

        signed short int receive_right_speed_data;             // 接收到的右侧电机速度数据

}small_device_value_struct;

extern small_device_value_struct motor_value;



void uart_control_callback(void);                                   // 无刷驱动 串口接收回调函数

void small_driver_set_duty(signed short int left_duty, signed short int right_duty);      // 无刷驱动 设置电机占空比

void small_driver_get_speed(void);                                  // 无刷驱动 获取速度信息

void small_driver_uart_init(void);                                  // 无刷驱动 串口通讯初始化



#endif /* CODE_MOTOR_CONTROL_H_ */
