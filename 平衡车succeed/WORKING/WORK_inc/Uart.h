#ifndef __UART_H
#define __UART_H
#include "stm32f10x.h"

#define USARTz                   USART3
#define USARTz_GPIO              GPIOB
#define USARTz_CLK               RCC_APB1Periph_USART3
#define USARTz_GPIO_CLK          RCC_APB2Periph_GPIOB
#define USARTz_RxPin             GPIO_Pin_11
#define USARTz_TxPin             GPIO_Pin_10
#define USARTz_IRQn              USART3_IRQn
#define USARTz_IRQHandler        USART3_IRQHandler


void Send_Message(u8* str);
void Uart8_init(void);

//У��
void judge_AA_CS_2F(void);


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


//�������ݵ�����

typedef union {
  char Ch[4];
  float Float;
  int32_t Int;
}Format_UnionTypedef;

typedef struct {
  __IO uint8_t  Code ;  	
  __IO Format_UnionTypedef data[3];//����֡��3������
}MSG_TypeDef;

void Transmit_Message(u32 PID_Out);

float Hex2Float(unsigned long number);//hex  ת float
void Uart_Rx_PID_Analyze(void);//������pid����

#endif
