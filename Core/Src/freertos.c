/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
 #include <stdio.h>
 #include <string.h>
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
#include "adc.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

uint16_t adcValue[4] = {0,};

uint8_t btn_r_flag = 0;
uint8_t btn_l_flag = 0;

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for task1 */
osThreadId_t task1Handle;
const osThreadAttr_t task1_attributes = {
  .name = "task1",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for task2 */
osThreadId_t task2Handle;
const osThreadAttr_t task2_attributes = {
  .name = "task2",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for task3 */
osThreadId_t task3Handle;
const osThreadAttr_t task3_attributes = {
  .name = "task3",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal2,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
    if(GPIO_Pin == GPIO_PIN_14){
        btn_l_flag = 1;
    }
    else if(GPIO_Pin == GPIO_PIN_10){
        btn_r_flag = 1;
    }
    return;
}

PUTCHAR_PROTOTYPE{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
    return ch;
}
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void thread1(void *argument);
void thread2(void *argument);
void thread3(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  HAL_ADC_Start_DMA(&hadc1, adcValue, 4); // 채널 4개
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
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of task1 */
  task1Handle = osThreadNew(thread1, NULL, &task1_attributes);

  /* creation of task2 */
  task2Handle = osThreadNew(thread2, NULL, &task2_attributes);

  /* creation of task3 */
  task3Handle = osThreadNew(thread3, NULL, &task3_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
    osDelay(500);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_thread1 */
/**
* @brief Function implementing the task1 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_thread1 */
void thread1(void *argument)
{
  /* USER CODE BEGIN thread1 */
  /* Infinite loop */
  uint8_t btn_temp;
  for(;;)
  { 
    btn_temp = 0;
    if (btn_l_flag == 1){
        btn_temp += 1;
        btn_l_flag = 0;
    }
    if (btn_r_flag == 1){
        btn_temp += 2;
        btn_r_flag = 0;
    }
    // left_x, left_y, right_y, right_x, btn_l:1, btn_r:2 btn_l+btn_r:3
    printf("%d,%d,%d,%d,%d\n", adcValue[0], adcValue[1], adcValue[2], adcValue[3], btn_temp);
    osDelay(20);
  }
  /* USER CODE END thread1 */
}

/* USER CODE BEGIN Header_thread2 */
/**
* @brief Function implementing the task2 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_thread2 */
void thread2(void *argument)
{
  /* USER CODE BEGIN thread2 */
  /* Infinite loop */
  for(;;)
  {
    //
    // HAL_UART_Transmit(&huart1, "Hello from task2\n", 17, 1000);
    osDelay(500);
  }
  /* USER CODE END thread2 */
}

/* USER CODE BEGIN Header_thread3 */
/**
* @brief Function implementing the task3 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_thread3 */
void thread3(void *argument)
{
  /* USER CODE BEGIN thread3 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(500);
  }
  /* USER CODE END thread3 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

