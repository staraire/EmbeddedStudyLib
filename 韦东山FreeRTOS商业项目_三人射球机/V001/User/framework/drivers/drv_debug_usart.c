


#include "drv_opt.h"
#include "fk_soft_typedef.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>


/*--------------------------------------------------------------------------------*/
extern loop_pt loop;
extern void init_drv_loop_port(void);

static dbg_opt_t do_dbg;



/*--------------------------------------------------------------------------------*/

/**
  * @brief  配置嵌套向量中断控制器NVIC - USART
  * @param  无
  * @retval 无
  */
static void USART_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 嵌套向量中断控制器组选择 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
  
  /* 配置USART为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
  /* 抢断优先级*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
  /* 子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  /* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure);
}

 
/**
  * @brief  USART GPIO 配置,工作参数配置
  * @param  无
  * @retval 无
  */
static void USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// 打开串口GPIO的时钟
	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);
	
	// 打开串口外设的时钟
	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);

	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  // 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// 配置串口的工作参数
	// 配置波特率
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
	// 配置 针数据字长
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// 配置停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// 配置校验位
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// 配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl = 
	USART_HardwareFlowControl_None;
	// 配置工作模式，收发一起
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// 完成串口的初始化配置
	USART_Init(DEBUG_USARTx, &USART_InitStructure);
	
	// 串口中断优先级配置
	USART_NVIC_Configuration();
	// 使能串口接收中断
	USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);	
	
	// 使能串口
	USART_Cmd(DEBUG_USARTx, ENABLE);	    
}



/*****************  发送一个字节 **********************/
static void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* 发送一个字节数据到USART */
	USART_SendData(pUSARTx,ch);
		
	/* 等待发送数据寄存器为空 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

///重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到串口 */
		USART_SendData(DEBUG_USARTx, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		/* 等待串口输入数据 */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(DEBUG_USARTx);
}


/*--------------------------------------------------------------------------------*/




/*--------------------------------------------------------------------------------*/
//使用队列方式传递打印信息
static thread_mq_t ptf_mq;
#define size_ptf_buf	60
void mq_printf(char* format,...)
{
	char  buf_str[size_ptf_buf];
	va_list   v_args;
	va_start(v_args, format);
	(void)vsnprintf(  (char       *)(&buf_str[0]),
	                  (size_t      ) sizeof(buf_str),
	                  (char const *) format,
	                                 v_args);
	   va_end(v_args);
	//使用队列将打印内容发送出去
	thread_mq_send(ptf_mq,buf_str, size_ptf_buf, 0xff);
	
}
void mq_printf_isr(char* format,...)
{
	char  buf_str[size_ptf_buf];
	va_list   v_args;
	va_start(v_args, format);
	(void)vsnprintf(  (char       *)(&buf_str[0]),
	                  (size_t      ) sizeof(buf_str),
	                  (char const *) format,
	                                 v_args);
	   va_end(v_args);
	//使用队列将打印内容发送出去
	thread_mq_send_irq(ptf_mq,buf_str, size_ptf_buf, 1);
	
}

void mq_deal_coroutine(void)
{	
	u8 buf[size_ptf_buf];
	int ret = 0;
	//从队列中读出数据
	ret = thread_mq_recv(ptf_mq, buf, size_ptf_buf, 1)
	if(ret!=bFALSE) printf("%s",buf);
}


/*--------------------------------------------------------------------------------*/
dbg_opt_pt dbg;


//操作系统起来之前，初始化此函数，可使用 printf()函数打印内容
void init_drv_debug_usart_before_os_run(void)
{
	USART_Config();
	//printf("init_drv_debug_usart_before_os_run \r\n");
	dbg = &do_dbg;	
	
	dbg->deal_coroutine = mq_deal_coroutine;
	dbg->printf = mq_printf;
	dbg->printf_isr = mq_printf_isr;
}





void init_drv_debug_usart_after_os_run(void)
{
	/*构建项目相关的调试环境，涉及操作系统对象，需要在操作系统起来后使用，并且在临界区内。*/
	/*依赖于loop结构，需要在loop初始化后才能正常使用创建环境*/
	init_drv_loop_port();
	ptf_mq = thread_mq_init("ptf_mq", size_ptf_buf, 30);
	
}











