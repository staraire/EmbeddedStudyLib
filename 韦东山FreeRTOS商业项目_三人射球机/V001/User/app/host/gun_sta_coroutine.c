#include "host.h"
#include "task_manager.h"
#include "fk_soft_typedef.h"
#include "drv_opt.h"
#include "game_data.h"
#include "game_logic.h"
#define PRT_LOG(X...)//	dbg->printf(X)	

#define Cat_Gun_Ready_Signal( ID ) ex_inputbus->get_bit( 3 + ID  )

static u8 pin[ PlayerNum ];
static time_t otick = 0;
static time_t ntick = 0;


void listen_all_gun_ready_coroutine(void)
{
	u8 id=0;
	//执行频率限制
	ntick = timtick->get();
	if( 2000 > ( ntick - otick ) ) return;
	otick = ntick;
	
	
	//获取引脚的状态
	for( id=0; id<PlayerNum; id++ )
	{
			pin[id] = Cat_Gun_Ready_Signal( id );
	}
	
	if(
		( pin[0] == 1 )&&
		( pin[1] == 1 )&&
		( pin[2] == 1 )
	)//失活请求
	{
		req_doll_move_unactive();
		
		
	}
	else
	{//活跃请求
		req_doll_move_active();
		
	}
		
		
}


