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

#include "w25flash.h"

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
void Flash_TestWrite(void);
void Flash_TestRead(void);
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
  uint16_t ID = Flash_ReadID(); // Read the ID of the flash memory
  printf("Flash ID: %04X\n", ID); // Print the ID


  printf("---------------------\r\n");
  printf("Erasing Block 0(256 pages)...\r\n");
  uint32_t globalAddr=0;
  Flash_EraseBlock64K(globalAddr);
  Flash_EraseBlock64K(1);
  printf("Block 0 is erased.\r\n");
  Flash_TestWrite(); // Test writing to flash memory
  Flash_TestRead(); // Test reading from flash memory
  /* Infinite loop */
  for(;;)
  {
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
    HC595_ToggleIO(0); // Toggle the first IO
    HC595_ToggleIO(1); // Toggle the first IO
    HC595_ToggleIO(2); // Toggle the first IO
    HC595_ToggleIO(3); // Toggle the first IO
    HC595_ToggleIO(4); // Toggle the first IO
    // HC165_ReadMultiBytes(ioState, 3); // Read the state of the 74HC165
    // // printf("IO State: %02X %02X %02X\n", ioState[0], ioState[1], ioState[2]); // Print the state
    osDelay(100);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */


void Flash_TestWrite(void)
{
	uint8_t blobkNo = 0;
	uint16_t sectorNo = 0;
	uint16_t pageNo = 0;
	uint32_t memAddress = 0;
	
	printf("---------------------\r\n");
	//写入Page0两个字符串
	memAddress = Flash_Addr_byBlockSectorPage(blobkNo, sectorNo, pageNo);		//Page0的地址
	uint8_t	bufStr1[] = "Hello from beginning";
	uint16_t len = 1 + strlen("Hello from beginning"); 											//包括结束符'\0'
	Flash_WriteInPage(memAddress, bufStr1, len);   													//在Page0的起始位置写入数据
	printf("Write in Page0:0\r\n%s\r\n", bufStr1);
 
	uint8_t	bufStr2[]="Hello in page";
	len = 1 + strlen("Hello in page"); 																			//包括结束符'\0'
	Flash_WriteInPage(memAddress+100, bufStr2, len);   											//Page0内偏移100
	printf("Write in Page0:100\r\n%s\r\n", bufStr2);
 
	//写入Page1中0-255数字
	uint8_t	bufPage[FLASH_PAGE_SIZE];																				//EN25Q_PAGE_SIZE=256
	for (uint16_t i=0;i<FLASH_PAGE_SIZE;i++)
		bufPage[i] = i;																												//准备数据
	pageNo = 1; 																														//Page 1
	memAddress = Flash_Addr_byBlockSectorPage(blobkNo, sectorNo, pageNo);		//page1的地址
	Flash_WriteInPage(memAddress, bufPage, FLASH_PAGE_SIZE);   							//写一个Page
	printf("Write 0-255 in Page1\r\n");
	printf("---------------------\r\n");
}



//测试读取Page0 和 Page1的内容
void Flash_TestRead(void)
{
	uint8_t blobkNo=0;
	uint16_t sectorNo=0;
	uint16_t pageNo=0;
	
	printf("---------------------\r\n");
	//读取Page0
	uint8_t bufStr[50];																											//Page0读出的数据
	uint32_t memAddress = Flash_Addr_byBlockSectorPage(blobkNo, sectorNo,pageNo);
	Flash_ReadBytes(memAddress, bufStr, 50);																//读取50个字符
	printf("Read from Page0:0\r\n%s\r\n",bufStr);
 
	Flash_ReadBytes(memAddress+100, bufStr, 50);														//地址偏移100后的50个字字节
	printf("Read from Page0:100\r\n%s\r\n",bufStr);
 
	//读取Page1
	uint8_t	randData = 0;
	pageNo = 1;
	memAddress = Flash_Addr_byBlockSectorPage(blobkNo, sectorNo,pageNo);
 
	randData = Flash_ReadOneByte(memAddress+12);														//读取1个字节数据，页内地址偏移12
	printf("Page1[12] = %d\r\n",randData);
 
	randData = Flash_ReadOneByte(memAddress+136);														//页内地址偏移136
	printf("Page1[136] = %d\r\n",randData);
 
	randData = Flash_ReadOneByte(memAddress+210);														//页内地址偏移210
	printf("Page1[210] = %d\r\n",randData);
	printf("---------------------\r\n");
}
/* USER CODE END Application */

