/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"

/* FreeRTOSͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
/* ������Ӳ��bspͷ�ļ� */
#include "bsp_exti.h"

#include "drv_opt.h"
/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
extern void xPortSysTickHandler(void);
//systick�жϷ�����
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
//�����жϷ�����
void DEBUG_USART_IRQHandler(void)
{
	uint32_t ulReturn;
	uint8_t ucTemp;
	/* �����ٽ�Σ��ٽ�ο���Ƕ�� */
	ulReturn = taskENTER_CRITICAL_FROM_ISR();

	if(USART_GetITStatus(DEBUG_USARTx,USART_IT_RXNE)!=RESET)
	{		
		ucTemp = USART_ReceiveData(DEBUG_USARTx);
		
		//USART_SendData(DEBUG_USARTx,ucTemp);    
		//dbg->printf("%d \r\n",ucTemp);
	}	 
	/* �˳��ٽ�� */
	taskEXIT_CRITICAL_FROM_ISR( ulReturn );
}
/*--------------------------------------------------------------------------------*/
//����2�жϷ�����
void USART2_IRQHandler(void)
{
	uint32_t ulReturn;
	uint8_t ucTemp;
	/* �����ٽ�Σ��ٽ�ο���Ƕ�� */
	ulReturn = taskENTER_CRITICAL_FROM_ISR();
	
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)
	{		
		ucTemp = USART_ReceiveData(USART2);		
		//cnt->rec_data_isr(ucTemp);
	}	 
	/* �˳��ٽ�� */
	taskEXIT_CRITICAL_FROM_ISR( ulReturn );
}

/*--------------------------------------------------------------------------------*/
//@ͨ�ö�ʱ���жϷ�����
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
//�ⲿ�ж�
void EXTI15_10_IRQHandler(void)
{
  //��ѯ��ʽ�ж��Ƿ�����˶�Ӧ��EXTI Line�ж�
	if(EXTI_GetITStatus(EXTI_Line11) != RESET) 
	{
		if( 0 == GPIO_ReadInputDataBit( GPIOA, GPIO_Pin_11) )//�½���
			//coin_p1->trigger_isr( );
    	//����жϱ�־λ
		EXTI_ClearITPendingBit(EXTI_Line11);     
	} 
	
	if(EXTI_GetITStatus(EXTI_Line12) != RESET) 
	{
		if( 0 == GPIO_ReadInputDataBit( GPIOA, GPIO_Pin_12) )//�½���
			//coin_p2->trigger_isr( );
    	//����жϱ�־λ
		EXTI_ClearITPendingBit(EXTI_Line12);     
	}  

	if(EXTI_GetITStatus(EXTI_Line15) != RESET) 
	{
		if( 0 == GPIO_ReadInputDataBit( GPIOC, GPIO_Pin_15) )//�½���
			//eye_egg->trigger_isr( );
    	//����жϱ�־λ
		EXTI_ClearITPendingBit(EXTI_Line15);     
	}  
	
}








