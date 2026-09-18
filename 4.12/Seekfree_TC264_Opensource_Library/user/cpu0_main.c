/*********************************************************************************************************************
* TC264 Opensourec Library 即（TC264 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
*
* 本文件是 TC264 开源库的一部分
*
* TC264 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
*
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
*
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
*
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
*
* 文件名称          cpu0_main
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          ADS v1.10.2
* 适用平台          TC264D
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期              作者                备注
* 2022-09-15       pudding            first version
********************************************************************************************************************/
#include "zf_common_headfile.h"
#pragma section all "cpu0_dsram"
// 将本语句与#pragma section all restore语句之间的全局变量都放在CPU0的RAM中

// 本例程是开源库空工程 可用作移植或者测试各类内外设
// 本例程是开源库空工程 可用作移植或者测试各类内外设
// 本例程是开源库空工程 可用作移植或者测试各类内外设
short i=0;
short flag_balance_acc=0;
short flag_balance_angle=0;
short flag_balance_vel=0;
short high_control;
int high=12;
int tall;
float speed=0;
short speed_flag=0;
short jump_flag=0;
float balance_zero=12.85;
float left_vel=0;
float right_vel=0;
short hign_flag=0;
int flag_best=0;
short dir_flag=0;
int high_real=0;
int high_true=0;

float angle_left_up=0;
float angle_left_down=0;
float angle_right_up=0;
float angle_right_down=0;

float miuerr1,miuerr2;

// **************************** 代码区域 ****************************
int core0_main(void)
{
    clock_init();                   // 获取时钟频率<务必保留>
    debug_init();                   // 初始化默认调试串口
    // 此处编写用户代码 例如外设初始化代码等

    all_init();
    mt9v03x_init();
    gpio_init(P20_8, GPO, 1, GPO_PUSH_PULL);
   // gpio_init(P00_0, GPO, 1, GPO_PUSH_PULL);
   // seekfree_assistant_interface_init(SEEKFREE_ASSISTANT_DEBUG_UART);
   // seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_MT9V03X, mt9v03x_image[0], MT9V03X_W, MT9V03X_H);
    PID_init();
    uart_init(UART_2,115200,UART2_TX_P10_5,UART2_RX_P10_6);
    key_init(10);
  //  uart_init(UART_2,115200,UART2_TX_P10_5,UART2_RX_P10_6);
    pit_ms_init(CCU60_CH0,1);

   // pit_ms_init(CCU60_CH1,1);
   system_delay_ms(100);
    // 此处编写用户代码 例如外设初始化代码等
    cpu_wait_event_ready();         // 等待所有核心初始化完毕
    while (TRUE)
    {
        // 此处编写需要循环执行的代码

                key_scanner();
                if(key_get_state(KEY_1))
                {
                    flag_best=1;

                }
              //  ips200_show_float(0, 0, Angle_X_Final, 2, 3);
     //           ips200_show_gray_image(0, 0, Image_use_zip[0], IMAGE_W, IMAGE_H, IMAGE_W, IMAGE_H, 0);
        // 此处编写需要循环执行的代码
              //  ANO_DT_send_int16byte16((short)miuerr1,(short)Gyro_x,(short)Angle_X_Final,(short)motor_value.receive_right_speed_data,angle_left_up,angle_left_down,angle_right_up,angle_right_down);
    }

}
//(short)(0-Gyro_y),Angle_Y_Final

