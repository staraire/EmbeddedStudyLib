#include "drv_opt.h"

extern hc165_pt hc165_port1;					//对外接口
extern void init_drv_hc165_port1(void);

/*-------------------------------------------------------------------------------------------------*/

static thread_cslock_t cslock;
static input_bus_t do_ex_inputbus;
static input_bus_pt p_ex_inputbus;

#define CHIP_MAX 3
static u8 Pin[ CHIP_MAX ];
static u8 tPin[ CHIP_MAX ];
static void input_bus_refresh(void)
{
	u8 i=0;
	
	hc165_port1->update_bus();

	for(i=0;i< CHIP_MAX ;i++)
	{	
		tPin[i] = hc165_port1->read_byte();
	}
	
	thread_cslock_lock(cslock,MaxTick);	//上锁
	for(i=0;i< CHIP_MAX ;i++)
	{	
		Pin[i] = tPin[i];
	}
	thread_cslock_free(cslock);					//解锁
	
	
}

static u8 input_bus_get_bit(u8 pin)
{
	u8 val=0;
	u8 tbyte = 0;
	u8 chip = 0;
	chip = pin/8;
	thread_cslock_lock(cslock,MaxTick);	//上锁
	tbyte = Pin[chip];									//拿数据
	thread_cslock_free(cslock);					//解锁
	val = (tbyte>>(pin%8))&0x01;
	return val;
}
/*-------------------------------------------------------------------------------------------------*/

input_bus_pt ex_inputbus;
void init_drv_ex_inputbus(void)
{
	
	init_drv_hc165_port1();
	cslock = thread_cslock_init("ex_inputbus");
	
	p_ex_inputbus = &do_ex_inputbus;
	p_ex_inputbus->refresh = input_bus_refresh;
	p_ex_inputbus->get_bit = input_bus_get_bit;
	
	//接口外放
	ex_inputbus = p_ex_inputbus;
	
}

