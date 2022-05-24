#include "Bluetooth.h"
#include "com_inc.h"



char Spend_Val[20];  //存储速度的数值
int len;               //保存接收的数据长度
int Got_Speed;					//用来存放从串口得到的速度数据
int Got_Turn;                   //用来存放从串口得到的角度数据
u8 Direction = 1;       //判断方向，前进或后退
char Turn;              //判断左右转
u8 Rotating_fal = 0;    //左旋，右旋
u16 Moto_Val_R,Moto_Val_L;           //通过蓝牙发送电机的速度值变量

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

void Bluetooth_Rotating(void)  //左右旋
{
    if(strcmp(RX3_Buff,"<Rota_L>") == 0 && Car_Speed == 0 && Stop_fal == 0)   //小车接收到字符串<Rota_L> 速度为0，前方无障碍 才执行
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

void Bluetooth_Turn(void)  //小车转弯
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

void Bluetooth_Direction(void)      //判断前进或后退的方向函数
{
    if(strcmp(RX3_Buff,"<Up>") == 0)        
    {
        Direction = 1;
        printf("up");
        Car_Speed = 0;            //速度慢慢归零
        memset(RX3_Buff, 0, sizeof RX3_Buff);
    }        
    else if(strcmp(RX3_Buff,"<Down>") == 0)
    {
        Direction = 0;
        Car_Speed = 0;           //速度慢慢归零
        printf("down");
        memset(RX3_Buff, 0, sizeof RX3_Buff);
    }        
}


void Bluetooth_Spend(void)  //控制小车速度大小
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
    else if(strcmp(RX3_Buff,"<Stop>") == 0) //紧急停车
    {
        Car_Speed = 0;
    }
}

void Send_Bluetooth_Data(void)
{
//    int Poi;
    Moto_Val_L = 70 - Encoder_Left;
    Moto_Val_R = 70 - Encoder_Right;
     
    printf("*L%d",Moto_Val_L);  //发送左电机速度
    printf("*R%d",Moto_Val_R);  //发送右电机速度
    printf("*U%d",Point);     //发送电池电量
    printf("delay");
}

