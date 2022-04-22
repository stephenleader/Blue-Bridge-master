#include "absacc.h"
//底层公共部分

void DelaySMG(unsigned int t)
{
	while(t--);
}

void DisplaySMG_Bit(unsigned char pos, unsigned char dat)
{
	//数码管调用
	XBYTE[0xe000] = 0xff;//消隐
	XBYTE[0xc000] = 0x01 << pos;//左移
	XBYTE[0xe000] = dat;//所显示的内容
}

//设置段码值，是全部数码管有效
void Display_All(unsigned char dat)
{
	XBYTE[0xc000] = 0xff;
	XBYTE[0xe000] = dat;
}