/*
 * camear.c
 *
 *  Created on: 2025年4月27日
 *      Author: 王开阳
 */


#include "zf_common_headfile.h"

uint8 l_border[image_h];//左线数组
uint8 r_border[image_h];//右线数组
uint8 center_line[image_h];//中线数组

unsigned char  base_image[MT9V03X_H][MT9V03X_W];    // 图像原始数据传到此数组使用，防止图像割裂

//
///*
//函数名称：int my_abs(int value)
//功能说明：求绝对值
//参数说明：
//函数返回：绝对值
//备    注：
//example：  my_abs( x)；
// */
int My_abs(int value)
{
if(value>=0) return value;
else return -value;
}
//
int limit_a_b(int x, int a, int b)
{
    if(x < a)  x = a;
    if(x > b ) x = b;
    return x;
}
/************************************************************
【函数名称】connect_line
【功    能】连线函数
【参    数】两点横纵坐标
【返 回 值】无
【实    例】connect_line(l_border,0,0,h_num,l_border[h_num]);
【注意事项】x1 > x2
************************************************************/
void connect_line(uint8 *array_value,uint8 x1,uint8 y1,uint8 x2,uint8 y2)
{
    float k;
    if(y1 == y2)
    {
        for(uint8 i = 0; i < x1-x2; i++)
        {
            array_value[x2+i] = (int)y2;
            array_value[x2+i] = (uint8)limit_a_b(array_value[x2+i],0,MT9V03X_W-1);
        }
    }
    if(x1 != x2 && y1 != y2)
    {
        k = (1.0*(y1-y2))/(1.0*(x1-x2));
        for(uint8 i = 0; i < x1-x2; i++)
        {
            array_value[x2+i] = (int)( y2 + i*k);
            array_value[x2+i] = (uint8)limit_a_b(array_value[x2+i],0,MT9V03X_W-1);
        }
    }
}
//
///*
//函数名称：int16 limit(int16 x, int16 y)
//功能说明：求x,y中的最小值
//参数说明：
//函数返回：返回两值中的最小值
//备    注：
//example：  limit( x,  y)
// */
int16 limit1(int16 x, int16 y)
{
    if (x > y)             return y;
    else if (x < -y)       return -y;
    else                return x;
}


/*变量声明*/
unsigned char original_image[image_h][image_w];
unsigned char image_thereshold;//图像分割阈值
//------------------------------------------------------------------------------------------------------------------
//  @brief      获得一副灰度图像
//  @since      v1.0
//------------------------------------------------------------------------------------------------------------------
void Get_image(unsigned char(*mt9v03x_image)[image_w])
{
#define use_num     1   //1就是不压缩，2就是压缩一倍
    uint8 i = 0, j = 0, row = 0, line = 0;
    for (i = 0; i < 120; i += use_num)          //
    {
        for (j = 0; j <188; j += use_num)     //
        {
            original_image[row][line] = mt9v03x_image[i][j];//这里的参数填写你的摄像头采集到的图像
            line++;
        }
        line = 0;
        row++;
    }
}
//------------------------------------------------------------------------------------------------------------------
//  @brief     动态阈值
//  @since      v1.0
//------------------------------------------------------------------------------------------------------------------
uint8 otsuThreshold(uint8 *image, uint16 col, uint16 row)
{
#define GrayScale 256
    uint16 Image_Width  = col;
    uint16 Image_Height = row;
    int X; uint16 Y;
    uint8* data = image;
    int HistGram[GrayScale] = {0};

    uint32 Amount = 0;
    uint32 PixelBack = 0;
    uint32 PixelIntegralBack = 0;
    uint32 PixelIntegral = 0;
    int32 PixelIntegralFore = 0;
    int32 PixelFore = 0;
    double OmegaBack=0, OmegaFore=0, MicroBack=0, MicroFore=0, SigmaB=0, Sigma=0; // 类间方差;
    uint8 MinValue=0, MaxValue=0;
    uint8 Threshold = 0;


    for (Y = 0; Y <Image_Height; Y++) //Y<Image_Height改为Y =Image_Height；以便进行 行二值化
    {
        //Y=Image_Height;
        for (X = 0; X < Image_Width; X++)
        {
        HistGram[(int)data[Y*Image_Width + X]]++; //统计每个灰度值的个数信息
        }
    }
    for (MinValue = 0; MinValue < 256 && HistGram[MinValue] == 0; MinValue++) ;        //获取最小灰度的值
    for (MaxValue = 255; MaxValue > MinValue && HistGram[MinValue] == 0; MaxValue--) ; //获取最大灰度的值
    if (MaxValue == MinValue)
    {
        return MaxValue;          // 图像中只有一个颜色
    }
    if (MinValue + 1 == MaxValue)
    {
        return MinValue;      // 图像中只有二个颜色
    }

    for (Y = MinValue; Y <= MaxValue; Y++)
    {
        Amount += HistGram[Y];        //  像素总数
    }

    PixelIntegral = 0;
    for (Y = MinValue; Y <= MaxValue; Y++)
    {
        PixelIntegral += HistGram[Y] * Y;//灰度值总数
    }
    SigmaB = -1;
    for (Y = MinValue; Y < MaxValue; Y++)
    {
          PixelBack = PixelBack + HistGram[Y];    //前景像素点数
          PixelFore = Amount - PixelBack;         //背景像素点数
          OmegaBack = (double)PixelBack / Amount;//前景像素百分比
          OmegaFore = (double)PixelFore / Amount;//背景像素百分比
          PixelIntegralBack += HistGram[Y] * Y;  //前景灰度值
          PixelIntegralFore = PixelIntegral - PixelIntegralBack;//背景灰度值
          MicroBack = (double)PixelIntegralBack / PixelBack;//前景灰度百分比
          MicroFore = (double)PixelIntegralFore / PixelFore;//背景灰度百分比
          Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);//g
          if (Sigma > SigmaB)//遍历最大的类间方差g
          {
              SigmaB = Sigma;
              Threshold = (uint8)Y;
          }
    }
   return Threshold;
}
//------------------------------------------------------------------------------------------------------------------
//  @brief      图像二值化，这里用的是大津法二值化。
//  @since      v1.0
//------------------------------------------------------------------------------------------------------------------
uint8 bin_image[image_h][image_w];//图像数组
void turn_to_bin(void)
{
  uint8 i,j;
 image_thereshold = otsuThreshold(original_image[0], image_w, image_h);
  for(i = 0;i<image_h;i++)
  {
      for(j = 0;j<image_w;j++)
      {
          if(original_image[i][j]>image_thereshold)bin_image[i][j] = white_pixel;
          else bin_image[i][j] = black_pixel;
      }
  }
}


