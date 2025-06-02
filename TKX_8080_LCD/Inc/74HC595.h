#ifndef __74HC595_H
#define __74HC595_H
#include "main.h"

// 定义74HC595引脚
#define HC595_SHCP_PORT     GPIOE
#define HC595_SHCP_PIN      GPIO_PIN_4
#define HC595_STCP_PORT     GPIOE
#define HC595_STCP_PIN      GPIO_PIN_3
#define HC595_DS_PORT       GPIOE
#define HC595_DS_PIN        GPIO_PIN_2

// 定义引脚操作宏
#define HC595_SHCP_HIGH()   HAL_GPIO_WritePin(HC595_SHCP_PORT, HC595_SHCP_PIN, GPIO_PIN_SET)
#define HC595_SHCP_LOW()    HAL_GPIO_WritePin(HC595_SHCP_PORT, HC595_SHCP_PIN, GPIO_PIN_RESET)
#define HC595_STCP_HIGH()   HAL_GPIO_WritePin(HC595_STCP_PORT, HC595_STCP_PIN, GPIO_PIN_SET)
#define HC595_STCP_LOW()    HAL_GPIO_WritePin(HC595_STCP_PORT, HC595_STCP_PIN, GPIO_PIN_RESET)
#define HC595_DS_HIGH()     HAL_GPIO_WritePin(HC595_DS_PORT, HC595_DS_PIN, GPIO_PIN_SET)
#define HC595_DS_LOW()      HAL_GPIO_WritePin(HC595_DS_PORT, HC595_DS_PIN, GPIO_PIN_RESET)

// 函数声明
void HC595_Init(void);
void HC595_WriteByte(uint8_t byte);
void HC595_WriteMultiBytes(uint8_t* bytes, uint8_t len);
void HC595_Latch(void);

// 新增IO控制函数
void HC595_SetIOHigh(uint8_t ioNum);
void HC595_SetIOLow(uint8_t ioNum);
void HC595_ToggleIO(uint8_t ioNum);
void HC595_UpdateAllIOs(void);
void HC595_ClearAllIOs(void);
void HC595_SetAllIOs(void);

#endif // __74HC595_H