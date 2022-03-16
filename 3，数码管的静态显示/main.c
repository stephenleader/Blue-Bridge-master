#include "reg52.h"

sbit HC138_C = P2^7;
sbit HC138_B = P2^6;
sbit HC138_A = P2^5;

/*LED模块一共有八个二极管，连接的是P2口，
所以这里通过十六进制数和左移右移函数结合，
来实现LED的流水效果。
默认情况下P2 = 0xff，对应二进制数为1111 1111，
此时LED模块中所有的灯都处于熄灭状态，
这八个数字对应八个灯，最低位对应D1,最高位对应D8。
如：P2 = 0xd0，即1101 0000，对应LED模块中，
D1、D2、D3、D4、D6亮，D5、D7、D8不亮。*/

/*即对于led：0是亮，1是不亮。*/

/*但是对于接口（看实际情况，
大多数寄存器、继电器之类的
上的接口已经有取非，0就是通电，1就是通电。*/

/*同时，注意对于138这种译码器，
y4输出低电平，即为CBA=10。*/


//函数
unsigned char code SMG_duanma[18] = 
{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
0x80,0x90,0x88,0x80,0xc6,0xc0,0x86,0x8e,
0xbf,0x7f};

void InitHC138(unsigned char n)
{
		switch(n)
		{
		case 4:
				HC138_C = 1;
				HC138_B = 0;
				HC138_A = 0;
				break;
		case 5:
				HC138_C = 1;
				HC138_B = 0;
				HC138_A = 1;
				break;
		case 6:
				HC138_C = 1;
				HC138_B = 1;
				HC138_A = 0;
				break;
		case 7:
				HC138_C = 1;
				HC138_B = 1;
				HC138_A = 1;
				break;
		}
}

void ShowSMG_Bit(unsigned char dat,unsigned pos)
{
		InitHC138(6);   //数码管的位置
		P0= 0x01 << pos;     //一般都是右移多少位
		InitHC138(7);    //数码管的内容
		P0 = dat;    //0x82对应码值的6
}

void Delay_ms(unsigned int t)
{
		while(t--);
		while(t--);
}
//关闭无关设备,包括蜂鸣器和继电器
void InitSystem()
{
		HC138_C = 1;
		HC138_B = 0;
		HC138_A = 1;
		
		P0 = 0X00;
	
}

void SMG_Static()
{
		unsigned char i,j;
		for ( i = 0 ; i < 8 ; i++ )
		{
		Delay_ms(60000);
				for ( j= 0 ; j < 16 ;j++ )
				{
				ShowSMG_Bit(SMG_duanma[j],i);
				Delay_ms(60000);
				Delay_ms(60000);
				Delay_ms(60000);//单个地方显示0到9；再到其他地方
				}
				
		}
}

//主函数
void main()
{
		InitSystem();
		while(1)
		{
			SMG_Static();
		}
		
}