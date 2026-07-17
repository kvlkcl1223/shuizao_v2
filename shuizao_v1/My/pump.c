//
// Created by 张文波 on 2025/9/26.
//
#include "pump.h"
#include "command.h"
#define pump_number 6
#define FLASH_START_ADDR   0x0800F000  // STM32 的 Flash 最后一页的起始地址







volatile uint32_t pump_delay_time_50[6]={4526,1508,4937,2811,4835,4526
};

volatile uint32_t pump_delay_time_100[6]={4595,400,4869,1988,4972,2983
};

volatile bool all_delay_done=false;
volatile uint32_t time_count=0;
volatile bool flag_pump_delay_50=false;
volatile bool flag_pump_delay_100=false;

//初始为false。默认为True时说明该处被遮光了
volatile bool flag_PG_pump[2]={false,false};

// 定义定时器与其通道的映射结构
typedef struct
{
    TIM_HandleTypeDef *htim; // 定时器句柄
    uint32_t channel;        // 通道号（如TIM_CHANNEL_1）
} PWM_Config;


// 泵编号到定时器的映射表
static const PWM_Config pump_A[pump_number] = {

        {&htim1, TIM_CHANNEL_3},
        {&htim4, TIM_CHANNEL_3},
        {&htim2, TIM_CHANNEL_1},
        {&htim2, TIM_CHANNEL_3},
        {&htim3, TIM_CHANNEL_1},
        {&htim3, TIM_CHANNEL_3}


};

static const PWM_Config pump_B[pump_number] = {

        {&htim1, TIM_CHANNEL_4},
        {&htim4, TIM_CHANNEL_4},
        {&htim2, TIM_CHANNEL_2},
        {&htim2, TIM_CHANNEL_4},
        {&htim3, TIM_CHANNEL_2},
        {&htim3, TIM_CHANNEL_4}
};


typedef struct
{
    GPIO_TypeDef *GPIO_PORT; // GPIO_Port
    uint16_t GPIO_PIN;        // GPIO_Pin
} GPIO_Config;

// 假定第一个是后面的，第二个是前面的
static const GPIO_Config PG_GPIO_pump[2] = {
        {PG6_GPIO_Port, PG6_Pin},
        {PG5_GPIO_Port, PG5_Pin},
};

void pump_set_out()
{
    for(int i =0;i<pump_number;i++)
    {
        uint32_t arr = __HAL_TIM_GET_AUTORELOAD(pump_A[i].htim);
        __HAL_TIM_SET_COMPARE(pump_A[i].htim, pump_A[i].channel, arr);
        __HAL_TIM_SET_COMPARE(pump_B[i].htim, pump_B[i].channel, 0);
    }
}
//void pump_set_out_different_times()
//{
//
//    uint32_t  arr = __HAL_TIM_GET_AUTORELOAD(pump_B[1].htim);
//    __HAL_TIM_SET_COMPARE(pump_A[1].htim, pump_A[1].channel, arr);
//    __HAL_TIM_SET_COMPARE(pump_B[1].htim, pump_B[1].channel, 0);
//    pump_delay(400);
//
//
//    pump_delay(100);
//
//    arr = __HAL_TIM_GET_AUTORELOAD(pump_B[3].htim);
//    __HAL_TIM_SET_COMPARE(pump_A[3].htim, pump_A[3].channel, arr);
//    __HAL_TIM_SET_COMPARE(pump_B[3].htim, pump_B[3].channel, 0);
//    pump_delay(400);
//    arr = __HAL_TIM_GET_AUTORELOAD(pump_B[0].htim);
//    __HAL_TIM_SET_COMPARE(pump_A[0].htim, pump_A[0].channel, arr);
//    __HAL_TIM_SET_COMPARE(pump_B[0].htim, pump_B[0].channel, 0);
//    pump_delay(0);
//
//
//    arr = __HAL_TIM_GET_AUTORELOAD(pump_B[5].htim);
//    __HAL_TIM_SET_COMPARE(pump_A[5].htim, pump_A[5].channel, arr);
//    __HAL_TIM_SET_COMPARE(pump_B[5].htim, pump_B[5].channel, 0);
//
//    pump_delay(800);
//    arr = __HAL_TIM_GET_AUTORELOAD(pump_B[4].htim);
//    __HAL_TIM_SET_COMPARE(pump_A[4].htim, pump_A[4].channel, arr);
//    __HAL_TIM_SET_COMPARE(pump_B[4].htim, pump_B[4].channel, 0);
//
//    arr = __HAL_TIM_GET_AUTORELOAD(pump_B[2].htim);
//    __HAL_TIM_SET_COMPARE(pump_A[2].htim, pump_A[2].channel, arr);
//    __HAL_TIM_SET_COMPARE(pump_B[2].htim, pump_B[2].channel, 0);
//
//}

