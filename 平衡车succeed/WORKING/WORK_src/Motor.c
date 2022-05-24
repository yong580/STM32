#include "motor.h"
#include "PID.h"
#include "Bluetooth.h"

u16 PWM_Left;  //����PWMֵ
u16 PWM_Right;  //�ҵ��PWMֵ

/**************************************************************************
�������ܣ��������PWM������������Ƿ�رյ��
��ڲ�������
����  ֵ����
**************************************************************************/	 
//extern u8 stop_flag;
void Set_Pwm(int moto1,int moto2)
{
    if(moto1>0)
    {
        AIN2=1;
        AIN1=0;   
    }			
    else 
    {
        AIN2=0;
        AIN1=1;
    }                			
        PWM_Left=myabs(moto1);   //ȡ����ֵ
    if(moto2>0)
    {
        BIN1=1;
        BIN2=0;
    }            			
    else
    {
        BIN1=0;
        BIN2=1;
    }            			
    PWM_Right=myabs(moto2);	//ȡ����ֵ
    Xianfu_Pwm();   //���PWM�޷�
    Motorc_Abnormal_Spot(); //���С�����»��쳣�����ֹͣ���
    TIM_SetCompare3(TIM2, PWM_Left);    //��
    TIM_SetCompare4(TIM2, PWM_Right);   //��
}

/**************************************************************************
�������ܣ������������ֵ����
��ڲ���������
����  ֵ������ֵ
**************************************************************************/	 
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}

/**************************************************************************
�������ܣ�PWM�޷�����
��ڲ�������
����  ֵ����
**************************************************************************/	 
void Xianfu_Pwm(void)  //PWM�޷�����
{	
	int Amplitude=7198;    //===PWM������7200 ������6900
    if(PWM_Left<-Amplitude) PWM_Left=-Amplitude;	
    if(PWM_Left>Amplitude)  PWM_Left=Amplitude;	
    if(PWM_Right<-Amplitude) PWM_Right=-Amplitude;	
    if(PWM_Right>Amplitude)  PWM_Right=Amplitude;		
	
}

void Motorc_Abnormal_Spot(void)     //С���쳣�����ֹͣ���
{
    if(pitch >= 10 || pitch <= -10 || (roll - pid.Sv) > 50 || (roll - pid.Sv) < -50) 
    {
        PWM_Left = 0;
        PWM_Right = 0;
    }
}

void MOTOR_GPIO_Init(void)      //�������IO�ڳ�ʼ��
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
													   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 | GPIO_Pin_5;	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  	GPIO_Init(GPIOB, &GPIO_InitStructure);		  

	/* �͵�ƽ	*/
	GPIO_SetBits(GPIOB, GPIO_Pin_13 | GPIO_Pin_14|GPIO_Pin_12 | GPIO_Pin_15);   //��������ƶ�ģʽ
    GPIO_SetBits(GPIOB, GPIO_Pin_5);                                            //ʹ��STBY
	GPIO_SetBits(GPIOB, GPIO_Pin_13 | GPIO_Pin_14);
	GPIO_ResetBits(GPIOB, GPIO_Pin_12 | GPIO_Pin_15);	 
}

void Time2_PWM_CH34(u16 CCR4_Val, u16 CCR3_Val)     //��ʱ��2pwm,���Ƶ��ת��
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
    
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // �����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    TIM_TimeBaseStructure.TIM_Period = 7199 ;       //ARR ����ʱ����0������7199����Ϊ7200�Σ�Ϊһ����ʱ����
    TIM_TimeBaseStructure.TIM_Prescaler = 0;	    //����Ԥ��Ƶ����Ԥ��Ƶ����Ϊ72MHz
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	//����ʱ�ӷ�Ƶϵ��������Ƶ
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //����ΪPWMģʽ1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_Pulse = CCR3_Val;	   //��������ֵ�������������������ֵʱ����ƽ��������
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);	 //ʹ��ͨ��3
    
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
    
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = CCR4_Val;	  //����ͨ��4�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM
    TIM_OC4Init(TIM2, &TIM_OCInitStructure);	 //ʹ��ͨ��4
    
    TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
    
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); 
    TIM_Cmd(TIM2, ENABLE);  
}

u16 TIM2_T;
void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)
    {
        TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
        TIM2_T ++;
        if(TIM2_T == 444)
        {
            TIM2_T = 0;
            
            if(Turn == 'L')
            {
                Car_Turn += 2;
                if(Car_Turn >= 183 && yaw >= -179 && yaw < 0) Car_Turn = -179;
            }
            else if(Turn == 'R')
            {
                Car_Turn -= 2;
                if(Car_Turn <= -183 && yaw <= 179 && yaw > 0) Car_Turn = 179;
            }
        }
    }
}


