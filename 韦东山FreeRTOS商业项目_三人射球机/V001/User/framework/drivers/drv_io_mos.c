#include "drv_opt.h"

/*--------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------*/
//static thread_cslock_t cslock;
/*--------------------------------------------------------------------------------*/
static mos_t do_mos;
static mos_pt pmos;

/*-----------------------------------------------------------------*/
//P14 MOS管
static void open_io_mos_0(void){GPIO_SetBits(GPIOB, GPIO_Pin_9);}
static void open_io_mos_1(void){GPIO_SetBits(GPIOB, GPIO_Pin_8);}
static void open_io_mos_2(void){GPIO_SetBits(GPIOB, GPIO_Pin_7);}
static void open_io_mos_3(void){GPIO_SetBits(GPIOB, GPIO_Pin_6);}

//P8 MOS管信号
static void open_io_mos_4(void){GPIO_ResetBits(GPIOB, GPIO_Pin_0);}



//P14 MOS管
static void close_io_mos_0(void){GPIO_ResetBits(GPIOB, GPIO_Pin_9);}
static void close_io_mos_1(void){GPIO_ResetBits(GPIOB, GPIO_Pin_8);}
static void close_io_mos_2(void){GPIO_ResetBits(GPIOB, GPIO_Pin_7);}
static void close_io_mos_3(void){GPIO_ResetBits(GPIOB, GPIO_Pin_6);}
//P8 MOS管信号 
static void close_io_mos_4(void){GPIO_SetBits(GPIOB, GPIO_Pin_0);}


#define IO_MOS_MAX 5
vvfunc open_io_mos[IO_MOS_MAX] = {
	open_io_mos_0,
	open_io_mos_1,
	open_io_mos_2,
	open_io_mos_3,
	open_io_mos_4,
	
	
};
vvfunc close_io_mos[IO_MOS_MAX] = {
	close_io_mos_0,
	close_io_mos_1,
	close_io_mos_2,
	close_io_mos_3,
	close_io_mos_4,
	
};


static void init_hw_io_mos(void)
{
	//初始化引脚
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*开启GPIO的端口时钟*/
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE); 
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);		//使用映射功能
	
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz; 
	
	//P14 MOS管
	/*PB6*/	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_6;	GPIO_Init(GPIOB, &GPIO_InitStructure);			 
	/*PB7*/	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_7;	GPIO_Init(GPIOB, &GPIO_InitStructure);			 
	/*PB8*/	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_8;	GPIO_Init(GPIOB, &GPIO_InitStructure);			 
	/*PB9*/	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9;	GPIO_Init(GPIOB, &GPIO_InitStructure);			 
	
	//P8 MOS管信号
	/*SCK4->PB0*/	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_0;	GPIO_Init(GPIOB, &GPIO_InitStructure);			 
	
	
	//马上关闭mos管
	for(int i=0;i<IO_MOS_MAX;i++)
		close_io_mos[i]();
	
	
}

/*-----------------------------------------------------------------*/
static void mos_open(u8 road)
{
	//thread_cslock_lock(cslock,MaxTick);
	open_io_mos[road]();
	//thread_cslock_free(cslock);
}

static void mos_close(u8 road)
{
	//thread_cslock_lock(cslock,MaxTick);
	close_io_mos[road]();
	//thread_cslock_free(cslock);
}

/*-----------------------------------------------------------------*/



mos_pt iomos;
void init_drv_io_mos(void)
{
	//cslock = thread_cslock_init("ex_mos_h1");
	init_hw_io_mos();
	
	pmos = &do_mos;
	pmos->open = mos_open;
	pmos->close = mos_close;

	/*接口外放*/
	iomos = pmos;
	
	iomos->close(3);
	iomos->close(4);
}


/*--------------------------------------------------------------------------------*/













