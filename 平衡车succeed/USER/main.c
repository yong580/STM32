#include "stm32f10x.h"
#include "com_inc.h"

//���ٶȴ�����ԭʼ����
short aacx,aacy,aacz;
//������ԭʼ����
short gyrox,gyroy,gyroz;
//�����ǣ�����ǣ�ƫ����
float pitch,roll,yaw;       

extern unsigned long Uart_Hex_PUart_Hex_I,Uart_Hex_D;
extern float result_Float_P,result_Float_I,result_Float_D;

int main(void)
{   

	USART_Config();

	GENERAL_TIM_Init();     //��ʱ����ʼ��
	Encoder_TIM_init();     //��������ʼ��

	delay_init();       	//��ʱ��ʼ��

    USART_Config();         //���ڳ�ʼ�����˴�ΪUSART1��
	printf("MPU6050_Init start");
	MPU6050_Init();         //�����ǳ�ʼ��

    mpu_dmp_init();         //������DMP��ʼ��

    PID_Init();             //PID��ʼ��

    EXTI_Pxy_Config();      // MPU6050�ⲿ�ж����ã�ͨ��PID���ڿ���С������ƽ��ĺ������ⲿ�жϷ��������棡��

	
	
 while(1)
    {
																	
    }	
	                                                                	
}
                                                                
                                                                






