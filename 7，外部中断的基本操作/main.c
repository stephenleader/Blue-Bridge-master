#include "reg52.h"

sbit L1 = P0^0;
sbit L8 = P0^7;
sbit S5 = P3^2;

sbit HC573_C = P2^7;
sbit HC573_B = P2^6;
sbit HC573_A = P2^5;

void Delay_ms(unsigned int t);
void SelectHC573(unsigned char n);

void Working()
{
	SelectHC573(4);
	L1 = 0;
	Delay_ms(60000);
	L1 = 1;
	Delay_ms(60000);
}

void Delay_ms(unsigned int t)
{
	while(t--);
	while(t--);
}

void SelectHC573(unsigned char n)
{
		switch(n)
		{
		case 4:
				HC573_C = 1;
				HC573_B = 0;
				HC573_A = 0;
				break;
		case 5:
				HC573_C = 1;
				HC573_B = 0;
				HC573_A = 1;
				break;
		case 6:
				HC573_C = 1;
				HC573_B = 1;
				HC573_A = 0;
				break;
		case 7:
				HC573_C = 1;
				HC573_B = 1;
				HC573_A = 1;
				break;
		}
}

void InitSystem()
{
	SelectHC573(5);
	P0=0x00;
	SelectHC573(4);
	P0=0xff;
}

//------------中断-------------
void Init_INT0()//初始化触发方式
{
	IT0 = 1;//=1选择下降沿触发
	EX0 = 1;//打开IE寄存器的定时器0
	EA = 1;//打开IE定时寄存器的总开关
}
//------------中断-------------
//中断服务函数
//外部中断号为0
void ServiceINT0() interrupt 0
{
	L8 = 0;
	Delay_ms(60000);
	Delay_ms(60000);
	Delay_ms(60000);
	Delay_ms(60000);
	Delay_ms(60000);
	Delay_ms(60000);
	L8 = 1;
}


void main()
{
	InitSystem();
	Init_INT0();
	while(1)
	{
		Working();
	}
}