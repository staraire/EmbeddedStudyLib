
#include "task_manager.h"
#include "fk_soft_typedef.h"
#include "drv_opt.h"
#include "host.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "ui_manager.h"

void printf_ex_inputbus(void);

static TaskHandle_t task_listen_Handle = NULL;/* 创建监听句柄 */

extern void listten_seg(void);
static void task_listen(void)
{
	static time_t log_oldtick=0;
	static time_t log_nowtick=0;
	
	//Console.Init();
	//Console.Bootloader();
	while(1)
	{
#ifdef DEBUGPTR
		printf_ex_inputbus();
#endif
		ex_inputbus->refresh();
		vTaskDelay(3);
		listten_seg();	
		
	}
}

void do_create_listen_task(void)
{	
	BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
	/* 创建AppTaskCreate任务 */
	xReturn = thread_create(task_listen, "task_listen", 512, NULL, 3, &task_listen_Handle);
	if(pdPASS == xReturn) 	
	{
		printf("do_create_listen_task success! \r\n");
	}
	else 					
	{
		printf("Error:do_create_listen_task! \r\n");
	}
	printf("do_create_listen_task \t:mem_get_free -> %d B \r\n",mem_get_free() );
}

TaskHandle_t* get_task_listen_Handle(void)
{
	return &task_listen_Handle;
}
static time_t oldtick = 0;
static time_t nowtick = 0;
void printf_ex_inputbus(void)
{
	
	nowtick = timtick->get();
	if( 100 <( nowtick - oldtick ) )
	{
		
		dbg->printf(" input -> " );
#if 1
		for(int i=0;i<9;i++)
		{
			dbg->printf(" %d ",eye_input->read_eye(i) );
		}
#else
		for(int i=0;i<24;i++)
		{
			dbg->printf(" %d ",ex_inputbus->get_bit(i) );
		}
#endif
			
		dbg->printf(" \r\n ");
		oldtick = nowtick;
	}

}





