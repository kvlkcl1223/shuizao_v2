//
// Created by 张文波 on 2025/9/26.
//

#include "command.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "pump.h"
#include "motor.h"

#define parameter_save true

#define MAX_VALUES 6
volatile bool flag_start=false;
volatile bool flag_100=false;
volatile bool flag_200=false;
volatile bool flag_50=false;
volatile bool flag_shouqu=false;
volatile bool flag_xiqu=false;


volatile bool flag_mf=false;//电机上升
volatile bool flag_ms=false;//电机停止
volatile bool flag_mb=false;//电机下降

volatile bool flag_po=false;//泵out
volatile bool flag_ps=false;//泵停止
volatile bool flag_pi=false;//泵in


volatile bool flag_S100=false;//手动时100
volatile bool flag_S200=false;//手动时200
volatile bool flag_S50=false;//手动时300

volatile bool flag_reset=false;
volatile bool flag_show=false;

uint8_t rx_buffer[100] = {0}; // 接收缓冲区

void UART_Command_Init()
{
//    cmd_huart = huart;
//    HAL_UART_Receive_IT(cmd_huart, &rx_buffer[0], 1); // 启动第一次接收

    __HAL_UART_ENABLE_IT(COMMAND_UART, UART_IT_IDLE);                                                      // 启用空闲中断
    HAL_UART_Receive_DMA(COMMAND_UART, (uint8_t *)rx_buffer, sizeof(rx_buffer)); // 启动DMA接收
}

void UART_CALLBACK()
{
    if (RESET != __HAL_UART_GET_FLAG(COMMAND_UART, UART_FLAG_IDLE)) // 判断是否是空闲中断
    {
        HAL_UART_DMAStop(COMMAND_UART);
        __HAL_UART_CLEAR_IDLEFLAG(COMMAND_UART);                // 清除空闲中断标志（否则会一直不断进入中断）
        UART_Command_Parse(rx_buffer);
        memset(rx_buffer, 0, sizeof(rx_buffer));                                          // 清空接收缓冲区
        HAL_UART_Receive_DMA(COMMAND_UART, (uint8_t *)rx_buffer,  sizeof(rx_buffer)); // 重启开始DMA传输
    }
}

