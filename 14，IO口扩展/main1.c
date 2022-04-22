/**
*���ڴ洢��ӳ����չ����Ӧ��
*CT107Dƽ̨��J13Ҫ��1-2�Ŷ̽�
*���롰absacc.h"ͷ�ļ���
*ͨ��XBYTE�ؼ�����ֱ�Ӳ�����չ��Դ
*�洢��ӳ����չ��ʽҪռ�õ�Ƭ����P3.6����
*C�����е�XBYTE��Ӧ����е�MOV,MOVX
*
*
*/
#include "reg52.h"

void Delay(unsigned int t)
{
		while(t--);
		while(t--);
		while(t--);
}

void SelectHC573(unsigned char n)
{
		switch(n)
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
		}
}

void LEDRunnning()
{
	SelectHC573(4);
	P0=0xf0;
	Delay(60000);
	Delay(60000);
	Delay(60000);
	P0=0x0f;//����λ����
	Delay(60000);
	Delay(60000);
	Delay(60000);
	P0=0xff;//����λ����
	Delay(60000);
	Delay(60000);
	Delay(60000);
}

void SMGRunnning()
{
	unsigned char i;
	for ( i = 0 ; i < 8 ; i++ )
	{
		SelectHC573(6);
		P0 = 0x01 << i;
		SelectHC573(7);
		P0=0x00;
		Delay(60000);
		Delay(60000);
	}
}

void InitSystem()
{
	SelectHC573(5);
	P0=0x00;
	SelectHC573(4);
	P0=0xff;
}

void main01(void)
{
	InitSystem();
	while(1)
	{
		LEDRunnning();
		SMGRunnning();
	}
}
