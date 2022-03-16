#include <reg52.h>
typedef unsigned char u8;
typedef unsigned int u16;

void delay_ms(u16 x);//延时函数声明
sbit key = P2^0;
sbit led = P3^0;



void main()
{
		while(1)
		{
				if(key == 0 )
				{
						delay_ms(100);
						if(key == 0)
						{
						
								led = 0;
						}else
						{
								led = 1;
						}
				}
		}
}

//延时函数，但进行按键的时候必须要使用延时函数进行消抖
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

