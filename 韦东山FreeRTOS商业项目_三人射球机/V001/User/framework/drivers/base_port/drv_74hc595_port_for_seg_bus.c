#include "drv_opt.h"

#define BUS_DEL 50
/*--------------------------------------------------------------------------------*/
static void SCK_H(void){GPIO_SetBits(GPIOB, GPIO_Pin_12);}
static void SCK_L(void){GPIO_ResetBits(GPIOB, GPIO_Pin_12);}

static void RCK_H(void){GPIO_SetBits(GPIOB, GPIO_Pin_2);}
static void RCK_L(void){GPIO_ResetBits(GPIOB, GPIO_Pin_2);}

static void SDO_H(void){GPIO_SetBits(GPIOB, GPIO_Pin_13);}
static void SDO_L(void){GPIO_ResetBits(GPIOB, GPIO_Pin_13);}

static void enable_hc595(void){}
static void disable_hc595(void){}
static void init_hw_hc595(void)
{
	//初始化引脚
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*开启GPIO的端口时钟*/  
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 
							   
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;   
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz; 
	//PB13  ->DS3
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_13;	GPIO_Init(GPIOB, &GPIO_InitStructure);			 
	//PB12	->CLK3
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_12;	GPIO_Init(GPIOB, &GPIO_InitStructure);			 
	//PB2		->RCK3
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_2;	GPIO_Init(GPIOB, &GPIO_InitStructure);	
}


/*--------------------------------------------------------------------------------*/
#define ChipNum	27					//总线上有多少个595 一个数码管占用2个
static unsigned char OUT[ChipNum];

static hc595_t do_hc595;
static hc595_pt hc595;


static volatile int d=0;

/*--------------------------------------------------------------------------------*/

	
	
//写数据到移位寄存器
static void write_data_hc595(unsigned char dat)
{
	
	unsigned char i;
	for(i=8;i>=1;i--)
	{
		if (dat&0x80) SDO_H(); else SDO_L();
		dat<<=1;
		SCK_L();
		d=BUS_DEL;while(d--);
		SCK_H();
		d=BUS_DEL;while(d--);
	}
	SCK_L();
	
		
}

//从移位寄存器更新数到锁存器以改变595引脚输出
static void update_data_hc595(void)
{
	RCK_L();
	d=BUS_DEL;while(d--);
	RCK_H();
	d=BUS_DEL;while(d--);
	RCK_L();
	
}

static void write_bus_data(void)
{
	for(int i=0;i<ChipNum;i++)
		write_data_hc595(OUT[ChipNum-1-i]);
}




static void set_bit_hc595(int Port)
{
	u8 chip = Port/8; 		//得到芯片号
	u8 pin = Port%8;		//得到芯片引脚号

	//修改对应芯片的对应引脚为目标电平值
	OUT[chip] |=  (1<<pin);
	//发送总线上所有芯片数据,远处优先
	write_bus_data();
	//更新总线上所以芯片的数据								
	update_data_hc595();	
	
}

static void clr_bit_hc595(int Port)
{
	u8 chip = Port/8; 		//得到芯片号
	u8 pin = Port%8;		//得到芯片引脚号
	//修改对应芯片的对应引脚为目标电平值
	OUT[chip] &=  ~(1<<pin);
	//发送总线上所有芯片数据,远处优先
	write_bus_data();
	//更新总线上所以芯片的数据												
	update_data_hc595();	
	
}

static u8 read_bit_hc595(int Port)
{
	u8 chip = Port/8; 		//得到芯片号
	u8 pin = Port%8;		//得到芯片引脚号
	u8 ret=0;	
	ret = OUT[chip];		//得到芯片的输出数据
	ret = ( ret>>pin )&0x01;
	return ret;
}

/*--------------------------------------------------------------------------------*/
//以下为扩展接口,用于数码管等静态刷新器件
static void set_bit_but_no_out(int Port)
{
	u8 chip = Port/8; 		//得到芯片号
	u8 pin = Port%8;		//得到芯片引脚号
	//修改对应芯片的对应引脚为目标电平值
	OUT[chip] |=  (1<<pin);	
}
static void clr_bit_but_no_out(int Port)
{
	u8 chip = Port/8; 		//得到芯片号
	u8 pin = Port%8;		//得到芯片引脚号
	//修改对应芯片的对应引脚为目标电平值
	OUT[chip] &=  ~(1<<pin);	
}
static void update_out_from_drv(void)
{
	
	write_bus_data();			//发送总线上所有芯片数据,远处优先
}
static void update_out_from_usr(u8* buf,u8 len)
{
	for(int i=0;i<len;i++)
		write_data_hc595(buf[len-1-i]);
}
static void update_bus(void)
{
										
	update_data_hc595();		//更新总线上所以芯片的数据			
	
}
	



/*--------------------------------------------------------------------------------*/
//因为被其他上层驱动深度调用，所以，由上层驱动进行互斥管理，
hc595_pt hc595_seg_bus;					//对外接口
void init_drv_hc595_seg_bus(void)
{
	int i=0;
	static u8 flag = 0;
	if(flag == 1)return ;
	flag = 1;
	
	
	init_hw_hc595();
			 
	hc595 = &do_hc595;
	hc595->enable 	= enable_hc595;
	hc595->disable 	= disable_hc595;
	hc595->set_bit 	= set_bit_hc595;
	hc595->clr_bit 	= clr_bit_hc595;
	hc595->set_bit_but_no_out = set_bit_but_no_out;
	hc595->clr_bit_but_no_out = clr_bit_but_no_out;
	hc595->update_out_from_drv = update_out_from_drv;
	hc595->update_out_from_usr = update_out_from_usr;
	hc595->update_bus = update_bus;
	
	hc595->enable();
	i=10;while(i--);
	
	for(i=0;i<ChipNum;i++) OUT[i] = 0xFF;
 	write_bus_data();		
	update_data_hc595();					
	/*接口外放*/
	hc595_seg_bus = hc595;

}
/*--------------------------------------------------------------------------------*/




