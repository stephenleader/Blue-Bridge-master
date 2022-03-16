#include<reg52.h>
#define u8 unsigned char
#define u16 unsigned int

//一维数组
u8 code dis_tab[] = {
	0x40,0x79,0x24,0x30,0x19,
	0x12,0x02,0x78,0x00,0x10};
	
	void delay_ms(u16);//引用延时函数
	
	void main()
	{
		u8 i = 0;
		P3 = 0xff;
		while(1)
		{
			delay_ms(500);
			P3 = dis_tab[i];//遍历
			i++;
			i = i < 10 ? i % 10 : 0;
			}
		}
		
void delay_ms(u16 num)
{
	u16 x,y;
	for ( x = num; x > 0 ; x-- )
	for (y = 110 ; y > 0 ; y-- );
}