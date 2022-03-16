#include "reg52.h"

//�Ĵ�������,���������Ĵ���
sfr AUXR = 0x8e;

unsigned char urdat;

void SendByte(unsigned char dat);

void InitUart()//���ڳ�ʼ����SM1,SM2����ģʽ��
{
	//��Ʒ�ֲ�����,�ö�ʱ��1����������
	TMOD = 0x20;
	TH1 = 0xfd; //��Сд����
	TL0 = 0xfd;
	TR1 = 1;
	
	SCON = 0x50; //��ס����
	AUXR = 0x00; //B0 = 0ʱ�����������ʷ���������ÿ12��ʱ�Ӽ���һ��
	
	ES = 1;
	EA = 1;
}

//4��Ϊ�����ж�,0�����ⲿ�ж�0��1���Ƕ�ʱ��0��2�����ⲿ�ж�1��3���Ƕ�ʱ��1
void ServiceUart() interrupt 1
{
	if(RI == 1)//�������ݽ������
	{
		RI = 0;//�����˹�����
		urdat = SBUF;//����SBUF
		SendByte(urdat);
	}
}

//�����ֽں������ò�ѯ��ʽ
void SendByte(unsigned char dat)
{
	SBUF = dat;
	while(TI == 0);//��������1
	TI = 0;//�ֶ�����
}

void main()
{
	InitUart();

	while(1);
}