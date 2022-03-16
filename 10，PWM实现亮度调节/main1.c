/*
ռ�ձ�30%��
��ʾ�ߵ�ƽռ�Ȱٷ�֮30��
�͵�ƽռ�Ȱٷ�֮70
��ͨ��ռ�ձ��ñ���duty��ʾ
*/

#include "reg52.h"

sbit L1 = P0^0;
sbit S7 = P3^0;
sbit L2 = P14^5;

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
	}	
}

void Delay(unsigned int t)
{
			while(t--);
}

//��������
unsigned char count = 0;//��������
unsigned char pwm_duty = 0;//ռ�ձ�
//============��ʱ����ʼ������==========
void InitTimer0()
{
	TMOD = 0x01;//ͨ������������
	//10ms = 1000us, 1ms(ÿ��ʱ������)=100us * 100
	TH0 = (65535 - 100)/256;//��ʱ100��΢��Ķ�ʱ�ж�
	TL0 = (65535 - 100)%256;
	
	ET0 = 1;
	EA = 1;
}
//============��ʱ��������=============
void ServiceTimer0() interrupt 1
{
	//��װд����
	TH0 = (65535 - 100)/256;//��ʱ100��΢��Ķ�ʱ�ж�
	TL0 = (65535 - 100)%256;
	
	count++;
	if(count == pwm_duty)//��ռ�ձ�֮ǰ��1
	{
		L1 = 1;
	}
	else if(count == 100)//��100֮ǰ��0
	{
		L1 = 0;
		count = 0;
	}
}

//����һ����������ʾ��ǰ��������ʲô״̬
unsigned char stat = 0;

void ScanKey()
{
		if(S7 == 0)
		{
			Delay(100);//��ʱ����
			if(S7 == 0)
			{
				switch(stat)
				{
					case 0:
						L1 = 0;
						stat = 1;
						TR0 = 1;//����ſ�����ʱ��
						pwm_duty = 10;//10%������״̬,ռ�ձȸĳ�10%
					break;
						
					case 1:
						pwm_duty = 50;
						stat = 2;
					break;
					
					case 2:
						pwm_duty = 90;
						stat = 3;
					break;
					
					case 3:
						L1 = 1;//Ϩ��
						TR0 = 0;
						stat = 0;
					break;
				}
				while(S7== 0);//���ⷴ����ȡ����
			}		
		}
}

void InitSystem()
{
	SelectHC573(5);
	P0 = 0x00;
	SelectHC573(4);
	P0 = 0xff;
}

void main()
{
	InitSystem();
	InitTimer0();
	while(1)
	{
		ScanKey();
	}
}