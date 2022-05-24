#ifndef __USART_H
#define	__USART_H

#include "stm32f10x.h" 
#include <stdio.h>
#include <string.h>

extern u8 USAR3_fal;        //���ڽ�����ɱ�־λ
extern char RX3_Buff[100];   //����3���ջ�����
extern u16 USART_RX3_STA;
extern u8 busy;


    
void USART3_Init(u32 Bound);
void usart3_send_byte(u8 data);

void usart1_send_byte(u8 data);
void USART1_Init(u32 Bound);

//---------------------------------------------------------------

#define FRAME_LENTH               16    // ָ���
#define FRAME_START               0xAA  // Э��֡��ʼ
#define FRAME_END                 '/'   // Э��֡����
#define FRAME_CHECK_BEGIN          1    // У���뿪ʼ��λ�� RxBuf[1]
#define FRAME_CHECKSUM            14    // У�����λ��   RxBuf[14]
#define FRAME_CHECK_NUM           13    // ��ҪУ����ֽ���
#define FILL_VALUE                0x55  // ���ֵ
#define CODE_SETPID               0x07  // ����PID����
#define CODE_SETTGT               0x08  // ����Ŀ��ֵ
#define CODE_RESET                0x09   // ��λ����
#define CODE_STARTMOTOR           0x0A   // �������

typedef union {
  char Ch[4];
  float Float;
  int32_t Int;
}Format_UnionTypedef;

typedef struct {
  __IO uint8_t  Code ;  	
  __IO Format_UnionTypedef data[3];//����֡��3������
}MSG_TypeDef;


uint8_t CheckSum(uint8_t *Ptr,uint8_t Num );
void Transmit_Message(u32 PID_Out);
void Transmit_float(float PID_Out);   //����������
float Hex2Float(unsigned long number);//hex  ת float
void Uart_Rx_PID_Analyze(void);//������pid����

void Send_Message(u8* str);
void Uart8_init(void);

//У��
void judge_AA_CS_2F(void);
long FloatTohex(float HEX);
//******************************** MyUsart *****************************************
//**********************************************************************************
// ����1-USART1
#define  DEBUG_USARTx                   USART1
#define  DEBUG_USART_CLK                RCC_APB2Periph_USART1
#define  DEBUG_USART_APBxClkCmd         RCC_APB2PeriphClockCmd
#define  DEBUG_USART_BAUDRATE           115200

// USART GPIO ���ź궨��
#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  DEBUG_USART_TX_GPIO_PORT         GPIOA   
#define  DEBUG_USART_TX_GPIO_PIN          GPIO_Pin_9
#define  DEBUG_USART_RX_GPIO_PORT       GPIOA
#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_10

#define  DEBUG_USART_IRQ                USART1_IRQn
#define  DEBUG_USART_IRQHandler         USART1_IRQHandler


// ����2-USART2
//#define  DEBUG_USARTx                   USART2
//#define  DEBUG_USART_CLK                RCC_APB1Periph_USART2
//#define  DEBUG_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
//#define  DEBUG_USART_BAUDRATE           115200

//// USART GPIO ���ź궨��
//#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
//#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
//    
//#define  DEBUG_USART_TX_GPIO_PORT       GPIOA   
//#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_2
//#define  DEBUG_USART_RX_GPIO_PORT       GPIOA
//#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_3

//#define  DEBUG_USART_IRQ                USART2_IRQn
//#define  DEBUG_USART_IRQHandler         USART2_IRQHandler
void USART_Config(void);
#endif
