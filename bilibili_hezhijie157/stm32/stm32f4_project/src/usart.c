#include "usart.h"

void USART1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;//IO配置结构体
    USART_InitTypeDef USART_InitStructure;//串口配置结构体
    NVIC_InitTypeDef NVIC_InitStructure;
    // 使能串口时钟和引脚时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
    // 初始GPIO
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10; //IO口
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输出 
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //没有输出时上拉
    GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed; //50Mhz
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    // 复用配置
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
    // 串口参数初始化
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART1,&USART_InitStructure);
    // 串口使能
    USART_Cmd(USART1,ENABLE);
    // 中断初始化
    NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    // 初始化串口中断
    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
    // 清除发送完成标志位
    USART_ClearFlag(USART1,USART_FLAG_TC);
		
}




void USARTSendByte(USART_TypeDef* USARTx, unsigned char ch)
{
    USART_SendData(USARTx,(unsigned char)ch);
    while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
}

// 重定义fputc函数 为了printf能够调用串口打印
int fputc(int ch, FILE *f)
{ 	
    USARTSendByte(USART1, ch);
	return ch;
}


void USARTSendString(USART_TypeDef* USARTx, unsigned char *str)
{
    unsigned int pos = 0;
    while(*(str+pos) != '\0')
    {
        USARTSendByte(USARTx, *(str+pos));
        pos++;
    }
}
















