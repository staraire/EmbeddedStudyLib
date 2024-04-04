#include "game_data.h"


/*--------------------------------------------------------------------------------*/
//模块内部
extern gdat_pt game_data_create(void);
extern void game_data_destory(gdat_pt pgdat);
extern void read_board_config(u8 id,void* pdat);
extern void write_game_data_player_data(uint8_t id,void* pdat);

/*--------------------------------------------------------------------------------*/
//数据的具体操作
extern void wirte_game_data_gift_cntr( u8 id,u32 cntr );
extern void weite_game_data_player_data(uint8_t id,void* pdat);
/*--------------------------------------------------------------------------------*/
static gdat_opt_t do_gdat_opt;


gdat_opt_pt gdat_opt;




void init_gdat_opt(void)
{
	
	
	//总体操作
	gdat_opt = &do_gdat_opt;
	gdat_opt->create 	= game_data_create;
	gdat_opt->destory = game_data_destory;
	gdat_opt->config = read_board_config;
	gdat_opt->data_store.write_gift_cntr = wirte_game_data_gift_cntr;
	gdat_opt->data_store.write_player_data = write_game_data_player_data;
	
	
}