void UART_Command_Parse(uint8_t *cmd_buffer) {
    if (strncmp((char *) cmd_buffer, (char *) "Z100", 4) == 0) {
        PG_read_pump();
        if(flag_100||flag_50||flag_200)
        {
            uint8_t message[] = {0xD0,0xE8,0xD2,0xAA,0xCF,0xC8,0xB5,0xC8,0xB4,0xFD,0xD7,0xD4,0xB6,0xAF,0xB2,0xD9,0xD7,0xF7,0xBD,0xE1,0xCA,0xF8
            };
            sendMessage(message, 22);
        }

        // 说明此时Y轴并没有到达合适位置
        else if (!flag_PG_pump[1]) {page_warn();}
        else {
            // 中文：开始执行
            uint8_t message[] = {0xBF, 0xAA, 0xCA, 0xBC, 0xD6, 0xB4, 0xD0, 0xD0
            };
            sendMessage(message, 8);

            flag_100 = true;
        }
    }
//    else if(strncmp((char *)cmd_buffer, (char *)"Z200", 4)==0){
//        uint8_t message[]={0xBF,0xAA,0xCA,0xBC,0xD6,0xB4,0xD0,0xD0
//        };
//        sendMessage(message,8);
//        flag_200=true;
//    }
    else if (strncmp((char *) cmd_buffer, (char *) "Z50", 3) == 0) {
        PG_read_pump();
        if(flag_100||flag_50||flag_200)
        {
            uint8_t message[] = {0xD0,0xE8,0xD2,0xAA,0xCF,0xC8,0xB5,0xC8,0xB4,0xFD,0xD7,0xD4,0xB6,0xAF,0xB2,0xD9,0xD7,0xF7,0xBD,0xE1,0xCA,0xF8
            };
            sendMessage(message, 22);
        }
        else if (!flag_PG_pump[1]) {page_warn();}
        else
        {
            uint8_t message[] = {0xBF, 0xAA, 0xCA, 0xBC, 0xD6, 0xB4, 0xD0, 0xD0
            };
            sendMessage(message, 8);
            flag_50 = true;
        }
    } else if (strncmp((char *) cmd_buffer, (char *) "TZ", 2) == 0) {
        if (flag_100 || flag_200 || flag_50)
            flag_xiqu = true;  // 表示吸完了的标志量
        else {
            // 中文：无效信息
            uint8_t message[] = {0xCE, 0xDE, 0xD0, 0xA7, 0xD0, 0xC5, 0xCF, 0xA2
            };
            sendMessage(message, 8);

        }
    } else if (strncmp((char *) cmd_buffer, (char *) "QX", 2) == 0) {
        if (flag_100 || flag_200 || flag_50)
            flag_shouqu = true;  // 表示吸完了的标志量
        else {
            // 中文：无效信息
            uint8_t message[] = {0xCE, 0xDE, 0xD0, 0xA7, 0xD0, 0xC5, 0xCF, 0xA2
            };
            sendMessage(message, 8);

        }
    } else if (strncmp((char *) cmd_buffer, (char *) "ZD", 2) == 0) {
//        flag_100 = false;
//        flag_200 = false;
//        flag_50 = false;
//        flag_shouqu = false;
//        flag_xiqu = false;
//        flag_mf = false;//电机上升
//        flag_ms = false;//电机停止
//        flag_mb = false;//电机下降
//        flag_po = false;//泵out
//        flag_ps = false;//泵停止
//        motor_no_no_delay();
//        pump_set_no_no_delay();
//        flag_pi = false;//泵in
//        flag_S100 = false;//手动时100
//        flag_S200 = false;//手动时200
//        flag_S50 = false;//手动时300
    } else if (strncmp((char *) cmd_buffer, (char *) "SD", 2) == 0) {
//        flag_100 = false;
//        flag_200 = false;
//        flag_50 = false;
//        flag_shouqu = false;
//        flag_xiqu = false;
//        flag_mf = false;//电机上升
//        flag_ms = false;//电机停止
//        flag_mb = false;//电机下降
//        flag_po = false;//泵out
//        flag_ps = false;//泵停止
//        motor_no_no_delay();
//        pump_set_no_no_delay();
//        flag_pi = false;//泵in
//        flag_S100 = false;//手动时100
//        flag_S200 = false;//手动时200
//        flag_S50 = false;//手动时300
        if(flag_show) {
            parameter_show_50();
            //flag_show=false;
        }
        else {
            parameter_show_100();
            //flag_show=true;

        }
    }
        // 急停
    else if (strncmp((char *) cmd_buffer, (char *) "ZJT", 3) == 0 ||
             strncmp((char *) cmd_buffer, (char *) "JT", 2) == 0) {
        flag_100 = false;
        flag_200 = false;
        flag_50 = false;
        flag_shouqu = false;
        flag_xiqu = false;
        flag_mf = false;//电机上升
        flag_ms = false;//电机停止
        flag_mb = false;//电机下降
        flag_po = false;//泵out
        flag_ps = false;//泵停止
        flag_pi = false;//泵in
        flag_S100 = false;//手动时100
        flag_S200 = false;//手动时200
        flag_S50 = false;//手动时300
        pump_set_no_no_delay();
        motor_no_no_delay();
    } else if (strncmp((char *) cmd_buffer, (char *) "SS", 2) == 0) {
        PG_read_pump();
        // 说明此时Y轴并没有到达合适位置
        if (!flag_PG_pump[1]) {
            //屏幕显示，或者蜂鸣器叫之类的
//            sendMessage("ERROR Y");
            page_warn();
        } else if (flag_50||flag_100||flag_200) {
            // 中文：需要先停止自动操作
            uint8_t message[] = {0xD0,0xE8,0xD2,0xAA,0xCF,0xC8,0xB5,0xC8,0xB4,0xFD,0xD7,0xD4,0xB6,0xAF,0xB2,0xD9,0xD7,0xF7,0xBD,0xE1,0xCA,0xF8};
            sendMessage(message, 22);
        }else if (flag_mb) {
//            sendMessage("need to stop motor first");
            // 中文：需要先停止电机
            uint8_t message[] = {0xD0, 0xE8, 0xD2, 0xAA, 0xCF, 0xC8, 0xCD, 0xA3, 0xD6, 0xB9, 0xB5, 0xE7, 0xBB, 0xFA
            };
            sendMessage(message, 14);
        } else {
            flag_mf = true;//推杆电机上升
        }
    } else if (strncmp((char *) cmd_buffer, (char *) "XJ", 2) == 0) {
        PG_read_pump();
        // 说明此时Y轴并没有到达合适位置
        if (!flag_PG_pump[1]) {
            page_warn();
        } else if (flag_50||flag_100||flag_200) {
             // 中文：需要先等待自动操作结束
            uint8_t message[] = {0xD0,0xE8,0xD2,0xAA,0xCF,0xC8,0xB5,0xC8,0xB4,0xFD,0xD7,0xD4,0xB6,0xAF,0xB2,0xD9,0xD7,0xF7,0xBD,0xE1,0xCA,0xF8};
            sendMessage(message, 22);
        }
        else if (flag_mf) {

            // 中文：需要先停止电机
            uint8_t message[] = {0xD0, 0xE8, 0xD2, 0xAA, 0xCF, 0xC8, 0xCD, 0xA3, 0xD6, 0xB9, 0xB5, 0xE7, 0xBB, 0xFA
            };
            sendMessage(message, 14);
        } else {
            flag_mb = true;//推杆电机下降
        }
    } else if (strncmp((char *) cmd_buffer, (char *) "ZTZ", 3) == 0) {
        flag_100 = false;
        flag_200 = false;
        flag_50 = false;
    } else if (strncmp((char *) cmd_buffer, (char *) "TT", 2) == 0) {
        if (flag_50||flag_100||flag_200) {
             // 中文：需要先等待自动操作结束
            uint8_t message[] = {0xD0,0xE8,0xD2,0xAA,0xCF,0xC8,0xB5,0xC8,0xB4,0xFD,0xD7,0xD4,0xB6,0xAF,0xB2,0xD9,0xD7,0xF7,0xBD,0xE1,0xCA,0xF8};
            sendMessage(message, 22);
        }else {
            flag_ms = true;  // 推杆电机停止运动
            flag_mf = false;
            flag_mb = false;
        }
    } else if (strncmp((char *) cmd_buffer, (char *) "S100", 4) == 0) {
        PG_read_pump();
        // 说明此时Y轴并没有到达合适位置
        if (!flag_PG_pump[1]) {
            page_warn();
        }
        else if (flag_50||flag_100||flag_200) {
             // 中文：需要先等待自动操作结束
            uint8_t message[] = {0xD0,0xE8,0xD2,0xAA,0xCF,0xC8,0xB5,0xC8,0xB4,0xFD,0xD7,0xD4,0xB6,0xAF,0xB2,0xD9,0xD7,0xF7,0xBD,0xE1,0xCA,0xF8};
            sendMessage(message, 22);
        }else
            flag_S100 = true;  // 手动100
    } else if (strncmp((char *) cmd_buffer, (char *) "S200", 4) == 0) {
        PG_read_pump();
        // 说明此时Y轴并没有到达合适位置
        if (!flag_PG_pump[1]) {
            page_warn();
        }
        else if (flag_50||flag_100||flag_200) {
             // 中文：需要先等待自动操作结束
            uint8_t message[] = {0xD0,0xE8,0xD2,0xAA,0xCF,0xC8,0xB5,0xC8,0xB4,0xFD,0xD7,0xD4,0xB6,0xAF,0xB2,0xD9,0xD7,0xF7,0xBD,0xE1,0xCA,0xF8};
            sendMessage(message, 22);
        }else
            flag_S200 = true;  // 手动200
    } else if (strncmp((char *) cmd_buffer, (char *) "S50", 3) == 0) {
        PG_read_pump();
        // 说明此时Y轴并没有到达合适位置
        if (!flag_PG_pump[1]) {
            page_warn();
        }
        else if (flag_50||flag_100||flag_200) {
             // 中文：需要先等待自动操作结束
            uint8_t message[] = {0xD0,0xE8,0xD2,0xAA,0xCF,0xC8,0xB5,0xC8,0xB4,0xFD,0xD7,0xD4,0xB6,0xAF,0xB2,0xD9,0xD7,0xF7,0xBD,0xE1,0xCA,0xF8};
            sendMessage(message, 22);
        }else
            flag_S50 = true;  // 手动50
    } else if (strncmp((char *) cmd_buffer, (char *) "SCQ", 3) == 0) {
        if (!flag_po)
            flag_pi = true;  // 泵抽取，即in
        else if (flag_50||flag_100||flag_200) {
             // 中文：需要先等待自动操作结束
            uint8_t message[] = {0xD0,0xE8,0xD2,0xAA,0xCF,0xC8,0xB5,0xC8,0xB4,0xFD,0xD7,0xD4,0xB6,0xAF,0xB2,0xD9,0xD7,0xF7,0xBD,0xE1,0xCA,0xF8};
            sendMessage(message, 22);
        }
        else {
            // 中文：需要先停止泵
            uint8_t message[] = {0xD0, 0xE8, 0xD2, 0xAA, 0xCF, 0xC8, 0xCD, 0xA3, 0xD6, 0xB9, 0xB1, 0xC3
            };
            sendMessage(message, 12);
        }
    } else if (strncmp((char *) cmd_buffer, (char *) "SQX", 3) == 0) {
        if (!flag_pi)
            flag_po = true;  // 泵冲洗，即 out
        else if (flag_50||flag_100||flag_200) {
             // 中文：需要先等待自动操作结束
            uint8_t message[] = {0xD0,0xE8,0xD2,0xAA,0xCF,0xC8,0xB5,0xC8,0xB4,0xFD,0xD7,0xD4,0xB6,0xAF,0xB2,0xD9,0xD7,0xF7,0xBD,0xE1,0xCA,0xF8};
            sendMessage(message, 22);
        }
        else {
            // 中文：需要先停止泵
            uint8_t message[] = {0xD0, 0xE8, 0xD2, 0xAA, 0xCF, 0xC8, 0xCD, 0xA3, 0xD6, 0xB9, 0xB1, 0xC3
            };
            sendMessage(message, 12);
        }
//            sendMessage("need to stop pump first");
    } else if (strncmp((char *) cmd_buffer, (char *) "STZ", 3) == 0) {
        if (flag_50||flag_100||flag_200) {
             // 中文：需要先等待自动操作结束
            uint8_t message[] = {0xD0,0xE8,0xD2,0xAA,0xCF,0xC8,0xB5,0xC8,0xB4,0xFD,0xD7,0xD4,0xB6,0xAF,0xB2,0xD9,0xD7,0xF7,0xBD,0xE1,0xCA,0xF8};
            sendMessage(message, 22);
        } else {
            flag_ps = true;  // 泵停止
            flag_pi = false;
            flag_po = false;
        }
    }
        //复位，整体复位
    else if (strncmp((char *) cmd_buffer, (char *) "ZFW", 3) == 0 ||
             strncmp((char *) cmd_buffer, (char *) "FW", 2) == 0) {
        uint8_t message[] = {0xD7,0xBC,0xB1,0xB8,0xB8,0xB4,0xCE,0xBB};
        sendMessage(message, 8);
        __set_FAULTMASK(1);
        HAL_NVIC_SystemReset();
        flag_reset = true;

        flag_100 = false;
        flag_200 = false;
        flag_50 = false;
        flag_shouqu = false;
        flag_xiqu = false;
        flag_mf = false;//电机上升
        flag_ms = false;//电机停止
        flag_mb = false;//电机下降
        flag_po = false;//泵out
        flag_ps = false;//泵停止
        flag_pi = false;//泵in
        flag_S100 = false;//手动时100
        flag_S200 = false;//手动时200
        flag_S50 = false;//手动时300
        pump_set_no_no_delay();
        motor_no_no_delay();
    }
    else if (strncmp((char *) cmd_buffer, (char *) "SZ=", 3) == 0) {
        uint32_t values[MAX_VALUES];
        // 提取并存储数值
        extract_and_store(cmd_buffer, values);
        if(!flag_show)
        {
            pump_delay_time_100[0] = delay_time_sum - values[5];
            pump_delay_time_100[1] = delay_time_sum - values[0];
            pump_delay_time_100[2] = delay_time_sum - values[4];
            pump_delay_time_100[3] = delay_time_sum - values[1];
            pump_delay_time_100[4] = delay_time_sum - values[3];
            pump_delay_time_100[5] = delay_time_sum - values[2];
        }
        else
        {
            pump_delay_time_50[0] = delay_time_sum - values[5];
            pump_delay_time_50[1] = delay_time_sum - values[0];
            pump_delay_time_50[2] = delay_time_sum - values[4];
            pump_delay_time_50[3] = delay_time_sum - values[1];
            pump_delay_time_50[4] = delay_time_sum - values[3];
            pump_delay_time_50[5] = delay_time_sum - values[2];
        }

        if(parameter_save){
            if((WriteToFlash((uint32_t*)&pump_delay_time_50[0],(uint32_t*)&pump_delay_time_100[0])==HAL_OK)) {
                // 中文：参数保存成功
                uint8_t message[] = {0xB2,0xCE,0xCA,0xFD,0xB1,0xA3,0xB4,0xE6,0xB3,0xC9,0xB9,0xA6
                };
                sendMessage(message, 12);
            }
        }
    }
//
//    else if (strncmp((char *) cmd_buffer, (char *) "SZ50=", 5) == 0) {
//        uint32_t values[MAX_VALUES];
//
//        // 提取并存储数值
//        extract_and_store(cmd_buffer, values);
//        pump_delay_time_50[0] = delay_time_sum - values[5];
//        pump_delay_time_50[1] = delay_time_sum - values[0];
//        pump_delay_time_50[2] = delay_time_sum - values[4];
//        pump_delay_time_50[3] = delay_time_sum - values[1];
//        pump_delay_time_50[4] = delay_time_sum - values[3];
//        pump_delay_time_50[5] = delay_time_sum - values[2];
//
//        if(parameter_save){
//            if((WriteToFlash((uint32_t*)&pump_delay_time_50[0],(uint32_t*)&pump_delay_time_100[0])==HAL_OK)) {
//                // 中文：参数保存成功
//                uint8_t message[] = {0xB2,0xCE,0xCA,0xFD,0xB1,0xA3,0xB4,0xE6,0xB3,0xC9,0xB9,0xA6
//                };
//                sendMessage(message, 12);
//            }
//        }
//
//    } else if (strncmp((char *) cmd_buffer, (char *) "SZ100=", 6) == 0) {
//        uint32_t values[MAX_VALUES];
//
//        // 提取并存储数值
//        extract_and_store(cmd_buffer, values);
//        pump_delay_time_100[0] = delay_time_sum - values[5];
//        pump_delay_time_100[1] = delay_time_sum - values[0];
//        pump_delay_time_100[2] = delay_time_sum - values[4];
//        pump_delay_time_100[3] = delay_time_sum - values[1];
//        pump_delay_time_100[4] = delay_time_sum - values[3];
//        pump_delay_time_100[5] = delay_time_sum - values[2];
//        if(parameter_save) {
//            if (WriteToFlash((uint32_t*)&pump_delay_time_50[0], (uint32_t*)&pump_delay_time_100[0]) == HAL_OK) {
//                // 中文：参数保存成功
//                uint8_t message[] = {0xB2,0xCE,0xCA,0xFD,0xB1,0xA3,0xB4,0xE6,0xB3,0xC9,0xB9,0xA6
//                };
//                sendMessage(message, 12);
//            }
//        }
//    }
    else if (strncmp((char *) cmd_buffer, (char *) "XSQH", 4) == 0)
    {
        if(flag_show) {
            parameter_show_100();
            flag_show=false;
        }
        else {
            parameter_show_50();
            flag_show=true;

        }
    }
//    // 初始化复位
//    else if(strncmp((char *)cmd_buffer, (char *)"ABC", 3)==0) {
//        HAL_NVIC_SystemReset();
//    }
}


