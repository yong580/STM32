#include "motor.h"
#include "PID.h"
#include "Bluetooth.h"

u16 PWM_Left;  //左电机PWM值
u16 PWM_Right;  //右电机PWM值

/**************************************************************************
函数功能：最后设置PWM函数，并检测是否关闭电机
入口参数：无
返回  值：无
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
        PWM_Left=myabs(moto1);   //取绝对值
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
    PWM_Right=myabs(moto2);	//取绝对值
    Xianfu_Pwm();   //电机PWM限幅
    Motorc_Abnormal_Spot(); //如果小车倒下或异常，电机停止输出
    TIM_SetCompare3(TIM2, PWM_Left);    //右
    TIM_SetCompare4(TIM2, PWM_Right);   //左
}

/**************************************************************************
函数功能：获得整数绝对值函数
入口参数：整数
返回  值：绝对值
**************************************************************************/	 
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}

/**************************************************************************
函数功能：PWM限幅函数
入口参数：无
返回  值：无
**************************************************************************/	 
void Xianfu_Pwm(void)  //PWM限幅函数
{	
	int Amplitude=7198;    //===PWM满幅是7200 限制在6900
    if(PWM_Left<-Amplitude) PWM_Left=-Amplitude;	
    if(PWM_Left>Amplitude)  PWM_Left=Amplitude;	
    if(PWM_Right<-Amplitude) PWM_Right=-Amplitude;	
    if(PWM_Right>Amplitude)  PWM_Right=Amplitude;		
	
}

void Motorc_Abnormal_Spot(void)     //小车异常，电机停止输出
{
    if(pitch >= 10 || pitch <= -10 || (roll - pid.Sv) > 50 || (roll - pid.Sv) < -50) 
    {
        PWM_Left = 0;
        PWM_Right = 0;
    }
}

void MOTOR_GPIO_Init(void)      //电机控制IO口初始化
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
													   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 | GPIO_Pin_5;	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  	GPIO_Init(GPIOB, &GPIO_InitStructure);		  

	/* 低电平	*/
	GPIO_SetBits(GPIOB, GPIO_Pin_13 | GPIO_Pin_14|GPIO_Pin_12 | GPIO_Pin_15);   //电机驱动制动模式
    GPIO_SetBits(GPIOB, GPIO_Pin_5);                                            //使能STBY
	GPIO_SetBits(GPIOB, GPIO_Pin_13 | GPIO_Pin_14);
	GPIO_ResetBits(GPIOB, GPIO_Pin_12 | GPIO_Pin_15);	 
}

void Time2_PWM_CH34(u16 CCR4_Val, u16 CCR3_Val)     //定时器2pwm,控制电机转速
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
    
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // 复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    TIM_TimeBaseStructure.TIM_Period = 7199 ;       //ARR 当定时器从0计数到7199，即为7200次，为一个定时周期
    TIM_TimeBaseStructure.TIM_Prescaler = 0;	    //设置预分频：不预分频，即为72MHz
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	//设置时钟分频系数：不分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //配置为PWM模式1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//比较输出使能
    TIM_OCInitStructure.TIM_Pulse = CCR3_Val;	   //设置跳变值，当计数器计数到这个值时，电平发生跳变
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //当定时器计数值小于CCR1_Val时为高电平
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);	 //使能通道3
    
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
    
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = CCR4_Val;	  //设置通道4的电平跳变值，输出另外一个占空比的PWM
    TIM_OC4Init(TIM2, &TIM_OCInitStructure);	 //使能通道4
    
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


