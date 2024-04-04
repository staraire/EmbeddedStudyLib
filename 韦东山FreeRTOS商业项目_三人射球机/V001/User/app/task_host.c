
#include "task_manager.h"
#include "fk_soft_typedef.h"
#include "drv_opt.h"
#include "game_logic.h"
#include "game_data.h"
#include "host.h"
static TaskHandle_t task_host_Handle = NULL;/* 创建监听句柄 */
/*--------------------------------------------------------------------------------*/
static void task_host(void)
{ 
	host_master_thread();	
} 
/*--------------------------------------------------------------------------------*/

void do_create_host_task(void)
{	
	BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
	/* 创建AppTaskCreate任务 */
	xReturn = thread_create(task_host, "task_host", 2048, NULL, 2, &task_host_Handle);
	if(pdPASS == xReturn) 	
	{
		printf("do_create_host_task success! \r\n");
	}
	else 					
	{
		printf("Error:do_create_host_task! \r\n");
	}
	printf("do_create_host_task \t:mem_get_free -> %d B \r\n",mem_get_free() );
}
/*--------------------------------------------------------------------------------*/
TaskHandle_t* get_task_host_Handle(void)
{
	return &task_host_Handle;
}
/*--------------------------------------------------------------------------------*/


