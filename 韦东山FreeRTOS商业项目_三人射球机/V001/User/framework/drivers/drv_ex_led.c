#include "drv_opt.h"

/*--------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------*/
extern hc595_pt hc595_for_2803;					//对外接口
extern void init_drv_hc595_for_2803(void);
static thread_cslock_t cslock;
/*--------------------------------------------------------------------------------*/
static led_t do_led;
static led_pt pled;

/*-----------------------------------------------------------------*/

static void led_open(u8 road)
{
	thread_cslock_lock(cslock,MaxTick);
	hc595_for_2803->set_bit(road);
	thread_cslock_free(cslock);
}

static void led_close(u8 road)
{
	thread_cslock_lock(cslock,MaxTick);
	hc595_for_2803->clr_bit(road);
	thread_cslock_free(cslock);
}

/*-----------------------------------------------------------------*/

led_pt exled;
void init_drv_ex_led(void)
{
	cslock = thread_cslock_init("ex_led_h1");
	init_drv_hc595_for_2803();
	
	pled = &do_led;
	pled->open = led_open;
	pled->close = led_close;

	/*接口外放*/
	exled = pled;
	
	
}


/*--------------------------------------------------------------------------------*/



