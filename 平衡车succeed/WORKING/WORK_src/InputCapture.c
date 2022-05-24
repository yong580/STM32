#include "InputCapture.H"
/*-------------------------------------------------------------------------My_Config-----------------------------------------------------------------------------------*/

void ENCODER_TIM_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//�������ͨ��1 GPIO��ʼ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //����AFIOʱ��
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//ʹ��JTAGDisable��������JTAG�ӿ�


	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8 ;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//�������ͨ��2 GPIO��ʼ��
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9 ;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	//TIM2 CH1 CH2��GPIO��ʼ��
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//�������ͨ��2 GPIO��ʼ��
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

void ENCODER_TIM_Mode_Config(void)
{	
		//����TIM1ʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
		
	    //ʱ����//ʱ���ṹ��
		TIM_TimeBaseInitTypeDef  ENCODER_TimeBaseInitStruct;
		ENCODER_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
		ENCODER_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
		ENCODER_TimeBaseInitStruct.TIM_Period=999;//�˴�ֻҪ���������ܼ���ֵ���У�������������Ϊencoder���Ϊ250
		ENCODER_TimeBaseInitStruct.TIM_Prescaler=0x00;
		// �ظ���������ֵ��û�õ����ù�
		ENCODER_TimeBaseInitStruct.TIM_RepetitionCounter=0;
		//��ʼ��������ʱ��ʱ��
		TIM_TimeBaseInit(TIM1, &ENCODER_TimeBaseInitStruct);

		//������ģʽ����

		TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
		//***************************************************************
		TIM_ICInitTypeDef  TIM_ICInitStructure;
		TIM_ICStructInit(&TIM_ICInitStructure);//�ṹ��ʼ��
		TIM_ICInitStructure.TIM_ICFilter = 10; //�˲�
		//������������
		TIM_ICInit(TIM1, &TIM_ICInitStructure);

		TIM_ClearFlag(TIM1, TIM_FLAG_Update);//���TIM�ĸ��±�־λ
		TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
		 
		//���������� 
		TIM_SetCounter(TIM1,0);
		TIM_Cmd(TIM1, ENABLE);
}

void ENCODER_TIM2_Mode_Config(void)
{	
		//����TIM2ʱ��
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
		//ʱ������
		//ʱ���ṹ��
		//***************************************************************
		TIM_TimeBaseInitTypeDef  ENCODER_TimeBaseInitStruct;

		ENCODER_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
		ENCODER_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
		ENCODER_TimeBaseInitStruct.TIM_Period=999;
		ENCODER_TimeBaseInitStruct.TIM_Prescaler=0x00;
		// �ظ���������ֵ��û�õ����ù�
		ENCODER_TimeBaseInitStruct.TIM_RepetitionCounter=0;
		//��ʼ����ʱ��ʱ��
		TIM_TimeBaseInit(TIM2, &ENCODER_TimeBaseInitStruct);
		//*****************************************************************



		//������ģʽ����

		TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
		//***************************************************************
		TIM_ICInitTypeDef  TIM_ICInitStructure;
		TIM_ICStructInit(&TIM_ICInitStructure);//�ṹ��ʼ��
		TIM_ICInitStructure.TIM_ICFilter = 10; //�˲�
		//������������
		TIM_ICInit(TIM2, &TIM_ICInitStructure);
		 
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);//���TIM�ĸ��±�־λ
		TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
		//���������� 
		TIM_SetCounter(TIM2,0);
		TIM_Cmd(TIM2, ENABLE);
}

//��ȡ�������ļ���ֵ
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




