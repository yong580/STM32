#include "Uart.h"
#include "stdio.h"
#include "math.h"
#include "oled.h"

//void Uart1_init(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;		
//	NVIC_InitTypeDef NVIC_InitStructure;
//	USART_InitTypeDef USART_InitStructure;
//	
//	RCC_APB2PeriphClockCmd(USARTz_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);
//	RCC_APB2PeriphClockCmd(USARTz_CLK, ENABLE);  
//	GPIO_PinRemapConfig(GPIO_Remap_USART3, ENABLE);
//	
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_InitStructure.GPIO_Pin = USARTz_RxPin;
//	GPIO_Init(USARTz_GPIO, &GPIO_InitStructure);  
//	
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Pin = USARTz_TxPin;
//	GPIO_Init(USARTz_GPIO, &GPIO_InitStructure);  
//	
//	USART_InitStructure.USART_BaudRate = 9600;
//  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//  USART_InitStructure.USART_StopBits = USART_StopBits_1;
//  USART_InitStructure.USART_Parity = USART_Parity_No;
//  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;


//  USART_Init(USARTz, &USART_InitStructure);
//  USART_ITConfig(USARTz, USART_IT_RXNE, ENABLE);


//  USART_Cmd(USARTz, ENABLE);
//	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
//	NVIC_InitStructure.NVIC_IRQChannel = USARTz_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
//}

u8 string_Tx[] = "OK\r\n";
void Send_Message(u8* str)
{
	u8 index = 0;
	do
	{
		USART_SendData(USART3, str[index++]);
		while(USART_GetFlagStatus(USART3,USART_FLAG_TXE) == RESET);
	}while(str[index] != '\0');
	index = 0;
}

u8 Uart[16];     //接收缓冲
u8 Uart_Tx[16];  //发送缓冲
u8 value[20];
u8 String_Re[20];

void Uart_Re(void)  // 按格式输入  并输出显示
{

	u8 t = 0;
	
//****************读取、倒序保存*******************//	
	
	while(USART_GetFlagStatus(USART3,USART_FLAG_RXNE)!= SET);
	Uart[6] = USART_ReceiveData(USART3);    
	
	for(t=0;t<6;t++)  //全体向左移位一次
	{
			Uart[t] = Uart[t+1];
	}
	Uart[6] = '\0';
	
	sprintf((char*)String_Re,"%x  %x  %x",Uart[0],Uart[1],Uart[2]);
	OLED_ShowString(16,0,String_Re,1);
	
	sprintf((char*)String_Re,"%x",Uart[3]);
	OLED_ShowString(16,2,String_Re,1);
				
//	sprintf((char*)String_Re,"%x  %x ",Uart[6],Uart[7]);
//	OLED_ShowString(16,4,String_Re,1);		
}

u8 cnt_entert = 0;
u8 count_flag = 0;
//void USARTz_IRQHandler(void)
//{
//	
//	static u8 Re_Count = 0;
//	if(USART_GetITStatus(USARTz, USART_IT_RXNE) != RESET)
//  {
//	
//		Uart[Re_Count] = USART_ReceiveData(USART3);
//		

//	
//		if(Re_Count == 15)
//		{
//			Re_Count = 0;
//			count_flag = 1;
//		}
//		else 
//			Re_Count++;

//		
//	}
//}




//////用于校验串口发来的数据
uint8_t CheckSum(uint8_t *Ptr,uint8_t Num )
{
  uint8_t Sum = 0;
  while(Num--)
  {
    Sum += *Ptr;
    Ptr++;
  }
  return Sum;
}





void judge_AA_CS_2F(void)
{

    if(Uart[0] != FRAME_START )    // ????
    {
      return;
    }
    if(Uart[FRAME_LENTH-1] == FRAME_END ) // ????
    { 
      if(CheckSum((uint8_t*)&Uart[FRAME_CHECK_BEGIN],FRAME_CHECK_NUM) != Uart[FRAME_CHECKSUM] )
      {
      
        return;
      }
      else
      {
				
				
				//此处空白处为处理串口数据所留//
				
				Uart_Rx_PID_Analyze();//更新PID数据
				Send_Message(string_Tx);
      }
    }
}

MSG_TypeDef Msg;


void Transmit_Message(u32 PID_Out)
{
	u8 i = 0;
	u8 Tx_cnt = 0;
	for(i=0;i<FRAME_LENTH;i++)
  {
    Uart_Tx[i] = FILL_VALUE;  // ???? 0x55
  }
	//Msg.data[0].Int = *Feedback;
	Msg.data[0].Int = PID_Out; //测试  从低位开始存放 d4 c3 b2 a1
	
	Uart_Tx[0] = FRAME_START;   // 帧头
  Uart_Tx[1] = 0x80|CODE_SETTGT; // 指令码
  Uart_Tx[2] = Msg.data[0].Ch[0];
  Uart_Tx[3] = Msg.data[0].Ch[1];
  Uart_Tx[4] = Msg.data[0].Ch[2];
  Uart_Tx[5] = Msg.data[0].Ch[3];
  
  Uart_Tx[FRAME_CHECKSUM] = CheckSum((uint8_t*)&Uart_Tx[FRAME_CHECK_BEGIN],FRAME_CHECK_NUM);  // 计算校验和
  Uart_Tx[FRAME_LENTH-1] = FRAME_END;   // 加入帧尾
	
	for(Tx_cnt=0;Tx_cnt<16;Tx_cnt++)
	{
		USART_SendData(USART3,Uart_Tx[Tx_cnt]);
		while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)!= SET);
	}
}

////////////////////hex转float//////////////


//hex转float
float Hex2Float(unsigned long number)
{
    unsigned int sign = (number & 0x80000000) ? -1 : 1;
    unsigned int exponent = ((number >> 23) & 0xff) - 127;
    float mantissa = 1 + ((float)(number & 0x7fffff) / 0x7fffff);
    return sign * mantissa * pow(2, exponent);
}





////处理串口发来的数据//////////

float result_Float_P,result_Float_I,result_Float_D;
unsigned long Uart_Hex_P,Uart_Hex_I,Uart_Hex_D;

void Uart_Rx_PID_Analyze(void)
{
		Uart_Hex_P = (Uart[2]*16*16*16*16*16*16)+(Uart[3]*16*16*16*16)+(Uart[4]*16*16)+Uart[5];		
		if(Uart_Hex_P != 0)
		result_Float_P = Hex2Float(Uart_Hex_P); 
		else 
		result_Float_P = 0;	
		

		
/////////////////////////		
		
		Uart_Hex_I = (Uart[6]*16*16*16*16*16*16)+(Uart[7]*16*16*16*16)+(Uart[8]*16*16)+Uart[9];
		
		if(Uart_Hex_I != 0)
		result_Float_I = Hex2Float(Uart_Hex_I);
	  else 
		result_Float_I = 0;	
		
		
		
/////////////////////////////		
		
		Uart_Hex_D = (Uart[10]*16*16*16*16*16*16)+(Uart[11]*16*16*16*16)+(Uart[12]*16*16)+Uart[13];
		
		if(Uart_Hex_D != 0)
		result_Float_D = Hex2Float(Uart_Hex_D);
		else 
		result_Float_D = 0;	
		
	 
}





