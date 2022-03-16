#include "reg52.h"
typedef unsigned int u16;
u16 count;

sbit led = P1^0;
 void InitTimer0(void)
 {
		TMOD = 0x01;//设置工作模式
		//相当于方式1
		TH0 = 0xFF;
		TL0 = 0x9c;//对存储器设置初值，
		//取决于设定计时多少时长
		EA = 1;//打开总中断
		ET0 = 1;
		TR0 = 1;
 }
 
 void main(void)
 {//主函数里面一定要调用初始化函数
		InitTimer0();//T0初始化
		led = 1;
		count = 0;
		while(1);//程序堵塞到这里一直循环
 }
 
 //每一次溢出都会进入到定时器的中断响应
 void Timer0Tnterrupt(void) interrupt 1
 {
		TH0 = 0xFF;
		TL0 = 0x9C;
		//这里要重新赋予一个初值
		//如果是interrupt2 ，他的初值是可以自动重装的
		//，不用再写初值
		count++;
		if(count == 5000)
		{
				count = 0;
				led = ~led;
		}
 }
 