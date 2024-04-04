#include "host.h"
#include "task_manager.h"
#include "fk_soft_typedef.h"
#include "drv_opt.h"
#include "game_data.h"
#include "game_logic.h"
#define PRT_LOG(X...)	//dbg->printf(X)	


static time_t owtick = 0;
static time_t nwtick = 0;

static int sta = 0;
static int active = 0;		//描述是否活跃
static int req_active = 0;		//请求活跃
static int req_unactive = 0;	//请求失活
#define open_doll_move_mtr() 	{iomos->open(3);}
#define close_doll_move_mtr() {iomos->close(3);}


//请求活跃
void req_doll_move_active(void)
{
	req_active = 1;
	PRT_LOG("req_doll_move_active \r\n");
}

//请求失活
void req_doll_move_unactive(void)
{
	req_unactive = 1;
	PRT_LOG("req_doll_move_unactive \r\n");
}



void listen_doll_move_coroutine(void)
{
	if( active == 1)//活跃状态
	{
		//如果还是有活跃请求，就忽略掉，直接清除
		if( req_active ) req_active = 0;
		
		//如果有失活请求，则进入失活状态
		if( req_unactive )
		{
			req_unactive = 0;	//处理请求
			active = 0;				//失活
			sta = 0;					//清零状态
			return;
		}
		
		switch( sta )
		{
			case 0://设置电机
				open_doll_move_mtr();		//开电机
				sta ++;
				PRT_LOG( "active , sta ->%d \r\n",sta );
				break;
			case 1://电机一直开，等待失活信号
				
				break;
			default:
				sta = 0;
				break;
		}
	}
	/*----------------------------------------------*/
	else	//失活状态
	{
		//如果还是有失活请求，就忽略掉，直接清除
		if( req_unactive ) req_unactive = 0;
		
		//如果有活跃请求，则进入活跃状态
		if( req_active )
		{
			req_active = 0;	//处理请求
			active = 1; 		//活跃
			sta = 0;				//清零状态
			return;
		}
		
		switch( sta )
		{
			case 0://开始计时
				owtick = timtick->get();
				sta ++;
				PRT_LOG( "unactive , sta ->%d \r\n",sta );
				break;
			case 1://时间到了，就把电机给停下来
				nwtick = timtick->get();
				if( 30000 >( nwtick - owtick ) ) break;
				close_doll_move_mtr();
				sta ++;
				PRT_LOG( "unactive , sta ->%d \r\n",sta );
				break;
			case 2://啥都不做，等活跃
				
				break;
			default:
				sta = 0;
				break;
		}
	}

}





