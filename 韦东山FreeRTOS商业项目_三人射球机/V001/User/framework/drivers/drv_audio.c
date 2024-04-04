#include "drv_opt.h"

/*--------------------------------------------------------------------------------*/
//底层接口
extern void set_audio_aux(u8 val);

extern void init_audio_usart_port_1st(void);
extern void init_audio_usart_port_2nd(void);

extern void send_char_to_audio_0(uint8_t ch);
extern void send_char_to_audio_1(uint8_t ch);


/*--------------------------------------------------------------------------------*/
#define mp3Num 20				//最多能放多少首音乐
static u8 reqflag0[mp3Num];		
static u8 reqflag1[mp3Num];

static u8 buf0[6];				//cmd send buf 
static u8 buf1[6];				//cmd send buf

static audio_t do_audio_0;
static audio_t do_audio_1;
static audio_pt paudio_0;
static audio_pt paudio_1;

static thread_cslock_t cslock_0;
static thread_cslock_t cslock_1;

/*--------------------------------------------------------------------------------*/
//基础操作
static u8 sum_data(u8 *buf,u8 len)
{
	u8 ret = 0;
	for(int i=0;i<len;i++)
		ret = ret + buf[i];
	return ret;
}
static void put_cmd_to_audio_0(u8* buf,u8 len)
{
	for(int i=0;i<len;i++)
		send_char_to_audio_0(buf[i]);
}
static void put_cmd_to_audio_1(u8* buf,u8 len)
{
	for(int i=0;i<len;i++)
		send_char_to_audio_1(buf[i]);
}
/*--------------------------------------------------------------------------------*/
//选择音乐文件
static void cmd_set_music_file_audio_0(u8 val)
{
	buf0[0] = 0xAA;
	buf0[1] = 0X07;
	buf0[2] = 0x02;
	buf0[3] = val>>8;			
	buf0[4] = (u8)val;
	buf0[5] = sum_data(buf0,5);
	put_cmd_to_audio_0(buf0,6);
}

static void cmd_set_music_file_audio_1(u8 val)
{
	buf1[0] = 0xAA;
	buf1[1] = 0X07;
	buf1[2] = 0x02;
	buf1[3] = val>>8;			
	buf1[4] = (u8)val;
	buf1[5] = sum_data(buf1,5);
	put_cmd_to_audio_0(buf1,6);
}
/*--------------------------------------------------------------------------------*/
//设置音量
static void cmd_set_volume_audio_0(u8 vol)
{
	if(vol>30) vol = 30;
	buf0[0] = 0xAA;
	buf0[1] = 0X13;
	buf0[2] = 0x01;
	buf0[3] = vol;
	buf0[4] = sum_data(buf0,4);
	put_cmd_to_audio_0(buf0,5);
}
static void cmd_set_volume_audio_1(u8 vol)
{
	if(vol>30) vol = 30;
	buf1[0] = 0xAA;
	buf1[1] = 0X13;
	buf1[2] = 0x01;
	buf1[3] = vol;
	buf1[4] = sum_data(buf1,4);
	put_cmd_to_audio_1(buf1,5);
	
}
/*--------------------------------------------------------------------------------*/
//设置单曲循环
static void cmd_set_gingle_cycle_audio_0(void)
{
	buf0[0] = 0xAA;
	buf0[1] = 0x18;
	buf0[2] = 0x01;
	buf0[3] = 0x01;
	buf0[4] = 0xc4;
	put_cmd_to_audio_0(buf0,5);
}

static void cmd_set_gingle_cycle_audio_1(void)
{
	buf1[0] = 0xAA;
	buf1[1] = 0x18;
	buf1[2] = 0x01;
	buf1[3] = 0x01;
	buf1[4] = 0xc4;
	put_cmd_to_audio_0(buf1,5);
}
/*--------------------------------------------------------------------------------*/
//设置工作模式
static void cmd_set_work_mode_audio_0(void)
{
	u32 i=0;
	buf0[0] = 0xAA;
	buf0[1] = 0X0B;
	buf0[2] = 0x01;
	buf0[3] = 0X02;
	buf0[4] = 0xB8;
	put_cmd_to_audio_0(buf0,5);
	i=1000;while(i--);
	buf0[0] = 0xAA;
	buf0[1] = 0X1D;
	buf0[2] = 0x01;
	buf0[3] = 0X00;
	buf0[4] = 0xC8;
	put_cmd_to_audio_0(buf0,5);
	i=1000;while(i--);
}
static void cmd_set_work_mode_audio_1(void)
{
	u32 i=0;
	buf1[0] = 0xAA;
	buf1[1] = 0X0B;
	buf1[2] = 0x01;
	buf1[3] = 0X02;
	buf1[4] = 0xB8;
	i=1000;while(i--);
	put_cmd_to_audio_1(buf1,5);
	buf1[0] = 0xAA;
	buf1[1] = 0X1D;
	buf1[2] = 0x01;
	buf1[3] = 0X00;
	buf1[4] = 0xC8;
	put_cmd_to_audio_1(buf1,5);
	i=1000;while(i--);
}
/*--------------------------------------------------------------------------------*/
//开始播放
static void cmd_play_now_audio_0(void)
{
	buf0[0] = 0xAA;
	buf0[1] = 0x02;
	buf0[2] = 0x00;
	buf0[3] = 0xAC;
	put_cmd_to_audio_0(buf0,4);
}
static void cmd_play_now_audio_1(void)
{
	buf1[0] = 0xAA;
	buf1[1] = 0x02;
	buf1[2] = 0x00;
	buf1[3] = 0xAC;
	put_cmd_to_audio_0(buf1,4);
}

