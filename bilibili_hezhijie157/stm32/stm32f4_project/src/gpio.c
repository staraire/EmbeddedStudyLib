#include "gpio.h"

void LED_GPIO_Init(void)
{

    GPIO_InitTypeDef GPIO_InitStructure; // IO配置结构体
    // 时钟使能
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
    // 初始GPIO
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;   //IO口
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;           //输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          //推挽输出 
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;            //没有输出时上拉
    GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;        //50Mhz
    GPIO_Init(GPIOF,&GPIO_InitStructure);
    // 初始默认输出高
    GPIO_SetBits(GPIOF,GPIO_Pin_9|GPIO_Pin_10);

}


void LEDControl(void)
{
    GPIO_SetBits(GPIOF,GPIO_Pin_9);
    GPIO_ResetBits(GPIOF,GPIO_Pin_10);
    SystickDelayMs(1000);
    GPIO_SetBits(GPIOF,GPIO_Pin_10);
    GPIO_ResetBits(GPIOF,GPIO_Pin_9);
    SystickDelayMs(1000);
}





