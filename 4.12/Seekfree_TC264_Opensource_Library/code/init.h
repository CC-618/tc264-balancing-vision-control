/*
 * init.h
 *
 *  Created on: 2025年4月4日
 *      Author: 王开阳
 */

#ifndef CODE_INIT_H_
#define CODE_INIT_H_


#include "zf_common_headfile.h"
#define PWM_CH1                 (ATOM1_CH0_P21_2)
#define PWM_CH2                 (ATOM0_CH3_P21_5)
#define PWM_CH3                 (ATOM0_CH1_P21_3)
#define PWM_CH4                 (ATOM0_CH2_P21_4)
void all_init(void);
void ANO_DT_send_int16(short data1, short data2, short data3, short data4, short data5, short data6, short data7, short data8 );
void ANO_DT_send_int16byte16(short data1, short data2, short data3, short data4, short data5, short data6, short data7, short data8 );
void ANO_DT_send_int16byte18(short data1, short data2, short data3, short data4, short data5, short data6, short data7, short data8, short data9 );



#endif /* CODE_INIT_H_ */
