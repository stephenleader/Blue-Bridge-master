#include <reg52.h>
#define u8 unsigned char
#define u16 unsigned int

#include <stdlib.h>

//λѡ
//code ROM
//u8 tab[34] = {};

//���������ʱ��Ҫдһ��code
//����code��ʱ�򣬼������ȻҪ�����ڴ�ռ䣬
//����code����䵽RAM���棬����51��Ƭ���洢�ռ�С�����׵��³����ڴ�����
//������code��ʱ�򣬻����ROM���洢�ռ����
u8 code seg_tab[] ={
 0x40,0x79,0x24,0x30,0x19
 ,0x12,0x02,0x78,0x00,0x10};
 
 void delay_ms(u16 num);//��������
 
 //void main()
 //{
		//while(1)
		//{
		//	delay_ms(200);
		//		P3 = 0x01;//λѡ
			//	P2 = seg_tab[4];//��ѡ��ѡ��һ��,
				//��ʾ���ֶ�Ӧ�����
				
				
				//�ڶ�λ
			//	delay_ms(200);
			//	P3 = 0x02;
			//	P2 = seg_tab[9];
	//	}
 //}
 
 //����дһ��ѭ��
	void main()
	{	u8 num = 0;//����0-60
			u16 shiwei,gewei;
			while(1)//ѭ����ʾ0-60
			{
			//eg.34 ʮλΪ3����λΪ4
		
			
			shiwei = num/10;//ȡģ34/10 = 3
			gewei = num% 10;//ȡ��34%10 = 4
			
			delay_ms(50);
			P3 = 0x01;//��λ��λѡ����һλ��
			P2 = seg_tab[shiwei];
			
			delay_ms(50);
			P3 = 0x02;//��λѡ��ڶ�λ
			P2 = seg_tab[gewei];
			
			
			num++;
			
			if(num == 60 ){
			num = 0;
			}
					
			}
	}
	void delay_ms(u16 num)//u16 ���ֵ��65535
	//u8 256 8λ��
	{
			u16 x,y; //222222
			for ( x = num ; x > 0 ; x-- )
					for ( y = 110 ; y > 0 ; y-- );
	}