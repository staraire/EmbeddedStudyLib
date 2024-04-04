#include "game_data.h"
#include "fk_soft_typedef.h"
#include "game_logic.h"
#include "drv_opt.h"
mod_use_no_ex gdat_pt game_data_create(void)
{
	gdat_pt pgdat = mem_alloc( sizeof(gdat_t) );
		
	return pgdat;
}