void pump_set_out_different_times_50ml()
{


    flag_pump_delay_50=true;
    all_delay_done=false;
    time_count=0;
    HAL_TIM_Base_Start_IT(&htim4);
    while((flag_100||flag_200||flag_50)&&(!all_delay_done))
    {
        HAL_Delay(10);
    }

//    uint32_t arr = __HAL_TIM_GET_AUTORELOAD(pump_B[1].htim);
//    __HAL_TIM_SET_COMPARE(pump_A[1].htim, pump_A[1].channel, arr);
//    __HAL_TIM_SET_COMPARE(pump_B[1].htim, pump_B[1].channel, 0);
//    pump_delay(400);
//    arr = __HAL_TIM_GET_AUTORELOAD(pump_B[0].htim);
//    __HAL_TIM_SET_COMPARE(pump_A[0].htim, pump_A[0].channel, arr);
//    __HAL_TIM_SET_COMPARE(pump_B[0].htim, pump_B[0].channel, 0);
//    pump_delay(200);
//    arr = __HAL_TIM_GET_AUTORELOAD(pump_B[3].htim);
//    __HAL_TIM_SET_COMPARE(pump_A[3].htim, pump_A[3].channel, arr);
//    __HAL_TIM_SET_COMPARE(pump_B[3].htim, pump_B[3].channel, 0);
//
//    pump_delay(900);
//    arr = __HAL_TIM_GET_AUTORELOAD(pump_B[2].htim);
//    __HAL_TIM_SET_COMPARE(pump_A[2].htim, pump_A[2].channel, arr);
//    __HAL_TIM_SET_COMPARE(pump_B[2].htim, pump_B[2].channel, 0);
//
//
//    pump_delay(600);
//    arr = __HAL_TIM_GET_AUTORELOAD(pump_B[5].htim);
//    __HAL_TIM_SET_COMPARE(pump_A[5].htim, pump_A[5].channel, arr);
//    __HAL_TIM_SET_COMPARE(pump_B[5].htim, pump_B[5].channel, 0);
//
//
//    arr = __HAL_TIM_GET_AUTORELOAD(pump_B[4].htim);
//    __HAL_TIM_SET_COMPARE(pump_A[4].htim, pump_A[4].channel, arr);
//    __HAL_TIM_SET_COMPARE(pump_B[4].htim, pump_B[4].channel, 0);
//
//    pump_delay(400);

}



