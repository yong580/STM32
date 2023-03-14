#include "exti.h"
#include "pid.h"
#include "motor.h"
#include "Timer1.h"
#include "com_inc.h"
#include "usart.h"

//传感器读出值
extern short aacx,aacy,aacz;		//加速度传感器原始数据
extern short gyrox,gyroy,gyroz;	   //陀螺仪原始数据
extern float pitch,roll,yaw;       //俯仰角，横滚角，偏航角

//输出量计算值
static int Balance_out,Velocity_out;
extern uint8_t flag;

//输出和值
int sum_pwm;
uint16_t siqu=120;

//两个编码器数值
extern int Encoder_Left; 
extern int Encoder_Right;



/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------myinit---mpu6050的中断配置------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
//中断优先组别分级
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* 配置中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

//中断模式配置
void EXTI_Pxy_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	/* config the extiline clock and AFIO clock */
	RCC_APB2PeriphClockCmd(EXTI_GPIO_CLK,ENABLE);
												
	/* config the NVIC */
	NVIC_Configuration();

	/* EXTI line gpio config*/	
  GPIO_InitStructure.GPIO_Pin = EXTI_GPIO_PIN;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	 // 上拉输入
  GPIO_Init(EXTI_GPIO_PORT, &GPIO_InitStructure);

	/* EXTI line mode config */
  GPIO_EXTILineConfig(EXTI_SOURCE_PORT, EXTI_SOURCE_PIN); 
  EXTI_InitStructure.EXTI_Line = EXTI_LINE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿中断
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
}

void EnableInvInterrupt(void)
{
  EXTI_InitTypeDef   EXTI_InitStructure;
  /* Configure EXTI Line1 */
  EXTI_InitStructure.EXTI_Line = EXTI_LINE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
}
void DisableInvInterrupt(void)
{
  EXTI_InitTypeDef   EXTI_InitStructure;
    /* Configure EXTI Line1 */
  EXTI_InitStructure.EXTI_Line = EXTI_LINE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = DISABLE;
  EXTI_Init(&EXTI_InitStructure);
  EXTI_ClearITPendingBit(EXTI_LINE);
}
/*--------------------------------------------------------------中断处理函数---------------------------------------------------------------------*/

//int count = 0;
void EXTI15_10_IRQHandler (void)
{       
	    
    //确保是否产生了EXTI Line中断
	if(EXTI_GetITStatus(EXTI_LINE) != RESET) 	
	{   
		//读取传感器数值
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz); //得到陀螺仪数据						 
        mpu_dmp_get_data(&pitch,&roll,&yaw);	 //角度	
//		if(count > 10)
//		{
//			printf("roll = %f\r\n",roll);
//			printf("pitch = %f\r\n",pitch);
//			printf("yaw = %f\r\n",yaw);
//			count = 0;
//		}
//		count++;
        MPU_Get_Accelerometer(&aacx,&aacy,&aacz);//加速度	                     
		//读取编码器的值
		Encoder_Left = My_Read_Encoder(1);                               
        Encoder_Right= My_Read_Encoder(2);
		//前后倾斜角度，用于计算
		pid.Pv = roll;
		//printf("gyrox= %d\n",gyrox);测试


		//计算两个pid输出值
		Balance_out = Balance(pid.Pv);//直立环
        Velocity_out = Velocity(Encoder_Left,Encoder_Right);//速度环		
		
		//两环求和叠加
		sum_pwm=(int)Balance_out+Velocity_out;
		
				
		if(sum_pwm>0)          
		{   //补上死区给到电机
			sum_pwm=sum_pwm+siqu;
			TIM_SetCompare1(TIM3,sum_pwm);
			TIM_SetCompare3(TIM3,sum_pwm);
			TIM_SetCompare2(TIM3,0);
			TIM_SetCompare4(TIM3,0);
		}
		else
		{	//补上死区给到电机
			sum_pwm=Myabs(sum_pwm)+siqu;
			TIM_SetCompare1(TIM3,0);
			TIM_SetCompare3(TIM3,0);
			TIM_SetCompare2(TIM3,sum_pwm);
			TIM_SetCompare4(TIM3,sum_pwm);
		}
		
		
		//清除中断标志位	
		EXTI_ClearITPendingBit(EXTI_LINE);     
	
	}
	                                                                                        																						  
}	



