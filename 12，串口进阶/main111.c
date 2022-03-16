#include "reg52.h"

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
	P0 = 0x00;
	SelectHC573(4);
	P0 = 0xff;
}

//======中断初始=====
void InitUart()
{
	TMOD = 0X20;
	TH1 = 0Xfd;
	TL1 = 0xfd;
	TR1= 1;
	
	//配置串口工作模式
	SCON = 0x50;//允许接受
	
	ES = 1;
	EA = 1;
}

//======中断服务=====
void ServiceUart() interrupt 4
{
	if(RI == 1)
	{
		
	}
}

//send Byte(zijie), not Bit(wei)
void SendByte(unsigned char dat)
{
	SBUF = dat;
	while(TI == 0);
	TI = 0;
}

void SendString(unsigned char *str)
{
	while(*str != '\0')
	{
	  SendByte(*str++);    //字符串发送
	}
}
void Delay(unsigned int t)
{
	while(t--);
}

//====main======
void main()
{
	InitSystem();
	InitUart();
	Delay(100);
	SendString("Welcome!!!my friend");
	while(1)
	{
		
	}
}