#include "string.h"  // for strlen

//传入需要显示的信息作为参数
//void sendMessage(const char* message) {
//    char send_buf[40];
//
//    // 使用传入的错误信息格式化字符串
//    sprintf(send_buf, "t6.txt=\"%s\"", message);
//
//    // 发送格式化后的数据
//    HAL_UART_Transmit(COMMAND_UART, (uint8_t *)send_buf, strlen(send_buf), 100);
//
//    // 加上三个 0xFF 结尾
//    uint8_t end_bytes[3] = {0xFF, 0xFF, 0xFF};
//    HAL_UART_Transmit(COMMAND_UART, end_bytes, 3, 100);
//}

void page_warn()
{
    char send_buf[10];

    // 使用传入的错误信息格式化字符串
    sprintf(send_buf,"page 9");

    // 发送格式化后的数据
    HAL_UART_Transmit(COMMAND_UART, (uint8_t *)send_buf, strlen(send_buf), 100);

    // 加上三个 0xFF 结尾
    uint8_t end_bytes[3] = {0xFF, 0xFF, 0xFF};
    HAL_UART_Transmit(COMMAND_UART, end_bytes, 3, 100);
}
void sendMessage(uint8_t * message,uint8_t length) {
    char send_buf[10];  // 缓冲区

    // 准备格式化字符串，格式为 "t6.txt=\"message\""
    sprintf(send_buf, "t6.txt=\"");

    // 发送格式化后的字符串（前缀部分）
    HAL_UART_Transmit(&huart1, (uint8_t *)send_buf, strlen(send_buf), 1000);

    // 发送 GB2312 编码的字节流（直接传递已经定义好的字节数组）
    HAL_UART_Transmit(&huart1, message, length, 1000);

    // 发送格式化字符串的后缀部分
    sprintf(send_buf, "\"");
    HAL_UART_Transmit(&huart1, (uint8_t *)send_buf, strlen(send_buf), 100);

    // 加上三个 0xFF 结尾
    uint8_t end_bytes[3] = {0xFF, 0xFF, 0xFF};
    HAL_UART_Transmit(&huart1, end_bytes, 3, 100);
}


