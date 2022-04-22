/**
*对于存储器映射拓展技术应用
*CT107D平台的J13要将1-2脚短接
*引入“absacc.h"头文件，
*通过XBYTE关键字来直接操作扩展资源
*存储器映射扩展方式要占用单片机的P3.6引脚
*C语言中的XBYTE对应汇编中的MOV,MOVX
*
*
*/
#include "reg52.h"
#include "absacc.h"

void Delay(unsigned int t)
{
		while(t--);
		while(t--);
		while(t--);
}

void SelectHC573(unsigned char n)
{
		switch(n)
		{
			case 4:
				P2 = (P2 & 0x1f) | 0x80;
			break;
			
			case 5:
				P2 = (P2 & 0x1f) | 0xa0;
			break;
			
			case 6:
				P2 = (P2 & 0x1f) | 0xc0;
			break;
			
			case 7:
				P2 = (P2 & 0x1f) | 0xe0;
			break;
		}
}

/**
注意外部资源的地址映射关系
|   0x8000----指示灯
|   0xa000----蜂鸣器和继电器
|   0xc000----数码管位选
|   0xe000----数码管段码
*/

void LEDRunnning()
{
	//T0口操作
	XBYTE[0x8000]=0xf0;
	Delay(60000);
	Delay(60000);
	XBYTE[0x8000]=0x0f;  //高四位点亮
	Delay(60000);
	Delay(60000);
	XBYTE[0x8000]=0xff; 
	Delay(60000);
	Delay(60000);
}

void SMGRunnning()
{
	unsigned char i;
	for ( i = 0 ; i < 8 ; i++ )
	{
		//数码管位选
		XBYTE[0xc000]=0x01 <<i;
		//段选
		XBYTE[0xe000]=0x00;
		Delay(60000);
		Delay(60000);
	}
	XBYTE[0xc000]=0xff;
	Delay(60000);
	Delay(60000);
}

void InitSystem()
{
	SelectHC573(5);
	P0=0x00;
}

void main(void)
{
	InitSystem();
	while(1)
	{
		LEDRunnning();
		SMGRunnning();
	}
}
