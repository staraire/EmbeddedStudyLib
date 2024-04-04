#ifndef USART_H
#define USART_H

#include "config.h"


void USART1_Init(void); // 串口1初始化
void USARTSendByte(USART_TypeDef*, unsigned char); // 发送单字节
void USARTSendString(USART_TypeDef*, unsigned char *); // 发送字符串

#endif