void pump_set_out_different_times_100ml()
{


    flag_pump_delay_100=true;
    all_delay_done=false;
    time_count=0;
    HAL_TIM_Base_Start_IT(&htim4);
    while((flag_100||flag_200||flag_50)&&(!all_delay_done))
    {
        HAL_Delay(10);
    }

//    uint32_t arr = __HAL_TIM_GET_AUTORELOAD(pump_B[1].htim);
//    __HAL_TIM_SET_COMPARE(pump_A[1].htim, pump_A[1].channel, arr);
//    __HAL_TIM_SET_COMPARE(pump_B[1].htim, pump_B[1].channel, 0);
//    pump_delay(300);
//
//    arr = __HAL_TIM_GET_AUTORELOAD(pump_B[0].htim);
//    __HAL_TIM_SET_COMPARE(pump_A[0].htim, pump_A[0].channel, arr);
//    __HAL_TIM_SET_COMPARE(pump_B[0].htim, pump_B[0].channel, 0);
//
//
//    pump_delay(500);
//    arr = __HAL_TIM_GET_AUTORELOAD(pump_B[3].htim);
//    __HAL_TIM_SET_COMPARE(pump_A[3].htim, pump_A[3].channel, arr);
//    __HAL_TIM_SET_COMPARE(pump_B[3].htim, pump_B[3].channel, 0);
//
//
//    pump_delay(1400);
//
//    arr = __HAL_TIM_GET_AUTORELOAD(pump_B[2].htim);
//    __HAL_TIM_SET_COMPARE(pump_A[2].htim, pump_A[2].channel, arr);
//    __HAL_TIM_SET_COMPARE(pump_B[2].htim, pump_B[2].channel, 0);
//
//
//    pump_delay(300);
//    arr = __HAL_TIM_GET_AUTORELOAD(pump_B[4].htim);
//    __HAL_TIM_SET_COMPARE(pump_A[4].htim, pump_A[4].channel, arr);
//    __HAL_TIM_SET_COMPARE(pump_B[4].htim, pump_B[4].channel, 0);
//    pump_delay(700);
//
//
//
//
//
//
//
//
//    pump_delay(200);
//
//    pump_delay(200);
//    arr = __HAL_TIM_GET_AUTORELOAD(pump_B[5].htim);
//    __HAL_TIM_SET_COMPARE(pump_A[5].htim, pump_A[5].channel, arr);
//    __HAL_TIM_SET_COMPARE(pump_B[5].htim, pump_B[5].channel, 0);
}
void pump_set_in()
{
    for(int i =0;i<pump_number;i++)
    {
        uint32_t arr = __HAL_TIM_GET_AUTORELOAD(pump_B[i].htim);
        __HAL_TIM_SET_COMPARE(pump_B[i].htim, pump_B[i].channel, arr);
        __HAL_TIM_SET_COMPARE(pump_A[i].htim, pump_A[i].channel, 0);
    }
}


void pump_set_no()
{
    for(int i =0;i<pump_number;i++)
    {
        uint32_t arr = __HAL_TIM_GET_AUTORELOAD(pump_B[i].htim);
        __HAL_TIM_SET_COMPARE(pump_B[i].htim, pump_B[i].channel, arr);
        __HAL_TIM_SET_COMPARE(pump_A[i].htim, pump_A[i].channel, arr);
    }
    HAL_Delay(1000);
}


void pump_set_no_no_delay()
{
    for(int i =0;i<pump_number;i++)
    {
        uint32_t arr = __HAL_TIM_GET_AUTORELOAD(pump_B[i].htim);
        __HAL_TIM_SET_COMPARE(pump_B[i].htim, pump_B[i].channel, arr);
        __HAL_TIM_SET_COMPARE(pump_A[i].htim, pump_A[i].channel, arr);
    }

}

void PG_read_pump()
{
    for(int i=0;i<2;i++)
    {
        if(HAL_GPIO_ReadPin(PG_GPIO_pump[i].GPIO_PORT,PG_GPIO_pump[i].GPIO_PIN))
            flag_PG_pump[i]=true;
        else
            flag_PG_pump[i]=false;
    }
}

void pump_test()
{
    for(int i =0;i<pump_number;i++)
    {
        uint32_t arr = __HAL_TIM_GET_AUTORELOAD(pump_A[i].htim);
        __HAL_TIM_SET_COMPARE(pump_A[i].htim, pump_A[i].channel, arr);
        __HAL_TIM_SET_COMPARE(pump_B[i].htim, pump_B[i].channel, arr/2);
        pump_delay(2000);
    }
    pump_delay(2000);
    pump_set_no_no_delay();
}

