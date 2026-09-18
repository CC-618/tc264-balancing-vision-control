/*
 * kalaman.h
 *
 *  Created on: 2025年4月4日
 *      Author: 王开阳
 */

#ifndef CODE_KALAMAN_H_
#define CODE_KALAMAN_H_

#include "zf_common_headfile.h"

extern signed short int aacx,aacy,aacz;      //加速度传感器原始数据  angular acceleration
extern signed short int gyrox,gyroy,gyroz;       //陀螺仪原始数据  gyroscope
extern float Accel_x;              //X轴加速度值暂存
extern float Accel_y;              //Y轴加速度值暂存
extern float Accel_z;              //Z轴加速度值暂存
extern float Gyro_x;               //X轴陀螺仪数据暂存
extern float Gyro_y;               //Y轴陀螺仪数据暂存
extern float Gyro_z;               //Z轴陀螺仪数据暂存
extern float Angle_x_temp;
extern float Angle_y_temp;
extern float Angle_z_temp;         //由加速度计算的y倾斜角度
extern float Angle_X_Final;        //X最终倾斜角度
extern float Angle_Y_Final;        //Y最终倾斜角度

extern float Q_bias;


void Angle_Calcu(void);
float Kalman_Filter_x(float Accel,float Gyro);
float Kalman_Filter_y(float Accel,float Gyro);
float Complementary_Filter_y(float angle_m, float gyro_m);
float Complementary_Filter_x(void);




#endif /* CODE_KALAMAN_H_ */
