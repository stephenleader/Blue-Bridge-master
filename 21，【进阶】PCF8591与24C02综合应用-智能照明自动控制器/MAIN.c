//题目要求
//      在蓝桥杯大赛“单片机设计与开发”的在CT107D综合训练平台上，利用竞赛提供的IIC底层驱动代码，设计程序，实现以下的功能：

//    1- 系统上电后，通过采样PCF8591通道1中光敏电阻RD1的实时数据，并根据该数据控制8个LED灯的亮灭情况。

//       灯光的控制分为5个等级：

//       等级1：255 <= 光照数据 < 200，L1点亮。

//       等级2：200 <= 光照数据 < 150，L1~L2点亮。

//       等级3：150 <= 光照数据 < 100，L1~L4点亮。

//       等级4：100 <= 光照数据 < 50，L1~L6点亮。

//       等级5：50 <= 光照数据 < 0，L1～L8点亮。
//    2- 灯光控制等级和光敏数据实时显示在数码管上，如下如所示，灯光等级在左，光敏实时数据在右。

//    3- 按下S4按键，将当前的灯光控制等级和光敏电阻的数据保存到存储器24C02的0x01和0x02内存单元中。

//    4- 按下S5按键时，读取存储在24C02的历史数据并显示在数码管上，松开S5按键后，恢复显示实时数据。

#include "reg52.h"
#include "absacc.h"
#include "iic.h"

sbit S5 = P3^2;
sbit S4 = P3^3;

unsigned char dat_rd1 = 0;		//光敏电阻实时数据
unsigned char level = 0;		//当前灯光控制等级
unsigned char bak_level = 0;	//光敏电阻历史数据
unsigned char bak_rd1 = 0;		//历史灯光控制等级

//数码管显示段码数组
unsigned char code SMG_duanma[18]=
		{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,
		 0x88,0x80,0xc6,0xc0,0x86,0x8e,0xbf,0x7f};
//简单的延时函数
void DelaySMG(unsigned int t)
{
	while(t--);
}
/*=====================显示单位数码管函数=====================*/
void DisplaySMG_Bit(unsigned char pos, unsigned char value)
{
	XBYTE[0xE000] = 0xFF;				//消隐
	XBYTE[0xC000] = 0x01 << pos;		//选择数码管的位置
	XBYTE[0xE000] = value;				//输出数码管显示的内容
}

/*=====================显示当前的实时数据=====================*/
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
	
	DisplaySMG_Bit(5, SMG_duanma[dat_rd1 / 100]);//百位
	DelaySMG(500);
	DisplaySMG_Bit(6, SMG_duanma[(dat_rd1 % 100) / 10]);//十位
	DelaySMG(500);
	DisplaySMG_Bit(7, SMG_duanma[dat_rd1 % 10]);//个位
	DelaySMG(500);
}


/*=====================显示读取的历史数据=====================*/
void Display_old()
{
	DisplaySMG_Bit(0, SMG_duanma[16]);//最左边那一个数码管
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

//**补充XBYTE的对应的东西**
//XBYTE[0x8000]= 0x00;LED 指示灯全部点亮
//XBYTE[0x8000] = 0xff;LED指示灯全部熄灭
//XBYTE[0xE000] = 0xFF;数码管效应

/*===================采样PCF8591通道1的数据===================*/
void Read_RD1()
{
	IIC_Start();					//起始信号
	IIC_SendByte(0x90); 			//设备写地址
	IIC_WaitAck();  				//等待应答
	IIC_SendByte(0x01); 			//设置控制寄存器
	IIC_WaitAck();  				//等待应答
	IIC_Stop(); 					//停止信号
	
	IIC_Start();					//起始信号
	IIC_SendByte(0x91); 			//设备读地址
	IIC_WaitAck(); 				    //等待应答
	dat_rd1 = IIC_RecByte(); 	    //读取当前通道1的数据
	IIC_Stop(); 					//停止信号
}

/*======================灯光等级控制函数======================*/
void Right_Auto()
{
	Read_RD1();						//读取光敏电阻的实时数据
	if(dat_rd1 > 200)				//等级1
	{
		XBYTE[0x8000] = 0xfe;		//点亮L1,[0x8000]对应led
		level = 1;
	}
	else if(dat_rd1 > 150)		    //等级2
	{
		XBYTE[0x8000] = 0xfc;		//点亮L1~L2
		level = 2;
	}
	else if(dat_rd1 > 100)		    //等级3
	{
		XBYTE[0x8000] = 0xf0;		//点亮L1~L4
		level = 3;
	}
	else if(dat_rd1 > 50)			//等级4
	{
		XBYTE[0x8000] = 0xc0;		//点亮L1~L6
		level = 4;
	}
	else							//等级5
	{
		XBYTE[0x8000] = 0x00;		//点亮L1~L8
		level = 5;
	}
}
/*=======================24C02字节写函数=======================*/
void Write_24C02(unsigned char addr, unsigned char dat)
{
	IIC_Start();					//起始信号					
	IIC_SendByte(0xa0); 			//设备写地址
	IIC_WaitAck();					//等待应答
	IIC_SendByte(addr); 			//发送内存字节地址
	IIC_WaitAck(); 					//等待应答
	IIC_SendByte(dat); 				//写入目标数据
	IIC_WaitAck();					//等待应答
	IIC_Stop();						//停止信号
}

/*=======================24C02字节读函数=======================*/
unsigned char Read_24C02(unsigned char addr)
{
	unsigned char tmp;
	IIC_Start();					//起始信号			
	IIC_SendByte(0xa0); 			//设备写地址
	IIC_WaitAck();					//等待应答
	IIC_SendByte(addr); 			//发送内存字节地址
	IIC_WaitAck(); 					//等待应答
 
	IIC_Start();					//起始信号
	IIC_SendByte(0xa1); 			//设备读地址
	IIC_WaitAck();					//等待应答
	tmp = IIC_RecByte();			//读出目标数据
	IIC_Stop();						//停止信号
	return tmp;
}


/*=======================按键扫描处理函数=======================*/
void Scan_Keys()
{
	if(S4 == 0)								
	{
		DelaySMG(100);
		if(S4 == 0)
		{
			//将当前的灯光控制等级和光敏电阻数据写入24C02存储器
			Write_24C02(0x01, level);
			DelaySMG(1000);				//等待数据写入完成
			Write_24C02(0x02, dat_rd1);
			
			while(S4 == 0)
			{
				Display_now();			//在等待按键松开时刷新数码管
			}
		}
	}
	if(S5 == 0)
	{
		DelaySMG(100);
		if(S5 == 0)
		{
			//读出24C02中的历史数据
			bak_level = Read_24C02(0x01);
			bak_rd1 = Read_24C02(0x02);
			while(S5 == 0)
			{
				Display_old();		//在S5按键按下时，显示历史数据
			}
		}
	}
}
/*============================主函数============================*/
void main()
{
	while(1)
	{
		Right_Auto();			//光敏数据实时采样和灯光控制
		Display_now();			//数码管动态显示
		Scan_Keys();			//按键扫描处理
	}
}







