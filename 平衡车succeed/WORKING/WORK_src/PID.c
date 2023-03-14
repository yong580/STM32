#include "pid.h"
#include "motor.h"
#include "mpu6050.h"
#include "InputCapture.H"

//�������������λ��0������ת�򻷣���ת  1����ת��2����ת
extern u8 Rotating_fal;

//�ṹ������
PID pid;

int Encoder_Left;   //�����ٶ�
int Encoder_Right;  //�ҵ���ٶ�
 
int Encoder_Err;    //����ת��
int Car_Speed = 0;  //Ŀ���ٶ�
int Car_Turn = 0;   //Ŀ��ת��Ƕ�


void PID_Init(void)
{
                
    /*ƽ��PID�����Ʋ�����ʼ��*/
	//��е��ֵ
    pid.Sv =2.1;		
    pid.Kp =75;       
    pid.Kd =0.15; //0.12       
                                         
    /*�ٶ�PID�����Ʋ�����ʼ��*/		
    pid.Kp_speed = -8;  //0.285
    pid.Ki_speed = -0.04;//0.00202

    /*ת��PID�����Ʋ�����ʼ��*/
    pid.Kp_turn =0; //-10
}

/*С��ƽ�⻷���֣�΢��+�������ơ�΢�ֱ���Ϊֱ�Ӷ�ȡ�ļ��ٶ�*/    
//ֱ����kp+kd
int Balance(float Angle)
{  

    signed  int  Bias;
    int balance;
    Bias=(Angle-pid.Sv);
    balance = pid.Kp * Bias + (gyrox+70) * pid.Kd;  
    return balance;
}

//�ٶȻ�
int Velocity(int encoder_left,int encoder_right)
{
	static int PWM_out,Encoder_S,EnC_Err_Lowout,EnC_Err_Lowout_last;//��2��
	float a=0.7;//��3��
	
	//1.�����ٶ�ƫ��
	Encoder_Err = (encoder_left + encoder_right) - Car_Speed;//��ȥ���
	//2.���ٶ�ƫ����е�ͨ�˲�
	//low_out=(1-a)*Ek+a*low_out_last;
	EnC_Err_Lowout = (1-a) * Encoder_Err + a * EnC_Err_Lowout_last;//ʹ�ò��θ���ƽ�����˳���Ƶ���ţ���ֹ�ٶ�ͻ�䡣
	EnC_Err_Lowout_last = EnC_Err_Lowout;//��ֹ�ٶȹ����Ӱ��ֱ����������������
	//3.���ٶ�ƫ����֣����ֳ�λ��
	Encoder_S += EnC_Err_Lowout;    //��4��
	//4.�����޷�
	Encoder_S = (Encoder_S > 1400)? 1400:(Encoder_S<(-1400)?(-1400):Encoder_S);
	//5.�ٶȻ������������
	PWM_out = pid.Kp_speed * EnC_Err_Lowout + pid.Ki_speed * Encoder_S;//��5��
	return PWM_out;
}


/*С��ת�򻷲��֣���������*/
//ת��
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
	
				/*����ת���ٶȵĵ����޷�*/
		if(Turn>=1500)	Turn =1500;
		else if(Turn<=-1500)Turn =-1500;
			
		return Turn;
}
//����ֵ����
int Myabs(double pwm)
{
	if (pwm<0)
	return ((int)(-pwm));
	else
	return ((int)pwm);
}




