#ifndef SYSTICK_H
#define SYSTICK_H

#include "config.h"

#define MAX_TIMER 5 // 利用系统滴答定时器实现软件定时器的个数

extern uint32 sysTimer[MAX_TIMER]; // 
#define DelayTimer sysTimer[0] // 延时定时器
#define ATaskTimear sysTimer[1] // A任务运行定时器
#define BTaskTimer sysTimer[2] // B 任务运行定时器
#define CTaskTimer sysTimer[3] // C 任务运行定时器

void Systick_Init(void); // 滴答定时器初始化
void SystickDelayMs(uint32 nms); // ms延时
uint32 GetRunTime(void); // 获取系统运行时间






#endif


