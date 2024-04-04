#include "drv_opt.h"

#define wide 40
static void SL_H(void){ GPIO_SetBits(GPIOC, GPIO_Pin_14); }
static void SL_L(void){ GPIO_ResetBits(GPIOC, GPIO_Pin_14); }

static void CLK_H(void){ GPIO_SetBits(GPIOC, GPIO_Pin_13); }
static void CLK_L(void){ GPIO_ResetBits(GPIOC, GPIO_Pin_13); }

static u8 DAT_STA(void){ return GPIO_ReadInputDataBit( GPIOB, GPIO_Pin_5 );	}


void init_hw_hc165(void)
{
	
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*开启GPIO的端口时钟*/  
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE); 
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);		//使用映射功能
	
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;   
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz; 
	//PC13--> CLK
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_13;	GPIO_Init(GPIOC, &GPIO_InitStructure);		
	//PC14--> SL
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_14;	GPIO_Init(GPIOC, &GPIO_InitStructure);		
	//PB15--> DAT
	
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IPU;   
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_5;	GPIO_Init(GPIOB, &GPIO_InitStructure);		

	
	
}	


/*--------------------------------------------------------------------------------*/

static hc165_t do_hc165;
static hc165_pt hc165;


static volatile int d=0;

/*--------------------------------------------------------------------------------*/


//从引脚加载数据到移位寄存器
static void update_bus(void)
{
	SL_L();		 		//HC165加载外部引脚数据到移位寄存器
	d=wide;while(d--);
	SL_H();				//HC165停止加载外部引脚数据
	d=wide;while(d--);
	
}

	
//从移位寄存器读出数据
static u8 read_byte(void)
{
	unsigned char i;
	unsigned char Temp;	   
	
	Temp = 0;
	if( DAT_STA() == 1) Temp |= 0x01;   //此时HC165_OUT为Temp的最高位。
	for(i = 0;i < 7;i++)  //需要移位7次，才能读完
	{	
	   CLK_L();
	   d=wide;while(d--);
	   CLK_H();	  //时钟上升沿，HC165移位 
	   d=wide;while(d--);
	   Temp <<= 1;		  //Temp左移一位，为新来的数据空出最后一位
	   if( DAT_STA() == 1) //读入新来的数据到最后一位
	   {
		   Temp |= 0x01;  
	   }
	}
	CLK_L();
	d=wide;while(d--);
	CLK_H();
	d=wide;while(d--);
	return(Temp);		 //经过7次循环后，D6，D5，D4，D3，D2，D1，D0被依次读入Temp的7个位中

}


/*--------------------------------------------------------------------------------*/
//因为被其他上层驱动深度调用，所以，由上层驱动进行互斥管理，
hc165_pt hc165_port1;					//对外接口
void init_drv_hc165_port1(void)
{
	int i=0;
	static u8 flag = 0;
	if(flag == 1)return ;
	flag = 1;
	init_hw_hc165();
	hc165 = &do_hc165;
	hc165->read_byte = read_byte;
	hc165->update_bus = update_bus;
	/*接口外放*/
	hc165_port1 = hc165;
	
}
/*--------------------------------------------------------------------------------*/





