#include "reg52.h"
#include "absacc.h"
#include "xmf_smg_ca.h"

//通过定时计数器0,1对频率信号进行计数,从而达到测量频率的作用
unsigned int cunt_f = 0;
unsigned int dat_f = 0;
unsigned char count_f = 0;

void Init_Timer()
{
	//定时器0用作计数，8位重装方式
	TH0 = 0xff;//255
	TL0 = 0xff;//255
	
	//定时器1用作定时，定时时间是50ms
	TH0 = (65535 - 50000) / 256;
	TL0 = (65535 - 50000) % 256;
	
	TMOD = 0x16;//最大65535，自动重装功能
	
	ET0 = 1;
	ET1 = 1;
	EA = 1;
	TR0 = 1;
	TR1 = 1;
}

void Serive_T0() interrupt 1
{
	count_f++;
}

void Serive_T1() interrupt 3//计算脉冲个数
{
	TH0 = (65535 - 50000) / 256;
	TL0 = (65535 - 50000) % 256;
	
	count_f++;
	if(count_f == 20)//50.000ms x 20 = 1s
	{
		dat_f = count_f;
		count_f = 0;
		count_f = 0;
		
	}
}
void DisPlay_SMG()
{
	DisplaySMG_Bit(0, 0x8e); //显示第一位
	//0000 1110共阳极数码管
	DelaySMG(100);
	
	DisplaySMG_Bit(1, 0xff); //显示第一位
	//0000 1110共阳极数码管
	DelaySMG(100);
	
	DisplaySMG_Bit(2, 0xff); //显示第一位
	//0000 1110共阳极数码管
	DelaySMG(100);
	
	if(dat_f > 9999 )//第五位
	{
		DisplaySMG_Bit(3,SMGNoDot_CA[dat_f / 10000]);
		DelaySMG(100);
	}
	
		if(dat_f > 999 )//第四位
	{
		DisplaySMG_Bit(4,SMGNoDot_CA[(dat_f / 1000) % 10]);
		DelaySMG(100);
	}
	
		if(dat_f > 99 )//第三位
	{
		DisplaySMG_Bit(5,SMGNoDot_CA[(dat_f / 100) % 10]);
		DelaySMG(100);
	}
	
		if(dat_f > 9 )//第二位
	{
		DisplaySMG_Bit(6,SMGNoDot_CA[(dat_f / 10) % 10]);
		DelaySMG(100);
	}
	
		if(dat_f > 0 )//第一位
	{
		DisplaySMG_Bit(7,SMGNoDot_CA[(dat_f / 1) % 10]);
		DelaySMG(100);
	}
	Display_All(0xff);//消隐
}

void main()
{
	XBYTE[0x8000] = 0xff;//对应8个指示灯
	XBYTE[0xa000] = 0x00;//寄存器赋0,关掉
	Init_Timer();
	while(1)
	{
		DisPlay_SMG();
	}
}