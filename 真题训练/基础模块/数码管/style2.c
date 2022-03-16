#include<stc15.h>
#include<intrins.h>
typedef unsigned int uint;
typedef unsigned char uchar;

uchar code tab[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xFF};
uchar smg[8];

void AllInit()
{
	P2=0x80;P0=0xFF;
	P2=0xA0;P0=0x00;
	P2=0xC0;P0=0xFF;
	P2=0xE0;P0=0xFF;
}
void Timer0Init(void)		//1����@11.0592MHz
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0xCD;		//���ö�ʱ��ֵ
	TH0 = 0xD4;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	ET0=1;
	EA=1;
}
void main()
{
	AllInit();
	Timer0Init();
	while(1)
	{
		smg[0]=tab[0];smg[1]=tab[1];
		smg[2]=tab[2];smg[3]=tab[3];
		smg[4]=tab[4];smg[5]=tab[5];
		smg[6]=tab[6];smg[7]=tab[7];
	}
}
uchar smg_cnt,i;
void Timer0()interrupt 1
{
	smg_cnt++;
	if(smg_cnt==2)
	{
		smg_cnt=0;
		P2=0xc0;P0=0x01<<i;P2=0;
		P2=0xe0;P0=smg[i];P2=0;
		i++;
		if(i==8)i=0;
	}
}