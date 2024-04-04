#ifndef USART_H
#define USART_H

#include "config.h"

#define USART1_BAUD          115200
#define USART1_RX_BUF_SIZE   256

extern unsigned char g_USART1_RxBuf[USART1_RX_BUF_SIZE]; // 串口1接收缓冲数组
extern unsigned int g_USART1_RecPos; // 存放当前串口接收数据存放的位置




void USART1_Init(void); // 串口1初始化
void USARTSendByte(USART_TypeDef*, unsigned char); // 发送单字节
void USARTSendString(USART_TypeDef*, unsigned char *); // 发送字符串

unsigned char USART1_RecProcess(void); // 串口1接收处理函数

#endif
