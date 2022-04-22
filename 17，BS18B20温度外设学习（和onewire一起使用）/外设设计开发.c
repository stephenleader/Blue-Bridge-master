//×¢Òâ¿´DS18B20ÎÂ¶È×ª»»ºÍ¶ÁÈ¡Á÷³Ì.png

#include "reg52.h"
#include "absacc.h"
#include "onewire.h"

//ÉèÖÃÎÂ¶È±äÁ¿
unsigned int temp = 0;

//0~9(Ã»ÓĞµã£©
unsigned char code SMGNoDot_CA[10]={
		0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
		0x80,0x90};

//0.~9.
unsigned char code SMGDot_CA[10]={
		0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,
		0x00,0x10};	

void DelaySMG(unsigned int t)
{
	while(t--);
}

void DisplaySMG_Bit(unsigned char pos, unsigned char dat)
{
	//ÊıÂë¹Üµ÷ÓÃ
	XBYTE[0xe000] = 0xff;//ÏûÒş
	XBYTE[0xc000] = 0x01 << pos;//×óÒÆ
	XBYTE[0xe000] = dat;//ËùÏÔÊ¾µÄÄÚÈİ
}

//ÉèÖÃ¶ÎÂëÖµ£¬ÊÇÈ«²¿ÊıÂë¹ÜÓĞĞ§
void Display_All(unsigned char dat)
{
	XBYTE[0xc000] = 0xff;
	XBYTE[0xe000] = dat;
}

//½¨ÕûĞÎÊı¾İ
//567
//  567 / 100 = 5 = S
//  567 % 100 = 67 / 10 = 6 
//  567 % 10 = 7
void DisplaySMG_Temp()
{
	DisplaySMG_Bit(7,SMGNoDot_CA[temp % 10]);	 //¸öÎ»
	DelaySMG(100);
	DisplaySMG_Bit(6,SMGDot_CA[(temp % 100) / 10]);	 //Ê®Î»
	DelaySMG(100);
	DisplaySMG_Bit(5,SMGNoDot_CA[temp / 100]);	  //°ÙÎ»
	DelaySMG(100);
	
	
	DisplaySMG_Bit(4,0xff);	
	DelaySMG(100);
	DisplaySMG_Bit(3,0xff);	
	DelaySMG(100);
	DisplaySMG_Bit(2,0xff);	
	DelaySMG(100);
	DisplaySMG_Bit(1,0xff);	
	DelaySMG(100);
	DisplaySMG_Bit(0,0xff);	
	DelaySMG(100);
	
	Display_All(0xff);

}

//ÊıÂë¹Ü¶¯Ì¬ÑİÊ¾µÄÑÓÊ±
void Delay(unsigned int t)
{
	while(t--)
	{
		DisplaySMG_Temp();
	}
}

//onewire¡£hÉùÃ÷µÄº¯ÊıÒªµ÷ÓÃ
//¶Áonwire.h
//²é¿´¸ùÄ¿Â¼ÏÂµÄpng
//×¢Òâ×ª»»µÄ²½Öè
void Read_DS18B20_temp()
{
	unsigned char LSB,MSB;//low 8 bit ,high 8 bit
	
	init_ds18b20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0x44);
	
	Delay(1000);
	
	init_ds18b20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0xbe);
	
	LSB = Read_DS18B20();
	MSB = Read_DS18B20();
	
	temp = MSB;
	temp = (temp << 8) | LSB;  //16Î»ÎÂ¶ÈÊı¾İ
	
	if((temp & 0xf800) == 0x0000)  //0ÉÏ¹¤×÷
	{
		temp >>= 4;//Ğ¡ÊıÒÆ×ß
		temp = temp * 10;
		temp = temp + (LSB & 0x0f) * 0.625;//·Å´óÊ®±¶£¬·Ö±æÂÊÊÇ0£¡£0625
	}
}

void main()
{
	XBYTE[8000] = 0x00;//ÃğµÆ
	while(1)
	{
		Read_DS18B20_temp();
		DisplaySMG_Temp();
	}
}