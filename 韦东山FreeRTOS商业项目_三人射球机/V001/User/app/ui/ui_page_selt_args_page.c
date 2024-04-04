

#include <stdio.h>
#include "ui_manager.h"
#include "game_data.h"
#include "drv_opt.h"
#include "game_logic.h"
#include "host.h"
#define PRT_LOG(X...)	dbg->printf(X)	

//游戏数据
extern gdat_pt gamedat[ ];

uint8_t mouse_to_board( uint8_t mouse );
void mouse_move( page_action_pt this_page );
void add_args( page_action_pt this_page );
void reduce_args( page_action_pt this_page );


static void run(u8 dat,u8 *ch);						//页面调用(包含显示调用)
static void disp(u8 mouse);							//显示调用(提供给其他文件显示页面)
static u8 	get_input_event(void);					//获取输入事件


page_action_t selt_args_page = {
	.run = run,
	.disp = disp,
	.get_input_event = get_input_event,
	.mouse = 0,
};

static page_action_pt this_page;

static time_t otick = 0;
static time_t ntick = 0;

static void screen_tick( time_t tick )
{
	ntick = timtick->get();
	if( tick >( ntick - otick ) ) return;
	otick = ntick;
	disp( this_page->mouse );
	
}

/*--------------------------------------------------------------------------------*/
//页面调用(包含显示调用)
static void run(u8 dat,u8 *ch)
{
	
	u8 ipEvent = 0;
	this_page = &selt_args_page;
	
	suspend_seg_coroutine();	//挂起数码管
	
	PRT_LOG("selt_args_page->run() \r\n ");
	while(1)
	{
		screen_tick( 200 );
		
		
		ipEvent = get_input_event();
		if( ipEvent == _key_none ) continue;
		audio_1->req_play_file( _em_key );		//请求播放按键声音
		
		switch( ipEvent )
		{
			case _key_up:	//增加参数数值
				add_args( this_page );
				break;
			
			case _key_dn: //减小参数数值
				reduce_args( this_page );
				break;
			
			case _key_menu://切换到下一个参数
				mouse_move( this_page );
				
				break;
				
			case _key_back://
				goto __exit_page;
				
			default:
				break;
		}
		//存储数据
		gdat_opt->data_store.write_player_data( _P1_,gamedat[_P1_] );
		gdat_opt->data_store.write_player_data( _P2_,gamedat[_P2_] );
		gdat_opt->data_store.write_player_data( _P3_,gamedat[_P3_] );
		
		disp( this_page->mouse ); 

	}
__exit_page:
	//存储数据
	//每次进入后台，默认清空礼品计数器
	gamedat[_P1_]->gift_cntr =0;
	gamedat[_P2_]->gift_cntr =0;
	gamedat[_P3_]->gift_cntr =0;
	
	gdat_opt->data_store.write_player_data( _P1_,gamedat[_P1_] );
	gdat_opt->data_store.write_player_data( _P2_,gamedat[_P2_] );
	gdat_opt->data_store.write_player_data( _P3_,gamedat[_P3_] );
	
	__set_FAULTMASK(1);
	NVIC_SystemReset();
	return;
}
//显示调用(提供给其他文件显示页面)
/*--------------------------------------------------------------------------------*/
static void disp(uint8_t mouse)
{
	//一个玩家3个板子 mouse表示板子号
	static uint8_t color = 0;
	uint8_t i = 0;
	uint8_t id = 0 ;		
	//颠倒显示颜色
	if( color == 0 ) color = 1; else color = 0;
	
	if( color )
	{
		//直接刷新数码管显示 
		for( id = 0; id< PlayerNum; id++ )
		{
			ex_seg_bus->set_val( id*3+0,gamedat[id]->one_round_bullets );
			ex_seg_bus->set_val( id*3+1,gamedat[id]->one_gift_score );
			ex_seg_bus->set_graph( id*3+2,_seg_off );
		}
		ex_seg_bus->display();
	}
	else
	{
		//关闭某个数码管板子的显示
		uint8_t board = 0;
		ex_seg_bus->set_graph( mouse_to_board( mouse ),_seg_off );
		ex_seg_bus->display();
	}
	
}
/*--------------------------------------------------------------------------------*/
//获取输入事件
static uint8_t 	get_input_event(void)
{
	
	return get_keyboard_event();
}

uint8_t mouse_to_board( uint8_t mouse )
{
	uint8_t ret = 0;
		switch( mouse )
		{
			case 0: 
			case 1: 
				ret = mouse;
				break;
			case 2: 
			case 3: 
				ret = mouse + 1;
				break;
			
			case 4: 
			case 5: 
				ret = mouse + 2;
				break;
			
			default:break;
		}
	return ret;
}



void mouse_move( page_action_pt this_page )
{
	if( this_page->mouse < (6-1) )
		this_page->mouse ++;
	else
		this_page->mouse = 0;
}

//
void add_args( page_action_pt this_page )
{
	uint8_t id = this_page->mouse / 2;
	if(  0 == (this_page->mouse % 2) )
	{//说明是一币几球
		if (gamedat[id]->one_round_bullets < 99)
			gamedat[id]->one_round_bullets++;
		else
			gamedat[id]->one_round_bullets = 1;
	}
	else
	{//说明是礼品分值
		if (gamedat[id]->one_gift_score < 99)
			gamedat[id]->one_gift_score++;
		else
			gamedat[id]->one_gift_score = 1;
	}
}

void reduce_args( page_action_pt this_page )
{
	uint8_t id = this_page->mouse / 2;
	if(  0 == (this_page->mouse % 2) )
	{//说明是一币几球
		if (gamedat[id]->one_round_bullets > 1)
			gamedat[id]->one_round_bullets--;
		else
			gamedat[id]->one_round_bullets = 99;
	}
	else
	{//说明是礼品分值
		if (gamedat[id]->one_gift_score > 1)
			gamedat[id]->one_gift_score--;
		else
			gamedat[id]->one_gift_score = 99;
	}
}
