
#include "drv_opt.h"
#include "fk_soft_typedef.h"

static thread_cslock_t cslock;


/*iic 底层操作*/
extern void IIC_hw_init(void);
extern void IIC_Start(void);  		// 起始位 
extern void IIC_Stop(void);			// 停止位
extern void IIC_Ack(void);			// 主机应答位
extern void  IIC_NoAck(void) ;		// 主机反向应答位
extern u8 IIC_GetACK(void);       // 获取从机应答信号，返回为0时收到ACK,返回为1时没收到ACK 
extern u8 IIC_SendByte(u8 Data);//主设备向从设备发送个一字节
extern u8 IIC_RecByte(void);	    // 接收单字节的数据,并返回该字节值

static void delay1ms()		//@24.000MHz
{
	u8 i, j;

	i = 24;
	j = 85;
	do
	{
		while (--j);
	} while (--i);
}

static void delay10ms()		//@24.000MHz
{
	u8 i, j, k;

	
	i = 1;
	j = 234;
	k = 113;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}




//******************************************************************
// 功能描述：内部函数，页写，不能跨页，也可只写单个字节
//           向指定的首地址DataAddress写入一个或多个（24C01\02最多8个）字节
// 参数说明：SlaveAddress  要写入的从器件硬件地址
//                         1010 A2 A1 A0 R/W [A2:A0]是AT24C01的芯片硬件地址
//	                       R/W是读/写选择位，0为写操作，1为读操作
//						   这里函数内部已对R/W作了处理，外部固定为0或1即可 
//           DataAddress   要写入的存储单元开始地址
//			 pbuf          指向数据缓冲区的指针
//			 Len           写入数据长度              
// 返回说明：0->成功    1->失败
//****************************************************************** 
static u8  WrToRomPageA(u8 SlaveAddress, u8 DataAddress,u8 *pbuf,u8 Len)
{
	u8 i=0;
	IIC_Start();		// 启动总线
	if(IIC_SendByte(SlaveAddress&0xfe)==1) return 1;  // 写命令，已包含有应答函数
	if(IIC_SendByte(DataAddress)==1) return 1;	      // 已包含有应答函数
	for(i=0;i<Len;i++)
	{
		if(IIC_SendByte(*pbuf++)==1) return 1;       //单片机向从机发送1个字节数据 	
	}
	IIC_Stop();			// 结束总线
	return 0;			// 写入多字节成功
}  

//******************************************************************
// 多字节写入，完全不考虑芯片分页问题，速度慢（写入时间大约是字节数n×10mS）
//******************************************************************
static u8 WrToRomA(u8 SlaveAddress, u8 DataAddress,u8 *pbuf,u8 Len)
{
	u8 i;
	while (Len--)
	{
		if (WrToRomPageA(SlaveAddress,DataAddress++,pbuf++,0x01)) //写入一个字节 	
		     return 1;		// 单字节写失败，程序返回。	
//		SDA_H();				// 判忙处理
	
		// 以下循环可用一句delay10ms()代替，为了不让总线不停的发数据产生干扰
		// 所以每延时1ms再检测芯片是否写入完毕
		for (i=0;i<10;i++)	// 写入最长时间不超过10mS	 
		{
			delay1ms();
			IIC_Start();		// 启动总线
			if(IIC_SendByte(SlaveAddress&0xfe)==0) break;   
		}		
	}
	return 0;				 // 成功返回0
}

//******************************************************************
// 内部函数，页写，为跨页编写
//******************************************************************
static u8  WrToRomPageB(u8 SlaveAddress, u8 DataAddress,u8 *pbuf,u8 Len)
{
	u8 i=0;
	IIC_Start();		// 启动总线
	if(IIC_SendByte(SlaveAddress&0xfe)==1) return 0xff;   // 失败返回0xff
	if(IIC_SendByte(DataAddress)==1) return 0xff;	      // 失败返回0xff
	for(i=0;i<Len;)
	{
		if(IIC_SendByte(*pbuf++)==1) return 0xff;         // 失败返回0xff
		i++;			     // 如果i++放到for循环语句行，一旦 break
		DataAddress++;		 // 退出当前循环，i就不能完成本次加1而出错。
		if ((DataAddress&0x07)==0) break;     // 页越界。24C01\02每页8字节，	
	}										  // 每页起始地址低3位都是000
	                         // 24C04/08/16	每页16字节，0x07应改为0x0f。
	IIC_Stop();			// 结束总线
	return (Len-i);		// 还有(Len-i)个字节没能写入
}

//******************************************************************
// 多字节写入，不受页面大小限制，速度快，若不是24C01/02芯片
// 而是24C04/08/16，则需要根据芯片分页修改被调函数的一个常数0x07为0x0f
//******************************************************************
static u8 WrToRomB(u8 SlaveAddress, u8 DataAddress,u8 *pbuf,u8 Len)
{
	u8 temp=Len;
	thread_cslock_lock(cslock,MaxTick);
	do
	{
		temp=WrToRomPageB(SlaveAddress,DataAddress+(Len-temp),pbuf+(Len-temp),temp);
		if 	(temp==0xff) return 1;	  // 失败返回1
		delay10ms();
	}while(temp);

	thread_cslock_free(cslock);
	return 0;						  // 成功返回0
}
//******************************************************************
// 功能描述：连续读操作，从DataAddress地址连续读取Len个字节到pbuf中
// 参数说明：SlaveAddress  要读取的从器件硬件地址
//                         1010 A2 A1 A0 R/W [A2:A0]是AT24C01的芯片硬件地址
//	                       R/W是读/写选择位，0为写操作，1为读操作
//						   这里函数内部已对R/W作了处理，外部固定为0或1即可 
//           DataAddress   要读取的存储单元开始地址
//			 pbuf          指向数据缓冲区的指针
//			 Len           读取数据长度              
// 返回说明：0->成功    1->失败
// ******************************************************************
static u8  RdFromROM(u8 SlaveAddress, u8 DataAddress,u8 *pbuf,u8 Len)
{ 	
    u8 i=0;                                   	      // 伪写开始
	thread_cslock_lock(cslock,MaxTick);
	IIC_Start();			                          // 启动总线
	if(IIC_SendByte(SlaveAddress&0xfe)==1) return 1;  // 写命令，已包含有应答函数
	if(IIC_SendByte(DataAddress)==1) return 1;	      // 已包含有应答函数
	                                                  // 伪写结束
	IIC_Start();				                      // 从新启动总线
	if(IIC_SendByte(SlaveAddress|0x01)==1) return 1;  // 读命令
	for(i=0;i<Len-1;i++)
	{
		*pbuf++=IIC_RecByte();	   // 接收1个字节数据
		IIC_Ack();				   // 应答0，告诉器件还要读下一字节数据
	}	
	*pbuf=IIC_RecByte();		   // 接收最后1个字节数据
    IIC_NoAck();				   // 应答1，告诉器件不再读取数据
    IIC_Stop();					   // 结束总线

	thread_cslock_free(cslock);
	return 0;					   // 读取多字节成功
}
/*--------------------------------------------------------------------------------*/



static iic_eeprom_t do_iic_24c0x;
iic_eeprom_pt iic_24c0x;

void init_drv_24c0x(void)
{
	cslock = thread_cslock_init("24c0x");
	IIC_hw_init();//不同的单板，需要修改这里
	iic_24c0x = &do_iic_24c0x;
	iic_24c0x->write 	= WrToRomB;		//对外开放的接口，需要加互斥锁
	iic_24c0x->read 	= RdFromROM;	//尤其是存储芯片
	
}




