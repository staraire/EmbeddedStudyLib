#include "system.h"


void CPU_Init(void)
{
    // CPU相关设置
    // 优先级分组 选择第2组
		// 即2位主优先级 2位次优先级
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
}


void System_Init(void)
{

    CPU_Init(); // 配置系统信息
	
		// 外设初始化 
		Systick_Init(); // 系统滴答定时器初始化
		LED_GPIO_Init(); // LED 初始化

}






