//
// Created by 张文波 on 2025/9/26.
//
#include "motor.h"

//假定有东西遮着为true
volatile bool flag_PG_motor[PG_Z_Number]=
        {false,false,false,false};

typedef struct
{
    TIM_HandleTypeDef *htim; // 定时器句柄
    uint32_t channel;        // 通道号（如TIM_CHANNEL_1）
} PWM_Config;

static const PWM_Config motor[2] = {
        {&htim1, TIM_CHANNEL_2},
        {&htim1, TIM_CHANNEL_1},
};

typedef struct
{
    GPIO_TypeDef *GPIO_PORT; // GPIO_Port
    uint16_t GPIO_PIN;        // GPIO_Pin
} GPIO_Config;

// 假定顺序是从下到上
static const GPIO_Config PG_GPIO[4] = {
        {PG1_GPIO_Port, PG1_Pin},
        {PG2_GPIO_Port, PG2_Pin},
        {PG3_GPIO_Port, PG3_Pin},
        {PG4_GPIO_Port, PG4_Pin},
};

void motor_forward()
{
    uint32_t arr = __HAL_TIM_GET_AUTORELOAD(motor[0].htim);
    __HAL_TIM_SET_COMPARE(motor[1].htim, motor[1].channel, arr);
    __HAL_TIM_SET_COMPARE(motor[0].htim, motor[0].channel, 0);
}
void motor_back()
{
    uint32_t arr = __HAL_TIM_GET_AUTORELOAD(motor[0].htim);
    __HAL_TIM_SET_COMPARE(motor[0].htim, motor[0].channel, arr);
    __HAL_TIM_SET_COMPARE(motor[1].htim, motor[1].channel, 0);
}
void motor_no()
{
    uint32_t arr = __HAL_TIM_GET_AUTORELOAD(motor[0].htim);
    __HAL_TIM_SET_COMPARE(motor[1].htim, motor[1].channel, arr);
    __HAL_TIM_SET_COMPARE(motor[0].htim, motor[0].channel, arr);
    HAL_Delay(2000);
}

void motor_no_no_delay()
{
    uint32_t arr = __HAL_TIM_GET_AUTORELOAD(motor[0].htim);
    __HAL_TIM_SET_COMPARE(motor[1].htim, motor[1].channel, arr);
    __HAL_TIM_SET_COMPARE(motor[0].htim, motor[0].channel, arr);

}

void motor_forward_low()
{
    uint32_t arr = __HAL_TIM_GET_AUTORELOAD(motor[0].htim);
    __HAL_TIM_SET_COMPARE(motor[1].htim, motor[1].channel, arr);
    __HAL_TIM_SET_COMPARE(motor[0].htim, motor[0].channel, 0);
}

void PG_read_motor()
{
    for(int i=0;i<PG_Z_Number;i++)
    {
        if(HAL_GPIO_ReadPin(PG_GPIO[i].GPIO_PORT,PG_GPIO[i].GPIO_PIN))
            flag_PG_motor[i]=true;
        else
            flag_PG_motor[i]=false;
    }
}