//人物公仔倒下检测
//检测到人物倒下，则加分
#include "game_logic.h"
#include "game_data.h"
#include "drv_opt.h"
#include "host.h"
#define PRT_LOG(X...)	//dbg->printf(X)	
/*--------------------------------------------------------------------------------*/
//ball_recycle_eye
//P1
//	--> Pin7  ->bit6
//	--> Pin8  ->bit7
//	--> Pin9  ->bit8
//P2
//	--> Pin10 ->bit9
//	--> Pin11 ->bit10
//	--> Pin12 ->bit11
//P3
//	--> Pin13 ->bit12
//	--> Pin14 ->bit13
//	--> Pin15 ->bit14



#define Cat_Ball_Recycle_Signal( ID,eyenum ) ex_inputbus->get_bit( 6 + ID*3 + eyenum )



void deal_ball_recycle_socre(  void* pgdat,uint8_t line )
{
	gdat_pt gdat = (gdat_pt)pgdat;
	u8 id = gdat->id;
	static uint8_t sta[PlayerNum][ 3 ] = {0,0,0};
	static uint8_t owtick[PlayerNum][3]= {0,0,0};
	static uint8_t nwtick[PlayerNum][3]= {0,0,0};
	
	switch( sta[id][line] )
	{
		case 0://等待下降沿
			if( 0 == Cat_Ball_Recycle_Signal(id,line) )
			{
				sta[id][line] = 1;
				owtick[id][line] = timtick->get();
			}
			break;
		case 1://等到信号或者超时
			nwtick[id][line] = timtick->get();
			if( 1000 < ( nwtick[id][line] - owtick[id][line] ) ) 
			{
				sta[id][line] ++;
				
				break;
			}
			
			if( 1 == Cat_Ball_Recycle_Signal(id,line) )
			{
				sta[id][line] ++;
				break;
			}
//			else
//			{
//				sta[id][line] --;
//			}
			break;
		case 2://处理
			
			PRT_LOG("recycle:id(%d)->%d r_score:%d \r\n",id,line,gdat->remaining_score);
			gdat->remaining_score = gdat->remaining_score + line + 1;
			gdat->remaining_score = gdat->remaining_score% 999;
			//请求数码管显示得分
			pull_seg_disp_num_req( id,_seg_score,gdat->remaining_score );
			audio_1->req_play_file( _em_recovery1_score + line );
			sta[id][line] = 0;
			owtick[id][line] = timtick->get();
			break;
		case 3:
			nwtick[id][line] = timtick->get();
			if( 3000 > ( nwtick[id][line] - owtick[id][line] ) ) break;
				sta[id][line] = 0;
		default:
			break;
	}
}

//球回收得分逻辑
void deal_ball_recycle_score_logic( void* pgdat )
{
	deal_ball_recycle_socre(  pgdat,0  );
	deal_ball_recycle_socre(  pgdat,1  );
	deal_ball_recycle_socre(  pgdat,2  );
	
}
#if 0

static u8 sta[PlayerNum];
static time_t otick[PlayerNum]={0,0,0};
static time_t ntick[PlayerNum]={0,0,0};

static time_t owtick[PlayerNum]={0,0,0};
static time_t nwtick[PlayerNum]={0,0,0};
static u8 esta[PlayerNum][3];//描述所以玩家自己3个光眼的状态
void deal_ball_recycle_score_logic( void* pgdat )
{
	gdat_pt gdat = (gdat_pt)pgdat;
	u8 id = gdat->id;
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
			esta[id][0] = Cat_Ball_Recycle_Signal(id,0);
			esta[id][1] = Cat_Ball_Recycle_Signal(id,1);
			esta[id][2] = Cat_Ball_Recycle_Signal(id,2);
		
			if( (esta[id][0]==0)||(esta[id][1]==0)||(esta[id][2]==0))
			{
				sta[id] ++;
				owtick[id] = timtick->get();
			}				
			break;
		case 1://滤波
			//滤波
			nwtick[id] = timtick->get();
			if( 200 > ( nwtick[id] - owtick[id] ) ) break;
			//TODO:add score,play music
			for( int i=0;i<3;i++ )
				if( 0 == esta[id][i] ) 
				{
					PRT_LOG("recycle:id(%d)->%d r_score:%d \r\n",id,i,gdat->remaining_score);
					gdat->remaining_score = gdat->remaining_score + i + 1;
					gdat->remaining_score = gdat->remaining_score% 999;
					//请求数码管显示得分
					pull_seg_disp_num_req( id,_seg_score,gdat->remaining_score );
					audio_1->req_play_file( _em_recovery1_score + i );
				}
				
			sta[id] = 2;
			break;
		case 2:
			esta[id][0] = Cat_Ball_Recycle_Signal(id,0);
			esta[id][1] = Cat_Ball_Recycle_Signal(id,1);
			esta[id][2] = Cat_Ball_Recycle_Signal(id,2);
			if( (esta[id][0]==1)&&(esta[id][1]==1)&&(esta[id][2]==1))
			{
				sta[id] = 0;
			}

			break;
		default:
			sta[id] = 0;
			break;
	}
	
}
#endif

