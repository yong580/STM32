#ifndef _PID_TEST_
#define _PID_TEST_
#include "usart.h"

MSG_TypeDef Msg;
u8 Uart[16];     //���ջ���
u8 Uart_Tx[16];  //���ͻ���
u8 cnt_entert = 0;
u8 count_flag = 0;

float result_Float_P,result_Float_I,result_Float_D;
unsigned long Uart_Hex_P,Uart_Hex_I,Uart_Hex_D;

void judge_AA_CS_2F(void);
void Uart_Rx_PID_Analyze(void);
float Hex2Float(unsigned long number);//hexתfloat
void Transmit_float(float PID_Out);   //����������
void Transmit_float(float PID_Out);   //����������
long FloatTohex(float HEX);  //������תHEX
void Transmit_Message(u32 PID_Out);
//////����У�鴮�ڷ���������
uint8_t CheckSum(uint8_t *Ptr,uint8_t Num );



#endif