void pump_delay(int time)
{
    int count=0;
    while ((flag_100||flag_200||flag_50)&&(count<time))
    {
        HAL_Delay(10);
        count+=10;
    }
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    // 判断是定时器3发生的中断
    if(htim->Instance == TIM4)
    {
        if (flag_pump_delay_50)
        {

            for(int i=0;i<6;i++)
            {
                if(time_count==(pump_delay_time_50[i]*20))
                {
                    uint32_t arr = __HAL_TIM_GET_AUTORELOAD(pump_B[i].htim);
                    __HAL_TIM_SET_COMPARE(pump_B[i].htim, pump_B[i].channel, 0);
                    __HAL_TIM_SET_COMPARE(pump_A[i].htim, pump_A[i].channel, arr);
//                    pump_delay_done_50[i]=true;
//                    // 如果所有泵的延时都已完成，停止计时
//                    for(int j =0;j<6;j++)
//                    {
//                        if(pump_delay_done_50[j]!=true)
//                        {
//                            all_delay_done = false;
//                            break;
//                        }
//                    }
                }
            }
            time_count++;
            if(time_count>(delay_time_sum*20+100))
            {
                flag_pump_delay_50=false;
                all_delay_done=true;
                time_count=0;
                HAL_TIM_Base_Stop_IT(&htim4);
            }

        } else if(flag_pump_delay_100){

            for(int i=0;i<6;i++) {
                if ( time_count == (pump_delay_time_100[i] * 20)) {
                    uint32_t arr = __HAL_TIM_GET_AUTORELOAD(pump_B[i].htim);
                    __HAL_TIM_SET_COMPARE(pump_B[i].htim, pump_B[i].channel, 0);
                    __HAL_TIM_SET_COMPARE(pump_A[i].htim, pump_A[i].channel, arr);

                }
            }
            time_count++;
            if(time_count>(delay_time_sum*20+100))
            {
                flag_pump_delay_100=false;
                all_delay_done=true;
                time_count=0;
                HAL_TIM_Base_Stop_IT(&htim4);
            }
        }
    }
}


// 写入 Flash 的函数
HAL_StatusTypeDef WriteToFlash(uint32_t *data1,uint32_t *data2)
{
    // 解锁 Flash 写入操作
    HAL_FLASH_Unlock();

    // 擦除最后一页的 Flash
    FLASH_EraseInitTypeDef eraseInit;
    uint32_t pageError;
    eraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
    eraseInit.PageAddress = FLASH_START_ADDR;
    eraseInit.NbPages = 1;  // 擦除最后一页
    HAL_FLASHEx_Erase(&eraseInit, &pageError);

    // 写入数据到 Flash
    for (uint32_t i = 0; i < 6; i++) {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, FLASH_START_ADDR + (i * 4), data1[i]) != HAL_OK) {
            return HAL_ERROR;  // 写入失败
        }
    }
    for (uint32_t i = 0; i < 6; i++) {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, FLASH_START_ADDR + 24+(i * 4), data2[i]) != HAL_OK) {
            return HAL_ERROR;  // 写入失败
        }
    }

    // 锁定 Flash 写入操作
    HAL_FLASH_Lock();

    return HAL_OK;
}

// 从 Flash 读取数据的函数
HAL_StatusTypeDef ReadFromFlash(uint32_t *data1, uint32_t *data2)
{
    // 从 Flash 中读取数据
    for (uint32_t i = 0; i < 6; i++) {
        data1[i] = *(volatile uint32_t *)(FLASH_START_ADDR + (i * 4));
        data2[i] = *(volatile uint32_t *)(FLASH_START_ADDR +24+ (i * 4));
    }

    return HAL_OK;
}