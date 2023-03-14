#ifndef __DISPLAY_H
#define __DISPLAY_H

#include "stm32f10x.h"

void Display_CSB(void);   //超声波距离显示
void Display_U(void);     //电量电压显示

void Display_All(void);
    
void Calculate_U(void); //电压计算

void Key_Init(void); //按键初始化
void KEY_Scan(void);


#define KEY GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)

#endif
