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

#include "fatfs.h"
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

  FATFS fs;
  FRESULT res;
  FIL file;
  UINT bytesWritten;
  char writeBuffer[64];
  char readBuffer[64] = {0};


  HC595_Init(); // Initialize the 74HC595
  HC165_Init(); // Initialize the 74HC165

  

  // Mount SD card
  res = f_mount(&fs, "0:", 1);
  if (res != FR_OK) {
    printf("SD card mount failed, error code: %d\n", res);
  } else {
    printf("SD card mounted successfully!\n");
  }

  // 创建文件夹
  res = f_mkdir("/testdir");
  if (res == FR_OK) {
    printf("Directory created successfully!\n");
  } else {
    printf("Failed to create directory, error code: %d\n", res);
  }

  res = f_open(&file, "/testdir/test.txt", FA_CREATE_ALWAYS | FA_WRITE);
  if (res == FR_OK) {
    // Prepare test data
    sprintf(writeBuffer, "STM32 SD Card Test: %lu\r\n", HAL_GetTick());
    
    // Write to file
    res = f_write(&file, writeBuffer, strlen(writeBuffer), &bytesWritten);
    if (res == FR_OK) {
      printf("Write successful, %u bytes written\r\n", bytesWritten);
    } else {
      printf("Write failed, error: %d\r\n", res);
    }
  }
  f_close(&file);
  res = f_open(&file, "/testdir/test.txt", FA_READ);
  if (res == FR_OK) {
    res == f_read(&file, readBuffer, sizeof(readBuffer) - 1, &bytesWritten);
    if (res == FR_OK) {
      readBuffer[bytesWritten] = '\0'; // Null-terminate the string
      printf("Read successful, %u bytes read: %s\r\n", bytesWritten, readBuffer);
    } else {
      printf("Read failed, error: %d\r\n", res);
    }
    f_close(&file);
  } else {
    printf("Failed to open file for reading, error: %d\r\n", res);
  }
  f_close(&file);





  

  /* Infinite loop */
  for(;;)
  {
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
    HC595_ToggleIO(0); 
    HC595_ToggleIO(1); 
    HC595_ToggleIO(2); 
    HC595_ToggleIO(3);
    osDelay(1000);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

