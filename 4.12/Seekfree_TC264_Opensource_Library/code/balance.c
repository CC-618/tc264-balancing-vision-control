/*
 * balance.c
 *
 *  Created on: 2025年4月4日
 *      Author: 王开阳
 */
#include "balance.h"
#include "math.h"
#include "stdio.h"
//float angle_left_up=0;
//float angle_left_down=0;
//float angle_right_up=0;
//float angle_right_down=0;
int MotorA;
int MotorB;
int MotorA_stright;
int MotorB_stright;
signed short int MotorA_out=0;
signed short int MotorB_out=0;
int true_speed;
int servo_h;
int servo_hh;
int servo_str;
int start_servo_1=4220;
int start_servo_2=4720;
int start_servo_3=4720;
int start_servo_4=4920;
int servo_1_change;
int servo_2_change;
int servo_3_change;
int servo_4_change;

float my_abs(float a,float b)
{
    if(a > b)   return  (a - b);
    else        return  (b - a);
}


void five_kill_left(float x,float y)
{
        int Leg1=61;
        int Leg2=91;
        int distance=38;
        float angle1,angle2;
        float angle12,angle22;
        float a,b;
        float a1,b1;
        float temp;
        a = sqrtf(x * x + (y + 30) * (y + 30));
           b = sqrtf((x + distance/2) * (x + distance/2) + (y + 30) * (y + 30));
           angle1 = acosf((distance/2.0f * distance/2.0f + b*b - a*a) / (distance * b));
           angle2 = acosf((Leg1 * Leg1 + b*b - Leg2*Leg2) / (2.0f * Leg1 * b));
           temp = 180.0f - (angle1 * 180.0f / 3.14 + angle2 * 180.0f / 3.14);
           if( my_abs(temp, angle_left_up) < 200) angle_left_up = temp;

           // 计算第二组角度
           a1 = sqrtf(x*x + (y+30)*(y+30)); // 注意：a1与a计算相同，可能是笔误?
           b1 = sqrtf((x - distance/2)*(x - distance/2) + (y+30)*(y+30));
           angle12 = acosf((distance/2.0f*distance/2.0f + b1*b1 - a1*a1) / (distance*b1));
           angle22 = acosf((Leg1*Leg1 + b1*b1 - Leg2*Leg2) / (2.0f*Leg1*b1));
           temp = 180.0f - (angle12*180.0f/3.14 + angle22*180.0f/3.14);
           if( my_abs(temp, angle_left_down) < 200) angle_left_down = temp;
//           a=(sqrt((x) * (x) + (y+30) * (y+30)));
//           b=sqrt(((x) + distance/2) * ((x) + distance/2) + (y+30) * (y+30));
//           angle1=acos((distance/2 * distance/2 +(b*b) - (a*a))/(distance * b));
//           angle2=acos((Leg1 * Leg1+ (b*b) - (Leg2*Leg2) )/( 2* Leg1 * b));
//           angle_left_up=180- (angle1 * 180/ 3.14 +angle2 * 180 /3.14);
//           a1=sqrt((x)*(x)+(y+30)*(y+30));
//           b1=sqrt(((x)-distance/2)*((x)-distance/2)+(y+30)*(y+30));
//           angle12=acos((distance/2*distance/2+(b1*b1)-(a1*a1))/(distance*b1));
//           angle22=acos((Leg1*Leg1+b1*b1-Leg2*Leg2)/(2*Leg1*b1));
//           angle_left_down=180-(angle12*180/3.14+angle22*180/3.14);

}
void five_kill_right(float x,float y)
{
            int Leg1=61;
            int Leg2=91;
            int distance=38;
            float angle1,angle2;
            float angle12,angle22;
            float a,b;
            float a1,b1;
            float temp;
            a = sqrtf(x * x + (y + 30) * (y + 30));
                b = sqrtf((x + distance/2) * (x + distance/2) + (y + 30) * (y + 30));
                angle1 = acosf((distance/2.0f * distance/2.0f + b*b - a*a) / (distance * b));
                angle2 = acosf((Leg1 * Leg1 + b*b - Leg2*Leg2) / (2.0f * Leg1 * b));
                temp = 180.0f - (angle1 * 180.0f / 3.14 + angle2 * 180.0f / 3.14);
                if( my_abs(temp, angle_right_up) < 200) angle_right_up = temp;

                // 计算第二组角度
                a1 = sqrtf(x*x + (y+30)*(y+30)); // 注意：a1与a计算相同，可能是笔误?
                b1 = sqrtf((x - distance/2)*(x - distance/2) + (y+30)*(y+30));
                angle12 = acosf((distance/2.0f*distance/2.0f + b1*b1 - a1*a1) / (distance*b1));
                angle22 = acosf((Leg1*Leg1 + b1*b1 - Leg2*Leg2) / (2.0f*Leg1*b1));
                temp = 180.0f - (angle12*180.0f/3.14 + angle22*180.0f/3.14);
                if( my_abs(temp, angle_right_down) < 200) angle_right_down = temp;

//            a=(sqrt((x) * (x) + (y+30) * (y+30)));
//             b=sqrt(((x) + distance/2) * ((x) + distance/2) + (y + 30) * (y + 30));
//             angle1=acos((distance/2 * distance/2 + b * b - a * a) / (distance * b));
//             angle2=acos((Leg1 * Leg1 +( b * b) - (Leg2 * Leg2)) / (2 * Leg1 * b));
//             angle_right_up=180 - (angle1 * 180 / 3.14 + angle2 * 180 / 3.14);
//             a1=sqrt((x)*(x)+(y+30)*(y+30));
//             b1=sqrt(((x)-distance/2)*((x)-distance/2)+(y+30)*(y+30));
//             angle12=acos((distance/2*distance/2+(b1*b1)-(a1*a1))/(distance*b1));
//             angle22=acos((Leg1*Leg1+(b1*b1)-(Leg2*Leg2))/(2*Leg1*b1));
//             angle_right_down=180-(angle12*180/3.14+angle22*180/3.14);


}
void angle_change(float angle_left,float angle_right,float angle_left_other,float angle_right_other)
{

    servo_1_change=(int)((((angle_left )*0.00333)+0.05)*10000);
    servo_2_change=(int)((((angle_right)*0.00333)+0.05)*10000);
    servo_3_change=(int)((((angle_right_other)*0.00333)+0.05)*10000);
    servo_4_change=(int)((((angle_left_other)*0.00333)+0.05)*10000);
}
void servo_start(void)
{
        pwm_set_duty(PWM_CH1,start_servo_1+servo_1_change);//4720//大向下  1号最大6520  最小2420
        pwm_set_duty(PWM_CH2,start_servo_2-servo_2_change);//4720//小向下//2号最小2220  最大6620
        pwm_set_duty(PWM_CH3,start_servo_3+servo_3_change);//4720//大向下//3号最大7220  最小2620
        pwm_set_duty(PWM_CH4,start_servo_4-servo_4_change);//4720//小向下//4号最小2520  最大6620
//        pwm_set_duty(PWM_CH1,start_servo_1);//4720//大向下  1号最大6520  最小2420
//               pwm_set_duty(PWM_CH2,start_servo_2);//4720//小向下//2号最小2220  最大6620
//               pwm_set_duty(PWM_CH3,start_servo_3);//4720//大向下//3号最大7220  最小2620
//               pwm_set_duty(PWM_CH4,start_servo_4);//4720//小向下//4号最小2520  最大6620

}
void motor_max(void)
{
         if(MotorA > 0)
             MotorA = MotorA + 30;
          else if(MotorA < 0)
              MotorA = MotorA -30;
          if(MotorB > 0)
              MotorB = MotorB + 30;
           else if(MotorB < 0)
              MotorB = MotorB - 30;
          if(MotorA_out > 6000)
              MotorA_out=6000;
              else if(MotorA_out < -6000)
                  MotorA_out = -6000;
          if(MotorB_out > 6000)
              MotorB_out=6000;
                   else if(MotorB_out < -6000)
                       MotorB_out = -6000;

}
void balance_init(void)
{

}
//static inline float fun_abs(float num)
//{
//    if(num >= 0)        return  num;
//    else                return -num;
//}
//float sv_p = 0.5,sr_i = 0.1;
//static float output,Sum;
//
//float bridge_roll_balance(void)
//{
//
//    float bias = Angle_X_Final;
//    //if(fun_abs(bias) <= 1.5)        bias = 0;
//    Sum += bias;
//    Sum = Sum > 50 ? 50 : (Sum < (-50) ? (-50) : Sum);
//    output = sv_p * bias + sr_i * Sum;
//    output = output > 50 ? 50 : (output < (-50) ? (-50) : output);
//
//    return -output;
//}

int turn_out;
const float Turn_p1 = 1.2,Turn_d = 0;            //转向环参数Turn_p = -55  Turn_d = -10
const int8 GDk = 5,Turn_i = -30;
float camare_turn_control(void)
{
    float output,bias;
    float L_bias = 0;
    float Sum = 0;
    bias = 94-final_mid_line;
    Sum += bias;
    L_bias = bias;
    output = Turn_p1 * bias + Turn_d * (bias - L_bias) +
             GDk * Gyro_z + Sum * Turn_i;
    return -output;
}
void balance(void)
{


       motor_max();
     // small_driver_set_duty(L_acc_pid.Out,-R_acc_pid.Out);
       small_driver_set_duty(L_acc_pid.Out+turn_out,-R_acc_pid.Out+turn_out);

  //     small_driver_set_duty(L_acc_pid.Out+turn_real,-R_acc_pid.Out+turn_real);
}


