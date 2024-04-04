#ifndef __DRV_OPT_H__
#define __DRV_OPT_H__

#include "stm32f10x.h"
#include "fk_soft_typedef.h"


typedef void (*vvfunc)(void);	//无参构造函数类型

/*--------------------------------------------------------------------------------*/
//drv_timer_tick.c
/**************通用定时器TIM参数定义，只限TIM2、3、4、5************/
#define            GENERAL_TIM                   TIM2
#define            GENERAL_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            GENERAL_TIM_CLK               RCC_APB1Periph_TIM2
#define            GENERAL_TIM_Period            (1000-1)
#define            GENERAL_TIM_Prescaler         71
#define            GENERAL_TIM_IRQ               TIM2_IRQn
#define            GENERAL_TIM_IRQHandler        TIM2_IRQHandler

typedef struct{
	
	time_t (*get)(void);
	void (*delay)(time_t tick);
}systick_t,*systick_pt;

extern systick_pt timtick;
extern void init_drv_timer_tick(void);

/*--------------------------------------------------------------------------------*/
//drv_24c0x_iic_port.c
#define SlaveADDR 0xA0	  // 从机地址,格式：1010 ***0
typedef struct{	
	u8 (*write)(u8 SlaveAddress, u8 DataAddress,u8 *pbuf,u8 Len);
	u8  (*read)(u8 SlaveAddress, u8 DataAddress,u8 *pbuf,u8 Len);
}iic_eeprom_t,*iic_eeprom_pt;


extern iic_eeprom_pt iic_24c0x;
extern void init_drv_24c0x(void);

/*--------------------------------------------------------------------------------*/

//74HC595族
typedef struct{
	
	void (*enable)(void);
	void (*disable)(void);
	void (*set_bit)(int Port);
	void (*clr_bit)(int Port);
	//以下为扩展接口,用于数码管等静态刷新器件	
	void (*set_bit_but_no_out)(int Port);
	void (*clr_bit_but_no_out)(int Port);
	void (*update_out_from_drv)(void);
	void (*update_out_from_usr)(u8* buf,u8 len);
	void (*update_bus)(void);
	//	bool (*read_bit)(int Port);
}hc595_t,*hc595_pt;

/*--------------------------------------------------------------------------------*/
//drv_input_bus.c
typedef struct{
	void (*refresh)(void);
	u8 (*get_bit)(u8 pin);
}input_bus_t,*input_bus_pt;
extern input_bus_pt input_bus;
extern void init_drv_input_bus(void);

/*--------------------------------------------------------------------------------*/
//外部中断族
typedef struct{
	void (*trigger_isr)(void);
	void (*clr)(void);
	void (*enable)(void);
	void (*disable)(void);
	bool_t (*refresh_coroutine)(void);
}mq_eint_t,*mq_eint_pt;

/*--------------------------------------------------------------------------------*/
//数码管族 seg

//一位数码管的图形元素
enum _e_graph{
	_seg_bit_off=0,			/*关闭一位数码管*/
	_seg_E,							/*字母E*/
	
};

//三位数码管元素的集合组成一个图形
enum _e_seg_graph_set{
	_seg_off = 0,				//
	_seg_err_1,					//错误代码1
	_seg_err_2,					//错误代码2
};

typedef struct{
	
	void (*display)(void);
	
	void (*set_num)(u8 board,u8 ibit,u8 val);	
	void (*set_val)(u8 board,int val);
	void (*display_val)(u8 board,int val);
	
	void (*set_graph)(uint8_t board,uint8_t snum);
	void (*display_graph)(uint8_t board,uint8_t snum);
}seg_t,*seg_pt;


//typedef struct{
//	seg_pt line[4];
//}seg_bus_t,*seg_bus_pt;

extern seg_pt ex_seg_bus;
extern void init_drv_seg_bus(void);


/*--------------------------------------------------------------------------------*/
//led 操作族
typedef struct{
	void (*open)(u8 road);
	void (*close)(u8 road);	
}led_t,*led_pt;

extern led_pt exled;
extern void init_drv_ex_led(void);


/*--------------------------------------------------------------------------------*/
//mos管 操作族
typedef struct{
	void (*open)(u8 road);
	void (*close)(u8 road);	
}mos_t,*mos_pt;

