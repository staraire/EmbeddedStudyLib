#include "fk_soft_typedef.h"
static u8 seg_num_font[] = {
	0XC0,0XF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X98
};


//根据数字返回数码管字模
//num 范围:0-9
u8 seg_get_num_font(u8 num )
{
	return seg_num_font[num];
}

//修饰数码管的显示内容为字模
//num:想要显示的数字值，范围0-99
//one:需要修饰的个位字模
//ten:需要修饰的十位字模
void seg_set_num_to_font(int num,u8 *one,u8 *ten)
{
	u8 val = num;
	u8 iOne = 0;
	u8 iTen = 0;
	
	//大小限制
	if( val>100) val = val%100;
	iOne = val%10;
	iTen = val%100/10;
	
	*one = seg_get_num_font( iOne );
	*ten = seg_get_num_font( iTen );
}




