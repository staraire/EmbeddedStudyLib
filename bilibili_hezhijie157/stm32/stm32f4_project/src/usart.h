#ifndef USART_H
#define USART_H

#include "config.h"

#define USART1_EN 1
#define USART2_EN 1
#define USART3_EN 1


#if USART1_EN == 1
    #define USART1_BAUD          115200
    #define USART1_RX_BUF_SIZE   256

	#define USART1_TX_GPIO GPIOA
	#define USART1_TX_GPIO_Pin GPIO_Pin_9
	#define USART1_RX_GPIO GPIOA
	#define USART1_RX_GPIO_Pin GPIO_Pin_10
#endif

#if USART2_EN == 1
    #define USART2_BAUD          115200
    #define USART2_RX_BUF_SIZE   256

	#define USART2_TX_GPIO GPIOA
	#define USART2_TX_GPIO_Pin GPIO_Pin_2
	#define USART2_RX_GPIO GPIOA
	#define USART2_RX_GPIO_Pin GPIO_Pin_3
#endif

#if USART3_EN == 1
    #define USART3_BAUD          115200
    #define USART3_RX_BUF_SIZE   256

	#define USART3_TX_GPIO GPIOB
	#define USART3_TX_GPIO_Pin GPIO_Pin_10
	#define USART3_RX_GPIO GPIOB
	#define USART3_RX_GPIO_Pin GPIO_Pin_11
#endif

#if USART1_EN == 1
    extern unsigned char g_USART1_RxBuf[USART1_RX_BUF_SIZE]; // 串口1接收缓冲数组
    extern unsigned int g_USART1_RecPos; // 存放当前串口接收数据存放的位置
#endif

#if USART2_EN == 1
    extern unsigned char g_USART2_RxBuf[USART2_RX_BUF_SIZE]; // 串口1接收缓冲数组
    extern unsigned int g_USART2_RecPos; // 存放当前串口接收数据存放的位置
#endif

#if USART3_EN == 1
    extern unsigned char g_USART3_RxBuf[USART3_RX_BUF_SIZE]; // 串口1接收缓冲数组
    extern unsigned int g_USART3_RecPos; // 存放当前串口接收数据存放的位置
#endif

void USART_USER_Init(void); // 串口初始化
void USARTSendByte(USART_TypeDef*, unsigned char); // 发送单字节
void USARTSendString(USART_TypeDef*, unsigned char *); // 发送字符串
#if USART1_EN == 1
    unsigned char USART1_RecProcess(void); // 串口1接收处理函数
#endif
#if USART2_EN == 1
    unsigned char USART2_RecProcess(void); // 串口1接收处理函数
#endif
#if USART3_EN == 1
    unsigned char USART3_RecProcess(void); // 串口1接收处理函数
#endif



#endif
