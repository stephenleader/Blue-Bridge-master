//外部中断学习

#include "reg52.h"
typedef unsigned int u16;
u16 count;
//也可以写成unsigned int count;

sbit led = P1^0;
sbit key = P3^2;

 void ex0init(void)
 {
//	TMOD = 0x01;//设置工作模式
		//相当于方式1
	//	TH0 = 0x0FF;
	//	TL0 = 0x9c;对存储器设置初值，
		//取决于设定计时多少时长
		EA = 1;//寄存器1，打开总中断
		EX0 = 1;//打开定时计数器o(to)中断
		//TR0 = 1;
 }
 
 //主函数
 void main(void)
 {
		ex0init();//to初始化
		led = 1;
		while(1);
}
//服务函数，即中断响应
 void Ex0Interrupt(void) interrupt 0
 {
		led = ~led;
		while(!key);
 }
 