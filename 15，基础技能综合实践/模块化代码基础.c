/**
拿到题，要分析他的需求
要实现哪些功能:
*
==============================================================
题目如下：
1，设计系统初始函数，
关闭蜂鸣器和继电器等无关设备
2，设计设备检测函数，首先检测LED指示灯，
从L1~L8一次逐个点亮，再逐个熄灭；然后的检查数码管
，从左到右依次亮再从左到右熄灭
3，系统上显示00时00分00秒开始
|  1    2  |   -   |  0   0  |   -  |  0   2 | 
------------------------------------------------
|  12时    | 间隔  | 0分      |间隔 |  2秒    |
4，L1~L4指示灯为远程控制组，L7~L8为本地控制组
远程通过上位机串口发送命令控制开关，本地S5控制L7,按键S4控制L8
==============================================================
*
*
*此外通信规约请看：该文件根目录下的png
*/

#include "reg52.h"

sfr AUXR = 0x8e;

sbit S5 = P3^2;
sbit S4 = P3^3;

/*注意都最好设置为char型变量*/
unsigned char t_s = 0;
unsigned char t_m = 0;
unsigned char t_h = 0;
unsigned char count = 0;
unsigned char command = 0;
	
//定义LED等当前开关状态
unsigned char stat_led = 0xff;   

//共阳极数码管码值，为0时导通
unsigned char code SMG_duanma[18]={
		0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
		0x80,0x90,0x88,0x80,0xc6,0xc0,0x86,0x8e,
		0xbf,0x7f};
																		
void Delay(unsigned int t)
{
	while(t--);
}

/*选择锁存器*/
void SelectHC573(unsigned channel)
{
	switch(channel)
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
		
		case 0:
			P2 = (P2 & 0x1f) | 0x00;
		break;
	}
}

/*==========单个数码管显示函数==============
*参数 value--数码管上显示的内容
*     pos  --数码管位选。即要点亮的数码管位置
*/
void DisplaySMG_Bit(unsigned char value,unsigned char pos)
{
	SelectHC573(6);
	P0 = 0x01 << pos;
	SelectHC573(7);
	P0 = value;
}

/*==========系统运行时间显示函数============
*功能 在数码管上显示系统运行的时间
*参数 无
*/
void DisplayTime()
{
	DisplaySMG_Bit(SMG_duanma[t_s%10],7);
	Delay(600);
	DisplaySMG_Bit(SMG_duanma[t_s/10],6);
	Delay(600);
	DisplaySMG_Bit(SMG_duanma[16],5);
	//表示-符号
	Delay(600);
	
	DisplaySMG_Bit(SMG_duanma[t_m%10],4);
	Delay(600);
	DisplaySMG_Bit(SMG_duanma[t_m/10],3);
	Delay(600);
	DisplaySMG_Bit(SMG_duanma[16],2);
	Delay(600);
	
	DisplaySMG_Bit(SMG_duanma[t_h%10],1);
	Delay(600);
	DisplaySMG_Bit(SMG_duanma[t_h/10],0);
	Delay(600);
}

/*=========定时器函数=================
*功能 进行系统运行时间的逻辑处理,将定时器T0设置为16位模式
*，且计算初始值为50ms
*参数 无
*/
void InitTimer0()
{
	TMOD = 0x21;   //高4位定时/计数器1模式为可以自动重装
								 //低4位定时/计数器为16位定时/计数器模式
	TH0 = (65535 - 50000)/256;
	TL0 = (65535 - 50000)%256;
	ET0 = 1;    //使能定时器T0
	EA = 1;     //使能总中断
	TR0 = 1;    //启动定时器T0
}

/*==========定时器T0中断服务函数=======
*进行系统时间的逻辑处理
*参数 无
*/
void ServiceTimer0() interrupt 1
{
	TH0 = (65535 - 50000)/256;
	TL0 = (65535 - 50000)%256;
	
	count++;
	if(count == 20)
	{
		count = 0;
		t_s++;
	}
	if(t_s == 60)
	{
		t_s = 0;	
		t_m++;					/*到60秒的时候
										，秒数清零，
										分钟数开始加1*/
		if(t_m == 60)
		{
			t_m = 0;
			t_h++;
		}
	}
}

