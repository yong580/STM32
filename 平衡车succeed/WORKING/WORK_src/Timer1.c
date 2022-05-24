#include "Timer1.h"
#include "com_inc.h"
int N=0;
float distance=0;
u16 count=0;
u8 Stop_fal;   //����������С��15cm Stop_fal = 1��С��ֹͣ��
float Dis;      //�������˲���ľ���

void Timer1_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//    NVIC_InitTypeDef NVIC_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
    
    CSB_GPIO_Init();
    
    TIM_TimeBaseStructure.TIM_Period = 0xffff ;       //ARR ����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����
    TIM_TimeBaseStructure.TIM_Prescaler = 72-1;	    //����Ԥ��Ƶ����Ԥ��Ƶ����Ϊ72MHz
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	//����ʱ�ӷ�Ƶϵ��������Ƶ
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ
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
		GPIO_ResetBits(GPIOB, GPIO_Pin_0);//Ԥ������Trig����
		GPIO_SetBits(GPIOB, GPIO_Pin_0);
		delay_us(15);
		GPIO_ResetBits(GPIOB, GPIO_Pin_0);//����10us������
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0){}	//�ȴ��źŷ������źŷ�������������0,�źŷ�������Ϊ�ߵ�ƽ��������1
		
        TIM_Cmd(TIM1, ENABLE);
		TIM1->CNT=0;//us�����ȡ
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 1) //�ȴ��źŽ��ܣ��źŷ��������У�����һֱ��λ1 
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

void CSB_Stop(void)     //r�����������⵽����̫�� ǿ��ֹͣ
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
            if(Car_Speed < 0)       //�������ǰ��
            {
                Car_Speed = 0;      //�򳵵��ٶȵ���0
                Stop_fal = 1;       //ֹͣ��־λ ������������������
            }
            else if(Direction == 0)  //���˵�
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
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PB�˿�ʱ��
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//Trig
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
    GPIO_Init(GPIOB, &GPIO_InitStructure);	

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;//Echo
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		 //��������	 
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

