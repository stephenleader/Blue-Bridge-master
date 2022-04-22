//��ĿҪ��
//      �����ű���������Ƭ������뿪��������CT107D�ۺ�ѵ��ƽ̨�ϣ����þ����ṩ��IIC�ײ��������룬��Ƴ���ʵ�����µĹ��ܣ�

//    1- ϵͳ�ϵ��ͨ������PCF8591ͨ��1�й�������RD1��ʵʱ���ݣ������ݸ����ݿ���8��LED�Ƶ����������

//       �ƹ�Ŀ��Ʒ�Ϊ5���ȼ���

//       �ȼ�1��255 <= �������� < 200��L1������

//       �ȼ�2��200 <= �������� < 150��L1~L2������

//       �ȼ�3��150 <= �������� < 100��L1~L4������

//       �ȼ�4��100 <= �������� < 50��L1~L6������

//       �ȼ�5��50 <= �������� < 0��L1��L8������
//    2- �ƹ���Ƶȼ��͹�������ʵʱ��ʾ��������ϣ���������ʾ���ƹ�ȼ����󣬹���ʵʱ�������ҡ�

//    3- ����S4����������ǰ�ĵƹ���Ƶȼ��͹�����������ݱ��浽�洢��24C02��0x01��0x02�ڴ浥Ԫ�С�

//    4- ����S5����ʱ����ȡ�洢��24C02����ʷ���ݲ���ʾ��������ϣ��ɿ�S5�����󣬻ָ���ʾʵʱ���ݡ�

#include "reg52.h"
#include "absacc.h"
#include "iic.h"

sbit S5 = P3^2;
sbit S4 = P3^3;

unsigned char dat_rd1 = 0;		//��������ʵʱ����
unsigned char level = 0;		//��ǰ�ƹ���Ƶȼ�
unsigned char bak_level = 0;	//����������ʷ����
unsigned char bak_rd1 = 0;		//��ʷ�ƹ���Ƶȼ�

//�������ʾ��������
unsigned char code SMG_duanma[18]=
		{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,
		 0x88,0x80,0xc6,0xc0,0x86,0x8e,0xbf,0x7f};
//�򵥵���ʱ����
void DelaySMG(unsigned int t)
{
	while(t--);
}
/*=====================��ʾ��λ����ܺ���=====================*/
void DisplaySMG_Bit(unsigned char pos, unsigned char value)
{
	XBYTE[0xE000] = 0xFF;				//����
	XBYTE[0xC000] = 0x01 << pos;		//ѡ������ܵ�λ��
	XBYTE[0xE000] = value;				//����������ʾ������
}

/*=====================��ʾ��ǰ��ʵʱ����=====================*/
void Display_now()
{
	DisplaySMG_Bit(0, SMG_duanma[16]);			
	DelaySMG(500);
	DisplaySMG_Bit(1, SMG_duanma[level]);
	DelaySMG(500);
	DisplaySMG_Bit(2, SMG_duanma[16]);
	DelaySMG(500);
			
	DisplaySMG_Bit(3, 0xFF);
	DelaySMG(500);
	DisplaySMG_Bit(4, 0xFF);
	DelaySMG(500);
	
	DisplaySMG_Bit(5, SMG_duanma[dat_rd1 / 100]);//��λ
	DelaySMG(500);
	DisplaySMG_Bit(6, SMG_duanma[(dat_rd1 % 100) / 10]);//ʮλ
	DelaySMG(500);
	DisplaySMG_Bit(7, SMG_duanma[dat_rd1 % 10]);//��λ
	DelaySMG(500);
}


/*=====================��ʾ��ȡ����ʷ����=====================*/
void Display_old()
{
	DisplaySMG_Bit(0, SMG_duanma[16]);//�������һ�������
	DelaySMG(500);
	DisplaySMG_Bit(1, SMG_duanma[bak_level]);
	DelaySMG(500);
	DisplaySMG_Bit(2, SMG_duanma[16]);
	DelaySMG(500);
			
	DisplaySMG_Bit(3, 0xFF);
	DelaySMG(500);
	DisplaySMG_Bit(4, 0xFF);
	DelaySMG(500);
	
	DisplaySMG_Bit(5, SMG_duanma[bak_rd1 / 100]);
	DelaySMG(500);
	DisplaySMG_Bit(6, SMG_duanma[(bak_rd1 % 100) / 10]);
	DelaySMG(500);
	DisplaySMG_Bit(7, SMG_duanma[bak_rd1 % 10]);
	DelaySMG(500);
}

