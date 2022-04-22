#include "reg52.h"
#include "ds1302.h"
#include "xmf_smg_ca.h"

//读密和解密，写和读的相关物理地址
unsigned char Write_DS1302_adrr[7] = {0x80,0x82,0x84,0x86,0x88,0x8a,0x8c};
unsigned char Read_DS1302_adrr[7] = {0x81,0x83,0x85,0x87,0x89,0x8b,0x8d};
//20年4月18日，周六，22点59分24秒
unsigned char Timer[7] = {0x24,0x59,0x23,0x18,0x04,0x06,0x20};
//miao-fen-xiaoshi-riqi-...
void DS1302_Config()
{
	char i;
	Write_Ds1302_Byte(0x8e,0x00);//0x8e表示可以写入数据，0x00写入数据
	for ( i = 0 ; i < 7 ;i++ )
	{
		Write_Ds1302_Byte(Write_DS1302_adrr[i],Timer[i]);
	}
	Write_Ds1302_Byte(0x8e,0x80);
}

void Read_DS1302_Timer()
{
	char i;
	for ( i = 0 ; i < 7 ; i++ )
	{
		Timer[i] = Read_Ds1302_Byte( Read_DS1302_adrr[i]);
	}
}

void Display_DS1302()
{
	//时间小时的十位（小时）
	DisplaySMG_Bit(0, SMGNoDot_CA[Timer[2] / 16]);
	DelaySMG(100);
	DisplaySMG_Bit(1, SMGNoDot_CA[Timer[2] % 16]);
	DelaySMG(100);
	
	//-
	DisplaySMG_Bit(2, 0xbf);
	DelaySMG(100);
	
	//时间的个位（分钟）
	DisplaySMG_Bit(3, SMGNoDot_CA[Timer[1] / 16]);
	DelaySMG(100);
	DisplaySMG_Bit(4, SMGNoDot_CA[Timer[1] % 16]);
	DelaySMG(100);
	
	//-
	DisplaySMG_Bit(5, 0xbf);
	DelaySMG(100);
	
	//秒
	DisplaySMG_Bit(6, SMGNoDot_CA[Timer[0] / 16]);
	DelaySMG(100);
	DisplaySMG_Bit(7, SMGNoDot_CA[Timer[0] % 16]);
	DelaySMG(100);
	
	Display_All(0xff);
}

void main()
{
	DS1302_Config();
	while(1)
	{
		Read_DS1302_Timer();
		Display_DS1302();
	}
}