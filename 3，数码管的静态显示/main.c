#include "reg52.h"

sbit HC138_C = P2^7;
sbit HC138_B = P2^6;
sbit HC138_A = P2^5;

/*LEDģ��һ���а˸������ܣ����ӵ���P2�ڣ�
��������ͨ��ʮ�����������������ƺ�����ϣ�
��ʵ��LED����ˮЧ����
Ĭ�������P2 = 0xff����Ӧ��������Ϊ1111 1111��
��ʱLEDģ�������еĵƶ�����Ϩ��״̬��
��˸����ֶ�Ӧ�˸��ƣ����λ��ӦD1,���λ��ӦD8��
�磺P2 = 0xd0����1101 0000����ӦLEDģ���У�
D1��D2��D3��D4��D6����D5��D7��D8������*/

/*������led��0������1�ǲ�����*/

/*���Ƕ��ڽӿڣ���ʵ�������
������Ĵ������̵���֮���
�ϵĽӿ��Ѿ���ȡ�ǣ�0����ͨ�磬1����ͨ�硣*/

/*ͬʱ��ע�����138������������
y4����͵�ƽ����ΪCBA=10��*/


//����
unsigned char code SMG_duanma[18] = 
{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
0x80,0x90,0x88,0x80,0xc6,0xc0,0x86,0x8e,
0xbf,0x7f};

void InitHC138(unsigned char n)
{
		switch(n)
		{
		case 4:
				HC138_C = 1;
				HC138_B = 0;
				HC138_A = 0;
				break;
		case 5:
				HC138_C = 1;
				HC138_B = 0;
				HC138_A = 1;
				break;
		case 6:
				HC138_C = 1;
				HC138_B = 1;
				HC138_A = 0;
				break;
		case 7:
				HC138_C = 1;
				HC138_B = 1;
				HC138_A = 1;
				break;
		}
}

void ShowSMG_Bit(unsigned char dat,unsigned pos)
{
		InitHC138(6);   //����ܵ�λ��
		P0= 0x01 << pos;     //һ�㶼�����ƶ���λ
		InitHC138(7);    //����ܵ�����
		P0 = dat;    //0x82��Ӧ��ֵ��6
}

void Delay_ms(unsigned int t)
{
		while(t--);
		while(t--);
}
//�ر��޹��豸,�����������ͼ̵���
void InitSystem()
{
		HC138_C = 1;
		HC138_B = 0;
		HC138_A = 1;
		
		P0 = 0X00;
	
}

void SMG_Static()
{
		unsigned char i,j;
		for ( i = 0 ; i < 8 ; i++ )
		{
		Delay_ms(60000);
				for ( j= 0 ; j < 16 ;j++ )
				{
				ShowSMG_Bit(SMG_duanma[j],i);
				Delay_ms(60000);
				Delay_ms(60000);
				Delay_ms(60000);//�����ط���ʾ0��9���ٵ������ط�
				}
				
		}
}

//������
void main()
{
		InitSystem();
		while(1)
		{
			SMG_Static();
		}
		
}