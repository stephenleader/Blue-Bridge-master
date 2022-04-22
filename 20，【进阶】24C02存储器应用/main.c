#include "reg52.h"
#include "absacc.h"
#include "iic.h"

sbit HC573_C = P2^7;
sbit HC573_B = P2^6;
sbit HC573_A = P2^5;

unsigned char dat1 = 0, dat2 = 0, dat3 = 0;

unsigned char code SMG_duanma[18]=
		{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,
		 0x88,0x80,0xc6,0xc0,0x86,0x8e,0xbf,0x7f};

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
void InitSystem()
{
	SelectHC573(5);
	P0=0x00;
	SelectHC573(4);
	P0=0xff;
}

void DelaySMG(unsigned int t)
{
	while(t--);
}
 
void DisplaySMG_Bit(unsigned char pos, unsigned char value)
{
	XBYTE[0xE000] = 0xFF;
	XBYTE[0xC000] = 0x01 << pos;
	XBYTE[0xE000] = value;
}

void Write_24C02(unsigned char addr, unsigned char dat)
{
	IIC_Start();						
	IIC_SendByte(0xa0); 		
	IIC_WaitAck();				
	IIC_SendByte(addr); 		
	IIC_WaitAck(); 					
	IIC_SendByte(dat); 			
	IIC_WaitAck();					
	IIC_Stop();							
}

unsigned char Read_24C02(unsigned char addr)
{
	unsigned char tmp;
	
	IIC_Start();					
	IIC_SendByte(0xa0); 		
	IIC_WaitAck();					
	IIC_SendByte(addr); 		
	IIC_WaitAck(); 					
	
	IIC_Start();						
	IIC_SendByte(0xa1); 		
	IIC_WaitAck();				
	tmp = IIC_RecByte();							
	IIC_Stop();							
	return tmp;
}

void Read_Write()
{
	dat1 = Read_24C02(0x01);
	dat2 = Read_24C02(0x03);
	dat3 = Read_24C02(0x05);
	
	dat1 = dat1 + 1;
	dat2 = dat2 + 2;
	dat3 = dat3 + 3;
	
	if(dat1 > 10)
		dat1 = 0;
	if(dat2 > 20)
		dat2 = 0;
	if(dat3 > 30)
		dat3 = 0;
	
	Write_24C02(0x01, dat1);
	DelaySMG(1000);
	Write_24C02(0x03, dat2);
	DelaySMG(1000);
	Write_24C02(0x05, dat3);
	DelaySMG(1000);
}

void DisplaySMG_24C02()
{
	DisplaySMG_Bit(0, SMG_duanma[dat1/10]);
	DelaySMG(500);
	DisplaySMG_Bit(1, SMG_duanma[dat1%10]);
	DelaySMG(500);
	DisplaySMG_Bit(2, SMG_duanma[16]);
	DelaySMG(500);
	
	DisplaySMG_Bit(3, SMG_duanma[dat2/10]);
	DelaySMG(500);
  DisplaySMG_Bit(4, SMG_duanma[dat2%10]);
	DelaySMG(500);
	DisplaySMG_Bit(5, SMG_duanma[16]);
	
	DelaySMG(500);
	DisplaySMG_Bit(6, SMG_duanma[dat3/10]);
	DelaySMG(500);
	DisplaySMG_Bit(7, SMG_duanma[dat3%10]);	
	DelaySMG(500);
 
}

main()
{
	InitSystem();
	XBYTE[0x8000] = 0xFF;
	XBYTE[0xA000] = 0x00;
	Read_Write();
	while(1)
	{
		DisplaySMG_24C02();
	}
}
