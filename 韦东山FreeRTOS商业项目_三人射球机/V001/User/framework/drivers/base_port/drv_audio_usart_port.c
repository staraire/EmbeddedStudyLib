
#include "drv_opt.h"
/*--------------------------------------------------------------------------------*/

void set_audio_aux(u8 val)
{
	switch( val )
	{
		case 0: GPIO_ResetBits(GPIOA, GPIO_Pin_3);	break;
		case 1: GPIO_SetBits(GPIOA, GPIO_Pin_3);		break;
		default:
				printf("error: set_audio_aux!\r\n ");
		break;
	}
}

void init_audio_usart_port_1st(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	//串口2
	//初始化IO口及其时钟
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	//PA3-->AUX
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA, GPIO_Pin_3);
	
	
	//PA2-->TX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	
	//初始化外设及其时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	USART_InitStructure.USART_BaudRate 		= 9600;	
	USART_InitStructure.USART_WordLength 	= USART_WordLength_8b;
	USART_InitStructure.USART_StopBits 		= USART_StopBits_1;
	USART_InitStructure.USART_Parity 			= USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode 				= USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	// 使能串口
	USART_Cmd(USART2, ENABLE);
	
}

void init_audio_usart_port_2nd(void)
{
}


static void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* 发送一个字节数据到USART */
	USART_SendData(pUSARTx,ch);
		
	/* 等待发送数据寄存器为空 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

void send_char_to_audio_0(uint8_t ch)
{
	Usart_SendByte(USART2,ch);
}
void send_char_to_audio_1(uint8_t ch)
{
	Usart_SendByte(USART2,ch);
}
/*--------------------------------------------------------------------------------*/

