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

#include "tftlcd.h"

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
  .stack_size = 128 * 8,
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

  uint8_t lcd_id[12];
  

  HC595_Init(); // Initialize the 74HC595
  HC165_Init(); // Initialize the 74HC165
  uint8_t ioState[3] = {0}; // Buffer to store the state of the 74HC165
  lcd_init(); // Initialize the LCD

    // 创建颜色表
  const uint16_t colorTable[] = {
      WHITE,      // 白色
      BLACK,      // 黑色
      RED,        // 红色
      GREEN,      // 绿色
      BLUE,       // 蓝色
      MAGENTA,    // 品红色/紫红色
      YELLOW,     // 黄色
      CYAN,       // 青色
      BROWN,      // 棕色
      BRRED,      // 棕红色
      GRAY,       // 灰色
      DARKBLUE,   // 深蓝色
      LIGHTBLUE,  // 浅蓝色
      GRAYBLUE,   // 灰蓝色
      LIGHTGREEN, // 浅绿色
      LGRAY,      // 浅灰色
      LGRAYBLUE,  // 浅灰蓝色
      LBBLUE      // 浅棕蓝色
  };
  
  // 颜色表大小
  const uint8_t colorTableSize = sizeof(colorTable) / sizeof(colorTable[0]);
  
  // 颜色名称表
  const char *colorNames[] = {
      "WHITE",
      "BLACK",
      "RED",
      "GREEN",
      "BLUE",
      "MAGENTA",
      "YELLOW",
      "CYAN",
      "BROWN",
      "BRRED",
      "GRAY",
      "DARKBLUE",
      "LIGHTBLUE",
      "GRAYBLUE",
      "LIGHTGREEN",
      "LGRAY",
      "LGRAYBLUE",
      "LBBLUE"
  };
  
  // 当前颜色索引
  uint8_t currentColorIndex = 0;

  // sprintf((char *)lcd_id, "LCD ID:%04X", lcddev.id);  /* 将LCD ID打印到lcd_id数组 */
  // lcd_show_string(10, 40, 240, 32, 32, "STM32", RED);
  // lcd_show_string(10, 80, 240, 24, 24, "TFTLCD TEST", RED);
  // lcd_show_string(10, 110, 240, 16, 16, "ATOM@ALIENTEK", RED);
  // lcd_show_string(10, 130, 240, 16, 16, (char *)lcd_id, RED); /* 显示LCD ID */

  /* Infinite loop */
  for(;;)
  {
      // 刷屏 - 使用表驱动法切换颜色
    // lcd_clear(colorTable[currentColorIndex]);
    
    // 选择对比色作为文字颜色(确保文字可见)
    uint16_t textColor = (colorTable[currentColorIndex] == WHITE || 
                           colorTable[currentColorIndex] == YELLOW ||
                           colorTable[currentColorIndex] == CYAN ||
                           colorTable[currentColorIndex] == LIGHTGREEN) ? BLACK : WHITE;
    
    // 显示当前颜色名称
    char colorInfo[30];
    sprintf(colorInfo, "Color: %s", colorNames[currentColorIndex]);
    // lcd_show_string(10, 10, 220, 24, 16, colorInfo, textColor);
   
    currentColorIndex = (currentColorIndex + 1) % colorTableSize;

    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
    HC595_ToggleIO(0); // Toggle the first IO
    HC595_ToggleIO(1); // Toggle the first IO
    HC595_ToggleIO(2); // Toggle the first IO
    HC595_ToggleIO(3); // Toggle the first IO
    HC595_ToggleIO(4); // Toggle the first IO
    HC165_ReadMultiBytes(ioState, 3); // Read the state of the 74HC165
    // Display the state on the LCD
    
    printf("IO State: %02X %02X %02X\n", ioState[0], ioState[1], ioState[2]); // Print the state
    lcd_show_string(10, 10, 220, 24, 16, "IO State:", BLACK);
    lcd_show_string(10, 40, 220, 24, 16, "IO0:", BLACK);
    lcd_show_num(80, 40, ioState[0], 2, 16, BLACK);
    lcd_show_string(10, 70, 220, 24, 16, "IO1:", BLACK);
    lcd_show_num(80, 70, ioState[1], 2, 16, BLACK);
    lcd_show_string(10, 100, 220, 24, 16, "IO2:", BLACK);
    lcd_show_num(80, 100, ioState[2], 2, 16, BLACK);
    osDelay(100);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

