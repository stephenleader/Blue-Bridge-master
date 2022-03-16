#include "reg52.h" 



sfr AUXR=0x8e;



unsigned char tmpRecv;

void Init_Uart()

{

        TMOD=0x20; 

        TH1=0xfd;

        TL1=0xfd; 

        AUXR=0x00;

        TR1=1; 

        SCON = 0x50; 

        ES=1; 

        EA=1; 

}



void SendByte(unsigned char dat)

{

        SBUF = dat; 

        while(TI == 0);

        TI = 0; 

}



void Sevice_Uart() interrupt 4

{

        if(RI == 1) 

        {

                RI = 0; 

                tmpRecv = SBUF;

                SendByte(tmpRecv + 1);

        }
}



void main()

{

        Init_Uart();

        SendByte(0x11);

        SendByte(0x13);

        while(1);

}