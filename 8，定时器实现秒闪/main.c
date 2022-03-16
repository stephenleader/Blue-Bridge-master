#include "reg52.h"

//ע������
//typedef ���� ������
//#define ������ ����
#define u16 unsigned int;
typedef unsigned char u8;

sbit L1 = P0^0;
sbit L8 = P0^7;

sbit HC573_C = P2^7;
sbit HC573_B = P2^6;
sbit HC573_A = P2^5;

void SelectHC573(u8 n)
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
			HC573_A =1;
		break;
	}
}

void InitTimer0()//��ʼ����ʱ��0
{
	SelectHC573(5);
	P0=0x00;
	SelectHC573(4);
	P0=0xff;
	
	//TMOD��ʱ/���������ƼĴ���
	TMOD = 0x01;
	TH0 = (65535 - 50000) / 256;
	TL0 = (65535 - 50000) % 256;
	
	ET0 = 1;//ѡ��򿪶�ʱ��0�жϣ�ET0����ET1��1
	EA = 1;//�����жϿ���
	TR0 = 1;//�򿪶�ʱ/������T0����������TR0����TR1��1
}

u8 count = 0;
u8 count1 = 0;

void ServiceTimer0() interrupt 1
{
	//�������ó�ֵ
	TH0 = (65535 - 50000) / 256;
	TL0 = (65535 - 50000) % 256;
	
	count++;
	count1++;
	
	if(count % 10 == 0 )//L1:10X5ms��һ��
	{
		L1=~L1;
		count = 0;
	}
	if(count1 == 100)//L8:100X5ms��һ��
	{
		L8 = ~L8;
		count1 = 0;
	}
}
void main()
{
	InitTimer0();
	while(1)
	{
		
	}
}