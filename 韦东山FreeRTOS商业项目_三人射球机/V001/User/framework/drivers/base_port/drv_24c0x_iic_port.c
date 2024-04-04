
#include "drv_opt.h"
#include "fk_soft_typedef.h"


void IIC_hw_init(void)
{//配置为开漏输出 PA0 PC15
	
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*开启GPIO的端口时钟*/
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE); 
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);		//使用映射功能
	
	//初始化PC15 EEP_DAT									   
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_15;	
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_OD;   
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);			 

	//初始化PA0 EEP_CLK
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_0;	
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_OD;   
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);			 


}

void SDA_H(void)
{
	GPIO_SetBits(GPIOC, GPIO_Pin_15);	 
}
void SDA_L(void)
{
	GPIO_ResetBits(GPIOC, GPIO_Pin_15);	 
}

void SCL_H(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_0);	 
}
void SCL_L(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);	 
}

u8 Get_SDA(void)
{
	return GPIO_ReadInputDataBit( GPIOC, GPIO_Pin_15 );
}



static void Delay5us()		//@24.000MHz
{
	u8 i;
	
	i = 50;
	while (--i);
}

void IIC_Start(void)     // 起始位    
{
		SDA_H();
		SCL_H();
		Delay5us();		 // 保持4.7uS以上（TSU.STA）  
		SDA_L();
		Delay5us();		 // 保持4uS以上（THD.STA）
		SCL_L();		 // 方便下一次时钟从低到高变化的操作。
}
void IIC_Stop(void)      // 停止位
{
		SDA_L();
		SCL_H();        	 
		Delay5us();		 // 保持4.7uS以上（TSU.STO）
		SDA_H();        	  
		Delay5us();		 //	保持4.7uS以上（TBUF）
}
void IIC_Ack(void)       // 主机应答位 
{
		SDA_L();        
		Delay5us();		 // 保持0.2uS以上（TSU.DAT)
		SCL_H();        
		Delay5us();		 // 保持4.0uS以上(THIGH)
		SCL_L();
		Delay5us();		 // 保持4.7uS以上(TLOW),此语句可以不要			
}
void  IIC_NoAck(void)    // 主机反向应答位 
{
        SDA_H();        
		Delay5us();		   
        SCL_H();        
		Delay5us();		   
        SCL_L();
		Delay5us();		 		
}
u8 IIC_GetACK()       // 获取从机应答信号，返回为0时收到ACK,返回为1时没收到ACK 
{
		u8 ErrorBit;
		SDA_H(); 
	    Delay5us();		// 保持0.2uS以上（TSU.DAT)
		SCL_H();
		Delay5us();		// 保持4.0uS以上(THIGH)
		ErrorBit=Get_SDA();
		SCL_L();         
		Delay5us();		// 保持4.7uS以上(TLOW),此语句可以不要
		return ErrorBit;
} 
//*********************************************************************
// 功能描述：主设备向从设备发送个一字节
// 返回值： 0->成功  1->失败
//*********************************************************************/
u8 IIC_SendByte(u8 Data)    
{
	u8 i;               // 位数控制 
	for (i=0;i<8;i++)	   // 写入时是用时钟上升沿同步数据
	{
		if (Data & 0x80) 
			SDA_H();
        else 
			SDA_L();
        Delay5us(); 		  // 保持0.2uS以上（TSU.DAT)
        SCL_H();
		Delay5us(); 		  // 保持4.0uS以上(THIGH)
		SCL_L();
		Delay5us();		  // 保持4.7uS以上(TLOW)
        Data <<= 1;
	 }	
	 return IIC_GetACK();  
}

//*********************************************************************
// 功能描述：主设备向从设备读取一个字节
// 返回值：  读到的字节  
// *********************************************************************/
u8 IIC_RecByte(void)       // 接收单字节的数据,并返回该字节值
{
	u8 i,rbyte=0;
	SDA_H();
	for(i=0;i<8;i++)		  // 读出时是用时钟下降沿同步数据
	{
		SCL_L();
		Delay5us();			  // 保持4.7uS以上(TLOW)
		SCL_H();
		Delay5us();			  // 保持4.0uS以上(THIGH)
		if( Get_SDA() ) rbyte|=(0x80>>i); 
	}
	SCL_L();
	return rbyte; 
} 


