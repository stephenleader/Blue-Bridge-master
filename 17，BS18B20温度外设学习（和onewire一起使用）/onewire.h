#ifndef __ONEWIRE_H
#define __ONEWIRE_H

//������ĺ���ҲҪ�Լ�д��Ҫ�����ʼ����
bit init_ds18b20(void);
void Write_DS18B20(unsigned char dat);
unsigned char Read_DS18B20(void);

#endif
