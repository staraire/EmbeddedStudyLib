#include "game_data.h"
#include "host.h"
#include "drv_opt.h"
#include "ui_manager.h"
#include <stdint.h>
#define PRT_LOG(X...)	//dbg->printf(X)	

/*----------------------------------------------------------------------------------*/
//按键事件传递部分程序
/*----------------------------------------------------------------------------------*/

static uint8_t init_flag = 0;		//模块是否被初始化

static thread_mq_t local_mq;
void init_keyboard_coroutine(void)
{
	//创建队列
	local_mq = thread_mq_init("local_mq", sizeof(uint8_t), 10);
	init_flag = 1;
}

void pull_keyboard_event( uint8_t val )
{
	if( init_flag == 0 ) return;
	thread_mq_send(local_mq,&val, sizeof(uint8_t), 0xf);
}

uint8_t get_keyboard_event( void )
{
	
	uint8_t ret = 0;
	uint8_t val = 0;
	if( init_flag == 0 ) return 0;
	ret = thread_mq_recv( local_mq, &val, sizeof(uint8_t), 0);
	if(ret!=bFALSE)
	{
		return val;
	}
	return _key_none;
}

/*----------------------------------------------------------------------------------*/







/*----------------------------------------------------------------------------------*/
//按键检测部分程序
/*----------------------------------------------------------------------------------*/


//菜单->Pin21-->bit20
//返回->Pin22-->bit21
//上键->Pin23-->bit22
//下键->Pin24-->bit23

/* 获取按键键值 */
static uint8_t Action_Key_Get(void)
{
	uint8_t temp = _key_none;
	if( 0 == ex_inputbus->get_bit(20) ){temp = _key_menu;}
	if( 0 == ex_inputbus->get_bit(21) ){temp = _key_back;}
	if( 0 == ex_inputbus->get_bit(22) ){temp = _key_up;}
	if( 0 == ex_inputbus->get_bit(23) ){temp = _key_dn;}	
	return temp;
} 
/*----------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------*/
/*  
函数名:listen_coroutine_keyboard()
功能:调用按键检测函数，检测键值，并根据键值进行相应的操作。
返回值:无
*/

static uint8_t state = 0;
static uint8_t okeyval = 0;
static uint8_t nkeyval = 0;


void listen_coroutine_keyboard(void)
{
	//如果模块没有被初始化，直接退出，防止队列错误
	if( init_flag == 0 ) return;
	
	nkeyval = Action_Key_Get();
	switch( state )
	{
		case 0://等待有按键信号
			if( nkeyval != _key_none )
			{
				okeyval = nkeyval;	//存下来
				state ++;
			}
			break;
		case 1://明确是否真的是有效的信号
			if( nkeyval == okeyval )
			{//说明的确是有信号按键按下了
				state ++;
			}
			break;
		case 2://等待松开的信号
			if( nkeyval != okeyval )
			{//说明按键可能松开了
				state++;
			}
			break;
		case 3://明确是否真的松开了
			if( nkeyval != okeyval )
			{//说明按键真的松开了
				state++;
			}
			break;
		case 4://将事件发送出去
			
			PRT_LOG(" key event:val = 0x%02x \r\n",okeyval);
			pull_keyboard_event( okeyval );
			state = 0;
			break;
	}
}

