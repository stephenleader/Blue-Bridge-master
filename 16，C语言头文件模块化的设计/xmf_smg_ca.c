#include "absacc.h"
//�ײ㹫������

void DelaySMG(unsigned int t)
{
	while(t--);
}

void DisplaySMG_Bit(unsigned char pos, unsigned char dat)
{
	//����ܵ���
	XBYTE[0xe000] = 0xff;//����
	XBYTE[0xc000] = 0x01 << pos;//����
	XBYTE[0xe000] = dat;//����ʾ������
}

//���ö���ֵ����ȫ���������Ч
void Display_All(unsigned char dat)
{
	XBYTE[0xc000] = 0xff;
	XBYTE[0xe000] = dat;
}