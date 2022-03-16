#include "reg52.h"

sbit HC573_C = P2^7;
sbit HC573_B = P2^6;
sbit HC573_A = P2^5;

sbit S4 = P3^3;
sbit S5 = P3^2;

//����
unsigned char t_fen = 0;

//��
unsigned char t_m = 0;
	
//����
unsigned char t_005ms = 0;

//�������ֵ
unsigned char code SMG_NoDot[18] = 
{
	0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
	0x80,0x90,0x88,0x80,0xc6,0xc0,0x86,0x8e,
	0xbf,0x7f
};

//������ʱ
void DelayKey(unsigned int t)
{
	while(t--);
}
//�������ʱ
void Delay(unsigned int t)
{
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
	P0 = 0x00;
	SelectHC573(4);
	P0 = 0xff;
}

void DisplaySMG_Bit(unsigned char value,unsigned char pos)
{
	SelectHC573(6);
	P0 = 0x01 << pos;//���Ʒ���
	SelectHC573(7);
	P0 = value;
}

//ʱ����ʾ
void DisplayTime()
{
	//�����
	DisplaySMG_Bit(SMG_NoDot[t_005ms % 10],7);
	Delay(500);
	DisplaySMG_Bit(SMG_NoDot[t_005ms / 10],6);
	Delay(500);
	//�������ʾ�ָ���
	DisplaySMG_Bit(SMG_NoDot[16],5);
	Delay(500);
	
	
	//����ʾ
	DisplaySMG_Bit(SMG_NoDot[t_m % 10],4);
	Delay(500);
	DisplaySMG_Bit(SMG_NoDot[t_m / 10],3);
	Delay(500);//500�պ�һ��
	//�������ʾ�ָ���
	DisplaySMG_Bit(SMG_NoDot[16],2);
	Delay(500);
	
	//������ʾ
	DisplaySMG_Bit(SMG_NoDot[t_fen % 10],1);
	Delay(500);
	DisplaySMG_Bit(SMG_NoDot[t_fen / 10],0);
	Delay(500);
}

//��ʱ������
void InitTimer0()
{
		TMOD = 0x01;
	//50msΪ�����ʱ
	TH0 = (65535 - 50000) / 256;
	TL0 = (65535 - 50000) % 256;
	
	ET0 = 1;
	EA = 1;
	TR0 = 1;
}
//
void ServiceTimer0() interrupt 1
{
	TH0 = (65535 - 50000) / 256;
	TL0 = (65535 - 50000) % 256;
	
	t_005ms++;
	if(t_005ms == 20) //20x50=1000ms=1s
	{
		t_m++;
		t_005ms = 0;
	}
	if(t_m == 60)
	{
		t_fen++;
		t_m = 0;
	}
	if(t_fen == 99)//100 cannot be displayed
	{
		t_fen = 0;
	}
}

void ScanKeys()
{
	if(S4 == 0 )  //�����������ͣ
	{
		DelayKey(100);
		if(S4 == 0)
		{
			TR0 = ~TR0;//ȡ����������
		}
	}
	
	if(S5 == 0)  //����ʾ������
	{
		DelayKey(100);
		if(S5 == 0)
		{
			t_fen= 0;
			t_005ms = 0;
			t_m = 0;
			
		}
	}
}

void main()
{
	InitSystem();
	InitTimer0();//��ʱ������Ҫ�ŵ�������������������
	while(1)
	{
		DisplayTime();
		ScanKeys();//ʹ�ð���
	}
}