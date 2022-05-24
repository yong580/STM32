#ifndef __PID_H
#define __PID_H 

#include "stm32f10x.h"

extern short gyrox,gyroy,gyroz;	//������ԭʼ����
extern float pitch,roll,yaw;       //�����ǣ�����ǣ�ƫ����
extern short aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����
extern int Moto1,Moto2;
extern int Car_Speed;   //Ŀ���ٶ�
extern int Car_Turn;       //Ŀ��ת��Ƕ�
extern int Encoder_Err;     //����ת��
extern int Encoder_Left;
extern int Encoder_Right;


typedef struct 
{
	float  Sv;			//�û��趨ƽ��λ��ֵ
	float Pv;
					/*ƽ�⻷�����趨 */
	float  Kd;					//ƽ��΢����ϵ��
	float  Kp;			//ƽ�������ϵ��
	

	
					/*�ٶȻ������趨 */
	signed int left;      //Ŀ�����ٶ�
	signed int right;     //Ŀ�����ٶ�
	float  Ki_speed;				//�ٶȻ�������ϵ��
	float  Kp_speed;		//�ٶȻ�������ϵ��
	signed int EK_speed;  	//�ٶ�ƫ��
	signed int SEK_speed;   //��ʷƫ��֮��

					/*ת�򻷲����趨 */
	float Kp_turn;					//ת�򻷱�����ϵ��
	
}PID;

void PID_Init(void);
int Balance(float Angle);  //*С��ƽ�⻷���֣�΢��+��������
int Velocity(int encoder_left,int encoder_right);  /*С���ٶȻ����֣� ����+��������*/
int turn(float gyro);   //ת�����
void PID_Calculation(void);     //��PID����
int Myabs(double pwm);

extern PID pid;

#endif

