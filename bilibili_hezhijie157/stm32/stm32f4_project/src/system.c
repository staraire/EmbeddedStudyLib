#include "system.h"
// 任务函数
void ATask(void);
void BTask(void);
void CTask(void);
void Usart1ControlBeepTask(void);
// 任务周期
#define ATaskPeriod 1000
#define BTaskPeriod 2000
#define CTaskPeriod 5000
#define Usart1ControlBeepTaskPeriod 20

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
    Task_Init(); // 任务初始化
	
    // 外设初始化 
    Systick_Init(); // 系统滴答定时器初始化
    LED_GPIO_Init(); // LED 初始化
		BEEP_GPIO_Init(); // BEEP 初始化
		USART1_Init(); // 串口初始化
    

}

void Task_Init(void)
{
    ATaskTimer = ATaskPeriod;
    BTaskTimer = BTaskPeriod;
    CTaskTimer = CTaskPeriod;
    Usart1ControlBeepTimer = Usart1ControlBeepTaskPeriod;
}


void Task_Run(void)
{
    ATask();
    BTask();
    CTask();
    Usart1ControlBeepTask();
}


void ATask(void)
{
    if(ATaskTimer) return;
    ATaskTimer = ATaskPeriod;
    GPIO_ToggleBits(GPIOF,GPIO_Pin_9);
}

void BTask(void)
{
    if(BTaskTimer) return;
    BTaskTimer = BTaskPeriod;
    GPIO_ToggleBits(GPIOF,GPIO_Pin_10);
}

void CTask(void)
{
    if(CTaskTimer) return;
    CTaskTimer = CTaskPeriod;
    printf("System Total time is %f s\n",GetRunTime()/1000.0);
    USARTSendString(USART1,"CTask is running\n");
}

void Usart1ControlBeepTask(void)
{
    if(Usart1ControlBeepTimer) return;
    Usart1ControlBeepTimer = Usart1ControlBeepTaskPeriod;
    USART1_RecProcess(); // 串口1接收处理
}



