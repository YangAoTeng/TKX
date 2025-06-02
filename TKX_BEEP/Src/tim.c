/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.c
  * @brief   This file provides code for the configuration
  *          of the TIM instances.
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
#include "tim.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

TIM_HandleTypeDef htim2;

/* TIM2 init function */
void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 83;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 249;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 124;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */
  
  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspInit 0 */

  /* USER CODE END TIM2_MspInit 0 */
    /* TIM2 clock enable */
    __HAL_RCC_TIM2_CLK_ENABLE();
  /* USER CODE BEGIN TIM2_MspInit 1 */

  /* USER CODE END TIM2_MspInit 1 */
  }
}
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(timHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspPostInit 0 */

  /* USER CODE END TIM2_MspPostInit 0 */

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**TIM2 GPIO Configuration
    PA15     ------> TIM2_CH1
    */
    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN TIM2_MspPostInit 1 */

  /* USER CODE END TIM2_MspPostInit 1 */
  }

}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspDeInit 0 */

  /* USER CODE END TIM2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM2_CLK_DISABLE();
  /* USER CODE BEGIN TIM2_MspDeInit 1 */

  /* USER CODE END TIM2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

// 音阶频率定义（单位：Hz）
// 低音1-7
#define L1_DO  262
#define L2_RE  294
#define L3_MI  330
#define L4_FA  349
#define L5_SOL 392
#define L6_LA  440
#define L7_SI  494

// 中音1-7
#define M1_DO  523
#define M2_RE  587
#define M3_MI  659
#define M4_FA  698
#define M5_SOL 784
#define M6_LA  880
#define M7_SI  988

// 高音1-7
#define H1_DO  1046
#define H2_RE  1175
#define H3_MI  1318
#define H4_FA  1397
#define H5_SOL 1568
#define H6_LA  1760
#define H7_SI  1976

// 无声
#define REST 0

// 设置蜂鸣器频率
void Beep_Set_Freq(uint32_t freq)
{
    if (freq == 0) {
        // 静音
        HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
        return;
    }
    
    // 84MHz / (84 + 1) = 1MHz 的时钟
    uint32_t arr = 1000000 / freq;    // 计算自动重装载值
    uint32_t ccr = arr / 2;           // 设置占空比为50%
    
    __HAL_TIM_SET_AUTORELOAD(&htim2, arr - 1);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, ccr);
    
    // 确保PWM输出开启
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
}

// 播放音符序列
// notes: 音符频率数组
// durations: 每个音符持续时间(ms)
// count: 音符数量
void Beep_Play_Notes(const uint32_t *notes, const uint32_t *durations, uint32_t count)
{
    for (uint32_t i = 0; i < count; i++) {
        Beep_Set_Freq(notes[i]);
        // HAL_Delay(durations[i]);
        osDelay(durations[i]); // 确保有足够的时间来处理PWM
    }
    
    // 播放完毕后关闭PWM输出
    HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
}

// 简单的音乐演示函数
void Beep_Demo(void)
{
    // 低音范围的完整民歌片段集合
    const uint32_t notes[] = {
        // 《小毛驴》片段
        L5_SOL, L6_LA, L5_SOL, L3_MI, // 我有一只小毛驴
        L1_DO, L1_DO, L2_RE, L1_DO,   // 我从来也不骑
        L5_SOL, L6_LA, L5_SOL, L3_MI, // 有一天我心血来潮
        L2_RE, L2_RE, L1_DO, REST,    // 骑它去赶集
        
        L5_SOL, L6_LA, L5_SOL, L3_MI, // 我手里拿着小皮鞭
        L1_DO, L1_DO, L2_RE, L1_DO,   // 嘴里唱着歌曲
        L5_SOL, L6_LA, L5_SOL, L3_MI, // 不知不觉走了多远
        L2_RE, L2_RE, L1_DO, REST,    // 天已经黑了
        
        // 《两只老虎》片段
        L1_DO, L2_RE, L3_MI, L1_DO,   // 两只老虎，两只老虎
        L1_DO, L2_RE, L3_MI, L1_DO,   // 跑得快，跑得快
        L3_MI, L4_FA, L5_SOL, REST,   // 一只没有尾巴
        L3_MI, L4_FA, L5_SOL, REST,   // 一只没有耳朵
        L5_SOL, L6_LA, L5_SOL, L4_FA, L3_MI, L1_DO,  // 真奇怪，真奇怪
        L5_SOL, L6_LA, L5_SOL, L4_FA, L3_MI, L1_DO,  // 真奇怪，真奇怪
        
        REST, REST,  // 短暂停顿
        
        // 《小星星》片段
        L1_DO, L1_DO, L5_SOL, L5_SOL, L6_LA, L6_LA, L5_SOL,  // 一闪一闪亮晶晶
        L4_FA, L4_FA, L3_MI, L3_MI, L2_RE, L2_RE, L1_DO,     // 满天都是小星星
        L5_SOL, L5_SOL, L4_FA, L4_FA, L3_MI, L3_MI, L2_RE,   // 挂在天上放光明
        L5_SOL, L5_SOL, L4_FA, L4_FA, L3_MI, L3_MI, L2_RE,   // 好像许多小眼睛
        
        REST, REST,  // 短暂停顿
        
        // 《新年好》片段
        L5_SOL, L5_SOL, L5_SOL, L3_MI, // 新年好呀
        L5_SOL, L5_SOL, L5_SOL, L3_MI, // 新年好呀
        L5_SOL, L6_LA, L5_SOL, L3_MI, L2_RE, L1_DO, // 祝贺大家新年好
        L5_SOL, L5_SOL, L5_SOL, L3_MI, // 我们唱歌
        L5_SOL, L5_SOL, L5_SOL, L3_MI, // 又跳舞
        L5_SOL, L6_LA, L5_SOL, L3_MI, L2_RE, L1_DO, // 祝贺大家新年好
        
        REST, REST,  // 短暂停顿
        
        // 《茉莉花》片段
        L3_MI, L5_SOL, L6_LA, L6_LA, L5_SOL, // 好一朵美丽的茉莉花
        L3_MI, L2_RE, L3_MI, L1_DO,          // 好一朵美丽的茉莉花
        L6_LA, L5_SOL, L3_MI, L6_LA, L5_SOL, // 芬芳美丽满枝桠
        L3_MI, L2_RE, L1_DO, REST,           // 又香又白人人夸
        
        // 《送别》片段
        L3_MI, L3_MI, L5_SOL, L3_MI, L2_RE, L1_DO, // 长亭外，古道边
        L1_DO, L3_MI, L2_RE, L2_RE, REST,          // 芳草碧连天
        L3_MI, L3_MI, L5_SOL, L3_MI, L2_RE, L1_DO, // 晚风拂柳笛声残
        L1_DO, L2_RE, L1_DO, L1_DO, REST           // 夕阳山外山
    };
    
    const uint32_t durations[] = {
        // 《小毛驴》节奏
        300, 300, 300, 600,
        300, 300, 300, 600,
        300, 300, 300, 600,
        300, 300, 600, 300,
        
        300, 300, 300, 600,
        300, 300, 300, 600,
        300, 300, 300, 600,
        300, 300, 600, 300,
        
        // 《两只老虎》节奏
        300, 300, 300, 300,
        300, 300, 300, 300,
        300, 300, 600, 300,
        300, 300, 600, 300,
        300, 300, 300, 300, 300, 600,
        300, 300, 300, 300, 300, 600,
        
        500, 500,  // 短暂停顿
        
        // 《小星星》节奏
        300, 300, 300, 300, 300, 300, 600,
        300, 300, 300, 300, 300, 300, 600,
        300, 300, 300, 300, 300, 300, 600,
        300, 300, 300, 300, 300, 300, 600,
        
        500, 500,  // 短暂停顿
        
        // 《新年好》节奏
        400, 200, 400, 800,
        400, 200, 400, 800,
        300, 300, 300, 300, 300, 600,
        400, 200, 400, 800,
        400, 200, 400, 800,
        300, 300, 300, 300, 300, 600,
        
        500, 500,  // 短暂停顿
        
        // 《茉莉花》节奏
        400, 400, 400, 200, 600,
        400, 400, 400, 800,
        400, 400, 400, 200, 600,
        400, 400, 800, 400,
        
        // 《送别》节奏
        300, 300, 300, 300, 300, 600,
        300, 300, 300, 600, 300,
        300, 300, 300, 300, 300, 600,
        300, 300, 300, 600, 300
    };
    
    Beep_Play_Notes(notes, durations, sizeof(notes)/sizeof(notes[0]));
}

/* USER CODE END 1 */
