#ifndef _PID_TEST_
#define _PID_TEST_
#include "usart.h"

MSG_TypeDef Msg;
u8 Uart[16];     //接收缓冲
u8 Uart_Tx[16];  //发送缓冲
u8 cnt_entert = 0;
u8 count_flag = 0;

float result_Float_P,result_Float_I,result_Float_D;
unsigned long Uart_Hex_P,Uart_Hex_I,Uart_Hex_D;

void judge_AA_CS_2F(void);
void Uart_Rx_PID_Analyze(void);
float Hex2Float(unsigned long number);//hex转float
void Transmit_float(float PID_Out);   //浮点数发送
void Transmit_float(float PID_Out);   //浮点数发送
long FloatTohex(float HEX);  //浮点数转HEX
void Transmit_Message(u32 PID_Out);
//////用于校验串口发来的数据
uint8_t CheckSum(uint8_t *Ptr,uint8_t Num );



#endif
