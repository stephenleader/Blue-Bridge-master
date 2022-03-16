#include<stc15.h>
#include<iic.h>
#include<intrins.h>
typedef unsigned int uint;
typedef unsigned char uchar;
uchar code tab[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xFF};
uchar menu1[8];
uchar menu2[8];
uchar menu3[8];
uchar time[2];

sbit TX=P1^0;
sbit RX=P1^1;

uchar set_index=0;
bit key_flag=0,sonic_flag=0;
uchar state;
uchar menu_index;
bit stop,work=0;
uchar goods,left_time;
bit relay,buzz;
bit smg_blink,led_blink;

uchar Trg=0,Cont=0;
void BTN()
{
	uchar dat=P3^0xFF;
	Trg=dat&(dat^Cont);
	Cont=dat;
}

void Delay20us()		//@11.0592MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	_nop_();
	i = 52;
	while (--i);
}

void Delay20ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 1;
	j = 216;
	k = 35;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

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

void Timer1Init(void)		
{
	AUXR &= 0xBF;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
}

void main()
{
	uchar weight;
	uchar distance;
	AllInit();
	time[0]=read_rom(0);
	time[1]=read_rom(1);
	if(time[0]<1 || time[0]>10 || time[1]<1 || time[1]>10 )
	{
		time[0]=2;time[1]=4;
	}
	Timer0Init();
	Timer1Init();
	while(1)
	{
		EA=0;
		weight=read_adc(0x03);
		EA=1;
		Delay20ms();
		if(weight < 51){if(menu_index!=3)menu_index=0;state=1;buzz=0;}//����
		else if(weight >= 204){state=3;buzz=1;}
		else {state=2;buzz=0;}
		//������Ԫ
		if(key_flag)
		{
			key_flag=0;
			BTN();
			if(Trg & 0x08)//s4
			{
				if(state==2)
				{
					work=1;
					relay=1;
					menu_index=2;
				}
			}
			if(Trg & 0x04)//s5
			{
				if(work)
				{
					if(stop==0)
					{
						stop=1;
						relay=0;
					}else 
					{
						stop=0;
						relay=1;
					}
				}
			}
			if(Trg & 0x02)//s6
			{
				if(state==1)
				{
					menu_index=3;
					set_index++;
					if(set_index==3)
					{
						menu_index=0;
						set_index=0;
						write_rom(0,time[0]);Delay20ms();
						write_rom(1,time[1]);Delay20ms();
					}
				}
			}
			if(Trg & 0x01)//s7
			{
				if(state==1)
				{
					time[set_index-1]++;
					if(time[set_index-1]>10)time[set_index-1]=1;
				}
			}
		}
		//��������Ԫ
		if(sonic_flag)
		{
			sonic_flag=0;
			TX=1;
			Delay20us();
			TX=0;
			while(!RX);
			TH1=0;TL1=0;
			TF1=0;TR1=1;
			while(RX==1 && TF1==0);
			TR1=0;
			if(TF1==1){TF1=0;distance=99;}
			else 
			{
				distance=(TH1<<8|TL1)*0.017;
				if(distance>99)distance=99;
			}
			if(distance <= 30)
			{
				goods=1;
				menu_index=1;
			}else
			{
				goods=2;
				menu_index=1;
			}
			left_time=time[goods-1];
		}
		//LED��Ԫ
		if(state==1)
		{
			P0=0xfe;
			P2=0x80;P0=0xfe;P2=0;
		}else if(state==2)
		{
			if(stop==0)
			{
				P0=0xfd;
				P2=0x80;P0=0xfd;P2=0;
			}else
			{
				if(led_blink)
				{
					P0=0xf7;
					P2=0x80;P0=0xf7;P2=0;
				}else
				{
					P0=0xff;
					P2=0x80;P0=0xff;P2=0;
				}
			}
		}else if(state==3)
		{
			if(led_blink)
			{
				P0=0xfb;
				P2=0x80;P0=0xfb;P2=0;
			}else
			{
				P0=0xff;
				P2=0x80;P0=0xff;P2=0;
			}
		}
		//�������̵�����Ԫ
		if(relay==0 && buzz==0)
		{
			P0=0x00;
			P2=0xa0;P0=0x00;P2=0;
		}else if(relay==1 && buzz==0)
		{
			P0=0x10;
			P2=0xa0;P0=0x10;P2=0;
		}else if(relay==0 && buzz==1)
		{
			P0=0x40;
			P2=0xa0;P0=0x40;P2=0;
		}else if(relay==1 && buzz==1)
		{
			P0=0x50;
			P2=0xa0;P0=0x50;P2=0;
		}
		//����ܵ�Ԫ
		if(menu_index==1)
		{
			menu1[0]=tab[1];menu1[1]=0xFF;menu1[2]=0xFF;
			menu1[3]=tab[distance/10];menu1[4]=tab[distance%10];
			menu1[5]=0xFF;menu1[6]=0xFF;
			menu1[7]=tab[goods];
		}else if(menu_index==2)
		{
			menu2[0]=tab[2];menu2[1]=0xff;menu2[2]=0xff;
			menu2[3]=0xff;menu2[4]=0xff;menu2[5]=0xff;
			menu2[6]=tab[left_time/10];
			menu2[7]=tab[left_time%10];
		}else if(menu_index==3)
		{
			menu3[0]=tab[3];menu3[1]=0xff;menu3[2]=0xff;menu3[5]=0xff;
			if(set_index==1)
			{
				if(smg_blink){menu3[3]=tab[time[0]/10];menu3[4]=tab[time[0]%10];}
				else {menu3[3]=0xff;menu3[4]=0xff;}
				menu3[6]=tab[time[1]/10];menu3[7]=tab[time[1]%10];
			}else if(set_index==2)
			{
				if(smg_blink){menu3[6]=tab[time[1]/10];menu3[7]=tab[time[1]%10];}
				else {menu3[6]=0xff;menu3[7]=0xff;}
				menu3[3]=tab[time[0]/10];menu3[4]=tab[time[0]%10];
			}
		}
	}
}

