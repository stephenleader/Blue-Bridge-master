#include "reg52.h"

//�Ȱ�λ������
sbit HC138_A = P2^5;
sbit HC138_B = P2^6;
sbit HC138_C = P2^7;


//����
void Delay(unsigned int t)
{
		while(t--);
		while(t--);
}

void LEDRunning()
{
		unsigned char i;
		HC138_C = 1;
		HC138_B = 0;
		HC138_A = 0;
		
		//��˸����
		for(i = 0 ; i < 3 ; i++ )
		{
				P0 = 0x00;
				//0X00����IO����˵�ǵ���
				Delay(60000);
				Delay(60000);

				P0 = 0xff;
				//0xff����IO����Ϩ��
				Delay(60000);
				Delay(60000);
		}
		
		//ѭ��֮����������,����8���ƣ�ÿ���ƶ�һλ
		for(i = 1 ; i <= 8 ; i++ )
		{
		//0xff��ȫ��0x00����=1�ߵ�ƽ��=0�͵�ƽ
				P0 = 0xff << i;
				//��ˮ��һ��Ҫ��ʱ����Ȼ������Ч��
				Delay(60000);
				Delay(60000);
		}
		
		for( i = 1 ; i <= 8 ; i++ )
		{
				P0 = ~(0xff << i);
				Delay(60000);
				Delay(60000);
		}
}

//������
void main()
{
		while(1)
		{
				LEDRunning();//�������ڵ��ú���
		}
}

