#ifndef __host_thread_h__
#define __host_thread_h__

#include "fk_soft_typedef.h"

/*LED的控制参数*/
typedef struct{
	uint8_t road;
	uint8_t pwr;
}_a_led_t,*_a_led_pt;

//数码管的控制参数
typedef struct{
	uint8_t mode;		//模式
	uint8_t arg;		//本次需要设置的参数
	uint32_t val;		//值
	
	
}_a_seg_t,*_a_seg_pt;

extern void host_master_thread(void);

//exled_coroutine.c
extern void init_exled_coroutine(void);
extern void pull_exled_req( uint8_t id,uint8_t road,uint8_t pwr);

extern void listen_exled_coroutine(void);

//seg_coroutine.c
#define _seg_one_coin_ball	0		//数码管序号，一币几球
#define _seg_one_gift_score	1		//数码管序号，多少分一个礼品
#define _seg_score 					2		//数码管序号，当前分数
extern void init_seg_coroutine(void);
//推送数码管显示数字请求
extern void pull_seg_disp_num_req( uint8_t id,uint8_t arg,uint32_t val);
//推送数码管显示图形请求
extern void pull_seg_disp_graphical_req( uint8_t id,uint8_t arg,uint32_t val);
//刷新数码管内容
extern void cycle_refresh( uint8_t id,uint8_t arg,uint32_t val,time_t tick);
extern void listen_seg_coroutine(void);
extern void suspend_seg_coroutine(void);


//doll_move_coroutine.c
extern void req_doll_move_active(void);
extern void req_doll_move_unactive(void);
extern void listen_doll_move_coroutine(void);

//gun_sta_coroutine.c
extern void listen_all_gun_ready_coroutine(void);

//keyboard_coroutine.c
extern void init_keyboard_coroutine(void);
extern void listen_coroutine_keyboard(void);
extern uint8_t get_keyboard_event( void );



#endif

