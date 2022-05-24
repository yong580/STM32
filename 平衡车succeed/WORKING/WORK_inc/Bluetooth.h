#ifndef __Bluetooth_H
#define	__Bluetooth_H

#include "stm32f10x.h" 
#include "motor.h"

void Bluetooth_Command(void);
void Bluetooth_Direction(void);      //�ж�ǰ������˵ķ�����
void Bluetooth_Spend(void);  //����С���ٶȴ�С
void Bluetooth_Turn(void);  //����ת��
void Bluetooth_Rotating(void);  //������
int Xianshu(int speed);        //����
void Clear_RX3_Buff(void);      //���RX3���飬�����busy��־λ
void Send_Bluetooth_Data(void); //�����ݷ��͵��ֻ� 


extern char Turn;              //�ж�����ת
extern u8 Rotating_fal;         //����������
extern u8 Direction;       //�жϷ���ǰ�������


#endif
