

#include <stdio.h>
#include "ui_manager.h"
#include "game_data.h"
#include "drv_opt.h"
#include "game_logic.h"
#include "host.h"
#define PRT_LOG(X...)	dbg->printf(X)	


static void run(u8 dat,u8 *ch);						//页面调用(包含显示调用)
static void disp(u8 mouse);							//显示调用(提供给其他文件显示页面)
static u8 	get_input_event(void);					//获取输入事件


page_action_t idle_page = {
	.run = run,
	.disp = disp,
	.get_input_event = get_input_event,
	.mouse = 0,
};
/*--------------------------------------------------------------------------------*/
//页面调用(包含显示调用)
static void run(u8 dat,u8 *ch)
{
	//u8 id = _P2_;
	//dbg->printf("mtr_curr->read(%d) = %d \r\n",id,mtr_curr->read(id) );
	
	u8 ipEvent = get_input_event();
	if( ipEvent == _key_none ) return ;
	
	
	audio_1->req_play_file( _em_key );		//请求播放按键声音	
	//等待按键事件，如果是菜单键，则进行页面深度调整
	if(  _key_menu == ipEvent  ) 
	{
		//通知所有玩家停止游戏
		suspend_game_logic();
		//进入深度页面(选择玩家id) (注意，页面是阻塞的哦)
		PRT_LOG("_p_selt_id->run() \r\n");
		PageV( _p_selt_args )->run( NULL,PNULL );
		//复位系统
		
	}
	else if(  _key_back == ipEvent  ) 
	{
		//请求清除错误 (即使没有错误，也发出请求)
		//通知所有玩家清除错误
		
		PRT_LOG("req clr_err \r\n");
	}
}
//显示调用(提供给其他文件显示页面)
/*--------------------------------------------------------------------------------*/
static void disp(uint8_t mouse)
{
	//本页面不用显示
}
/*--------------------------------------------------------------------------------*/
//获取输入事件
static uint8_t 	get_input_event(void)
{
	
	return get_keyboard_event();
	
	
}


