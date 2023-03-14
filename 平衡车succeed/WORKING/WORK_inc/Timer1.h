#ifndef __TIMER1_H
#define __TIMER1_H 

#include "stm32f10x.h"

void Timer1_Init(void);

void CSB_GPIO_Init(void);       //超声波GPIO初始化
float Distance(void);             //超声波距离
void CSB_Stop(void);     //r如果超声波检测到距离太近 强制停止

extern u8 Stop_fal;   //超声波距离小于15cm Stop_fal = 1，小车停止，
extern float Dis;      //超声波滤波后的距离


#endif