/*
函数名称：void get_start_point(uint8 start_row)
功能说明：寻找两个边界的边界点作为八邻域循环的起始点
参数说明：输入任意行数
函数返回：无
备    注：
example：  get_start_point(image_h-2)
// */
uint8 start_point_l[2] = { 0 };//左边起点的x，y值
uint8 start_point_r[2] = { 0 };//右边起点的x，y值
uint8 get_start_point(uint8 start_row)
{
    uint8 i = 0,l_found = 0,r_found = 0;
    //清零
    start_point_l[0] = 0;//x
    start_point_l[1] = 0;//y

    start_point_r[0] = 0;//x
    start_point_r[1] = 0;//y

        //从中间往左边，先找起点
    for (i = image_w / 2; i > border_min; i--)
    {
        start_point_l[0] = i;//x
        start_point_l[1] = start_row;//y
        if (bin_image[start_row][i] == 255 && bin_image[start_row][i - 1] == 0)
        {
            //printf("找到左边起点image[%d][%d]\n", start_row,i);
            l_found = 1;
            break;
        }
    }

    for (i = image_w / 2; i < border_max; i++)
    {
        start_point_r[0] = i;//x
        start_point_r[1] = start_row;//y
        if (bin_image[start_row][i] == 255 && bin_image[start_row][i + 1] == 0)
        {
            //printf("找到右边起点image[%d][%d]\n",start_row, i);
            r_found = 1;
            break;
        }
    }

    if(l_found&&r_found)return 1;
    else {
        //printf("未找到起点\n");
        return 0;
    }
}

///*
//函数名称：void search_l_r(uint16 break_flag, uint8(*image)[image_w],uint16 *l_stastic, uint16 *r_stastic,
//                            uint8 l_start_x, uint8 l_start_y, uint8 r_start_x, uint8 r_start_y,uint8*hightest)
//
//功能说明：八邻域正式开始找右边点的函数，输入参数有点多，调用的时候不要漏了，这个是左右线一次性找完。
//参数说明：
//break_flag_r            ：最多需要循环的次数
//(*image)[image_w]       ：需要进行找点的图像数组，必须是二值图,填入数组名称即可
//                       特别注意，不要拿宏定义名字作为输入参数，否则数据可能无法传递过来
//*l_stastic              ：统计左边数据，用来输入初始数组成员的序号和取出循环次数
//*r_stastic              ：统计右边数据，用来输入初始数组成员的序号和取出循环次数
//l_start_x               ：左边起点横坐标
//l_start_y               ：左边起点纵坐标
//r_start_x               ：右边起点横坐标
//r_start_y               ：右边起点纵坐标
//hightest                ：循环结束所得到的最高高度
//函数返回：无
//备    注：
//example：
//    search_l_r((uint16)USE_num,image,&data_stastics_l, &data_stastics_r,start_point_l[0],
//                start_point_l[1], start_point_r[0], start_point_r[1],&hightest);
// */

