#include<reg51.h>
#include"lcd.h"
#include"temp.h"

uchar CNCHAR[6] = "摄氏度";
void LcdDisplay(int);
void UsartConfiguration();


void main()
{
	UsartConfiguration();
	LcdInit();	
	LcdWriteCom(0x88);
	LcdWriteData('C'); 
	while(1)
	{
		LcdDisplay(Ds18b20ReadTemp());
	}
}

void LcdDisplay(int temp) 	 {
    
  	unsigned char i, datas[] = {0, 0, 0, 0, 0}; 
		float tp;  
	if(temp< 0)				
  	{
	  	LcdWriteCom(0x80);		
		SBUF='-';
		while(!TI);	    
		TI=0;
	    LcdWriteData('-');
		temp=temp-1;
		temp=~temp;
		tp=temp;
		temp=tp*0.0625*100+0.5;	
  	}
 	else
  	{			
	  	LcdWriteCom(0x80);
	    LcdWriteData('+');
		SBUF='+';
		while(!TI);
		TI=0;
		tp=temp;
		temp=tp*0.0625*100+0.5;	
	}
	datas[0] = temp / 10000;
	datas[1] = temp % 10000 / 1000;
	datas[2] = temp % 1000 / 100;
	datas[3] = temp % 100 / 10;
	datas[4] = temp % 10;

	LcdWriteCom(0x82);
	LcdWriteData('0'+datas[0]);
	SBUF = '0'+datas[0];
	while (!TI);
	TI = 0;
	
	LcdWriteCom(0x83);
	LcdWriteData('0'+datas[1]); 
	SBUF = '0'+datas[1];
	while (!TI);
	TI = 0;

	LcdWriteCom(0x84);
	LcdWriteData('0'+datas[2]);
	SBUF = '0'+datas[2];
	while (!TI);
	TI = 0;

	LcdWriteCom(0x85);
	LcdWriteData('.');
	SBUF = '.';
	while (!TI);
	TI = 0;

	LcdWriteCom(0x86);
	LcdWriteData('0'+datas[3]);
	SBUF = '0'+datas[3];
	while (!TI);
	TI = 0;

	LcdWriteCom(0x87);
	LcdWriteData('0'+datas[4]);
	SBUF = '0'+datas[4];
	while (!TI);
	TI = 0;
	for(i=0; i<6; i++)
	{
	 	SBUF = CNCHAR[i];
		while (!TI);
		TI = 0;
	} 
}


void UsartConfiguration()
{
	SCON=0X50;
	TMOD=0X20;
	PCON=0X80;
	TH1=0XF3;
	TL1=0XF3;
//	ES=1;						//打开接收中断
//	EA=1;						//打开总中断
	TR1=1;
}
