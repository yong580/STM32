#include "key.h"
uint8_t Key_Scan(void)
{
    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == KEY_ON)
    {
        while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == KEY_ON)
            ;
        return KEY_ON;
    }
    else
    {
        return KEY_OFF;
    }
}