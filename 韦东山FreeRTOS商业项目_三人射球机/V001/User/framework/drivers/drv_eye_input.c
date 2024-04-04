
#include "drv_opt.h"
/*--------------------------------------------------------------------------------*/

		
//P6口的输入引脚
#define Get_EB_00()	GPIO_ReadInputDataBit( GPIOA, GPIO_Pin_8 )		
#define Get_EB_01()	GPIO_ReadInputDataBit( GPIOB, GPIO_Pin_15 )		
#define Get_EB_02()	GPIO_ReadInputDataBit( GPIOB, GPIO_Pin_14 )

//P12口的输入引脚
#define Get_EB_03()	GPIO_ReadInputDataBit( GPIOA, GPIO_Pin_7 )		
#define Get_EB_04()	GPIO_ReadInputDataBit( GPIOA, GPIO_Pin_6 )		
#define Get_EB_05()	GPIO_ReadInputDataBit( GPIOA, GPIO_Pin_5 )
#define Get_EB_06()	GPIO_ReadInputDataBit( GPIOA, GPIO_Pin_4 )

//P8口的Pin2
#define Get_EB_07()	GPIO_ReadInputDataBit( GPIOB, GPIO_Pin_1 )
#define Get_EB_08()	GPIO_ReadInputDataBit( GPIOA, GPIO_Pin_1 )


/*--------------------------------------------------------------------------------*/

static thread_cslock_t cslock;
static eye_bus_t do_eye_bus;
/*--------------------------------------------------------------------------------*/



static uint32_t read_bus(void)
{
	
	uint32_t ret = 0;
	uint32_t tval = 0;
	thread_cslock_lock(cslock, MaxTick);			//上锁
	
	tval = Get_EB_00();			ret |= (tval<<0);
	tval = Get_EB_01();			ret |= (tval<<1);
	tval = Get_EB_02();			ret |= (tval<<2);
	tval = Get_EB_03();			ret |= (tval<<3);
	tval = Get_EB_04();			ret |= (tval<<4);
	tval = Get_EB_05();			ret |= (tval<<5);
	tval = Get_EB_06();			ret |= (tval<<6);
	tval = Get_EB_07();			ret |= (tval<<7);
	tval = Get_EB_08();			ret |= (tval<<8);
	thread_cslock_free(cslock);						//解锁
	
	return ret;
}

static u8 read_eye(u8 pin)
{
	u8 ret = 0;
	
	//thread_cslock_lock(cslock, MaxTick);			//上锁
	switch(pin)
	{
		case 0:ret = Get_EB_00();					break;
		case 1:ret = Get_EB_01();					break;
		case 2:ret = Get_EB_02();					break;
		case 3:ret = Get_EB_03();					break;
		case 4:ret = Get_EB_04();					break;
		case 5:ret = Get_EB_05();					break;
		case 6:ret = Get_EB_06();					break;		
		case 7:ret = Get_EB_07();					break;		
		case 8:ret = Get_EB_08();					break;
		default:
			printf("error: read_eye\r\n");
			break;
	}
	//thread_cslock_free(cslock);						//解锁
	
	return ret;
}


/*--------------------------------------------------------------------------------*/
//对外开放接口

eye_bus_pt eye_input;
void init_drv_eye_input(void)
{	
	cslock = thread_cslock_init("eye_bus");
	//初始化引脚
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*开启GPIO的端口时钟*/
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE); 
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);

	/*以下是输入引脚*/
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IPU;  
	
	 
	
	//P6口的输入引脚
	/*PA8*/		GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_8;	  	GPIO_Init(GPIOA, &GPIO_InitStructure);		
	/*PB14*/	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_14;	 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/*PB15*/	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_15;	 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//P12口的输入引脚
	/*PA4*/		GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_4;	  	GPIO_Init(GPIOA, &GPIO_InitStructure);		
	/*PA5*/		GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_5;	  	GPIO_Init(GPIOA, &GPIO_InitStructure);		
	/*PA6*/		GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_6;	  	GPIO_Init(GPIOA, &GPIO_InitStructure);		
	/*PA7*/		GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_7;	  	GPIO_Init(GPIOA, &GPIO_InitStructure);		
	
	//P8口
	/*PB1*/   GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_1;	  	GPIO_Init(GPIOB, &GPIO_InitStructure);		
	/*PA1*/   GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_1;	  	GPIO_Init(GPIOA, &GPIO_InitStructure);		
 

	//操作映射
	eye_input = &do_eye_bus;
	eye_input->read_eye = read_eye;
	eye_input->read_bus = read_bus;
	
	
}




