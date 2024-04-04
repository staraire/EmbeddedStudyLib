
#include "drv_opt.h"
#include <stdio.h>

extern hc595_pt hc595_for_zfmtr;					//对外接口
extern void init_drv_hc595_for_zfmtr(void);
/*--------------------------------------------------------------------------------*/
static u32 mtr_dir[12];					//描述电机方向
static u32 mtr_pwr[12];					//描述电机电源
static thread_cslock_t cslock;
static zfmtr_t do_zfmtr;
static zfmtr_pt zfmtr;
u32 hc245_init = 0;
/*--------------------------------------------------------------------------------*/
static void enable_ctrl(void)
{
	if( hc245_init ){return;}
	hc245_init = 1;
	
}


static void init_this_drv_hw(void)
{
	enable_ctrl();
	init_drv_hc595_for_zfmtr();
		
}

static void hw_set_mtr_dir_z(u8 road)
{
//H L
	hc595_for_zfmtr->set_bit( 0+road*2 );
	hc595_for_zfmtr->clr_bit( 1+road*2 );
	
}

static void hw_set_mtr_dir_f(u8 road)
{
//L H
	hc595_for_zfmtr->clr_bit( 0+road*2 );
	hc595_for_zfmtr->set_bit( 1+road*2 );
	
}
static void hw_set_mtr_pwr_off(u8 road)
{
//L L
	hc595_for_zfmtr->clr_bit( 0+road*2 );
	hc595_for_zfmtr->clr_bit( 1+road*2 );
	

}

/*--------------------------------------------------------------------------------*/

//设置转动方向(直接设置引脚)
static void set_dir(u8 road,u32 dir )
{
	switch(dir)
	{
		case __DIR_Z_:mtr_dir[road] = dir;break;
		case __DIR_F_:mtr_dir[road] = dir;break;
		default:break;
	}
}
//设置电机电源(直接设置引脚)
static void set_pwr(u8 road,u32 pwr)
{
	if( pwr == OFF )
	{
		mtr_pwr[road] = OFF;
		hw_set_mtr_pwr_off(road);
	}
	else
	{
		switch( mtr_dir[road] )
		{
			case __DIR_Z_:
				hw_set_mtr_dir_z(road);
				mtr_pwr[road] = ON;
				break;
			case __DIR_F_:
				hw_set_mtr_dir_f(road);
				mtr_pwr[road] = ON;
				break;
			default:
				break;
		}
	}
}
/*--------------------------------------------------------------------------------*/
static u32 get_dir(u8 road)
{
	u32 ret = 0;
	thread_cslock_lock(cslock,MaxTick);
	ret = mtr_dir[road];
	thread_cslock_free(cslock);
	return ret;
}
static u32 get_pwr(u8 road)
{
	u32 ret = 0;
	thread_cslock_lock(cslock,MaxTick);
	ret = mtr_pwr[road];
	thread_cslock_free(cslock);
	return ret;
}
static void set(u8 road,u32 pwr,u32 dir)
{
	dbg->printf("do set(u8 road,u32 pwr,u32 dir) \r\n ");
	thread_cslock_lock(cslock,MaxTick);
	set_dir(road,dir);
	set_pwr(road,pwr);
	thread_cslock_free(cslock);
	
}

/*--------------------------------------------------------------------------------*/
zfmtr_pt ex_zfmtr;

void init_drv_ex_zfmtr(void)
{
	init_this_drv_hw();
	
	cslock = thread_cslock_init("drv_zfmtr");
	zfmtr = &do_zfmtr;
	zfmtr->get_dir = get_dir;
	zfmtr->get_pwr = get_pwr;
	zfmtr->set = set;

	//外放接口
	ex_zfmtr = zfmtr;
	
}



