#include <reg52.h>

#define SEG P0;
#define KEYPAD P1;

typedef unsigned char u8;
typedef unsigned int u16;

void delay_ms(u16 x);//��ʱ��������
void led_display(u8 number);//�������ʾ��ע���ʽʹ���ַ���
u8 keypad_scan(void);
//����һ�㶼�з���ֵ��voidʲô�������أ���������Ҫ����һ��u8
sbit key = P2^0;
sbit led = P3^0;


//��Ҫ*
void main()
{
		u8 num;
		while(1)
		{
				switch(keypad_scan())//����ɨ��������
				//��numberΪ0�İ�������k0����ʱ
				//��num=0
				{
				case 0xee:num = 0; break;
				case 0xed:num = 1; break;
				case 0xeb:num = 2; break;
				case 0xe7:num = 3; break;
				default: break;
				}
}


void delay_ms(u16 x)
{
	u16 i,j;
	for ( i = x ; i > 0 ; i-- )
	{
		for (j = 110 ; j > 0 ; j-- )
		{
		
		}
	}
}
void led_display(u8 number)
{
		SEG = seg_tab[number];
}