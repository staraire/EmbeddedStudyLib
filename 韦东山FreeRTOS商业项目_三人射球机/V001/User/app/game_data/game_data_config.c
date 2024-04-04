#include "game_data.h"
#include "game_logic.h"
#include "drv_opt.h"
#include <stddef.h>
#define PRT_LOG(X...)	//dbg->printf(X)	
#define BootCode 0X5B

//5个球1个币，
void read_board_config(u8 id,void* pdat)
{
	gdat_pt pgdat = (gdat_pt)pdat;
	iic_24c0x->read( SlaveADDR,sizeof( gdat_t )*id,(u8*)pgdat,sizeof( gdat_t ) );
	if( pgdat->bootcode != BootCode  )
	{
		PRT_LOG("player( %d ) is first boot!\r\n",id);
		pgdat->bootcode = BootCode;
		pgdat->id = id;
		pgdat->light_tick = 0;
		pgdat->mainstate = 0;
		pgdat->substate = 0;
		pgdat->remaining_coins = 0;
		pgdat->one_round_coins = 1;					//几币一局
		pgdat->remaining_times = 0;	
		pgdat->one_round_times = 60;				//默认一局时长
		pgdat->one_round_bullets = 5;				//默认一局5个子弹
		
		//一个礼品需要多少分
		//默认礼品分值 30 20 25
		switch( id )
		{
			case _P1_:pgdat->one_gift_score  = 30;	break;
			case _P2_:pgdat->one_gift_score  = 20;	break;
			case _P3_:pgdat->one_gift_score  = 45;	break;
			
		}
		pgdat->remaining_score = 0;					//默认得分0分
		pgdat->gift_cntr = 0;								//默认出了0个礼品
		//将配置写进去
		iic_24c0x->write( SlaveADDR,sizeof( gdat_t )*id,(u8*)pgdat,sizeof( gdat_t ) );
		return;
	}
	pgdat->light_tick = 0;
	pgdat->mainstate = 0;
	pgdat->substate = 0;
	pgdat->remaining_coins = 0;
	
	pgdat->remaining_times = 0;	
	
	
	
	pgdat->remaining_score = 0;					//默认得分0分
	
	PRT_LOG("player( %d ) is not first boot!\r\n",id);
}	

void write_game_data_player_data(uint8_t id,void* pdat)
{
		gdat_pt pgdat = (gdat_pt)pdat;

		iic_24c0x->write( SlaveADDR,sizeof( gdat_t )*id,(uint8_t*)pgdat,sizeof( gdat_t ) );

}

//这个数据经常需要存储，为了节省资源，单独创建一个函数
void wirte_game_data_gift_cntr( u8 id,u32 cntr )
{
	u32 offset = offsetof( gdat_t,gift_cntr );
	PRT_LOG("wirte_gift_cntr:%d \r\n",offset);
	//将gift_cntr写进去
	iic_24c0x->write( 
		SlaveADDR,
		sizeof( gdat_t )*id+offset,
		(u8*)cntr,sizeof( u32 ) 
		);
}


