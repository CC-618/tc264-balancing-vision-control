/*
 * camear.h
 *
 *  Created on: 2025年4月27日
 *      Author: 王开阳
 */

#ifndef CODE_CAMEAR_H_
#define CODE_CAMEAR_H_

#define image_h 120//图像高度
#define image_w 188//图像宽度

#define white_pixel 255
#define black_pixel 0

#define bin_jump_num    1//跳过的点数
#define border_max  image_w-2 //边界最大值
#define border_min  1   //边界最小值

#define MID_W                   94              //赛道中值
#define search_start_line       120             //搜线起始行
#define search_end_line         30              //搜线停止行
#define USE_num image_h*3   //定义找点的数组成员个数按理说300个点能放下，但是有些特殊情况确实难顶，多定义了一点

enum Color {
    left,
    right
};

extern uint8 l_border[image_h];
extern uint8 r_border[image_h];
extern uint8 center_line[image_h];
extern unsigned char  base_image[MT9V03X_H][MT9V03X_W];

extern uint16 points_l[(uint16)USE_num][2];//左线
extern uint16 points_r[(uint16)USE_num][2];//右线

extern uint16 points_l_x[(uint16)USE_num];//左线
extern uint16 points_l_y[(uint16)USE_num];//左线
extern uint16 points_r_X[(uint16)USE_num];//右线
extern uint16 points_r_y[(uint16)USE_num];//右线

extern unsigned char original_image[image_h][image_w];
extern unsigned char bin_image[image_h][image_w];//图像数组
extern uint8 final_mid_line;
extern void image_process(void); //直接在中断或循环里调用此程序就可以循环执行了



//
//extern unsigned char Image_use_zip[IMAGE_H][IMAGE_W];
//
//void image_draw_rectan(unsigned char(*image)[IMAGE_W]);
#endif /* CODE_CAMEAR_H_ */
