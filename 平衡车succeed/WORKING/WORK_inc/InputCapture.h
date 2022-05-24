#ifndef __InputCapture_H
#define __InputCapture_H 

#include "stm32f10x.h"

void TIM3_Encoder_Init(void);        //��ʱ��3 PA67��������
void TIM4_Encoder_Init(void);       //��ʱ��4 PB67��������
int Read_Encoder(u8 TIMX);

/*-----------------------------------------------------------------------------*/
#define ENCODER_TIM   TIM1
#define ENCODER_TIM_APB2Clock_FUN     RCC_APB2PeriphClockCmd
#define ENCODER_TIM_CLK               RCC_APB2Periph_TIM1
//���벶��ͨ��1
#define            	ENCODER_TIM_CH1_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            	ENCODER_TIM_CH1_PORT          GPIOA
#define            	ENCODER_TIM_CH1_PIN           GPIO_Pin_8


//���벶��ͨ��2
#define           	ENCODER_TIM_CH2_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            	ENCODER_TIM_CH2_PORT          GPIOA
#define            	ENCODER_TIM_CH2_PIN           GPIO_Pin_9
/*-----------------------------------------------------------------------------*/
#define ENCODER_TIM2   TIM2
#define ENCODER_TIM2_APB1Clock_FUN     RCC_APB1PeriphClockCmd
#define ENCODER_TIM2_CLK               RCC_APB1Periph_TIM2
//���벶��ͨ��1
#define            	ENCODER_TIM2_CH1_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            	ENCODER_TIM2_CH1_PORT          GPIOA
#define            	ENCODER_TIM2_CH1_PIN           GPIO_Pin_15

//���벶��ͨ��2
#define           	ENCODER_TIM2_CH2_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            	ENCODER_TIM2_CH2_PORT          GPIOB
#define            	ENCODER_TIM2_CH2_PIN           GPIO_Pin_3
void Encoder_TIM_init(void);
int32_t My_Read_Encoder(u8 TIMX);

#endif
