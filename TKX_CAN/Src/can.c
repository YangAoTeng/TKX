/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    can.c
  * @brief   This file provides code for the configuration
  *          of the CAN instances.
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
#include "can.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

CAN_HandleTypeDef hcan1;

/* CAN1 init function */
void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 6;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_4TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */
  CAN_FilterTypeDef filter;
  filter.FilterIdHigh = 0;                       // 高16位标识符全为0
  filter.FilterIdLow = 0;                        // 低16位标识符全为0
  filter.FilterMaskIdHigh = 0;                   // 高16位掩码全为0
  filter.FilterMaskIdLow = 0;                    // 低16位掩码全为0
  filter.FilterFIFOAssignment = CAN_FILTER_FIFO0; // 使用FIFO0
  filter.FilterBank = 0;                         // 使用过滤器0
  filter.FilterMode = CAN_FILTERMODE_IDMASK;     // 标识符掩码模式
  filter.FilterScale = CAN_FILTERSCALE_32BIT;    // 32位过滤器
  filter.FilterActivation = ENABLE;              // 激活过滤器
  filter.SlaveStartFilterBank = 14;              // 从模式滤波器组的起始编号(对单CAN控制器不重要)
  
  if (HAL_CAN_ConfigFilter(&hcan1, &filter) != HAL_OK)
  {
    Error_Handler();
  }
  
  // 启动CAN通信
  if (HAL_CAN_Start(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  
  // 启用接收中断
  if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE END CAN1_Init 2 */

}

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */

  /* USER CODE END CAN1_MspInit 0 */
    /* CAN1 clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**CAN1 GPIO Configuration
    PB8     ------> CAN1_RX
    PB9     ------> CAN1_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* CAN1 interrupt Init */
    HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
  /* USER CODE BEGIN CAN1_MspInit 1 */

  /* USER CODE END CAN1_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();

    /**CAN1 GPIO Configuration
    PB8     ------> CAN1_RX
    PB9     ------> CAN1_TX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8|GPIO_PIN_9);

    /* CAN1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);
  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }
}


/* USER CODE BEGIN 1 */

/**
 * @brief  发送CAN消息
 * @param  id: 标准ID (11位) 或扩展ID (29位)
 * @param  data: 要发送的数据指针
 * @param  length: 数据长度 (0-8)
 * @param  isExtID: 0表示标准ID, 1表示扩展ID
 * @param  isRemote: 0表示数据帧, 1表示远程帧
 * @retval HAL_StatusTypeDef: HAL_OK表示发送成功
 */
HAL_StatusTypeDef CAN_Send_Message(uint32_t id, uint8_t *data, uint8_t length, uint8_t isExtID, uint8_t isRemote)
{
  CAN_TxHeaderTypeDef txHeader;
  uint32_t txMailbox;
  
  // 配置发送消息头
  if (isExtID == 0) {
    txHeader.StdId = id;
    txHeader.ExtId = 0;
    txHeader.IDE = CAN_ID_STD;
  } else {
    txHeader.StdId = 0;
    txHeader.ExtId = id;
    txHeader.IDE = CAN_ID_EXT;
  }

  if (isRemote == 0) {
    txHeader.RTR = CAN_RTR_DATA;
  } else {
    txHeader.RTR = CAN_RTR_REMOTE;
  }

  txHeader.DLC = (length > 8) ? 8 : length;

  // 设置发送优先级 (0最高 - 31最低)
  txHeader.TransmitGlobalTime = DISABLE;

  // 发送消息
  if (HAL_CAN_AddTxMessage(&hcan1, &txHeader, data, &txMailbox) != HAL_OK) {
    return HAL_ERROR;
  }

  return HAL_OK;
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  CAN_RxHeaderTypeDef rxHeader;
  uint8_t rxData[8];

  if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rxHeader, rxData) == HAL_OK)
  {
    printf("Received CAN message: ID=0x%X, DLC=%d, Data=", rxHeader.StdId, rxHeader.DLC);
    for (int i = 0; i < rxHeader.DLC; i++)
    {
      printf("0x%02X ", rxData[i]);
    }
  }
}



/* USER CODE END 1 */
