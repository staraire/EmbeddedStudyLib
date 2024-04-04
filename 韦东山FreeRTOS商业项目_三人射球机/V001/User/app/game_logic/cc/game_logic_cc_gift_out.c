
#include "game_logic.h"
#include "game_data.h"
#include "drv_opt.h"
#include "host.h"
#define PRT_LOG(X...)	//dbg->printf(X)	
/*--------------------------------------------------------------------------------*/
static int SecondMotor[PlayerNum]= {0,0,0};//是否使用第二个货道电机


#define store_data_of_cntr(id,val) \
	gdat_opt->data_store.write_gift_cntr( id,val );

//Mtr gift_out
// P1-> iomos MOS0 
// P2-> iomos MOS4
// P3-> iomos MOS1 + MOS2
//根据ID打开电机，arg为是否使用备用电机
static void open_mtr(u8 id,u8 arg)
{
	switch( id )
	{
		case _P1_:iomos->open( 0 );break;
		case _P2_:iomos->open( 4); break;
		case _P3_:
			
			iomos->open( 1 + arg );break;
		default:break;
	}
}
//根据ID关闭电机，arg为是否使用备用电机
static void close_mtr(u8 id,u8 arg)
{
	switch( id )
	{
		case _P1_:iomos->close( 0 );break;
		case _P2_:iomos->close( 4 );break;
		case _P3_:iomos->close( 1 + arg );break;
		default:break;
	}
}


//Signal gifout
// P1 -> eye_input 0
// P2 -> eye_input 1
// P3 -> eye_input 7 + 8(备用)

u8 get_gift_signal(u8 id, u8 arg)
{
	u8 ret = 0;
	switch( id )
	{
		case _P1_:ret = eye_input->read_eye( 0 );break;
		case _P2_:ret = eye_input->read_eye( 1 );break;
		case _P3_:
			if( SecondMotor[id] ==0 )
			{
				ret = eye_input->read_eye( 8 );
			}
			else
			{
				ret = eye_input->read_eye( 7 );
			}
			break;
		default:
			break;
	}
	return ret;
}

static time_t otick[PlayerNum]={0,0,0};
static time_t ntick[PlayerNum]={0,0,0};

static time_t owtick[PlayerNum]={0,0,0};
static time_t nwtick[PlayerNum]={0,0,0};

static time_t oetick[PlayerNum]={0,0,0};
static time_t netick[PlayerNum]={0,0,0};

static time_t sta[PlayerNum];
static u32 cntr[ PlayerNum ] = {0,0,0};
void deal_gift_out_logic( void* pgdat )
{
	gdat_pt gdat = (gdat_pt)pgdat;
	u8 id = gdat->id;
	
	//访问频率控制
#if 0
	ntick[ id ] = timtick->get();
	if( 5 >( ntick[id] - otick[id] )  )
		return;
	otick[ id ] = ntick[ id ];
#endif
	
	switch( sta[id] )
	{
		case 0://等待分数满足条件
			if( gdat->remaining_score >= gdat->one_gift_score  )
			{
				PRT_LOG("deal_gift_out_logic: \r\n");
				PRT_LOG("gdat->remaining_score:%d \r\n",gdat->remaining_score);
				PRT_LOG("gdat->one_gift_score:%d \r\n",gdat->one_gift_score);
				
				
				sta[id] ++;
			}
			break;
		case 1://开启电机
			gdat->gift_cntr++;
			//只有P3有第二个出货电机，要对P3进行特殊处理
			if( id == _P3_ )
			{
				//(总次数 / 循环计数最大值) %2，得出奇偶值
				//0，偶数，默认电机，
				//1，奇数，第二电机
				
				
				SecondMotor[ id ] = 
					(gdat->gift_cntr / (P3_GIFT_MTRCYC+1) )%2;
				
			}
			
			open_mtr(id,SecondMotor[id]);
			owtick[ id ] = timtick->get();
			sta[id] ++;
			break;
		case 2://等待反馈信号
			if( 0 == get_gift_signal(id ,SecondMotor[id]) )
			{
				sta[id] ++;
				owtick[ id ] = timtick->get();
				break;
			}
			nwtick[id] = timtick->get();
			if( 30000 < ( nwtick[id] - owtick[id] ) )
			{
				//error:关闭电机，进入错误状态
				close_mtr(id,SecondMotor[id]);
				
				
				PRT_LOG("error: id(%d) gift out timeout \r\n",id);
				sta[id] = 0XFF;
				
			}
			
			break;
		case 3://明确信号并处理
			nwtick[id] = timtick->get();
			//if( 2 > ( nwtick[id] - owtick[id] ) )break;
			if( 0==get_gift_signal(id,SecondMotor[id]) )
			{
				//TODO:music
				sta[id] ++;
				audio_1->req_play_file( _em_gift_out  );
				owtick[ id ] = timtick->get();
				break;
			}
			
			
			break;
		case 4://等松开
			nwtick[id] = timtick->get();
			if( 200 > ( nwtick[id] - owtick[id] ) )break;
			if( 1==get_gift_signal(id,SecondMotor[id]) )
			{
				close_mtr(id,SecondMotor[id]);//close mtr
				sta[id] ++;
				owtick[ id ] = timtick->get();
				//存储出礼品的总次数
				
				gdat->gift_cntr = gdat->gift_cntr%1000;
				store_data_of_cntr( id, gdat->gift_cntr );
				
			}
			break;
		
		case 5://间隔
			nwtick[id] = timtick->get();
			if( 1000 > ( nwtick[id] - owtick[id] ) )break;
			
			gdat->remaining_score = gdat->remaining_score - gdat->one_gift_score;
			//请求数码管显示得分
			pull_seg_disp_num_req( id,_seg_score,gdat->remaining_score );
			
			sta[id] = 0;
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
			audio_1->req_play_file( _em_gift_error_p1 + id  );
			sta[id] = 0xFE;
			pull_seg_disp_graphical_req( id, _seg_one_coin_ball, _seg_err_2 );
			break;
		default:break;
		
	}		
		
	
}
