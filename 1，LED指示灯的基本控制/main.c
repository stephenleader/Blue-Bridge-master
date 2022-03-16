#include "reg52.h"

//先把位定义上
sbit HC138_A = P2^5;
sbit HC138_B = P2^6;
sbit HC138_C = P2^7;


//函数
void Delay(unsigned int t)
{
		while(t--);
		while(t--);
}

void LEDRunning()
{
		unsigned char i;
		HC138_C = 1;
		HC138_B = 0;
		HC138_A = 0;
		
		//闪烁三下
		for(i = 0 ; i < 3 ; i++ )
		{
				P0 = 0x00;
				//0X00对于IO口来说是点亮
				Delay(60000);
				Delay(60000);

				P0 = 0xff;
				//0xff对于IO口是熄灭
				Delay(60000);
				Delay(60000);
		}
		
		//循环之后灯逐个点亮,点亮8个灯，每次移动一位
		for(i = 1 ; i <= 8 ; i++ )
		{
		//0xff灭，全灭；0x00亮；=1高电平；=0低电平
				P0 = 0xff << i;
				//流水灯一定要延时，不然看不到效果
				Delay(60000);
				Delay(60000);
		}
		
		for( i = 1 ; i <= 8 ; i++ )
		{
				P0 = ~(0xff << i);
				Delay(60000);
				Delay(60000);
		}
}

//主函数
void main()
{
		while(1)
		{
				LEDRunning();//主函数内调用函数
		}
}

