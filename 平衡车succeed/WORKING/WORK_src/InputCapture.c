#include "InputCapture.H"
/*-------------------------------------------------------------------------My_Config-----------------------------------------------------------------------------------*/

void ENCODER_TIM_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//输出捕获通道1 GPIO初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //开启AFIO时钟
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//使能JTAGDisable，即禁用JTAG接口


	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8 ;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//输出捕获通道2 GPIO初始化
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9 ;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	//TIM2 CH1 CH2的GPIO初始化
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//输出捕获通道2 GPIO初始化
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

void ENCODER_TIM_Mode_Config(void)
{	
		//开启TIM1时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
		
	    //时基配//时基结构体
		TIM_TimeBaseInitTypeDef  ENCODER_TimeBaseInitStruct;
		ENCODER_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
		ENCODER_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
		ENCODER_TimeBaseInitStruct.TIM_Period=999;//此处只要大于最大可能计数值就行，留有余量，因为encoder最大为250
		ENCODER_TimeBaseInitStruct.TIM_Prescaler=0x00;
		// 重复计数器的值，没用到不用管
		ENCODER_TimeBaseInitStruct.TIM_RepetitionCounter=0;
		//初始化两个定时器时基
		TIM_TimeBaseInit(TIM1, &ENCODER_TimeBaseInitStruct);

		//编码器模式配置

		TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
		//***************************************************************
		TIM_ICInitTypeDef  TIM_ICInitStructure;
		TIM_ICStructInit(&TIM_ICInitStructure);//结构初始化
		TIM_ICInitStructure.TIM_ICFilter = 10; //滤波
		//两个捕获配置
		TIM_ICInit(TIM1, &TIM_ICInitStructure);

		TIM_ClearFlag(TIM1, TIM_FLAG_Update);//清除TIM的更新标志位
		TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
		 
		//计数器清零 
		TIM_SetCounter(TIM1,0);
		TIM_Cmd(TIM1, ENABLE);
}

void ENCODER_TIM2_Mode_Config(void)
{	
		//开启TIM2时钟
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
		//时基配置
		//时基结构体
		//***************************************************************
		TIM_TimeBaseInitTypeDef  ENCODER_TimeBaseInitStruct;

		ENCODER_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
		ENCODER_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
		ENCODER_TimeBaseInitStruct.TIM_Period=999;
		ENCODER_TimeBaseInitStruct.TIM_Prescaler=0x00;
		// 重复计数器的值，没用到不用管
		ENCODER_TimeBaseInitStruct.TIM_RepetitionCounter=0;
		//初始化定时器时基
		TIM_TimeBaseInit(TIM2, &ENCODER_TimeBaseInitStruct);
		//*****************************************************************



		//编码器模式配置

		TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
		//***************************************************************
		TIM_ICInitTypeDef  TIM_ICInitStructure;
		TIM_ICStructInit(&TIM_ICInitStructure);//结构初始化
		TIM_ICInitStructure.TIM_ICFilter = 10; //滤波
		//两个捕获配置
		TIM_ICInit(TIM2, &TIM_ICInitStructure);
		 
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);//清除TIM的更新标志位
		TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
		//计数器清零 
		TIM_SetCounter(TIM2,0);
		TIM_Cmd(TIM2, ENABLE);
}

//读取编码器的计数值
int32_t My_Read_Encoder(u8 TIMX)
{
		 int32_t Encoder_TIM;    
		 switch(TIMX)
		 {
			 case 1:  
			 {
				if((int32_t)TIM1-> CNT>700)
				 {
					 Encoder_TIM=((int32_t)TIM1-> CNT)-1000;
				 }
				else
				 {
					 Encoder_TIM=(int32_t)TIM1 -> CNT;  
				 }
				 TIM1 -> CNT=0;
				 break;
				}
			 case 2:  
			 { 
				 if((int32_t)TIM2-> CNT>700)
				  {
					Encoder_TIM=((int32_t)TIM2-> CNT)-1000;
				  }
				 else
				  {
					Encoder_TIM=(int32_t)TIM2 -> CNT;  
				  }
				  TIM2 -> CNT=0;
				  break;
			 }
			 default:  Encoder_TIM=888;
		 }
			return Encoder_TIM;
}

void Encoder_TIM_init(void)
{	

	ENCODER_TIM_GPIO_Config();
	ENCODER_TIM_Mode_Config();
	ENCODER_TIM2_Mode_Config();
}




