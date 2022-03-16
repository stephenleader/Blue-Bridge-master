#include "reg52.h"
typedef unsigned int u16;
u16 count;

sbit led = P1^0;
 void InitTimer0(void)
 {
		TMOD = 0x01;//���ù���ģʽ
		//�൱�ڷ�ʽ1
		TH0 = 0xFF;
		TL0 = 0x9c;//�Դ洢�����ó�ֵ��
		//ȡ�����趨��ʱ����ʱ��
		EA = 1;//�����ж�
		ET0 = 1;
		TR0 = 1;
 }
 
 void main(void)
 {//����������һ��Ҫ���ó�ʼ������
		InitTimer0();//T0��ʼ��
		led = 1;
		count = 0;
		while(1);//�������������һֱѭ��
 }
 
 //ÿһ�����������뵽��ʱ�����ж���Ӧ
 void Timer0Tnterrupt(void) interrupt 1
 {
		TH0 = 0xFF;
		TL0 = 0x9C;
		//����Ҫ���¸���һ����ֵ
		//�����interrupt2 �����ĳ�ֵ�ǿ����Զ���װ��
		//��������д��ֵ
		count++;
		if(count == 5000)
		{
				count = 0;
				led = ~led;
		}
 }
 