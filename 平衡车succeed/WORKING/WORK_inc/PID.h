#ifndef __PID_H
#define __PID_H 

#include "stm32f10x.h"

extern short gyrox,gyroy,gyroz;	//陀螺仪原始数据
extern float pitch,roll,yaw;       //俯仰角，横滚角，偏航角
extern short aacx,aacy,aacz;		//加速度传感器原始数据
extern int Moto1,Moto2;
extern int Car_Speed;   //目标速度
extern int Car_Turn;       //目标转向角度
extern int Encoder_Err;     //车轮转速
extern int Encoder_Left;
extern int Encoder_Right;


typedef struct 
{
	float  Sv;			//用户设定平衡位置值
	float Pv;
					/*平衡环参数设定 */
	float  Kd;					//平衡微分项系数
	float  Kp;			//平衡比例项系数
	

	
					/*速度环参数设定 */
	signed int left;      //目标左速度
	signed int right;     //目标右速度
	float  Ki_speed;				//速度环积分项系数
	float  Kp_speed;		//速度环比例项系数
	signed int EK_speed;  	//速度偏差
	signed int SEK_speed;   //历史偏差之和

					/*转向环参数设定 */
	float Kp_turn;					//转向环比例项系数
	
}PID;

void PID_Init(void);
int Balance(float Angle);  //*小车平衡环部分，微分+比例控制
int Velocity(int encoder_left,int encoder_right);  /*小车速度环部分， 积分+比例控制*/
int turn(float gyro);   //转向控制
void PID_Calculation(void);     //总PID计算
int Myabs(double pwm);

extern PID pid;

#endif

