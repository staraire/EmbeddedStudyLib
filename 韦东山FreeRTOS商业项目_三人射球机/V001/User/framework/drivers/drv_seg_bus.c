#include "drv_opt.h"
#include "drv_seg_graph_lib.h"
/*--------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------*/
extern hc595_pt hc595_seg_bus;					//对外接口
extern void init_drv_hc595_seg_bus(void);

/*--------------------------------------------------------------------------------*/
static seg_t do_seg;
static seg_pt seg;

static thread_cslock_t cslock;


/*--------------------------------------------------------------------------------*/
//显示数字操作
//设置数字，得到字模
#define SegNum 27	//3*3*3
static u8 Font[SegNum];
static u8 backFont[SegNum];
static void set_num_bit(u8 board,u8 bit,char val)
{
	u8 font = 0;
	//val取值 0-9
	switch(val)
	{
		case 0:font = 0XC0;break;// 1100 0000
		case 1:font = 0XF9;break;// 1111 1001
		case 2:font = 0XA4;break;
		case 3:font = 0XB0;break;
		case 4:font = 0X99;break;
		case 5:font = 0X92;break;
		case 6:font = 0X82;break;
		case 7:font = 0XF8;break;
		case 8:font = 0X80;break;
		case 9:font = 0X98;break;
		default:break;
	}
	Font[board*3+bit ] = font;
}
static void set_num(u8 board,u8 ibit,u8 val)
{
//	thread_cslock_lock(cslock,MaxTick);
	switch(ibit)
	{
		case 0:set_num_bit(board,0,val);break;
		case 1:set_num_bit(board,1,val);break;
		case 2:set_num_bit(board,2,val);break;
		
		default:break;
	}
//	thread_cslock_free(cslock);
}


static void display(void)
{
	//thread_cslock_lock(cslock,MaxTick);
	hc595_seg_bus->update_out_from_usr(Font,SegNum);
	hc595_seg_bus->update_bus();
	//thread_cslock_free(cslock);
}



static void set_val(u8 board,int val)
{
	u8 ibai = val/100;//取百位
	u8 ishi = val/10%10;//取10位
	u8 ige  = val%10;//取个位
	
	set_num(board,0,ibai);
	set_num(board,1,ishi );
	set_num(board,2,ige );
}

static void display_val(u8 board,int val)
{
	set_val( board,val );
	
	
	display();
//	dbg->printf("display_val,b->%d,v->%d \r\n",board,val);
}


static void set_graph_bit(uint8_t board,uint8_t ibit,uint8_t snum)
{
//	thread_cslock_lock(cslock,MaxTick);

	uint8_t font = 0;
	
	switch(snum)
	{
		case _seg_bit_off:font = 0XFF;break;// 1111
		case _seg_E:font = 0x86;break;	//1000 0110
		default:break;
	}
	Font[board*3+ibit ] = font;

	
//	thread_cslock_free(cslock);
}

static void set_graph(uint8_t board,uint8_t snum)
{
	switch( snum )
	{
		case _seg_off:
			set_graph_bit( board,0,_seg_bit_off );
			set_graph_bit( board,1,_seg_bit_off );
			set_graph_bit( board,2,_seg_bit_off );
			break;
		
		case _seg_err_1:
			set_graph_bit( board,0,_seg_E );
			set_num_bit  ( board,1,0);
			set_num_bit  ( board,2,1);
			break;
		case _seg_err_2:
			set_graph_bit( board,0,_seg_E );
			set_num_bit  ( board,1,0);
			set_num_bit  ( board,2,2);
			break;
		
	}
}

static void display_graph(uint8_t board,uint8_t snum)
{
	
	set_graph( board,snum );
	display();
}


/*--------------------------------------------------------------------------------*/
seg_pt ex_seg_bus;

void init_drv_seg_bus(void)
{
	
	init_drv_hc595_seg_bus();
	seg = &do_seg;
	seg->display = display;
	
	seg->set_num = set_num;
	seg->set_val = set_val;
	seg->display_val = display_val;
	
	seg->set_graph = set_graph;
	seg->display_graph = display_graph;
	cslock = thread_cslock_init("drv_seg1");
	
	//hc595_seg_bus->clr_bit(10);
	
	
	/*接口外放*/
	ex_seg_bus = seg;
	
	ex_seg_bus->display_val( 0,100 );
	ex_seg_bus->display_val( 1,101 );
	ex_seg_bus->display_val( 2,102 );
	ex_seg_bus->display_val( 3,103 );
	ex_seg_bus->display_val( 4,104 );
	ex_seg_bus->display_val( 5,105 );
	ex_seg_bus->display_val( 6,106 );
	ex_seg_bus->display_val( 7,107 );
	ex_seg_bus->display_val( 8,108 );
}
/*--------------------------------------------------------------------------------*/




