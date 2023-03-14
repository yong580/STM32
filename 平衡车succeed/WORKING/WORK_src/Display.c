#include "Display.h"
#include "com_inc.h"

u8 set = 0;

void Display_All(void)
{
    if(set == 0) Display_CSB();
    else if(set == 1) Display_U();
    OLED_Refresh(); //更新Oled
}

void Display_CSB(void)   //超声波距离显示
{
    OLED_ShowChinese(16,8,4,16);   //距
    OLED_ShowChinese(32,8,5,16);   //离
    sprintf((char *)string, ": %.2fcm  ",Dis);
    OLED_ShowString(48,8,string ,16);
    
    OLED_ShowChinese(16,36,6,16);   //角
    OLED_ShowChinese(32,36,7,16);   //度
    sprintf((char *)string, ": %.2fC  ",yaw);
    OLED_ShowString(48,36,string ,16);
}

void Display_U(void)
{
    OLED_ShowChinese(16,8,0,16);   //电
    OLED_ShowChinese(32,8,3,16);   //压
    sprintf((char *)string, ": %.2fV   ",dian);
    OLED_ShowString(48,8,string ,16);
    
    OLED_ShowChinese(16,36,0,16);   //电
    OLED_ShowChinese(32,36,2,16);   //量
    sprintf((char *)string, ": %d%%   ",Point);
    OLED_ShowString(48,36,string ,16);
}

void Calculate_U(void)  //电压计算
{
    u16 val;
    float U;
    
    val = Kalman_Filter_U(ADC_Read_PA0());
    U = 3.3 * val /4096.0;
    
    dian = U * 11.51 / 2.372 ;
    Point = ((dian - 11.1) / 1.4) * 100;
}

void Key_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}



void KEY_Scan(void)
{
	static u8 up = 1;
	if(up == 1 && KEY == 0)
	{
		delay_ms(15);
		if(KEY == 0)
        {
            up = 0;
            set = !set;
            OLED_Clear();
        }
	}
	else if(KEY == 1) up = 1;
}

