/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"

/* FreeRTOS头文件 */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
/* 开发板硬件bsp头文件 */
#include "bsp_exti.h"

#include "drv_opt.h"
/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
extern void xPortSysTickHandler(void);
//systick中断服务函数
void SysTick_Handler(void)
{	
#if (INCLUDE_xTaskGetSchedulerState  == 1 )
  if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
  {
#endif  /* INCLUDE_xTaskGetSchedulerState */  
    
    xPortSysTickHandler();
    
#if (INCLUDE_xTaskGetSchedulerState  == 1 )
  }
#endif  /* INCLUDE_xTaskGetSchedulerState */
}



extern SemaphoreHandle_t BinarySem_Handle;

/*--------------------------------------------------------------------------------*/
//串口中断服务函数
void DEBUG_USART_IRQHandler(void)
{
	uint32_t ulReturn;
	uint8_t ucTemp;
	/* 进入临界段，临界段可以嵌套 */
	ulReturn = taskENTER_CRITICAL_FROM_ISR();

	if(USART_GetITStatus(DEBUG_USARTx,USART_IT_RXNE)!=RESET)
	{		
		ucTemp = USART_ReceiveData(DEBUG_USARTx);
		
		//USART_SendData(DEBUG_USARTx,ucTemp);    
		//dbg->printf("%d \r\n",ucTemp);
	}	 
	/* 退出临界段 */
	taskEXIT_CRITICAL_FROM_ISR( ulReturn );
}
/*--------------------------------------------------------------------------------*/
//串口2中断服务函数
void USART2_IRQHandler(void)
{
	uint32_t ulReturn;
	uint8_t ucTemp;
	/* 进入临界段，临界段可以嵌套 */
	ulReturn = taskENTER_CRITICAL_FROM_ISR();
	
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)
	{		
		ucTemp = USART_ReceiveData(USART2);		
		//cnt->rec_data_isr(ucTemp);
	}	 
	/* 退出临界段 */
	taskEXIT_CRITICAL_FROM_ISR( ulReturn );
}

/*--------------------------------------------------------------------------------*/
//@通用定时器中断服务函数
extern void add_systick(void);
void  GENERAL_TIM_IRQHandler (void)
{
	if ( TIM_GetITStatus( GENERAL_TIM, TIM_IT_Update) != RESET ) 
	{	
		add_systick();
		TIM_ClearITPendingBit(GENERAL_TIM , TIM_FLAG_Update);  		 
	}		 	
}

/*--------------------------------------------------------------------------------*/
extern void add_pwm_tick(void);

void  TIM3_IRQHandler (void)
{
	if ( TIM_GetITStatus( TIM3, TIM_IT_Update) != RESET ) 
	{	
		TIM_ClearITPendingBit( TIM3 , TIM_FLAG_Update);  		 
	}		 	
}



/*--------------------------------------------------------------------------------*/
//外部中断
void EXTI15_10_IRQHandler(void)
{
  //查询方式判断是否产生了对应的EXTI Line中断
	if(EXTI_GetITStatus(EXTI_Line11) != RESET) 
	{
		if( 0 == GPIO_ReadInputDataBit( GPIOA, GPIO_Pin_11) )//下降沿
			//coin_p1->trigger_isr( );
    	//清除中断标志位
		EXTI_ClearITPendingBit(EXTI_Line11);     
	} 
	
	if(EXTI_GetITStatus(EXTI_Line12) != RESET) 
	{
		if( 0 == GPIO_ReadInputDataBit( GPIOA, GPIO_Pin_12) )//下降沿
			//coin_p2->trigger_isr( );
    	//清除中断标志位
		EXTI_ClearITPendingBit(EXTI_Line12);     
	}  

	if(EXTI_GetITStatus(EXTI_Line15) != RESET) 
	{
		if( 0 == GPIO_ReadInputDataBit( GPIOC, GPIO_Pin_15) )//下降沿
			//eye_egg->trigger_isr( );
    	//清除中断标志位
		EXTI_ClearITPendingBit(EXTI_Line15);     
	}  
	
}