/*--------------------------------------------------------------------------------*/
//请求播放一个曲目
static void req_audio_0_play(u8 val)
{
	thread_cslock_lock(cslock_0,MaxTick);	//lock
	reqflag0[val] = 1;
	thread_cslock_free(cslock_0);			//unlock
}

static void req_audio_1_play(u8 val)
{
	thread_cslock_lock(cslock_1,MaxTick);	//lock
	reqflag1[val] = 1;
	thread_cslock_free(cslock_1);			//unlock
}


//处理请求 协程
static void deal_req_audio_0_coroutine(void)
{
	
	buf0[0] = 0xAA;
	buf0[1] = 0X07;
	buf0[2] = 0x02;
	for(int i=0;i<mp3Num;i++)
	{	
		if(reqflag0[i])
		{
			cmd_set_music_file_audio_0(i);
			reqflag0[i] = 0;
		}
		
	}
}

static void deal_req_audio_1_coroutine(void)
{
	
	buf1[0] = 0xAA;
	buf1[1] = 0X07;
	buf1[2] = 0x02;
	for(int i=0;i<mp3Num;i++)
	{	
		if(reqflag1[i])
		{
			buf1[3] = i>>8;			
			buf1[4] = (u8)i;
			buf1[5] = sum_data(buf1,5);
			put_cmd_to_audio_0(buf1,6);
			reqflag1[i] = 0;
		}
	}
}
/*--------------------------------------------------------------------------------*/

audio_pt audio_0;
audio_pt audio_1;
void init_drv_audio_1st(void)
{
	u32 i=0;
	paudio_0 = &do_audio_0;
	paudio_0->cmd_set_music_file 	= cmd_set_music_file_audio_0;
	paudio_0->cmd_set_volume		= cmd_set_volume_audio_0;
	paudio_0->cmd_set_gingle_cycle	= cmd_set_gingle_cycle_audio_0;
	paudio_0->cmd_set_work_mode		= cmd_set_work_mode_audio_0;
	paudio_0->cmd_play_now			= cmd_play_now_audio_0;
	paudio_0->req_play_file			= req_audio_0_play;
	paudio_0->deal_req_coroutine	= deal_req_audio_0_coroutine;
	
	paudio_1 = &do_audio_1;
	paudio_1->cmd_set_music_file 	= cmd_set_music_file_audio_1;
	paudio_1->cmd_set_volume		= cmd_set_volume_audio_1;
	paudio_1->cmd_set_gingle_cycle	= cmd_set_gingle_cycle_audio_1;
	paudio_1->cmd_set_work_mode		= cmd_set_work_mode_audio_1;
	paudio_1->cmd_play_now			= cmd_play_now_audio_1;
	paudio_1->req_play_file			= req_audio_1_play;
	paudio_1->deal_req_coroutine	= deal_req_audio_1_coroutine;

	/*接口外放*/
	audio_0 = paudio_0;
	audio_1 = paudio_1;
	
	//创建锁
	cslock_0 = thread_cslock_init("audio_0");
	cslock_1 = thread_cslock_init("audio_1");

	
}

void init_drv_audio_2nd(void)
{
	volatile int i = 0;
	init_audio_usart_port_1st();
#if 1
	set_audio_aux( 0 );	//选通 背景音乐
	i=1000;while(i--);
	audio_0->cmd_set_work_mode();			i=1000;while(i--);
	audio_0->cmd_set_volume(25);			i=1000;while(i--);
	audio_0->cmd_set_gingle_cycle();	i=1000;while(i--);
	//默认认为001.MP3就是背景音
	audio_0->cmd_set_music_file(1); 	i=1000;while(i--);
	audio_0->cmd_play_now();					i=1000;while(i--);
	
#endif
	
	
	init_audio_usart_port_2nd();

#if 1
	set_audio_aux( 1 );	//选通效果音
	audio_1->cmd_set_work_mode();
	//audio_1->cmd_set_music_file(1);
	//audio_1->cmd_play_now();
	audio_1->cmd_set_volume(28);
	//audio_1->cmd_set_gingle_cycle();
#endif	
}


