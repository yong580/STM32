#ifndef __MOTOR_H
#define __MOTOR_H 

#include "stm32f10x.h"
#include <sys.h>	

void Time2_PWM_CH34(u16 CCR3_Val, u16 CCR4_Val); //��ʱ��2pwm,���Ƶ��ת��
void MOTOR_GPIO_Init(void);     //�������IO�ڳ�ʼ��
int myabs(int a);               //ȡ����ֵ
void Set_Pwm(int moto1,int moto2);
void Xianfu_Pwm(void);  //PWM�޷�����
void Motorc_Abnormal_Spot(void); //С���쳣�����ֹͣ���
    

#define AIN2   PBout(15)
#define AIN1   PBout(14)

#define BIN1   PBout(12)
#define BIN2   PBout(13)

extern u16 PWM_Left;  //����PWMֵ
extern u16 PWM_Right;  //�ҵ��PWMֵ
extern u8 Turn_fal;

#endif