//**����XBYTE�Ķ�Ӧ�Ķ���**
//XBYTE[0x8000]= 0x00;LED ָʾ��ȫ������
//XBYTE[0x8000] = 0xff;LEDָʾ��ȫ��Ϩ��
//XBYTE[0xE000] = 0xFF;�����ЧӦ

/*===================����PCF8591ͨ��1������===================*/
void Read_RD1()
{
	IIC_Start();					//��ʼ�ź�
	IIC_SendByte(0x90); 			//�豸д��ַ
	IIC_WaitAck();  				//�ȴ�Ӧ��
	IIC_SendByte(0x01); 			//���ÿ��ƼĴ���
	IIC_WaitAck();  				//�ȴ�Ӧ��
	IIC_Stop(); 					//ֹͣ�ź�
	
	IIC_Start();					//��ʼ�ź�
	IIC_SendByte(0x91); 			//�豸����ַ
	IIC_WaitAck(); 				    //�ȴ�Ӧ��
	dat_rd1 = IIC_RecByte(); 	    //��ȡ��ǰͨ��1������
	IIC_Stop(); 					//ֹͣ�ź�
}

/*======================�ƹ�ȼ����ƺ���======================*/
void Right_Auto()
{
	Read_RD1();						//��ȡ���������ʵʱ����
	if(dat_rd1 > 200)				//�ȼ�1
	{
		XBYTE[0x8000] = 0xfe;		//����L1,[0x8000]��Ӧled
		level = 1;
	}
	else if(dat_rd1 > 150)		    //�ȼ�2
	{
		XBYTE[0x8000] = 0xfc;		//����L1~L2
		level = 2;
	}
	else if(dat_rd1 > 100)		    //�ȼ�3
	{
		XBYTE[0x8000] = 0xf0;		//����L1~L4
		level = 3;
	}
	else if(dat_rd1 > 50)			//�ȼ�4
	{
		XBYTE[0x8000] = 0xc0;		//����L1~L6
		level = 4;
	}
	else							//�ȼ�5
	{
		XBYTE[0x8000] = 0x00;		//����L1~L8
		level = 5;
	}
}
/*=======================24C02�ֽ�д����=======================*/
void Write_24C02(unsigned char addr, unsigned char dat)
{
	IIC_Start();					//��ʼ�ź�					
	IIC_SendByte(0xa0); 			//�豸д��ַ
	IIC_WaitAck();					//�ȴ�Ӧ��
	IIC_SendByte(addr); 			//�����ڴ��ֽڵ�ַ
	IIC_WaitAck(); 					//�ȴ�Ӧ��
	IIC_SendByte(dat); 				//д��Ŀ������
	IIC_WaitAck();					//�ȴ�Ӧ��
	IIC_Stop();						//ֹͣ�ź�
}

/*=======================24C02�ֽڶ�����=======================*/
unsigned char Read_24C02(unsigned char addr)
{
	unsigned char tmp;
	IIC_Start();					//��ʼ�ź�			
	IIC_SendByte(0xa0); 			//�豸д��ַ
	IIC_WaitAck();					//�ȴ�Ӧ��
	IIC_SendByte(addr); 			//�����ڴ��ֽڵ�ַ
	IIC_WaitAck(); 					//�ȴ�Ӧ��
 
	IIC_Start();					//��ʼ�ź�
	IIC_SendByte(0xa1); 			//�豸����ַ
	IIC_WaitAck();					//�ȴ�Ӧ��
	tmp = IIC_RecByte();			//����Ŀ������
	IIC_Stop();						//ֹͣ�ź�
	return tmp;
}


/*=======================����ɨ�账����=======================*/
void Scan_Keys()
{
	if(S4 == 0)								
	{
		DelaySMG(100);
		if(S4 == 0)
		{
			//����ǰ�ĵƹ���Ƶȼ��͹�����������д��24C02�洢��
			Write_24C02(0x01, level);
			DelaySMG(1000);				//�ȴ�����д�����
			Write_24C02(0x02, dat_rd1);
			
			while(S4 == 0)
			{
				Display_now();			//�ڵȴ������ɿ�ʱˢ�������
			}
		}
	}
	if(S5 == 0)
	{
		DelaySMG(100);
		if(S5 == 0)
		{
			//����24C02�е���ʷ����
			bak_level = Read_24C02(0x01);
			bak_rd1 = Read_24C02(0x02);
			while(S5 == 0)
			{
				Display_old();		//��S5��������ʱ����ʾ��ʷ����
			}
		}
	}
}
/*============================������============================*/
void main()
{
	while(1)
	{
		Right_Auto();			//��������ʵʱ�����͵ƹ����
		Display_now();			//����ܶ�̬��ʾ
		Scan_Keys();			//����ɨ�账��
	}
}







