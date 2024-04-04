
#include "task_manager.h"
#include "fk_soft_typedef.h"
#include "drv_opt.h"
#include "game_logic.h"
#include "game_data.h"
#include "host.h"
static TaskHandle_t task_player1_Handle = NULL;/* 创建监听句柄 */
extern gdat_pt gamedat[ ];

static void task_player1(void)
{
	//创建游戏环境
	u8 id = _P1_;
	gdat_pt pgdat = gamedat[id];
	//请求数码管显示一币几球
	pull_seg_disp_num_req( id,_seg_one_coin_ball,pgdat->one_round_bullets );
	
	//请求数码管显示多少分一个礼品
	pull_seg_disp_num_req( id,_seg_one_gift_score,pgdat->one_gift_score );
	//请求数码管显示剩余多少分
	pull_seg_disp_num_req( id,_seg_score,pgdat->remaining_score );
	
	
	while( 1 )
	{
		
		//处理游戏逻辑
		game_logic_deal(pgdat);
		//切换调度
		vTaskDelay(1);
		
	}
}

void do_create_player1_task(void)
{	
	BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
	/* 创建AppTaskCreate任务 */
	xReturn = thread_create(task_player1, "task_player1", 1024, NULL, 2, &task_player1_Handle);
	if(pdPASS == xReturn)	
	{
		printf("do_create_player1_task success! \r\n");
	}
	else 					
	{
		printf("Error:do_create_player1_task! \r\n");
	}
	printf("do_create_player1_task \t:mem_get_free -> %d B \r\n",mem_get_free() );
}

TaskHandle_t* get_task_player1_Handle(void)
{
	return &task_player1_Handle;
}


