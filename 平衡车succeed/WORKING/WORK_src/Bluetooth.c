#include "Bluetooth.h"
#include "com_inc.h"



char Spend_Val[20];  //�洢�ٶȵ���ֵ
int len;               //������յ����ݳ���
int Got_Speed;					//������ŴӴ��ڵõ����ٶ�����
int Got_Turn;                   //������ŴӴ��ڵõ��ĽǶ�����
u8 Direction = 1;       //�жϷ���ǰ�������
char Turn;              //�ж�����ת
u8 Rotating_fal = 0;    //����������
u16 Moto_Val_R,Moto_Val_L;           //ͨ���������͵�����ٶ�ֵ����

void Bluetooth_Command(void)
{
    if(USAR3_fal)
    {
        USAR3_fal = 0;
        len = USART_RX3_STA & 0X3FFF;
        USART_RX3_STA = 0;
        Bluetooth_Direction();
        Bluetooth_Spend();
        Bluetooth_Turn();
        Bluetooth_Rotating();
        memset(RX3_Buff, 0, sizeof RX3_Buff);
    }      
}

void Bluetooth_Rotating(void)  //������
{
    if(strcmp(RX3_Buff,"<Rota_L>") == 0 && Car_Speed == 0 && Stop_fal == 0)   //С�����յ��ַ���<Rota_L> �ٶ�Ϊ0��ǰ�����ϰ� ��ִ��
    {
        Rotating_fal = 1;
    }
    else if(strcmp(RX3_Buff,"<Rota_R>") == 0 && Car_Speed == 0 && Stop_fal == 0)
    {
        Rotating_fal = 2;
    }
    else if(strcmp(RX3_Buff,"<Rota_S>") == 0)
    {
        Rotating_fal = 0;
    }
     
}

void Bluetooth_Turn(void)  //С��ת��
{
    if(strcmp(RX3_Buff,"<Left>") == 0)
    {
        Turn = 'L';
    }
    else if(strcmp(RX3_Buff,"<Right>") == 0)
    {
        Turn = 'R';
    }
    else if(strcmp(RX3_Buff,"<OK>") == 0)
    {
        Turn = ' ';
    }
}

void Bluetooth_Direction(void)      //�ж�ǰ������˵ķ�����
{
    if(strcmp(RX3_Buff,"<Up>") == 0)        
    {
        Direction = 1;
        printf("up");
        Car_Speed = 0;            //�ٶ���������
        memset(RX3_Buff, 0, sizeof RX3_Buff);
    }        
    else if(strcmp(RX3_Buff,"<Down>") == 0)
    {
        Direction = 0;
        Car_Speed = 0;           //�ٶ���������
        printf("down");
        memset(RX3_Buff, 0, sizeof RX3_Buff);
    }        
}


void Bluetooth_Spend(void)  //����С���ٶȴ�С
{
//    char Spend[20];
//    static u8 i_s;
    static u8 i;
    if(RX3_Buff[1] == 'S' && RX3_Buff[2] == 'p' && RX3_Buff[3] == 'e' && RX3_Buff[4] == 'e'&& RX3_Buff[5] == 'd' && Stop_fal == 0)
    {
        for(i = 0; i < len; i++)
		{
            if((RX3_Buff[i] >= '0') && (RX3_Buff[i] <= '9'))
            {
               Got_Speed = RX3_Buff[i] - '0';
            }
        }
//        Got_Speed = (Spend_Val[0] - '0');
        if(Direction == 1)
        {
            Car_Speed = -Got_Speed*6;
        }
        else if(Direction == 0)
        {
            Car_Speed = Got_Speed*6;
        }
        memset(Spend_Val, 0, sizeof Spend_Val);
        memset(RX3_Buff, 0, sizeof RX3_Buff);
        printf("speed = %d  ",Car_Speed);
    }
    else if(strcmp(RX3_Buff,"<Stop>") == 0) //����ͣ��
    {
        Car_Speed = 0;
    }
}

void Send_Bluetooth_Data(void)
{
//    int Poi;
    Moto_Val_L = 70 - Encoder_Left;
    Moto_Val_R = 70 - Encoder_Right;
     
    printf("*L%d",Moto_Val_L);  //���������ٶ�
    printf("*R%d",Moto_Val_R);  //�����ҵ���ٶ�
    printf("*U%d",Point);     //���͵�ص���
    printf("delay");
}

