//��1������ԭ��
//     ����������ģ����ĳһ�����䳬�������ڷ���ʱ�̵�ͬʱ��ʼ��ʱ���������ڿ����д�����;�������ϰ�����������������������������յ����䲨������ֹͣ��ʱ��

//��2�����㹫ʽ
//    ���� = ���� �� ���������������շ��ص�ʱ��/2

//    ��Ϊ�������ڿ����еĴ����ٶ����¶�Ӱ�졣�����ڿ����еĴ����ٶ����¶ȵĹ�ϵ�ǣ�

//    ��20���϶�ʱ������V = 332 + 0.607t  (m/s)

//    ���ó��������о��������Ҫ�󾫶�һ�㶼���ߣ�ȡ20���϶�ʱ�������344M/S���м��㣬�ٽ����ʵ��Ĳ����Ϳ��ԡ����ԣ�����㹫ʽΪ������L = 344M/S * T / 2 = 172M/S * T ��

//��3��ʵ�ֲ���
//  1-����8��40KHz�ĳ������źţ�ͨ��TX���ŷ����ȥ��

//  2-������ʱ��������������塣

//  3-�ȴ��������źŷ��أ�������յ�����������źţ�RX���ű�Ϊ�͵�ƽ��

//  4-ֹͣ��ʱ������ȡ��������������ʱ��T��

//  5-���ݹ�ʽ��L = V * T /2m�����о���ļ��㡣

//��4�����ű�ѵ������
//    �����ű�����Ƭ������뿪���������У������������һֱ�������ǹ�������Ҫ�Ŀ��㣬����ÿ��ؿ�����Ȼʡ����û�п�������Ҳ�����ų��Ժ󲻿�Ŷ���ܶ�ͬѧ����ó����������ѣ�ԭ���ж���һ����������������ģ�鲻ͬ��û�еײ����������ṩ������ͬѧ�ǲ��˽��乤��ԭ����ʵ�ֲ��衣��ʵ�����������һ�㶼���ѣ�ϣ��С�۷���ʦ�������ܹ��ﵽ�㡣

//    �����ű���Ƭ���ľ����ۺ�ƽ̨CT107D�У�������ģ���TX���Žӵ���Ƭ����P1.0�˿ڣ�RX���Žӵ���Ƭ����P1.1�˿ڡ���Ƴ��򣬽��г�������࣬��������ΧԼ140cm�����������cm��Ϊ��ʾ��λ����ʾ������ܵ����ұ���λ������������볬����Χ�����ԡ�F����Ϊ��־����ʾ������ܵ������һλ��
#include "reg52.h"
#include "absacc.h"
#include "intrins.h"
 
sbit TX = P1^0;
sbit RX = P1^1;
 
unsigned int distance = 0;
 
unsigned char code SMG_duanma[18]=
		{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,
		 0x88,0x80,0xc6,0xc0,0x86,0x8e,0xbf,0x7f};
 
void DelaySMG(unsigned int t)
{
	while(t--);
}
 
void DisplaySMG_Bit(unsigned char pos, unsigned char value)
{
	XBYTE[0xE000] = 0xFF;
	XBYTE[0xC000] = 0x01 << pos;
	XBYTE[0xE000] = value;
}
 
void Display_Distance()
{
	if(distance == 999)
	{
		DisplaySMG_Bit(0, SMG_duanma[15]);			//����������Χ��־��F
		DelaySMG(500);
	}
	else
	{
		DisplaySMG_Bit(5, SMG_duanma[distance / 100]);
		DelaySMG(500);
		DisplaySMG_Bit(6, SMG_duanma[(distance % 100) / 10]);
		DelaySMG(500);
		DisplaySMG_Bit(7, SMG_duanma[distance % 10]);
		DelaySMG(500);
	}
}
 
void Delay12us()                  //@12.000MHz ��ʱ12us
{
	unsigned char i;
 
	_nop_();
	_nop_();
	i = 33;
	while (--i);
}
 
void Send_Wave()                 //����8��40KHx�������ź�
{
	unsigned char i;
	for(i = 0; i < 8; i++)
	{
		TX = 1;
		Delay12us();	
		TX = 0;
		Delay12us();
	}
}
 
void Measure_Distance()	        //���������
{
	unsigned int time = 0;
	
	TMOD &= 0x0f;	            //��ʱ��1ģʽ0��13λ�����8192����������								
	TL1 = 0x00;										
	TH1 = 0x00;		
	
	Send_Wave();		        //���ͳ������ź�							
	TR1 = 1;            //������ʱ��						
	while((RX == 1) && (TF1 == 0));    //�ȴ��������źŷ��ػ��ߵȵ�����������Χ
	TR1 = 0;            //ֹͣ��ʱ��				
	
	if(TF1 == 0)	            //����������Χ							
	{
		time = TH1;									
		time = (time << 8) | TL1;		
		distance = ((time / 10) * 17) / 100 + 3;
	}
	else                        //����������Χ			
	{
		TF1 = 0;
		distance = 999;
	}
}
 
void Delay(unsigned char n)        //�������ʾ��ǿ
{
	while(n--)
	{
		Display_Distance();
	}
}
 
void main()
{
	while(1)
	{
		Measure_Distance();
		Delay(10);
	}
}