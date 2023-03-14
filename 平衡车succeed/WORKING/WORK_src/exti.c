#include "exti.h"
#include "pid.h"
#include "motor.h"
#include "Timer1.h"
#include "com_inc.h"
#include "usart.h"

//����������ֵ
extern short aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����
extern short gyrox,gyroy,gyroz;	   //������ԭʼ����
extern float pitch,roll,yaw;       //�����ǣ�����ǣ�ƫ����

//���������ֵ
static int Balance_out,Velocity_out;
extern uint8_t flag;

//�����ֵ
int sum_pwm;
uint16_t siqu=120;

//������������ֵ
extern int Encoder_Left; 
extern int Encoder_Right;



/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------myinit---mpu6050���ж�����------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
//�ж��������ּ�
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* �����ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

//�ж�ģʽ����
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
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	 // ��������
  GPIO_Init(EXTI_GPIO_PORT, &GPIO_InitStructure);

	/* EXTI line mode config */
  GPIO_EXTILineConfig(EXTI_SOURCE_PORT, EXTI_SOURCE_PIN); 
  EXTI_InitStructure.EXTI_Line = EXTI_LINE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�������ж�
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
/*--------------------------------------------------------------�жϴ�����---------------------------------------------------------------------*/

//int count = 0;
void EXTI15_10_IRQHandler (void)
{       
	    
    //ȷ���Ƿ������EXTI Line�ж�
	if(EXTI_GetITStatus(EXTI_LINE) != RESET) 	
	{   
		//��ȡ��������ֵ
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz); //�õ�����������						 
        mpu_dmp_get_data(&pitch,&roll,&yaw);	 //�Ƕ�	
//		if(count > 10)
//		{
//			printf("roll = %f\r\n",roll);
//			printf("pitch = %f\r\n",pitch);
//			printf("yaw = %f\r\n",yaw);
//			count = 0;
//		}
//		count++;
        MPU_Get_Accelerometer(&aacx,&aacy,&aacz);//���ٶ�	                     
		//��ȡ��������ֵ
		Encoder_Left = My_Read_Encoder(1);                               
        Encoder_Right= My_Read_Encoder(2);
		//ǰ����б�Ƕȣ����ڼ���
		pid.Pv = roll;
		//printf("gyrox= %d\n",gyrox);����


		//��������pid���ֵ
		Balance_out = Balance(pid.Pv);//ֱ����
        Velocity_out = Velocity(Encoder_Left,Encoder_Right);//�ٶȻ�		
		
		//������͵���
		sum_pwm=(int)Balance_out+Velocity_out;
		
				
		if(sum_pwm>0)          
		{   //���������������
			sum_pwm=sum_pwm+siqu;
			TIM_SetCompare1(TIM3,sum_pwm);
			TIM_SetCompare3(TIM3,sum_pwm);
			TIM_SetCompare2(TIM3,0);
			TIM_SetCompare4(TIM3,0);
		}
		else
		{	//���������������
			sum_pwm=Myabs(sum_pwm)+siqu;
			TIM_SetCompare1(TIM3,0);
			TIM_SetCompare3(TIM3,0);
			TIM_SetCompare2(TIM3,sum_pwm);
			TIM_SetCompare4(TIM3,sum_pwm);
		}
		
		
		//����жϱ�־λ	
		EXTI_ClearITPendingBit(EXTI_LINE);     
	
	}
	                                                                                        																						  
}	