//drv_io_mos_h1.c
extern mos_pt iomos;
extern void init_drv_io_mos(void);
/*--------------------------------------------------------------------------------*/
//JQ8400串行命令 音频芯片族
typedef struct{
	
	void (*cmd_set_music_file)(u8 val);
	void (*cmd_set_volume)(u8 vol);
	void (*cmd_set_gingle_cycle)(void);
	void (*cmd_set_work_mode)(void);
	void (*cmd_play_now)(void);
	void (*req_play_file)(u8 val);
	void (*deal_req_coroutine)(void);
	
}audio_t,*audio_pt;

extern audio_pt audio_0;
extern audio_pt audio_1;
extern void init_drv_audio_1st(void);
extern void init_drv_audio_2nd(void);

/*--------------------------------------------------------------------------------*/
//环形缓冲区
#define Loop_MAXBUFLEN	(256)
#if (Loop_MAXBUFLEN>512)
#error "Rx buffer must be less than 512 bytes because of head/rear data size is 8-bit unsigned char."
#endif
typedef struct{
	thread_cslock_t lock;
	int head;
	int rear;
	u8 buffer[Loop_MAXBUFLEN];
}loopbuf_t,*loopbuf_pt;

typedef struct{
	int (*read)(loopbuf_pt ploop,void *vbuf,unsigned char len);
	void (*write)(loopbuf_pt ploop,u8 dat);
	loopbuf_pt (*create)(void);
}loop_t,*loop_pt;

//drv_loop_port.c

/*--------------------------------------------------------------------------------*/
//drv_debug_usart.c
// 串口1-USART1
#define  DEBUG_USARTx                   USART1
#define  DEBUG_USART_CLK                RCC_APB2Periph_USART1
#define  DEBUG_USART_APBxClkCmd         RCC_APB2PeriphClockCmd
#define  DEBUG_USART_BAUDRATE           921600

// USART GPIO 引脚宏定义
#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  DEBUG_USART_TX_GPIO_PORT       GPIOA   
#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_9
#define  DEBUG_USART_RX_GPIO_PORT       GPIOA
#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_10

#define  DEBUG_USART_IRQ                USART1_IRQn
#define  DEBUG_USART_IRQHandler         USART1_IRQHandler


// 串口2-USART2
#if 0
#define  DEBUG_USARTx                   USART2
#define  DEBUG_USART_CLK                RCC_APB1Periph_USART2
#define  DEBUG_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  DEBUG_USART_BAUDRATE           460800

// USART GPIO 引脚宏定义
#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  DEBUG_USART_TX_GPIO_PORT       GPIOA   
#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_2
#define  DEBUG_USART_RX_GPIO_PORT       GPIOA
#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_3

#define  DEBUG_USART_IRQ                USART2_IRQn
#define  DEBUG_USART_IRQHandler         USART2_IRQHandler
#endif

typedef struct{
	void (*printf)(char* format,...);		
	void (*printf_isr)(char* format,...);
	void (*deal_coroutine)(void);
	//loopbuf_pt (*create_env)(void);
}dbg_opt_t,*dbg_opt_pt;


extern void init_drv_debug_usart_before_os_run(void);
extern void init_drv_debug_usart_after_os_run(void);
extern dbg_opt_pt dbg;

/*--------------------------------------------------------------------------------*/
//正反转电机
#define __DIR_Z_	0
#define __DIR_F_	1
typedef struct{
	u32 (*get_dir)(u8 road);
	u32 (*get_pwr)(u8 road);
	void (*set)(u8 road,u32 pwr,u32 dir);
}zfmtr_t,*zfmtr_pt;

//drv_zfmtr_1.c
extern zfmtr_pt ex_zfmtr;
extern void init_drv_ex_zfmtr(void);


/*--------------------------------------------------------------------------------*/
//adc
typedef struct{
	u16 (*get)(u8 ch);
}adc_t,*adc_pt;

extern adc_pt adc1;
extern void init_drv_adc1(void);


/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/
//HC165族
typedef struct{
	
	void (*enable)(void);
	void (*disable)(void);
	u8 (*read_byte)(void);
	void (*update_bus)(void);
	
}hc165_t,*hc165_pt;
/*--------------------------------------------------------------------------------*/
//输入总线
extern input_bus_pt ex_inputbus;
extern void init_drv_ex_inputbus(void);


/*--------------------------------------------------------------------------------*/
//
typedef struct{
	uint32_t (*read_bus)(void);
	u8 (*read_eye)(u8 road);
}eye_bus_t,*eye_bus_pt;

extern eye_bus_pt eye_input;
extern void init_drv_eye_input(void);




/*--------------------------------------------------------------------------------*/


#endif


