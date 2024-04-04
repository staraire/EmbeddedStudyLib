#ifndef GPIO_H
#define GPIO_H

#include "config.h"

void LED_GPIO_Init(void); // LED初始化
void LEDControl(void);    // LED控制

void BEEP_GPIO_Init(void); // 蜂鸣器初始化
void BEEP_ON(void); // 开蜂鸣器
void BEEP_OFF(void); // 关蜂鸣器






#endif
