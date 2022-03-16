#include "reg52.h"
//添加月份，日期
unsigned char yue = 1;
sbit HC573_C = P2^7;
sbit HC573_B = P2^6;
sbit HC573_A = P2^5;

void Display_Dynamic();
unsigned char code SMG_duanma[18]={
		0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
		0x80,0x90,0x88,0x80,0xc6,0xc0,0x86,0x8e,
		0xbf,0x7f};
		
void DelaySMG(unsigned int t)
{
		while (t--);

}

void Delay(unsigned char t)
{
		while(t--)
		{
				Display_Dynamic();
		}
}


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
		case 6:
				HC573_C = 1;
				HC573_B = 1;
				HC573_A = 0;
				break;
		case 7:
				HC573_C = 1;
				HC573_B = 1;
				HC573_A = 1;
				break;
		}
}

void DisplaySMG_Bit(unsigned char value, unsigned char pos)
{
		SelectHC573(6);
		P0 = 0x01 << pos;//右移位
		SelectHC573(7);
		P0 = value;
}

void Display_Dynamic()
{
//时间很短的时候，才能进是全部显示
		DisplaySMG_Bit(SMG_duanma[2],0);
		DelaySMG(50);
		DisplaySMG_Bit(SMG_duanma[0],1);
		DelaySMG(50);
		DisplaySMG_Bit(SMG_duanma[1],2);
		DelaySMG(50);
		DisplaySMG_Bit(SMG_duanma[8],3);
		DelaySMG(50);
		
		//2018之后，段码16表示分隔符号
		DisplaySMG_Bit(SMG_duanma[16],4);
		DelaySMG(50);
		DisplaySMG_Bit(SMG_duanma[16],5);
		DelaySMG(50);
		
		DisplaySMG_Bit(SMG_duanma[yue/10],6);
		DelaySMG(50);
		DisplaySMG_Bit(SMG_duanma[yue%10],7);
		DelaySMG(50);
}

void InitSystem()
{
		HC573_C = 1;
		HC573_B = 0;
		HC573_A = 1;
		
		P0 = 0x00;
}

void main()
{
		InitSystem();
		while(1)
		{
		Display_Dynamic();
		yue++;
		if ( yue > 12)
		{
				yue =1;
		}
		Delay(100);
		}
}