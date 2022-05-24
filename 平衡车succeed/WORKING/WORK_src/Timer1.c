#include "Timer1.h"
#include "com_inc.h"
int N=0;
float distance=0;
u16 count=0;
u8 Stop_fal;   //超声波距离小于15cm Stop_fal = 1，小车停止，
float Dis;      //超声波滤波后的距离

void Timer1_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//    NVIC_InitTypeDef NVIC_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
    
    CSB_GPIO_Init();
    
    TIM_TimeBaseStructure.TIM_Period = 0xffff ;       //ARR 当定时器从0计数到999，即为1000次，为一个定时周期
    TIM_TimeBaseStructure.TIM_Prescaler = 72-1;	    //设置预分频：不预分频，即为72MHz
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	//设置时钟分频系数：不分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;   
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    
    
       
//    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
    
    TIM_Cmd(TIM1, ENABLE);  
}
//TIM_GetCapture1(TIM3);

//u16 T;
//void TIM1_UP_IRQHandler(void)
//{
//    if(TIM_GetITStatus(TIM1,TIM_IT_Update)!=RESET)
//    {
//        N ++;
//    }   
//    TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
//}

float Distance(void)
{
		GPIO_ResetBits(GPIOB, GPIO_Pin_0);//预先拉低Trig引脚
		GPIO_SetBits(GPIOB, GPIO_Pin_0);
		delay_us(15);
		GPIO_ResetBits(GPIOB, GPIO_Pin_0);//发出10us的脉冲
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0){}	//等待信号发出，信号发出，计数器置0,信号发出，变为高电平，引脚置1
		
        TIM_Cmd(TIM1, ENABLE);
		TIM1->CNT=0;//us级别读取
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 1) //等待信号接受，信号发出过程中，引脚一直置位1 
        {
            ;
        }
		count=(TIM1->CNT);
        
        TIM1->CNT = 0;
        TIM_Cmd(TIM1, DISABLE); 
		
		distance=(float)count*0.017;
//        distance = distance*100;
        printf("count = %d\r\n ",count);
		return distance;
}

void CSB_Stop(void)     //r如果超声波检测到距离太近 强制停止
{   
//    static u8 i;
    static float temp;
    static float Lowout_last;
    float a = 0.7; 
    temp = Distance();
    Dis = (1-a) * temp + a * Lowout_last;   //low_out=(1-a)*Ek+a*low_out_last;
    Lowout_last = Dis;
    printf("temp = %f   ",temp);
    printf("Dis = %f \r\n",Dis);
        if(Dis < 25)
        {
            if(Car_Speed < 0)       //如果继续前进
            {
                Car_Speed = 0;      //则车的速度等于0
                Stop_fal = 1;       //停止标志位 ，用于限制蓝牙控制
            }
            else if(Direction == 0)  //后退档
            {
                Stop_fal = 0;
            }
            
        }
        else if(Dis > 35)
        {
            Stop_fal = 0;
        }
}


void CSB_GPIO_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB端口时钟
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//Trig
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
    GPIO_Init(GPIOB, &GPIO_InitStructure);	

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;//Echo
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		 //下拉输入	 
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

