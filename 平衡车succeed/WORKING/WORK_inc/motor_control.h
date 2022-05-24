#ifndef _motor_control_H_
#define _motor_control_H_

#include "stdint.h"
typedef struct PID{
    double setpoint;            //�趨ֵ
		double error;
		double lasterror;
    double kp;                  //����ϵ��
    double ki;                  //����ϵ��
    double kd;                  //΢��ϵ��
		double sumerror; 						//������
		double output;
}PID;



extern float  Pitch,Roll,Yaw;
extern float  GYRO[3];
double balance_pid(float Pitch,float gyro);
void xianfu_pwm(void);
void dir_Gpioconfig(void);
int my_abs(double pwm);
double v_pid(int Encoder_Left,int Encoder_Right);

#endif
