#include "exti.h"

void EXTI_KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    //时钟使能
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
    //按键初始化
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; //输入模式
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //按键接地，所以这里接上拉
    GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;
    GPIO_Init(GPIOE,&GPIO_InitStructure);
}

void EXTI_USER_Init(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    // 时钟使能
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    // 按键GPIO初始化
    EXTI_KEY_Init();
    // 设置IO口与中断线映射关系
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource4);
    // 初始化线上中断
    EXTI_InitStructure.EXTI_Line = EXTI_Line4;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger =EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    // 中断优先级初始化
    NVIC_InitStructure.NVIC_IRQChannel=EXTI4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


void EXTI4_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line4) == SET)
    {
        printf("key press\n");
    }
    EXTI_ClearITPendingBit(EXTI_Line4);
}












