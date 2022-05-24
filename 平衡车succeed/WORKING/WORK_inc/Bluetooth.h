#ifndef __Bluetooth_H
#define	__Bluetooth_H

#include "stm32f10x.h" 
#include "motor.h"

void Bluetooth_Command(void);
void Bluetooth_Direction(void);      //判断前进或后退的方向函数
void Bluetooth_Spend(void);  //控制小车速度大小
void Bluetooth_Turn(void);  //控制转向
void Bluetooth_Rotating(void);  //左右旋
int Xianshu(int speed);        //限速
void Clear_RX3_Buff(void);      //清除RX3数组，并解除busy标志位
void Send_Bluetooth_Data(void); //把数据发送到手机 


extern char Turn;              //判断左右转
extern u8 Rotating_fal;         //左旋，右旋
extern u8 Direction;       //判断方向，前进或后退


#endif
