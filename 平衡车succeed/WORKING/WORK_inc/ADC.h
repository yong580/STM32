#ifndef __ADC_H
#define	__ADC_H

#include "stm32f10x.h" 


extern float dian;  //��ص�ѹ
extern int Point;      //��ذٷֱ�

void ADC_Init_PA0(void);
int ADC_Read_PA0(void);


#endif