IFX_INTERRUPT(cc60_pit_ch0_isr, 0, CCU6_0_CH0_ISR_PRIORITY)
{
    interrupt_global_enable(0);                     // 开启中断嵌套
    pit_clear_flag(CCU60_CH0);
    gpio_set_level(P20_8, 0);
    i++;
//  int middle1,middle2;

     if(i>=2)
     {
       imu660ra_get_acc();                                                         // 获取 IMU660RA 的加速度测量数值
       imu660ra_get_gyro();                                                        // 获取 IMU660RA 的角速度测量数值
       Angle_Calcu();
       i=0;
     }
     if(flag_best==1)
     {
     high_control++;
     flag_balance_acc++;
     flag_balance_angle++;
     flag_balance_vel++;
     speed_flag++;
     hign_flag++;
     }
     if(flag_balance_acc>=2)
     {
         flag_balance_acc=0;
         R_acc_pid.err1 = Gyro_y + R_angle_pid.Out;
         L_acc_pid.err1 = Gyro_y + L_angle_pid.Out;
         PID_Positional(&R_acc_pid);
         PID_Positional(&L_acc_pid);
        turn_out=camare_turn_control();

         S_high_pid.err1 = -2 - Angle_X_Final;
         PID_Positional(&S_high_pid);


//
//         S_high_pid.Out += Gyro_x * 0.01;
//         S_high_pid.Out = S_high_pid.Out > 20 ? 20 : (S_high_pid.Out < (-20) ? (-20) : S_high_pid.Out);

         miuerr1 = S_high_pid.Out;

//         if((S_high_pid.Out) < 0)
//         {
//             miuerr1 = S_high_pid.Out;
//             miuerr2 = 0;
//
//         }
//         else
//         {
//             miuerr1 = 0;
//             miuerr2 = S_high_pid.Out;
//
//         }




         five_kill_left(L_vel_pid.Out+3 ,(float)(0));//L_vel_pid.Out,12-h_left_pid.Out  0 - miuerr1
         five_kill_right(R_vel_pid.Out+3 ,(float)(0));//R_vel_pid.Out,14+h_right_pid.Out  0 + miuerr1

         angle_change(angle_left_up,angle_left_down,angle_right_up,angle_right_down);
         servo_start();


         //         else
         //         {
         //             S_high_pid.err1=0;
         //             S_high_right_pid.err1=0;
         //
         //         }


         //         if((Angle_X_Final) > 0)
         //         {
         //             miuerr1 = bridge_roll_balance();
         //             miuerr2 = 0;
         //         }
         //         else
         //         {
         //             miuerr1 = 0;
         //             miuerr2 = bridge_roll_balance();
         //         }
     }
     if(flag_balance_angle>=5)
      {
         flag_balance_angle=0;
         R_angle_pid.err1 = -(Angle_Y_Final - balance_zero);
         L_angle_pid.err1 = -(Angle_Y_Final - balance_zero);
          PID_Positional(&R_angle_pid);
          PID_Positional(&L_angle_pid);
      }
     if(flag_balance_vel>=7)
     {

         flag_balance_vel=0;

         L_vel_pid.err1 = left_vel - (((float)(motor_value.receive_left_speed_data)*0.00340339f )-((float)(motor_value.receive_right_speed_data)*0.00340339f )/2);
         R_vel_pid.err1 = right_vel - (((float)(motor_value.receive_left_speed_data)*0.00340339f )-((float)(motor_value.receive_right_speed_data)*0.00340339f )/2);
         PID_Incremental(&R_vel_pid);
         PID_Incremental(&L_vel_pid);
     }
     if(hign_flag>2000)
     {
         left_vel = 2;
                 right_vel =2;
     }

     balance();
     gpio_set_level(P20_8, 1);












    // angle_left_up,angle_left_down,angle_right_up,angle_right_down
   //  angle_right_up

}

IFX_INTERRUPT(cc60_pit_ch1_isr, 0, CCU6_0_CH1_ISR_PRIORITY)
{
    interrupt_global_enable(0);                     // 开启中断嵌套
    pit_clear_flag(CCU60_CH1);



}
//if(high_control > 0 && high_control <110)
//      {
//          high_real=90;
//          high_true=90;
//      }
//      else if(high_control > 110)
//      {
//
//          high_real=0;
//          high_true=0;
//
//      }       else if(high_control > 300 )
//      {
//          high_real=40;
//           high_true=40;
//      }
//
//      else if(high_control > 320 )
//      {
//          middle1 = 40;
//          middle1 = middle1 - 2;
//          if(middle1 <= 0)     middle1 = 0;
//          high_real=middle1;
//          middle2 = 40;
//          middle2 = middle2 - 2;
//              if(middle2 <= 0)     middle2 = 0;
//              high_true=middle2;
//
//      }
//     else if(high_control >= 2000)
//      {
//         high_control=0;
//
//      }
//   if(speed_flag>=9)
//    {
//        speed_flag=0;
//
////       L_turn_pid.err1 = 94-final_mid_line;
////        R_turn_pid.err1 = 94-final_mid_line;
////
////        PID_Incremental(&R_turn_pid);
////        PID_Incremental(&L_turn_pid);
//    }
//  if(abs(94-final_mid_line)<=8)
//  {
//      high_real=13;
//      high_true=13;
//  }
//  if(94-final_mid_line>8)
//  {
//      high_real=13;
//        high_true=13+((94-final_mid_line+8)*0.7);
//  }
//  if(94-final_mid_line<-8)
//  {
//      high_real=13-((94-final_mid_line+8)*0.7);
//        high_true=13;
//  }
//  turn_real=kp*(94-final_mid_line)+kp2*(94-final_mid_line)*abs(94-final_mid_line);
//  }
//      h_right_pid.err1 = 94-final_mid_line;
//      h_left_pid.err1= 94-final_mid_line;
//      if(abs(94-final_mid_line)<50)
//      {
//          h_right_pid.err1 = 0;
//          h_left_pid.err1= 0;
//      }
   //       PID_Positional(&h_right_pid);
   //       PID_Positional(&h_left_pid);

   /*   if(fabs(94-final_mid_line)<=45)
      {
      high_real=12;
      high_true=12;
      }*/
    //if((R_turn_pid.err1-L_turn_pid.err1))
 // if(hign_flag>=11)
 // {
 //     hign_flag=0;

/*       PID_Positional(&h_right_pid);
      PID_Positional(&h_left_pid);
      if(h_right_pid.Out+12<0)
      {
          h_right_pid.Out=0;
      }
      if(12-h_left_pid.Out<0)
      {
          h_left_pid.Out=0;
      }*/
/*              if(speed_flag<5000)
              {
                  high=12;
 h_right_pid.err1 = -0.5- Angle_X_Final;
      h_left_pid.err1= -0.5- Angle_X_Final;

              }
              if(speed_flag>=5030)
               {
                  high=110;

               }
           if(speed_flag>=5115)
           {
               high=12;

           }
           if(speed_flag>=8000)
           {
               speed_flag=0;

           }*/

#pragma section all restore
// **************************** 代码区域 ****************************