void extract_and_store(const char* str, uint32_t* arr)
{
    // 找到 '=' 字符的位置
    char* equal_pos = strchr(str, '=');

    if (equal_pos != NULL) {
        // 获取 '=' 后面的子字符串（不包括 '=' 本身）
        equal_pos++;  // 跳过 '='

        // 使用 strtok 分割字符串
        char* token = strtok(equal_pos, ",");
        int index = 0;

        // 逐个提取并存储数值到数组
        while (token != NULL && index < MAX_VALUES) {
            arr[index] = (uint32_t) atoi(token);  // 使用 atoi 将字符串转换为 uint32_t
            token = strtok(NULL, ",");  // 获取下一个数值
            index++;
        }
    }
}

void parameter_show_100()
{
    uint16_t values[6];
    values[0] = delay_time_sum - pump_delay_time_100[1];
    values[1] = delay_time_sum - pump_delay_time_100[3];
    values[2] = delay_time_sum - pump_delay_time_100[5];
    values[3] = delay_time_sum - pump_delay_time_100[4];
    values[4] = delay_time_sum - pump_delay_time_100[2];
    values[5] = delay_time_sum - pump_delay_time_100[0];

    uint8_t send_buffer[50];
    sprintf(send_buffer, "b9.txt=\"%s\"","100ml");
    HAL_UART_Transmit(&huart1, (uint8_t *)send_buffer, strlen(send_buffer), 100);
    // 加上三个 0xFF 结尾
    uint8_t end_bytes[3] = {0xFF, 0xFF, 0xFF};
    HAL_UART_Transmit(&huart1, end_bytes, 3, 100);
    for(int i=0;i<6;i++)
    {
        sprintf(send_buffer, "h%d.val=%d",i,values[i]);
        HAL_UART_Transmit(&huart1, (uint8_t *)send_buffer, strlen(send_buffer), 100);

        // 加上三个 0xFF 结尾
        uint8_t end_bytes[3] = {0xFF, 0xFF, 0xFF};
        HAL_UART_Transmit(&huart1, end_bytes, 3, 100);
    }
    for(int i=0;i<6;i++)
    {
        sprintf(send_buffer, "n%d.val=%d",i,values[i]);
        HAL_UART_Transmit(&huart1, (uint8_t *)send_buffer, strlen(send_buffer), 100);

        // 加上三个 0xFF 结尾
        uint8_t end_bytes[3] = {0xFF, 0xFF, 0xFF};
        HAL_UART_Transmit(&huart1, end_bytes, 3, 100);
    }
}

