//ע�⿴DS18B20�¶�ת���Ͷ�ȡ����.png

#include "reg52.h"
#include "absacc.h"
#include "onewire.h"
#include "xmf_smg_ca.h"  //�Լ������ͷ�ļ���c�ļ����ص��ø�Ŀ¼��

//�����¶ȱ���
unsigned int temp = 0;

//����������
//567
//  567 / 100 = 5 = S
//  567 % 100 = 67 / 10 = 6 
//  567 % 10 = 7
void DisplaySMG_Temp()
{
	DisplaySMG_Bit(7,SMGNoDot_CA[temp % 10]);	 //��λ
	DelaySMG(100);
	DisplaySMG_Bit(6,SMGDot_CA[(temp % 100) / 10]);	 //ʮλ
	DelaySMG(100);
	DisplaySMG_Bit(5,SMGNoDot_CA[temp / 100]);	  //��λ
	DelaySMG(100);
	
	
	DisplaySMG_Bit(4,0xff);	
	DelaySMG(100);
	DisplaySMG_Bit(3,0xff);	
	DelaySMG(100);
	DisplaySMG_Bit(2,0xff);	
	DelaySMG(100);
	DisplaySMG_Bit(1,0xff);	
	DelaySMG(100);
	DisplaySMG_Bit(0,0xff);	
	DelaySMG(100);
	
	Display_All(0xff);

}

//����ܶ�̬��ʾ����ʱ
void Delay(unsigned int t)
{
	while(t--)
	{
		DisplaySMG_Temp();
	}
}

//onewire��h�����ĺ���Ҫ����
//��onwire.h
//�鿴��Ŀ¼�µ�png
//ע��ת���Ĳ���
void Read_DS18B20_temp()
{
	unsigned char LSB,MSB;//low 8 bit ,high 8 bit
	
	init_ds18b20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0x44);
	
	Delay(1000);
	
	init_ds18b20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0xbe);
	
	LSB = Read_DS18B20();
	MSB = Read_DS18B20();
	
	temp = MSB;
	temp = (temp << 8) | LSB;  //16λ�¶�����
	
	if((temp & 0xf800) == 0x0000)  //0�Ϲ���
	{
		temp >>= 4;//С������
		temp = temp * 10;
		temp = temp + (LSB & 0x0f) * 0.625;//�Ŵ�ʮ�����ֱ�����0���0625
	}
}

void main()
{
	XBYTE[8000] = 0x00;//���
	while(1)
	{
		Read_DS18B20_temp();
		DisplaySMG_Temp();
	}
}