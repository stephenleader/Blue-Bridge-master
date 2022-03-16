#include "reg52.h"

//P4的地址 = 0xc0(大小写都可以）
sfr P4 = 0xc0;

sbit HC573_C = P2^7;
sbit HC573_B = P2^6;
sbit HC573_A = P2^5;

//位定义
//一行,right
sbit R1 = P3^0;
sbit R2 = P3^1;
sbit R3 = P3^2;
sbit R4 = P3^3;

//一列,
sbit C4 = P3^4;
sbit C3 = P3^5;
sbit C2 = P4^2;
sbit C1 = P4^4;

unsigned char key_num = 0;

//考试的时候没有只有自己计算
unsigned char code SMG_duanma[18]=
{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
0x80,0x90,0x88,0x80,0xc6,0xc0,0x86,0x8e,
0xbf,0x7f};

void Delay_ms(unsigned char t)
{
			while(t--);
			while(t--);
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

void DisplayKeyNum(unsigned char value,unsigned pos)
{
		SelectHC573(6);
		P0 = 0x01 << pos;
		SelectHC573(7);
		P0 = value;
}

void InitSystem()
{
	SelectHC573(5);
	P0=0x00;
	SelectHC573(4);
	P0=0xff;
}


void ScanMulti()
{
	
	//注意记住扫描过程
		R1 = 0;
		R2 = R3 = R4 = 1;
		C1 = C2 = C3 = C4 = 1;
		if(C1 == 0 )
		{
			while(C1 == 0);//C1若没有松开=0；
			//当他松开则跳转到下一个语句
			key_num = 0;
			DisplayKeyNum(SMG_duanma[key_num],1);
		}
		else if (C2 == 0)//C2按下表示=0
		{
			while(C2==0);
			key_num = 1;
			DisplayKeyNum(SMG_duanma[key_num],1);
		}
		else if (C3 == 0)
		{
			while(C3 == 0);
			key_num = 2;
			DisplayKeyNum(SMG_duanma[key_num],1);
		}
		else if (C4 == 0 )
		{
			while(C4 == 0);
			key_num = 3;
			DisplayKeyNum(SMG_duanma[key_num],1);
		}
		
		
		//第二行扫描
		R2 = 0;
		R1 = R3 = R4 = 1;
		C1 = C2 = C3 = C4 = 1;
		if(C1 == 0 )
		{
			while(C1 == 0);//C1若没有松开=0；
			//当他松开则跳转到下一个语句
			key_num = 4;
			DisplayKeyNum(SMG_duanma[key_num],1);
		}
		else if (C2 == 0)//C2按下表示=0
		{
			while(C2==0);
			key_num = 5;
			DisplayKeyNum(SMG_duanma[key_num],1);
		}
		else if (C3 == 0 )
		{
			while(C3 == 0);
			key_num = 6;
			DisplayKeyNum(SMG_duanma[key_num],1);
		}
		else if (C4 == 0 )
		{
			while(C4 == 0);
			key_num = 7;
			DisplayKeyNum(SMG_duanma[key_num],1);
		}
		
		
		//第三行的扫描
		R3 = 0;
		R2 = R1 = R4 = 1;
		C1 = C2 = C3 = C4 = 1;
		if(C1 == 0 )
		{
			while(C1 == 0);//C1若没有松开=0；
			//当他松开则跳转到下一个语句
			key_num = 8;
			DisplayKeyNum(SMG_duanma[key_num],1);
		}
		else if (C2 == 0)//C2按下表示=0
		{
			while(C2==0);
			key_num = 9;
			DisplayKeyNum(SMG_duanma[key_num],1);
		}
		else if (C3 == 0 )
		{
			while(C3 == 3);
			key_num = 10;
			DisplayKeyNum(SMG_duanma[key_num],1);
		}
		else if (C4 == 0 )
		{
			while(C4 == 0);
			key_num = 11;
			DisplayKeyNum(SMG_duanma[key_num],1);
		}
		
		//第三行的扫描
		R3 = 0;
		R2 = R1 = R4 = 1;
		C1 = C2 = C3 = C4 = 1;
		if(C1 == 0 )
		{
			while(C1 == 0);//C1若没有松开=0；
			//当他松开则跳转到下一个语句
			key_num = 8;
			DisplayKeyNum(SMG_duanma[key_num],1);
		}
		else if (C2 == 0)//C2按下表示=0
		{
			while(C2==0);
			key_num = 9;
			DisplayKeyNum(SMG_duanma[key_num],1);
		}
		else if (C3 == 0 )
		{
			while(C3 == 3);
			key_num = 10;
			DisplayKeyNum(SMG_duanma[key_num],1);
		}
		else if (C4 == 0 )
		{
			while(C4 == 0);
			key_num = 11;
			DisplayKeyNum(SMG_duanma[key_num],1);
		}
		
		
		//第四行的扫描
		R4 = 0;
		R2 = R1 = R3 = 1;
		C1 = C2 = C3 = C4 = 1;
		if(C1 == 0 )
		{
			while(C1 == 0);//C1若没有松开=0；
			//当他松开则跳转到下一个语句
			key_num = 12;
			DisplayKeyNum(SMG_duanma[key_num],1);
		}
		else if (C2 == 0)//C2按下表示=0
		{
			while(C2==0);
			key_num = 13;
			DisplayKeyNum(SMG_duanma[key_num],1);
		}
		else if (C3 == 0 )
		{
			while(C3 == 3);
			key_num = 14;
			DisplayKeyNum(SMG_duanma[key_num],1);
		}
		else if (C4 == 0 )
		{
			while(C4 == 0);
			key_num = 15;
			DisplayKeyNum(SMG_duanma[key_num],1);
		}

}


void main()
{
	InitSystem();
		while(1)
		{
		ScanMulti();
		}
}