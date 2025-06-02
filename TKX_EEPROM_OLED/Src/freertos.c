/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "74HC595.h"
#include "74HC165.h"
#include "eeprom.h"
#include "IIC_OLED.h"
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
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

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
  HC595_Init(); // Initialize the 74HC595
  HC165_Init(); // Initialize the 74HC165
  uint8_t ioState[3] = {0}; // Buffer to store the state of the 74HC165
  OLED_Init();
  OLED_Clear();
  OLED_Printf(128, 0, 16, 1, "EEPROM Test");
  osDelay(2000); // Delay for 2 seconds
  uint8_t eepromTestResult = EEPROM_Test(); // Test the EEPROM
  if (eepromTestResult)
  {
    OLED_Printf(128, 3, 16, 0, "Passed");
    printf("EEPROM Test Passed\n");
  }
  else
  {
    OLED_Printf(128, 3, 16, 0, "Failed");
    printf("EEPROM Test Failed\n");
  }
  osDelay(2000); // Delay for 2 seconds
  OLED_Clear();
  OLED_Printf(0, 0, 16, 1, "IO State Monitor");
  OLED_Printf(0, 2, 16, 1, "IO[0]: 0x00");
  OLED_Printf(0, 4, 16, 1, "IO[1]: 0x00");
  OLED_Printf(0, 6, 16, 1, "IO[2]: 0x00");
  /* Infinite loop */
  for(;;)
  {
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
    HC595_ToggleIO(0); // Toggle the first IO
    HC595_ToggleIO(1); // Toggle the first IO
    HC595_ToggleIO(2); // Toggle the first IO
    HC595_ToggleIO(3); // Toggle the first IO
    HC165_ReadMultiBytes(ioState, 3); // Read the state of the 74HC165
    // 更新OLED显示的IO状态
    OLED_Printf(0, 2, 16, 1, "IO[0]: 0x%02X", ioState[0]);
    OLED_Printf(0, 4, 16, 1, "IO[1]: 0x%02X", ioState[1]);
    OLED_Printf(0, 6, 16, 1, "IO[2]: 0x%02X", ioState[2]);
    
    printf("IO State: %02X %02X %02X\n", ioState[0], ioState[1], ioState[2]); // Print the state
    osDelay(100);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

