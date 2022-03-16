#include "reg52.h"

//寄存器定义,新增辅助寄存器
sfr AUXR = 0x8e;

unsigned char urdat;

void SendByte(unsigned char dat);

void InitUart()//串口初始化，SM1,SM2采用模式二
{
	//产品手册上有,用定时器1产生波特率
	TMOD = 0x20;
	TH1 = 0xfd; //大小写均可
	TL0 = 0xfd;
	TR1 = 1;
	
	SCON = 0x50; //记住就行
	AUXR = 0x00; //B0 = 0时，独立波特率发生器才能每12个时钟计数一次
	
	ES = 1;
	EA = 1;
}

//4号为串口中断,0号是外部中断0，1号是定时器0，2号是外部中断1，3号是定时器1
void ServiceUart() interrupt 1
{
	if(RI == 1)//处理数据接收情况
	{
		RI = 0;//必须人工清零
		urdat = SBUF;//读出SBUF
		SendByte(urdat);
	}
}

//发送字节函数，用查询方式
void SendByte(unsigned char dat)
{
	SBUF = dat;
	while(TI == 0);//发送完变成1
	TI = 0;//手动清零
}

void main()
{
	InitUart();

	while(1);
}