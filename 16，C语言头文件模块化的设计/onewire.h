#ifndef __ONEWIRE_H
#define __ONEWIRE_H

//这里面的函数也要自己写，要补充初始函数
bit init_ds18b20(void);
void Write_DS18B20(unsigned char dat);
unsigned char Read_DS18B20(void);

#endif
