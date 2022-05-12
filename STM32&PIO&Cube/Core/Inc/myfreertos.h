#ifndef __MYFREERTOS_H__
#define __MYFREERTOS_H__

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

void StartDefaultTask(void const *argument);
void StartTask1(void const *argument);
void MX_FREERTOS_Init(void);
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize);
#endif /* __FREERTOS_H__ */