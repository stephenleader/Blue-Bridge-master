//在CT107D单片机综合实训平台上，产生PWM脉宽调制信号控制8个LED灯进行呼吸流水变化，具体功能要求如下：

//      [1] 上电开机运行时，关闭蜂鸣器和继电器，L4和L5点亮，其余LED灯熄灭。

//      [2] 点按独立按键S4松开后，开始控制CT107D板上的L1-L8八个LED小灯进行每隔1秒的呼吸流水点亮，即：L1缓慢亮->L1缓慢灭->L2缓慢亮->L2缓慢灭....L8缓慢亮->L8缓慢灭->L1缓慢亮->L1缓慢灭....循环往复。

//      [3] 再次点按独立按键S4松开后，控制CT107D板上的LED灯从当前灯开始逆向呼吸流水。即：比如当前的流水是L2缓慢灭->L3缓慢亮，按下S4按键松开后，L3缓慢灭->L2缓慢亮->L2缓慢灭->L1缓慢亮->L1缓慢灭->L8缓慢亮->L8缓慢灭....循环往复。

//      [4] 每个LED灯，缓慢点亮0.5秒，缓慢熄灭0.5秒。

//      [5] 按下独立按键S4时，当前LED灯暂停流水变化并保持现有亮度，直至按键松开后，亮度才开始变化。待当前的亮度变化完成后，才开始调整呼吸流水的方向。

//      [6] 按下独立按键S7时，在数码管上显示当前LED灯的位置和PWM信号占空比，松开后，数码管熄灭。在最左边的1位数码管显示LED灯的位置，在最右边的2位数码管显示PWM信号的占空比。例如：当前点亮L6灯光，PWM信号的占空比为30%，在数码管最左边的1位显示“6”，在最右边的2位数码管显示“30”，其余的数码管熄灭。

//      [7] 按下独立按键S7时，当前LED灯暂停流水变化并保持现有亮度，直至按键松开后，亮度才开始变化。
//————————————————
//版权声明：本文为CSDN博主「小蜜蜂老师」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
//原文链接：https://blog.csdn.net/ohy3686/article/details/88372244

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