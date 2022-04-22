/**
�õ��⣬Ҫ������������
Ҫʵ����Щ����:
*
==============================================================
��Ŀ���£�
1�����ϵͳ��ʼ������
�رշ������ͼ̵������޹��豸
2������豸��⺯�������ȼ��LEDָʾ�ƣ�
��L1~L8һ����������������Ϩ��Ȼ��ļ�������
���������������ٴ�����Ϩ��
3��ϵͳ����ʾ00ʱ00��00�뿪ʼ
|  1    2  |   -   |  0   0  |   -  |  0   2 | 
------------------------------------------------
|  12ʱ    | ���  | 0��      |��� |  2��    |
4��L1~L4ָʾ��ΪԶ�̿����飬L7~L8Ϊ���ؿ�����
Զ��ͨ����λ�����ڷ���������ƿ��أ�����S5����L7,����S4����L8
==============================================================
*
*
*����ͨ�Ź�Լ�뿴�����ļ���Ŀ¼�µ�png
*/

#include "reg52.h"

sfr AUXR = 0x8e;

sbit S5 = P3^2;
sbit S4 = P3^3;

/*ע�ⶼ�������Ϊchar�ͱ���*/
unsigned char t_s = 0;
unsigned char t_m = 0;
unsigned char t_h = 0;
unsigned char count = 0;
unsigned char command = 0;
	
//����LED�ȵ�ǰ����״̬
unsigned char stat_led = 0xff;   

//�������������ֵ��Ϊ0ʱ��ͨ
unsigned char code SMG_duanma[18]={
		0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
		0x80,0x90,0x88,0x80,0xc6,0xc0,0x86,0x8e,
		0xbf,0x7f};
																		
void Delay(unsigned int t)
{
	while(t--);
}

/*ѡ��������*/
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

/*==========�����������ʾ����==============
*���� value--���������ʾ������
*     pos  --�����λѡ����Ҫ�����������λ��
*/
void DisplaySMG_Bit(unsigned char value,unsigned char pos)
{
	SelectHC573(6);
	P0 = 0x01 << pos;
	SelectHC573(7);
	P0 = value;
}

/*==========ϵͳ����ʱ����ʾ����============
*���� �����������ʾϵͳ���е�ʱ��
*���� ��
*/
void DisplayTime()
{
	DisplaySMG_Bit(SMG_duanma[t_s%10],7);
	Delay(600);
	DisplaySMG_Bit(SMG_duanma[t_s/10],6);
	Delay(600);
	DisplaySMG_Bit(SMG_duanma[16],5);
	//��ʾ-����
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

/*=========��ʱ������=================
*���� ����ϵͳ����ʱ����߼�����,����ʱ��T0����Ϊ16λģʽ
*���Ҽ����ʼֵΪ50ms
*���� ��
*/
void InitTimer0()
{
	TMOD = 0x21;   //��4λ��ʱ/������1ģʽΪ�����Զ���װ
								 //��4λ��ʱ/������Ϊ16λ��ʱ/������ģʽ
	TH0 = (65535 - 50000)/256;
	TL0 = (65535 - 50000)%256;
	ET0 = 1;    //ʹ�ܶ�ʱ��T0
	EA = 1;     //ʹ�����ж�
	TR0 = 1;    //������ʱ��T0
}

/*==========��ʱ��T0�жϷ�����=======
*����ϵͳʱ����߼�����
*���� ��
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
		t_m++;					/*��60���ʱ��
										���������㣬
										��������ʼ��1*/
		if(t_m == 60)
		{
			t_m = 0;
			t_h++;
		}
	}
}

/*============���ڳ�ʼ������===========
*���� �����ڳ�ʼ��Ϊģʽ1��������Ϊ9600
*�����������
*/
void InitUart()
{
	TMOD = 0x21;  //��Ҫע�⣬T0��T1��һ��ֵ��
	TH1 = 0xfd;   /*���Ĺ�ʽ
								*��ס0xfd*/
	TL1 = 0xfd;
	TR1 = 1;
	
	SCON = 0x50;
	AUXR = 0x00;
	
	ES = 1;  //ʹ�ܴ����ж�
	EA = 1;		//ʹ�����ж�
}
	
/*===========�����жϷ�����==========
*���� ������λ�������ݲ�������command������
*/
void ServiceUart() interrupt 4
{
	if(RI == 1)
	{
		command = SBUF; //�����յ������ݷ���command������
		RI = 0;	//�����ձ�־λ��0
	}
}

/*==========��Ƭ�������ֽں���===========
*���� ��������λ������һ���ֽ�
*���� dat--Ҫ���͵�����
*/
void SendByte(unsigned char dat)
{
	SBUF = dat;
	while(TI == 0);//�жϷ�����û��
	TI = 0;
}

//=========��λ���������ִ�к���=========
//���� ������λ�������ݲ�������command��
void ExecuteCommand()
{
	if(command != 0x00) 				//���յ�һ����λ������
	{
		switch(command & 0xf0)		//����������ȡ����
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

//============��������ɨ�躯��==========
//���� ɨ��S5��S4������ִ���ֳ��ƹ����
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

//============�����ƹ��⺯��==============
//���� �����⹤���ƹ�Ĺ���״̬
void CheckLED()
{
	char i;
	SelectHC573(4);
	for ( i = 0 ; i < 9 ; i++ )
	{
		stat_led = 0xfe << i;  //���ҵ���������
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

//============��������ܼ�⺯��============
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

//============��ʼ������=============
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
