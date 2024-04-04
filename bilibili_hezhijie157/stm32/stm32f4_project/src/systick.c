#include "systick.h"

uint32 sysTimer[MAX_TIMER];
static uint32 g_iRunTimer = 0; //定义系统全局运行时间 

// 我们要用滴答定时器 1ms周期触发一次中断
// 时钟源 1、AHB = SYSCLK 2、AHB/8 
// AHB = 168MHZ
// 1s 计数168M次 1ms 计数168000次  预置数字 168000

void Systick_Init(void)
{
    unsigned char i;
    for(i = 0; i < MAX_TIMER; i++)
    {
        sysTimer[i] = 0;
    }
    // 滴答定时器时钟源选择
    // SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
    // 默认时钟为系统时钟AHB总线 HCLK 
    SysTick_Config(168000000/1000); // 每隔1ms进入一次中断
}

void SysTick_Handler(void)
{
    unsigned char i;
    for(i = 0; i < MAX_TIMER; i++)
    {   
        if(sysTimer[i])
            sysTimer[i]--;
    }
    if(g_iRunTimer++ > 0xFFFFFFFF)
        g_iRunTimer = 0;
}

void SystickDelayMs(uint32 nms)
{
    CPU_INT_ENABLE(); // 打开中断 
    DelayTimer = nms;
    while(DelayTimer); // 等待延时完成
}

uint32 GetRunTime(void)
{
    unsigned int RunTime;

    CPU_INT_DISABLE(); // 关闭中断
    // 防止g_iRunTimer冲突
    RunTime = g_iRunTimer;
    CPU_INT_ENABLE(); // 开中断
    return RunTime;
}

