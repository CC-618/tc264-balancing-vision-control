/*
 * control_xingjin.h
 *
 *  Created on: 2025年4月4日
 *      Author: 王开阳
 */

#ifndef CODE_CONTROL_XINGJIN_H_
#define CODE_CONTROL_XINGJIN_H_



#include "zf_common_headfile.h"

typedef struct
{
    float   err1;
    float   err2;
    float   err3;
    float   P_set;
    float   I_set;
    float   D_set;
    float   P_sum;
    float   I_sum;
    float   D_sum;
    float   Out;
    float   Out_last;
    float   I_max;
    float   Out_max;
}PID_TypeDef;
extern PID_TypeDef R_vel_pid;
extern PID_TypeDef R_angle_pid;
extern PID_TypeDef R_acc_pid;
extern PID_TypeDef L_vel_pid;
extern PID_TypeDef L_angle_pid;
extern PID_TypeDef L_acc_pid;
extern PID_TypeDef h_left_pid;
extern PID_TypeDef h_right_pid;
extern PID_TypeDef L_turn_pid;
extern PID_TypeDef R_turn_pid;
extern PID_TypeDef  S_high_right_pid;
extern PID_TypeDef   S_high_pid;
extern int turn_real;
void PID_init(void);
void PID_Positional(PID_TypeDef *Data);
void PID_Incremental(PID_TypeDef *Data);
#endif /* CODE_CONTROL_XINGJIN_H_ */
