#include "host.h"
#include "task_manager.h"
#include "fk_soft_typedef.h"
#include "drv_opt.h"
#include "game_data.h"
#include "game_logic.h"



static int host_begin = 0;


gdat_pt gamedat[ PlayerNum ];
void host_master_thread(void)
{
	
	u8 ret = 0;	
	dbg->printf("host_master_thread \r\n");
	/*-------------------------------------------------------------*/
	
	init_gdat_opt();
	//创建游戏环境
	for( u8 i=0;i<PlayerNum;i++ )
	{
		gamedat[i] = gdat_opt->create( );
		gdat_opt->config( i,gamedat[i] );
		dbg->printf("id(%d) nowscore = %d \r\n",i,gamedat[i]->remaining_score );
	}
	
	
	
	
	/*-------------------------------------------------------------*/
/*协程相关*/
	taskENTER_CRITICAL();								//进入临界区
	init_exled_coroutine();
	init_seg_coroutine();			
	init_keyboard_coroutine();					
	taskEXIT_CRITICAL();            		//退出临界区
	/*-------------------------------------------------------------*/
	printf("host_master_thread \t:mem_get_free -> %d B \r\n",mem_get_free() );
	taskENTER_CRITICAL();								//进入临界区
#ifndef DEBUGPTR
	do_create_ui_task();
	do_create_player1_task();
	do_create_player2_task();
	do_create_player3_task();
#endif
	taskEXIT_CRITICAL();            		//退出临界区
	host_begin = 1;
	/*-------------------------------------------------------------*/
	/*-------------------------------------------------------------*/
	
	u32 cnt = 0;
	while(1)
	{
		
#ifndef DEBUGPTR
		listen_doll_move_coroutine();			//监听公仔传送带
		audio_1->deal_req_coroutine();		//监听音乐播放请求
		listen_exled_coroutine();					//监听扩展LED
		listen_all_gun_ready_coroutine();	//监听枪准备状态，发送活跃请求
		listen_coroutine_keyboard();
#endif		
		
		dbg->deal_coroutine();						//调试接口
		
		
		vTaskDelay(20);										//丢弃剩余的时间片资源
		//dbg->printf("host %d \r\n",cnt++);
	}
}

void listten_seg(void)
{
	if(host_begin ==0 )return;
	listen_seg_coroutine();						//监听数码管请求
}



