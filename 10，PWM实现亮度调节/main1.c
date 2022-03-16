/*
占空比30%，
表示高电平占比百分之30，
低电平占比百分之70
，通常占空比用变量duty表示
*/

#include "reg52.h"

sbit L1 = P0^0;
sbit S7 = P3^0;
sbit L2 = P14^5;

sbit HC573_C = P2^7;
sbit HC573_B = P2^6;
sbit HC573_A = P2^5;

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
	}	
}

void Delay(unsigned int t)
{
			while(t--);
}

//计数变量
unsigned char count = 0;//计数个数
unsigned char pwm_duty = 0;//占空比
//============定时器初始化函数==========
void InitTimer0()
{
	TMOD = 0x01;//通常都是像这样
	//10ms = 1000us, 1ms(每个时钟脉冲)=100us * 100
	TH0 = (65535 - 100)/256;//延时100个微秒的定时中断
	TL0 = (65535 - 100)%256;
	
	ET0 = 1;
	EA = 1;
}
//============定时器服务函数=============
void ServiceTimer0() interrupt 1
{
	//重装写进来
	TH0 = (65535 - 100)/256;//延时100个微秒的定时中断
	TL0 = (65535 - 100)%256;
	
	count++;
	if(count == pwm_duty)//在占空比之前置1
	{
		L1 = 1;
	}
	else if(count == 100)//到100之前置0
	{
		L1 = 0;
		count = 0;
	}
}

//设置一个变量，表示当前按键处在什么状态
unsigned char stat = 0;

void ScanKey()
{
		if(S7 == 0)
		{
			Delay(100);//延时消抖
			if(S7 == 0)
			{
				switch(stat)
				{
					case 0:
						L1 = 0;
						stat = 1;
						TR0 = 1;//这里才开启定时器
						pwm_duty = 10;//10%的亮灯状态,占空比改成10%
					break;
						
					case 1:
						pwm_duty = 50;
						stat = 2;
					break;
					
					case 2:
						pwm_duty = 90;
						stat = 3;
					break;
					
					case 3:
						L1 = 1;//熄灭
						TR0 = 0;
						stat = 0;
					break;
				}
				while(S7== 0);//避免反复读取按键
			}		
		}
}

void InitSystem()
{
	SelectHC573(5);
	P0 = 0x00;
	SelectHC573(4);
	P0 = 0xff;
}

void main()
{
	InitSystem();
	InitTimer0();
	while(1)
	{
		ScanKey();
	}
}