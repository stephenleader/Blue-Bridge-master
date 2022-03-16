#include "reg52.h"

sbit HC138_A = P2^5;
sbit HC138_B = P2^6;
sbit HC138_C = P2^7;



//����
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
				HC138_C = 1;  //ѡY4����͵�ƽ
				HC138_B = 0;
				HC138_A = 0;
				break;
		
		case 5:
				HC138_C = 1;
				HC138_B = 0;  //ѡY5����͵�ƽ
				HC138_A = 1;
				break;
				
		case 6:
				HC138_C = 1;
				HC138_B = 1;  //ѡY6����͵�ƽ
				HC138_A = 0;
				break;
		
		
		case 7:
				HC138_C = 1;
				HC138_B = 1;  //ѡY7����͵�ƽ
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
		//y4��Ϊ�ߵ�ƽ
		//---��Ϊ�������Ż���
		//InitHC138(4);
		
		//��˸����
		for(i = 0 ; i < 3 ; i++ )
		{
				OutPutP0(4,0x00);
				//P0 = 0x00;
				//0X00����IO����˵�ǵ���,�͵�ƽȻ��ȡ��һ�η�
				Delay(60000);
				Delay(60000);

				OutPutP0(4,0xff);
				//P0 = 0xff;
				//0xff����IO����Ϩ��
				Delay(60000);
				Delay(60000);
		}
		
		for(i = 1 ; i <= 8 ; i++ )
		{
		//0xff��ȫ��0x00����=1�ߵ�ƽ��=0�͵�ƽ
		OutPut(4,(0xff<<i));
				//P0 = 0xff << i;
				//��ˮ��һ��Ҫ��ʱ����Ȼ������Ч��
				Delay(60000);
		}	
		

		//����֮�������⣬�̵������ϣ�����P0^4��Ӧ�ļ̵�����ͨ��P0^4 = 1
		/*HC138_C = 1;
		HC138_B = 0;
		HC138_A = 1;
		//������0001 0000 ,P0^4�պ�Ϊ1
		//---��Ϊ�������Ż���*/
		//InitHC138(5);
		//P0 = 0x10;
		OutPutP0(5,0x10);
		Delay(60000);
		Delay(60000);
		//P0 = 0x00;�ص�
		OutPutP0(5,0x00);
		
		
		//����y4����͵�ƽ,��Ϊǰ��Ķ���C,B,A����������Ҫ����д
		/*HC138_C = 1;
		HC138_B = 0;
		HC138_A = 0;*/
		//---��Ϊ�������Ż���
		InitHC138(4);
		for( i = 1 ; i <= 8 ; i++ )
		{
				P0 = ~(0xff << i);
				Delay(60000);
		}
		
		//�̵�������֮�󣬸������⣬��������Ӧ�ӿ�P0^6Ӧ��Ϊ1
		//����һ���
		/*HC138_C = 1;
		HC138_B = 0;
		HC138_A = 1;//y5����͵�ƽ*/
		//---��Ϊ�������Ż���
		InitHC138(5);
		P0 = 0x40;//ע��0100 0000��Ӧ����P0^7,P0^6....
		Delay(60000);
		Delay(60000);
		P0 = 0x00;//�ص�
		
}

//�ص��޹��豸�ĺ���
void InitSystem()
{
		HC138_C = 1;
		HC138_B = 0;
		HC138_A = 1;
		//Y5����͵�ƽ,LE����Ϊ1������Ӱ�쵽���з������ͼ̵����Ŀ�����
		
		P0 = 0X00;
		//P0^0~P0^7����P0Ӱ��
		
}


//������
void main()
{
		InitSystem();
		while(1)
		{
				LEDRunning();//�������ڵ��ú���
		}
}

