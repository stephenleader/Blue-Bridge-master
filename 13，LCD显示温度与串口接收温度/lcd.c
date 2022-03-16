#include"lcd.h"

void Lcd1602_Delay1ms(uint c) 
{
    uchar a,b;
	for (; c>0; c--)
	{
		 for (b=199;b>0;b--)
		 {
		  	for(a=1;a>0;a--);
		 }      
	}
    	
}

#ifndef 	LCD1602_4PINS	
void LcdWriteCom(uchar com)	 
{
	LCD1602_E = 0; 
	LCD1602_RS = 0;	
	LCD1602_RW = 0;	
	
	LCD1602_DATAPINS = com;
	Lcd1602_Delay1ms(1);

	LCD1602_E = 1;
	Lcd1602_Delay1ms(5);
	LCD1602_E = 0;
}
#else 
void LcdWriteCom(uchar com)	
{
	LCD1602_E = 0;
	LCD1602_RS = 0;
	LCD1602_RW = 0;

	LCD1602_DATAPINS = com;	//由于4位的接线是接到P0口的高四位，所以传送高四位不用改
	Lcd1602_Delay1ms(1);

	LCD1602_E = 1;
	Lcd1602_Delay1ms(5);
	LCD1602_E = 0;

//	Lcd1602_Delay1ms(1);
	LCD1602_DATAPINS = com << 4;
	Lcd1602_Delay1ms(1);

	LCD1602_E = 1;
	Lcd1602_Delay1ms(5);
	LCD1602_E = 0;
}
#endif
		   
#ifndef 	LCD1602_4PINS		   
void LcdWriteData(uchar dat)			
{
	LCD1602_E = 0;	
	LCD1602_RS = 1;	
	LCD1602_RW = 0;	

	LCD1602_DATAPINS = dat;
	Lcd1602_Delay1ms(1);

	LCD1602_E = 1;
	Lcd1602_Delay1ms(5);
	LCD1602_E = 0;
}
#else
void LcdWriteData(uchar dat)
{
	LCD1602_E = 0;	  
	LCD1602_RS = 1;
	LCD1602_RW = 0;	

	LCD1602_DATAPINS = dat;
	Lcd1602_Delay1ms(1);

	LCD1602_E = 1;
	Lcd1602_Delay1ms(5);
	LCD1602_E = 0;

	LCD1602_DATAPINS = dat << 4;
	Lcd1602_Delay1ms(1);

	LCD1602_E = 1;
	Lcd1602_Delay1ms(5);
	LCD1602_E = 0;
}
#endif
	   
#ifndef		LCD1602_4PINS
void LcdInit()	//LCD初始化
{
 	LcdWriteCom(0x38);
	LcdWriteCom(0x0c);
	LcdWriteCom(0x06);
	LcdWriteCom(0x01);
	LcdWriteCom(0x80);
}
#else
void LcdInit()	
{
	LcdWriteCom(0x32);	 //将8位总线转为4位总线
	LcdWriteCom(0x28);	 //在四位线下的初始化
	LcdWriteCom(0x0c); 
	LcdWriteCom(0x06);
	LcdWriteCom(0x01);  //清屏
	LcdWriteCom(0x80);
}
#endif
