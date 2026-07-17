//
// Created by 张文波 on 2025/9/26.
//

#ifndef PUMP_H
#define PUMP_H

#include "tim.h"
#include "stdbool.h"
#define delay_time_sum 6000
extern volatile bool flag_PG_pump[2];
extern volatile uint32_t pump_delay_time_100[6];
extern volatile uint32_t pump_delay_time_50[6];
void pump_set_out();//吸出
void pump_set_in();//吸入
void pump_set_no();//不吸也不出
void PG_read_pump(); //读取光电门的数据
void pump_set_no_no_delay();
void pump_test();
void pump_set_out_different_times();
void pump_set_out_different_times_50ml();
void pump_set_out_different_times_100ml();
void pump_delay(int time);
HAL_StatusTypeDef WriteToFlash(uint32_t *data1,uint32_t *data2);
HAL_StatusTypeDef ReadFromFlash(uint32_t *data1, uint32_t *data2);
#endif //PUMP_H
