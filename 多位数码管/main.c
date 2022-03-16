#include <reg52.h>
#define u8 unsigned char
#define u16 unsigned int

#include <stdlib.h>

//位选
//code ROM
//u8 tab[34] = {};

//定义数组的时候要写一个code
//不加code的时候，计算机必然要分配内存空间，
//不加code会分配到RAM里面，由于51单片机存储空间小，容易导致出现内存问题
//当加入code的时候，会存入ROM，存储空间更大
u8 code seg_tab[] ={
 0x40,0x79,0x24,0x30,0x19
 ,0x12,0x02,0x78,0x00,0x10};
 
 void delay_ms(u16 num);//函数声明
 
 //void main()
 //{
		//while(1)
		//{
		//	delay_ms(200);
		//		P3 = 0x01;//位选
			//	P2 = seg_tab[4];//段选：选择一段,
				//显示数字对应数码管
				
				
				//第二位
			//	delay_ms(200);
			//	P3 = 0x02;
			//	P2 = seg_tab[9];
	//	}
 //}
 
 //尝试写一个循环
	void main()
	{	u8 num = 0;//计数0-60
			u16 shiwei,gewei;
			while(1)//循环显示0-60
			{
			//eg.34 十位为3，个位为4
		
			
			shiwei = num/10;//取模34/10 = 3
			gewei = num% 10;//取余34%10 = 4
			
			delay_ms(50);
			P3 = 0x01;//高位的位选（第一位）
			P2 = seg_tab[shiwei];
			
			delay_ms(50);
			P3 = 0x02;//低位选择第二位
			P2 = seg_tab[gewei];
			
			
			num++;
			
			if(num == 60 ){
			num = 0;
			}
					
			}
	}
	void delay_ms(u16 num)//u16 最大值：65535
	//u8 256 8位的
	{
			u16 x,y; //222222
			for ( x = num ; x > 0 ; x-- )
					for ( y = 110 ; y > 0 ; y-- );
	}