#include "host.h"
#include "task_manager.h"
#include "fk_soft_typedef.h"
#include "drv_opt.h"
#include "game_data.h"
#include "game_logic.h"
#define PRT_LOG(X...)	//dbg->printf(X)	
	
#define TYPE_num 					0				//内容是数字
#define TYPE_graphical 		1				//内容是图形

static thread_mq_t local_mq[PlayerNum];
static uint8_t init_flag = 0;
void init_seg_coroutine(void)
{
	for( u8 i=0;i<PlayerNum;i++ )
		local_mq[i] = thread_mq_init("local_mq", sizeof(_a_seg_t), 20);
	init_flag = 1;
}


//请求显示数字
void pull_seg_disp_num_req( uint8_t id,uint8_t arg,uint32_t val)
{
	_a_seg_t tseg;
	tseg.mode = TYPE_num;
	tseg.arg = arg;
	tseg.val = val;
	
	if( init_flag == 0 ) return;
	thread_mq_send(local_mq[id],&tseg, sizeof(_a_seg_t), 0x1);
	
}


//请求显示图形
void pull_seg_disp_graphical_req( uint8_t id,uint8_t arg,uint32_t val)
{
	_a_seg_t tseg;
	tseg.mode = TYPE_graphical; //图形
	tseg.arg = arg;
	tseg.val = val;
	
	if( init_flag == 0 ) return;
	thread_mq_send(local_mq[id],&tseg, sizeof(_a_seg_t), 0x1);
}

static time_t oretick[PlayerNum];
static time_t nretick[PlayerNum];
//数码管内容
void cycle_refresh( uint8_t id,uint8_t arg,uint32_t val,time_t tick)
{
	nretick[id] = timtick->get();
	if( tick<( nretick[id] - oretick[id] ) )
	{
		oretick[id] = nretick[id];
		pull_seg_disp_num_req( id,arg,val);
	}
}


//暂停数码管请求
static int suspend = 0;
void suspend_seg_coroutine(void)
{
	suspend = 1;
}



static void listen_seg_req( u8 id )
{
	_a_seg_t tseg;
	int ret = 0;
	int mode = 0;
	int board = 0;
	uint32_t val  = 0;
	
	
	//从队列中读出数据
	ret = thread_mq_recv( local_mq[id], &tseg, sizeof(_a_seg_t), 0);
	if(ret!=bFALSE) 
	{
		mode = tseg.mode;
		board = id*3+tseg.arg;			//得到是第几片数码管
		val = tseg.val;							//得到需要显示的内容
		
		PRT_LOG("req seg_bus (%d)->board(%d)->val(%d) \r\n",id,board,val);
		PRT_LOG("display_val ->board(%d)->val(%d) \r\n",board,val);
		if( mode ==  TYPE_num )
		{
			ex_seg_bus->display_val( board,val );
		}
		else if( mode == TYPE_graphical )
		{
			ex_seg_bus->display_graph( board,val  );
		}
		
		
	}
}

void listen_seg_coroutine(void)
{
	if( init_flag == 0 ) return;		//如果还没初始化，则不需要监听
	if( suspend == 1 ) return;			//如果挂起了，则不需要监听
	for( u8 i=0;i<PlayerNum;i++ )
		listen_seg_req(i);
}
