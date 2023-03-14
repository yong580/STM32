#include "pid.h"
#include "motor.h"
#include "mpu6050.h"
#include "InputCapture.H"

//左旋，右旋标记位，0：加入转向环，不转  1：正转，2：反转
extern u8 Rotating_fal;

//结构体声明
PID pid;

int Encoder_Left;   //左电机速度
int Encoder_Right;  //右电机速度
 
int Encoder_Err;    //车轮转速
int Car_Speed = 0;  //目标速度
int Car_Turn = 0;   //目标转向角度


void PID_Init(void)
{
                
    /*平衡PID环控制参数初始化*/
	//机械中值
    pid.Sv =2.1;		
    pid.Kp =75;       
    pid.Kd =0.15; //0.12       
                                         
    /*速度PID环控制参数初始化*/		
    pid.Kp_speed = -8;  //0.285
    pid.Ki_speed = -0.04;//0.00202

    /*转向PID环控制参数初始化*/
    pid.Kp_turn =0; //-10
}

/*小车平衡环部分，微分+比例控制、微分变量为直接读取的加速度*/    
//直立环kp+kd
int Balance(float Angle)
{  

    signed  int  Bias;
    int balance;
    Bias=(Angle-pid.Sv);
    balance = pid.Kp * Bias + (gyrox+70) * pid.Kd;  
    return balance;
}

//速度环
int Velocity(int encoder_left,int encoder_right)
{
	static int PWM_out,Encoder_S,EnC_Err_Lowout,EnC_Err_Lowout_last;//【2】
	float a=0.7;//【3】
	
	//1.计算速度偏差
	Encoder_Err = (encoder_left + encoder_right) - Car_Speed;//舍去误差
	//2.对速度偏差进行低通滤波
	//low_out=(1-a)*Ek+a*low_out_last;
	EnC_Err_Lowout = (1-a) * Encoder_Err + a * EnC_Err_Lowout_last;//使得波形更加平滑，滤除高频干扰，防止速度突变。
	EnC_Err_Lowout_last = EnC_Err_Lowout;//防止速度过大的影响直立环的正常工作。
	//3.对速度偏差积分，积分出位移
	Encoder_S += EnC_Err_Lowout;    //【4】
	//4.积分限幅
	Encoder_S = (Encoder_S > 1400)? 1400:(Encoder_S<(-1400)?(-1400):Encoder_S);
	//5.速度环控制输出计算
	PWM_out = pid.Kp_speed * EnC_Err_Lowout + pid.Ki_speed * Encoder_S;//【5】
	return PWM_out;
}


/*小车转向环部分，比例控制*/
//转向环
int turn(float gyro)
{
    int Turn;
	  float Bias;
			
			if(Car_Turn <= -180&&gyro>0)
				Bias = (gyro - (Car_Turn + 360)) * 10;	
			else if(Car_Turn <= -360&&gyro<0)
				Bias = (gyro - (Car_Turn + 360)) * 10;	
			else if(Car_Turn >= 180 && gyro<0)
				 Bias = (gyro - (Car_Turn - 360))*10;
			else if(Car_Turn >= 360&&gyro>0)
					Bias = (gyro - (Car_Turn - 360))*10;
		  else
				 Bias = (gyro-Car_Turn)*10;	

	  Turn = (int)(Bias*pid.Kp_turn); 
	
				/*进行转向速度的单独限幅*/
		if(Turn>=1500)	Turn =1500;
		else if(Turn<=-1500)Turn =-1500;
			
		return Turn;
}
//绝对值函数
int Myabs(double pwm)
{
	if (pwm<0)
	return ((int)(-pwm));
	else
	return ((int)pwm);
}




