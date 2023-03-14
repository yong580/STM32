#ifndef __DISPLAY_H
#define __DISPLAY_H

#include "stm32f10x.h"

void Display_CSB(void);   //������������ʾ
void Display_U(void);     //������ѹ��ʾ

void Display_All(void);
    
void Calculate_U(void); //��ѹ����

void Key_Init(void); //������ʼ��
void KEY_Scan(void);


#define KEY GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)

#endif
