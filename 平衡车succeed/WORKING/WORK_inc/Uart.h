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

//校验
void judge_AA_CS_2F(void);


#define FRAME_LENTH               16    // 指令长度
#define FRAME_START               0xAA  // 协议帧开始
#define FRAME_END                 '/'   // 协议帧结束
#define FRAME_CHECK_BEGIN          1    // 校验码开始的位置 RxBuf[1]
#define FRAME_CHECKSUM            14    // 校验码的位置   RxBuf[14]
#define FRAME_CHECK_NUM           13    // 需要校验的字节数
#define FILL_VALUE                0x55  // 填充值
#define CODE_SETPID               0x07  // 设置PID参数
#define CODE_SETTGT               0x08  // 设置目标值
#define CODE_RESET                0x09   // 复位重启
#define CODE_STARTMOTOR           0x0A   // 启动电机


//发送数据到串口

typedef union {
  char Ch[4];
  float Float;
  int32_t Int;
}Format_UnionTypedef;

typedef struct {
  __IO uint8_t  Code ;  	
  __IO Format_UnionTypedef data[3];//数据帧有3个参数
}MSG_TypeDef;

void Transmit_Message(u32 PID_Out);

float Hex2Float(unsigned long number);//hex  转 float
void Uart_Rx_PID_Analyze(void);//处理串口pid数据

#endif
