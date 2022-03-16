#include<stc15.h>
#include<intrins.h>
typedef unsigned int uint;
typedef unsigned char uchar;

uchar code tab[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xFF};
uchar smg[8];

bit key_flag;

uchar Trg=0,Cont=0;
void BTN()
{
	uchar dat=P3^0xff;
	Trg=dat&(dat^Cont);
	Cont=dat;
}

void AllInit()
{
	P2=0x80;P0=0xFF;P2=0;
	P2=0xA0;P0=0x00;P2=0;
	P2=0xC0;P0=0xFF;P2=0;
	P2=0xE0;P0=0xFF;P2=0;
}
void Timer0Init(void)		//100΢��@11.0592MHz
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0xAE;		//���ö�ʱ��ֵ
	TH0 = 0xFB;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	ET0=1;
	EA=1;
}
void main()
{
	uchar num=10;
	AllInit();
	Timer0Init();
	while(1)
	{
		//������Ԫ
		if(key_flag==1)
		{
			key_flag=0;
			BTN();
			if(Trg & 0x08)//s4
			{
				num=4;
			}
			if(Trg & 0x04)//s5
			{
				num=5;
			}
			if(Trg & 0x02)//s6
			{
				num=6;
			}
			if(Trg & 0x01)//s7
			{
				num=7;
			}
		}
		if(Cont & 0x08)
		{
			//�жϳ���s4  ����ʧЧ
		}
		smg[0]=0xff;smg[2]=0xff;
		smg[1]=0xff;smg[3]=0xff;
		smg[4]=0xff;smg[5]=0xff;
		smg[6]=0xff;
		smg[7]=tab[num];
	}
}
uchar smg_cnt,i;
uchar key_cnt;
void Timer0()interrupt 1
{
	key_cnt++;smg_cnt++;
	if(key_cnt==100)
	{
		key_cnt=0;
		key_flag=1;
	}
	if(smg_cnt==20)
	{
		smg_cnt=0;
		P2=0xC0;P0=0x01<<i;P2=0;
		P2=0xE0;P0=smg[i];P2=0;
		i++;
		if(i==8)i=0;
	}
}