//
uint16 points_l_x[(uint16)USE_num] = { 0 };//左线
uint16 points_l_y[(uint16)USE_num] = { 0 };//左线
uint16 points_r_X[(uint16)USE_num] = { 0 };//右线
uint16 points_r_y[(uint16)USE_num] = { 0 };//右线
// //存放点的x，y坐标
uint16 points_l[(uint16)USE_num][2] = { {  0 } };//左线
uint16 points_r[(uint16)USE_num][2] = { {  0 } };//右线
uint16 dir_r[(uint16)USE_num] = { 0 };//用来存储右边生长方向
uint16 dir_l[(uint16)USE_num] = { 0 };//用来存储左边生长方向
uint16 data_stastics_l = 0;//统计左边找到点的个数
uint16 data_stastics_r = 0;//统计右边找到点的个数
uint8 hightest = 0;//最高点
void search_l_r(uint16 break_flag, uint8(*image)[image_w], uint16 *l_stastic, uint16 *r_stastic, uint8 l_start_x, uint8 l_start_y, uint8 r_start_x, uint8 r_start_y, uint8*hightest)
{

    uint8 i = 0, j = 0;

    //左边变量
    uint8 search_filds_l[8][2] = { {  0 } };
    uint8 index_l = 0;
    uint8 temp_l[8][2] = { {  0 } };
    uint8 center_point_l[2] = {  0 };
    uint16 l_data_statics;//统计左边
    //定义八个邻域
    static int8 seeds_l[8][2] = { {0,  1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1},{1,  0},{1, 1}, };
    //{-1,-1},{0,-1},{+1,-1},
    //{-1, 0},       {+1, 0},
    //{-1,+1},{0,+1},{+1,+1},
    //这个是顺时针

    //右边变量
    uint8 search_filds_r[8][2] = { {  0 } };
    uint8 center_point_r[2] = { 0 };//中心坐标点
    uint8 index_r = 0;//索引下标
    uint8 temp_r[8][2] = { {  0 } };
    uint16 r_data_statics;//统计右边
    //定义八个邻域
    static int8 seeds_r[8][2] = { {0,  1},{1,1},{1,0}, {1,-1},{0,-1},{-1,-1}, {-1,  0},{-1, 1}, };
    //{-1,-1},{0,-1},{+1,-1},
    //{-1, 0},       {+1, 0},
    //{-1,+1},{0,+1},{+1,+1},
    //这个是逆时针

    l_data_statics = *l_stastic;//统计找到了多少个点，方便后续把点全部画出来
    r_data_statics = *r_stastic;//统计找到了多少个点，方便后续把点全部画出来

    //第一次更新坐标点  将找到的起点值传进来
    center_point_l[0] = l_start_x;//x
    center_point_l[1] = l_start_y;//y
    center_point_r[0] = r_start_x;//x
    center_point_r[1] = r_start_y;//y

        //开启邻域循环
    while (break_flag--)
    {

        //左边
        for (i = 0; i < 8; i++)//传递8F坐标
        {
            search_filds_l[i][0] = center_point_l[0] + seeds_l[i][0];//x
            search_filds_l[i][1] = center_point_l[1] + seeds_l[i][1];//y
        }
        //中心坐标点填充到已经找到的点内
        points_l[l_data_statics][0] = center_point_l[0];//x
        points_l[l_data_statics][1] = center_point_l[1];//y
        l_data_statics++;//索引加一

        //右边
        for (i = 0; i < 8; i++)//传递8F坐标
        {
            search_filds_r[i][0] = center_point_r[0] + seeds_r[i][0];//x
            search_filds_r[i][1] = center_point_r[1] + seeds_r[i][1];//y
        }
        //中心坐标点填充到已经找到的点内
        points_r[r_data_statics][0] = center_point_r[0];//x
        points_r[r_data_statics][1] = center_point_r[1];//y

        index_l = 0;//先清零，后使用
        for (i = 0; i < 8; i++)
        {
            temp_l[i][0] = 0;//先清零，后使用
            temp_l[i][1] = 0;//先清零，后使用
        }

        //左边判断
        for (i = 0; i < 8; i++)
        {
            if (image[search_filds_l[i][1]][search_filds_l[i][0]] == 0
                && image[search_filds_l[(i + 1) & 7][1]][search_filds_l[(i + 1) & 7][0]] == 255)
            {
                temp_l[index_l][0] = search_filds_l[(i)][0];
                temp_l[index_l][1] = search_filds_l[(i)][1];
                index_l++;
                dir_l[l_data_statics - 1] = (i);//记录生长方向
            }

            if (index_l)
            {
                //更新坐标点
                center_point_l[0] = temp_l[0][0];//x
                center_point_l[1] = temp_l[0][1];//y
                for (j = 0; j < index_l; j++)
                {
                    if (center_point_l[1] > temp_l[j][1])
                    {
                        center_point_l[0] = temp_l[j][0];//x
                        center_point_l[1] = temp_l[j][1];//y
                    }
                }
            }

        }
        if ((points_r[r_data_statics][0]== points_r[r_data_statics-1][0]&& points_r[r_data_statics][0] == points_r[r_data_statics - 2][0]
            && points_r[r_data_statics][1] == points_r[r_data_statics - 1][1] && points_r[r_data_statics][1] == points_r[r_data_statics - 2][1])
            ||(points_l[l_data_statics-1][0] == points_l[l_data_statics - 2][0] && points_l[l_data_statics-1][0] == points_l[l_data_statics - 3][0]
                && points_l[l_data_statics-1][1] == points_l[l_data_statics - 2][1] && points_l[l_data_statics-1][1] == points_l[l_data_statics - 3][1]))
        {
            //printf("三次进入同一个点，退出\n");
            break;
        }
        if (My_abs(points_r[r_data_statics][0] - points_l[l_data_statics - 1][0]) < 2
            && My_abs(points_r[r_data_statics][1] - points_l[l_data_statics - 1][1] < 2)
            )
        {
            //printf("\n左右相遇退出\n");
            *hightest = (points_r[r_data_statics][1] + points_l[l_data_statics - 1][1]) >> 1;//取出最高点
            //printf("\n在y=%d处退出\n",*hightest);
            break;
        }
        if ((points_r[r_data_statics][1] < points_l[l_data_statics - 1][1]))
        {
           // printf("\n如果左边比右边高了，左边等待右边\n");
            continue;//如果左边比右边高了，左边等待右边
        }
        if (dir_l[l_data_statics - 1] == 7
            && (points_r[r_data_statics][1] > points_l[l_data_statics - 1][1]))//左边比右边高且已经向下生长了
        {
            //printf("\n左边开始向下了，等待右边，等待中... \n");
            center_point_l[0] = points_l[l_data_statics - 1][0];//x
            center_point_l[1] = points_l[l_data_statics - 1][1];//y
            l_data_statics--;
        }
        r_data_statics++;//索引加一

        index_r = 0;//先清零，后使用
        for (i = 0; i < 8; i++)
        {
            temp_r[i][0] = 0;//先清零，后使用
            temp_r[i][1] = 0;//先清零，后使用
        }

        //右边判断
        for (i = 0; i < 8; i++)
        {
            if (image[search_filds_r[i][1]][search_filds_r[i][0]] == 0
                && image[search_filds_r[(i + 1) & 7][1]][search_filds_r[(i + 1) & 7][0]] == 255)
            {
                temp_r[index_r][0] = search_filds_r[(i)][0];
                temp_r[index_r][1] = search_filds_r[(i)][1];
                index_r++;//索引加一
                dir_r[r_data_statics - 1] = (i);//记录生长方向
                //printf("dir[%d]:%d\n", r_data_statics - 1, dir_r[r_data_statics - 1]);
            }
            if (index_r)
            {

                //更新坐标点
                center_point_r[0] = temp_r[0][0];//x
                center_point_r[1] = temp_r[0][1];//y
                for (j = 0; j < index_r; j++)
                {
                    if (center_point_r[1] > temp_r[j][1])
                    {
                        center_point_r[0] = temp_r[j][0];//x
                        center_point_r[1] = temp_r[j][1];//y
                    }
                }

            }
        }


    }


    //取出循环次数
    *l_stastic = l_data_statics;
    *r_stastic = r_data_statics;

}
///*
//函数名称：void get_left(uint16 total_L)
//功能说明：从八邻域边界里提取需要的边线
//参数说明：
//total_L ：找到的点的总数
//函数返回：无
//备    注：
//example： get_left(data_stastics_l );
// */

void get_left(uint16 total_L)
{
    uint8 i = 0;
//    uint16 j = 0;
    uint8 h = 0;
    //初始化
    for (i = 0;i<image_h;i++)
    {
        l_border[i] = border_min;
    }
    h = image_h - 2;
    //左边
    // 遍历所有点，筛选每行的最大x值
    for (uint16 j = 0; j < total_L; j++)
    {
        uint16 y = points_l[j][1];
        uint16 x = points_l[j][0];

        // 边界保护：确保行号有效
        if (y >= image_h) continue;

        // 左边线应取最大x值
        if (x > l_border[y]) {
            l_border[y] = (uint8)(x + 1);  // 直接记录实际坐标
        }
    }
//    for (j = 0; j < total_L; j++)
//    {
//        //printf("%d\n", j);
//        if (points_l[j][1] == h)
//        {
//            l_border[h] = points_l[j][0]+1;
//        }
//        else continue; //每行只取一个点，没到下一行就不记录
//        h--;
//        if (h == 0)
//        {
//            break;//到最后一行退出
//        }
//    }
}
///*
//函数名称：void get_right(uint16 total_R)
//功能说明：从八邻域边界里提取需要的边线
//参数说明：
//total_R  ：找到的点的总数
//函数返回：无
//备    注：
//example：get_right(data_stastics_r);
// */
void get_right(uint16 total_R)
{
    uint8 i = 0;
    uint16 j = 0;
    uint8 h = 0;
    for (i = 0; i < image_h; i++)
    {
        r_border[i] = border_max;//右边线初始化放到最右边，左边线放到最左边，这样八邻域闭合区域外的中线就会在中间，不会干扰得到的数据
    }
    h = image_h - 2;
    for (uint16 j = 0; j < total_R; j++)
    {
        uint16 y = points_r[j][1];
        uint16 x = points_r[j][0];

        // 边界保护：确保行号有效
        if (y >= image_h) continue;

        // 左边线应取最小x值
        if (x < r_border[y]) {
            r_border[y] = (uint8)(x - 1);  // 直接记录实际坐标
        }
    }
    //右边
//    for (j = 0; j < total_R; j++)
//    {
//        if (points_r[j][1] == h)
//        {
//            r_border[h] = points_r[j][0] - 1;
//        }
//        else continue;//每行只取一个点，没到下一行就不记录
//        h--;
//        if (h == 0)break;//到最后一行退出
//    }
}

