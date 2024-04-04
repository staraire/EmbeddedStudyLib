#include "host.h"
#include "task_manager.h"
#include "fk_soft_typedef.h"
#include "drv_opt.h"
#include "game_data.h"
#include "game_logic.h"
extern gdat_pt gamedat[ ];

#define PRT_LOG(X...)	//dbg->printf(X)	

static thread_mq_t local_mq[PlayerNum];

static int eled_init = 0;
void init_exled_coroutine(void)
{
	for( uint8_t i=0;i<PlayerNum;i++ )
		local_mq[i] = thread_mq_init("local_mq", sizeof(_a_led_t), 10);
	eled_init = 1;
	
}

void pull_exled_req( uint8_t id,uint8_t road,uint8_t pwr)
{
	_a_led_t tled;
	tled.pwr = pwr;
	tled.road = road;
	thread_mq_send(local_mq[id],&tled, sizeof(_a_led_t), 0xffff);
	
}
static void listen_exled_req( uint8_t id )
{
	_a_led_t tled;
	gdat_pt pgdat = gamedat[id];
	int ret = 0;
	//从队列中读出数据
	ret = thread_mq_recv( local_mq[id], &tled, sizeof(_a_led_t), 0)
	if(ret!=bFALSE) 
	{
		
		PRT_LOG("req (%d)->led(%d)->pwr(%d) \r\n",id,tled.road,tled.pwr);
		if( tled.pwr == ON )
			exled->open( tled.road );
		else
			exled->close( tled.road );
		
		pull_seg_disp_num_req( id,_seg_score,pgdat->remaining_score);
	}
}

void listen_exled_coroutine(void)
{
	if( eled_init == 0 )return;
	for( uint8_t i=0;i<PlayerNum;i++ )
		listen_exled_req(i);
}
