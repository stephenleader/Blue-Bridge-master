//�ⲿ�ж�ѧϰ

#include "reg52.h"
typedef unsigned int u16;
u16 count;
//Ҳ����д��unsigned int count;

sbit led = P1^0;
sbit key = P3^2;

 void ex0init(void)
 {
//	TMOD = 0x01;//���ù���ģʽ
		//�൱�ڷ�ʽ1
	//	TH0 = 0x0FF;
	//	TL0 = 0x9c;�Դ洢�����ó�ֵ��
		//ȡ�����趨��ʱ����ʱ��
		EA = 1;//�Ĵ���1�������ж�
		EX0 = 1;//�򿪶�ʱ������o(to)�ж�
		//TR0 = 1;
 }
 
 //������
 void main(void)
 {
		ex0init();//to��ʼ��
		led = 1;
		while(1);
}
//�����������ж���Ӧ
 void Ex0Interrupt(void) interrupt 0
 {
		led = ~led;
		while(!key);
 }
 