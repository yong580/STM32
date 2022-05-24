#include "motor_control.h"
#include "stm32f10x_rcc.h"

PID pid1={-0.5,0,0,70,0,80,0};
PID pid2={0,0,0,0,0,0,0};
uint8_t flag;//
extern int sum_pwm;
double Encoder=0;
double balance_pid(float now_Pitch,float gyro)
{		
	pid1.error=now_Pitch-pid1.setpoint;
//	if(pid1.error>600||pid1.error<-600)//当误差大于一定阈值后才累计误差，防止震荡
//	{
//		pid1.sumerror+=pid1.error;
//	}
	
	pid1.output=pid1.kp*pid1.error+pid1.kd*gyro;//0.05为采样时间间隔
//	pid1.lasterror=pid1.error;
	//限幅
//	if(pid1.output>1000)
//		pid1.output=1000;
//	if(pid1.output<-1000)
//		pid1.output=-1000;
		return pid1.output;
		
}

double v_pid(int Encoder_Left,int Encoder_Right)
{		
	 double Velocity;
	
	pid2.error=(Encoder_Left+Encoder_Right)-pid2.setpoint;
	Encoder=Encoder*0.7;
	Encoder=Encoder+pid2.error*0.3;
	pid2.sumerror+=Encoder;
	if(pid2.sumerror>10000)
	{
		pid2.sumerror=10000;
	}
	if(pid2.sumerror<-10000)
	{
		pid2.sumerror=-10000;
	}
	pid2.output=pid2.kp*pid1.error+pid2.ki*pid2.sumerror;//0.05为采样时间间隔
//	pid1.lasterror=pid1.error;
	//限幅
	Velocity=pid2.output;
		return Velocity;
}




void xianfu_pwm(void)
{	
	uint16_t xianfu=999;
	if(sum_pwm>xianfu)
		sum_pwm=xianfu;
	if(sum_pwm<-xianfu)
		sum_pwm=-xianfu;
		
}
int my_abs(double pwm)
{
	if (pwm<0)
	return ((int)(-pwm));
	else
	return ((int)pwm);
}
	

void dir_Gpioconfig(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_4;
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	GPIO_ResetBits(GPIOC,GPIO_Pin_5);
	GPIO_ResetBits(GPIOC,GPIO_Pin_4);
}