uchar key_cnt,sonic_cnt;
uchar smg_cnt,i;
uint smg_blink_cnt;
uint work_cnt,led_cnt;
void Timer0()interrupt 1
{
	key_cnt++;smg_cnt++;
	if(key_cnt==10)
	{
		key_cnt=0;
		key_flag=1;
	}
	if(smg_cnt==2)
	{
		smg_cnt=0;
		P2=0xE0;P0=0xFF;P2=0;
		P2=0xC0;P0=0x01<<i;P2=0;
		if(menu_index==0){P2=0xE0;P0=0xFF;P2=0;}
		if(menu_index==1){P2=0xE0;P0=menu1[i];P2=0;}
		if(menu_index==2){P2=0xE0;P0=menu2[i];P2=0;}
		if(menu_index==3){P2=0xE0;P0=menu3[i];P2=0;}
		i++;
		if(i==8)i=0;
	}
	if(state==2 && work==0)
	{
		sonic_cnt++;
		if(sonic_cnt==200)
		{
			sonic_cnt=0;
			sonic_flag=1;
		}
	}else 
	{
		sonic_cnt=0;
		sonic_flag=0;
	}
	if(menu_index==3)
	{
		smg_blink_cnt++;
		if(smg_blink_cnt==800)
		{
			smg_blink_cnt=0;
			smg_blink=~smg_blink;
		}
	}else 
	{
		smg_blink_cnt=0;
		smg_blink=0;
	}
	if(work==1 && stop==0)
	{
		work_cnt++;
		if(work_cnt==1000)
		{
			work_cnt=0;
			left_time--;
			if(left_time==0){work=0;relay=0;}
		}
	}else work_cnt=0;
	if(state==3 || (state==2&&stop==1) )
	{
		led_cnt++;
		if(led_cnt==500)
		{
			led_cnt=0;
			led_blink=~led_blink;
		}
	}else 
	{
		led_cnt=0;
		led_blink=0;
	}
}