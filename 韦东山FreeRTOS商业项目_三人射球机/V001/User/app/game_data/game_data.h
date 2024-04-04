#ifndef __game_data_head__
#define __game_data_head__

#include "fk_soft_typedef.h"

//约定
#define mod_use_no_ex		//意思是模块内部使用，外部不能直接使用

//板子配置信息
typedef struct {
	u8 master;			/*是否为主机*/
	u8 price;				/*几币一局*/
	u8 time;				/*一局多长时间*/
	u8 bullet;			/*几一局*/
}bconf_t,*bconf_pt;
extern bconf_t bconf;			//本机的配置数据
extern u8 read_coder(void);	//获取拨码器值



typedef enum{
	_P1_ = 0,
	_P2_,
	_P3_,
	_P4_,
}id_t,*id_pt;
typedef struct{
	u8 bootcode;						//启动码
	u8 id;									//玩家ID =0 P1; =1 P2
	u8 light_tick;					//灯光闪烁
	u8 mainstate;						//主状态
	u8 substate;						//子状态
	u8 remaining_coins;			//剩余币数
	u8 one_round_coins;			//一局价格
	u8 remaining_times;			//剩余时长
	u8 one_round_times;			//一局时长
	u8 one_round_bullets;		//一局子弹数	
	u32 remaining_score;		//得分
	u32 one_gift_score;			//一个礼品需要多少分
	u32 gift_cntr;					//礼品输出计数
	
}gdat_t,*gdat_pt;

//游戏数存储操作
typedef struct{
	void (*write_gift_cntr)(u8 id,u32 val);
	void (*write_player_data)(u8 id,void* pdat);
}do_conf_t,*do_conf_pt;

typedef struct{
	gdat_pt (*create)(void);
	void (*destory)(gdat_pt pgdat);
	void (*config)(u8 id,void* pdat);
	do_conf_t data_store;
}gdat_opt_t,*gdat_opt_pt;

extern gdat_opt_pt gdat_opt;				//对外统一APP操作接口
extern void init_gdat_opt(void);			//对外统一初始化接口




#endif



