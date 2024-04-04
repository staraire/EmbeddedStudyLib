#ifndef CONFIG_H
#define CONFIG_H

// 无符号类型的定义
#define uchar unsigned char
#define uint unsigned int

#define uint8 unsigned char 
#define uint16 unsigned short int
#define uint32 unsigned int


#ifdef MAIN_CONFIG // 头文件被多个c调用时，避免变量冲突
    #define EXT
#else 
    #define EXT extern
#endif

// 全局变量定义，请使用EXT修饰
EXT unsigned char g_var;

//--------------------------

// 常用头文件包含
#include "stm32f4xx.h"
#include "stdio.h"
#include "string.h"
#include "math.h"

//--------------------------

// 外设驱动头文件包含
#include "system.h"
#include "systick.h"
#include "gpio.h"
#include "usart.h"

//--------------------------

// 全局宏定义
#define CPU_INT_DISABLE() {__set_PRIMASK(1);} // 关闭中断
#define CPU_INT_ENABLE() {__set_PRIMASK(0);}  // 打开中断

// 全局类型定义
typedef enum{FALSE = 0,TRUE = !FALSE}BOOL;

#endif


