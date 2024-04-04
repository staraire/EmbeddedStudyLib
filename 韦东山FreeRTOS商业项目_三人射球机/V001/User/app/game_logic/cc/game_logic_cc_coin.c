
#include "game_logic.h"
#include "game_data.h"
#include "drv_opt.h"
#include "host.h"

#define PRT_LOG(X...)	//dbg->printf(X)	

/*--------------------------------------------------------------------------------*/
static u8 get_coin_signal( u8 id )
{
	u8 ret = 0;
	if( id> PlayerNum )
	{
		PRT_LOG( "error:get_coin_signal \r\n ");
		return 1;
	}
	//P1 val = 0 ->eye3
	//P2 val = 1 ->eye4
	//P3 val = 2 ->eye5
//	ret = eye_input->read_eye( 3 + id );
	switch(id)
	{
		case _P1_:ret = eye_input->read_eye( 3 );break;
		case _P2_:ret = eye_input->read_eye( 4 );break;
		case _P3_:ret = eye_input->read_eye( 5 );break;
		default:PRT_LOG("error:get_coin_signal \r\n");
	}
	
	
	return ret;
}

static u8 sta[PlayerNum];
static time_t otick[PlayerNum]={0,0,0};
static time_t ntick[PlayerNum]={0,0,0};
void deal_coin_signal( void* pgdat )
{


	gdat_pt gdat = (gdat_pt)pgdat;
	u8 id = gdat->id;
	
	//访问频率控制
//	ntick[ id ] = timtick->get();
//	if( 2 >( ntick[id] - otick[id] )  )
//		return;
//	otick[ id ] = ntick[ id ];

	switch( sta[id] )
	{
		case 0://等待低电平信号
			if( 0 == get_coin_signal( id ) )
			{
				sta[id]++;
			}
			break;
		case 1://明确信号
			if( 0 == get_coin_signal( id ) )
			{
				sta[id]++;
			}
			else
			{
				sta[id]--;
			}
			break;
		case 2://等待高电平
			if( 1 == get_coin_signal( id ) )
			{
				sta[id]++;
			}
			break;
		case 3://具体操作
			
			sta[id] = 0;						//state cycle
			
			PRT_LOG( "coin [%d] is trigger \r\n",id );
			gdat->remaining_coins  ++ ;
			
			audio_1->req_play_file( _em_coin );
			req_doll_move_active();	//请求传送带活跃
			
			break;
		
		default:break;
		
	}
		
	
}