/*============串口初始化函数===========
*功能 将串口初始化为模式1，波特率为9600
*，且允许接收
*/
void InitUart()
{
	TMOD = 0x21;  //需要注意，T0和T1是一起赋值的
	TH1 = 0xfd;   /*背的公式
								*记住0xfd*/
	TL1 = 0xfd;
	TR1 = 1;
	
	SCON = 0x50;
	AUXR = 0x00;
	
	ES = 1;  //使能串口中断
	EA = 1;		//使能总中断
}
	
/*===========串口中断服务函数==========
*功能 接收上位机的数据并保存在command变量中
*/
void ServiceUart() interrupt 4
{
	if(RI == 1)
	{
		command = SBUF; //将接收到的数据放在command变量中
		RI = 0;	//将接收标志位清0
	}
}

/*==========单片机发送字节函数===========
*功能 串口向上位机发送一个字节
*参数 dat--要发送的内容
*/
void SendByte(unsigned char dat)
{
	SBUF = dat;
	while(TI == 0);//判断发送了没有
	TI = 0;
}

//=========上位机命令解析执行函数=========
//参数 接受上位机的数据并保存在command中
void ExecuteCommand()
{
	if(command != 0x00) 				//接收到一个上位机命令
	{
		switch(command & 0xf0)		//将命令类型取出来
		{
			case 0xa0:
				SelectHC573(4);
				stat_led = (stat_led | 0x0f) & ~(-command | 0xf0);
				P0 = stat_led;
				SelectHC573(0);
				command = 0x00;
			break;
			
			case 0xb0:
				SendByte(( t_h / 10 << 4 ) | ( t_h % 10));
				SendByte(( t_m / 10 << 4 ) | ( t_m % 10));
				SendByte(( t_s / 10 << 4 ) | ( t_s % 10));
				command = 0x00;
			break;			
		}
	}
}

//============独立按键扫描函数==========
//功能 扫描S5和S4按键并执行现场灯光控制
void ScanKeys()
{
	if(S5 == 0)
	{
		DisplayTime();
		if(S5 == 0)
		{
			while(S5 == 0)
			{
				DisplayTime();
			}
			SelectHC573(4);
			stat_led = ( stat_led | 0x40 ) & (-stat_led | 0xbf);
			P0 = stat_led;
			SelectHC573(0);
		}
	}
	
	if(S4 == 0)
	{
		DisplayTime();
		if(S4 == 0)
		{
			while(S4 == 0)
			{
				DisplayTime();
			}
			SelectHC573(4);
			stat_led = ( stat_led | 0x80 ) & (-stat_led | 0x7f);
			P0 = stat_led;
			SelectHC573(0);
		}
	}
}

//============工厂灯光检测函数==============
//功能 逐个检测工厂灯光的工作状态
void CheckLED()
{
	char i;
	SelectHC573(4);
	for ( i = 0 ; i < 9 ; i++ )
	{
		stat_led = 0xfe << i;  //从右到左逐个检查
		P0 = stat_led;
		Delay(60000);
	}
	for ( i = 0 ; i < 9 ; i++ )
	{
		stat_led = ~(0xfe << i);
		P0 = stat_led;
		Delay(60000);
	}
	SelectHC573(0);
}

//============工厂数码管检测函数============
void CheckSMG()
{
	char i;
	SelectHC573(7);
	P0 = 0x00;
	for ( i = 0 ; i < 9 ; i++ )
	{
		SelectHC573(6);
		P0 = ~(0xfe << i);
		Delay(60000);
	}
	for ( i = 0 ; i < 9 ; i++ )
	{
		SelectHC573(6);
		P0 = 0xfe << i;
		Delay(60000);
	}
		SelectHC573(0);
}

//============初始化函数=============
void InitSystem()
{
	SelectHC573(5);
	P0 = 0x00;
	SelectHC573(4);
	P0 = stat_led;
	SelectHC573(0);
}

void main(void)
{
	InitSystem();
	CheckLED();
	CheckSMG();
	InitTimer0();
	InitUart();
	while(1)
	{
		ExecuteCommand();
		DisplayTime();
		ScanKeys();
	}
}
