//
// Created by 张文波 on 2025/9/26.
//

#ifndef MOTOR_H
#define MOTOR_H

#include "tim.h"
#include "stdbool.h"
#include "main.h"
#define PG_Z_Number 4
extern volatile bool flag_PG_motor[PG_Z_Number];

void motor_forward(); //全速或默认速度向上
void motor_back();   //全速或默认速度向下
void motor_no(); //刹车
void motor_forward_low(); //低速向上，用于复位
void PG_read_motor();
void motor_no_no_delay();
#endif //MOTOR_H
