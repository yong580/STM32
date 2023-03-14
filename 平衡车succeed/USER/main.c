#include "stm32f10x.h"
#include "com_inc.h"

//加速度传感器原始数据
short aacx,aacy,aacz;
//陀螺仪原始数据
short gyrox,gyroy,gyroz;
//俯仰角，横滚角，偏航角
float pitch,roll,yaw;       

extern unsigned long Uart_Hex_PUart_Hex_I,Uart_Hex_D;
extern float result_Float_P,result_Float_I,result_Float_D;

int main(void)
{   

	USART_Config();

	GENERAL_TIM_Init();     //定时器初始化
	Encoder_TIM_init();     //编码器初始化

	delay_init();       	//延时初始化

    USART_Config();         //串口初始化（此处为USART1）
	printf("MPU6050_Init start");
	MPU6050_Init();         //陀螺仪初始化

    mpu_dmp_init();         //陀螺仪DMP初始化

    PID_Init();             //PID初始化

    EXTI_Pxy_Config();      // MPU6050外部中断配置，通过PID调节控制小车保持平衡的函数在外部中断服务函数里面！！

	
	
 while(1)
    {
																	
    }	
	                                                                	
}
                                                                
                                                                






