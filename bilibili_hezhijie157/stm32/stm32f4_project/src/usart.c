#include "usart.h"

#if USART1_EN == 1
    unsigned char g_USART1_RxBuf[USART1_RX_BUF_SIZE]; // 串口1接收缓冲数组
    unsigned int g_USART1_RecPos = 0; // 存放当前串口接收数据存放的位置
#endif

#if USART2_EN == 1
    unsigned char g_USART2_RxBuf[USART2_RX_BUF_SIZE]; // 串口1接收缓冲数组
    unsigned int g_USART2_RecPos = 0; // 存放当前串口接收数据存放的位置
#endif

#if USART3_EN == 1
    unsigned char g_USART3_RxBuf[USART3_RX_BUF_SIZE]; // 串口1接收缓冲数组
    unsigned int g_USART3_RecPos = 0; // 存放当前串口接收数据存放的位置
#endif

void USART_USER_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;//IO配置结构体
    USART_InitTypeDef USART_InitStructure;//串口配置结构体
    NVIC_InitTypeDef NVIC_InitStructure;
#if USART1_EN
    // 使能串口时钟和引脚时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
    // 初始GPIO
    GPIO_InitStructure.GPIO_Pin = USART1_TX_GPIO_Pin|USART1_RX_GPIO_Pin; //IO口
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输出 
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //没有输出时上拉
    GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed; //50Mhz
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    // 复用配置
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
    // 串口参数初始化
    USART_InitStructure.USART_BaudRate = USART1_BAUD;
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
#endif

#if USART2_EN
    // 使能串口时钟和引脚时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
    // 初始GPIO
    GPIO_InitStructure.GPIO_Pin = USART2_TX_GPIO_Pin|USART2_RX_GPIO_Pin; //IO口
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输出 
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //没有输出时上拉
    GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed; //50Mhz
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    // 复用配置
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);
    // 串口参数初始化
    USART_InitStructure.USART_BaudRate = USART2_BAUD;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART2,&USART_InitStructure);
    // 串口使能
    USART_Cmd(USART2,ENABLE);
    // 中断初始化
    NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    // 初始化串口中断
    USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
    // 清除发送完成标志位
    USART_ClearFlag(USART2,USART_FLAG_TC);
#endif


#if USART3_EN
    // 使能串口时钟和引脚时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
    // 初始GPIO
    GPIO_InitStructure.GPIO_Pin = USART3_TX_GPIO_Pin|USART3_RX_GPIO_Pin; //IO口
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输出 
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //没有输出时上拉
    GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed; //50Mhz
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    // 复用配置
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);
    // 串口参数初始化
    USART_InitStructure.USART_BaudRate = USART3_BAUD;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART3,&USART_InitStructure);
    // 串口使能
    USART_Cmd(USART3,ENABLE);
    // 中断初始化
    NVIC_InitStructure.NVIC_IRQChannel=USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    // 初始化串口中断
    USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
    // 清除发送完成标志位
    USART_ClearFlag(USART3,USART_FLAG_TC);
#endif

}


#if USART1_EN == 1
void USART1_IRQHandler(void)
{
    unsigned char RecCh;
    // 串口接收数据
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
				Usart1RecTimer = 10;
        RecCh = USART_ReceiveData(USART1);
        g_USART1_RxBuf[g_USART1_RecPos++] = RecCh;
        USART_ClearITPendingBit(USART1,USART_IT_RXNE);
    }
    // 串口溢出错误
    if(USART_GetFlagStatus(USART1, USART_FLAG_ORE) == SET)
    {   
        USART_ClearFlag(USART1,USART_FLAG_ORE);
    }
}
#endif

#if USART2_EN == 1
void USART2_IRQHandler(void)
{
    unsigned char RecCh;
    // 串口接收数据
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {

        RecCh = USART_ReceiveData(USART2);
        g_USART2_RxBuf[g_USART2_RecPos++] = RecCh;
        USART_ClearITPendingBit(USART2,USART_IT_RXNE);
    }
    // 串口溢出错误
    if(USART_GetFlagStatus(USART2, USART_FLAG_ORE) == SET)
    {   
        USART_ClearFlag(USART2,USART_FLAG_ORE);
    }
}
#endif

#if USART3_EN == 1
void USART3_IRQHandler(void)
{
    unsigned char RecCh;
    // 串口接收数据
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {

        RecCh = USART_ReceiveData(USART3);
        g_USART3_RxBuf[g_USART3_RecPos++] = RecCh;
        USART_ClearITPendingBit(USART3,USART_IT_RXNE);
    }
    // 串口溢出错误
    if(USART_GetFlagStatus(USART3, USART_FLAG_ORE) == SET)
    {   
        USART_ClearFlag(USART3,USART_FLAG_ORE);
    }
}
#endif

#if USART1_EN == 1
unsigned char USART1_RecProcess(void)
{
    // 超时
    if(Usart1RecTimer) return FALSE;
    // 没数据
    if(!g_USART1_RecPos) return FALSE;
    g_USART1_RxBuf[g_USART1_RecPos] = '\0';
    // 收包处理
    if(strstr((const char *)g_USART1_RxBuf,"beep on") != NULL)
    {
        BEEP_ON();
    }
    else if(strstr((const char *)g_USART1_RxBuf,"beep off") != NULL)
    {
        BEEP_OFF();
    }
    memset(g_USART1_RxBuf,0,USART1_RX_BUF_SIZE);
    g_USART1_RecPos = 0;
    return TRUE;
}
#endif

#if USART2_EN == 1
unsigned char USART2_RecProcess(void)
{
    // 超时
    if(Usart2RecTimer) return FALSE;
    // 没数据
    if(!g_USART2_RecPos) return FALSE;
    g_USART2_RxBuf[g_USART2_RecPos] = '\0';
    // 收包处理

    memset(g_USART2_RxBuf,0,USART2_RX_BUF_SIZE);
    g_USART2_RecPos = 0;
    return TRUE;
}
#endif


#if USART3_EN == 1
unsigned char USART3_RecProcess(void)
{
    // 超时
    if(Usart3RecTimer) return FALSE;
    // 没数据
    if(!g_USART3_RecPos) return FALSE;
    g_USART3_RxBuf[g_USART3_RecPos] = '\0';
    // 收包处理

    memset(g_USART3_RxBuf,0,USART3_RX_BUF_SIZE);
    g_USART3_RecPos = 0;
    return TRUE;
}
#endif




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

///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
    /* 等待串口输入数据 */
    while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

    return (int)USART_ReceiveData(USART1);
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
















