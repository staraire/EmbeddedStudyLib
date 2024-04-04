//人物公仔倒下检测
//检测到人物倒下，则加分
#include "game_logic.h"
#include "game_data.h"
#include "drv_opt.h"
#include "host.h"
#define PRT_LOG(X...)	//dbg->printf(X)	
/*--------------------------------------------------------------------------------*/
//doll_eye
//P1--> Pin16 ->bit15
//P2--> Pin17 ->bit16
//P3--> Pin18 ->bit17

#define Cat_Doll_Ready_Signal( ID ) ex_inputbus->get_bit( 15 + ID  )

static u8 sta[PlayerNum];
static time_t otick[PlayerNum]={0,0,0};
static time_t ntick[PlayerNum]={0,0,0};

static time_t owtick[PlayerNum]={0,0,0};
static time_t nwtick[PlayerNum]={0,0,0};

//公仔得分逻辑
void deal_doll_score_logic( void* pgdat )
{
	gdat_pt gdat = (gdat_pt)pgdat;
	u8 id = gdat->id;
	
	//定时刷新数码管
//	cycle_refresh( id,_seg_score,gdat->remaining_score,5000);
	
#if 0	
	//访问频率控制
	ntick[ id ] = timtick->get();
	if( 2 >( ntick[id] - otick[id] )  )
		return;
	otick[ id ] = ntick[ id ];
#endif	
	switch( sta[id] )
	{
		case 0://等待下降沿
			if( 0== Cat_Doll_Ready_Signal(id) )
			{
				sta[id]++;
				owtick[id] = timtick->get();
			}
			break;
		case 1://滤波
			//滤波
			nwtick[id] = timtick->get();
			if( 20 > ( nwtick[id] - owtick[id] ) ) break;
			owtick[id] =	nwtick[id];
			//判断是否依然是触发了
			if( 0== Cat_Doll_Ready_Signal(id) )
			{
				sta[id]++;
				owtick[id] = timtick->get();
			}
			else
			{
				sta[id] = 0;
			}
			break;
		
		case 2://等待上升沿
			
			//等上升沿
			if( 1== Cat_Doll_Ready_Signal(id) )
			{
				//TODO:(1)add score (2)music
				gdat->remaining_score = gdat->remaining_score + 5;//一个公仔得5分
				PRT_LOG("DOLL:remaining_score:%d \r\n",gdat->remaining_score);
				//请求数码管显示得分
				pull_seg_disp_num_req( id,_seg_score,gdat->remaining_score );
				audio_1->req_play_file(_em_target_score);
				sta[id] = 3;	
				owtick[id] = timtick->get();
			}
			break;
		case 3://滤波
			//滤波
			nwtick[id] = timtick->get();
			if( 1000 > ( nwtick[id] - owtick[id] ) ) break;
			owtick[id] =	nwtick[id];
			sta[id] = 0;	
		
			break;
		default:break;
	}
	
}
