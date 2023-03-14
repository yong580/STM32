#include "PIDtest.h"
#include "math.h"

void judge_AA_CS_2F(void)
{

    if(Uart[0] != FRAME_START )    // ????
    {
      return;
    }
    if(Uart[FRAME_LENTH-1] == FRAME_END ) // ????
    { 
      if(CheckSum((uint8_t*)&Uart[FRAME_CHECK_BEGIN],FRAME_CHECK_NUM) != Uart[FRAME_CHECKSUM])
      {
      
        return;
      }
      else
      {
				//此处空白处为处理串口数据所留//
				Uart_Rx_PID_Analyze();//更新PID数据
//				Send_Message(string_Tx);
      }
    }
}

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
//		Uart_Hex_D = Uart[10]*1000 + Uart[11]*100 + Uart[12]*10 + Uart[13];
		if(Uart_Hex_D != 0)
		result_Float_D = Hex2Float(Uart_Hex_D);
		else 
		result_Float_D = 0;	
}

//hex转float
float Hex2Float(unsigned long number)
{
    unsigned int sign = (number & 0x80000000) ? -1 : 1;
    unsigned int exponent = ((number >> 23) & 0xff) - 127;
    float mantissa = 1 + ((float)(number & 0x7fffff) / 0x7fffff);
    return sign * mantissa * pow(2, exponent);
}


void Transmit_float(float PID_Out)   //浮点数发送
{
    long PID_Out_HEX;
    u8 i = 0;
    u8 Tx_cnt = 0;
    
    for(i=0;i<16;i++)
    {
        Uart_Tx[i] = FILL_VALUE;  // ???? 0x55
    }
    PID_Out_HEX = FloatTohex(PID_Out);
    Uart_Tx[0] = FRAME_START;       // 帧头
    Uart_Tx[1] = 0x80|CODE_SETTGT;  // 指令码
    Uart_Tx[3] = PID_Out_HEX / (16*16*16*16*16*16) % (16*16);
    Uart_Tx[2] = PID_Out_HEX / (16*16*16*16) % (16*16);
    Uart_Tx[4] = PID_Out_HEX / (16*16) % (16*16);
    Uart_Tx[5] = PID_Out_HEX;
    
    Uart_Tx[FRAME_CHECKSUM] = CheckSum((uint8_t*)&Uart_Tx[FRAME_CHECK_BEGIN],FRAME_CHECK_NUM);  // 计算校验和
    Uart_Tx[FRAME_LENTH-1] = FRAME_END;   // 加入帧尾
    
    	for(Tx_cnt=0;Tx_cnt<16;Tx_cnt++)
        {
            USART_SendData(USART3,Uart_Tx[Tx_cnt]);
            while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)!= SET);
        } 
}


long FloatTohex(float HEX)  //浮点数转HEX
{
    return *(long*)&HEX;
}

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
	
	Uart_Tx[0] = FRAME_START;       // 帧头
    Uart_Tx[1] = 0x80|CODE_SETTGT;  // 指令码
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




