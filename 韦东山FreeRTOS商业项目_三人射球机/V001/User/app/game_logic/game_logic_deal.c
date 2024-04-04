#include "game_logic.h"
#include "game_data.h"
#include "drv_opt.h"

extern void game_logic_deal_standby(gdat_pt gdat);
extern void game_logic_deal_ready(gdat_pt gdat);
extern void game_logic_deal_getres(gdat_pt gdat);
extern void game_logic_deal_playing(gdat_pt gdat);
extern void game_logic_deal_gameover(gdat_pt gdat);

extern void deal_tsmt_logic( void* pgdat );
extern void deal_gun_logic( void* pgdat );


//暂停游戏
static int suspend = 0;
void suspend_game_logic(void)
{
	suspend = 1;
}

/*
@游戏逻辑处理入口
@输入参数:
->gdat 游戏工作数据

*/

void game_logic_deal(void* pgdat)
{
	
	gdat_pt gdat = (gdat_pt)pgdat;
	u8 mSta = gdat->mainstate;
	
	//如果处于后台管理，则暂停游戏逻辑
	if( suspend == 1 ) return;
	
	
	//处理投币器信号
	deal_coin_signal(gdat);
	//处理发球器
	deal_tsmt_logic(gdat);
	//处理枪管逻辑
	deal_gun_logic(gdat);
	//人物得分逻辑
	deal_doll_score_logic(gdat);
	//回收得分逻辑
 	deal_ball_recycle_score_logic(gdat);
	//礼品机逻辑
	deal_gift_out_logic(gdat);
	//按键灯逻辑
	deal_key_logic( gdat );
				
}



