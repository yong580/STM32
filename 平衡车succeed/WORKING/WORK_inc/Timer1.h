#ifndef __TIMER1_H
#define __TIMER1_H 

#include "stm32f10x.h"

void Timer1_Init(void);

void CSB_GPIO_Init(void);       //������GPIO��ʼ��
float Distance(void);             //����������
void CSB_Stop(void);     //r�����������⵽����̫�� ǿ��ֹͣ

extern u8 Stop_fal;   //����������С��15cm Stop_fal = 1��С��ֹͣ��
extern float Dis;      //�������˲���ľ���


#endif
