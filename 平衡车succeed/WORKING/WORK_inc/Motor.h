#ifndef __MOTOR_H
#define __MOTOR_H 

#include "stm32f10x.h"
#include <sys.h>	

void Time2_PWM_CH34(u16 CCR3_Val, u16 CCR4_Val); //定时器2pwm,控制电机转速
void MOTOR_GPIO_Init(void);     //电机控制IO口初始化
int myabs(int a);               //取绝对值
void Set_Pwm(int moto1,int moto2);
void Xianfu_Pwm(void);  //PWM限幅函数
void Motorc_Abnormal_Spot(void); //小车异常，电机停止输出
    

#define AIN2   PBout(15)
#define AIN1   PBout(14)

#define BIN1   PBout(12)
#define BIN2   PBout(13)

extern u16 PWM_Left;  //左电机PWM值
extern u16 PWM_Right;  //右电机PWM值
extern u8 Turn_fal;

#endif
