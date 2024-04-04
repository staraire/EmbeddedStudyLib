
#include "drv_opt.h"
/*--------------------------------------------------------------------------------*/
static loop_t do_loop;
static loop_pt ploop;

/*----------------------------------------------------------------------*/
//从环形缓冲区中读出数据
static int loop_read(loopbuf_pt ploop,void *vbuf,unsigned char len)
{
	int i = 0;
	char *buf = (char *)vbuf;
	
	for(i=0; i<len; i++)
	{
		if(ploop->rear == ploop->head)
		{
			break;
		}
		buf[i] = ploop->buffer[ploop->rear];
		ploop->rear ++;
		if(ploop->rear >= Loop_MAXBUFLEN)
			ploop->rear = 0;
		
	}
	
	return i;
}
/*----------------------------------------------------------------------*/

static void loop_write(loopbuf_pt ploop,u8  dat)
{
	
	//printf("%c \r\n",dat);
	if( (ploop->head+1) % Loop_MAXBUFLEN == ploop->rear )
	{
		/*TODO: Add error handling here.*/
		//debug_Assert(0,DPFL,"UART0:buffer full.");
	}
	else
	{
		ploop->buffer[ploop->head] = dat;
		ploop->head++;
		if(ploop->head >= Loop_MAXBUFLEN)
			ploop->head = 0;
	}
	
}
/*----------------------------------------------------------------------*/
static loopbuf_pt loop_create(void)
{
	loopbuf_pt tloop = (loopbuf_pt) mem_alloc( sizeof(loopbuf_t) );
	tloop->lock = thread_cslock_init("new loop");
	return tloop;
}
/*----------------------------------------------------------------------*/
loop_pt loop;
void init_drv_loop_port(void)
{
	 ploop = &do_loop;
	 ploop->create = loop_create;
	 ploop->read = loop_read;
	 ploop->write = loop_write;
	 loop = ploop;
}
/*----------------------------------------------------------------------*/


