/*
 * balance.h
 *
 *  Created on: 2025年4月4日
 *      Author: 王开阳
 */

#ifndef CODE_BALANCE_H_
#define CODE_BALANCE_H_

#include "zf_common_headfile.h"
extern float angle_left_up;
extern float angle_left_down;
extern float angle_right_up;
extern float angle_right_down;
extern int MotorA;
extern int MotorB;
extern int true_speed;
extern int turn_out;
extern int servo_1_change;
extern int servo_2_change;
extern int servo_3_change;
extern int servo_4_change;
void encoder_speed(void);
void balance(void);
void balance_init(void);
void servo_start(void);
void five_kill_left(float x,float y);
void five_kill_right(float x,float y);
void angle_change(float angle_left,float angle_right,float angle_left_other,float angle_right_other);
float my_abs(float a,float b);
float camare_turn_control(void);
float bridge_roll_balance(void);
void servo_start(void);
#endif /* CODE_BALANCE_H_ */
