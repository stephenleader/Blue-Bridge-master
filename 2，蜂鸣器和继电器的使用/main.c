#include "reg52.h"

sbit HC138_A = P2^5;
sbit HC138_B = P2^6;
sbit HC138_C = P2^7;



//函数
void Delay(unsigned int t)
{
		while(t--);
		while(t--);
}

void InitHC138(unsigned char n)
{
		switch(n)
		{
				
		case 4:
				HC138_C = 1;  //选Y4输出低电平
				HC138_B = 0;
				HC138_A = 0;
				break;
		
		case 5:
				HC138_C = 1;
				HC138_B = 0;  //选Y5输出低电平
				HC138_A = 1;
				break;
				
		case 6:
				HC138_C = 1;
				HC138_B = 1;  //选Y6输出低电平
				HC138_A = 0;
				break;
		
		
		case 7:
				HC138_C = 1;
				HC138_B = 1;  //选Y7输出低电平
				HC138_A = 1;
				break;
				}
}

void OutPut0(unsigned char a, unsigned char b)
{
		InitHC138(a);
		P0 = b;
}


void LEDRunning()
{
		unsigned char i;
		//HC138_C = 1;
		//HC138_B = 0;
		//HC138_A = 0;
		//y4非为高电平
		//---换为函数（优化）
		//InitHC138(4);
		
		//闪烁三下
		for(i = 0 ; i < 3 ; i++ )
		{
				OutPutP0(4,0x00);
				//P0 = 0x00;
				//0X00对于IO口来说是点亮,低电平然后取了一次非
				Delay(60000);
				Delay(60000);

				OutPutP0(4,0xff);
				//P0 = 0xff;
				//0xff对于IO口是熄灭
				Delay(60000);
				Delay(60000);
		}
		
		for(i = 1 ; i <= 8 ; i++ )
		{
		//0xff灭，全灭；0x00亮；=1高电平；=0低电平
		OutPut(4,(0xff<<i));
				//P0 = 0xff << i;
				//流水灯一定要延时，不然看不到效果
				Delay(60000);
		}	
		

		//点亮之后，由题意，继电器吸合，即让P0^4对应的继电器导通，P0^4 = 1
		/*HC138_C = 1;
		HC138_B = 0;
		HC138_A = 1;
		//二进制0001 0000 ,P0^4刚好为1
		//---换为函数（优化）*/
		//InitHC138(5);
		//P0 = 0x10;
		OutPutP0(5,0x10);
		Delay(60000);
		Delay(60000);
		//P0 = 0x00;关掉
		OutPutP0(5,0x00);
		
		
		//改让y4输出低电平,因为前面改动过C,B,A，所以这里要重新写
		/*HC138_C = 1;
		HC138_B = 0;
		HC138_A = 0;*/
		//---换为函数（优化）
		InitHC138(4);
		for( i = 1 ; i <= 8 ; i++ )
		{
				P0 = ~(0xff << i);
				Delay(60000);
		}
		
		//继电器吸合之后，根据题意，蜂鸣器对应接口P0^6应该为1
		//，叫一会儿
		/*HC138_C = 1;
		HC138_B = 0;
		HC138_A = 1;//y5输出低电平*/
		//---换为函数（优化）
		InitHC138(5);
		P0 = 0x40;//注意0100 0000对应的是P0^7,P0^6....
		Delay(60000);
		Delay(60000);
		P0 = 0x00;//关掉
		
}

//关掉无关设备的函数
void InitSystem()
{
		HC138_C = 1;
		HC138_B = 0;
		HC138_A = 1;
		//Y5输出低电平,LE才能为1，可以影响到接有蜂鸣器和继电器的控制器
		
		P0 = 0X00;
		//P0^0~P0^7都受P0影响
		
}


//主函数
void main()
{
		InitSystem();
		while(1)
		{
				LEDRunning();//主函数内调用函数
		}
}

