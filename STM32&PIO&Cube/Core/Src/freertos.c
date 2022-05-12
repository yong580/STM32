/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "myfreertos.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
TaskHandle_t defaultTaskHandle;
// osThreadId Task1Handle;

TaskHandle_t Task1Handle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
//为空闲任务块和堆栈分配空间
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

//其实就是进行简单的传值，在提供给调度器中用于创建空闲任务
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize)
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void)
{
  /* USER CODE BEGIN Init */
  BaseType_t xReturn = pdPASS;
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  // osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  // defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);
  // osThreadDef(Task1, StartTask1, osPriorityNormal, 0, 128);
  // Task1Handle = osThreadCreate(osThread(Task1), NULL);

  //创建空闲任务，创建完就加入到就绪列表受调度器调度
  //由于优先级是最低0，后续会被调度器插入到下标为0的就绪链表中，视为空闲任务。
  defaultTaskHandle = xTaskCreateStatic((TaskFunction_t)StartDefaultTask,
                                        (const char *const)"DefaultTask",
                                        (const uint32_t)128,
                                        (void *const)NULL,
                                        (UBaseType_t)osPriorityNormal,
                                        (StackType_t *const)&defaultTaskHandle,
                                        (StaticTask_t *const)&xIdleStack);
  //创建任务1
  xReturn = xTaskCreate((TaskFunction_t)StartTask1,
                        (const char *)"Task1",
                        (uint16_t)128, (void *)NULL,
                        (UBaseType_t)1,
                        (TaskHandle_t *)&Task1Handle);
  if (xReturn != pdPASS)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */
}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for (;;)
  {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
    //库函数已经帮你将systick的频率转换为ms，但是最小值为1ms
    osDelay(1000);
  }
  /* USER CODE END StartDefaultTask */
}
void StartTask1(void const *argument)
{
  /* USER CODE BEGIN StartTask1 */
  for (;;)
  {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
    osDelay(1000);
  }
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
