//扩展功能

#include "reg52.h"

sbit HC573_C = P2^7;
sbit HC573_B = P2^6;
sbit HC573_A = P2^5;

//按键
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

unsigned char stat_k = 0;
//状态表示S6和S7都没被按下,初态定义为0


//扫描独立按键
void ScanKeys_Alone()
{
		if(S7 == 0)//S7状态设置为1
		{
				Delay_K(100);//延时消抖
				if(S7 == 0)//查询当前状态，为0者被按下
				//判断按键是否是真的按下
				{
						if(stat_k == 0 )
						{
								L1 = 0;
								
								stat_k = 1;
						}
						else if (stat_k == 1)
						{
								L1 = 1;
								stat_k = 0;
						}
				}
				while(S6 == 0);
		}
				
			
		if(S6 == 0)//S6对应状态设置为2
		{
				Delay_K(100);//延时消抖
				if(S6 == 0)//查询当前状态，为0者被按下
				//判断按键是否是真的按下
				{
						if(stat_k == 0)
						{
								L2 = 0;
								
								stat_k = 2;
							
						}
						else if (stat_k == 2)
						{
								L2 = 1;
								stat_k = 0;
							
						}
						
				}
		}
				
	
		if(S5 == 0)//状态设为3
		{
				Delay_K(100);//延时消抖
				if(S5 == 0)//查询当前状态，为0者被按下
				//判断按键是否是真的按下
				{
						if(stat_k == 1 )
						{
								L3 = 0;
								while(S5 == 0);//当S5松开=1
								L3 = 1;
						}
						else if (stat_k == 1)
						{
								L5 = 1;
								while(S5 == 0);
								L5 = 1;
						}
				}
				while(S5 == 0);
		}
				
		if(S4 == 0)
		{
				Delay_K(100);//延时消抖
				if(S4 == 0)//查询当前状态，为0者被按下
				//判断按键是否是真的按下
				{
						if(stat_k == 1 )
						{
								L4 = 0;
								while(S4 == 0);
								L4 =1;
						}
						else if (stat_k == 2)
						{
								L6 = 0;
								while(S4 == 0);
								L6 = 1;
						}
				}
		}
			
}

void InitSystem()
{
		SelectHC573(5);
		P0 = 0X00;
		SelectHC573(4);
		P0 = 0xff;//若为0x00,则为0000 0000,led灯全亮

}

void main()
{
		
		InitSystem();
		while(1)
		{
				ScanKeys_Alone();
		}
}