////定义膨胀和腐蚀的阈值区间
#define threshold_max   255*5//此参数可根据自己的需求调节
#define threshold_min   255*2//此参数可根据自己的需求调节
void image_filter(uint8(*bin_image)[image_w])//形态学滤波，简单来说就是膨胀和腐蚀的思想
{
    uint16 i, j;
    uint32 num = 0;


    for (i = 1; i < image_h - 1; i++)
    {
        for (j = 1; j < (image_w - 1); j++)
        {
            //统计八个方向的像素值
            num =
                bin_image[i - 1][j - 1] + bin_image[i - 1][j] + bin_image[i - 1][j + 1]
                + bin_image[i][j - 1] + bin_image[i][j + 1]
                + bin_image[i + 1][j - 1] + bin_image[i + 1][j] + bin_image[i + 1][j + 1];


            if (num >= threshold_max && bin_image[i][j] == 0)
            {

                bin_image[i][j] = 255;//白  可以搞成宏定义，方便更改

            }
            if (num <= threshold_min && bin_image[i][j] == 255)
            {

                bin_image[i][j] = 0;//黑

            }

        }
    }
}
//
///*
//函数名称：void image_draw_rectan(uint8(*image)[image_w])
//功能说明：给图像画一个黑框
//参数说明：uint8(*image)[image_w] 图像首地址
//函数返回：无
//备    注：
//example： image_draw_rectan(bin_image);
// */
void image_draw_rectan(uint8(*image)[image_w])
{

    uint8 i = 0;
    for (i = 0; i < image_h; i++)
    {
        image[i][0] = 0;
        image[i][1] = 0;
        image[i][image_w - 1] = 0;
        image[i][image_w - 2] = 0;

    }
    for (i = 0; i < image_w; i++)
    {
        image[0][i] = 0;
        image[1][i] = 0;
        //image[image_h-1][i] = 0;

    }
}
//W矩阵参数
//60*80
#define a11 (-8.0769f)
#define a12 (-0.1923f)
#define a13 (336.1538f)
#define a21 (0.0f)
#define a22 (2.7885f)
#define a23 (-440.0f)
#define a31 (-0.0f)
#define a32 (-0.2404f)
#define a33 (1.0f)
#define getx(u,v) (a11*(u)+a12*(v)+a13)
#define gety(u,v) (a21*(u)+a22*(v)+a23)
#define getw(u,v) (a31*(u)+a32*(v)+a33)
//D矩阵(求逆矩阵)
//由俯视图改摄像头
//60*80
#define b11 (-0.1238f)
#define b12 (-0.0969f)
#define b13 (-1.0252f)
#define b21 ( 0.0000f)
#define b22 (-0.0097f)
#define b23 (-4.2726f)
#define b31 (-0.0000f)
#define b32 (-0.0023f)
#define b33 (-0.0271f)
#define getx_b(u,v) (b11*(u)+b12*(v)+b13)
#define gety_b(u,v) (b21*(u)+b22*(v)+b23)
#define getw_b(u,v) (b31*(u)+b32*(v)+b33)

void Pespective(int pts_in[][2],int int_num ,  float pts_out[][2])
//带入提取出来的两条边线，得到透视变换后的两条边线
{
    float x, y, w;
    for (int16_t i = 0; i < int_num; i++)
    {
        x = getx((pts_in[i][0]), (pts_in[i][1]));
        y = gety((pts_in[i][0]), (pts_in[i][1]));
        w = getw((pts_in[i][0]), (pts_in[i][1]));
        pts_out[i][0] = x / w;
        pts_out[i][1] = y / w;
    }
}


// 使用示例：


/**
* @brief 最小二乘法
* @param uint8 begin                输入起点
* @param uint8 end                  输入终点
* @param uint8 *border              输入需要计算斜率的边界首地址
*  @see CTest       Slope_Calculate(start, end, border);//斜率
* @return 返回说明
*     -<em>false</em> fail
*     -<em>true</em> succeed
*/
float Slope_Calculate(uint8 begin, uint8 end, uint8 *border)
{
    float xsum = 0, ysum = 0, xysum = 0, x2sum = 0;
    int16 i = 0;
    float result = 0;
    static float resultlast;

    for (i = begin; i < end; i++)
    {
        xsum += i;
        ysum += border[i];
        xysum += i * (border[i]);
        x2sum += i * i;

    }
    if ((end - begin)*x2sum - xsum * xsum) //判断除数是否为零
    {
        result = ((end - begin)*xysum - xsum * ysum) / ((end - begin)*x2sum - xsum * xsum);
        resultlast = result;
    }
    else
    {
        result = resultlast;
    }
    return result;
}
/**
* @brief 计算斜率截距
* @param uint8 start                输入起点
* @param uint8 end                  输入终点
* @param uint8 *border              输入需要计算斜率的边界
* @param float *slope_rate          输入斜率地址
* @param float *intercept           输入截距地址
*  @see CTest       calculate_s_i(start, end, r_border, &slope_l_rate, &intercept_l);
* @return 返回说明
*     -<em>false</em> fail
*     -<em>true</em> succeed
*/
void calculate_s_i(uint8 start, uint8 end, uint8 *border, float *slope_rate, float *intercept)
{
    uint16 i, num = 0;
    uint16 xsum = 0, ysum = 0;
    float y_average, x_average;

    num = 0;
    xsum = 0;
    ysum = 0;
    y_average = 0;
    x_average = 0;
    for (i = start; i < end; i++)
    {
        xsum += i;
        ysum += border[i];
        num++;
    }

    //计算各个平均数
    if (num)
    {
        x_average = (float)(xsum / num);
        y_average = (float)(ysum / num);

    }

    /*计算斜率*/
    *slope_rate = Slope_Calculate(start, end, border);//斜率
    *intercept = y_average - (*slope_rate)*x_average;//截距
}
uint8 l_roundabout_flag = 0;
uint8 r_roundabout_flag = 0;
uint8 lh_num = 0;
uint8 rh_num = 0;
/**
* @brief 十字补线函数
* @param uint8(*image)[image_w]     输入二值图像
* @param uint8 *l_border            输入左边界首地址
* @param uint8 *r_border            输入右边界首地址
* @param uint16 total_num_l         输入左边循环总次数
* @param uint16 total_num_r         输入右边循环总次数
* @param uint16 *dir_l              输入左边生长方向首地址
* @param uint16 *dir_r              输入右边生长方向首地址
* @param uint16(*points_l)[2]       输入左边轮廓首地址
* @param uint16(*points_r)[2]       输入右边轮廓首地址
*  @see CTest       cross_fill(image,l_border, r_border, data_statics_l, data_statics_r, dir_l, dir_r, points_l, points_r);
* @return 返回说明
*     -<em>false</em> fail
*     -<em>true</em> succeed
 */
