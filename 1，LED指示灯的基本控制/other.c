#include <STC15F2K60S2.H>
void close()
{
   P2 = 0XA0;
   P06 = 0;
   P2 = 0X00;

   P2 = 0X80;
   P0 = 0XFF;
   P2 = 0X00;
}  
void main()
{
  close();
  while(1)
  {
		
  }
}