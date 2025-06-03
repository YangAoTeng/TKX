#ifndef __OLED_DRIVER_H
#define __OLED_DRIVER_H
#include "main.h"  
#include "gpio.h"  
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <stdarg.h>
#include "stdio.h"
#include "spi.h"


// PE0 RES
#define OLED_RES_Clr()  HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, GPIO_PIN_RESET)   // 复位 RES
#define OLED_RES_Set()  HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, GPIO_PIN_SET)     // 置位 RES

// PD12 DC
#define OLED_DC_Clr()   HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET)  // 复位 DC
#define OLED_DC_Set()   HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET)    // 置位 DC

// PB1 CS
#define OLED_CS_Clr()   HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_RESET)   // 复位 CS
#define OLED_CS_Set()   HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_SET)     // 置位 CS



#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据


//	oled初始化函数
void OLED_Init(void);
//	oled全局刷新函数
void OLED_Update(void);
//	oled局部刷新函数
void OLED_UpdateArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height);
// 设置颜色模式
void OLED_SetColorMode(bool colormode);
// OLED 设置亮度函数
void OLED_Brightness(int16_t Brightness);





#endif