void cross_fill(uint8(*image)[image_w], uint8 *l_border, uint8 *r_border, uint16 total_num_l, uint16 total_num_r,
                                         uint16 *dir_l, uint16 *dir_r, uint16(*points_l)[2], uint16(*points_r)[2])
{
    unsigned short i;
    uint8 break_num_l = 0;
    uint8 break_num_r = 0;
    uint8 start, end;
    float slope_l_rate = 0, intercept_l = 0;
    //出十字
    for (i = 1; i < total_num_l; i++)
    {
        if (dir_l[i - 1] == 4 && dir_l[i] == 4 && dir_l[i + 3] == 6 && dir_l[i + 5] == 6 && dir_l[i + 7] == 6)
        {
            break_num_l = points_l[i][1];//传递y坐标
//            printf("brea_knum-L:%d\n", break_num_l);
//            printf("I:%d\n", i);
//            printf("十字标志位：1\n");
            break;
        }
    }
    for (i = 1; i < total_num_r; i++)
    {
        if (dir_r[i - 1] == 4 && dir_r[i] == 4 && dir_r[i + 3] == 6 && dir_r[i + 5] == 6 && dir_r[i + 7] == 6)
        {
            break_num_r = points_r[i][1];//传递y坐标
            break;
        }
    }
    if (break_num_l&&break_num_r&&image[image_h - 1][4] && image[image_h - 1][image_w - 4] && l_roundabout_flag == 0 && r_roundabout_flag == 0)//两边生长方向都符合条件
    {
        //计算斜率
        start = break_num_l - 15;
        start = limit_a_b(start, 0, image_h);
        end = break_num_l - 5;
        calculate_s_i(start, end, l_border, &slope_l_rate, &intercept_l);
        //printf("slope_l_rate:%d\nintercept_l:%d\n", slope_l_rate, intercept_l);
        for (i = break_num_l - 5; i < image_h - 1; i++)
        {
            l_border[i] = slope_l_rate * (i)+intercept_l;//y = kx+b
            l_border[i] = limit_a_b(l_border[i], border_min, border_max);//限幅
        }

        //计算斜率
        start = break_num_r - 15;//起点
        start = limit_a_b(start, 0, image_h);//限幅
        end = break_num_r - 5;//终点
        calculate_s_i(start, end, r_border, &slope_l_rate, &intercept_l);
        //printf("slope_l_rate:%d\nintercept_l:%d\n", slope_l_rate, intercept_l);
        for (i = break_num_r - 5; i < image_h - 1; i++)
        {
            r_border[i] = slope_l_rate * (i)+intercept_l;
            r_border[i] = limit_a_b(r_border[i], border_min, border_max);
        }
    }

}

uint8 zebra_crossing_flag=0;//斑马线标志位

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     判断斑马线
// 参数说明     void
// 返回参数     无
// 使用示例
// 备注信息     无
//-------------------------------------------------------------------------------------------------------------------
void zebra_crossing_line(void)
{
    uint8 num1 = 0;
    uint8 dark_spotnum = 0;
    uint8 white_dotnum = 0;
    uint8 jump_pointnum = 0;
    num1 = (uint8)My_abs(r_border[image_h/2] - l_border[image_h/2]);
    for(uint8 j = 0;j < num1;j ++ )
    {
        if(bin_image[image_h/2][l_border[image_h/2] + j] == 0)
        {
            dark_spotnum++;
        }
        if(bin_image[image_h/2][l_border[image_h/2] + j] == 255)
        {
            white_dotnum++;
        }
        if(bin_image[image_h/2][l_border[image_h/2] + j] == 255 && bin_image[image_h/2][l_border[image_h/2] + j + 1] == 0)
        {
            jump_pointnum++;
        }
        if(bin_image[image_h/2][l_border[image_h/2] + j] == 0 && bin_image[image_h/2][l_border[image_h/2] + j + 1] == 255)
        {
            jump_pointnum++;
        }
    }
    if(dark_spotnum > 10 && white_dotnum > 20 &&  jump_pointnum > 10)
    {
        zebra_crossing_flag = 1;
    }
    else
    {
        zebra_crossing_flag = 0;
    }
}
uint8 bridge_l_flag = 0;
uint8 bridge_r_flag = 0;

//识别单边桥
void single_sided_bridge(void)
{
    unsigned short i;
    uint8 break_num_l = 0;
    uint8 break_num_r = 0;
    uint8 start, end;
    float slope_l_rate = 0, intercept_l = 0;
    //左单边桥
    for (i = 5; i < data_stastics_l; i++)
    {
        if (dir_l[i - 1] == 4 && dir_l[i] == 4 && dir_l[i + 3] == 2)
        {
            if(l_border[105] > 50 && l_border[110] > 50)
            {
                break_num_l = (uint8)points_l[i][1];//传递y坐标
                bridge_l_flag = 1;
                break;
            }

        }
        else
        {
            bridge_l_flag = 0;
        }
    }
    for (i = 5; i < data_stastics_r; i++)
    {
        if (dir_r[i - 1] == 4 && dir_r[i] == 4 && dir_r[i + 3] == 2)
        {
            if(r_border[105] < 150 && r_border[100] < 150)
            {
                break_num_r = (uint8)points_r[i][1];//传递y坐标
                bridge_r_flag = 1;
                break;
            }
        }
        else
        {
            bridge_r_flag = 0;
        }
    }
    if (bridge_l_flag)//左单边桥补线
    {
        //计算斜率

        start = break_num_l - 20;
        start = (uint8)limit_a_b(start, 0, image_h);
        end = break_num_l - 10;
        calculate_s_i(start, end, l_border, &slope_l_rate, &intercept_l);
        for (i = break_num_l - 5; i < image_h - 1; i++)
        {
            l_border[i] = slope_l_rate * (i)+intercept_l;//y = kx+b
            l_border[i] = (uint8)limit_a_b(l_border[i], border_min, border_max);//限幅
        }
    }
    if(bridge_r_flag)
    {
        //计算斜率
        start = break_num_r - 20;//起点
        start = (uint8)limit_a_b(start, 0, image_h);//限幅
        end = break_num_r - 10;//终点
        calculate_s_i(start, end, r_border, &slope_l_rate, &intercept_l);
        //printf("slope_l_rate:%d\nintercept_l:%d\n", slope_l_rate, intercept_l);
        for (i = break_num_r - 5; i < image_h - 1; i++)
        {
            r_border[i] = slope_l_rate * (i)+intercept_l;
            r_border[i] = (uint8)limit_a_b(r_border[i], border_min, border_max);
        }
    }
}

