#ifndef __ADC_H
#define	__ADC_H

#include "stm32f10x.h" 


extern float dian;  //电池电压
extern int Point;      //电池百分比

void ADC_Init_PA0(void);
int ADC_Read_PA0(void);


#endif

