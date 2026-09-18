/*
 * control_xingjin.c
 *
 *  Created on: 2025年4月4日
 *      Author: 王开阳
 */

#include "control_xingjin.h"
//位置式
 PID_TypeDef R_vel_pid;
 PID_TypeDef R_angle_pid;
 PID_TypeDef R_acc_pid;
 PID_TypeDef L_vel_pid;
 PID_TypeDef L_angle_pid;
 PID_TypeDef L_acc_pid;
 PID_TypeDef h_left_pid;
 PID_TypeDef h_right_pid;
 PID_TypeDef L_turn_pid;
 PID_TypeDef R_turn_pid;
 PID_TypeDef  S_high_right_pid;
 PID_TypeDef   S_high_pid;
 int turn_real;
void PID_init(void)
{
    R_acc_pid.P_set = 15.0;
    R_acc_pid.I_set = 0.85;
    R_acc_pid.D_set = 0.0;
    R_acc_pid.I_max = 4000.0;
    R_acc_pid.Out_max = 5000.0;

    L_acc_pid.P_set = 15.0;
    L_acc_pid.I_set = 0.85;
    L_acc_pid.D_set = 0.0;
    L_acc_pid.I_max = 4000.0;
    L_acc_pid.Out_max = 5000.0;

    R_angle_pid.P_set = 1.0;
    R_angle_pid.I_set = 0.0;
    R_angle_pid.D_set = 1;
    R_angle_pid.I_max = 3000.0;
    R_angle_pid.Out_max = 5000.0;

    L_angle_pid.P_set = 1.0;
    L_angle_pid.I_set = 0.0;
    L_angle_pid.D_set = 1;
    L_angle_pid.I_max = 3000.0;
    L_angle_pid.Out_max = 5000.0;

    L_vel_pid.P_set = 0.6;//0.04
    L_vel_pid.I_set = 3.0;//0.0008
    L_vel_pid.D_set = 1.0;
    L_vel_pid.I_max = 15;
    L_vel_pid.Out_max = 15.0;

    R_vel_pid.P_set = 0.6;//0.035//0.015//0.04//0.028
    R_vel_pid.I_set = 3.0;//0.005//0.0015//0.0008//0.0013
    R_vel_pid.D_set = 1.0;
    R_vel_pid.I_max = 15;
    R_vel_pid.Out_max = 15.0;

    /*h_right_pid.P_set = 1.5;
    h_right_pid.I_set = 0.22;
    h_right_pid.D_set = 0;
    h_right_pid.I_max = 110;
    h_right_pid.Out_max = 110.0;

    h_left_pid.P_set = 1.5;
    h_left_pid.I_set = 0.22;
    h_left_pid.D_set = 0;
    h_left_pid.I_max = 110;
    h_left_pid.Out_max = 110.0;
*/
    S_high_pid.P_set = 0.5;
    S_high_pid.I_set = 0.05;
    S_high_pid.D_set = 0.0;
    S_high_pid.I_max = 20.0;
    S_high_pid.Out_max = 20.0;





}

void PID_Positional(PID_TypeDef *Data)
{
    Data->P_sum = Data->P_set * Data->err1;
    Data->I_sum += Data->I_set * Data->err1;
    if      (Data->I_sum >= Data->I_max)    Data->I_sum = Data->I_max;
    else if (Data->I_sum <= -Data->I_max) Data->I_sum = -Data->I_max;
    Data->D_sum = Data->D_set * (Data->err1 - Data->err2);

    Data->Out = Data->P_sum + Data->I_sum + Data->D_sum;
    if      (Data->Out >= Data->Out_max)    Data->Out = Data->Out_max;
    else if (Data->Out <= -Data->Out_max) Data->Out = -Data->Out_max;

    Data->err2 = Data->err1;
}


//增量式PID
void PID_Incremental(PID_TypeDef *Data)
{
    Data->P_sum = Data->P_set * (Data->err1 - Data->err2);
    Data->I_sum = Data->I_set * Data->err1;
    Data->D_sum = Data->D_set * (Data->err1 - 2 * Data->err2 + Data->err3);

    Data->Out = Data->P_sum + Data->I_sum + Data->D_sum;
    if      (Data->Out >= Data->Out_max)    Data->Out = Data->Out_max;
    else if (Data->Out <= -Data->Out_max) Data->Out = -Data->Out_max;

    Data->err3 = Data->err2;
    Data->err2 = Data->err1;
}




