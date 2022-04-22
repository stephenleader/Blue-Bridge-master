#include "reg52.h"
#include "absacc.h"
#include "xmf_smg_ca.h"

//ͨ����ʱ������0,1��Ƶ���źŽ��м���,�Ӷ��ﵽ����Ƶ�ʵ�����
unsigned int cunt_f = 0;
unsigned int dat_f = 0;
unsigned char count_f = 0;

void Init_Timer()
{
	//��ʱ��0����������8λ��װ��ʽ
	TH0 = 0xff;//255
	TL0 = 0xff;//255
	
	//��ʱ��1������ʱ����ʱʱ����50ms
	TH0 = (65535 - 50000) / 256;
	TL0 = (65535 - 50000) % 256;
	
	TMOD = 0x16;//���65535���Զ���װ����
	
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

void Serive_T1() interrupt 3//�����������
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
	DisplaySMG_Bit(0, 0x8e); //��ʾ��һλ
	//0000 1110�����������
	DelaySMG(100);
	
	DisplaySMG_Bit(1, 0xff); //��ʾ��һλ
	//0000 1110�����������
	DelaySMG(100);
	
	DisplaySMG_Bit(2, 0xff); //��ʾ��һλ
	//0000 1110�����������
	DelaySMG(100);
	
	if(dat_f > 9999 )//����λ
	{
		DisplaySMG_Bit(3,SMGNoDot_CA[dat_f / 10000]);
		DelaySMG(100);
	}
	
		if(dat_f > 999 )//����λ
	{
		DisplaySMG_Bit(4,SMGNoDot_CA[(dat_f / 1000) % 10]);
		DelaySMG(100);
	}
	
		if(dat_f > 99 )//����λ
	{
		DisplaySMG_Bit(5,SMGNoDot_CA[(dat_f / 100) % 10]);
		DelaySMG(100);
	}
	
		if(dat_f > 9 )//�ڶ�λ
	{
		DisplaySMG_Bit(6,SMGNoDot_CA[(dat_f / 10) % 10]);
		DelaySMG(100);
	}
	
		if(dat_f > 0 )//��һλ
	{
		DisplaySMG_Bit(7,SMGNoDot_CA[(dat_f / 1) % 10]);
		DelaySMG(100);
	}
	Display_All(0xff);//����
}

void main()
{
	XBYTE[0x8000] = 0xff;//��Ӧ8��ָʾ��
	XBYTE[0xa000] = 0x00;//�Ĵ�����0,�ص�
	Init_Timer();
	while(1)
	{
		DisPlay_SMG();
	}
}