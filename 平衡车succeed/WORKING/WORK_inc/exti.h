#ifndef  _EXTI_H
#define  _EXTI_H


#include "sys.h" 

void MPU6050_EXTI_Init(void);

/*------------------------------my define-----------------------------------------*/


#define NumOfTask 2
extern unsigned int Task_Delay[NumOfTask];
#define             EXTI_GPIO_CLK                        (RCC_APB2Periph_GPIOA )     
#define             EXTI_GPIO_PORT                       GPIOA   
#define             EXTI_GPIO_PIN                        GPIO_Pin_14
#define             EXTI_SOURCE_PORT                     GPIO_PortSourceGPIOA
#define             EXTI_SOURCE_PIN                      GPIO_PinSource14
#define             EXTI_LINE                            EXTI_Line14
#define             EXTI_IRQ                             EXTI15_10_IRQn


#define ENABLE_INV_INTERRUPTS  EnableInvInterrupt()
#define DISABLE_INV_INTERRUPTS DisableInvInterrupt()


void EXTI_Pxy_Config(void);
void EnableInvInterrupt(void);
void DisableInvInterrupt(void);
void TimingDelay_Decrement(void);
void TimeStamp_Increment(void);


#endif


