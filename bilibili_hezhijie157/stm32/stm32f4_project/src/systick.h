#ifndef SYSTICK_H
#define SYSTICK_H

#include "config.h"

#define MAX_TIMER 8 // 利用系统滴答定时器实现软件定时器的个数

extern uint32 sysTimer[MAX_TIMER]; // 
#define DelayTimer sysTimer[0] // 延时定时器
#define ATaskTimer sysTimer[1] // A任务运行定时器
#define BTaskTimer sysTimer[2] // B 任务运行定时器
#define CTaskTimer sysTimer[3] // C 任务运行定时器
#define Usart1RecTimer sysTimer[4] // 串口1接收定时器，如果超时，表示一帧接收完毕
#define Usart2RecTimer sysTimer[5] // 串口2接收定时器，如果超时，表示一帧接收完毕
#define Usart3RecTimer sysTimer[6] // 串口3接收定时器，如果超时，表示一帧接收完毕
#define Usart1ControlBeepTimer sysTimer[7] // 串口1控制蜂鸣器定时器

void Systick_Init(void); // 滴答定时器初始化
void SystickDelayMs(uint32 nms); // ms延时
uint32 GetRunTime(void); // 获取系统运行时间






#endif