//左环岛状态机
void l_state_machine(void)
{
    unsigned short i;
    uint8 l_num = 0;
    uint8 r_num = 0;
    uint8 white_date = 0;
    uint8 black_date = 0;
    if(l_roundabout_flag == 0 && r_roundabout_flag == 0)
    {
        for(i = 0; i < 5;i ++)//左下角丢线
        {
            if(l_border[image_h/2 + i] < 4)
            {
                l_num ++;
            }
            if(r_border[image_h/2 + i] >= 184)
            {
                r_num ++;
            }
        }
        if(l_num >= 4 && r_num <= 1)
        {
            for(i = 10;i < data_stastics_l; i++)//找到左下拐点
            {
                if(points_l[i][0] > points_l[i+7][0] && points_l[i][0] > points_l[i-7][0] && points_l[i][1] < points_l[i+7][1] && points_l[i][1] < points_l[i-7][1])
                {
                    if((uint8)points_l[i][1] > 70)
                    {
                        l_roundabout_flag = 1;
                        break;
                    }
                }
            }
        }
    }
    else if(l_roundabout_flag == 1)//进入左环岛状态
    {
        for(i = 0;i < 10;i++)//转弯时识别到特殊左线
        {
            if(r_border[image_h/2 - i] > 184)
            {
                white_date ++;
            }
            if(r_border[image_h/2 - i] <= 184)
            {
                black_date ++;
            }
        }
        if(white_date >= 3 && black_date >= 3)//准备进入第二个口
        {
            l_roundabout_flag = 2;
        }
        for(i = 10;i < data_stastics_l - 10; i++)//左下角连圆环中拐点
        {
            if(points_l[i][0] > points_l[i+4][0] && points_l[i][0] > points_l[i-4][0] && points_l[i][1] > points_l[i+4][1] && points_l[i][1] < points_l[i-4][1])
            {
                if((uint8)points_l[i][1] > 30 && (uint8)points_l[i][1] < 100 && l_border[(uint8)points_l[i][1] - 8] > 5)
                {
                    lh_num = (uint8)points_l[i][1];//传递y坐标
                    connect_line(l_border,119,0,lh_num,l_border[lh_num]);
                    break;
                }
            }
        }
        for(i = 10;i < data_stastics_l - 10; i++)//右下角连圆环上拐点
        {
            if(points_l[i][0] < points_l[i+4][0] && points_l[i][0] > points_l[i-4][0] && points_l[i][1] > points_l[i+4][1] && points_l[i][1] > points_l[i-4][1])
            {
                if((uint8)points_l[i][1] > 20 && l_border[(uint8)points_l[i][1] + 8] < 5)
                {
                    rh_num = (uint8)points_l[i][1];//传递y坐标
                    connect_line(r_border,119,187,rh_num,l_border[rh_num]);
                    break;
                }
            }
        }
    }
    else if(l_roundabout_flag == 2)
    {
        l_num = 0;
        white_date = 0;
        black_date = 0;
        for(i = 0;i < 5;i++)//进入第二个口
        {
            if(r_border[image_h - i - 20] <= 178)
            {
                black_date ++;
            }
        }
        if(black_date >= 4)
        {
            l_roundabout_flag = 3;
        }
        for(i = 10;i < data_stastics_l - 10; i++)
        {
            if(points_l[i][0] < points_l[i+4][0] && points_l[i][0] > points_l[i-4][0] && points_l[i][1] > points_l[i+4][1] && points_l[i][1] > points_l[i-4][1])
            {
                if((uint8)points_l[i][1] > 20 && l_border[(uint8)points_l[i][1] + 5] < 5)
                {
                    rh_num = (uint8)points_l[i][1];//传递y坐标
                    connect_line(r_border,119,187,rh_num,l_border[rh_num]);
                    break;
                }
            }
        }
    }
    else if(l_roundabout_flag == 3)
    {
        l_num = 0;
        r_num = 0;
        white_date = 0;
        black_date = 0;
        //出第一个口
        for(i = 0;i < 5;i++)
        {
            if(bin_image[image_h - i - 25][2] == 255)//左边丢线
            {
                white_date ++;
            }
            if(bin_image[image_h - i - 25][185] == 255)//右边丢线
            {
                black_date ++;
            }
            if(l_border[image_h - i - 80] == 1)//左上方识别不到
            {
                l_num ++;
            }
            if(r_border[image_h - i - 80] <= 185)//右上方识别不到
            {
                r_num ++;
            }
        }
        if(white_date >= 4 && black_date >= 4 && l_num >= 4 && r_num == 0)
        {
            l_roundabout_flag = 4;
        }
    }
    else if(l_roundabout_flag == 4)
    {
        l_num = 0;
        white_date = 0;
        black_date = 0;
        connect_line(r_border,119,187,60,104);
        for(i = 10;i < data_stastics_l - 10; i++)//左下角连圆环上拐点
        {
            if(points_l[i][0] < points_l[i+4][0] && points_l[i][0] > points_l[i-4][0] && points_l[i][1] > points_l[i+4][1] && points_l[i][1] > points_l[i-4][1])
            {
                if((uint8)points_l[i][1] > 30 && l_border[(uint8)points_l[i][1] + 8] < 5)
                {
                    rh_num = (uint8)points_l[i][1];//传递y坐标
                    connect_line(l_border,119,2,rh_num,l_border[rh_num]);
                    break;
                }
            }
        }
        if(points_l[i][1] > 100 && l_border[105] >= 3 && l_border[106] >= 3 && l_border[107] >= 3)//下拐点快要消失
        {
            l_roundabout_flag = 5;
        }
    }
    else if(l_roundabout_flag == 5)
    {
        for(i = 0;i < 5;i++)//两边不丢线
        {
            if(l_border[image_h - i - 1] >= 2)
            {
                white_date ++;
            }
            if(r_border[image_h - i - 1] <= 185)
            {
                black_date ++;
            }
        }
        if(white_date >= 3 && black_date >= 3)
        {
            l_roundabout_flag = 0;
        }
    }
}

