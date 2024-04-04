#ifndef __ui_manager_h__
#define __ui_manager_h__
#include "fk_soft_typedef.h"
#include <stdint.h>

enum _page_num_val{
	_p_idle = 0,		//空闲页面
	_p_selt_args,		//选择要设置的参数
	_p_set_args,		//设置参数的值
	
	
};

enum _key_val{
	_key_none = 0,
	_key_up,
	_key_dn,
	_key_menu,
	_key_back,
};

typedef struct PageAction {
	void 	(*run)(uint8_t dat,uint8_t *ch);						//页面调用(包含显示调用)
	void 	(*disp)(uint8_t mouse);							//显示调用(提供给其他文件显示页面)
	uint8_t 	(*get_input_event)(void);					//获取输入事件
	uint8_t		mouse;										//通用鼠标描述符	
}page_action_t,*page_action_pt;


extern page_action_pt PageV(uint8_t page_num);

#endif

