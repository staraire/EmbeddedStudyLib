
#include "ui_manager.h"

#define PRT_LOG(X...)	//dbg->printf(X)	


//本项目涉及到的页面句柄
 extern page_action_t idle_page;			//空闲页面
 extern page_action_t selt_args_page; 	//选择参数页面

static 	void 	null_page_run(uint8_t dat,uint8_t *ch)
{
	PRT_LOG(" null_page_run \r\n ");
}
static void 	null_page_disp(uint8_t mouse)
{
	PRT_LOG(" null_page_disp \r\n ");
}
static uint8_t null_page_get_input_event(void)
{
	PRT_LOG(" null_page_get_input_event \r\n ");
	return 0;
}
	
	


page_action_t null_page={
	.run = null_page_run,
	.disp = null_page_disp,
	.get_input_event = null_page_get_input_event,
	.mouse = 0,
};




//根据页面号，返回句柄
page_action_pt PageV(u8 page_num)
{
	
	switch(page_num)
	{
		case _p_idle:			//空闲页面
			return &idle_page;
		case _p_selt_args:		//选择要设置的参数
			return &selt_args_page;
		default:
			
			break;
	}

	return &null_page;
}