//右环岛状态机
void r_state_machine(void)
{
    unsigned short i;
    uint8 l_num = 0;
    uint8 r_num = 0;
    uint8 white_date = 0;
    uint8 black_date = 0;
    if(r_roundabout_flag == 0 && l_roundabout_flag == 0)
    {
        for(i = 0; i < 5;i ++)//右下角丢线
        {
            if(r_border[image_h/2 + i] >= 184)
            {
                r_num ++;
            }
            if(l_border[image_h/2 + i] < 4)
            {
                l_num ++;
            }
        }
        if(r_num >= 4 && l_num <= 1)
        {
            for(i = 10;i < data_stastics_r; i++)//找到左下拐点
            {
                if(points_r[i][0] < points_r[i+7][0] && points_r[i][0] < points_r[i-7][0] && points_r[i][1] < points_r[i+7][1] && points_r[i][1] < points_r[i-7][1])
                {
                    if((uint8)points_r[i][1] > 70 && r_border[(uint8)points_r[i][1] - 8] > 184)
                    {
                        r_roundabout_flag = 1;
                        break;
                    }
                }
            }
        }
    }
    else if(r_roundabout_flag == 1)//进入右环岛状态,不进第一个口
    {
        for(i = 0; i < 5;i ++)//右下角不丢线
        {
            if(r_border[image_h/2 + i] <= 140)
            {
                r_num ++;
            }
            if(l_border[image_h/2 + i] < 4)
            {
                l_num ++;
            }
        }
        if(r_num >= 4 && l_num <= 1)//准备进入第二个口
        {
            r_roundabout_flag = 2;
        }
        for(i = 10;i < data_stastics_r - 10; i++)//右下角连圆环中拐点
        {
            if(points_r[i][0] < points_r[i+4][0] && points_r[i][0] < points_r[i-4][0] && points_r[i][1] > points_r[i+4][1] && points_r[i][1] < points_r[i-4][1])
            {
                if((uint8)points_r[i][1] > 20 && r_border[(uint8)points_r[i][1] - 5] < 184)
                {
                    rh_num = (uint8)points_r[i][1];//传递y坐标
                    connect_line(r_border,119,186,rh_num,r_border[rh_num]);
                    break;
                }
            }
        }
    }
    else if(r_roundabout_flag == 2)//进入右环岛状态,不进第一个口，进第二个口
    {
        for(i = 0;i < 10;i++)//转弯时识别到特殊右线
        {
            if(l_border[image_h/2 - i] > 3)
            {
                white_date ++;
            }
            if(l_border[image_h/2 - i] <= 3)
            {
                black_date ++;
            }
        }
        if(white_date >= 3 && black_date >= 3)//准备进入第二个口
        {
            r_roundabout_flag = 3;
        }
        for(i = 10;i < data_stastics_r - 10; i++)//左下角连圆环上拐点
        {
            if(points_r[i][0] > points_r[i+4][0] && points_r[i][0] < points_r[i-4][0] && points_r[i][1] > points_r[i+4][1] && points_r[i][1] > points_r[i-4][1])
            {
                if((uint8)points_r[i][1] > 40 && r_border[(uint8)points_r[i][1] + 8] > 184)
                {
                    lh_num = (uint8)points_r[i][1];//传递y坐标
                    connect_line(l_border,119,30,lh_num,r_border[lh_num] + 30);
                    break;
                }
            }
        }
        for(i = 10;i < data_stastics_r - 10; i++)//右下角连圆环中拐点
        {
            if(points_r[i][0] < points_r[i+4][0] && points_r[i][0] < points_r[i-4][0] && points_r[i][1] > points_r[i+4][1] && points_r[i][1] < points_r[i-4][1])
            {
                if((uint8)points_r[i][1] > 50 && r_border[(uint8)points_r[i][1] - 5] < 184)
                {
                    rh_num = (uint8)points_r[i][1];//传递y坐标
                    connect_line(r_border,119,186,rh_num,r_border[rh_num]);
                    break;
                }
            }
        }
    }
    else if(r_roundabout_flag == 3)//进入第二个口
    {
        l_num = 0;
        lh_num = 0;
        white_date = 0;
        black_date = 0;
        for(i = 0;i < 5;i++)//进入第二个口
        {
            if(l_border[image_h - i - 5] >= 10)
            {
                black_date ++;
            }
        }
        if(black_date >= 4)
        {
            r_roundabout_flag = 4;
        }
        for(i = 10;i < data_stastics_r - 10; i++)//左下角连圆环上拐点
        {
            if(points_r[i][0] > points_r[i+4][0] && points_r[i][0] < points_r[i-4][0] && points_r[i][1] > points_r[i+4][1] && points_r[i][1] > points_r[i-4][1])
            {
                if((uint8)points_r[i][1] > 40 && r_border[(uint8)points_r[i][1] + 8] > 184)
                {
                    lh_num = (uint8)points_r[i][1];//传递y坐标
                    connect_line(l_border,119,30,lh_num,r_border[lh_num] + 30);
                    for(uint8 j = lh_num;j > 0;j --)
                    {
                        l_border[j] = 94;
                        r_border[j] = 94;
                    }
                    break;
                }
            }
        }
    }
    else if(r_roundabout_flag == 4)//寻找出口拐角
    {
        l_num = 0;
        r_num = 0;
        white_date = 0;
        black_date = 0;
        //出第一个口
        for(i = 10;i < data_stastics_r - 10; i++)
        {
            if(points_l[i][0] > points_l[i+4][0] && points_l[i][0] < points_l[i-4][0] && points_l[i][1] < points_l[i+4][1] && points_l[i][1] < points_l[i-4][1])
            {
                if((uint8)points_l[i][1] > 40 && l_border[(uint8)points_l[i][1] - 8] < 3)
                {
                    lh_num = (uint8)points_r[i][1];//传递y坐标
                    r_roundabout_flag = 5;
                    break;
                }
            }
        }
    }
    else if(r_roundabout_flag == 5)
    {
        l_num = 0;
        white_date = 0;
        black_date = 0;
        for(i = 10;i < data_stastics_r - 10; i++)//右下角连圆环上拐点
        {
            if(points_r[i][0] > points_r[i+4][0] && points_r[i][0] < points_r[i-4][0] && points_r[i][1] > points_r[i+4][1] && points_r[i][1] > points_r[i-4][1])
            {
                if((uint8)points_r[i][1] > 40 && r_border[(uint8)points_r[i][1] + 5] > 184)
                {
                    lh_num = (uint8)points_r[i][1];//传递y坐标
                    connect_line(r_border,119,186,lh_num,r_border[lh_num]);
                    break;
                }
            }
        }
        if(points_r[i][1] > 100 && r_border[95] <= 185 && r_border[96] <= 185 && r_border[97] <= 185)//下拐点快要消失
        {
            r_roundabout_flag = 6;
        }
        connect_line(l_border,119,30,70,70);
    }
    else if(r_roundabout_flag == 6)
    {
        for(i = 0;i < 5;i++)//两边不丢线
        {
            if(l_border[image_h - i - 1] >= 2)
            {
                white_date ++;
            }
            if(r_border[image_h - i - 1] <= 185)
            {
                black_date ++;
            }
        }
        if(white_date >= 3 && black_date >= 3)
        {
            r_roundabout_flag = 0;
        }
    }
}
uint8 roadblock_flag = 0;
//路障
void roadblock(void)
{
    uint8 i = 0;
    uint8 date = 0;
    for(i = 0;i < 10;i++)
    {
        if(bin_image[115][89 + i] == 0 && bin_image[114][89 + i] == 0 && bin_image[113][89 + i] == 0)
        {
            date ++;
        }
    }
    if(date == 10)
    {
        roadblock_flag = 1;
    }
}

