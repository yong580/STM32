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
osThreadId defaultTaskHandle;
TaskHandle_t Task1Handle;
TaskHandle_t UsartcontorlHandle;
TaskHandle_t ADCHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void StartDefaultTask(void const *argument);
void StartTask1(void const *argument);
void StartUsartcontorl(void const *argument);
void StartADCHandle(void const *argument);
/* USER CODE END FunctionPrototypes */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize);

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

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
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  xReturn = xTaskCreate((TaskFunction_t)StartTask1,
                        (const char *const)"Task1",
                        (const uint16_t)configMINIMAL_STACK_SIZE,
                        (void *const)NULL,
                        (UBaseType_t)1,
                        (TaskHandle_t *const)&Task1Handle);
  if (xReturn != pdPASS)
  {
    /* Error */
    printf("create Task1 ERROR\r\n");
  }

  xReturn = xTaskCreate((TaskFunction_t)StartUsartcontorl,
                        (const char *const)"Usartcontorl",
                        (const uint16_t)configMINIMAL_STACK_SIZE,
                        (void *const)NULL,
                        (UBaseType_t)2,
                        (TaskHandle_t *const)&UsartcontorlHandle);

  if (xReturn != pdPASS)
  {
    /* Error */
    printf("create Usartcontorl ERROR\r\n");
  }
  xReturn = xTaskCreate((TaskFunction_t)StartADCHandle,
                        (const char *const)"ADCHandle",
                        (const uint16_t)configMINIMAL_STACK_SIZE,
                        (void *const)NULL,
                        (UBaseType_t)3,
                        (TaskHandle_t *const)&ADCHandle);
  if (xReturn != pdPASS)
  {
    /* Error */
    printf("create ADCHandle ERROR\r\n");
  }
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
    // HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
    printf("led0\r\n");
    osDelay(1000);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void StartTask1(void const *argument)
{
  /* USER CODE BEGIN StartTask1 */
  /* Infinite loop */
  for (;;)
  {
    // HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
    printf("led1\r\n");
    osDelay(500);
  }
  /* USER CODE END StartTask1 */
}
void StartUsartcontorl(void const *argument)
{
  /* USER CODE BEGIN StartUsartcontorl */
  /* Infinite loop */
  extern uint8_t ch;
  for (;;)
  {
    if (ch == 'a')
    {
      HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
      printf("led2\r\n");
    }
    vTaskDelay(100);
  }
  /* USER CODE END StartUsartcontorl */
}

void StartADCHandle(void const *argument)
{
  extern __IO uint32_t ADC_ConvertedValue;
  float ADC_val = 0;
  /* USER CODE BEGIN StartADCHandle */
  /* Infinite loop */
  for (;;)
  {
    printf("adc start\r\n");
    extern uint8_t adc_flag;
    if (adc_flag == 1)
    {
      printf("\r\nADC_ConvertedValue = 0x%ld \r\n", ADC_ConvertedValue);
      ADC_val = (float)ADC_ConvertedValue / 4095 * (float)3.3;
      
      ftprintf(ADC_val, 2);
      adc_flag = 0;
    }
    vTaskDelay(500);
  }
}
/* USER CODE END Application */