void parameter_show_50()
{
    uint16_t values[6];
    values[0] = delay_time_sum - pump_delay_time_50[1];
    values[1] = delay_time_sum - pump_delay_time_50[3];
    values[2] = delay_time_sum - pump_delay_time_50[5];
    values[3] = delay_time_sum - pump_delay_time_50[4];
    values[4] = delay_time_sum - pump_delay_time_50[2];
    values[5] = delay_time_sum - pump_delay_time_50[0];

    uint8_t send_buffer[50];
    sprintf(send_buffer, "b9.txt=\"%s\"","50ml");
    HAL_UART_Transmit(&huart1, (uint8_t *)send_buffer, strlen(send_buffer), 100);
    // 加上三个 0xFF 结尾
    uint8_t end_bytes[3] = {0xFF, 0xFF, 0xFF};
    HAL_UART_Transmit(&huart1, end_bytes, 3, 100);
    for(int i=0;i<6;i++)
    {
        sprintf(send_buffer, "h%d.val=%d",i,values[i]);
        HAL_UART_Transmit(&huart1, (uint8_t *)send_buffer, strlen(send_buffer), 100);

        // 加上三个 0xFF 结尾
        uint8_t end_bytes[3] = {0xFF, 0xFF, 0xFF};
        HAL_UART_Transmit(&huart1, end_bytes, 3, 100);
    }
    for(int i=0;i<6;i++)
    {
        sprintf(send_buffer, "n%d.val=%d",i,values[i]);
        HAL_UART_Transmit(&huart1, (uint8_t *)send_buffer, strlen(send_buffer), 100);

        // 加上三个 0xFF 结尾
        uint8_t end_bytes[3] = {0xFF, 0xFF, 0xFF};
        HAL_UART_Transmit(&huart1, end_bytes, 3, 100);
    }
}