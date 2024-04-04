
#include "game_logic.h"
#include "game_data.h"
#include "drv_opt.h"
#include "host.h"
#define PRT_LOG(X...)	//dbg->printf(X)	
/*--------------------------------------------------------------------------------*/
//LED
//P1--> Pin1 ->bit0
//P2--> Pin2 ->bit1
//P3--> Pin3 ->bit2
#define Open_LED( ID ) 		{ pull_exled_req(ID,0+ID,ON);  } 
//#define Open_Gun( ID ) 		{ iomos->open( 0+ID );  } 

#define Close_LED( ID )		{	pull_exled_req(ID,0+ID,OFF); }
//#define Close_Gun( ID )		{ iomos->close( 0+ID );  }
//Gun Ready Signal 
// P1-> Pin4 ->ex_inputbus Pin3 
// P2-> Pin5 ->ex_inputbus Pin4
// P3-> Pin6 ->ex_inputbus Pin5
#define Cat_Gun_Ready_Signal( ID ) ex_inputbus->get_bit( 3 + ID  )

static uint8_t led_sta[ PlayerNum ];

static u8 sta[PlayerNum];
static time_t otick[PlayerNum]={0,0,0};
static time_t ntick[PlayerNum]={0,0,0};

static time_t owtick[PlayerNum]={0,0,0};
static time_t nwtick[PlayerNum]={0,0,0};

void deal_key_logic( void* pgdat )
{
	gdat_pt gdat = (gdat_pt)pgdat;
	u8 id = gdat->id;
#if 1	
	//访问频率控制
	ntick[ id ] = timtick->get();
	if( 2 >( ntick[id] - otick[id] )  )
		return;
	otick[ id ] = ntick[ id ];
#endif	
	switch( sta[id] )
	{
		case 0://点亮LED灯
			Open_LED( id );
			led_sta[id] = 1;
			sta[ id ]++;
			break;
		case 1://等待球信号
			if( 0 == Cat_Gun_Ready_Signal( id ) )
			{
				sta[ id ]++;
			}
			break;
		case 2:	//闪烁
			
			//如果
			if( 1 == Cat_Gun_Ready_Signal( id ) )
			{
				sta[ id ]= 0;
			}
			
			//闪烁频率控制
			nwtick[ id ] = timtick->get();
			if( 200 > ( nwtick[id] - owtick[id] ) ) break;
			owtick[ id ] = nwtick[ id ];
			
			//更换亮灭状态
			if( led_sta[id] == 1 )
			{
				Close_LED( id );
				led_sta[id] = 0;
			}
			else
			{
				Open_LED( id );
				led_sta[id] = 1;
			}
			
			break;
		default:
			sta[id] = 0; 
			break;
	}
	
}
