
#include "task_manager.h"
#include "fk_soft_typedef.h"
#include "drv_opt.h"
#include "host.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "ui_manager.h"

void printf_ex_inputbus(void);

static TaskHandle_t task_ui_Handle = NULL;/* 创建监听句柄 */

static void task_ui(void)
{
	while(1)
	{
		PageV( _p_idle )->run( NULL,PNULL );
		vTaskDelay(50);
	}
}

void do_create_ui_task(void)
{	
	BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
	/* 创建AppTaskCreate任务 */
	xReturn = thread_create(task_ui, "task_ui", 1024, NULL, 2, &task_ui_Handle);
	if(pdPASS == xReturn) 	
	{
		printf("do_create_ui_task success! \r\n");
	}
	else 					
	{
		printf("Error:do_create_ui_task! \r\n");
	}
	printf("do_create_ui_task \t:mem_get_free -> %d B \r\n",mem_get_free() );
}

TaskHandle_t* get_task_ui_Handle(void)
{
	return &task_ui_Handle;
}


