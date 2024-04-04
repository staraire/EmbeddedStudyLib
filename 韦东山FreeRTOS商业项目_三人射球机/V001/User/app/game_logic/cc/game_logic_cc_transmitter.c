
#include "game_logic.h"
#include "game_data.h"
#include "drv_opt.h"
#include "host.h"
#define PRT_LOG(X...)	//dbg->printf(X)	
/*--------------------------------------------------------------------------------*/
//Mtr TSMT
// P1-> exled Pin5 ->exled 4
// P2-> exled Pin6 ->exled 5
// P3-> exled Pin7 ->exled 6
#define Open_TSMT( ID ) 		{ pull_exled_req(ID,4+ID,ON);} 
#define Close_TSMT( ID )		{	pull_exled_req(ID,4+ID,OFF);}

//Signal TSMT
// P1 ->Pin19 -> ex_inputbus 18
// P2 ->Pin20 -> ex_inputbus 19
// P3 ->eye_input -> 6 

int Cat_TSMT_Signal(int id)
{
	int ret = 0;
	switch( id )
	{
		case _P1_: ret = ex_inputbus->get_bit( 18 );break;
		case _P2_: ret = ex_inputbus->get_bit( 19 );break;
		case _P3_: ret = eye_input->read_eye( 6 );break;
	}
	return ret ;
}

static u8 sta[PlayerNum];
static time_t otick[PlayerNum]={0,0,0};
static time_t ntick[PlayerNum]={0,0,0};

static time_t owtick[PlayerNum]={0,0,0};
static time_t nwtick[PlayerNum]={0,0,0};

static time_t oetick[PlayerNum]={0,0,0};
static time_t netick[PlayerNum]={0,0,0};



static u32 cntr[ PlayerNum ] = {0,0,0};
void deal_tsmt_logic( void* pgdat )
{
	gdat_pt gdat = (gdat_pt)pgdat;
	u8 id = gdat->id;
	
	//访问频率控制
	ntick[ id ] = timtick->get();
	if( 10 >( ntick[id] - otick[id] )  )
		return;
	otick[ id ] = ntick[ id ];
	
	switch( sta[id] )
	{
		case 0://等待玩家投币
			if( gdat->remaining_coins >= 1 )
			{
				//减少币数
				gdat->remaining_coins = gdat->remaining_coins - 1;
				//剩余球数
				cntr[id] = gdat->one_round_bullets;
				
				PRT_LOG("req faqiu id(%d)->%d ball \r\n",id,cntr[id]);
				//开始发球
				sta[id] ++;
				
				PRT_LOG("tsmt_logic(%d) goto -> %d \r\n",id,sta[id] );
				
			}
			break;
		case 1://开启发球电机
			Open_TSMT( id );
			sta[id] ++;
			owtick[id] = timtick->get();
			PRT_LOG("tsmt_logic(%d) goto -> %d \r\n",id,sta[id] );
			
			
			break;
		case 2://等待低电平
			if( 0== Cat_TSMT_Signal(id) )		
			{
				
				cntr[id]--;
				PRT_LOG("tsmt_logic(%d) in -> wait(%d) \r\n",id,cntr[id] );
				owtick[id] = timtick->get();
				if( cntr[id] == 0 )
				{
					Close_TSMT( id );						//关闭电机
					sta[id] = 5;						//继续等待投币
					PRT_LOG("tsmt_logic(%d) goto -> %d \r\n",id,sta[id] );
				}
				else
				{
					
					sta[id] ++;						//开始滤波
					PRT_LOG("tsmt_logic(%d) goto -> %d(%d)  \r\n",id,sta[id],cntr[id] );
					
				}
				break;
			}
			
			//check error
			nwtick[id] = timtick->get();
			if( 10000 <( nwtick[id] - owtick[id] )  )
			{//error
				Close_TSMT( id );						//关闭电机
				PRT_LOG("error: deal_tsmt_logic (id %d)\r\n",id );
				sta[id] = 0xFE;
			}
			
			break;
		case 3://滤波
			nwtick[id] = timtick->get();
			if( 100< ( nwtick[id] - owtick[id]) )
			{
				
				sta[id] ++;							//完成滤波
				PRT_LOG("tsmt_logic(%d) goto -> %d \r\n",id,sta[id] );
			}
			break;
		case 4://等待高电平
			if( 1== Cat_TSMT_Signal(id) )
			{
				sta[id] = 2;							//等待下一个球的信号
				PRT_LOG("tsmt_logic(%d) goto -> %d \r\n",id,sta[id] );
			}
			break;
		case 5://结束发球，滤波
			nwtick[id] = timtick->get();
			if( 1000< ( nwtick[id] - owtick[id]) )
			{
				
				sta[id] = 0 ;							//完成滤波
				PRT_LOG("tsmt_logic(%d) goto -> %d \r\n",id,sta[id] );
			}
			break;
		case 0xFE://错误，等待播音
			netick[id] = timtick->get();
			if( 30000<( netick[id]- oetick[id] )  )
			{
				oetick[id] = netick[id];
				sta[id] = 0xFF;
			}
			break;
		case 0xFF://错误，播音
			oetick[id] = timtick->get();
			audio_1->req_play_file( _em_tranball_error_p1 + id  );
			sta[id] = 0xFE;
			pull_seg_disp_graphical_req( id, _seg_one_coin_ball, _seg_err_1 );

			break;
		
		default:break;
		
	}
		
	
}