//图像权重数组
//权重越靠上，权重越小
uint8 mid_weight_list[120]=
{
        1,1,1,1,1,1,1,1,1,1,
         1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,
        6,6,6,6,6,6,6,6,6,6,
        7,8,9,10,11,12,13,14,15,16,
        17,18,19,20,20,20,20,19,18,17,
        16,15,14,13,12,11,10,9,8,7,
        6,6,6,6,6,6,6,6,6,6,

};
uint8 final_mid_line = MID_W;//最终输出中线值
uint8 last_mid_line = MID_W;//上次中线值
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     求中线加权值
// 参数说明     void
// 返回参数     中线加权值
// 使用示例
// 备注信息     无
//-------------------------------------------------------------------------------------------------------------------
uint8 find_mid_line_weight(void)
{
    uint8 mid_line_value = MID_W;   //最终中线输出值
    uint8 mid_line = MID_W;         //最终中线输出值
    uint32 weight_midline_sum = 0;  //最终中线输出值
    uint32 weight_sum = 0;          //最终中线输出值
    for(uint8 i=search_start_line-1;i>search_end_line;i--)
    {
        weight_midline_sum += center_line[i] * mid_weight_list[i];
        weight_sum += mid_weight_list[i];
    }
    mid_line = (uint8)(weight_midline_sum/weight_sum);
   mid_line_value = (float)last_mid_line*0.2 + (float)mid_line*0.8;//互补滤波
    last_mid_line = mid_line_value;
    return mid_line_value;
}
//
///*
//函数名称：void image_process(void)
//功能说明：最终处理函数
//参数说明：无
//函数返回：无
//备    注：
//example： image_process();
// */
void image_process(void)
{
    int mid_stright=0;
    uint16 i;
    uint16 mid_line=0;
    uint8 hightest = 0;//定义一个最高行，tip：这里的最高指的是y值的最小
/*这是离线调试用的*/
    Get_image(base_image);//图像压缩
    turn_to_bin();//图像二值化

///*提取赛道边界*/
    image_filter(bin_image);//滤波,消除噪点
    image_draw_rectan(bin_image);//给图像画一个黑框
////清零
        data_stastics_l = 0;
        data_stastics_r = 0;
if (get_start_point(image_h - 2))//找到起点了，再执行八领域，没找到就一直找
{

    search_l_r((uint16)USE_num, bin_image, &data_stastics_l, &data_stastics_r, start_point_l[0], start_point_l[1], start_point_r[0], start_point_r[1], &hightest);

    get_left(data_stastics_l);
    get_right(data_stastics_r);
   cross_fill(bin_image,l_border, r_border, data_stastics_l, data_stastics_r, dir_l, dir_r, points_l, points_r);
    //处理函数放这里，不要放到if外面去了，不要放到if外面去了，不要放到if外面去了，重要的事说三遍
   zebra_crossing_line();//斑马线
   single_sided_bridge();//单边桥
   roadblock();//路障
   l_state_machine();//左环岛
   r_state_machine();//右环岛
}

      ips200_show_gray_image(0, 0, bin_image[0], image_w, image_h, image_w, image_h, 0);
////    //根据最终循环次数画出边界点
   for (i = 0; i < data_stastics_l; i++)
    {
        ips200_draw_point(points_l[i][0]+2, points_l[i][1], RGB565_BLUE);//显示起点
//       points_l_x[i] = points_l[i][0];
//       points_l_y[i] = points_l[i][1];
    }
   for (i = 0; i < data_stastics_r; i++)
   {
       ips200_draw_point(points_r[i][0]-2, points_r[i][1], RGB565_RED);//显示起点
//       points_r_X[i] = points_r[i][0];
//       points_r_y[i] = points_r[i][1];
   }
//
   for (i = hightest; i < image_h-1; i++)
    {
      center_line[i] = (l_border[i] + r_border[i]) >> 1;//求中线
//        //求中线最好最后求，不管是补线还是做状态机，全程最好使用一组边线，中线最后求出，不能干扰最后的输出
//        //当然也有多组边线的找法，但是个人感觉很繁琐，不建议
        ips200_draw_point(center_line[i], i, RGB565_RED);//显示起点 显示中线

        ips200_draw_point(l_border[i], i, RGB565_GREEN);//显示起点 显示左边线
        ips200_draw_point(r_border[i], i, RGB565_GREEN);//显示起点 显示右边线
    }

   mid_stright = find_mid_line_weight();//找中线加权值
   final_mid_line = mid_stright;
//   ips200_show_int(120, 120, mid_stright, 4);
   ips200_show_int(5, 150, l_roundabout_flag, 4);   ips200_show_int(50, 150, lh_num, 4);    ips200_show_int(100, 150, rh_num, 4);
   ips200_show_int(5, 170, r_roundabout_flag, 4);
   ips200_show_int(5, 190, final_mid_line, 4);
}




/*

这里是起点（0.0）***************——>*************x值最大
************************************************************
************************************************************
************************************************************
************************************************************
******************假如这是一副图像*************************
***********************************************************
***********************************************************
***********************************************************
***********************************************************
***********************************************************
***********************************************************
y值最大*******************************************(188.120)

*/



