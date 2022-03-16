#include <reg52.h>

#define SEG P0;
#define KEYPAD P1;

typedef unsigned char u8;
typedef unsigned int u16;

void delay_ms(u16 x);//延时函数声明
void led_display(u8 number);//数码管显示，注意格式使用字符型
u8 keypad_scan(void);
//函数一般都有返回值，void什么都不返回，这里我们要返回一个u8
sbit key = P2^0;
sbit led = P3^0;


//重要*
void main()
{
		u8 num;
		while(1)
		{
				switch(keypad_scan())//这里扫描矩阵键盘
				//当number为0的按键，即k0按下时
				//，num=0
				{
				case 0xee:num = 0; break;
				case 0xed:num = 1; break;
				case 0xeb:num = 2; break;
				case 0xe7:num = 3; break;
				default: break;
				}
}


void delay_ms(u16 x)
{
	u16 i,j;
	for ( i = x ; i > 0 ; i-- )
	{
		for (j = 110 ; j > 0 ; j-- )
		{
		
		}
	}
}
void led_display(u8 number)
{
		SEG = seg_tab[number];
}