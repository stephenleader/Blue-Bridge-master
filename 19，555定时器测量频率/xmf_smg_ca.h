//Ò»¸öÊÇ¶¨ÒåµÄ£¬Ò»¸öÊÇÃ»ÓĞ¶¨Òå£¬Õâ¸ö±ÜÃâ³åÍ»

#ifndef XMF_SMG_CA_H
#define XMF_SMG_CA_H
//Í¨³£»¹¿ÉÒÔ·Åºê¶¨Òå
//0~9(Ã»ÓĞµã£©
//¹²Ñô¼«ÊıÂë¹ÜÂëÖµ
unsigned char code SMGNoDot_CA[10]={
		0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};

//0.~9.
unsigned char code SMGDot_CA[10]={
		0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};	

		
//»¹ĞèÒª¶ÔÓ¦¡/cÎÄ¼şµÄº¯ÊıÉùÃ÷
void DelaySMG(unsigned int t);
void DisplaySMG_Bit(unsigned char pos, unsigned char dat);
void Display_All(unsigned char dat);

#endif