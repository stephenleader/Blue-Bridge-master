#include "reg52.h"

sbit HC573_C = P2^7;
sbit HC573_B = P2^6;
sbit HC573_A = P2^5;

//����
sbit S7 = P3^0;
sbit S6 = P3^1;
sbit S5 = P3^2;
sbit S4 = P3^3;

//led
sbit L1 = P0^0;
sbit L2 = P0^1;
sbit L3 = P0^2;
sbit L4 = P0^3;
sbit L5 = P0^4;
sbit L6 = P0^5;

void SelectHC573(unsigned char n)
{
		switch(n)
		{
		case 4:
				P2 = (P2 & 0x1f ) | 0x80;
		break;	
		
		case 5:
		P2 = (P2 & 0x1f ) | 0xa0;
		break;
		
		case 6:
		P2 = (P2 & 0x1f ) | 0xc0;
		break;
		
		case 7:
		P2 = (P2 & 0x1f ) | 0xe0;
		break;
		}
}

void Delay_K(unsigned char y)
{
	while(y--);

}
//ɨ���������
void ScanKeys_Alone()
{
		if(S7 == 0)
		{
				
				if(S7 == 0)//��ѯ��ǰ״̬��Ϊ0�߱�����
				//�жϰ����Ƿ�����İ���
				{
				Delay_K(100);//��ʱ����
						L1 = 0;//Ϊ0ʱ��Ч��������
				}
				else
				{
						L1 = 1;
				}
		}
				
			
		if(S6 == 0)
		{
			
				if(S6 == 0)//�жϰ����Ƿ�����İ���.���½�ͨ
				{
				Delay_K(100);
						L2 = 0;//���ڵ���˵
				}
				else
				{
						L2 = 1;
				}
		}
				
	
		if(S5 == 0)
		{
				
				if(S5 == 0)//�жϰ����Ƿ�����İ���
				{
				Delay_K(100);
						L3 = 0;
				}
				else
				{
						L3 = 1;
				}
		}
				
			
		if(S4 == 0)
		{
				
				if(S4 == 0)//�жϰ����Ƿ�����İ���
				{
				Delay_K(100);
						L4 = 0;
				}
				else
				{						
						L4 = 1;		
				}
		}
			
}

void InitSystem()
{
		SelectHC573(5);
		P0 = 0X00;
		SelectHC573(4);
		P0 = 0xff;//��Ϊ0x00,��Ϊ0000 0000,led��ȫ��

}

void main()
{
		
		InitSystem();
		while(1)
		{
				ScanKeys_Alone();
		}
}