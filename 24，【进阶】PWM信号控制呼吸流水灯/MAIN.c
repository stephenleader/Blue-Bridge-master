//��CT107D��Ƭ���ۺ�ʵѵƽ̨�ϣ�����PWM��������źſ���8��LED�ƽ��к�����ˮ�仯�����幦��Ҫ�����£�

//      [1] �ϵ翪������ʱ���رշ������ͼ̵�����L4��L5����������LED��Ϩ��

//      [2] �㰴��������S4�ɿ��󣬿�ʼ����CT107D���ϵ�L1-L8�˸�LEDС�ƽ���ÿ��1��ĺ�����ˮ����������L1������->L1������->L2������->L2������....L8������->L8������->L1������->L1������....ѭ��������

//      [3] �ٴε㰴��������S4�ɿ��󣬿���CT107D���ϵ�LED�ƴӵ�ǰ�ƿ�ʼ���������ˮ���������統ǰ����ˮ��L2������->L3������������S4�����ɿ���L3������->L2������->L2������->L1������->L1������->L8������->L8������....ѭ��������

//      [4] ÿ��LED�ƣ���������0.5�룬����Ϩ��0.5�롣

//      [5] ���¶�������S4ʱ����ǰLED����ͣ��ˮ�仯�������������ȣ�ֱ�������ɿ������Ȳſ�ʼ�仯������ǰ�����ȱ仯��ɺ󣬲ſ�ʼ����������ˮ�ķ���

//      [6] ���¶�������S7ʱ�������������ʾ��ǰLED�Ƶ�λ�ú�PWM�ź�ռ�ձȣ��ɿ��������Ϩ��������ߵ�1λ�������ʾLED�Ƶ�λ�ã������ұߵ�2λ�������ʾPWM�źŵ�ռ�ձȡ����磺��ǰ����L6�ƹ⣬PWM�źŵ�ռ�ձ�Ϊ30%�������������ߵ�1λ��ʾ��6���������ұߵ�2λ�������ʾ��30��������������Ϩ��

//      [7] ���¶�������S7ʱ����ǰLED����ͣ��ˮ�仯�������������ȣ�ֱ�������ɿ������Ȳſ�ʼ�仯��
//��������������������������������
//��Ȩ����������ΪCSDN������С�۷���ʦ����ԭ�����£���ѭCC 4.0 BY-SA��ȨЭ�飬ת���븽��ԭ�ĳ������Ӽ���������
//ԭ�����ӣ�https://blog.csdn.net/ohy3686/article/details/88372244

#include "regx52.h"
#include "absacc.h"
 
sbit S7 = P3^0;
sbit S4 = P3^3;
 
unsigned char pwm = 0;          
unsigned char pwm_duty = 0;              
unsigned char times = 0;        
unsigned char led_go = 0;       
unsigned char stat_go = 0;      
unsigned char stat = 0;         
unsigned char key_puse = 0;     
 
unsigned char code SMG_duanma[18]=
		{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,
		 0x88,0x80,0xc6,0xc0,0x86,0x8e,0xbf,0x7f};
 
void DelaySMG(unsigned int t)
{
	while(t--);
}
 
void DisplaySMG_Bit(unsigned char pos, unsigned char value)
{
	XBYTE[0xE000] = 0xff;
	XBYTE[0xC000] = 0x01 << pos;
	XBYTE[0xE000] = value;
}
 
void Display_Info(unsigned char pos, unsigned char duty)
{
	DisplaySMG_Bit(0, SMG_duanma[pos]);
	DelaySMG(500);
 
	DelaySMG(500);
	DisplaySMG_Bit(6, SMG_duanma[duty / 10]);
	DelaySMG(500);
	DisplaySMG_Bit(7, SMG_duanma[duty % 10]);	
	DelaySMG(500);
	
	DisplaySMG_Bit(0, 0xff);
	DisplaySMG_Bit(6, 0xff);
	DisplaySMG_Bit(7, 0xff);
}
 
void Init_Timer0()
{
	TMOD = 0x01;
	TH0 = (65535 - 1000) / 256;			
	TL0 = (65535 - 1000) % 256;
	
	ET0 = 1;
	EA = 1;
	TR0 = 1;
}
 
void Service_Timer0() interrupt 1
{
	TH0 = (65535 - 1000) / 256;
	TL0 = (65535 - 1000) % 256;
	
	if(stat_go == 0)      						
      {
        XBYTE[0x8000] = 0xe7;						
        return;
      } 
	
        pwm++;                
  
      if(pwm <= pwm_duty)   
      {
	XBYTE[0x8000] = ~(0x01 << stat);
      }
      else if(pwm <= 10)
      {
        XBYTE[0x8000] = 0xff;
      }
      else
      {
        XBYTE[0x8000] = ~(0x01 << stat);
    
        pwm = 0;
        if(key_puse == 0)     
        {
          times++;
        }
      }
}
 
void LED_Control()
{
  if(times == 5)          
  {
    times = 0;
    if(led_go == 0)       
    {
      pwm_duty = pwm_duty + 1;
      if(pwm_duty == 11)
      {
				pwm_duty = 10;
        led_go = 1;
      }
    }
    else if(led_go == 1)  
    {
      pwm_duty = pwm_duty - 1;
      if(pwm_duty == 255)
      {
				pwm_duty = 0;
        led_go = 0;
        
        if(stat_go == 1)      
        {
          stat++;
          if(stat == 8)
          {
            stat = 0;
          }
        }
        else if(stat_go == 2) 
        {
          stat--;
          if(stat == 255)
          {
            stat = 7;
          }
        }
      }
    }
  }
}
 
void Scan_Keys()
{
	if(S4 == 0)
	{
		DelaySMG(100);
		if(S4 == 0)
		{
			while(S4 == 0)
			{
				key_puse = 1;
			}
			key_puse = 0;
			
      stat_go++;            
      if(stat_go == 3)
      {
        stat_go = 1;
      }
		}
	}
	
	if(S7 == 0)
	{
		DelaySMG(100);
		if(S7 == 0)
		{
			while(S7 == 0)
			{
				key_puse = 1;
				Display_Info(stat + 1, pwm_duty * 10);
			}
			key_puse = 0;
		}
	}
}
 
void Init_System()
{
	XBYTE[0xA000] = 0xff;
	XBYTE[0xA000] = 0x00;
	XBYTE[0xE000] = 0xff;
	XBYTE[0xC000] = 0xff;
	
	Init_Timer0();
}
 
main()
{
	Init_System();
	while(1)
	{
		LED_Control();
		Scan_Keys();
	}
}