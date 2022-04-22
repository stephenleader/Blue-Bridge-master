//【1】基本原理
//     超声波发射模块向某一方向发射超声波，在发射时刻的同时开始计时，超声波在空气中传播，途中碰到障碍物就立即返回来，超声波接收器收到反射波就立即停止计时。

//【2】计算公式
//    距离 = 声速 × 发出超声波到接收返回的时间/2

//    因为超声波在空气中的传播速度受温度影响。声波在空气中的传播速度与温度的关系是：

//    在20摄氏度时，声速V = 332 + 0.607t  (m/s)

//    利用超声波进行距离测量，要求精度一般都不高，取20摄氏度时候的声速344M/S进行计算，再进行适当的补偿就可以。所以，其计算公式为：距离L = 344M/S * T / 2 = 172M/S * T 。

//【3】实现步骤
//  1-产生8个40KHz的超声波信号，通过TX引脚发射出去。

//  2-启动定时器，计算计数脉冲。

//  3-等待超声波信号返回，如果接收到反射回来的信号，RX引脚变为低电平。

//  4-停止定时器，读取脉冲个数，即获得时间T。

//  5-根据公式，L = V * T /2m，进行距离的计算。

//【4】蓝桥杯训练样例
//    在蓝桥杯“单片机设计与开发”比赛中，超声波测距是一直以来都是国赛中重要的考点，几乎每届必考。虽然省赛还没有考过，但也不能排除以后不考哦。很多同学会觉得超声波测距很难，原因有二，一是它和其他的外设模块不同，没有底层驱动代码提供；二是同学们不了解其工作原理与实现步骤。其实，超声波测距一点都不难，希望小蜜蜂老师的文章能够帮到你。

//    在蓝桥杯单片机的竞赛综合平台CT107D中，超声波模块的TX引脚接到单片机的P1.0端口，RX引脚接到单片机的P1.1端口。设计程序，进行超声波测距，最大测量范围约140cm。测量结果用cm作为显示单位，显示在数码管的最右边三位。如果测量距离超出范围，则以“F”作为标志，显示在数码管的最左边一位。
#include "reg52.h"
#include "absacc.h"
#include "intrins.h"
 
sbit TX = P1^0;
sbit RX = P1^1;
 
unsigned int distance = 0;
 
unsigned char code SMG_duanma[18]=
		{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,
		 0x88,0x80,0xc6,0xc0,0x86,0x8e,0xbf,0x7f};
 
void DelaySMG(unsigned int t)
{
	while(t--);
}
 
void DisplaySMG_Bit(unsigned char pos, unsigned char value)
{
	XBYTE[0xE000] = 0xFF;
	XBYTE[0xC000] = 0x01 << pos;
	XBYTE[0xE000] = value;
}
 
void Display_Distance()
{
	if(distance == 999)
	{
		DisplaySMG_Bit(0, SMG_duanma[15]);			//超出测量范围标志：F
		DelaySMG(500);
	}
	else
	{
		DisplaySMG_Bit(5, SMG_duanma[distance / 100]);
		DelaySMG(500);
		DisplaySMG_Bit(6, SMG_duanma[(distance % 100) / 10]);
		DelaySMG(500);
		DisplaySMG_Bit(7, SMG_duanma[distance % 10]);
		DelaySMG(500);
	}
}
 
void Delay12us()                  //@12.000MHz 延时12us
{
	unsigned char i;
 
	_nop_();
	_nop_();
	i = 33;
	while (--i);
}
 
void Send_Wave()                 //产生8个40KHx超声波信号
{
	unsigned char i;
	for(i = 0; i < 8; i++)
	{
		TX = 1;
		Delay12us();	
		TX = 0;
		Delay12us();
	}
}
 
void Measure_Distance()	        //超声波测距
{
	unsigned int time = 0;
	
	TMOD &= 0x0f;	            //定时器1模式0，13位，最大8192个计数脉冲								
	TL1 = 0x00;										
	TH1 = 0x00;		
	
	Send_Wave();		        //发送超声波信号							
	TR1 = 1;            //启动定时器						
	while((RX == 1) && (TF1 == 0));    //等待超声波信号返回或者等到测量超出范围
	TR1 = 0;            //停止定时器				
	
	if(TF1 == 0)	            //正常测量范围							
	{
		time = TH1;									
		time = (time << 8) | TL1;		
		distance = ((time / 10) * 17) / 100 + 3;
	}
	else                        //超出测量范围			
	{
		TF1 = 0;
		distance = 999;
	}
}
 
void Delay(unsigned char n)        //数码管显示增强
{
	while(n--)
	{
		Display_Distance();
	}
}
 
void main()
{
	while(1)
	{
		Measure_Distance();
		Delay(10);
	}
}