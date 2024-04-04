
#include "game_logic.h"
#include "game_data.h"
#include "drv_opt.h"
#include "host.h"
#define PRT_LOG(X...)	//dbg->printf(X)	
/*--------------------------------------------------------------------------------*/
//Mtr Gun
//P1--> Pin8 ->bit7
//P2--> Pin9 ->bit8
//P3--> Pin10->bit89
#define Open_Gun( ID ) 		{ pull_exled_req(ID,7+ID,ON);  } 
//#define Open_Gun( ID ) 		{ iomos->open( 0+ID );  } 

#define Close_Gun( ID )		{	pull_exled_req(ID,7+ID,OFF); }
//#define Close_Gun( ID )		{ iomos->close( 0+ID );  }
//Gun Ready Signal 
// P1-> Pin4 ->ex_inputbus Pin3 
// P2-> Pin5 ->ex_inputbus Pin4
// P3-> Pin6 ->ex_inputbus Pin5
#define Cat_Gun_Ready_Signal( ID ) ex_inputbus->get_bit( 3 + ID  )

//Gun Open Signal
//P1-> Pin1 ->ex_inputbus Pin0 
//P2-> Pin2 ->ex_inputbus Pin1 
//P3-> Pin3 ->ex_inputbus Pin2 
#define Cat_Gun_Open_Signal( ID ) ex_inputbus->get_bit( 0 + ID  )


static u8 sta[PlayerNum];
static time_t otick[PlayerNum]={0,0,0};
static time_t ntick[PlayerNum]={0,0,0};

static time_t owtick[PlayerNum]={0,0,0};
static time_t nwtick[PlayerNum]={0,0,0};

void deal_gun_logic( void* pgdat )
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
		case 0://等待就绪，并且按下
			//如果没就绪，直接退出
			if( 1 == Cat_Gun_Ready_Signal(id) ) break;
			//如果没按下开枪键，直接退出
			if( 1 == Cat_Gun_Open_Signal(id) ) break;
			sta[id]++;
			owtick[id] = timtick->get();
		
			break;
		case 1:
			nwtick[id] = timtick->get();
			if( 20 >( nwtick[id] - owtick[id] ) ) break;
			
			//如果没就绪，直接退出
			if( 1 == Cat_Gun_Ready_Signal(id) ) 
			{sta[id] = 0; break;}
			//如果没按下开枪键，直接退出
			if( 1 == Cat_Gun_Open_Signal(id) )
			{sta[id] = 0; break;}
			
			sta[id]++;
			break;
		case 2:	
			//开枪
			Open_Gun( id );
			PRT_LOG("id(%d) ->open gun \r\n",id);
			audio_1->req_play_file( _em_shooting );
			sta[id]++;
			owtick[id] = timtick->get();
			break;
		case 3://关枪
			nwtick[id] = timtick->get();
			if( 150 <( nwtick[id] - owtick[id] ) )
			{
				Close_Gun( id );
				PRT_LOG("id(%d) ->close gun \r\n",id);
				sta[id]++;	//间隔延时
				owtick[id] = nwtick[id];
			}
			break;
		case 4://间隔控制
			nwtick[id] = timtick->get();
			if( 100 <( nwtick[id] - owtick[id] ) )
			{
				sta[id] = 5;
				owtick[id] = nwtick[id];
			}
			break;
		case 5:
			//如果没按下开枪键，直接退出
			if( 1 == Cat_Gun_Open_Signal(id) )
			{
				sta[id] = 6;
				owtick[id] = nwtick[id];
			}
			break;
		case 6://确认松手了
			//频率控制
			nwtick[id] = timtick->get();
			if( 20 >( nwtick[id] - owtick[id] ) ) break;
			
			//等待释放
			if( 1 == Cat_Gun_Open_Signal(id) )
			{
				sta[id] = 0;
			}
			else
			{
				sta[id]--;
			}
			
			break;
		default:
			sta[id] = 0; 
			break;
	}
	
}
