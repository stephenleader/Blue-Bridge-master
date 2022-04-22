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
#include "absacc.h"

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

/**
ע���ⲿ��Դ�ĵ�ַӳ���ϵ
|   0x8000----ָʾ��
|   0xa000----�������ͼ̵���
|   0xc000----�����λѡ
|   0xe000----����ܶ���
*/

void LEDRunnning()
{
	//T0�ڲ���
	XBYTE[0x8000]=0xf0;
	Delay(60000);
	Delay(60000);
	XBYTE[0x8000]=0x0f;  //����λ����
	Delay(60000);
	Delay(60000);
	XBYTE[0x8000]=0xff; 
	Delay(60000);
	Delay(60000);
}

void SMGRunnning()
{
	unsigned char i;
	for ( i = 0 ; i < 8 ; i++ )
	{
		//�����λѡ
		XBYTE[0xc000]=0x01 <<i;
		//��ѡ
		XBYTE[0xe000]=0x00;
		Delay(60000);
		Delay(60000);
	}
	XBYTE[0xc000]=0xff;
	Delay(60000);
	Delay(60000);
}

void InitSystem()
{
	SelectHC573(5);
	P0=0x00;
}

void main(void)
{
	InitSystem();
	while(1)
	{
		LEDRunnning();
		SMGRunnning();
	}
}
