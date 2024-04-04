
#include "task_manager.h"
#include "fk_soft_typedef.h"
#include "drv_opt.h"
#include "game_data.h"
static TaskHandle_t task_start_Handle = NULL;/* 创建任务句柄 */

extern void init_drv_hc595_p1(void);
extern void init_drv_hc595_p2(void);
extern void init_drv_hc595_h1(void);


static void task_start(void)
{
	
	/*-------------------------------------------------*/
	//创建驱动对象*/	
	taskENTER_CRITICAL();           			//进入临界区	
	init_drv_timer_tick();								//定时器滴答初始化
	init_drv_debug_usart_after_os_run();	//调试输出
	
	//存储器
	init_drv_24c0x();											
	//独立输入信号				(5P座子->P6)+(6P座子->P12)+(P8-Pin2 光眼信号)
	init_drv_eye_input();
	//数码管接口					(5P座子->P7)
	init_drv_seg_bus();
	
	//扩展输入总线				(30P牛角座->CON2)
	init_drv_ex_inputbus();								
	//扩展输出总线				(30P牛角座->CON1)
	init_drv_ex_led();
	//独立MOS管接口				(P14-MOS管)+(P8-Pin3 mos管信号)
	init_drv_io_mos();
	
	
	
	//创建软件对象
	taskEXIT_CRITICAL();            			//退出临界区
/*-------------------------------------------------*/

	vTaskDelay(2000);
	taskENTER_CRITICAL();           					//进入临界区
	
	//背景音-音频接口			
	init_drv_audio_1st();					
	//前景音
	init_drv_audio_2nd();
	
	taskEXIT_CRITICAL();            			//退出临界区
/*-------------------------------------------------*/
	//创建任务
	taskENTER_CRITICAL();           					//进入临界区
	do_create_host_task();										//创建主机任务
	do_create_listen_task(); 									//创建监听任务
	
	vTaskDelete(task_start_Handle); 					//删除AppTaskCreate任务
	taskEXIT_CRITICAL();            					//退出临界区
	
}

void do_create_start_task(void)
{	
	BaseType_t xReturn = pdPASS;						/* 定义一个创建信息返回值，默认为pdPASS */
	/* 创建AppTaskCreate任务 */
	xReturn = thread_create(task_start, "task_start", 1024, NULL, 1, &task_start_Handle);        
	if(pdPASS == xReturn)	vTaskStartScheduler();  	/* 启动任务，开启调度 */
	 
}



