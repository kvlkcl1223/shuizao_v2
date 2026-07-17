//
// Created by 张文波 on 2025/9/26.
//

#ifndef COMMAND_H
#define COMMAND_H

#include "usart.h"
#include "string.h"
#include "stdbool.h"

#define COMMAND_UART &huart1
// 自动部分
extern volatile bool flag_start;
extern volatile bool flag_100;
extern volatile bool flag_200;
extern volatile bool flag_50;
extern volatile bool flag_shouqu;
extern volatile bool flag_xiqu;
//手动部分
extern volatile bool flag_mf;//电机上升
extern volatile bool flag_ms;//电机停止
extern volatile bool flag_mb;//电机下降

extern volatile bool flag_po;//泵out
extern volatile bool flag_ps;//泵停止
extern volatile bool flag_pi;//泵in

extern volatile bool flag_S100;//手动时100
extern volatile bool flag_S200;//手动时200
extern volatile bool flag_S50;//手动时300

extern volatile bool flag_reset;

void UART_Command_Init();
void UART_CALLBACK();
void UART_Command_Parse(uint8_t *cmd_buffer);
void sendMessage(uint8_t* message,uint8_t length);
void page_warn();
static void extract_and_store(const char* str, uint32_t* arr);
void parameter_show_50();
void parameter_show_100();
#endif //COMMAND_H
