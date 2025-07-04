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
#include "lcd_st7735s_driver.h"
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

osTimerId_t ledTimerHandle;
const osTimerAttr_t ledTimer_attributes = {
  .name = "LEDTimer"
};


// LED状态表
typedef struct {
  uint8_t led1;
  uint8_t led2;
  uint8_t led3;
  uint8_t led4;
} LedState;

// 定义LED状态序列
const LedState ledStates[12] = {
  {1, 0, 0, 0}, // 状态0: 仅LED1亮
  {0, 1, 0, 0}, // 状态1: 仅LED2亮
  {0, 0, 1, 0}, // 状态2: 仅LED3亮
  {0, 0, 0, 1}, // 状态3: 仅LED4亮
  {1, 1, 0, 0}, // 状态4: LED1和LED2亮
  {0, 1, 1, 0}, // 状态5: LED2和LED3亮
  {0, 0, 1, 1}, // 状态6: LED3和LED4亮
  {1, 0, 1, 0}, // 状态7: LED1和LED3亮
  {1, 0, 0, 1}, // 状态8: LED1和LED4亮
  {0, 1, 0, 1}, // 状态9: LED2和LED4亮
  {1, 1, 1, 1}, // 状态10: 全亮
  {0, 0, 0, 0}  // 状态11: 全灭
};

uint8_t currentLedStateIndex = 0;  // 当前LED状态索引

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 20,
  .priority = (osPriority_t) osPriorityNormal,
};



/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

void LED_TimerCallback(void *argument);

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

  ledTimerHandle = osTimerNew(LED_TimerCallback, osTimerPeriodic, NULL, &ledTimer_attributes);
  osTimerStart(ledTimerHandle, 1000);  // 设置1000ms周期

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

  LCD_ST7735S_Init(); // Initialize the ST7735S LCD
  uint16_t colors[] = {
    0xF800,  // 红色 (RGB565格式)
    0x07E0,  // 绿色
    0x001F,  // 蓝色
    0xFFE0,  // 黄色
    0xF81F,  // 品红色
    0x07FF,  // 青色
    0xFFFF,  // 白色
    0x0000   // 黑色
  };

  uint8_t colorIndex = 0;
  uint8_t numColors = sizeof(colors) / sizeof(colors[0]);
  
  /* Infinite loop */
  for(;;)
  {
        // 选择当前颜色
        uint16_t currentColor = colors[colorIndex];
    
        LCD_ST7735S_RefreshAll(currentColor);
        
        // 更新颜色索引
        colorIndex = (colorIndex + 1) % numColors;
        
        // 延时3秒
        osDelay(1000);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */


 /**
  * @brief  LED定时器回调函数，1000ms周期调用
  * @param  argument: 未使用
  * @retval 无
  */
void LED_TimerCallback(void *argument)
{
  // 获取当前状态
  const LedState* currentState = &ledStates[currentLedStateIndex];
  
  // 根据当前状态控制LED
  if (currentState->led1) {
    HC595_SetIOHigh(0);  // 点亮LED1
  } else {
    HC595_SetIOLow(0); // 熄灭LED1
  }
  
  if (currentState->led2) {
    HC595_SetIOHigh(1);  // 点亮LED2
  } else {
    HC595_SetIOLow(1); // 熄灭LED2
  }
  
  if (currentState->led3) {
    HC595_SetIOHigh(2);  // 点亮LED3
  } else {
    HC595_SetIOLow(2); // 熄灭LED3
  }
  
  if (currentState->led4) {
    HC595_SetIOHigh(3);  // 点亮LED4
  } else {
    HC595_SetIOLow(3); // 熄灭LED4
  }
  
  // 更新为下一个状态
  currentLedStateIndex = (currentLedStateIndex + 1) % 12;
}

/* USER CODE END Application */

