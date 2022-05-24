#ifndef _motor_control_H_
#define _motor_control_H_

#include "stdint.h"
typedef struct PID{
    double setpoint;            //设定值
		double error;
		double lasterror;
    double kp;                  //比例系数
    double ki;                  //积分系数
    double kd;                  //微分系数
		double sumerror; 						//误差积分
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
