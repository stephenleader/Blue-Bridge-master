#include <reg52.h>
typedef unsigned char u8;
typedef unsigned int u16;

void delay_ms(u16 x);//��ʱ��������
sbit key = P2^0;
sbit led = P3^0;



void main()
{
		while(1)
		{
				if(key == 0 )
				{
						delay_ms(100);
						if(key == 0)
						{
						
								led = 0;
						}else
						{
								led = 1;
						}
				}
		}
}

//��ʱ�����������а�����ʱ�����Ҫʹ����ʱ������������
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

