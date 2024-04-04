#ifndef __game_logic_h__
#define __game_logic_h__
#include "fk_soft_typedef.h"
#include "game_music.h"
#define PlayerNum 3
//主状态枚举
enum _main_state{
	_e_m_standy = 0,	/*待机*/
	_e_m_ready,			/*就绪*/
	_e_m_getres,		/*获取资源*/
	_e_m_playing,		/*游戏中*/
	_e_m_gameover,		/*游戏结束*/
	_e_m_error,			/*错误状态*/
	
};





extern void game_logic_deal(void* pgdat);			//游戏逻辑处理


extern void suspend_game_logic(void);				//暂停游戏

/*--------------------------------------------------------------------------------*/
extern void deal_coin_signal( void* pgdat );

/*--------------------------------------------------------------------------------*/
extern void deal_doll_score_logic( void* pgdat );
/*--------------------------------------------------------------------------------*/


extern void deal_ball_recycle_score_logic( void* pgdat );
/*--------------------------------------------------------------------------------*/
#define P3_GIFT_MTRCYC	11	//P3电机轮询计数
extern void deal_gift_out_logic( void* pgdat );


extern void deal_key_logic( void* pgdat );

#endif

